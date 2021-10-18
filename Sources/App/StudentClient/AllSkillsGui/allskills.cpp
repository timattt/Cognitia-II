#include "allskills.h"
#include "ui_allskills.h"

AllSkills::AllSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllSkills)
{
    ui->setupUi(this);
}

AllSkills::~AllSkills()
{
    delete ui;
}
