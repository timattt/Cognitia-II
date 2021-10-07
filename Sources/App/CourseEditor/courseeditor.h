#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QtWidgets>

// Used classes
//====================================
class SkillsModel;
class Node;
class CourseUnit;
class Skill;
//====================================

// Constants
//====================================
#define SKILL_PACK_UPDATE_TIME 1000
//====================================

namespace Ui {
class CourseEditor;
}

class CourseEditor : public QMainWindow
{
    Q_OBJECT

public:

	// Constructor and destructor
	//--------------------------------------
	/**
	 * Basic constructor.
	 * What it do:
	 * 1. initializes all fields with 0.
	 * 2. initializes modelview for skills table.
	 * 3. connects signals
	 * 4. calls all clear functions
	 * 5. starts skillpack update timer
	 * @author timattt
	 */
    CourseEditor();
    /**
     * Deletes everything
     * @author timattt
     */
    ~CourseEditor();
    //--------------------------------------

public:

	// public functions
	//--------------------------------------
	/**
	 * Gives current node that user is currently maniplating with. it can be head node or node from courseviewer.
	 * @returns current node object
	 * @author timattt
	 */
	Node* getCurrentNode();
	//--------------------------------------

private:

	// private functions
	//--------------------------------------
	/**
	* Fully clears skills lib that was loaded and clears all skill info from gui.
	* Clears everything about skills in this gui.
	* @author timattt
	*/
	void clearSkillsLib();
	/**
	* Fully clears everything about course graph except skillLib and skill information in this gui.
	* @author timattt
	*/
	void clearCourseUnit();
	/**
	 * Transfer all required info from given course unit to this gui.
	 * @param crs - take info from this course unit.
	 * @author timattt
	 */
	void fromFileToGui(CourseUnit * crs);
	/**
	 * Transfer all required info from this gui to courseUnit.
	 * @param crs - destination course unit.
	 * @author timattt
	 */
	void fromGuiToFile(CourseUnit * crs);
	/**
	 * Shows message in status bar.
	 * @param mes - QString to show
	 * @author timattt
	 */
	void mes(QString mes);
	/**
	 * Loads skill pack from given. Loads info into skillsLib and to this gui.
	 * @param path - path to skillpack file.
	 * @author timattt
	 */
	void setSkillPack(QString path);
	/**
	 * Adds skill to skillsLib and also into gui combo box.
	 * @param sk - skill to add.
	 * @author timattt
	 */
	void addSkillToLib(Skill * sk);
	/**
	 * This function sets given node to this gui.
	 * It updates skills table and sets node values to info panels.
	 * @param nd - node to sets into gui.
	 * @author timattt
	 */
    void setNodeToRedactor(Node * nd);
	//--------------------------------------

private:

    // Fields
    //--------------------------------------
	/**
	 * Default ui field.
	 */
    Ui::CourseEditor *ui;
    /**
     * This map contains all info about currently loaded skillpack.
     * {QString: skill name} -> { {int: level number} -> { QString: level description} }
     * @author timattt
     */
    QMap<QString, QMap<int, QString>> skillsLib;
    /**
     * Model that contains income skills for current node.
     * @author timattt
     */
    SkillsModel * inMd;
    /**
     * Model that contains outcome skills for current node.
     * @author timattt
     */
    SkillsModel * outMd;
    /**
     * Node that represent course unit on upper level then units in course unit viewer.
     * It is set to current node when nothing is selected.
     */
    Node * head;
    /**
     * Id of timer that updates skillsLib.
     * So you can parallel edit skillPack and it will be updated by this timer.
     * @author timattt
     */
    int timerId;
    /**
     * Time when skill pack was updated last.
     * @author timattt
     */
    long long lastSkillPackUpdate;
    //--------------------------------------

public slots:

	// public slots
	//--------------------------------------
	/**
	 * This slot sets new node to redactor.
	 * Calls setNodeToRedactor.
	 * @param nd - node selected by course unit viewer.
	 * @author timattt
	 */
	void nodeSelected(Node * nd);
	/**
	 * If node where skills were updated is currently editing by this gui so it will call setNodeToRedactor
	 * function to refresh everything.
	 * @param nd - node where skills changed.
	 * @author timattt
	 */
	void nodeSkillsChanged(Node * nd);
	//--------------------------------------

private slots:

    // private slots
    //--------------------------------------
	///////////////////////////courseunit parameters

	/**
	 * This function update currentNode decription.
	 * And sets new description to markdown panel.
	 * @author timattt
	 */
	void on_descrPanel_textChanged();
	/**
	 * This function update currentNode name.
	 * @author timattt
	 */
    void on_nameLineEdit_textChanged();

    ///////////////////////////drag

    /**
     * This function starts drag of skill and level.
     * To add it into node or course unit skills table.
     * @author timattt
     */
    void on_addSkill_pressed();
    /**
     * This function starts drag of skill and level.
     * To remove it from node or course unit skills table.
     * @author timattt
     */
    void on_removeSkill_pressed();
    /**
     * This will update level selector when skills selector item is changed.
     * @author timattt
     */
    void on_skillsSelector_currentTextChanged(const QString &arg1);
    /**
     * This function will update description panel when level is changed.
     * @author timattt
     */
    void on_levelsSelector_currentTextChanged(const QString &arg1);

    ///////////////////////////skills panel

    /**
     * This function removes currently selected skill from income skills panel (from model).
     * @author timattt
     */
    void on_removeIn_clicked();
	/**
	 * This function removes currently selected skill from outcome skills panel (from model).
	 * @author timattt
	 */
    void on_removeOut_clicked();

    ///////////////////////////courseunit

    void on_actionCourseUnitOpen_triggered();
    void on_actionCourseUnitSave_triggered();
    void on_actionCourseUnitCreate_triggered();
    void on_actionClose_courseUnit_triggered();

    ///////////////////////////skillpack

    void on_actionSkillPackOpen_triggered();
    void on_actionClose_skillPack_triggered();

    ///////////////////////////other panel buttons

    void on_actionReturn_to_launcher_triggered();
    void on_actionHelp_me_triggered();

    ///////////////////////////other

    void on_showParent_clicked();
    void on_markDownShow_stateChanged(int v);
    //--------------------------------------

signals:

	// signals
	//--------------------------------------
	/**
	 * This signal is used by launcher to repopen it after close of this gui.
	 * @author timattt
	 */
	void onClose();
	//--------------------------------------

protected:

	// protected functions
	//--------------------------------------
	/**
	 * Timer function.
	 * If necessary it updates skillsLib.
	 * @author timattt
	 */
	void timerEvent(QTimerEvent *event) override;
	//--------------------------------------
};

#endif // COURSEEDITOR_H
