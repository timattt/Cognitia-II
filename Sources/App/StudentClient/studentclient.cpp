#include "studentclient.h"
#include "ui_studentclient.h"

StudentClient::StudentClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentClient)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);
    chooseserv = new ChooseServ(this);
    connect(mSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(mSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(chooseserv, SIGNAL(onServConnectclicked()), SLOT(startConnection()));
}

StudentClient::~StudentClient()
{
    delete mSocket;
    delete chooseserv;
    delete ui;
}

void StudentClient::on_actionChange_Server_triggered()
{
    ui->statusbar->showMessage("Changing server");

}

void StudentClient::onStart(){
    chooseserv -> show();
}

void StudentClient::startConnection(){
    QString IP = chooseserv -> getIP();
    unsigned Port = (chooseserv -> getPort()).toUInt();

    mSocket -> connectToHost(IP, Port);

}

void StudentClient::slotConnected(){
    this -> show();
    chooseserv -> hide();

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

    QMessageBox::critical(0, "Failing", strEr);
    mSocket -> close();
}

void StudentClient::slotReadyRead(){
    QDataStream in(mSocket);

    for(;;){
        if(!nextBlockSize){
            if(mSocket -> bytesAvailable() < sizeof(quint16))
                break;
            in >> nextBlockSize;
        }

        if(mSocket -> bytesAvailable() < nextBlockSize)
            break;

        QString str;
        in >> str;
        nextBlockSize = 0;
    }
}


void StudentClient::slotSendToServer(){

}


void StudentClient::on_actionSave_all_and_send_triggered()
{
     ui->statusbar->showMessage("Save");
}



