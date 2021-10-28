#ifndef COURSEUNITINFO_H
#define COURSEUNITINFO_H

#include <QWidget>

namespace Ui {
class CourseUnitInfo;
}

// used classes
//=======================================
class StudentClient;
class Node;
class StudentProgress;
//=======================================

class CourseUnitDetailsPanel : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//=======================================
    CourseUnitDetailsPanel(QWidget *parent);
    ~CourseUnitDetailsPanel();
    //=======================================

public slots:

	// public slots
	//=======================================
	/**
	 * This slot is invoked when user double click some node.
	 * @param nd - node to be set in this widget.
	 * @author timattt
	 */
	void prepareNode(Node * nd);
	void clearAll();
	//=======================================

private:

	// private fields
    //=======================================
    Ui::CourseUnitInfo *ui;
    //=======================================

};

#endif // COURSEUNITINFO_H
