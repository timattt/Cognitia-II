#ifndef COURSEUNITVIEWER_H
#define COURSEUNITVIEWER_H

#include <QtWidgets>

class Edge;
class Node;
class CourseScene;

namespace Ui {
class CourseUnitViewer;
}

class CourseUnitViewer : public QWidget
{
    Q_OBJECT

public:

    // Constructor and destructor
    //----------------------------------
    CourseUnitViewer(QWidget *parent);
    ~CourseUnitViewer();
    //----------------------------------

private:

    // Fields
    //----------------------------------
    Ui::CourseUnitViewer *ui;
    CourseScene * scene;
    int timerId = 0;
    double attFac;
    double repFac;
    double massFac;
    //----------------------------------

private:

    // Private functions
    //----------------------------------
    void timerEvent(QTimerEvent * event);
    //----------------------------------

public:

    // Public functions
    //----------------------------------
    bool nodesCanMove();
    bool deleteModeIsOn();
	double getAttFac() const;
	double getMassFac() const;
	double getRepFac() const;
    //----------------------------------
    
private slots:

    // Slots
	//----------------------------------
	void on_pushButton_2_clicked();
	void on_zoomOut_clicked();
	void on_zoomIn_clicked();
	void on_areaDec_clicked();
	void on_areaIn_clicked();
	void on_attFact_textChanged(const QString &arg1);
	void on_repFact_textChanged(const QString &arg1);
	void on_massFac_textChanged(const QString &arg1);
	void on_attFact_editingFinished();
    void on_repFact_editingFinished();
    void on_massFac_editingFinished();
    //----------------------------------

signals:

	// signals
	//----------------------------------
	void nodeAdded(Node *nd);
	void nodeSelected(Node * nd);
	void nodeSkillsChanged(Node * nd);
	//----------------------------------
};
#endif
