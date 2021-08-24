#ifndef NODE_H
#define NODE_H

#include <QtWidgets>

class Edge;
class CourseUnitViewer;

class Node : public QGraphicsItem
{

public:

    // Types
    //--------------------------------------
    enum { Type = UserType + 1 };
    //--------------------------------------

public:

    // Constructor
    //--------------------------------------
    Node(CourseUnitViewer *viewer);
    //--------------------------------------

public:

    // public functions
    //--------------------------------------
    void addEdge(Edge *edge);
    QList<Edge *> edges() const;
    int type() const override { return Type; }
    void calculateForces();
    bool advancePosition();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //--------------------------------------

protected:

    // protected functions
    //--------------------------------------
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    //--------------------------------------

private:

    // Fields
    //--------------------------------------
    QList<Edge *> edgeList;
    QPointF newPos;
    CourseUnitViewer *graph;
    //--------------------------------------
};
#endif
