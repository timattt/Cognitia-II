#include "launcher.h"
#include "ui_launcher.h"

Launcher::Launcher() :
    QWidget(nullptr),
    ui(new Ui::Launcher),
	mayHideWhenUnused(true)
{
    ui->setupUi(this);

    this->show();
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::on_runButton_clicked()
{
     if (ui->modesChooser->currentText() == "Skill pack editor") {
        emit startSkillPackEditor();
        if (mayHideWhenUnused) {
        	hide();
        }
     }

     else if (ui->modesChooser->currentText() == "Course editor") {
        emit startCourseEditor();
        if (mayHideWhenUnused) {
        	hide();
        }
     }

     else if (ui->modesChooser->currentText() == "Cognitia repository") {
        emit startServer();
        if (mayHideWhenUnused) {
            hide();
        }
     }

     else if (ui->modesChooser->currentText() == "Student client") {
        emit startStudentClient();
        if (mayHideWhenUnused) {
            hide();
        }
     }

}

void Launcher::on_keepLauncherOpenCheckbox_stateChanged(int v) {
	if (v == 0) {
		// unchecked
		mayHideWhenUnused = true;
	} else {
		// checked
		mayHideWhenUnused = false;
	}
}
