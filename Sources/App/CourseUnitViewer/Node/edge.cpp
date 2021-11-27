#include "edge.h"
#include "node.h"
#include "../courseunitviewer.h"
#include "Design/nodedesign.h"
#include "../../Core/logger.h"

Edge::Edge(Node *sourceNode, Node *destNode, CourseUnitViewer * view) :
		source(sourceNode),
		dest(destNode),
		viewer(view) {

	NOT_NULL(sourceNode);
	NOT_NULL(destNode);
	NOT_NULL(view);

	setAcceptedMouseButtons(Qt::NoButton);
	source->addEdge(this);
	dest->addEdge(this);
	setZValue(-2);
}

Edge::Edge(Node *sourceNode, CourseUnitViewer * view) :
		source(sourceNode),
		dest(nullptr),
		viewer(view) {

	NOT_NULL(source);
	NOT_NULL(view);

	source->addEdge(this);

	setAcceptedMouseButtons(Qt::NoButton);
	setZValue(-2);
}

Node* Edge::getSourceNode() const
{
    return source;
}

Node *Edge::getDestNode() const
{
    return dest;
}

void Edge::adjust()
 {
	QLineF line;

	if (dest != nullptr) {
		line = QLineF(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
	} else {
		line = QLineF(mapFromItem(source, 0, 0), target);
	}
	//qreal length = line.length();

	prepareGeometryChange();
/*
	if (length > qreal(2 * NODE_RAD)) {
		sourcePoint = line.p1();
		destPoint = line.p2();
	} else {
		sourcePoint = destPoint = line.p1();
	}
	*/
	sourcePoint = line.p1();
	destPoint = line.p2();
}

QRectF Edge::boundingRect() const
{
    qreal penWidth = NODE_RAD / 10;

    qreal extra = (penWidth + getArrowSize()) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::setTarget(QPointF p) {
	ASSERTM(dest == nullptr, "trying to set target for edge that has destination node");

	target = p;
	adjust();
}

QPointF Edge::getTarget() const {
	ASSERTM(dest == nullptr, "trying to get target for edge that has destination node");
	return target;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (source == nullptr) {
		return;
	}

	double factor = viewer->getCurrentDesign()->getEdgeThickness();

	QLineF line(sourcePoint, destPoint);

    // Draw the line itself
    painter->setPen(QPen(Qt::black, NODE_RAD * factor, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    qreal arrowSize = getArrowSize();

    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    //QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
    //                                              cos(angle + M_PI / 3) * arrowSize);
    //QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
    //                                              cos(angle + M_PI - M_PI / 3) * arrowSize);

    double f = 0.5;
    QPointF pt = QPointF(((destPoint.x() - sourcePoint.x()) * f + sourcePoint.x()), ((destPoint.y()-sourcePoint.y())*f + sourcePoint.y()));
    QPointF destArrowP1 = pt + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = pt + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    //painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << pt << destArrowP1 << destArrowP2);
}

Edge::~Edge() {
	if (source != nullptr) {
		source->removeEdge(this);
	}
	if (dest != nullptr) {
		dest->removeEdge(this);
	}
	if (scene() != nullptr) {
		scene()->removeItem(this);
	}
}

bool Edge::isDragable() const {
	return dest == nullptr;
}

void Edge::connectToNode(Node * dest) {
	ASSERTM(isDragable(), "trying to connect edge to node. Edge already has dest node!");

	this->dest = dest;
	dest->addEdge(this);

	target.setX(0);
	target.setY(0);
}

double Edge::getArrowSize() const {
	QLineF line(sourcePoint, destPoint);
	return qMin(line.length(), NODE_RAD / 5.0);
}
