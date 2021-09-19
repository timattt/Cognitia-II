#include "server.h"
#include "ui_server.h"
#include "Structures/StudentProgress/StudentProgress.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
    nextblocksize(0)
{
	qInfo() << "Server init started";

    ui->setupUi(this);
    mtcpServ = new QTcpServer(this);

    ui -> Log -> setReadOnly(true);
    ui -> ActiveUsers -> setReadOnly(true);

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
        ui->Log->append("Server closed!\n");
    }
    else{
        QMessageBox::critical(0, "Server Error", "Start server first");
    }
}


void Server::on_StartServ_clicked()
{

    if (mtcpServ -> isListening())
    {
        ui->Log->append("Server has already been started!\n");
        return;
    }
    connect(mtcpServ, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    if( !mtcpServ->listen(QHostAddress::Any, nPort) ){
        QMessageBox::critical(0, "Server Error", "Unable to start server:" + mtcpServ->errorString());
        mtcpServ -> close();
        return;
    }
    ui->Log->append("Server started!\n");

}

void Server::slotNewConnection(){
    QTcpSocket* ClientSocket = mtcpServ -> nextPendingConnection();
    connect(ClientSocket, SIGNAL(disconnected()), ClientSocket, SLOT(deleteLater()));
    connect(ClientSocket, SIGNAL(disconnected()), SLOT(deleteFromLog()));
    connect(ClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    qDebug() << "new connection";
}

void Server::slotReadClient(){

    QTcpSocket* ClientSocket = static_cast<QTcpSocket*>(sender());
    QDataStream in(ClientSocket);

    for(;;){
        if(!nextblocksize){
            if(ClientSocket -> bytesAvailable() < sizeof(quint32))
                break;
            in >> nextblocksize;
        }

        if(ClientSocket -> bytesAvailable() < nextblocksize)
            break;

        QByteArray str = ClientSocket -> read(nextblocksize);
        qDebug() << "accept sending " << str;
        handleReq(ClientSocket, nextblocksize, str);

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
    if(client = Find_Dead(Users)){

        QString name = Users[client];
        Users.remove(client);
        QString log = ui -> ActiveUsers -> toPlainText();
        log.replace(name, "");
        ui -> ActiveUsers -> setText(log);

        ui -> Log -> append(name + QString(" disconnected\n"));
    }
    else if (client = Find_Dead(Mentors)){
        QString name = Mentors[client];
        Mentors.remove(client);
        QString log = ui -> ActiveMentors -> toPlainText();
        log.replace(name, "");
        ui -> ActiveMentors -> setText(log);

        ui -> Log -> append(name + QString(" (mentor) disconnected\n"));
    }
}



void Server::handleReq(QTcpSocket* client, quint32 block, const QByteArray &data){
    QDataStream in(data);

    QString name;
    in >> name;
    quint16 incCode;
    in >> incCode;
    qDebug() << "accept name" << name;
    if (!CheckClient(name, incCode)){
        sendToClient(client, static_cast<quint16>(retrieveFailAutorisation), "");
        return;
    }


    QDir dir = QDir();
    QStringList students = dir.entryList(QDir::Dirs);

    switch (incCode) {
    case getUserName:
        Users[client] = name;
        ui->Log-> append(name + QString(" connected!\n"));
        ui->ActiveUsers->append(name);
        if (!SendCoursetoClient(client, name) ||
            !SendSkillpacktoClient(client, name) ||
            !SendStudentProgresstoClient(client, name))
        {
            sendToClient(client, static_cast<quint16>(retrieveFail), "");
            ui->Log-> append(QString("Fail! to send CU + SkP to ") + name);
            break;
        }

        sendToClient(client, static_cast<quint16>(firstConnectionSuccess), "");

        break;

    case getMentorName:
        Mentors[client] = name;
        ui->Log-> append(name + QString(" (mentor) connected!\n"));
        ui->ActiveMentors->append(name);
        if (!SendCoursetoClient(client, name) ||
            !SendSkillpacktoClient(client, name))
        {
            sendToClient(client, static_cast<quint16>(retrieveFail), "");
            ui->Log-> append(QString("Fail! to send CU + SkP to ") + name);
            break;
        }

        for (int i = 0; i < students.size(); ++i){
            if(!SendStudentProgresstoClient(client, students[i])){
                sendToClient(client, static_cast<quint16>(retrieveFail), "");
                ui->Log-> append(QString("Fail! to send SP ") + students[i]
                                 + QString(" to ") + name);
                return;
            }
        }

        sendToClient(client, static_cast<quint16>(firstConnectionSuccess), "");

        break;

    case saveSkillpack:

        break;

    case saveStudentProgress:

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
           // filename = filename.section("/", -1);
            qDebug() << quint32(0) << code << filename << file.readAll();
            file.seek(0);
            out << quint32(0) << code << filename << file.readAll();
            out.device()->seek(0);
            out << quint32(arrBlock.size() - sizeof(quint32));
            qDebug() << arrBlock;
            client -> write(arrBlock);


       }
    else
        return false;
    return true;
}


bool Server::SendCoursetoClient(QTcpSocket *client, const QString &name){

     ui->Log-> append(QString("Sendind course to ") + name);

     QStringList filters;
     QDir curdir = QDir();
     filters << "*.mainCourseUnit" << "*.CourseUnit";
     curdir.setNameFilters(filters);
     QStringList courseFiles = curdir.entryList();

     for (int i = 0; i < courseFiles.size(); ++i){

         if(!SendFile(courseFiles[i], client, static_cast<quint16>(retrieveCourseUnit)))
             return false;

     }

     return true;
}



bool Server::SendSkillpacktoClient(QTcpSocket *client, const QString &name){

     ui->Log-> append(QString("Sendind skillpack to ") + name);
     QDataStream out(client);
     QDir curdir = QDir();

     QStringList filters;
     filters << "*.cognitiaSkill" << "*.cognitiaSkillPack";
     curdir.setNameFilters(filters);

     QStringList courseFiles = curdir.entryList();

     for (int i = 0; i < courseFiles.size(); ++i){

         if(!SendFile(courseFiles[i], client, static_cast<quint16>(retrieveSkillpack)))
             return false;

     }

     return true;
}




bool Server::SendStudentProgresstoClient(QTcpSocket *client, const QString &name){

     ui->Log-> append(QString("Sendind Student Progress to ") + name);
     QDataStream out(client);
     QDir curdir = QDir(name);

     QStringList courseFiles = curdir.entryList(QStringList() << "*.StudentProgress");

     for (int i = 0; i < courseFiles.size(); ++i){

         if(!SendFile(curdir.dirName() + QString("/") + courseFiles[i], client, retrieveStudentProgress))
             return false;
     }
    return true;
}




bool Server::CheckClient(const QString & name, quint16 code){
    //qDebug() << QDir::currentPath();
    return code == getMentorName ? true : QDir(name).exists();
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
    emit onClose();
}


void Server::on_addStudent_clicked()
{
    QString name = QFileDialog::getSaveFileName(this, "Create student Directory");

    QDir dir = QDir();
    dir.mkdir(name);

    StudentProgress student = StudentProgress();
    QFile file(name + QString("/") + name + QString(".StudentProgress"));
    if (!file.exists())
        student.save(&file);

}

