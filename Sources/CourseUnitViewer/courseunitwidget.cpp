#include "courseunitwidget.h"
#include "ui_courseunitwidget.h"

CourseUnitWidget::CourseUnitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitWidget)
{
    ui->setupUi(this);
}

CourseUnitWidget::~CourseUnitWidget()
{
    delete ui;
}
