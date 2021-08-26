#ifndef NODE_H
#define NODE_H

#include <QtWidgets>

class Edge;
class CourseUnitViewer;

#define DEFAULT_ATT_FAC 10
#define DEFAULT_REP_FAC 1
#define DEFAULT_MASS_FAC 100

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
    ~Node();
    //--------------------------------------

public:

    // public functions
    //--------------------------------------
    void addEdge(Edge *edge);
    QList<Edge *> edges() const;
    int type() const override { return Type; }
    void calculateForces();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void removeEdge(Edge * e);
    bool hasEdgeToNode(Node * nd);
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
    CourseUnitViewer *graph;
    //--------------------------------------
};
#endif
