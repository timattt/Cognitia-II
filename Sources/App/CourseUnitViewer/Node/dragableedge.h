#ifndef DRAGABLEEDGE_H
#define DRAGABLEEDGE_H

#include <QtWidgets>

#include "edge.h"

class DragableEdge : public Edge
{

public:

    // Constructor
    //----------------------------
    DragableEdge(Node * from);
    ~DragableEdge();
    //----------------------------

protected:

    // protected overrided functions
    //----------------------------
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //----------------------------

private:

    // Fields
    //----------------------------
    QPointF mousePos;
    //----------------------------

protected:

    // protected functions
    //----------------------------
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //----------------------------

};
#endif
