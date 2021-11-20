/*
 * outputrelativeskillspanel.h
 *
 *  Created on: Nov 19, 2021
 *      Author: ZoomZero
 */

#ifndef OUTDELTASKILLS_H
#define OUTDELTASKILLS_H

#include <QWidget>
#include <QMap>

namespace Ui {
class OutputRelativeSkillsPanel;
}

class Node;
class StudentProgress;
class CourseUnit;
class OutCircleScene;
class OutCircle;

class OutputRelativeSkillsPanel : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//===============================================
    OutputRelativeSkillsPanel(QWidget *parent);
    ~OutputRelativeSkillsPanel();
    //===============================================

private:

    // private fields
    //===============================================
    Ui::OutputRelativeSkillsPanel *ui;
    StudentProgress * currentProgress;
    OutCircleScene * scene;
    QMap<QString, OutCircle*> circles;
    //===============================================

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
     * @author ZoomZero
	 */
	void progressMade(QString skill, double lev);

	/**
	 * Unpacks single node to this gui.
	 * This will be called when some node is selected.
	 * Can be nullptr if no node is selected.
	 * @param nd - node that will be unpacked. Can be nullptr.
     * @author ZoomZero
	 */
	void nodeSelected(Node *nd);
	//==============================================

protected:

    // protected functions
    //==============================================
    void resizeEvent(QResizeEvent * event);
    //==============================================
};

#endif // OUTDELTASKILLS_H
