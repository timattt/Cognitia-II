#include "chooseserv.h"
#include "ui_chooseserv.h"

#define SERVEREXTENTION ".server"
#define ServDelim "|||"

ChooseServ::ChooseServ(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseServ)
{
    ui->setupUi(this);
}

ChooseServ::~ChooseServ()
{
    delete ui;
}


void ChooseServ::on_ConnectButton_clicked()
{
    emit onServConnectclicked();
}

QString ChooseServ::getIP(){
    return ui -> IPaddress -> text();
}

QString ChooseServ::getPort(){
    return ui -> PortNum -> text();
}

QString ChooseServ::getName(){
    return ui -> ServName -> text();
}


void ChooseServ::on_actionChoose_server_triggered()
{
    QString filter = QString("(*") + QString(SERVEREXTENTION) + QString(")");
    QString path = QFileDialog::getOpenFileName(this, "Select server file", QString(), filter);

    setServ(path);
}


void ChooseServ::setServ(const QString& path){
        QFile serv = QFile(path);

        if (!serv.exists()) {
            QMessageBox::critical(0, "Failing to load", "Please choose server file");
            return;
        }

        QString data = nullptr;

        if (serv.open(QIODevice::ReadOnly)) {
            QTextStream stream(&serv);
            data = stream.readAll();
            serv.close();
        } else {
            QMessageBox::critical(0, "Failing to load", "Please try one more time");
            return;
        }

        QStringList unit_data = data.split(ServDelim, Qt::SkipEmptyParts);

        ui -> ServName -> textChanged(unit_data[0]);
        ui -> IPaddress -> textChanged(unit_data[1]);
        ui -> PortNum -> textChanged(unit_data[2]);
}


void ChooseServ::on_actionSave_Server_triggered()
{
    qDebug() << "Saving server" << objectName();

    QString path = QFileDialog::getSaveFileName(this, "Save file") + SERVEREXTENTION;

    QFile file = QFile(path);
    QFileInfo in = QFileInfo(file);
    QDir dr = in.dir();

    if (!dr.exists()) {
        dr.mkpath(dr.absolutePath());
    }

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        stream << ui -> ServName -> text() << ServDelim;
        stream << ui -> IPaddress -> text() << ServDelim;
        stream << ui -> PortNum -> text();

        file.close();
    } else {
        QMessageBox::critical(0, "Failing to save", "Please try again");
        return;
    }
}

