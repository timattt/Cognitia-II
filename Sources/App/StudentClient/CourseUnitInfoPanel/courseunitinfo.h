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
//=======================================

class CourseUnitInfo : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//=======================================
    CourseUnitInfo(QWidget *parent);
    ~CourseUnitInfo();
    //=======================================

public:

    // public functions
    //=======================================
    /**
     * Sets client gui this widget works with.
     * May be initialized as soon as posible.
     * @param cl - StudentClient class object.
     * @author timattt
	 */
	void setClient(StudentClient *cl);
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
	//=======================================

private slots:

	// private slots
	//=======================================
	void on_returnBack_clicked();
	//=======================================

private:

	// private fields
    //=======================================
    Ui::CourseUnitInfo *ui;
    StudentClient * client;
    //=======================================

};

#endif // COURSEUNITINFO_H
