#include "courseeditor.h"
#include "ui_courseeditor.h"
#include "skillsmodel.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../CourseUnitViewer/courseunitviewer.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../CourseUnitViewer/Node/edge.h"
#include "../Help/smarthelper.h"

CourseEditor::CourseEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CourseEditor),
	current(nullptr),
	head(nullptr),
	lastSkillPackModified(0)
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

    clearSkillsLib();
    clearCourseUnit();

    timerId = startTimer(1000);
}

CourseEditor::~CourseEditor()
{
    delete ui;
    killTimer(timerId);
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

void CourseEditor::on_nameLineEdit_textChanged() {
	current->setName(ui->nameLineEdit->text());
}

void CourseEditor::nodeSelected(Node *nd) {
	setNodeToRedactor(nd);
}
void CourseEditor::nodeSkillsChanged(Node *nd) {
	setNodeToRedactor(nd);
}

void CourseEditor::on_actionCourseUnitOpen_triggered() {
	QString path = QFileDialog::getOpenFileName(this, "Select course unit file");
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
	} catch (QString ex) {
		mes("Error while opening: " + ex);
	}


}

void CourseEditor::on_actionCourseUnitSave_triggered() {
	QFile f = QFile(head->getFile());

	CourseUnit crs;
	fromGuiToFile(&crs);

	try {
		crs.saveCourseUnit(&f);
		clearCourseUnit();//
		fromFileToGui(&crs);// to insure that it will write file names into panels
		mes("Saved course unit file " + head->getFile());
	} catch (QString ex) {
		mes("Error while saving: " + ex);
	}
}

void CourseEditor::on_actionCourseUnitCreate_triggered() {
	QString path = QFileDialog::getSaveFileName(this, "Create course unit file");

	QFile f = QFile(path);
	QFileInfo in = QFileInfo(f);

	clearCourseUnit();

	head->setName(in.fileName());
	head->setFile(path);

	CourseUnit crs;
	fromGuiToFile(&crs);

	try {
		crs.saveCourseUnit(&f);
		mes("Created course unit file " + path);
	} catch (QString err) {
		mes("Error wjile creating: " + err);
	}

	fromFileToGui(&crs);
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
	head->setFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/NEW_COURSE_UNIT.cognitiaCourseUnit");
	head->clearSkills();
	setNodeToRedactor(head);
	ui->widget->clearAllScene();
}

void CourseEditor::fromFileToGui(CourseUnit *crs) {
	fromCourseUnitToNode(crs, head);

	QMap<QString, Node*> nodes;

	for (CourseUnit * u : crs->getEmbedded()) {
		Node * nd = new Node(ui->widget);
		fromCourseUnitToNode(u, nd);
		nodes[nd->getName()] = nd;
		ui->widget->addNode(nd);
	}

	for (CourseUnit * u : crs->getEmbedded()) {
		for (QString v : u->getConnections()) {
			ui->widget->addEdge(new Edge(nodes[u->objectName()], nodes[v]));
		}
	}

	setNodeToRedactor(head);
}

void CourseEditor::fromGuiToFile(CourseUnit *crs) {
	fromNodeToCourseUnit(head, crs);

	QMap<QString, CourseUnit*> units;

	for (QGraphicsItem * it : ui->widget->getAllItems()) {
		if (!it) {
			continue;
		}

		Node * nd = dynamic_cast<Node*>(it);

		if (nd != nullptr) {
			CourseUnit * un = new CourseUnit(crs);
			crs->addEmbedded(un);
			fromNodeToCourseUnit(nd, un);
			units[un->objectName()] = un;
		}
	}

	for (QGraphicsItem * it : ui->widget->getAllItems()) {
		if (!it) {
			continue;
		}

		Edge * ed = dynamic_cast<Edge*>(it);
		if (ed != nullptr) {
			units[ed->sourceNode()->getName()]->addConnection(ed->destNode()->getName());
			units[ed->destNode()->getName()]->addConnection(ed->sourceNode()->getName());
		}
	}
}

void CourseEditor::fromNodeToCourseUnit(Node *nd, CourseUnit *cu) {
	cu->setObjectName(nd->getName());

	for (QString sk : nd->getInSkills().keys()) {
		int lev = nd->getInSkills()[sk];

		cu->addIncome({sk, lev});
	}

	for (QString sk : nd->getOutSkills().keys()) {
		int lev = nd->getOutSkills()[sk];

		cu->addOutcome({sk, lev});
	}

	cu->setCoords((long long)nd->pos().x(), (long long)nd->pos().y());
}

void CourseEditor::fromCourseUnitToNode(CourseUnit *cu, Node *nd) {
	nd->setName(cu->objectName());
	nd->setFile(cu->getLastFilePath());

	for (std::pair<QString, size_t> in : cu->getIncome()) {
		nd->addInSkill(in.first, in.second);
	}

	for (std::pair<QString, size_t> out : cu->getOutcome()) {
		nd->addOutSkill(out.first, out.second);
	}

	nd->setPos(cu->getCoords().first, cu->getCoords().second);
}

void CourseEditor::on_actionSkillPackOpen_triggered() {
	clearSkillsLib();
	QString path = QFileDialog::getOpenFileName(this, "Select skill pack file");

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
		addSkillToLib(skp.getSkill(i)->objectName(), skp.getSkill(i)->getLevelsCount());
	}

	mes("SkillPack file " + path + " is loaded!");
	skillPackPath = path;

	ui->skillsSelector->setCurrentIndex(i1);
	ui->levelsSelector->setCurrentIndex(i2);
}

void CourseEditor::timerEvent(QTimerEvent *event) {
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
