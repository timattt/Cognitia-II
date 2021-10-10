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
    ui(new Ui::CourseEditor),
	skillsLib(),
	inMd(nullptr),
	outMd(nullptr),
	head(new Node(nullptr)),
	timerId(0),
	lastSkillPackUpdate(0)
{
	SAY("CourseEditor init started");

	ui->setupUi(this);

	// Skills table
	//-----------------------------
	inMd = new SkillsModel(this, 1);
	outMd = new SkillsModel(this, 0);

	ui->inList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->outList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->inList->setAcceptDrops(true);
	ui->inList->setDropIndicatorShown(true);

	ui->outList->setAcceptDrops(true);
	ui->outList->setDropIndicatorShown(true);

	ui->inList->setModel(inMd);
	ui->outList->setModel(outMd);
	//-----------------------------

	// signals
	//-----------------------------
	connect(ui->widget, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));
	connect(ui->widget, SIGNAL(nodeSkillsChanged(Node*)), this, SLOT(nodeSkillsChanged(Node*)));
	//-----------------------------

	// clear and refresh everything
	//-----------------------------
	clearSkillsLib();
	clearCourseUnit();
	//-----------------------------

	timerId = startTimer(SKILL_PACK_UPDATE_TIME);

	SAY("CourseEditor init finished!");
}

CourseEditor::~CourseEditor()
{
	clearCourseUnit();
	clearSkillsLib();
    delete ui;
    killTimer(timerId);
    delete head;
}

void CourseEditor::addSkillToLib(Skill * sk) {
	NOT_NULL(sk);

	skillsLib[sk->objectName()] = QMap<int, QString>();
	for (int i = 0; i < sk->getLevelsCount(); i++) {
		skillsLib[sk->objectName()][i + 1] = sk->getLevelDescription(i);
	}
	ui->skillsSelector->addItem(sk->objectName());
}

void CourseEditor::on_addSkill_pressed()
{
	bool ok = 0;

	QString name = ui->skillsSelector->currentText();

	ui->levelsSelector->currentText().toInt(&ok);

	if (!ok || !skillsLib.contains(name)) {
		return;
	}

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText(ui->skillsSelector->currentText() + SKILL_PACK_DELIMITER + ui->levelsSelector->currentText());

    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/icons/Icons/SkillDrag.png"));

    drag->exec(Qt::CopyAction);
}


void CourseEditor::on_skillsSelector_currentTextChanged(const QString &arg1)
{
	ui->levelsSelector->clear();
	if (skillsLib.contains(arg1)) {
		for (int i = 1; i <= skillsLib[arg1].size(); i++) {
			ui->levelsSelector->addItem(QString::number(i));
		}
	}
}

void CourseEditor::on_removeSkill_pressed() {
	bool ok = 0;

	QString name = ui->skillsSelector->currentText();

	ui->levelsSelector->currentText().toInt(&ok);

	if (!ok || !skillsLib.contains(name)) {
		return;
	}

	QDrag *drag = new QDrag(this);
	QMimeData *mimeData = new QMimeData;

	mimeData->setText(
			ui->skillsSelector->currentText() + QString(SKILL_PACK_DELIMITER) + QString(SKILL_PACK_DELIMITER)
					+ ui->levelsSelector->currentText());

	drag->setMimeData(mimeData);
	drag->setPixmap(QPixmap(":/icons/Icons/SkillDrag.png"));

	drag->exec();
}

void CourseEditor::setNodeToRedactor(Node *nd) {
	NOT_NULL(nd);

	// Update skills table
	//--------------------------------------------
	inMd->clear();
	outMd->clear();

	inMd->setHorizontalHeaderLabels(QStringList() << "Skill" << "Level");
	outMd->setHorizontalHeaderLabels(QStringList() << "Skill" << "Level");

	for (QString in : nd->getInSkills().keys()) {
		int lev = nd->getInSkills()[in];
		inMd->addSkill(in, lev);
	}
	for (QString out : nd->getOutSkills().keys()) {
		int lev = nd->getOutSkills()[out];
		outMd->addSkill(out, lev);
	}
	//--------------------------------------------

	// update gui panels
	//--------------------------------------------
	ui->nameLineEdit->setText(nd->getName());
	ui->fileLineEdit->setText(nd->getFile());
	ui->descrPanel->setText(nd->getDescription());
	//--------------------------------------------
}

void CourseEditor::on_levelsSelector_currentTextChanged(const QString &arg1)
{
	Q_UNUSED(arg1);

	if (skillsLib.contains(ui->skillsSelector->currentText()) &&
			skillsLib[ui->skillsSelector->currentText()].contains(ui->levelsSelector->currentText().toInt())) {
		ui->levelDescription->setText(skillsLib[ui->skillsSelector->currentText()][ui->levelsSelector->currentText().toInt()]);
	} else {
		ui->levelDescription->clear();
	}
}

void CourseEditor::on_showParent_clicked() {
	ui->widget->setSelectedNode(nullptr);
	setNodeToRedactor(head);
}

void CourseEditor::on_removeIn_clicked() {
	int row = ui->inList->currentIndex().row();
	QString name = inMd->data(inMd->index(row, 0)).toString();
	getCurrentNode()->removeInSkill(name);
	inMd->removeRow(row);
}

void CourseEditor::on_removeOut_clicked() {
	int row = ui->outList->currentIndex().row();
	QString name = outMd->data(outMd->index(row, 0)).toString();
	getCurrentNode()->removeOutSkill(name);
	outMd->removeRow(row);
}

void CourseEditor::on_nameLineEdit_textChanged() {
	getCurrentNode()->setName(ui->nameLineEdit->text());
}

void CourseEditor::nodeSelected(Node *nd) {
	if (nd == nullptr) {
		nd = head;
	}
	setNodeToRedactor(nd);
}
void CourseEditor::nodeSkillsChanged(Node *nd) {
	if (nd == getCurrentNode()) {
		setNodeToRedactor(nd);
	}
}

void CourseEditor::on_actionCourseUnitOpen_triggered() {
	on_actionClose_courseUnit_triggered();

	QString path = QFileDialog::getOpenFileName(this, "Select course unit file", QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));

	QFile f = QFile(path);

	if (!f.exists()) {
		mes("File " + f.fileName() + " not exists!");
		return;
	}

	head->setFile(path);

	CourseUnit crs;
	try {
		crs.loadCourseUnit(&f);
		fromFileToGui(&crs);

		mes("Opened course unit file " + f.fileName());
	} catch (QString & ex) {
		QMessageBox::critical(this, "Error", ex);
		mes("Error while opening: " + ex);
	}


}

void CourseEditor::on_actionCourseUnitSave_triggered() {
	QFile f = QFile(head->getFile());

	CourseUnit crs;

	try {
		fromGuiToFile(&crs);
		crs.saveCourseUnit(&f);

		clearCourseUnit();//
		fromFileToGui(&crs);// to insure that it will write file names into panels

		mes("Saved course unit file " + head->getFile());
	} catch (QString & ex) {
		QMessageBox::critical(this, "Error", ex);
		mes("Error while saving: " + ex);
	}
}

void CourseEditor::on_actionCourseUnitCreate_triggered() {
	on_actionClose_courseUnit_triggered();

	QString path = QFileDialog::getSaveFileName(this, "Create course unit file", QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));

	QFile f = QFile(path);
	QFileInfo in = QFileInfo(f);

	head->setName(in.fileName());
	head->setFile(path);

	CourseUnit crs;

	try {
		fromGuiToFile(&crs);
		crs.saveCourseUnit(&f);
		mes("Created course unit file " + path);

		fromFileToGui(&crs);// to insure that it will write file names into panels
	} catch (QString err) {
		QMessageBox::critical(this, "Error", err);
		mes("Error wjile creating: " + err);
	}

}

void CourseEditor::clearSkillsLib() {
	skillsLib.clear();
	ui->skillsSelector->clear();
	ui->levelsSelector->clear();
	ui->skillPackFile->clear();
}

void CourseEditor::clearCourseUnit() {
	head->setName("Parent course unit");
	head->setFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/NEW_COURSE_UNIT") + COURSE_UNIT_FILE_EXTENSION);
	head->clearSkills();
	head->setDescription("");
	setNodeToRedactor(head);
	ui->widget->clearAllScene();
	ui->descrPanel->clear();
}

void CourseEditor::fromFileToGui(CourseUnit *crs) {
	NOT_NULL(crs);

	fromCourseUnitToNode(crs, head);

	ui->widget->unpack(crs);

	setNodeToRedactor(head);
}

void CourseEditor::fromGuiToFile(CourseUnit *crs) {
	NOT_NULL(crs);

	fromNodeToCourseUnit(head, crs);

	ui->widget->pack(crs);
}

void CourseEditor::on_actionSkillPackOpen_triggered() {
	on_actionClose_skillPack_triggered();

	QString path = QFileDialog::getOpenFileName(this, "Select skill pack file", QString(), QString("(*") + SKILL_PACK_FILE_EXTENSION + QString(")"));

	setSkillPack(path);
}

void CourseEditor::on_actionReturn_to_launcher_triggered() {
	clearCourseUnit();
	clearSkillsLib();
	emit onClose();
}

void CourseEditor::on_actionHelp_me_triggered() {
	SmartHelper helper(":/help/Help/CourseEditorHelp.txt", this);
	helper.exec();
}

void CourseEditor::on_actionClose_skillPack_triggered() {
	clearSkillsLib();
}

void CourseEditor::mes(QString mes) {
	ui->statusbar->showMessage(mes);
}

void CourseEditor::on_actionClose_courseUnit_triggered() {
	clearCourseUnit();
}

void CourseEditor::setSkillPack(QString path) {
	int i1 = ui->skillsSelector->currentIndex();
	int i2 = ui->levelsSelector->currentIndex();

	SkillPack skp;
	QFile f = QFile(path);

	if (!f.exists()) {
		mes("SkillPack file " + path + " does not exists!");
		QMessageBox::critical(this, "Error", "SkillPack file " + path + " does not exists!");
		return;
	}

	clearSkillsLib();
	ui->skillPackFile->setText(path);

	skp.load(&f);
	for (int i = 0; i < skp.getSkillsCount(); i++) {
		addSkillToLib(skp.getSkill(i));
	}

	mes("SkillPack file " + path + " is loaded!");

	ui->skillsSelector->setCurrentIndex(i1);
	ui->levelsSelector->setCurrentIndex(i2);
}

void CourseEditor::on_descrPanel_textChanged() {
	getCurrentNode()->setDescription(ui->descrPanel->toPlainText());
	ui->markDownPreview->setMarkdown(ui->descrPanel->toPlainText());
}

Node* CourseEditor::getCurrentNode() {
	return (ui->widget->getSelectedNode() == nullptr ? head : ui->widget->getSelectedNode());
}

void CourseEditor::timerEvent(QTimerEvent *event) {
	Q_UNUSED(event);

	QFile f = QFile(ui->skillPackFile->text());
	if (!f.exists()) {
		return;
	}
	QFileInfo in = QFileInfo(f);

	long long t = in.lastModified().toMSecsSinceEpoch();

	if (lastSkillPackUpdate < t) {
		lastSkillPackUpdate = t;
		setSkillPack(ui->skillPackFile->text());
	}
}

void CourseEditor::on_markDownShow_stateChanged(int v) {
	if (v == 0) {
		ui->markDownPreview->hide();
		ui->markdownPreviewLabel->hide();
	} else {
		ui->markDownPreview->show();
		ui->markdownPreviewLabel->show();
	}
}
