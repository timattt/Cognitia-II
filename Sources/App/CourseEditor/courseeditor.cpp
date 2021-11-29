#include "courseeditor.h"
#include "ui_courseeditor.h"
#include "skillsmodel.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/SkillPack/skill.h"
#include "../CourseUnitViewer/courseunitviewer.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../CourseUnitViewer/Node/edge.h"
#include "../Help/smarthelper.h"
#include "../Core/logger.h"
#include <QtWidgets>

CourseEditor::CourseEditor() :
    QMainWindow(nullptr),
	fileSignature(""),
    ui(new Ui::CourseEditor),
	head(nullptr)
{
	SAY("CourseEditor init started");

	ui->setupUi(this);


	// signals
	//-----------------------------
	connect(ui->widget, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));
	connect(ui->widget, SIGNAL(nodeSelected(Node*)), this->ui->skillsChooser, SLOT(nodeSelected(Node*)));
	connect(ui->widget, SIGNAL(nodeSelected(Node*)), this->ui->description, SLOT(nodeSelected(Node*)));
	connect(ui->widget, SIGNAL(nodeSelected(Node*)), this->ui->baseInfoAndSkills, SLOT(nodeSelected(Node*)));
	connect(ui->widget, SIGNAL(nodeSkillsChanged(Node*)), this, SLOT(nodeSkillsChanged(Node*)));
	connect(this, SIGNAL(selectNode(Node*)), ui->widget, SLOT(setSelectedNode(Node *)));
	connect(this, SIGNAL(clearSkillsLib()), ui->skillsChooser, SLOT(clearSkillsLib()));

	connect(this, SIGNAL(ensureLocked()), ui->skillsChooser, SLOT(ensureLocked()));
	connect(this, SIGNAL(unlock()), ui->skillsChooser, SLOT(unlock()));
	connect(this, SIGNAL(ensureLocked()), ui->description, SLOT(ensureLocked()));
	connect(this, SIGNAL(unlock()), ui->description, SLOT(unlock()));
	connect(this, SIGNAL(ensureLocked()), ui->baseInfoAndSkills, SLOT(ensureLocked()));
	connect(this, SIGNAL(unlock()), ui->baseInfoAndSkills, SLOT(unlock()));

	connect(this, SIGNAL(setSkillPack(QString)), ui->skillsChooser, SLOT(setSkillPack(QString)));
	//-----------------------------

	// clear and refresh everything
	//-----------------------------
	emit clearSkillsLib();
	ensureGuiLocked();
	//-----------------------------

	SAY("CourseEditor init finished!");
}

CourseEditor::~CourseEditor()
{
	ensureGuiLocked();
	emit clearSkillsLib();
    delete ui;
    if (head != nullptr) {
    	delete head;
    }
}

bool CourseEditor::isChanged() {
	try {
		if (!checkCourseUnitAvailable(false)) {
			return false;
		}

		CourseUnit cu;
		fromGuiToFile(&cu);
		return fileSignature.size() > 0 && QString::compare(fileSignature, cu.print(), Qt::CaseInsensitive);
	} catch (QString & err) {
		QMessageBox::information(this, "Error", err);
		return 0;
	}
}

void CourseEditor::nodeSelected(Node *nd) {
	Q_UNUSED(nd);
}

void CourseEditor::nodeSkillsChanged(Node *nd) {
	if (!checkCourseUnitAvailable(false)) {
		return;
	}
	if (nd == getCurrentNode()) {
		emit selectNode(nd);
	}
}

void CourseEditor::on_actionCourseUnitOpen_triggered() {
	ensureGuiLocked();

	QString path = QFileDialog::getOpenFileName(this, "Select course unit file", QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));

	QFile f = QFile(path);

	if (!f.exists()) {
		mes("File " + f.fileName() + " not exists!");
		return;
	}

	unlockGui();

	CourseUnit crs;
	try {
		crs.loadCourseUnit(&f);
		fromFileToGui(&crs);
		fileSignature = crs.print();

		mes("Opened course unit file " + f.fileName());
	} catch (QString & ex) {
		QMessageBox::critical(this, "Error", ex);
		mes("Error while opening: " + ex);
		ensureGuiLocked();
	}


}

void CourseEditor::on_actionCourseUnitSave_triggered() {
	if (!checkCourseUnitAvailable(true)) {
		return;
	}

	selectHead();

	QFile f = QFile(head->getFile());

	CourseUnit crs;

	try {
		fromGuiToFile(&crs);
		crs.saveCourseUnit(&f);
		fileSignature = crs.print();
		mes("Saved course unit file " + head->getFile());
	} catch (QString & ex) {
		QMessageBox::critical(this, "Error", ex);
		mes("Error while saving: " + ex);
		return;
	}

	// PATHS
	// CU name -> file path
	QMap<QString, QString> paths;

	paths[crs.objectName()] = crs.getLastFilePath();

	for (CourseUnit * cu : crs.getEmbedded()) {
		paths[cu->objectName()] = cu->getLastFilePath();
	}

	head->setFile(paths[head->getName()]);
	ui->widget->setPaths(paths);

	selectHead();
}

void CourseEditor::on_actionCourseUnitCreate_triggered() {
	ensureGuiLocked();

	QString path = QFileDialog::getSaveFileName(this, "Create course unit file", QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));

	if (path.length() == 0) {
		return;
	}

	QFile f = QFile(path);
	QFileInfo in = QFileInfo(f);

	CourseUnit crs;
	crs.setObjectName(in.baseName());
	crs.setFieldSize(20 * NODE_RAD, 20 * NODE_RAD);

	unlockGui();

	try {
		crs.saveCourseUnit(&f);
		fromFileToGui(&crs);

		mes("Created course unit file " + path);
	} catch (QString & err) {
		QMessageBox::critical(this, "Error", err);
		mes("Error while creating: " + err);
		ensureGuiLocked();
	}

}

void CourseEditor::ensureGuiLocked() {
	if (head != nullptr && isChanged()
			&& QMessageBox::question(this, "Save file or not",
					"Would you like to save your file before closing it?",
					QMessageBox::Save | QMessageBox::Discard)
					== QMessageBox::Save) {
		CourseUnit cu;
		fromGuiToFile(&cu);
		QFile f = QFile(head->getFile());
		cu.saveCourseUnit(&f);
	}

	emit ensureLocked();
	ui->widget->setEditable(false);

	ui->widget->clearAllScene();

	fileSignature = "";

	if (head != nullptr) {
		delete head;
		head = nullptr;
	}
}

void CourseEditor::fromFileToGui(CourseUnit *crs) {
	NOT_NULL(crs);
	NOT_NULL(head);

	head->fromCourseUnitToNode(crs);

	ui->widget->unpack(crs);

	selectHead();
}

void CourseEditor::fromGuiToFile(CourseUnit *crs) {
	NOT_NULL(crs);
	NOT_NULL(head);

	head->fromNodeToCourseUnit(crs);

	ui->widget->pack(crs);

	for (CourseUnit * cu : crs->getEmbedded()) {
		if (cu->objectName().toLower() == crs->objectName().toLower()) {
			throw QString("Head courseunit has same name as child courseunit!");
		}
	}
}

void CourseEditor::on_actionSkillPackOpen_triggered() {
	on_actionClose_skillPack_triggered();

	QString path = QFileDialog::getOpenFileName(this, "Select skill pack file", QString(), QString("(*") + SKILL_PACK_FILE_EXTENSION + QString(")"));

	if (path.length() == 0) {
		return;
	}

	emit setSkillPack(path);
}

void CourseEditor::on_actionReturn_to_launcher_triggered() {
	ensureGuiLocked();
	emit clearSkillsLib();
	emit onClose();
}

void CourseEditor::on_actionHelp_me_triggered() {
	SmartHelper helper(":/help/Help/CourseEditorHelp.txt", this);
	helper.exec();
}

void CourseEditor::on_actionClose_skillPack_triggered() {
	emit clearSkillsLib();
}

void CourseEditor::mes(QString mes) {
	ui->statusbar->showMessage(mes);
}

void CourseEditor::on_actionClose_courseUnit_triggered() {
	ensureGuiLocked();
}

Node* CourseEditor::getCurrentNode() {
	NOT_NULL(head);
	return (ui->widget->getSelectedNode() == nullptr ? head : ui->widget->getSelectedNode());
}

bool CourseEditor::checkCourseUnitAvailable(bool showMessage) {
	if (head == nullptr && showMessage) {
		mes("No course unit! Create or open it!");
	}
	return head != nullptr;
}

void CourseEditor::unlockGui() {
	ASSERT(head == nullptr);

	head = new Node(nullptr);
	ui->widget->setEditable(true);
	emit unlock();
}

void CourseEditor::selectHead() {
	if (!checkCourseUnitAvailable(true)) {
		return;
	}

	emit selectNode(head);
}
