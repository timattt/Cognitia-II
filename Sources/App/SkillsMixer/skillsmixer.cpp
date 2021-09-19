#include "skillsmixer.h"
#include "ui_skillsmixer.h"
#include "../CourseUnitViewer/Node/node.h"

SkillsMixer::SkillsMixer(QWidget *parent, double from, double to, double val, QString name, Node *nd) :
    QWidget(parent),
	node(nd),
    ui(new Ui::SkillsMixer)
{
	qInfo() << "Init skills mixer";

    ui->setupUi(this);

    ui->skillName->setText(name);
    ui->from->setText(QString::number(from) + " <");
    ui->to->setText("< " + QString::number(to));
    ui->val->setText(QString::number(val));

    ui->horizontalSlider->setRange(from * 10, to * 10);
    ui->horizontalSlider->setSingleStep(1);
    ui->horizontalSlider->setPageStep(2);
    ui->horizontalSlider->setValue(val * 10);

    qInfo() << "Skills mixer init done";
}

SkillsMixer::~SkillsMixer()
{
    delete ui;
}

void SkillsMixer::on_horizontalSlider_valueChanged(int a) {
	double v = (double)a / 10.0;
	ui->val->setText(QString::number(v));
	node->setProgress(ui->skillName->text(), v);
}

void SkillsMixer::setValue(double v) {
	ui->horizontalSlider->setValue(v * 10);
	ui->val->setText(QString::number(v));
	update();
}
