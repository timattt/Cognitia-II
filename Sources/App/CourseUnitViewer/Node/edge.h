#ifndef EDGE_H
#define EDGE_H

#include <QtWidgets>

class Node;

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
    ~Edge();
    //--------------------------------------

public:

    // public functions
    //--------------------------------------
    Node *sourceNode() const;
    Node *destNode() const;
    void adjust();
    int type() const override { return Type; }
    bool isDragable();
    void setTarget(QPointF p);
	void connectToNode(Node * dest);
    //--------------------------------------

protected:

    // protected functions
    //--------------------------------------
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    double getArrowSize() const;
    //--------------------------------------

protected:

    // Fields
    //--------------------------------------
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    QPointF target;
    //--------------------------------------

};
#endif
