/*
 * circlescene.h
 *
 *  Created on: Oct 26, 2021
 *      Author: ZoomZero
 */

#ifndef CIRCLESCENE_H
#define CIRCLESCENE_H

#include <QtWidgets>

// Used classes
//====================================
class Edge;
class Node;
class InputAbsoluteSkillsPanel;
//====================================

/**
 * This class is fully reimplemented from QGraphicsScene.
 * It is used to draw background.
 *
 * @author ZoomZero
 */
class CircleScene: public QGraphicsScene {
public:

    // Constructor and destructor
    //-----------------------------------
    /**
     * Simple init constructor.
     * @author ZoomZero
     */
    CircleScene(InputAbsoluteSkillsPanel * v);
    ~CircleScene() = default;
    //-----------------------------------

private:

    // Fields
    //-----------------------------------
    /**
     * Parent object.
     * @author ZoomZero
     */
    InputAbsoluteSkillsPanel * view;
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

#endif // CIRCLESCENE_H
