#include "node.h"
#include "edge.h"
#include "../courseunitviewer.h"

Node::Node(CourseUnitViewer *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

void Node::calculateForces()
{
	if (graph == nullptr) {
		return;
	}

    if (!scene() || scene()->mouseGrabberItem() == this || !graph->nodesCanMove()) {
    	return;
    }

    QPointF newPos(0, 0);

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node) {
            continue;
        }

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = (dx * dx + dy * dy);
        if (l > 0) {
            xvel += dx / l;
            yvel += dy / l;
        }
    }

    xvel *= graph->getAttFac();
    yvel *= graph->getAttFac();

    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * graph->getMassFac();
    for (const Edge *edge : qAsConst(edgeList)) {
        QPointF vec;

        if (edge->sourceNode() == this) {
            vec = mapToItem(edge->destNode(), 0, 0);
        } else {
            vec = mapToItem(edge->sourceNode(), 0, 0);
        }
        xvel -= graph->getRepFac() * vec.x() / weight;
        yvel -= graph->getRepFac() * vec.y() / weight;
    }

    if (qAbs(xvel) < EPSILON && qAbs(yvel) < EPSILON) {
        xvel = yvel = 0;
    }

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + NODE_RAD), sceneRect.right() - NODE_RAD));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + NODE_RAD), sceneRect.bottom() - NODE_RAD));

    setPos(newPos);
}

QRectF Node::boundingRect() const
{
    qreal adjust = NODE_RAD / 5;
    return QRectF( -NODE_RAD - adjust, -NODE_RAD - adjust, 2 * NODE_RAD + NODE_SHADOW_SHIFT + adjust, 2 * NODE_RAD + NODE_SHADOW_SHIFT + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-NODE_RAD, -NODE_RAD, 2 * NODE_RAD, 2 * NODE_RAD);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * w)
{
	// Shadow
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(50, 50, 50, 128)));
    painter->drawEllipse(-NODE_RAD + NODE_SHADOW_SHIFT, -NODE_RAD + NODE_SHADOW_SHIFT, NODE_RAD * 2, NODE_RAD * 2);

    QRadialGradient gradient(-NODE_SHADOW_SHIFT, -NODE_SHADOW_SHIFT, NODE_RAD);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).lighter(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 1));

    painter->drawEllipse(-NODE_RAD, -NODE_RAD, 2 * NODE_RAD, 2 * NODE_RAD);

    gradient.setCenter(0, 0);
    gradient.setFocalPoint(0, 0);

    drawSkills(painter, option, w);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : qAsConst(edgeList)) {
            edge->adjust();
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

Node::~Node() {
	while (!edgeList.isEmpty()) {
		Edge * e = edgeList.first();
		edgeList.removeFirst();
		delete e;
	}
	if (scene() != nullptr) {
		scene()->removeItem(this);
	}
}

void Node::removeEdge(Edge *e) {
	edgeList.removeAll(e);
}

bool Node::hasEdgeToNode(Node *nd) {
	for (Edge * e : edgeList) {
		if (e->sourceNode() == nd || e->destNode() == nd) {
			return true;
		}
	}

	return false;
}

#define PI 3.1415

void Node::drawSkills(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget) {


	double rad = NODE_RAD / 4;

	QFont f = painter->font();
	f.setPointSizeF(rad / 2);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);

	painter->setPen(QPen(Qt::black, 0.4));

    double anglePerSkill = 30;
    double startAngle = 180 -inSkills.size() * anglePerSkill / 2;
    double endAngle = 180 + inSkills.size() * anglePerSkill / 2;

    if (endAngle > 270) {
        startAngle = 90;
        endAngle = 270;
        anglePerSkill = (endAngle - startAngle) / (double) inSkills.size();
    }

	int i = -1;
	for (QString key : inSkills.keys()) {
		i++;
		double alpha = endAngle - anglePerSkill * ((double)(i) + 0.5);

		double x = cos(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;
		double y = sin(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;

        painter->setBrush(Qt::black);
        painter->drawLine(0, 0, x, y);

        painter->translate(x, y);
        painter->rotate(alpha - 180);

        QRect r = QRect(0 - rad, 0 - rad / 2, 2.0 * rad, rad);

		QRadialGradient gradient(0, 0, rad);
        gradient.setColorAt(1, QColor(Qt::darkRed));
        gradient.setColorAt(0, QColor(Qt::red));
        painter->setBrush(gradient);
        painter->drawRect(0 - rad, - rad, 2.0 * rad, 2.0 * rad);

        painter->drawText(QRectF(0 - rad, 0 - rad, 2.0 * rad, rad), Qt::AlignCenter, rebuildStr(key));
        painter->drawText(QRectF(0 - rad, 0, 2.0 * rad, rad), Qt::AlignLeft, rebuildStr(QString::number(inSkills[key])));

        painter->rotate(-alpha +180);
        painter->translate(-x, -y);
	}

	anglePerSkill = 30;
    startAngle = -outSkills.size() * anglePerSkill / 2;
    endAngle = outSkills.size() * anglePerSkill / 2;

    if (endAngle > 90) {
        startAngle = -90;
        endAngle = 90;
        anglePerSkill = (endAngle - startAngle) / (double) outSkills.size();
    }

    i = -1;
	for (QString key : outSkills.keys()) {
		i++;
		double alpha = startAngle + anglePerSkill * ((double)(i) + 0.5);

		double x = cos(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;
		double y = sin(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;

        painter->setBrush(Qt::black);
        painter->drawLine(0, 0, x, y);

        painter->translate(x, y);
        painter->rotate( alpha);

        QRect r = QRect(0 - rad, 0 - rad / 2, 2.0 * rad, rad);

		QRadialGradient gradient(0, 0, rad);
        gradient.setColorAt(1, QColor(Qt::darkGreen));
        gradient.setColorAt(0, QColor(Qt::green));
        painter->setBrush(gradient);
        painter->drawRect(0 - rad, - rad, 2.0 * rad, 2.0 * rad);

        painter->drawText(QRectF(0 - rad, 0 - rad, 2.0 * rad, rad), Qt::AlignCenter, rebuildStr(key));
        painter->drawText(QRectF(0 - rad, 0, 2.0 * rad, rad), Qt::AlignRight, rebuildStr(QString::number(outSkills[key])));

        painter->rotate( -alpha);
        painter->translate(-x, -y);
	}

	// center
	QRadialGradient gr(0, 0, rad * 1.5);
	gr.setColorAt(1, QColor(this->color).darker(200));
	gr.setColorAt(0, QColor(this->color));
	painter->setBrush(gr);
	painter->drawEllipse(QPointF(0, 0), rad * 1.5, rad * 1.5);
}

QString Node::rebuildStr(QString str) {
	QStringList lst = str.split(" ");

	QStringList res = QStringList(lst.size());

	int added = 0;
	for (int i = 0, total = lst.join("").length(); added < MAX_SYMBOLS_PER_LINE && added < total; i++) {
		int partNumber = i % lst.size();
		int partSize = lst[partNumber].length();
		int partIndex = res[partNumber].size();

		if (partIndex < partSize) {
			QChar v = lst[partNumber][partIndex].toUpper();
			if (partIndex > 0) {
				v = v.toLower();
			}
			res[partNumber] += v;
			added++;
		}

	}

	return res.join("");
}

void Node::addInSkill(QString name, int lev) {
	inSkills[name] = lev;
	update();
}

void Node::addOutSkill(QString name, int lev) {
	outSkills[name] = lev;
	update();
}

void Node::removeInSkill(QString name) {
	inSkills.remove(name);
	update();
}

void Node::removeOutSkill(QString name) {
	outSkills.remove(name);
	update();
}

const QMap<QString, int>& Node::getInSkills() const {
	return inSkills;
}

const QMap<QString, int>& Node::getOutSkills() const {
	return outSkills;
}

const QString& Node::getName() const {
	return name;
}

void Node::setName(QString name) {
	this->name = name;
	update();
}

QString Node::getFile() const {
	return file;
}

void Node::setFile(QString file) {
	this->file = file;
}

void Node::setColor(QColor color) {
	this->color = color;
	update();
}

void Node::clearSkills() {
	inSkills.clear();
	outSkills.clear();
}
