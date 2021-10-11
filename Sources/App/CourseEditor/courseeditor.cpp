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
	skillsLib(),
	inMd(nullptr),
	outMd(nullptr),
	head(nullptr),
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
	ensureCourseUnitIsLocked();
	//-----------------------------

	timerId = startTimer(SKILL_PACK_UPDATE_TIME);

	SAY("CourseEditor init finished!");
}

CourseEditor::~CourseEditor()
{
	ensureCourseUnitIsLocked();
	clearSkillsLib();
    delete ui;
    killTimer(timerId);
    if (head != nullptr) {
    	delete head;
    }
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

bool CourseEditor::isChanged() {
	if (head == nullptr) {
		return false;
	}
	CourseUnit cu;
	fromGuiToFile(&cu);
	return fileSignature.size() > 0 && QString::compare(fileSignature, cu.print(), Qt::CaseInsensitive);
}

void CourseEditor::on_showParent_clicked() {
	if (!checkCourseUnitAvailable(true)) {
		return;
	}

	ui->widget->setSelectedNode(nullptr);
	setNodeToRedactor(head);
}

void CourseEditor::on_removeIn_clicked() {
	if (!checkCourseUnitAvailable(true)) {
		return;
	}

	int row = ui->inList->currentIndex().row();
	SAY(QString::number(row));
	QString name = inMd->data(inMd->index(row, 0)).toString();
	getCurrentNode()->removeInSkill(name);
	inMd->removeSkill(name);
}

void CourseEditor::on_removeOut_clicked() {
	if (!checkCourseUnitAvailable(true)) {
		return;
	}

	int row = ui->outList->currentIndex().row();
	QString name = outMd->data(outMd->index(row, 0)).toString();
	getCurrentNode()->removeOutSkill(name);
	outMd->removeSkill(name);
}

void CourseEditor::on_nameLineEdit_textChanged() {
	if (!checkCourseUnitAvailable(false)) {
		return;
	}
	getCurrentNode()->setName(ui->nameLineEdit->text());
}

void CourseEditor::nodeSelected(Node *nd) {
	if (!head) {
		return;
	}

	if (nd == nullptr) {
		nd = head;
	}
	setNodeToRedactor(nd);
}
void CourseEditor::nodeSkillsChanged(Node *nd) {
	if (!checkCourseUnitAvailable(false)) {
		return;
	}
	if (nd == getCurrentNode()) {
		setNodeToRedactor(nd);
	}
}

void CourseEditor::on_actionCourseUnitOpen_triggered() {
	ensureCourseUnitIsLocked();

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
		ensureCourseUnitIsLocked();
	}


}

void CourseEditor::on_actionCourseUnitSave_triggered() {
	if (!checkCourseUnitAvailable(true)) {
		return;
	}

	on_showParent_clicked();

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

	setNodeToRedactor(head);
}

void CourseEditor::on_actionCourseUnitCreate_triggered() {
	ensureCourseUnitIsLocked();

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
		ensureCourseUnitIsLocked();
	}

}

void CourseEditor::clearSkillsLib() {
	skillsLib.clear();
	ui->skillsSelector->clear();
	ui->levelsSelector->clear();
	ui->skillPackFile->clear();
}

void CourseEditor::ensureCourseUnitIsLocked() {
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

	ui->addSkill->setEnabled(false);
	ui->removeSkill->setEnabled(false);
	ui->removeIn->setEnabled(false);
	ui->removeOut->setEnabled(false);
	ui->showParent->setEnabled(false);
	ui->widget->setEditable(false);
	ui->inList->setEnabled(false);
	ui->outList->setEnabled(false);
	ui->nameLineEdit->setEnabled(false);
	ui->descrPanel->setEnabled(false);
	ui->widget->clearAllScene();
	inMd->clear();
	outMd->clear();
	ui->descrPanel->clear();
	ui->nameLineEdit->clear();
	ui->fileLineEdit->clear();
	ui->markDownPreview->clear();
	fileSignature = "";

	if (head != nullptr) {
		delete head;
		head = nullptr;
	}
}

void CourseEditor::fromFileToGui(CourseUnit *crs) {
	NOT_NULL(crs);
	NOT_NULL(head);

	fromCourseUnitToNode(crs, head);

	ui->widget->unpack(crs);

	setNodeToRedactor(head);
}

void CourseEditor::fromGuiToFile(CourseUnit *crs) {
	NOT_NULL(crs);
	NOT_NULL(head);

	fromNodeToCourseUnit(head, crs);

	ui->widget->pack(crs);
}

void CourseEditor::on_actionSkillPackOpen_triggered() {
	on_actionClose_skillPack_triggered();

	QString path = QFileDialog::getOpenFileName(this, "Select skill pack file", QString(), QString("(*") + SKILL_PACK_FILE_EXTENSION + QString(")"));

	if (path.length() == 0) {
		return;
	}

	setSkillPack(path);
}

void CourseEditor::on_actionReturn_to_launcher_triggered() {
	ensureCourseUnitIsLocked();
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
	ensureCourseUnitIsLocked();
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
	if (!checkCourseUnitAvailable(false)) {
		return;
	}
	getCurrentNode()->setDescription(ui->descrPanel->toPlainText());
	ui->markDownPreview->setMarkdown(ui->descrPanel->toPlainText());
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
	ui->inList->setEnabled(true);
	ui->outList->setEnabled(true);
	ui->nameLineEdit->setEnabled(true);
	ui->descrPanel->setEnabled(true);
	ui->addSkill->setEnabled(true);
	ui->removeSkill->setEnabled(true);
	ui->removeIn->setEnabled(true);
	ui->removeOut->setEnabled(true);
	ui->showParent->setEnabled(true);
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
