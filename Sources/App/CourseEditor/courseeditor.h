#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QtWidgets>

class SkillsModel;
class Node;
class CourseUnit;

namespace Ui {
class CourseEditor;
}

class CourseEditor : public QMainWindow
{
    Q_OBJECT

public:

	// Constructor and destructor
	//--------------------------------------
    CourseEditor();
    ~CourseEditor();
    //--------------------------------------

public:

	// public functions
	//--------------------------------------
    //! Gives node that is editing currently
	Node* getCurrent();
	//--------------------------------------

private:

	// private functions
	//--------------------------------------
	//! Fully clears skills lib that was loaded
	void clearSkillsLib();
	//! Fully clears everything except skillLib
	void clearCourseUnit();
	// Transfers everything from this gui (except skillLib) to CourseUnit object
	void fromFileToGui(CourseUnit * crs);
	//! Transfers everything from this courseUnit to this gui
	void fromGuiToFile(CourseUnit * crs);
	//! Shows message in status bar
	void mes(QString mes);
	//! Loads skill pack and sets it into skillLib
	void setSkillPack(QString path);
	//! Adds skill to lib
	void addSkillToLib(QString name, int totalLevels);
	//--------------------------------------

private:

    // Fields
    //--------------------------------------
    Ui::CourseEditor *ui;
    QMap<QString, int> skillsLib;
    SkillsModel * inMd;
    SkillsModel * outMd;
    Node * head;
    Node * current;
    QString skillPackPath;
    int timerId;
    long long lastSkillPackModified;
    //--------------------------------------

public slots:

	// public slots
	//--------------------------------------
	void nodeSelected(Node * nd);
	void nodeSkillsChanged(Node * nd);
	//--------------------------------------

private slots:

    // private slots
    //--------------------------------------
	void on_descrPanel_textChanged();
    void on_addSkill_pressed();
    void on_removeSkill_pressed();
    void setNodeToRedactor(Node * nd);
    void on_skillsSelector_currentTextChanged(const QString &arg1);
    void on_levelsSelector_currentTextChanged(const QString &arg1);
    void on_showParent_clicked();
    void on_removeIn_clicked();
    void on_removeOut_clicked();
    void on_nameLineEdit_textChanged();
    void on_actionCourseUnitOpen_triggered();
    void on_actionCourseUnitSave_triggered();
    void on_actionCourseUnitCreate_triggered();
    void on_actionSkillPackOpen_triggered();
    void on_actionReturn_to_launcher_triggered();
    void on_actionHelp_me_triggered();
    void on_actionClose_skillPack_triggered();
    void on_actionClose_courseUnit_triggered();
    void on_markDownShow_stateChanged(int v);
    //--------------------------------------

signals:
	// signals
	//--------------------------------------
	void onClose();
	//--------------------------------------

protected:

	// protected functions
	//--------------------------------------
	void timerEvent(QTimerEvent *event) override;
	//--------------------------------------
};

#endif // COURSEEDITOR_H
