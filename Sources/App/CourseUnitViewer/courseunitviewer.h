#ifndef COURSEUNITVIEWER_H
#define COURSEUNITVIEWER_H

#include <QtWidgets>
#include "Node/Design/nodedesign.h"

class Edge;
class Node;
class CourseScene;
class CourseUnit;
class StudentProgress;

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
    QMap<QString, NodeDesign*> nodesDesigns;
    Node * selectedNode;
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
	//! Set scene size
	void setSceneSize(int w, int h);
	//! Return scene size
	QPointF getSceneSize();
	//! Current design
	NodeDesign* getCurrentDesign();
	//! sets current node
	void setSelectedNode(Node * nd);
	//! returns current node
	Node* getSelectedNode();
    //----------------------------------

public slots:

	// public slots
	//----------------------------------
	//! Deletes everything from this scene
	void clearAllScene();
	//! Clears everything about student progress
	void clearStudentProgress();
	//! Adds node to this scene. Emit signal
	void addNode(Node *nd);
	//! Adds edge to this scene.
	void addEdge(Edge *e);
	//! Trasfers data from CourseUnit to this scene. From the second level of hierarchy of this course unit.
	void unpack(CourseUnit *head);
	//! Transfers data from this stp to this widget.
	void unpack(StudentProgress * prg);
	//! Trasfers data to CourseUnit from this scene. From the second level of hierarchy of this course unit.
	void pack(CourseUnit *head);
	//! pack widget to stp
	void pack(StudentProgress * prg);
	//! If drag is currently happining then it will be stoppped
	void abortDrag();
	//! sets progress to selected node
	void makeProgressToSelected(QString skill, double val);
	//! refit
	void refit();
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
    void on_repaintAll_stateChanged(int v);
    void on_designBox_currentTextChanged(QString v);
    //----------------------------------

signals:

	// signals
	//----------------------------------
	void nodeAdded(Node *nd);
	void nodeSelected(Node * nd);
	void nodeDeleted(Node * nd);
	void edgeDeleted(Edge * ed);
	void nodeSkillsChanged(Node * nd);
	void sceneSizeChanged(int w, int h);
	void progressMade(QString skill, double lev);
	//----------------------------------
};
#endif
