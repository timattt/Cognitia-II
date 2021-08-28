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
    explicit CourseEditor(QWidget *parent = nullptr);
    ~CourseEditor();
    //--------------------------------------

public:

	// public functions
	//--------------------------------------
	Node* getCurrent();
	//--------------------------------------

private:

	// private functions
	//--------------------------------------
	void clearSkillsLib();
	void clearCourseUnit();
	void fromFileToGui(CourseUnit * crs);
	void fromGuiToFile(CourseUnit * crs);
	void fromCourseUnitToNode(CourseUnit * cu, Node * nd);
	void fromNodeToCourseUnit(Node * nd, CourseUnit * cu);
	void mes(QString mes);
	void setSkillPack(QString path);
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
    QString helpMessage;
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
    void addSkillToLib(QString name, int totalLevels);
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
