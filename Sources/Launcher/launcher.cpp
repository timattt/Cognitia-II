#include "launcher.h"
#include "ui_launcher.h"

Launcher::Launcher() :
    QWidget(nullptr),
    ui(new Ui::Launcher)
{
    ui->setupUi(this);

    this->show();
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::on_runButton_clicked()
{
     if (ui->modesChooser->currentText() == "Skill pack editor") {this->hide();
        emit startSkillPackEditor();

     }
}

