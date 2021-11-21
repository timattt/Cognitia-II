#include "skillsmixer.h"
#include "ui_skillsmixer.h"
#include "../CourseUnitViewer/Node/node.h"
#include "skillsmixerholder.h"
#include "../Core/logger.h"

SkillsMixer::SkillsMixer(SkillsMixerHolder *parent, double from, double to, double val, QString name) :
    QWidget(parent),
	holder(parent),
    ui(new Ui::SkillsMixer),
	ready(false)
{
	SAY("Init skills mixer");

    ui->setupUi(this);

    ui->skillName->setText(name);
    ui->from->setText(QString::number(from) + " <");
    ui->to->setText("< " + QString::number(to));
    ui->val->setText(QString::number(val, 'g', 3));
    ui->dial->setRange(from * 10, to * 10);
    ui->dial->setValue(val * 10);

    ready = true;

    SAY("Skills mixer init done");
}

SkillsMixer::~SkillsMixer()
{
    delete ui;
}

void SkillsMixer::on_dial_valueChanged(int a) {
	if (!ready) {
		return;
	}

	double v = (double)a/10;
	ui->val->setText(QString::number(v));

	emit holder->skillLevelChanged(ui->skillName->text(), v);
}

void SkillsMixer::setValue(double v) {
	ui->dial->setValue(v*10);
	ui->val->setText(QString::number(v, 'g', 3));
	update();
}
