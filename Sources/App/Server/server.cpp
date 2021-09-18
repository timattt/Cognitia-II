#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
    nextblocksize(0)
{
    ui->setupUi(this);
    mtcpServ = new QTcpServer(this);
    ui -> Log -> setReadOnly(true);
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
    mTcpSocket* ClientSocket = mtcpServ -> nextPendingConnection();
    connect(ClientSocket, SIGNAL(client_disconnected(mTcpSocket*)), SLOT(deleteFromLog(mTcpSocket*)));
    connect(ClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    qDebug() << "new connection";
}

void Server::slotReadClient(){

    mTcpSocket* ClientSocket = static_cast<mTcpSocket*>(sender());
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


void Server::deleteFromLog(mTcpSocket* client){
    if(!Users.contains(client)){
        return;
    }
    QString name = Users[client];
    Users.remove(client);
    QString log = ui -> ActiveUsers -> toPlainText();
    log.replace(name, "");
    ui -> ActiveUsers -> setText(log);
}



void Server::handleReq(mTcpSocket* client, quint32 block, const QByteArray &data){
    QDataStream in(data);

    QString name;
    in >> name;
    qDebug() << "accept name" << name;
    if (!CheckClient(name)){
        sendToClient(client, static_cast<quint16>(retrieveFailAutorisation), "");
        return;
    }

    quint16 incCode;
    in >> incCode;


    switch (incCode) {
    case getUserName:
        Users[client] = name;
        ui->Log-> append(name + QString(" connected!"));
        ui->ActiveUsers->append(name);
        if (!SendCoursetoClient(client, name) ||
            !SendSkillpacktoClient(client, name) ||
            !SendStudentProgresstoClient(client, name))
        {
            sendToClient(client, static_cast<quint16>(retrieveFail), "");
            ui->Log-> append(QString("Fail!"));
            break;
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


bool Server::SendFile(const QString& filename, mTcpSocket *client, quint16 code){

    QFile file(filename);
    qDebug() << filename;
    if (file.open(QIODevice::ReadOnly))
       {
            QByteArray arrBlock;
            QDataStream out(&arrBlock, QIODevice::WriteOnly);
           // filename = filename.section("/", -1);
            out << quint32(0) << code << filename << file.readAll();
            out.device()->seek(0);
            out << quint32(arrBlock.size() - sizeof(quint32));

            client -> write(arrBlock);


       }
    else
        return false;
    return true;
}


bool Server::SendCoursetoClient(mTcpSocket *client, const QString &name){

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



bool Server::SendSkillpacktoClient(mTcpSocket *client, const QString &name){

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




bool Server::SendStudentProgresstoClient(mTcpSocket *client, const QString &name){

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




bool Server::CheckClient(const QString & name){
    qDebug() << QDir::currentPath();
    return QDir(name).exists();
}



void Server::sendToClient(mTcpSocket* Socket, quint16 code,  const QString& str) {
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
}

