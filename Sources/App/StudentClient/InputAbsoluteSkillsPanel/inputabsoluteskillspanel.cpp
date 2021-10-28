#include "inputabsoluteskillspanel.h"

#include "ui_inputabsoluteskillspanel.h"
#include "../../Core/logger.h"

InputAbsoluteSkillsPanel::InputAbsoluteSkillsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputAbsoluteSkillsPanel)
{
    ui->setupUi(this);
}

InputAbsoluteSkillsPanel::~InputAbsoluteSkillsPanel()
{
    delete ui;
}

void InputAbsoluteSkillsPanel::clearAll() {
}

void InputAbsoluteSkillsPanel::progressMade(QString skill, double lev) {
	Q_UNUSED(skill);
	Q_UNUSED(lev);
}

void InputAbsoluteSkillsPanel::unpack(Node *nd) {
	Q_UNUSED(nd);
}
