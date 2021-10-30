#ifndef INABSOLUTESKILL_H
#define INABSOLUTESKILL_H

#include <QWidget>

namespace Ui {
class InputAbsoluteSkillsPanel;
}

// used classes
//==============================================
class Node;
class StudentProgress;
class CourseUnit;
//==============================================

/**
 * This class is used to show all income skills and student progress in it for given node.
 *
 */
class InputAbsoluteSkillsPanel : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//==============================================
    InputAbsoluteSkillsPanel(QWidget *parent);
    ~InputAbsoluteSkillsPanel();
    //==============================================

public slots:

	// public slots
	//==============================================
	/**
	 * Clears everything from this widget.
	 * @author timattt
	 */
	void clearAll();
	/**
	 * This will be called when progress of current course unit is changed.
	 * @param skill - skill that is changed.
	 * @param lev - new skill progress.
	 * @author timattt
	 */
	void progressMade(QString skill, double lev);

    /**
     * Unpacks single node to this gui.
     * This will be called when some node is selected.
     * Can be nullptr if no node is selected.
     * @param nd - node that will be unpacked. Can be nullptr.
     * @author timattt
     */
    void nodeSelected(Node * nd);
    //==============================================

signals:

	// signals
	//====================================================
	/**
	 * This will be emited when user changed skill or progress.
	 * So it may be connected to course unit viewer to update data there.
	 * @param skill - skill that is changed.
	 * @param val - new skill progress.
	 * @author timattt
	 */
	void skillLevelChanged(QString skillName, double val);
	//====================================================

private:

	// private functions
	//==============================================
    Ui::InputAbsoluteSkillsPanel *ui;
    //==============================================

};

#endif // INABSOLUTESKILL_H
