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
    connect(mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
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
    chooseserv -> show();
}

void StudentClient::onStart(){
    chooseserv -> show();
    this -> show();
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

    StudentName = chooseserv -> getName();
    this -> sendToServer(static_cast<int>(getUserName), "");

    if (mSocket -> waitForReadyRead(10000))
        chooseserv -> hide();
    else
        QMessageBox::critical(this, "Failing", "Timeout");
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


        in >> datafromServer;

        endReception(nextBlockSize);
        nextBlockSize = 0;
    }
}



void StudentClient::endReception(int nextBlockSize){

}


void StudentClient::sendToServer(int code, const QString& str){

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    mSocket -> write(arrBlock);

}


void StudentClient::on_actionSave_all_and_send_triggered()
{
     ui->statusbar->showMessage("Save");
}




void StudentClient::on_actionReturn_to_Launcher_triggered()
{
    chooseserv -> hide();
    emit onClose();
}

