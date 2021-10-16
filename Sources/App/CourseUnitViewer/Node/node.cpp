#include "node.h"
#include "edge.h"
#include "../../Structures/CourseUnit/courseunit.h"
#include "../courseunitviewer.h"
#include "../../Core/logger.h"
#include "../viewport.h"

Node::Node(CourseUnitViewer *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    setName(NODE_DEFAULT_NAME);
}

void Node::addEdge(Edge *edge)
{
	NOT_NULL(edge);

	ASSERTM(!edgeList.contains(edge), "Already has such edge!");

    edgeList << edge;
    edge->adjust();
}

void Node::calculateForces()
{
	if (graph == nullptr || scene() == nullptr || scene()->mouseGrabberItem() == this || !graph->nodesCanMove()) {
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
        if (l > 0 && (l < 4 * NODE_RAD * NODE_RAD)) {
            xvel += dx / l;
            yvel += dy / l;
        }
    }

    xvel *= graph->getRepFac();
    yvel *= graph->getRepFac();

    // Now subtract all forces pulling items together
    for (const Edge *edge : qAsConst(edgeList)) {
        QPointF vec;

        if (edge->getSourceNode() == this) {
            vec = mapToItem(edge->getDestNode(), 0, 0);
            if (edge->isDragable()) {
            	vec.setX(vec.x() - edge->getTarget().x());
            	vec.setY(vec.y() - edge->getTarget().y());
            }
        } else {
            vec = mapToItem(edge->getSourceNode(), 0, 0);
        }

        double len = qSqrt(vec.x() * vec.x() + vec.y() * vec.y());

        double delta = -graph->getOwnLength() + len;

        xvel -= graph->getAttFac() * vec.x() / len * delta;
        yvel -= graph->getAttFac() * vec.y() / len * delta;
    }

    if (qAbs(xvel) < NODE_PHYSICS_EPSILON && qAbs(yvel) < NODE_PHYSICS_EPSILON) {
        xvel = yvel = 0;
    }

    //QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    //newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + NODE_RAD), sceneRect.right() - NODE_RAD));
    //newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + NODE_RAD), sceneRect.bottom() - NODE_RAD));

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
	NOT_NULL(graph->getCurrentDesign());
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

bool Node::hasEdgeToNode(Node *nd) const {
	NOT_NULL(nd);

	for (Edge * e : edgeList) {
		if (e->getSourceNode() == nd || e->getDestNode() == nd) {
			return true;
		}
	}

	return false;
}

QColor Node::getColor() const {
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

QString Node::getName() const {
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
	if (this->file != file) {
		update();
	}
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
	}

	update();
}

QString Node::getDescription() const {
	return description;
}

void Node::setDescription(QString str) {
	if (str != description) {
		update();
	}
	description = str;
}

void Node::fromNodeToCourseUnit(CourseUnit *cu) {
	NOT_NULL(cu);

	cu->setObjectName(getName());
	cu->setDescription(getDescription());
	cu->setColour(getColor().rgb());

	for (QString sk : getInSkills().keys()) {
		int lev = getInSkills()[sk];

        cu->addIncome(sk, lev);
	}

	for (QString sk : getOutSkills().keys()) {
		int lev = getOutSkills()[sk];

        cu->addOutcome(sk, lev);
	}

	if (graph != nullptr) {
		cu->setCoords(pos().x() - graph->getViewport()->getCameraPos().x(), pos().y() - graph->getViewport()->getCameraPos().y());
	} else {
		cu->setCoords(pos().x(), pos().y());
	}
}

void Node::fromCourseUnitToNode(CourseUnit *cu) {
	NOT_NULL(cu);

	setName(cu->objectName());
	setFile(cu->getLastFilePath());
	setDescription(cu->getDescription());
	setColor(QColor(cu->getColour()));

    const QMap<QString, size_t>& inskills = cu->getIncome();
    for (QString in : inskills.keys()) {
        addInSkill(in, inskills[in]);
	}

    const QMap<QString, size_t>& outskills = cu->getOutcome();
    for (QString out : outskills.keys()) {
        addOutSkill(out, outskills[out]);
	}

    if (graph != nullptr) {
    	setPos(cu->getCoords().first + graph->getViewport()->getCameraPos().x(), cu->getCoords().second + graph->getViewport()->getCameraPos().y());
    } else {
    	setPos(cu->getCoords().first, cu->getCoords().second);
    }
}

double Node::getSkillProgress(QString skill) const {
	ASSERTM(progress.contains(skill), "No such skill");
	return progress[skill];
}

bool Node::containsProgress(QString skill) {
	return progress.contains(skill);
}

void Node::setProgress(QString skill, double val) {
	progress[skill] = val;
	if (graph != nullptr && graph->isNodeSelected(this)) {
		emit graph->progressMadeToSelected(skill, val);
	}
	update();
}

bool Node::isSelected() {
	return graph != nullptr && graph->isNodeSelected(this);
}

double Node::getProgressScalar(QString skill) const {
	double from = -1;
	double to = -1;
	if (inSkills.contains(skill)) {
		from = inSkills[skill];
	}
	if (outSkills.contains(skill)) {
		to = outSkills[skill];
	}

	ASSERTM(from > 0 || to > 0, "No such skill!");

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
	update();
}
