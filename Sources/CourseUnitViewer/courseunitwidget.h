#ifndef COURSEUNITWIDGET_H
#define COURSEUNITWIDGET_H

#include <QWidget>

namespace Ui {
class CourseUnitWidget;
}

class CourseUnitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CourseUnitWidget(QWidget *parent = nullptr);
    ~CourseUnitWidget();

private:
    Ui::CourseUnitWidget *ui;
};

#endif // COURSEUNITWIDGET_H
