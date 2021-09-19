#include "mentorclient.h"
#include "ui_mentorclient.h"
#include "../CourseUnitViewer/courseunitviewer.h"
#include "../SkillsFlower/skillsflower.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/StudentProgress/StudentProgress.h"


MentorClient::MentorClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MentorClient)
{
	qInfo() << "Init mentor client";

    ui->setupUi(this);
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->skillFlower, SLOT(unpack(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->skillsMixerHolder, SLOT(nodeSelected(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));

    connect(ui->courseUnitViewer, SIGNAL(progressMade(QString, double)), ui->skillFlower, SLOT(progressMade(QString, double)));
    connect(ui->courseUnitViewer, SIGNAL(progressMade(QString, double)), ui->skillsMixerHolder, SLOT(progressMade(QString, double)));

    connect(ui->skillFlower, SIGNAL(skillLevelChanged(QString, double)), ui->courseUnitViewer, SLOT(makeProgressToSelected(QString, double)));

    ui->skillFlower->setEditable(true);
    ui->courseUnitViewer->setEditable(false);

    connect(mSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
    connect(chooseserv, SIGNAL(onServConnectclicked()), SLOT(startConnection()));
    connect(chooseserv, SIGNAL(chooseServClosed()), SLOT(onChooseServClosed()));

    // TEST
    /*/------------------------------
    headCourseUnit = new CourseUnit;

    QFile f = QFile("C:/Users/timat/Desktop/dedCourse/sem1.CourseUnit");
    headCourseUnit->loadCourseUnit(&f);

    StudentProgress * tim = new StudentProgress();
    StudentProgress * andrew = new StudentProgress();

    students["tim"] = tim;
    students["andrew"] = andrew;

    tim->addProgress("Akinator", "Graph", 0.5);
    andrew->addProgress("Onegin", "IO", 1.5);

    display();
    */
    //------------------------------

    qInfo() << "Mentor client init finished";
}

MentorClient::~MentorClient()
{
    delete ui;
}



void MentorClient::onChooseServClosed(){
    this -> setEnabled(true);
    ui->statusbar->showMessage("Server isnt connected, Please connect to the server");
}



void MentorClient::onStart(){
    this -> setEnabled(false);
    chooseserv -> setEnabled(true);
    chooseserv -> show();
    this -> show();
    ui->statusbar->showMessage("Server isnt connected");
}

void MentorClient::startConnection(){
    QString IP = chooseserv -> getIP();
    unsigned Port = (chooseserv -> getPort()).toUInt();

    if(mSocket -> state() == QAbstractSocket::ConnectedState){
        mSocket -> close();
    }
    mSocket -> connectToHost(IP, Port);

}

void MentorClient::slotConnected(){

    qDebug() << "connected to serv";


    QDir dir = QDir();
    if (!inworkingrepository)
    {
        dir.mkdir(chooseserv -> getName() + chooseserv -> getIP());
        QDir::setCurrent(chooseserv -> getName() + chooseserv -> getIP());
        inworkingrepository = true;
        this -> sendToServer(static_cast<quint16>(getMentorName), "");
    }



}

void MentorClient::slotError(QAbstractSocket::SocketError error){
    QString strEr = "Error: " +
            (error == QAbstractSocket::HostNotFoundError ?
                 "The host was not found" :
                 error == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed" :
                     error == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused" :
                         QString(mSocket -> errorString()));

    QMessageBox::critical(this, "Failing", strEr);
    mSocket -> close();
}

void MentorClient::slotReadyRead(){
    QDataStream in(mSocket);

    for(;;){
        if(!nextBlockSize){
            if(mSocket -> bytesAvailable() < sizeof(quint32))
                break;
            in >> nextBlockSize;
        }

        if(mSocket -> bytesAvailable() < nextBlockSize)
            break;

        qDebug() << "nextblock size " << nextBlockSize;
        qDebug() << "bytes available " << mSocket -> bytesAvailable();
        datafromServer = mSocket -> read(nextBlockSize);
        qDebug() << "receiving " << datafromServer;
        endReception();
        nextBlockSize = 0;
    }
}



void MentorClient::handleincFile(QDataStream& in){

     QString filename;
     in >> filename;
     qDebug() << "handling file" << filename ;
     QFile file(filename);

     if (file.open(QIODevice::WriteOnly)){
         QTextStream out(&file);
         QByteArray filecont;
         in >> filecont;
         qDebug() << filecont;
         out << filecont;
         file.write(in.device()->readAll());
     }
     else
     {
         qDebug() << "Cant handle inc file " << filename ;
     }

}



void MentorClient::endReception(){

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
        qDebug() << "confirm connection";
        confirmConnection();
        break;
    case retrieveFail:
        QMessageBox::critical(this, "Failing", "Last operation wasnt complited");
        break;
    default:
        qDebug() << "cant understand the resp code";
        break;
    }

}


void MentorClient::confirmConnection(){
    if (respCode == retrieveFailAutorisation){
        QMessageBox::critical(this, "Failing", "Wrong Name");
        mSocket -> close();
        QDir::setCurrent("../");
        QDir dir = QDir();
        dir.rmdir(chooseserv -> getName() + chooseserv -> getIP());
        inworkingrepository = false;
        return;
    }

    chooseserv -> hide();
    this -> setEnabled(true);

    OpenCourse();
}


void MentorClient::LoadCourse(){
    QDir curdir = QDir();
    QStringList filters;
    filters << "*.mainCourseUnit";
    curdir.setNameFilters(filters);

    QFile fileMain( curdir.entryList()[0]);


    if (fileMain.open(QIODevice::ReadOnly)){

        QString filename(fileMain.readAll());

        QFile course(filename);
        try {
            headCourseUnit -> loadCourseUnit(&course);
        }
        catch (QString message){
            qDebug() << message;
        }
    } else {
        qDebug() << "cant open course main file";
    }

}


void MentorClient::LoadSkillpack(){
    QDir curdir = QDir();
    QFile pack(curdir.entryList(QStringList() << "*.cognitiaSkillPack")[0]);
    try {
        skillPack -> load (&pack);
    }
    catch(QString message){
        qDebug() << message;
    }
}


void MentorClient::LoadStudentsProgresses(){
    QDir curdir = QDir();

    QStringList studentsProg = curdir.entryList(QStringList() << "*.StudentProgress");
    for (int i = 0; i < studentsProg.size(); ++i){
        QFile prog(studentsProg[i]);
        StudentProgress* progress = new StudentProgress(this);

        try {
            progress -> load(&prog);;
        }
        catch(QString message){
            qDebug() << message;
        }

        students[studentsProg[i]] = progress;
    }

}


void MentorClient::OpenCourse(){

    LoadCourse();
    LoadSkillpack();
    LoadStudentsProgresses();

    display();

}


void MentorClient::sendToServer(quint16 code, const QString& str){

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint32(0) << chooseserv -> getName() << code << str;
    out.device()->seek(0);
    out << quint32(arrBlock.size() - sizeof(quint32));

    mSocket -> write(arrBlock);
    qDebug() << "sendind to server " << arrBlock;
}








void MentorClient::display() {
	ui->courseUnitViewer->clearAllScene();
	ui->courseUnitViewer->unpack(headCourseUnit);

	ui->studentChooser->clear();
	ui->studentChooser->addItems(students.keys());

	ui->childDescr->clear();
	ui->parentDescr->clear();

	ui->parentDescr->setMarkdown(headCourseUnit->getDescription());

	ui->childCu->clear();
	ui->parentCu->clear();

	ui->parentCu->setText(headCourseUnit->objectName());
}

void MentorClient::on_studentChooser_currentTextChanged(const QString &name) {
	if (students.contains(currentStudent)) {
		ui->courseUnitViewer->pack(students[currentStudent]);
	}
	ui->courseUnitViewer->clearStudentProgress();

	ui->courseUnitViewer->unpack(students[name]);

	currentStudent = name;

	ui->courseUnitViewer->setSelectedNode(ui->courseUnitViewer->getSelectedNode());
}

void MentorClient::pack() {
	if (students.contains(ui->studentChooser->currentText())) {
		ui->courseUnitViewer->pack(students[ui->studentChooser->currentText()]);
	}
}

void MentorClient::nodeSelected(Node *nd) {
	if (nd == nullptr) {
		ui->childDescr->clear();
		ui->childCu->clear();
	} else {
		ui->childDescr->setMarkdown(nd->getDescription());
		ui->childCu->setText(nd->getName());
	}
}

void MentorClient::on_actionChoose_Server_triggered()
{

    ui->statusbar->showMessage("Changing server");
    if (inworkingrepository){
        QDir::setCurrent("../");
        inworkingrepository = false;
    }
    this -> setEnabled(false);
    chooseserv -> setEnabled(true);
    chooseserv -> show();

}


void MentorClient::on_actionReturn_to_Launcher_triggered()
{
    emit onClose();
}


void MentorClient::on_actionSave_all_and_send_triggered()
{

}

