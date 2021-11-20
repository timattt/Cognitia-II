/*
 * circlescene.h
 *
 *  Created on: Nov, 19, 2021
 *      Author: ZoomZero
 */

#ifndef OUTCIRCLESCENE_H
#define OUTCIRCLESCENE_H

#include <QtWidgets>

// Used classes
//====================================
class Edge;
class Node;
class OutputRelativeSkillsPanel;
//====================================

/**
 * This class is fully reimplemented from QGraphicsScene.
 * It is used to draw background.
 *
 * @author ZoomZero
 */
class OutCircleScene: public QGraphicsScene {
public:

    // Constructor and destructor
    //-----------------------------------
    /**
     * Simple init constructor.
     * @author ZoomZero
     */
    OutCircleScene(OutputRelativeSkillsPanel * v);
    ~OutCircleScene() = default;
    //-----------------------------------

private:

    // Fields
    //-----------------------------------
    /**
     * Parent object.
     * @author ZoomZero
     */
    OutputRelativeSkillsPanel * view;
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

#endif // OUTCIRCLESCENE_H
