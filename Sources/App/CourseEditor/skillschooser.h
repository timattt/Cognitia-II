#ifndef SKILLSCHOOSER_H
#define SKILLSCHOOSER_H

#include <QtWidgets>

namespace Ui {
class SkillsChooser;
}

class CourseEditor;
class Skill;
class Node;

class SkillsChooser : public QWidget
{
    Q_OBJECT

public:
    explicit SkillsChooser(QWidget *parent = nullptr);
    ~SkillsChooser();

private:

    Ui::SkillsChooser *ui;
    CourseEditor * editor;
    /**
     * This map contains all info about currently loaded skillpack.
     * {QString: skill name} -> { {int: level number} -> { QString: level description} }
     * @author timattt
     */
    QMap<QString, QMap<int, QString>> skillsLib;
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

public slots:

	void nodeSelected(Node * nd);
	/**
	* Fully clears skills lib that was loaded and clears all skill info from gui.
	* Clears everything about skills in this gui.
	* @author timattt
	*/
	void clearSkillsLib();
	/**
	 * Adds skill to skillsLib and also into gui combo box.
	 * @param sk - skill to add.
	 * @author timattt
	 */
	void addSkillToLib(Skill * sk);
	/**
	 * Loads skill pack from given. Loads info into skillsLib and to this gui.
	 * @param path - path to skillpack file.
	 * @author timattt
	 */
	void setSkillPack(QString path);
	void ensureLocked();
	void unlock();

private slots:

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

protected:

	/**
	 * Timer function.
	 * If necessary it updates skillsLib.
	 * @author timattt
	 */
	void timerEvent(QTimerEvent *event) override;

};

#endif // SKILLSCHOOSER_H
