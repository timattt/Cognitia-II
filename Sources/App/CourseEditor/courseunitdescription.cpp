#include "courseunitdescription.h"
#include "ui_courseunitdescription.h"
#include "../CourseUnitViewer/Node/node.h"
#include "courseeditor.h"
#include "../Core/logger.h"

CourseUnitDescription::CourseUnitDescription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitDescription),
	editor(nullptr)
{
    ui->setupUi(this);

    editor = dynamic_cast<CourseEditor*>(parent->parent()->parent()->parent());

    NOT_NULL(editor);
}

CourseUnitDescription::~CourseUnitDescription()
{
    delete ui;
}

void CourseUnitDescription::ensureLocked() {
	ui->descrPanel->setEnabled(false);
	ui->descrPanel->clear();
	ui->markDownPreview->clear();
}

void CourseUnitDescription::unlock() {
	ui->descrPanel->setEnabled(true);
}

void CourseUnitDescription::on_markDownShow_stateChanged(int v) {
	if (v == 0) {
		ui->markDownPreview->hide();
		ui->markdownPreviewLabel->hide();
	} else {
		ui->markDownPreview->show();
		ui->markdownPreviewLabel->show();
	}
}

void CourseUnitDescription::on_descrPanel_textChanged() {
	if (!editor->checkCourseUnitAvailable(false)) {
		return;
	}
	editor->getCurrentNode()->setDescription(ui->descrPanel->toPlainText());
	ui->markDownPreview->setMarkdown(ui->descrPanel->toPlainText());
}

void CourseUnitDescription::nodeSelected(Node *nd) {
	ui->descrPanel->clear();
	if (nd != nullptr) {
		ui->descrPanel->setText(nd->getDescription());
	}
}
