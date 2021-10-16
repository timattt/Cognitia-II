#include "courseunitviewersettings.h"
#include "ui_courseunitviewersettings.h"
#include "Node/node.h"

CourseUnitViewerSettings::CourseUnitViewerSettings(QWidget *parent, int totalN, int totalE, int totalI, double att, double rep, double own) :
    QDialog(parent),
	repulsion(rep),
	attraction(att),
	ownLength(own),
    ui(new Ui::CourseUnitViewerSettings)
{
    ui->setupUi(this);

	ui->totalEdges->setText(QString::number(totalE));
	ui->totalNodes->setText(QString::number(totalN));
	ui->totalItems->setText(QString::number(totalI));

	ui->attFact->setText(QString::number(att));
	ui->repFact->setText(QString::number(rep));
	ui->ownLength->setText(QString::number(own));
}

CourseUnitViewerSettings::~CourseUnitViewerSettings()
{
    delete ui;
}

void CourseUnitViewerSettings::on_attFact_textEdited(QString v) {
	bool ok = 0;
	double val = v.toDouble(&ok);
	if (!ok) {
		return;
	}

	attraction = val;
}

void CourseUnitViewerSettings::on_repFact_textEdited(QString v) {
	bool ok = 0;
	double val = v.toDouble(&ok);
	if (!ok) {
		return;
	}

	repulsion = val;
}

void CourseUnitViewerSettings::on_ownLength_textEdited(QString v) {
	bool ok = 0;
	double val = v.toDouble(&ok);
	if (!ok) {
		return;
	}

	ownLength = val;
}
