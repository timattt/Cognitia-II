#include "skillsmixer.h"
#include "ui_skillsmixer.h"
#include "../CourseUnitViewer/Node/node.h"
#include "skillsmixerholder.h"
#include "../Core/logger.h"

SkillsMixer::SkillsMixer(SkillsMixerHolder *parent, double from, double to, double val, QString name) :
    QWidget(parent),
	holder(parent),
    ui(new Ui::SkillsMixer)
{
	SAY("Init skills mixer");

    ui->setupUi(this);

    ui->skillName->setText(name);
    ui->from->setText(QString::number(from) + " <");
    ui->to->setText("< " + QString::number(to));
    ui->val->setText(QString::number(val, 'g', (val > 10 ? 3 : 2)));

    ui->horizontalSlider->setRange(from * 10, to * 10);
    ui->horizontalSlider->setSingleStep(1);
    ui->horizontalSlider->setPageStep(2);
    ui->horizontalSlider->setValue(val * 10);

    SAY("Skills mixer init done");
}

SkillsMixer::~SkillsMixer()
{
    delete ui;
}

void SkillsMixer::on_horizontalSlider_valueChanged(int a) {
	double v = (double)a / 10.0;
	ui->val->setText(QString::number(v));

	emit holder->skillLevelChanged(ui->skillName->text(), v);
}

void SkillsMixer::setValue(double v) {
	ui->horizontalSlider->setValue(v * 10);
	ui->val->setText(QString::number(v, 'g', (v > 10 ? 3 : 2)));
	update();
}
