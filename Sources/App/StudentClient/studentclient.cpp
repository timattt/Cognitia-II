#include "studentclient.h"
#include "ui_studentclient.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "../ChooseServ/chooseserv.h"
#include "../Help/smarthelper.h"
#include "../Core/logger.h"

StudentClient::StudentClient() :
    QMainWindow(nullptr),
    ui(new Ui::StudentClient),
	mSocket(nullptr),
	nextBlockSize(0),
	skillpack(nullptr),
	courseUnit(nullptr),
	progress(nullptr),
	chooseserv(nullptr),
	StudentName(),
	datafromServer(),
    respCode(0),
    inworkingrepository(false)

{
    SAY("StudentClient init started");
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);
    chooseserv = new ChooseServ(this);
    skillpack = new SkillPack(this);
    courseUnit = new CourseUnit(this);
    progress = new StudentProgress(this);
    connect(mSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
    connect(chooseserv, SIGNAL(onServConnectclicked()), SLOT(startConnection()));
    connect(chooseserv, SIGNAL(chooseServClosed()), SLOT(onChooseServClosed()));

    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->inAbsolute, SLOT(unpack(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(progressMadeToSelected(QString, double)), ui->inAbsolute, SLOT(progressMade(QString, double)));
    connect(ui->inAbsolute, SIGNAL(skillLevelChanged(QString, double)), ui->courseUnitViewer, SLOT(makeProgressToSelected(QString, double)));

    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));

    ui->courseUnitViewer->setEditable(false);


    SAY("StudentClient init finished");
}

StudentClient::~StudentClient()
{
    delete ui;
}



void StudentClient::ReplaceAll(){
    delete courseUnit;
    delete skillpack;
    courseUnit = new CourseUnit(this);
    skillpack = new SkillPack(this);
    delete progress;
    progress = new StudentProgress(this);
}



void StudentClient::on_actionChange_Server_triggered()
{
    ui->statusbar->showMessage("Changing server");

    ClearAll();
    ReplaceAll();




    if(mSocket -> state() == QAbstractSocket::ConnectedState){
        mSocket -> close();
    }

    if (inworkingrepository){
        QDir::setCurrent("../");
        inworkingrepository = false;
    }
    this -> setEnabled(false);
    chooseserv -> setEnabled(true);
    chooseserv -> show();
}

void StudentClient::onChooseServClosed(){
    this -> setEnabled(true);
     chooseserv -> setButtonEnabled();
    ui->statusbar->showMessage("Server isnt connected, Please connect to the server");
}

void StudentClient::on_actionExpand_triggered() {
	ui->allSkills->setVisible(!ui->allSkills->isVisible());
}

void StudentClient::onStart(){
    this -> setEnabled(false);
    chooseserv -> setEnabled(true);
    chooseserv -> show();
    this -> show();
    ui->statusbar->showMessage("Server isnt connected");
}

void StudentClient::startConnection(){
    QString IP = chooseserv -> getIP();
    unsigned Port = (chooseserv -> getPort()).toUInt();

    if(mSocket -> state() == QAbstractSocket::ConnectedState){
        mSocket -> close();
    }
    mSocket -> connectToHost(IP, Port);

}

void StudentClient::slotConnected(){

    SAY("connected to server\n")
    StudentName = chooseserv -> getName();
     chooseserv -> setButtonEnabled();
    QDir dir = QDir();
    if (!inworkingrepository)
    {
        SAY("Send name to server\n")
        dir.mkdir(StudentName + chooseserv -> getIP());
        QDir::setCurrent(StudentName + chooseserv -> getIP());
        inworkingrepository = true;
        this -> sendToServer(static_cast<quint16>(getUserName), "");
    }



}

void StudentClient::slotError(QAbstractSocket::SocketError error){
    QString strEr = "Error: " +
            (error == QAbstractSocket::HostNotFoundError ?
                 "The host was not found" :
                 error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed" :
                     error == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused" :
                         QString(mSocket -> errorString()));

    QMessageBox::critical(this, "Failing", strEr);
     chooseserv -> setButtonEnabled();
    mSocket -> close();
}

void StudentClient::slotReadyRead(){
    QDataStream in(mSocket);

    for(;;){
        if(!nextBlockSize){
            if(mSocket -> bytesAvailable() < (qint64) sizeof(quint32))
                break;
            in >> nextBlockSize;
        }

        if(mSocket -> bytesAvailable() < nextBlockSize)
            break;

        datafromServer = mSocket -> read(nextBlockSize);

        endReception();
        nextBlockSize = 0;
    }
}



void StudentClient::handleincFile(QDataStream& in){


     QString filename;
     in >> filename;
     SAY("handling file " + filename)
     QFile file(filename);

     if (file.open(QIODevice::WriteOnly)){
         QTextStream out(&file);
         QByteArray filecont;
         in >> filecont;
         out << filecont;
         //file.write(in.device()->readAll());
     }
     else
     {
         SAY("Cant handle inc file " + filename);
     }

}



void StudentClient::endReception(){

    QDataStream in(datafromServer);
    in >> respCode;

    switch (respCode) {
    case retrieveCourseUnit:
    case retrieveSkillpack:
    case retrieveStudentProgress:
        handleincFile(in);
        break;
    case retrieveFailAutorisation:
    case firstConnectionSuccess:
        SAY("confirm connection");
        confirmConnection();
        break;
    case retrieveFail:
        QMessageBox::critical(this, "Failing", "Last operation wasnt complited\n");
        break;
    default:
        SAY("cant understand the code\n");
        break;
    }

}


void StudentClient::confirmConnection(){
    if (respCode == retrieveFailAutorisation){
        QMessageBox::critical(this, "Failing", "Wrong Name");
        mSocket -> close();
        QDir::setCurrent("../");
        QDir dir = QDir();
        dir.rmdir(StudentName + chooseserv -> getIP());
        inworkingrepository = false;
        return;
    }

    chooseserv -> hide();
    this -> setEnabled(true);

    ui -> statusbar -> showMessage("Student connected: " + chooseserv -> getName());
    OpenCourse();
}

void StudentClient::nodeSelected(Node *nd) {
	if (nd == nullptr) {
		ui->cuDescription->clear();
		ui->cuName->clear();

		ui->inAbsolute->clearAll();

		if (courseUnit != nullptr) {
			ui->cuName->setText(this->courseUnit->objectName());
			ui->cuDescription->setMarkdown(this->courseUnit->getDescription());
		}
	} else {
		ui->cuDescription->setMarkdown(nd->getDescription());
		ui->cuName->setText(nd->getName());
	}
}

void StudentClient::LoadCourse(){
    QDir curdir = QDir();
    QStringList filters;
    filters << QString("*") + MAIN_COURSEUNIT_FILE_EXTENSION;
    curdir.setNameFilters(filters);

    QStringList list = curdir.entryList();
    ASSERT(list.size())

    QFile fileMain(list[0]);

    SAY("lOAD COURSE")

    if (fileMain.open(QIODevice::ReadOnly)){

        QString filename(fileMain.readAll());

        QFile course(filename);
        try {
            courseUnit -> loadCourseUnit(&course);
        }
        catch (QString & message){
           SAY(message);
        }
    } else {
        SAY("cant open course main file");
    }

}


void StudentClient::LoadSkillpack(){
    QDir curdir = QDir();
    QStringList list = curdir.entryList(QStringList() << QString("*") + QString(SKILL_PACK_FILE_EXTENSION));
    ASSERT(list.size())
    QFile pack(list[0]);
    SAY("LOAD SKILLPACK")
    try {
        skillpack -> load(&pack);
    }
    catch(QString & message){
        SAY(message);
    }
}


void StudentClient::LoadStudentsProgresses(){
    QDir curdir = QDir();

    QStringList list = curdir.entryList(QStringList() << QString("*") + QString(STUDENT_PROGRESS_FILE_EXTENSION));
    ASSERT(list.size())
    QFile prog(list[0]);

    SAY("LOAD  PROGRESS ")
    try {
        progress -> load(&prog);
    }
    catch(QString & message){
        SAY(message);
    }
}


void StudentClient::OpenCourse(){

    LoadCourse();
    LoadSkillpack();
    LoadStudentsProgresses();

    display();

}

void StudentClient::ClearAll(){
    ui->courseUnitViewer->clearAllScene();

    ui->inAbsolute->clearAll();

    ui->cuName->clear();
    ui->cuDescription->clear();
}


void StudentClient::display(){
    ClearAll();

	ui->cuDescription->setMarkdown(courseUnit->getDescription());
	ui->courseUnitViewer->unpack(courseUnit);
	ui->cuName->setText(courseUnit->objectName());

	ui->courseUnitViewer->unpack(progress);
}

void StudentClient::sendToServer(quint16 code, const QString& str){

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint32(0) << StudentName << code << str;
    out.device()->seek(0);
    out << quint32(arrBlock.size() - sizeof(quint32));

    mSocket -> write(arrBlock);
}


void StudentClient::on_actionSave_all_and_send_triggered()
{
     if(!inworkingrepository) {
         ui->statusbar->showMessage("Please, connect to server");
         return;
     }

     ui->statusbar->showMessage("Save");
}




void StudentClient::on_actionReturn_to_Launcher_triggered()
{
    ClearAll();

    ReplaceAll();

    if(mSocket -> state() == QAbstractSocket::ConnectedState){
        mSocket -> close();
    }

    if (inworkingrepository){
        QDir::setCurrent("../");
        inworkingrepository = false;
    }

    emit onClose();
}


void StudentClient::on_actionHelp_me_triggered()
{
    SmartHelper helper(":/help/Help/StudentClientHelp.txt", this);
    helper.exec();
}

