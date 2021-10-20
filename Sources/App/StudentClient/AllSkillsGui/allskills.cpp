#include "allskills.h"
#include "ui_allskills.h"

AllSkills::AllSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllSkills),
	skillPack(nullptr)
{
    ui->setupUi(this);
}

AllSkills::~AllSkills()
{
    delete ui;
}

void AllSkills::clearAll() {
}

void AllSkills::setSkp(SkillPack *skp) {
	Q_UNUSED(skp);
}
