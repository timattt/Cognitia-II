#ifndef NODE_H
#define NODE_H

#include <QtWidgets>

class Edge;
class CourseUnitViewer;

#define DEFAULT_ATT_FAC 300
#define DEFAULT_REP_FAC 0.1
#define DEFAULT_MASS_FAC 10
#define NODE_RAD 50
#define NODE_SHADOW_SHIFT (NODE_RAD / 20 * 3)
#define EPSILON 0.1
#define MAX_SYMBOLS_PER_LINE 4

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
    void addInSkill(QString name, int lev);
    void addOutSkill(QString name, int lev);
    void removeInSkill(QString name);
    void removeOutSkill(QString name);
	const QMap<QString, int>& getInSkills() const;
	const QMap<QString, int>& getOutSkills() const;
	const QString& getName() const;
	void setName(QString name);
	QString getFile() const;
	void setFile(QString file);
	void setColor(QColor color = Qt::blue);
    //--------------------------------------

protected:

    // protected functions
    //--------------------------------------
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    //--------------------------------------

private:

    // private functions
    //--------------------------------------
    void drawSkills(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString rebuildStr(QString str);
    //--------------------------------------

private:

    // Fields
    //--------------------------------------
    QList<Edge *> edgeList;
    CourseUnitViewer *graph;
    QMap<QString, int> inSkills;
    QMap<QString, int> outSkills;
    QString file;
    QString name;
    QColor color = Qt::blue;
    //--------------------------------------
};
#endif
