#include "courseunitviewer.h"
#include "ui_courseunitviewer.h"

CourseUnitViewer::CourseUnitViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitViewer)
{
    ui->setupUi(this);
}

CourseUnitViewer::~CourseUnitViewer()
{
    delete ui;
}
