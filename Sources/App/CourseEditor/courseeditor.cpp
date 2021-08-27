#include "courseeditor.h"
#include "ui_courseeditor.h"
#include "skillsmodel.h"
#include "../Structures/SkillPack/skillpack.h"

CourseEditor::CourseEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CourseEditor)
{
    ui->setupUi(this);

    addSkill("TestSkill 2", 2);
    addSkill("TestSkill 3", 3);

    inMd = new SkillsModel(this);
    outMd = new SkillsModel(this);

    ui->inList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->outList->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->inList->setAcceptDrops(true);
	ui->inList->setDropIndicatorShown(true);

	ui->outList->setAcceptDrops(true);
	ui->outList->setDropIndicatorShown(true);

	ui->inList->setModel(inMd);
    ui->outList->setModel(outMd);
}

CourseEditor::~CourseEditor()
{
	delete inMd;
	delete outMd;
    delete ui;
}

void CourseEditor::addSkill(QString name, int totalLevels) {
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

void CourseEditor::on_levelsSelector_currentTextChanged(const QString &arg1)
{
	bool ok = 0;

	QString name = ui->skillsSelector->currentText();
	int lev = ui->levelsSelector->currentText().toInt(&ok);

	if (!ok || !skillsLib.contains(name)) {
		return;
	}

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText(ui->skillsSelector->currentText() + '\0' + ui->levelsSelector->currentText());

    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/icons/Icons/SkillDrag.png"));

    drag->exec();
}

