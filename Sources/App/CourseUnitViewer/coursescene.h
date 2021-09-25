/*
 * coursescene.h
 *
 *  Created on: Aug 25, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_COURSESCENE_H_
#define APP_COURSEUNITVIEWER_COURSESCENE_H_

#include <qgraphicsscene.h>

class Edge;
class Node;
class CourseUnitViewer;

class CourseScene: public QGraphicsScene {
public:

	// Constructor and destructor
	//-----------------------------------
	CourseScene(CourseUnitViewer * view);
	//-----------------------------------

private:

	// Fields
	//-----------------------------------
    Edge* dragEdge;
    CourseUnitViewer * view;
    //-----------------------------------

public:

    // public functions
    //-----------------------------------
    void stopDrag();
    //-----------------------------------

protected:

    // protected functions
    //-----------------------------------
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void drawBackground(QPainter *painter, const QRectF &rect) override;
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
	//-----------------------------------

};

#endif /* APP_COURSEUNITVIEWER_COURSESCENE_H_ */
