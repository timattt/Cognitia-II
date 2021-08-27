#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class CourseEditor;
}

class CourseEditor : public QMainWindow
{
    Q_OBJECT

public:

	// Constructor and destructor
	//--------------------------------------
    explicit CourseEditor(QWidget *parent = nullptr);
    ~CourseEditor();
    //--------------------------------------

private:

    // Fields
    //--------------------------------------
    Ui::CourseEditor *ui;
    QMap<QString, int> skillsLib;
    //--------------------------------------

private slots:

    // Slots
    //--------------------------------------
    void addSkill(QString name, int totalLevels);
    void on_pushButton_pressed();
    void on_skillsSelector_currentTextChanged(const QString &arg1);
    void on_levelsSelector_currentTextChanged(const QString &arg1);
    //--------------------------------------

};

#endif // COURSEEDITOR_H
