#ifndef COURSEUNITVIEWER_H
#define COURSEUNITVIEWER_H

// Includes
//======================================================
#include <QtWidgets>
#include "Node/Design/nodedesign.h"
//======================================================

// Constants
//======================================================
#define SCALE_PER_PUSH 1.3
//======================================================

// Classes that are used by this class.
//======================================================
class Edge;
class Node;
class CourseScene;
class CourseUnit;
class StudentProgress;
//======================================================

namespace Ui {
class CourseUnitViewer;
}

/**
 * This widget class is responsible for drawing the course graph. You can add it to any editor.
 * It has many convenient slots and signals for connecting to other parts of the program.
 * In the constructor, you can add new designs for vertices. You can turn the editing mode on and off.
 *
 * @author timattt
 *
 * "Скажи мне, кто тебя хвалит, и я тебе скажу, в чем ты ошибся" - В.И.Ленин
 */
class CourseUnitViewer : public QWidget
{
    Q_OBJECT

public:

    // Constructor and destructor
    //======================================================
	/**
	 * This constructor initializes this widget. And sets its parent.
	 * @param parent - widget that will be used
	 * @author timattt
	 */
    CourseUnitViewer(QWidget *parent);
    /**
     *
     * Cleans up this widget. Deletes everything.
     * @author timattt
     */
    ~CourseUnitViewer();
    //======================================================

private:

    // Fields
    //======================================================
    /**
     * Regular QT ui field
     * @author timattt
     */
    Ui::CourseUnitViewer *ui;
    /**
     * Nodes can be selected by user. So if one is selected it will be stored here. Else this field will be nullptr.
     * @author timattt
     */
    Node * selectedNode;
    /**
     * Scene that is used in graphicsView.
     * @author timattt
     */
    CourseScene * scene;
    /**
     * Library for designs: {design name} -> {pointer to design object}.
     * It initializes in constructor and there keys are copied to designBox combo box.
     * @author timattt
     */
    QMap<QString, NodeDesign*> nodesDesigns;
    /**
     * Id of timer that is used to update physics.
     * @autor timattt
     */
    int timerId;
    /**
     * Physics parameter for nodes - attraction factor.
     * @author timattt
     */
    double attFac;
    /**
     * Physics parameter for nodes - repulsion factor.
     * @author timattt
     */
    double repFac;
    /**
     * Physics parameter for nodes - own length factor.
     * @author timattt
     */
    double ownLength;
    /**
     * this widget can be not editable.
     * So all not readonly buttons are not working.
     * @author timattt
     */
    bool editable;
    //======================================================

private:

    // Private functions
    //======================================================
    /**
     * This function updates scene physics.
     * @author timattt
     */
    void timerEvent(QTimerEvent * event);
    //======================================================

public:

    // Public functions
    //======================================================
    /**
     * Is physics applied currently to nodes.
     * @returns true if freeze checkbox is off.
     * @author timattt
     */
    bool nodesCanMove();
    /**
     * It delete mode is turned on then when you click something on the scene - it will be deleted.
     * @returns true if delete mode checkbox is on
     * @author timattt
     */
    bool deleteModeIsOn();
    /**
     * Attraction factor is used in physics calculation in nodes.
     * @returns - current attraction factor
     * @author timattt
     */
	double getAttFac() const;
    /**
     * Own length - length to which edge will try to resize
     * @returns - current own length
     * @author timattt
     */
	double getOwnLength() const;
    /**
     * Repulsion factor is used in physics calculation in nodes.
     * @returns - current repulsion factor
     * @author timattt
     */
	double getRepFac() const;
	/**
	 * If widget is not editable - you can not create new node or edges. Also you can't change field size.
	 * @returns true - if widget is editable
	 * @author timattt
	 */
	bool isEditable();
	/**
	 * If widget is not editable - you can not create new node or edges. Also you can't change field size.
	 * @param v - editable or not
	 * @author timattt
	 */
	void setEditable(bool v);
	/**
	 * Everything is inside scene rectangle. This method sets width and height of this rect.
	 * Also it x and y will be set to w/2 and h/2.
	 * @param w - scene rect width
	 * @param h - scene rect height
	 * @author timattt
	 */
	void setSceneSize(int w, int h);
	/**
	 * Everything is inside scene rectangle. This method gives size of this rect.
	 * @returns points where x = scene.width, y = scene.height
	 * @author timattt
	 */
	QPointF getSceneSize();
	/**
	 * There are several designs. User can choose one using designBox combo box.
	 * So all designs are stored in qmap: {design name} -> {pointer to design}.
	 * Design object has draw functions and some other params.
	 * When node ot drawn it uses current design.
	 * @returns pointer to current design. May not be nullptr.
	 * @author timattt
	 */
	NodeDesign* getCurrentDesign();
	/**
	 * User can select node. So inwoke this function if you want some node to be selected.
	 * This function modifies selectedNode field. And emits signal nodeSelected.
	 * Also it updates your node and previously selected node.
	 * @param nd - node to select. Can be nullptr, in this case no node is now selected.
	 * @author timattt
	 */
	void setSelectedNode(Node * nd);
	/**
	 * True if given node is equals to currently selected.
	 * If nd is nullptr and no node is selected to it will be true.
	 * @param nd - node to check
	 * @returns true if nd is selected.
	 * @author timattt
	 */
	bool isNodeSelected(Node * nd);
	/**
	 * Gives currently selected node.
	 * @returns selected node. Can be nullptr if nothing is selected.
	 * @author timattt
	 */
	Node* getSelectedNode();
    //======================================================

public slots:

	// public slots
	//======================================================
	/**
	 * This function clear every node from this scene.
	 * Before it aborts drag and set no node to be selected. For this it uses functions:
	 * abortDrag and setSelectedNode(nullptr).
	 * Then it collects every node from scene, removes it from it. And finally calls the destructor.
	 * Destructor may kill all edges that connects to node.
	 * @author timattt
	 */
	void clearAllScene();
	/**
	 * For every node on this scene it calls clearStudentProgress function and updates node.
	 * @author timattt
	 */
	void clearStudentProgress();
	/**
	 * Adds node to scene. Calls nodeAdded signal and updates scene.
	 * Only this function can add node correctly. So do not add nodes manually.
	 * @param nd - pointer to node that will be added. May not be nullptr.
	 * @author timattt
	 */
	void addNode(Node *nd);
	/**
	 * Adds edge to this scene.
	 * To add edge use only this function.
	 * @param e - pointer to edge that will be added. May not be nullptr.
	 * @author timattt
	 */
	void addEdge(Edge *e);
	/**
	 * Transfers data from CourseUnit to this scene. From the second level of hierarchy of this course unit.
	 * Sets scene size from this courseUnit values. Also calls refit function.
	 * @param head - top level courseUnit. Its children will be added. May not be nullptr.
	 * @author timattt
	 */
	void unpack(CourseUnit *head);
	/**
	 * Transfers data from this stp to this widget.
	 * Also call refit function.
	 * @param prg - pointer to StudentProgress object. May not be nullptr.
	 * @author timattt
	 */
	void unpack(StudentProgress * prg);
	/**
	 * Transfers data to CourseUnit from this scene. From the second level of hierarchy of this course unit.
	 * @param head - top level courseUnit, scene content will be added to its embedded. May not be nullptr.
	 * @author timattt
	 */
	void pack(CourseUnit *head);
	/**
	 * Transfers data from scene to stp structure.
	 * @param prg - destination StudentProgress. May not be nullptr.
	 * @author timattt
	 */
	void pack(StudentProgress * prg);
	/**
	 * Calls stopDrag in scene. Drag edge will be deleted and then set to null.
	 * @author timattt
	 */
	void abortDrag();
	/**
	 * If some node is selected. Then sets progress for it.
	 * Calls setProgress for selectedNode. It then calls signal progressMadeToSelected.
	 * @param skill - skill name you want to make progress
	 * @param val - progress level
	 * @author timattt
	 */
	void makeProgressToSelected(QString skill, double val);
	/**
	 * Scales grapjicsView to ensure scene fits it.
	 * @author timattt
	 */
	void refit();
	//======================================================
    
private slots:

    // Slots
	//======================================================
	void on_pushButton_2_clicked();
	void on_zoomOut_clicked();
	void on_zoomIn_clicked();
	void on_areaDec_clicked();
	void on_areaIn_clicked();
	void on_attFact_editingFinished();
    void on_repFact_editingFinished();
    void on_ownLength_editingFinished();
    void on_repaintAll_stateChanged(int v);
    void on_designBox_currentTextChanged(QString v);
    //======================================================

signals:

	// signals
	//======================================================
	/**
	 * Called every time node is added.
	 * @param nd - new node.
	 * @author timattt
	 */
	void nodeAdded(Node *nd);
	/**
	 * Called every time node is selected.
	 * @param nd - selected node. Can be nullptr it nothing is selected.
	 * @author timattt
	 */
	void nodeSelected(Node * nd);
	/**
	 * Called every time when node is deleted.
	 * Right after this call pointer is deleted.
	 * @param nd - node that will be deleted.
	 * @author timattt
	 */
	void nodeDeleted(Node * nd);
	/**
	 * Called every time when edge is deleted.
	 * Right after this call pointer is deleted.
	 * @param ed - edge that will be deleted.
	 * @author timattt
	 */
	void edgeDeleted(Edge * ed);
	/**
	 * Called every time skills is changed in some node.
	 * @param nd - node where skill changed.
	 * @author timattt
	 */
	void nodeSkillsChanged(Node * nd);
	/**
	 * Called when scene size changed.
	 * @param w - new width
	 * @param h - new height
	 * @author timattt
	 */
	void sceneSizeChanged(int w, int h);
	/**
	 * Called when for selected node progress is made.
	 * @param skill - skill name.
	 * @param lev - progress level.
	 * @author timattt
	 */
	void progressMadeToSelected(QString skill, double lev);
	//======================================================
};
#endif
