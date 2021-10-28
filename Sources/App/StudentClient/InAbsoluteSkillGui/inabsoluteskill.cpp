#include "inabsoluteskill.h"
#include "ui_inabsoluteskill.h"
#include "../../Core/logger.h"

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
	Q_UNUSED(skill);
	Q_UNUSED(lev);
}

void InAbsoluteSkill::unpack(Node *nd) {
	Q_UNUSED(nd);
}
