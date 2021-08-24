#ifndef EDGE_H
#define EDGE_H

#include <QtWidgets>

#include "../nodesdefs.h"

class Edge : public QGraphicsItem
{

public:

    // Types
    //--------------------------------------
    enum { Type = UserType + 2 };
    //--------------------------------------

public:

    // Constructors
    //--------------------------------------
    Edge(Node *sourceNode);
    Edge(Node *sourceNode, Node *destNode);
    //--------------------------------------

public:

    // public functions
    //--------------------------------------
    Node *sourceNode() const;
    Node *destNode() const;
    void adjust();
    int type() const override { return Type; }
    //--------------------------------------

protected:

    // protected functions
    //--------------------------------------
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //--------------------------------------

protected:

    // Fields
    //--------------------------------------
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize = 10;
    //--------------------------------------

};
#endif
