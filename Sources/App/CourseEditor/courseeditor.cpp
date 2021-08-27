#include "courseeditor.h"
#include "skillselector.h"
#include "ui_courseeditor.h"

CourseEditor::CourseEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CourseEditor)
{
    ui->setupUi(this);

    addSkill("TestSkill 2", 2);
    addSkill("TestSkill 3", 3);
}

CourseEditor::~CourseEditor()
{
    delete ui;
}

void CourseEditor::addSkill(QString name, int totalLevels) {
	skillsLib[name] = totalLevels;
	ui->skillsSelector->addItem(name);
}

void CourseEditor::on_pushButton_pressed()
{
	bool ok = 0;

	QString name = ui->skillsSelector->currentText();
	int lev = ui->levelsSelector->currentText().toInt(&ok);

	if (!ok || !skillsLib.contains(name)) {
		return;
	}

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setText(ui->skillsSelector->currentText() + "|" + ui->levelsSelector->currentText());

    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/icons/Icons/SkillDrag.png"));

    Qt::DropAction dropAction = drag->exec();
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


void CourseEditor::on_levelsSelector_currentTextChanged(const QString &arg1)
{

}

