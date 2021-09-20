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

CourseEditor::CourseEditor() :
    QMainWindow(nullptr),
    ui(new Ui::CourseEditor),
	head(nullptr),
	lastSkillPackModified(0)
{
	qInfo() << "CourseEditor init started";
    ui->setupUi(this);

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

    head = new Node(nullptr);

    setNodeToRedactor(head);

    connect(ui->widget, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));
    connect(ui->widget, SIGNAL(nodeSkillsChanged(Node*)), this, SLOT(nodeSkillsChanged(Node*)));

    clearSkillsLib();
    clearCourseUnit();

    timerId = startTimer(1000);

    qInfo() << "CourseEditor init finished!";
}

CourseEditor::~CourseEditor()
{
	clearCourseUnit();
	clearSkillsLib();
    delete ui;
    killTimer(timerId);
}

void CourseEditor::addSkillToLib(QString name, Skill * sk) {
	skillsLib[name] = QMap<int, QString>();
	for (int i = 0; i < sk->getLevelsCount(); i++) {
		skillsLib[name][i + 1] = sk->getLevelDescription(i);
	}
	ui->skillsSelector->addItem(name);
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
	if (nd == nullptr) {
		nd = head;
	}

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

	QString name = nd->getName();
	QString file = nd->getFile();
	QString descr = nd->getDescription();

	ui->nameLineEdit->setText(name);
	ui->fileLineEdit->setText(file);
	ui->descrPanel->setText(descr);
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
	setNodeToRedactor(nd);
}
void CourseEditor::nodeSkillsChanged(Node *nd) {
	setNodeToRedactor(nd);
}

void CourseEditor::on_actionCourseUnitOpen_triggered() {
	QString path = QFileDialog::getOpenFileName(this, "Select course unit file", QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));
	clearCourseUnit();

	QFile f = QFile(path);

	if (!f.exists()) {
		return;
	}

	clearCourseUnit();

	head->setFile(path);

	CourseUnit crs;
	try {
		crs.loadCourseUnit(&f);
		fromFileToGui(&crs);

		mes("Opened course unit file " + path);
	} catch (QString & ex) {
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
		mes("Error while saving: " + ex);
	}
}

void CourseEditor::on_actionCourseUnitCreate_triggered() {
	QString path = QFileDialog::getSaveFileName(this, "Create course unit file", QString(), QString("(*") + COURSE_UNIT_FILE_EXTENSION + QString(")"));

	QFile f = QFile(path);
	QFileInfo in = QFileInfo(f);

	clearCourseUnit();

	head->setName(in.fileName());
	head->setFile(path);

	CourseUnit crs;

	try {
		fromGuiToFile(&crs);
		crs.saveCourseUnit(&f);
		mes("Created course unit file " + path);
		fromFileToGui(&crs);
	} catch (QString err) {
		mes("Error wjile creating: " + err);
	}

}

void CourseEditor::clearSkillsLib() {
	skillsLib.clear();
	ui->skillsSelector->clear();
	ui->levelsSelector->clear();
	ui->skillPackFile->clear();
	skillPackPath.clear();
}

void CourseEditor::clearCourseUnit() {
	head->setName("Parent course unit");
	head->setFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QString("/NEW_COURSE_UNIT") + COURSE_UNIT_FILE_EXTENSION);
	head->clearSkills();
	setNodeToRedactor(head);
	ui->widget->clearAllScene();
	ui->descrPanel->clear();
}

void CourseEditor::fromFileToGui(CourseUnit *crs) {
	fromCourseUnitToNode(crs, head);

	ui->widget->unpack(crs);

	setNodeToRedactor(head);
}

void CourseEditor::fromGuiToFile(CourseUnit *crs) {
	fromNodeToCourseUnit(head, crs);

	crs->setFieldSize(ui->widget->getSceneSize().x(), ui->widget->getSceneSize().y());

	ui->widget->pack(crs);
}

void CourseEditor::on_actionSkillPackOpen_triggered() {
	clearSkillsLib();
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
		mes("SkillPack file " + skillPackPath + " does not exists!");
		return;
	}

	clearSkillsLib();
	ui->skillPackFile->setText(path);

	skp.load(&f);
	for (int i = 0; i < skp.getSkillsCount(); i++) {
		addSkillToLib(skp.getSkill(i)->objectName(), skp.getSkill(i));
	}

	mes("SkillPack file " + path + " is loaded!");
	skillPackPath = path;

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

	QFile f = QFile(this->skillPackPath);
	if (!f.exists()) {
		return;
	}
	QFileInfo in = QFileInfo(f);

	long long t = in.lastModified().toMSecsSinceEpoch();

	if (lastSkillPackModified < t) {
		lastSkillPackModified = t;
		setSkillPack(skillPackPath);
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
