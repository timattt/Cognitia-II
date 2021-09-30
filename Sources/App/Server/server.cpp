#include "server.h"
#include "ui_server.h"
#include "Structures/StudentProgress/StudentProgress.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
	mtcpServ(nullptr),
    nPort(1917),
    nextblocksize(0),
	Users(),
	Mentors()
{
	qInfo() << "Server init started";

    ui->setupUi(this);
    mtcpServ = new QTcpServer(this);

    ui -> Log -> setReadOnly(true);
    ui -> ActiveUsers -> setReadOnly(true);
    ui -> ActiveMentors -> setReadOnly(true);

    QDir().mkdir("Server");

    qInfo() << "Server init finished";
}

Server::~Server()
{
    delete mtcpServ;
    delete ui;
}

void Server::on_StopServ_clicked()
{
    if(mtcpServ -> isListening()){
        mtcpServ -> close();
        ui->Log->append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + "Server closed!\n");
    }
    else{
        QMessageBox::critical(0, "Server Error", "Start server first");
    }
}


void Server::on_StartServ_clicked()
{

    if (mtcpServ -> isListening())
    {
        ui->Log->append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + "Server has already been started!\n");
        return;
    }
    connect(mtcpServ, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    if( !mtcpServ->listen(QHostAddress::Any, nPort) ){
        QMessageBox::critical(0, "Server Error", "Unable to start server:" + mtcpServ->errorString());
        mtcpServ -> close();
        return;
    }
    ui->Log->append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + "Server started!\n");

}

void Server::slotNewConnection(){
    QTcpSocket* ClientSocket = mtcpServ -> nextPendingConnection();
    connect(ClientSocket, SIGNAL(disconnected()), ClientSocket, SLOT(deleteLater()));
    connect(ClientSocket, SIGNAL(disconnected()), SLOT(deleteFromLog()));
    connect(ClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    qDebug() << "new connection " << ClientSocket;
}

void Server::slotReadClient(){

    QTcpSocket* ClientSocket = static_cast<QTcpSocket*>(sender());
    QDataStream in(ClientSocket);

    for(;;){
        if(!nextblocksize){
            if(ClientSocket -> bytesAvailable() < (qint64) sizeof(quint32))
                break;
            in >> nextblocksize;
        }

        if(ClientSocket -> bytesAvailable() < nextblocksize)
            break;

        QByteArray str = ClientSocket -> read(nextblocksize);
        qDebug() << "accept sending " << str;
        handleReq(ClientSocket, str);

        nextblocksize = 0;

    }
}


QTcpSocket* Server::Find_Dead(const QMap<QTcpSocket*, QString>& clients){


    for(QTcpSocket* client : clients.keys())
    {
        if(client -> state() != QAbstractSocket::ConnectedState){
            return client;
        }
    }
    return nullptr;
}



void Server::deleteFromLog(){
    QTcpSocket* client;
    if ((client = Find_Dead(Users))){

        QString name = Users[client];
        Users.remove(client);
        QString log = ui -> ActiveUsers -> toPlainText();
        log.replace(name, "");
        ui -> ActiveUsers -> setText(log);

        ui -> Log -> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + name + QString(" disconnected\n"));
    }
    else if ((client = Find_Dead(Mentors))){
        QString name = Mentors[client];
        Mentors.remove(client);
        QString log = ui -> ActiveMentors -> toPlainText();
        log.replace(name, "");
        ui -> ActiveMentors -> setText(log);

        ui -> Log -> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + name + QString(" (mentor) disconnected\n"));
    }
}

bool Server::handleincStudentProgressFile(QDataStream& in){

     QString filename;
     in >> filename;
     ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("saving file ") + filename + QString("\n"));

     QString dirname = filename.section(".", 0, 0);

     QFile file(dirname + QString("/") + filename);

     if (file.open(QIODevice::WriteOnly)){
         QTextStream out(&file);
         QByteArray filecont;
         in >> filecont;
         qDebug() << filecont;
         out << filecont;
         //file.write(in.device()->readAll());
         file.close();
     }
     else
     {
         ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Cant handle inc file ") + filename + QString("\n")) ;
         return false;
     }
     return true;

}

void Server::handleReq(QTcpSocket* client, const QByteArray &data){

    QDataStream in(data);

    QString name;
    in >> name;
    quint16 incCode;
    in >> incCode;
    qDebug() << "accept name" << name;
    if (!CheckClient(name, incCode, client)){
        sendToClient(client, static_cast<quint16>(retrieveFailAutorisation), "");
        return;
    }


    QDir dir = QDir();
    QStringList students = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    switch (incCode) {
    case getUserName:
        Users[client] = name;
        ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + name + QString(" connected!"));

        if(!ui -> ActiveUsers -> toPlainText().contains(name))
            ui->ActiveUsers->append(name);

        if (!SendCoursetoClient(client, name) ||
            !SendSkillpacktoClient(client, name) ||
            !SendStudentProgresstoClient(client, name))
        {
            sendToClient(client, static_cast<quint16>(retrieveFail), "");
            ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Fail! to send CU + SkP to ") + name);
            break;
        }

        sendToClient(client, static_cast<quint16>(firstConnectionSuccess), "");

        break;

    case getMentorName:
        Mentors[client] = name;
        ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + name + QString(" (mentor) connected!"));

        if(!ui -> ActiveMentors -> toPlainText().contains(name))
            ui->ActiveMentors->append(name);

        if (!SendCoursetoClient(client, name) ||
            !SendSkillpacktoClient(client, name))
        {
            sendToClient(client, static_cast<quint16>(retrieveFail), "");
            ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Fail! to send CU + SkP to ") + name);
            break;
        }

        for (int i = 0; i < students.size(); ++i){
            if(!SendStudentProgresstoClient(client, students[i])){
                sendToClient(client, static_cast<quint16>(retrieveFail), "");
                ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Fail! to send SP ") + students[i]
                                 + QString(" to ") + name);
                return;
            }
        }

        sendToClient(client, static_cast<quint16>(firstConnectionSuccess), "");

        break;

    case saveSkillpack:

        break;

    case saveStudentProgress:
        Mentors[client] = name;
        ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + name + QString(" (mentor) has sent SP"));

        if (!handleincStudentProgressFile(in)){
            sendToClient(client, static_cast<quint16>(retrieveSavingFail), "");
        }

        break;
    case sendCourse:

        break;

    case sendSkillpack:

        break;

    case sendStudentProgress:

        break;

    case saveCourse:

        break;


    default:
        sendToClient(client, static_cast<quint16>(retrieveFail), "");
        break;
    }

}


bool Server::SendFile(const QString& filename, QTcpSocket *client, quint16 code){

    QFile file(filename);
    qDebug() << filename;
    if (file.open(QIODevice::ReadOnly))
       {
            QByteArray arrBlock;
            QDataStream out(&arrBlock, QIODevice::WriteOnly);
            qDebug() << quint32(0) << code << filename.section("/", -1) << file.readAll();
            file.seek(0);
            out << quint32(0) << code << filename.section("/", -1) << file.readAll();
            out.device()->seek(0);
            out << quint32(arrBlock.size() - sizeof(quint32));
            qDebug() << arrBlock;
            client -> write(arrBlock);

            file.close();
       }
    else
        return false;
    return true;
}


bool Server::SendCoursetoClient(QTcpSocket *client, const QString &name){

     ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Sendind course to ") + name);

     QStringList filters;
     QDir curdir = QDir();
     filters << QString("*") + MAIN_COURSEUNIT_FILE_EXTENSION << QString("*") + QString(COURSE_UNIT_FILE_EXTENSION);
     curdir.setNameFilters(filters);
     QStringList courseFiles = curdir.entryList();

     for (int i = 0; i < courseFiles.size(); ++i){

         if(!SendFile(courseFiles[i], client, static_cast<quint16>(retrieveCourseUnit)))
             return false;

     }

     return true;
}



bool Server::SendSkillpacktoClient(QTcpSocket *client, const QString &name){

     ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Sendind skillpack to ") + name);
     QDataStream out(client);
     QDir curdir = QDir();

     QStringList filters;
     filters << QString("*") + QString(SKILL_FILE_EXTENSION) << QString("*") + QString(SKILL_PACK_FILE_EXTENSION);
     curdir.setNameFilters(filters);

     QStringList courseFiles = curdir.entryList();

     for (int i = 0; i < courseFiles.size(); ++i){

         if(!SendFile(courseFiles[i], client, static_cast<quint16>(retrieveSkillpack)))
             return false;

     }

     return true;
}




bool Server::SendStudentProgresstoClient(QTcpSocket *client, const QString &name){

     ui->Log-> append("[" + QTime::currentTime().toString("hh:mm:ss.zzz") + "] " + QString("Sendind Student Progress of ") + name);
     QDataStream out(client);
     QDir curdir = QDir(name);

     QStringList courseFiles = curdir.entryList(QStringList() << QString("*") + QString(STUDENT_PROGRESS_FILE_EXTENSION));

     for (int i = 0; i < courseFiles.size(); ++i){

         if(!SendFile(curdir.dirName() + QString("/") + courseFiles[i], client, retrieveStudentProgress))
             return false;
     }
    return true;
}




bool Server::CheckClient(const QString & name, quint16 code, QTcpSocket* client){
    //qDebug() << QDir::currentPath();
    if (code == getMentorName || Mentors.contains(client))
        return true;
    return  QDir(name).exists();
}



void Server::sendToClient(QTcpSocket* Socket, quint16 code,  const QString& str) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint32(0) << code << str;
    out.device()->seek(0);
    out << quint32(arrBlock.size() - sizeof(quint32));

    Socket -> write(arrBlock);

}

void Server::on_returnToL_clicked()
{
    QDir::setCurrent("../");
    emit onClose();
}


void Server::onStart(){
    QDir::setCurrent("Server");
    this -> show();
}


void Server::on_addStudent_clicked()
{
    QString name = QFileDialog::getSaveFileName(this, "Create student Directory");
    if (name.length() == 0){
        return;
    }

    QDir dir = QDir();
    if(!dir.mkdir(name)){
        qDebug() << "cannot make new dir " << name ;
        return;
    }
    StudentProgress student = StudentProgress();

    name = name.section("/", -1);

    QFile file(name + QString("/") + name + QString(STUDENT_PROGRESS_FILE_EXTENSION));
    if (!file.exists())
        try {
            student.save(&file);
        }
        catch(QString message){
            qDebug() << message;
        }

}


void Server::on_chooseParent_clicked()
{
    QString name = QFileDialog::getOpenFileName(this, "Choose parent course",  QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));
    if (name.length() == 0){
        return;
    }

    QFile file(QString("crusial") + MAIN_COURSEUNIT_FILE_EXTENSION);

    if (file.open(QIODevice::WriteOnly))
       {
            file.write(name.toUtf8());
            file.close();
       }
    else
        QMessageBox::critical(this, "While picking course.." , "Can't open crusial file");

}

