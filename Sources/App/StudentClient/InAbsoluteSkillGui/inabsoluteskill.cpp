#include "inabsoluteskill.h"
#include "ui_inabsoluteskill.h"

InAbsoluteSkill::InAbsoluteSkill(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InAbsoluteSkill)
{
    ui->setupUi(this);
}

InAbsoluteSkill::~InAbsoluteSkill()
{
    delete ui;
}

void InAbsoluteSkill::clearAll() {
}

void InAbsoluteSkill::progressMade(QString skill, double lev) {
}

void InAbsoluteSkill::unpack(Node *nd) {
}
