#include "courseunitviewer.h"
#include "Node/edge.h"
#include "Node/node.h"
#include "coursescene.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "ui_courseunitviewer.h"

#define SCALE_PER_PUSH 1.3

CourseUnitViewer::CourseUnitViewer(QWidget *parent) :
		QWidget(parent), ui(new Ui::CourseUnitViewer), scene(
				new CourseScene(this)), attFac(DEFAULT_ATT_FAC), repFac(
				DEFAULT_REP_FAC), massFac(DEFAULT_MASS_FAC) {
	ui->setupUi(this);
	ui->graphicsView->setScene(scene);
	timerId = startTimer(1);

	ui->repFact->setText(QString::number(DEFAULT_REP_FAC));
	ui->massFac->setText(QString::number(DEFAULT_MASS_FAC));
	ui->attFact->setText(QString::number(DEFAULT_ATT_FAC));

	ui->graphicsView->setAcceptDrops(true);

	ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
}

CourseUnitViewer::~CourseUnitViewer() {
	delete ui;
	delete scene;
	killTimer(timerId);
}

void CourseUnitViewer::timerEvent(QTimerEvent *event) {
	Q_UNUSED(event);

	int total_nodes = 0;
	int total_edges = 0;
	int total_items = 0;

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

	// Update quantity
	ui->totalItems->setText(QString::number(total_items));
	ui->totalEdges->setText(QString::number(total_edges));
	ui->totalNodes->setText(QString::number(total_nodes));
}

bool CourseUnitViewer::nodesCanMove() {
	return !ui->freezeCheckbox->isChecked();
}

void CourseUnitViewer::on_pushButton_2_clicked() {
	Node *nd = new Node(this);
	QPointF pt = ui->graphicsView->mapToScene(
			ui->graphicsView->rect().center());
	nd->setPos(pt);
	addNode(nd);
}

bool CourseUnitViewer::deleteModeIsOn() {
	return ui->deleteModeCheckbox->isChecked();
}

void CourseUnitViewer::on_zoomOut_clicked() {
	ui->graphicsView->scale(1.0 / SCALE_PER_PUSH, 1.0 / SCALE_PER_PUSH);
	ui->zoomPercentage->setText(
			QString::number(
					ui->zoomPercentage->text().split("%")[0].toDouble() / SCALE_PER_PUSH)
					+ "%");
}

void CourseUnitViewer::on_zoomIn_clicked() {
	ui->graphicsView->scale(SCALE_PER_PUSH, SCALE_PER_PUSH);
	ui->zoomPercentage->setText(
			QString::number(
					ui->zoomPercentage->text().split("%")[0].toDouble() * SCALE_PER_PUSH)
					+ "%");
}

void CourseUnitViewer::on_areaDec_clicked() {
	QRectF r = scene->sceneRect();
	QRectF n = QRectF(r);
	n.setWidth(n.width() / 2);
	n.setHeight(n.height() / 2);
	scene->setSceneRect(n);
	emit sceneSizeChanged(n.width(), n.height());
}

void CourseUnitViewer::on_areaIn_clicked() {
	QRectF r = scene->sceneRect();
	QRectF n = QRectF(r);
	n.setWidth(n.width() * 2);
	n.setHeight(n.height() * 2);
	scene->setSceneRect(n);
	emit sceneSizeChanged(n.width(), n.height());
}

double CourseUnitViewer::getAttFac() const {
	return attFac;
}

double CourseUnitViewer::getMassFac() const {
	return massFac;
}

double CourseUnitViewer::getRepFac() const {
	return repFac;
}

void CourseUnitViewer::on_attFact_textChanged(const QString &arg1) {
}

void CourseUnitViewer::on_repFact_textChanged(const QString &arg1) {
}

void CourseUnitViewer::on_massFac_textChanged(const QString &arg1) {
}

void CourseUnitViewer::on_attFact_editingFinished() {
	bool ok = 0;
	double v = ui->attFact->text().toDouble(&ok);
	if (!ok) {
		ui->attFact->setText(QString::number(DEFAULT_ATT_FAC));
	} else {
		attFac = v;
	}
}

void CourseUnitViewer::on_repFact_editingFinished() {
	bool ok = 0;
	double v = ui->repFact->text().toDouble(&ok);
	if (!ok) {
		ui->repFact->setText(QString::number(DEFAULT_REP_FAC));
	} else {
		repFac = v;
	}
}

void CourseUnitViewer::abortDrag() {
	scene->stopDrag();
}

void CourseUnitViewer::on_massFac_editingFinished() {
	bool ok = 0;
	double v = ui->massFac->text().toDouble(&ok);
	if (!ok) {
		ui->massFac->setText(QString::number(DEFAULT_MASS_FAC));
	} else {
		massFac = v;
	}
}

void CourseUnitViewer::clearAllScene() {
	abortDrag();

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
	scene->addItem(nd);
	emit nodeAdded(nd);
}

void CourseUnitViewer::addEdge(Edge *e) {
	scene->addItem(e);
}

void CourseUnitViewer::unpack(CourseUnit *head) {
	QMap<QString, Node*> nodes;

	for (CourseUnit * u : head->getEmbedded()) {
		if (nodes.contains(u->objectName())) {
			throw QString("Some course units have same name!");
		}

		Node * nd = new Node(this);
		fromCourseUnitToNode(u, nd);
		nodes[nd->getName()] = nd;
		addNode(nd);
	}

	for (CourseUnit * u : head->getEmbedded()) {
		for (QString v : u->getConnections()) {
			addEdge(new Edge(nodes[u->objectName()], nodes[v]));
		}
	}
}

void CourseUnitViewer::pack(CourseUnit *head) {
	QMap<QString, CourseUnit*> units;

	for (QGraphicsItem * it : scene->items()) {
		if (!it) {
			continue;
		}

		Node * nd = dynamic_cast<Node*>(it);

		if (nd != nullptr) {
			if (units.contains(nd->getName())) {
				throw QString("Some nodes have same name!");
			}
			CourseUnit * un = new CourseUnit(head);
			head->addEmbedded(un);
			fromNodeToCourseUnit(nd, un);
			units[un->objectName()] = un;
		}
	}

	for (QGraphicsItem * it : scene->items()) {
		if (!it) {
			continue;
		}

		Edge * ed = dynamic_cast<Edge*>(it);
		if (ed != nullptr) {
			units[ed->sourceNode()->getName()]->addConnection(ed->destNode()->getName());
		}
	}
}

void CourseUnitViewer::setSceneSize(int w, int h) {
	QRectF r = scene->sceneRect();
	scene->setSceneRect((double) r.x(), (double) r.y(), (double) w, (double) h);
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
