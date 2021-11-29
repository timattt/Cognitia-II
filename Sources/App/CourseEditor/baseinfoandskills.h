#ifndef BASEINFOANDSKILLS_H
#define BASEINFOANDSKILLS_H

#include <QWidget>

namespace Ui {
class BaseInfoAndSkills;
}

class SkillsModel;
class CourseEditor;
class Node;

class BaseInfoAndSkills : public QWidget
{
    Q_OBJECT

public:
    explicit BaseInfoAndSkills(QWidget *parent = nullptr);
    ~BaseInfoAndSkills();

private:
    Ui::BaseInfoAndSkills *ui;
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
    CourseEditor * editor;

public slots:

	void nodeSelected(Node * nd);
	void ensureLocked();
	void unlock();

private:

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
    /**
     * Sets parent node to gui.
     * Selects nullptr node in course unit viewer.
     *
     * @author timattt
     */
    void on_showParent_clicked();
	/**
	 * This function update currentNode name.
	 * @author timattt
	 */
    void on_nameLineEdit_textChanged();
};

#endif // BASEINFOANDSKILLS_H
