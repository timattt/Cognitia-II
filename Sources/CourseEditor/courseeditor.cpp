#include "courseeditor.h"
#include "ui_courseeditor.h"

CourseEditor::CourseEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseEditor)
{
    ui->setupUi(this);
}

CourseEditor::~CourseEditor()
{
    delete ui;
}
