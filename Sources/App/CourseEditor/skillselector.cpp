#include "skillselector.h"
#include "ui_skillselector.h"

SkillSelector::SkillSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillSelector)
{
    ui->setupUi(this);
}

SkillSelector::~SkillSelector()
{
    delete ui;
}
