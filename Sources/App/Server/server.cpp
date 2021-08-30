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
    connect(mtcpServ, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    if (mtcpServ -> isListening())
    {
        ui->Log->append("Server has already been started!\n");
        return;
    }
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
    connect(ClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    sendToClient(ClientSocket, "Connected");
    ui->Log->append("New connection success!\n");
}

void Server::slotReadClient(){
    QTcpSocket* ClientSocket = static_cast<QTcpSocket*>(sender());
    QDataStream in(ClientSocket);

    for(;;){
        if(!nextblocksize){
            if(ClientSocket -> bytesAvailable() < sizeof(quint16))
                break;
            in >> nextblocksize;
        }

        if(ClientSocket -> bytesAvailable() < nextblocksize)
            break;

        QString str;
        in >> str;
        nextblocksize = 0;

        ui->Log-> append(str);
        ui->ActiveUsers->append(str);

    }
}

void Server::sendToClient(QTcpSocket* Socket, const QString& str) {
    /* QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    Socket -> write(arrBlock);
    */
}
