#include "skillwidget.h"
#include "ui_skillwidget.h"
#include "../../Structures/SkillPack/skill.h"

SkillWidget::SkillWidget(QWidget *parent, Skill * sk) :
    QWidget(parent),
    ui(new Ui::SkillWidget)
{
    ui->setupUi(this);
    ui->skillName->setText(sk->objectName());
}

SkillWidget::~SkillWidget()
{
    delete ui;
}
