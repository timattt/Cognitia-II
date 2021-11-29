#include "skillschooser.h"
#include "ui_skillschooser.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/SkillPack/skill.h"
#include "../Core/logger.h"
#include "../CourseUnitViewer/Node/node.h"
#include "courseeditor.h"

SkillsChooser::SkillsChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillsChooser),
	editor(nullptr),
	timerId(0),
	lastSkillPackUpdate(0)
{
    ui->setupUi(this);

    editor = dynamic_cast<CourseEditor*>(parent->parent()->parent()->parent());

    NOT_NULL(editor);

    timerId = startTimer(SKILL_PACK_UPDATE_TIME);
}

SkillsChooser::~SkillsChooser()
{
    killTimer(timerId);
    delete ui;
}


void SkillsChooser::addSkillToLib(Skill * sk) {
	NOT_NULL(sk);

	skillsLib[sk->objectName()] = QMap<int, QString>();
	for (int i = 0; i < sk->getLevelsCount(); i++) {
		skillsLib[sk->objectName()][i + 1] = sk->getLevelDescription(i);
	}
	ui->skillsSelector->addItem(sk->objectName());
}

void SkillsChooser::on_addSkill_pressed()
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


void SkillsChooser::on_skillsSelector_currentTextChanged(const QString &arg1)
{
	ui->levelsSelector->clear();
	if (skillsLib.contains(arg1)) {
		for (int i = 1; i <= skillsLib[arg1].size(); i++) {
			ui->levelsSelector->addItem(QString::number(i));
		}
	}
}

void SkillsChooser::on_removeSkill_pressed() {
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


void SkillsChooser::on_levelsSelector_currentTextChanged(const QString &arg1)
{
	Q_UNUSED(arg1);

	if (skillsLib.contains(ui->skillsSelector->currentText()) &&
			skillsLib[ui->skillsSelector->currentText()].contains(ui->levelsSelector->currentText().toInt())) {
		ui->levelDescription->setText(skillsLib[ui->skillsSelector->currentText()][ui->levelsSelector->currentText().toInt()]);
	} else {
		ui->levelDescription->clear();
	}
}

void SkillsChooser::clearSkillsLib() {
	skillsLib.clear();
	ui->skillsSelector->clear();
	ui->levelsSelector->clear();
	ui->skillPackFile->clear();
}

void SkillsChooser::setSkillPack(QString path) {
	int i1 = ui->skillsSelector->currentIndex();
	int i2 = ui->levelsSelector->currentIndex();

	SkillPack skp;
	QFile f = QFile(path);

	if (!f.exists()) {
		editor->mes("SkillPack file " + path + " does not exists!");
		QMessageBox::critical(this, "Error", "SkillPack file " + path + " does not exists!");
		return;
	}

	clearSkillsLib();
	ui->skillPackFile->setText(path);

	skp.load(&f);
	for (int i = 0; i < skp.getSkillsCount(); i++) {
		addSkillToLib(skp.getSkill(i));
	}

	editor->mes("SkillPack file " + path + " is loaded!");

	ui->skillsSelector->setCurrentIndex(i1);
	ui->levelsSelector->setCurrentIndex(i2);
}

void SkillsChooser::nodeSelected(Node *nd) {
	Q_UNUSED(nd);
}

void SkillsChooser::timerEvent(QTimerEvent *event) {
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

void SkillsChooser::ensureLocked() {
	ui->addSkill->setEnabled(false);
	ui->removeSkill->setEnabled(false);
}

void SkillsChooser::unlock() {
	ui->addSkill->setEnabled(true);
	ui->removeSkill->setEnabled(true);
}
