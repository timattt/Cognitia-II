#include "courseunitviewer.h"
#include "Node/edge.h"
#include "Node/node.h"
#include "coursescene.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "ui_courseunitviewer.h"
#include "Node/Design/nodedesignformal.h"
#include "Node/Design/nodedesignolive.h"
#include "Node/Design/nodedesignold.h"
#include "Node/Design/nodedesignshape.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "../Core/logger.h"
#include "courseunitviewersettings.h"
#include "Label/LabelHidden.h"
#include "Label/LabelBonus.h"
#include "Label/Label.h"

CourseUnitViewer::CourseUnitViewer(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::CourseUnitViewer),
		selectedNode(nullptr),
		scene(new CourseScene(this)),
		nodesDesigns(),
		timerId(0),
		attFac(DEFAULT_NODE_ATT_FAC),
		repFac(DEFAULT_NODE_REP_FAC),
		ownLength(DEFAULT_NODE_OWN_LENGTH),
		editable(true) {

	SAY("CourseUnitViewer init started");

	ui->setupUi(this);
	ui->graphicsView->setScene(scene);
	timerId = startTimer(1);

	ui->graphicsView->setAcceptDrops(true);

	ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);

	// designs
	//-------------------------------------------------
	nodesDesigns["Olive"] = new NodeDesignOlive(this);
	nodesDesigns["Formal"] = new NodeDesignFormal(this);
	nodesDesigns["Old"] = new NodeDesignOld(this);
    nodesDesigns["Shape"] = new NodeDesignShape(this);
    //-------------------------------------------------

	ui->designBox->addItems(nodesDesigns.keys());
	ui->designBox->setCurrentText("Old");
	//-------------------------------------------------

	ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	initLabels();

	SAY("CourseUnitViewer init finished");
}

CourseUnitViewer::~CourseUnitViewer() {
	clearAllScene();
	delete ui;
	killTimer(timerId);
}

void CourseUnitViewer::timerEvent(QTimerEvent *event) {
	Q_UNUSED(event);

	updatePhysics();
	updateLabels();
}

bool CourseUnitViewer::nodesCanMove() {
	return !ui->freezeCheckbox->isChecked();
}

void CourseUnitViewer::on_addNode_clicked() {
	Node *nd = new Node(this);
	QPointF pt = ui->graphicsView->mapToScene(ui->graphicsView->rect().center());
	nd->setPos(pt);
	addNode(nd);
}

bool CourseUnitViewer::deleteModeIsOn() {
	return ui->deleteModeCheckbox->isChecked();
}

void CourseUnitViewer::on_zoomOut_clicked() {
	ui->graphicsView->addScale(SCALE_PER_PUSH);
}

void CourseUnitViewer::on_zoomIn_clicked() {
	ui->graphicsView->addScale(-SCALE_PER_PUSH);
}

double CourseUnitViewer::getAttFac() const {
	return attFac;
}

double CourseUnitViewer::getOwnLength() const {
	return ownLength;
}

double CourseUnitViewer::getRepFac() const {
	return repFac;
}

void CourseUnitViewer::abortDrag() {
	scene->stopDrag();
}

void CourseUnitViewer::clearAllScene() {
	abortDrag();

	setSelectedNode(nullptr);

	QList<Node*> nds;
	for (QGraphicsItem * it : scene->items()) {
		Node * nd = dynamic_cast<Node*>(it);
		if (nd != nullptr) {
			nds.push_back(nd);
		}
	}

	while (!nds.empty()) {
		Node * nd = nds.first();
		nds.removeFirst();
		scene->removeItem(nd);
		delete nd;
	}
}

void CourseUnitViewer::addNode(Node *nd) {
	NOT_NULL(nd);

	scene->addItem(nd);
	emit nodeAdded(nd);
	scene->update();
}

void CourseUnitViewer::addEdge(Edge *e) {
	NOT_NULL(e);

	scene->addItem(e);
}

void CourseUnitViewer::unpack(CourseUnit *head) {
	NOT_NULL(head);

	QMap<QString, Node*> nodes;

	for (CourseUnit * u : head->getEmbedded()) {
		if (nodes.contains(u->objectName())) {
			throw QString("Some course units have same name!");
		}

		Node * nd = new Node(this);
		nd->fromCourseUnitToNode(u);
		nodes[nd->getName()] = nd;
		addNode(nd);
	}

	for (CourseUnit * u : head->getEmbedded()) {
		for (QString v : u->getConnections()) {
			addEdge(new Edge(nodes[u->objectName()], nodes[v], this));
		}
	}

	ui->graphicsView->focusOn();
}

void CourseUnitViewer::pack(CourseUnit *head) {
	NOT_NULL(head);

	head->setFieldSize(scene->width(), scene->height());

	QMap<QString, CourseUnit*> units;

	for (QGraphicsItem * it : scene->items()) {
		if (!it) {
			continue;
		}

		Node * nd = dynamic_cast<Node*>(it);

		if (nd != nullptr) {
			if (units.contains(nd->getName().toLower())) {
				throw QString("Some nodes have same name!");
			}
			CourseUnit * un = new CourseUnit(head);
			head->addEmbedded(un);
			nd->fromNodeToCourseUnit(un);
			units[un->objectName().toLower()] = un;
		}
	}

	for (QGraphicsItem * it : scene->items()) {
		if (!it) {
			continue;
		}

		Edge * ed = dynamic_cast<Edge*>(it);
		if (ed != nullptr) {
			units[ed->getSourceNode()->getName().toLower()]->addConnection(ed->getDestNode()->getName());
		}
	}
}

void CourseUnitViewer::setSceneSize(int w, int h) {
	scene->setSceneRect((double) - w/ 2, (double)- h/2, (double) w, (double) h);
}

QPointF CourseUnitViewer::getSceneSize() {
	return QPointF(scene->width(), scene->height());
}

void CourseUnitViewer::on_repaintAll_stateChanged(int v) {
	if (!v) {
		ui->graphicsView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
	} else {
		ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	}
}

NodeDesign* CourseUnitViewer::getCurrentDesign() {
	ASSERT(this->nodesDesigns.contains(ui->designBox->currentText()));
	return this->nodesDesigns[ui->designBox->currentText()];
}

void CourseUnitViewer::setSelectedNode(Node *nd) {
	Node * tmp = selectedNode;
	selectedNode = nd;
	emit nodeSelected(selectedNode);
	if (nd != nullptr) {
		nd->update();
	}
	if (tmp != nullptr) {
		tmp->update();
	}
}

void CourseUnitViewer::makeProgressToSelected(QString skill, double val) {
	if (selectedNode != nullptr) {
		selectedNode->setProgress(skill, val);
	}
}

void CourseUnitViewer::unpack(StudentProgress *prg) {
	NOT_NULL(prg);

	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		if (Node *node = qgraphicsitem_cast<Node*>(item)) {
			QString nodeName = node->getName();

			for (QString skillName : node->getInSkills().keys()) {
				if (prg->containsLevel(nodeName, skillName)) {
					node->setProgress(skillName, prg->getLevel(nodeName, skillName));
				}
			}
			for (QString skillName : node->getOutSkills().keys()) {
				if (prg->containsLevel(nodeName, skillName)) {
					node->setProgress(skillName, prg->getLevel(nodeName, skillName));
				}
			}
		}
	}
}

void CourseUnitViewer::pack(StudentProgress *prg) {
	NOT_NULL(prg);
	prg->clearProgress();
	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		if (Node *node = qgraphicsitem_cast<Node*>(item)) {
			QString nodeName = node->getName();
			for (QString skillName : node->getProgress().keys()) {
				double val = node->getSkillProgress(skillName);
				prg->addProgress(nodeName, skillName, val);
			}
		}
	}
}

void CourseUnitViewer::clearStudentProgress() {
	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		if (Node *node = qgraphicsitem_cast<Node*>(item)) {
			node->clearStudentProgress();
			node->update();
		}
	}
}

bool CourseUnitViewer::isNodeSelected(Node *nd) {
	return selectedNode == nd;
}

Node* CourseUnitViewer::getSelectedNode() {
	return selectedNode;
}

void CourseUnitViewer::on_designBox_currentTextChanged(QString v) {
	Q_UNUSED(v);

	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		if (Node *node = qgraphicsitem_cast<Node*>(item)) {
			node->update();
		}
	}

	scene->update();
}

bool CourseUnitViewer::isEditable() {
	return editable;
}

void CourseUnitViewer::setEditable(bool v) {
	editable = v;
	ui->addNode->setVisible(v);
	ui->deleteModeCheckbox->setVisible(v);
}

void CourseUnitViewer::setPaths(QMap<QString, QString> paths) {
	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		if (Node *node = qgraphicsitem_cast<Node*>(item)) {
			node->setFile(paths[node->getName()]);
		}
	}
}

Viewport* CourseUnitViewer::getViewport() {
	return ui->graphicsView;
}

void CourseUnitViewer::on_focusOn_clicked() {
	ui->graphicsView->focusOn();
}

void CourseUnitViewer::on_options_clicked() {
	CourseUnitViewerSettings sets(this, total_nodes, total_edges, total_items, attFac, repFac, ownLength);
	sets.exec();
	attFac = sets.attraction;
	repFac = sets.repulsion;
	ownLength = sets.ownLength;
}

void CourseUnitViewer::updatePhysics() {total_nodes = 0;
	total_edges = 0;
	total_items = 0;

	// Update physics
	QList<Node*> nodes;
	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		if (Node *node = qgraphicsitem_cast<Node*>(item)) {
			nodes << node;
			total_nodes++;
		}
		if (qgraphicsitem_cast<Edge*>(item)) {
			total_edges++;
		}
		total_items++;
	}

	for (Node *node : qAsConst(nodes)) {
		node->calculateForces();
	}
}

void CourseUnitViewer::updateLabels() {
	const QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem *item : items) {
		Node *node = qgraphicsitem_cast<Node*>(item);

		if (node != nullptr) {
			for (QString name : labelsLibrary.keys()) {
				labelsLibrary[name]->update(node);
			}
		}
	}
}

void CourseUnitViewer::initLabels() {
	labelsLibrary["Hidden"] = new LabelHidden(this);
	labelsLibrary["Bonus"] = new LabelBonus(this);
}

QMap<QString, Label*>& CourseUnitViewer::getLabelsLibrary() {
	return labelsLibrary;
}
