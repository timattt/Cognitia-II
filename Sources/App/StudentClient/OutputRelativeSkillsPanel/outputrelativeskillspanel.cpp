#include "outputrelativeskillspanel.h"

#include "ui_outputrelativeskillspanel.h"

OutputRelativeSkillsPanel::OutputRelativeSkillsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputRelativeSkillsPanel)
{
    ui->setupUi(this);
}

OutputRelativeSkillsPanel::~OutputRelativeSkillsPanel()
{
    delete ui;
}

void OutputRelativeSkillsPanel::clearAll() {
}

void OutputRelativeSkillsPanel::progressMade(QString skill, double lev) {
	Q_UNUSED(skill);
	Q_UNUSED(lev);
}

void OutputRelativeSkillsPanel::unpack(Node *nd) {
	Q_UNUSED(nd);
}
