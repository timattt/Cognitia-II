#include "studentclient.h"
#include "ui_studentclient.h"

StudentClient::StudentClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentClient)
{
    ui->setupUi(this);
}

StudentClient::~StudentClient()
{
    delete ui;
}

void StudentClient::on_actionChange_Server_triggered()
{
    ui->statusbar->showMessage("Changing server");
}


void StudentClient::on_actionSave_all_and_send_triggered()
{
     ui->statusbar->showMessage("Save");
}

