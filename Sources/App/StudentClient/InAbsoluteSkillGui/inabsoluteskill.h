#ifndef INABSOLUTESKILL_H
#define INABSOLUTESKILL_H

#include <QWidget>

namespace Ui {
class InAbsoluteSkill;
}

class Node;
class StudentProgress;
class CourseUnit;

class InAbsoluteSkill : public QWidget
{
    Q_OBJECT

public:

    InAbsoluteSkill(QWidget *parent);
    ~InAbsoluteSkill();

public slots:

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
     * Unpacks to this gui single node.
     * This will be called when some node is selected.
     * If argument are nullptr then it will only clears everything.
     * @param nd - node that will be unpacked. Can be nullptr.
     * @author timattt
     */
    void unpack(Node * nd);

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

    Ui::InAbsoluteSkill *ui;
};

#endif // INABSOLUTESKILL_H
