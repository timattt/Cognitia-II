#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QWidget>

namespace Ui {
class CourseEditor;
}

class CourseEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CourseEditor(QWidget *parent = nullptr);
    ~CourseEditor();

private:
    Ui::CourseEditor *ui;
};

#endif // COURSEEDITOR_H
