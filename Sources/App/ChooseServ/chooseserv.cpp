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

}


void ChooseServ::on_actionChoose_server_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Select server file");

    setServ(path);
}


void ChooseServ::setServ(const QString& path){

}


void ChooseServ::on_actionSave_Server_triggered()
{
    qDebug() << "Saving server" << objectName();


    QString name = ui->ServName->text();
    if(!name.size()){
        QMessageBox::critical(0, "Failing to save", "Please input server name");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Save file", ":/"+ name) + SERVEREXTENTION;

    QFile file = QFile(path);
    QFileInfo in = QFileInfo(file);
    QDir dr = in.dir();

    if (!dr.exists()) {
        dr.mkpath(dr.absolutePath());
    }

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        stream << name << ServDelim;
        stream << ui -> IPaddress -> text() << ServDelim;
        stream << ui -> PortNum -> text();

        file.close();
    } else {
        QMessageBox::critical(0, "Failing to save", "Please try again");
        return;
    }
}

