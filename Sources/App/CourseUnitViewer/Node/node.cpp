#include "node.h"
#include "edge.h"
#include "../../Structures/CourseUnit/courseunit.h"
#include "../courseunitviewer.h"
#include "../../Core/logger.h"

Node::Node(CourseUnitViewer *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    setName("New node");
}

void Node::addEdge(Edge *edge)
{
	NOT_NULL(edge);

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
    qreal adjust = NODE_RAD / 2;
    return QRectF( -NODE_RAD - adjust, -NODE_RAD - adjust, 2 * NODE_RAD + NODE_SHADOW_SHIFT + 2*adjust, 2 * NODE_RAD + NODE_SHADOW_SHIFT + 2*adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-NODE_RAD, -NODE_RAD, 2 * NODE_RAD, 2 * NODE_RAD);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * w)
{
	if (graph->getCurrentDesign() == nullptr) {
		throw QString("No current node design!");
	}
	graph->getCurrentDesign()->draw(this, painter, option, w);
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

const QMap<QString, double>& Node::getProgress() const {
	return progress;
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
	NOT_NULL(e);
	edgeList.removeAll(e);
}

bool Node::hasEdgeToNode(Node *nd) {
	NOT_NULL(nd);

	for (Edge * e : edgeList) {
		if (e->sourceNode() == nd || e->destNode() == nd) {
			return true;
		}
	}

	return false;
}

QColor Node::getColor() {
	return color;
}


void Node::addInSkill(QString name, int lev) {
	inSkills[name] = lev;

	if (graph != nullptr) {
		emit graph->nodeSkillsChanged(this);
		update();
	}
}

void Node::addOutSkill(QString name, int lev) {
	outSkills[name] = lev;

	if (graph != nullptr) {
		emit graph->nodeSkillsChanged(this);
		update();
	}
}

void Node::removeInSkill(QString name) {
	inSkills.remove(name);

	if (graph != nullptr) {
		emit graph->nodeSkillsChanged(this);
		update();
	}
}

void Node::removeOutSkill(QString name) {
	outSkills.remove(name);

	if (graph != nullptr) {
		emit graph->nodeSkillsChanged(this);
		update();
	}
}

const QMap<QString, int>& Node::getInSkills() const {
	return inSkills;
}

const QMap<QString, int>& Node::getOutSkills() const {
	return outSkills;
}

QString Node::getName() {
	return name;
}

void Node::setName(QString name) {
	if (this->name != name) {
		update();
	}
	this->name = name;
}

QString Node::getFile() const {
	return file;
}

void Node::setFile(QString file) {
	this->file = file;
}

void Node::setColor(QColor color) {
	if (this->color != color) {
		update();
	}
	this->color = color;
}

void Node::clearSkills() {
	inSkills.clear();
	outSkills.clear();

	if (graph != nullptr) {
		emit graph->nodeSkillsChanged(this);
		update();
	}
}

QString Node::getDescription() {
	return description;
}

void Node::setDescription(QString str) {
	if (str != description) {
		update();
	}
	description = str;
}

void fromNodeToCourseUnit(Node *nd, CourseUnit *cu) {
	NOT_NULL(nd);
	NOT_NULL(cu);

	cu->setObjectName(nd->getName());
	cu->setDescription(nd->getDescription());
	cu->setColour(nd->getColor().rgb());

	for (QString sk : nd->getInSkills().keys()) {
		int lev = nd->getInSkills()[sk];

        cu->addIncome(sk, lev);
	}

	for (QString sk : nd->getOutSkills().keys()) {
		int lev = nd->getOutSkills()[sk];

        cu->addOutcome(sk, lev);
	}

	cu->setCoords(nd->pos().x(), nd->pos().y());
}

void fromCourseUnitToNode(CourseUnit *cu, Node *nd) {
	NOT_NULL(cu);
	NOT_NULL(nd);

	nd->setName(cu->objectName());
	nd->setFile(cu->getLastFilePath());
	nd->setDescription(cu->getDescription());
	nd->setColor(QColor(cu->getColour()));

    const QMap<QString, size_t>& inskills = cu->getIncome();
    for (QString in : inskills.keys()) {
        nd->addInSkill(in, inskills[in]);
	}

    const QMap<QString, size_t>& outskills = cu->getOutcome();
    for (QString out : outskills.keys()) {
        nd->addOutSkill(out, outskills[out]);
	}

	nd->setPos(cu->getCoords().first, cu->getCoords().second);
}

CourseUnitViewer* Node::getViewer() {
	NOT_NULL(graph);
	return graph;
}

double Node::getSkillProgress(QString skill) {
	return progress[skill];
}

bool Node::containsProgress(QString skill) {
	return progress.contains(skill);
}

void Node::setProgress(QString skill, double val) {
	NOT_NULL(graph);

	progress[skill] = val;
	if (graph->isNodeSelected(this)) {
		emit graph->progressMadeToSelected(skill, val);
	}
	update();
}

bool Node::isSelected() {
	NOT_NULL(graph);
	return graph->isNodeSelected(this);
}

double Node::getProgressScalar(QString skill) {
	double from = -1;
	double to = -1;
	if (inSkills.contains(skill)) {
		from = inSkills[skill];
	}
	if (outSkills.contains(skill)) {
		to = outSkills[skill];
	}
	if (from < 0) {
		from = to - 1;
	}
	if (to < 0) {
		to = from + 1;
	}

	return (from - progress[skill]) / (from - to);
}

void Node::clearStudentProgress() {
	progress.clear();
}
