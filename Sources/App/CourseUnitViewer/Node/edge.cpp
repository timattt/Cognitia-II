#include "edge.h"
#include "node.h"

Edge::Edge(Node *sourceNode, Node *destNode) :
		source(sourceNode), dest(destNode) {
	setAcceptedMouseButtons(Qt::NoButton);
	source->addEdge(this);
	dest->addEdge(this);
	setZValue(-2);
}

Edge::Edge(Node *sourceNode) :
		source(sourceNode), dest(nullptr) {
	setAcceptedMouseButtons(Qt::NoButton);
	setZValue(-2);
}

Node* Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
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
	qreal length = line.length();

	prepareGeometryChange();

	if (length > qreal(2 * NODE_RAD)) {
		QPointF edgeOffset((line.dx() * NODE_RAD) / length,
				(line.dy() * NODE_RAD) / length);
		sourcePoint = line.p1() + edgeOffset;
		destPoint = line.p2() - edgeOffset;
	} else {
		sourcePoint = destPoint = line.p1();
	}
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
	target = p;
	adjust();
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (source == nullptr) {
		return;
	}
	QLineF line(sourcePoint, destPoint);
    if (line.length() < 0.5) {
        return;
    }

    // Draw the line itself
    painter->setPen(QPen(Qt::black, NODE_RAD / 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    qreal arrowSize = getArrowSize();

    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
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

bool Edge::isDragable() {
	return dest == nullptr;
}

void Edge::connectToNode(Node * dest) {
	this->dest = dest;
	dest->addEdge(this);
	source->addEdge(this);
}

double Edge::getArrowSize() const {
	QLineF line(sourcePoint, destPoint);
	return qMin(line.length(), NODE_RAD / 5.0);
}
