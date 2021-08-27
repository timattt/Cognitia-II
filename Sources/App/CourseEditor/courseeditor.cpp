#include "courseeditor.h"
#include "ui_courseeditor.h"
#include "skillsmodel.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../CourseUnitViewer/courseunitviewer.h"
#include "../Structures/CourseUnit/courseunit.h"

CourseEditor::CourseEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CourseEditor),
	current(nullptr),
	head(nullptr)
{
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
}

CourseEditor::~CourseEditor()
{
    delete ui;
}

void CourseEditor::addSkillToLib(QString name, int totalLevels) {
	qInfo() << "put" << name << totalLevels;
	skillsLib[name] = totalLevels;
	ui->skillsSelector->addItem(name);
}

void CourseEditor::on_addSkill_pressed()
{
	bool ok = 0;

	QString name = ui->skillsSelector->currentText();
	int lev = ui->levelsSelector->currentText().toInt(&ok);

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
		for (int i = 1; i <= skillsLib[arg1]; i++) {
			ui->levelsSelector->addItem(QString::number(i));
		}
	}
}

void CourseEditor::on_removeSkill_pressed() {
	bool ok = 0;

	QString name = ui->skillsSelector->currentText();
	int lev = ui->levelsSelector->currentText().toInt(&ok);

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
	if (current != nullptr && current != head) {
		current->setColor(Qt::blue);
	}
	if (nd == nullptr) {
		nd = head;
	}

	current = nd;
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

	ui->nameLineEdit->setText(nd->getName());
	ui->fileLineEdit->setText(nd->getFile());

	nd->setColor(Qt::cyan);
}

void CourseEditor::on_levelsSelector_currentTextChanged(const QString &arg1)
{
}

Node* CourseEditor::getCurrent() {
	return current;
}

void CourseEditor::on_showParent_clicked() {
	setNodeToRedactor(head);
}

void CourseEditor::on_removeIn_clicked() {
	int row = ui->inList->currentIndex().row();
	QString name = inMd->data(inMd->index(row, 0)).toString();
	current->removeInSkill(name);
	inMd->removeRow(row);
}

void CourseEditor::on_removeOut_clicked() {
	int row = ui->outList->currentIndex().row();
	QString name = outMd->data(outMd->index(row, 0)).toString();
	current->removeOutSkill(name);
	outMd->removeRow(row);
}

void CourseEditor::on_nameLineEdit_editingFinished() {
	current->setName(ui->nameLineEdit->text());
}

void CourseEditor::nodeSelected(Node *nd) {
	setNodeToRedactor(nd);
}

void CourseEditor::nodeSkillsChanged(Node *nd) {
	setNodeToRedactor(nd);
}

void CourseEditor::on_fileLineEdit_editingFinished() {
	current->setFile(ui->fileLineEdit->text());
}

void CourseEditor::on_actionCourseUnitOpen_triggered() {
	QString path = QFileDialog::getOpenFileName(this, "Select skill pack file");
	clearCourseUnit();

	QFile f = QFile(path);

	if (!f.exists()) {
		return;
	}

	current->setFile(path);

	CourseUnit crs;
	crs.loadCourseUnit(&f);
}

void CourseEditor::on_actionCourseUnitSave_triggered() {
}

void CourseEditor::on_actionCourseUnitCreate_triggered() {
}

void CourseEditor::clearSkillsLib() {
	skillsLib.clear();
	ui->skillsSelector->clear();
	ui->levelsSelector->clear();
}

void CourseEditor::clearCourseUnit() {
	head->setName("Parent course unit");
	head->setFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "NEW_COURSE_UNIT.cognitiaCourseUnit");
	head->clearSkills();
	setNodeToRedactor(head);
	ui->widget->clearAllScene();
}

void CourseEditor::fromFileToGui(CourseUnit *crs) {
}

void CourseEditor::fromGuiToFile(CourseUnit *crs) {
}

void CourseEditor::on_actionSkillPackOpen_triggered() {
	skillPackPath = QFileDialog::getOpenFileName(this, "Select skill pack file");
	clearSkillsLib();

	ui->skillPackFile->setText(skillPackPath);
	SkillPack skp;
	QFile f = QFile(skillPackPath);
	if (!f.exists()) {
		return;
	}
	skp.load(&f);
	for (int i = 0; i < skp.getSkillsCount(); i++) {
		addSkillToLib(skp.getSkill(i)->objectName(), skp.getSkill(i)->getLevelsCount());
	}
}
