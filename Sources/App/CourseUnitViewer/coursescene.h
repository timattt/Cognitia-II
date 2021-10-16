/*
 * coursescene.h
 *
 *  Created on: Aug 25, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_COURSESCENE_H_
#define APP_COURSEUNITVIEWER_COURSESCENE_H_

#include <QtWidgets>

// Used classes
//====================================
class Edge;
class Node;
class CourseUnitViewer;
//====================================

// Constants
//====================================
#define START_SCENE_WIDTH 200
#define START_SCENE_HEIGHT 200
//====================================

class CourseScene: public QGraphicsScene {
public:

	// Constructor and destructor
	//-----------------------------------
	/**
	 * Initializes empty scene with default size. See defines.
	 * @param view - widget, where this scene is used.
	 * @author timattt
	 */
	CourseScene(CourseUnitViewer * view);
	/**
	 * Clears dragEdge if it is not nullptr.
	 * @author timattt
	 */
	~CourseScene();
	//-----------------------------------

private:

	// Fields
	//-----------------------------------
	/**
	 * This edge is create when user is pressing some node.
	 * It will be initialized with only source node.
	 * @author timattt
	 */
    Edge* dragEdge;
    /**
     * Parent widget
     * @author timattt
     */
    CourseUnitViewer * view;
    //-----------------------------------

public:

    // public functions
    //-----------------------------------
    /**
     * Aborts drag forcibly.
     * Deletes gragEdge and sets it to nullptr.
     * @author timattt
     */
    void stopDrag();
    //-----------------------------------

protected:

    // protected functions
    //-----------------------------------
    /**
     * If drag is on so moves dragEdge target.
     */
    void mouseMoveEvent		(QGraphicsSceneMouseEvent *event) 		override;
    /**
     * In this function drag is ended.
     * Drag edge method connectToNode is invoked or dragEdge is delete. Sets dragEdge to nullptr if it was not.
     */
	void mouseReleaseEvent	(QGraphicsSceneMouseEvent *event) 		override;
	/**
	 * This function is used to select nodes and edges, deletes nodes and edges, create dragEdge.
	 * Some signals are emited from here.
	 */
	void mousePressEvent	(QGraphicsSceneMouseEvent *event) 		override;
	/**
	 * This function works with drops from some other gui (courseEditor maybe) and adds new skills.
	 */
	void dropEvent			(QGraphicsSceneDragDropEvent *event) 	override;
	/**
	 * This function used to accept all drops.
	 * Do not delete it.
	 */
	void dragEnterEvent		(QGraphicsSceneDragDropEvent *event) 	override;
	/**
	 * This function used to accept all drops.
	 * Do not delete it.
	 */
	void dragMoveEvent		(QGraphicsSceneDragDropEvent *event) 	override;
	/**
	 * This function used to accept all drops.
	 * Do not delete it.
	 */
	void dragLeaveEvent		(QGraphicsSceneDragDropEvent *event) 	override;
	/**
	 * Grey background is drawing here
	 */
	void drawBackground		(QPainter *painter, const QRectF &rect) override;
	//-----------------------------------

};

#endif /* APP_COURSEUNITVIEWER_COURSESCENE_H_ */
