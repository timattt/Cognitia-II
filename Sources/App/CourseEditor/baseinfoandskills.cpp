#include "baseinfoandskills.h"
#include "ui_baseinfoandskills.h"
#include "skillsmodel.h"
#include "courseeditor.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Core/logger.h"

BaseInfoAndSkills::BaseInfoAndSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseInfoAndSkills),
	inMd(nullptr),
	outMd(nullptr),
	editor(nullptr)
{
    ui->setupUi(this);

    editor = dynamic_cast<CourseEditor*>(parent->parent()->parent()->parent());

    NOT_NULL(editor);

	// Skills table
	//-----------------------------
	inMd = new SkillsModel(editor, 1);
	outMd = new SkillsModel(editor, 0);

	ui->inList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->outList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->inList->setAcceptDrops(true);
	ui->inList->setDropIndicatorShown(true);

	ui->outList->setAcceptDrops(true);
	ui->outList->setDropIndicatorShown(true);

	ui->inList->setModel(inMd);
	ui->outList->setModel(outMd);
	//-----------------------------
}

BaseInfoAndSkills::~BaseInfoAndSkills()
{
    delete ui;
}


void BaseInfoAndSkills::on_removeIn_clicked() {
	if (!editor->checkCourseUnitAvailable(true)) {
		return;
	}

	int row = ui->inList->currentIndex().row();
	QString name = inMd->data(inMd->index(row, 0)).toString();
	editor->getCurrentNode()->removeInSkill(name);
	inMd->removeSkill(name);
}

void BaseInfoAndSkills::on_removeOut_clicked() {
	if (!editor->checkCourseUnitAvailable(true)) {
		return;
	}

	int row = ui->outList->currentIndex().row();
	QString name = outMd->data(outMd->index(row, 0)).toString();
	editor->getCurrentNode()->removeOutSkill(name);
	outMd->removeSkill(name);
}

void BaseInfoAndSkills::on_showParent_clicked() {
	editor->selectHead();
}

void BaseInfoAndSkills::nodeSelected(Node *nd) {
	inMd->clear();
	outMd->clear();

	inMd->setHorizontalHeaderLabels(QStringList() << "Skill" << "Level");
	outMd->setHorizontalHeaderLabels(QStringList() << "Skill" << "Level");

	ui->nameLineEdit->clear();
	ui->fileLineEdit->clear();

	if (nd == nullptr) {
		return;
	}

	for (QString in : nd->getInSkills().keys()) {
		int lev = nd->getInSkills()[in];
		inMd->addSkill(in, lev);
	}
	for (QString out : nd->getOutSkills().keys()) {
		int lev = nd->getOutSkills()[out];
		outMd->addSkill(out, lev);
	}

	ui->nameLineEdit->setText(nd->getName());
	ui->fileLineEdit->setText(nd->getFile());
}

void BaseInfoAndSkills::ensureLocked() {
	ui->removeIn->setEnabled(false);
	ui->removeOut->setEnabled(false);
	ui->showParent->setEnabled(false);
	ui->inList->setEnabled(false);
	ui->outList->setEnabled(false);
	ui->nameLineEdit->setEnabled(false);
	inMd->clear();
	outMd->clear();
	ui->nameLineEdit->clear();
	ui->fileLineEdit->clear();
}

void BaseInfoAndSkills::unlock() {
	ui->inList->setEnabled(true);
	ui->outList->setEnabled(true);
	ui->nameLineEdit->setEnabled(true);
	ui->removeIn->setEnabled(true);
	ui->removeOut->setEnabled(true);
	ui->showParent->setEnabled(true);
}

void BaseInfoAndSkills::on_nameLineEdit_textChanged() {
	if (!editor->checkCourseUnitAvailable(false)) {
		return;
	}
	editor->getCurrentNode()->setName(ui->nameLineEdit->text());
}
