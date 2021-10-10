/*
 * FlowerScene.h
 *
 *  Created on: Aug 25, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_FlowerScene_H_
#define APP_COURSEUNITVIEWER_FlowerScene_H_

#include <QtWidgets>

// Used classes
//====================================
class Edge;
class Node;
class SkillsFlower;
//====================================

/**
 * This class is fully reimplemented from QGraphicsScene.
 * It is used to draw background.
 *
 * @author timattt
 */
class FlowerScene: public QGraphicsScene {
public:

	// Constructor and destructor
	//-----------------------------------
	/**
	 * Simple init constructor.
	 * @author timattt
	 */
	FlowerScene(SkillsFlower * v);
	~FlowerScene() = default;
	//-----------------------------------

private:

	// Fields
	//-----------------------------------
	/**
	 * Parent object.
	 * @author timattt
	 */
	SkillsFlower * view;
    //-----------------------------------

protected:

    // protected functions
    //-----------------------------------
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) 		override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) 	override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) 		override;
	void drawBackground(QPainter *painter, const QRectF &rect) 	override;
	//-----------------------------------

};

#endif /* APP_COURSEUNITVIEWER_FlowerScene_H_ */
