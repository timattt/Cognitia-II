#include "mentorclient.h"
#include "ui_mentorclient.h"
#include "../CourseUnitViewer/courseunitviewer.h"
#include "../SkillsFlower/skillsflower.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "../Core/logger.h"
#include "../Help/smarthelper.h"


MentorClient::MentorClient() :
    QMainWindow(nullptr),
    ui(new Ui::MentorClient),
	headCourseUnit(nullptr),
	skillPack(nullptr),
	mSocket(nullptr),
	chooseserv(nullptr),
	respCode(0)
{
    SAY("Init mentor client")

    ui->setupUi(this);

    mSocket = new QTcpSocket(this);
    chooseserv = new ChooseServ(this);
    skillPack = new SkillPack(this);
    headCourseUnit = new CourseUnit(this);

    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->skillFlower, SLOT(unpack(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->skillsMixerHolder, SLOT(nodeSelected(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));

    connect(ui->courseUnitViewer, SIGNAL(progressMadeToSelected(QString, double)), ui->skillFlower, SLOT(progressMade(QString, double)));
    connect(ui->courseUnitViewer, SIGNAL(progressMadeToSelected(QString, double)), ui->skillsMixerHolder, SLOT(progressMade(QString, double)));
    connect(ui->courseUnitViewer, SIGNAL(progressMadeToSelected(QString, double)), this, SLOT(progressMade(QString, double)));

    connect(ui->skillFlower, SIGNAL(skillLevelChanged(QString, double)), ui->courseUnitViewer, SLOT(makeProgressToSelected(QString, double)));
    connect(ui->skillsMixerHolder, SIGNAL(skillLevelChanged(QString, double)), ui->courseUnitViewer, SLOT(makeProgressToSelected(QString, double)));

    ui->skillFlower->setEditable(true);
    ui->courseUnitViewer->setEditable(false);

    connect(mSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
    connect(chooseserv, SIGNAL(onServConnectclicked()), SLOT(startConnection()));
    connect(chooseserv, SIGNAL(chooseServClosed()), SLOT(onChooseServClosed()));



    SAY("Mentor client init finished")
}

MentorClient::~MentorClient()
{
    delete ui;
}



void MentorClient::onChooseServClosed(){
    this -> setEnabled(true);
    chooseserv -> setButtonEnabled();
    ui->statusbar->showMessage("Server isnt connected, Please connect to the server");
}

void MentorClient::progressMade(QString skill, double val) {
	if (students.contains(ui->studentChooser->currentText())) {
		students[ui->studentChooser->currentText()]->addProgress(ui->courseUnitViewer->getSelectedNode()->getName(), skill, val);
	}
}

void MentorClient::onStart(){
    this -> setEnabled(false);
    chooseserv -> setEnabled(true);
    chooseserv -> show();
    this -> showMaximized();
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

    SAY("Connected to serv")
    chooseserv -> setButtonEnabled();

    QDir dir = QDir();
    if (!inworkingrepository)
    {
        SAY("sending name")
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
    chooseserv -> setButtonEnabled();
    mSocket -> close();
}

void MentorClient::slotReadyRead(){
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



void MentorClient::handleincFile(QDataStream& in){

     QString filename;
     in >> filename;
     SAY("handling file" + filename);
     //filename = filename.section("/", -1);
     QFile file(filename);

     if (file.open(QIODevice::WriteOnly)){
         QTextStream out(&file);
         QByteArray filecont;
         in >> filecont;

         out << filecont;
         file.close();
         //file.write(in.device()->readAll());
     }
     else
     {
         SAY("Cant handle inc file " + filename);
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
        SAY("confirm connection");
        confirmConnection();
        break;

    case retrieveSavingFail:
        QMessageBox::critical(this, "Failing", "Server has a problem with saving your files");
        ui -> statusbar -> showMessage("Try one more time to send all to server");
        break;
    case retrieveFail:
        QMessageBox::critical(this, "Failing", "Last operation wasnt complited");
        break;
    default:
        SAY("cant understand the resp code")
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
    ui -> statusbar -> showMessage("");
    OpenCourse();
}


void MentorClient::LoadCourse(){
    QDir curdir = QDir();
    QStringList filters;
    filters << QString("*") + MAIN_COURSEUNIT_FILE_EXTENSION;
    curdir.setNameFilters(filters);

    QFile fileMain( curdir.entryList()[0]);


    if (fileMain.open(QIODevice::ReadOnly)){

        QString filename(fileMain.readAll());
        fileMain.close();

        QFile course(filename);
        try {
            headCourseUnit -> loadCourseUnit(&course);
        }
        catch (QString message){
            SAY(message)
        }


    } else {
        SAY("cant open course main file")
    }

}


void MentorClient::LoadSkillpack(){
    QDir curdir = QDir();
    QFile pack(curdir.entryList(QStringList() << QString("*") + QString(SKILL_PACK_FILE_EXTENSION))[0]);
    try {
        skillPack -> load (&pack);
    }
    catch(QString & message){
        SAY(message)
    }
}


void MentorClient::LoadStudentsProgresses(){
    QDir curdir = QDir();

    QStringList studentsProg = curdir.entryList(QStringList() << QString("*") + QString(STUDENT_PROGRESS_FILE_EXTENSION));
    for (int i = 0; i < studentsProg.size(); ++i){
        QFile prog(studentsProg[i]);
        StudentProgress* progress = new StudentProgress(this);

        try {
            progress -> load(&prog);;
        }
        catch(QString & message){
            SAY(message)
        }
        students[studentsProg[i].section(".", 0, 0)] = progress;
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

}


void MentorClient::ClearAll(){
    ui->courseUnitViewer->clearAllScene();
    ui->studentChooser->clear();
    ui->childDescr->clear();
    ui->parentDescr->clear();
    ui->childCu->clear();
    ui->parentCu->clear();
}


void MentorClient::ReplaceAll(){

    delete headCourseUnit;
    delete skillPack;
    headCourseUnit = new CourseUnit(this);
    skillPack = new SkillPack(this);
    for (StudentProgress* student : students.values()){
        delete student;
    }
    students.clear();

}


void MentorClient::display() {
    ClearAll();

	ui->courseUnitViewer->unpack(headCourseUnit);

	ui->studentChooser->addItems(students.keys());

	ui->parentDescr->setMarkdown(headCourseUnit->getDescription());

	ui->parentCu->setText(headCourseUnit->objectName());
}

void MentorClient::on_studentChooser_currentTextChanged(const QString &name) {


	if (students.contains(currentStudent)) {
		ui->courseUnitViewer->pack(students[currentStudent]);
	}

	ui->courseUnitViewer->clearStudentProgress();

    if(name.size()){
        ui->courseUnitViewer->unpack(students[name]);
    }

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

		if (students.contains(ui->studentChooser->currentText())) {
			ui->skillFlower->setEditable(false);
			ui->skillFlower->unpackEmbed(headCourseUnit, students[ui->studentChooser->currentText()]);
		}
	} else {
		ui->skillFlower->setEditable(true);
		ui->childDescr->setMarkdown(nd->getDescription());
		ui->childCu->setText(nd->getName());
	}
}





void MentorClient::on_actionChoose_Server_triggered()
{

    ClearAll();

    ReplaceAll();

    if(mSocket -> state() == QAbstractSocket::ConnectedState){
        mSocket -> close();
    }

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



bool MentorClient::SendFile(QFile* file, quint16 code){

    ASSERT(file)

    if (file -> open(QIODevice::ReadOnly))
       {
            QByteArray arrBlock;
            QDataStream out(&arrBlock, QIODevice::WriteOnly);


            out << quint32(0) << chooseserv -> getName() << code << file -> fileName() << file -> readAll();
            out.device() -> seek(0);
            out << quint32(arrBlock.size() - sizeof(quint32));

            mSocket -> write(arrBlock);

       }
    else {
        return false;
    }
    file->close();
    return true;
}


void MentorClient::sendAll(){

    for(QString& studentname: students.keys())
    {
        QFile file = QFile(studentname + QString(STUDENT_PROGRESS_FILE_EXTENSION));

        if(!SendFile(&file, static_cast<quint16>(saveStudentProgress))){
            QMessageBox::critical(0, "Failing to send Student progresses", "Please try one more time");
            ui -> statusbar -> showMessage("Failing to send Student progresses");
            return;
        }
    }

    ui -> statusbar -> showMessage("All files are sent!");
}


void MentorClient::on_actionSave_all_and_send_triggered()
{
      ui -> statusbar -> showMessage("Saving student progresses..");
      pack();

      QDir dir = QDir();
      dir.mkdir("localsave");
      QDir::setCurrent("localsave");

      for(QString& studentname: students.keys())
      {
          StudentProgress* student = students[studentname];
          QFile file = QFile(studentname + QString(STUDENT_PROGRESS_FILE_EXTENSION));

          try {
              student -> save(&file);
          }
          catch (const QString mess){
              QMessageBox::critical(0, mess, "Please try one more time");
              ui -> statusbar -> showMessage("");
          }

      }

      ui -> statusbar -> showMessage("All files are saved! Sending..");
      sendAll();

      QDir::setCurrent("../");
}

void MentorClient::on_actionHelp_triggered()
{
    SmartHelper helper(":/help/Help/MentorClientHelp.txt", this);
    helper.exec();
}

