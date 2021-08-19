#ifndef COURSEUNITVIEWER_H
#define COURSEUNITVIEWER_H

#include <QWidget>

namespace Ui {
class CourseUnitViewer;
}

class CourseUnitViewer : public QWidget
{
    Q_OBJECT

public:
    explicit CourseUnitViewer(QWidget *parent = nullptr);
    ~CourseUnitViewer();

private:
    Ui::CourseUnitViewer *ui;
};

#endif // COURSEUNITVIEWER_H
