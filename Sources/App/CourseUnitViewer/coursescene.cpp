/*
 * coursescene.cpp
 *
 *  Created on: Aug 25, 2021
 *      Author: timat
 */

#include "coursescene.h"
#include "Node/edge.h"
#include "Node/node.h"
#include "courseunitviewer.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Core/logger.h"
#include "Label/Label.h"

CourseScene::CourseScene(CourseUnitViewer * v) : QGraphicsScene(v), dragEdge(nullptr), view(v) {
	NOT_NULL(v);

	setSceneRect(QRect(-START_SCENE_WIDTH / 2, - START_SCENE_HEIGHT / 2, START_SCENE_WIDTH, START_SCENE_HEIGHT));
}

void CourseScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	if (dragEdge != nullptr) {
		dragEdge->setTarget(event->lastScenePos());
	} else {
		QGraphicsScene::mouseMoveEvent(event);
	}
}

void CourseScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	if (dragEdge != nullptr) {
		QList<QGraphicsItem *> its = this->items(event->lastScenePos());

		Node * nd = nullptr;

		for (QGraphicsItem * it : its) {
			if (it != nullptr && (nd = dynamic_cast<Node*>(it)) != nullptr) {
				break;
			}
		}

		Node * src = dragEdge->getSourceNode();

		if (nd != nullptr && nd != dragEdge->getSourceNode() && !nd->hasEdgeToNode(dragEdge->getSourceNode())) {
			dragEdge->connectToNode(nd);
		} else {
			delete dragEdge;
		}

		dragEdge = nullptr;

		if (nd == src && event->button() == Qt::RightButton) {
			makeMenu(event);
		}
	} else {
		if (event->button() == Qt::RightButton) {
			makeMenu(event);
		}
		QGraphicsScene::mouseReleaseEvent(event);
	}
}

void CourseScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QList<QGraphicsItem*> its = this->items(event->lastScenePos());

	Node * nd = nullptr;
	Edge * ed = nullptr;

	for (QGraphicsItem * it : its) {
		if (nd == nullptr) {
			nd = dynamic_cast<Node*>(it);
		}
		if (ed == nullptr) {
			ed = dynamic_cast<Edge*>(it);
		}
	}

	if (nd == nullptr) {
		view->setSelectedNode(nullptr);
	}

	if (nd != nullptr || ed != nullptr) {
		if (view->deleteModeIsOn()) {
			if (nd != nullptr) {
				view->setSelectedNode(nullptr);
			}
			if (nd != nullptr) {
				emit view->nodeDeleted(nd);
				delete nd;
				return;
			}
			if (ed != nullptr) {
				emit view->edgeDeleted(ed);
				delete ed;
				return;
			}
		} else {
			if (nd != nullptr) {
				if (event->button() == Qt::RightButton && view->isEditable()) {
					addItem(dragEdge = new Edge(nd, view));
					dragEdge->setTarget(event->lastScenePos());
					return;
				} else {
					view->setSelectedNode(nd);
				}
			}
		}
	}

	if (!view->deleteModeIsOn() && event->button() == Qt::LeftButton) {
		QGraphicsScene::mousePressEvent(event);
	}
}

CourseScene::~CourseScene() {
	if (dragEdge != nullptr) {
		delete dragEdge;
	}
}

void CourseScene::drawBackground(QPainter *painter, const QRectF &rect) {
    // Shadow
    QRectF sceneRect = this->sceneRect();
    double bias = 0.1;
    double w = sceneRect.width();
    double h = sceneRect.height();
    double x = sceneRect.x();
    double y = sceneRect.y();
    sceneRect = {x - w * bias / 2, y - h * bias / 2, w + w * bias, h + h * bias};

    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(1, Qt::gray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void CourseScene::dropEvent(QGraphicsSceneDragDropEvent *event) {
	event->accept();

	QList<QGraphicsItem*> its = this->items(event->scenePos());

	Node *nd = nullptr;

	for (QGraphicsItem *it : its) {
		if (it != nullptr && (nd = dynamic_cast<Node*>(it)) != nullptr) {
			break;
		}
	}

	if (nd == nullptr) {
		return;
	}

	if (event->mimeData()->text().contains(QString(SKILL_PACK_DELIMITER) + QString(SKILL_PACK_DELIMITER))) {

		QStringList divs = event->mimeData()->text().split(QString(SKILL_PACK_DELIMITER) + QString(SKILL_PACK_DELIMITER));

		bool ok = 0;

		QString name = divs[0];
		divs[1].toInt(&ok);

		if (!ok) {
			return;
		}

		if (view->getCurrentDesign()->verticalSkillsLayout()) {
			if (nd->mapFromScene(event->scenePos()).y() > 0) {
				nd->removeOutSkill(name);
			} else {
				nd->removeInSkill(name);
			}
		} else {
			if (nd->mapFromScene(event->scenePos()).x() > 0) {
				nd->removeOutSkill(name);
			} else {
				nd->removeInSkill(name);
			}
		}

		return;
	}
	if (event->mimeData()->text().contains(QString(SKILL_PACK_DELIMITER))) {

		QStringList divs = event->mimeData()->text().split(
				QString(SKILL_PACK_DELIMITER));

		bool ok = 0;

		QString name = divs[0];
		int lev = divs[1].toInt(&ok);

		if (!ok) {
			return;
		}

		if (view->getCurrentDesign()->verticalSkillsLayout()) {
			if (nd->mapFromScene(event->scenePos()).y() > 0) {
				nd->addOutSkill(name, lev);
			} else {
				nd->addInSkill(name, lev);
			}
		} else {
			if (nd->mapFromScene(event->scenePos()).x() > 0) {
				nd->addOutSkill(name, lev);
			} else {
				nd->addInSkill(name, lev);
			}
		}

		return;
	}
}

void CourseScene::stopDrag() {
	if (dragEdge != nullptr) {
		delete dragEdge;
		dragEdge = nullptr;
	}
}

void CourseScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
	// Do not delete this function. It is used in drag and drop skill operation
	Q_UNUSED(event);
}

void CourseScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
	// Do not delete this function. It is used in drag and drop skill operation
	Q_UNUSED(event);
}

void CourseScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
	// Do not delete this function. It is used in drag and drop skill operation
	Q_UNUSED(event);
}

void CourseScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	QList<QGraphicsItem*> its = this->items(event->lastScenePos());

	Node *nd = nullptr;
	Edge *ed = nullptr;

	for (QGraphicsItem *it : its) {
		if (nd == nullptr) {
			nd = dynamic_cast<Node*>(it);
		}
		if (ed == nullptr) {
			ed = dynamic_cast<Edge*>(it);
		}
	}

	if (nd != nullptr && !view->deleteModeIsOn() && dragEdge == nullptr) {
		emit view->nodeDoubleClicked(nd);
	}
}

void CourseScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	QGraphicsScene::contextMenuEvent(event);
}

void CourseScene::makeMenu(QGraphicsSceneMouseEvent * event) {
	if (!view->isEditable()) {
		return;
	}

	QMenu menu(this->view);
	menu.addAction("New node");
	menu.addAction("Show all");
	menu.addAction("Smart hide");

	QList<QGraphicsItem*> its = this->items(event->scenePos());

	Node *nd = nullptr;
	Edge *ed = nullptr;

	for (QGraphicsItem *it : its) {
		if (nd == nullptr) {
			nd = dynamic_cast<Node*>(it);
		}
		if (ed == nullptr) {
			ed = dynamic_cast<Edge*>(it);
		}
	}

	if (nd != nullptr || ed != nullptr) {
		menu.addAction("Delete");
	}

	QMenu labelsMenu("Labels", &menu);
	if (nd != nullptr) {

		bool hasSomething = 0;

		for (QString name : this->view->getLabelsLibrary().keys()) {
			Label * label = this->view->getLabelsLibrary()[name];
			if (!label->isSelectable()) {
				continue;
			}
			hasSomething = 1;
			QAction * ac = labelsMenu.addAction(name);
			ac->setCheckable(true);
			ac->setChecked(nd->containsLabel(name));
		}
		if (hasSomething) {
			menu.addMenu(&labelsMenu);
		}
	}

	QAction * a = menu.exec(event->screenPos());

	if (a == nullptr) {
		return;
	}

	QString t = a->text();

	if (t == "New node") {
		Node *nd_ = new Node(this->view);
		QPointF pt = event->scenePos();
		nd_->setPos(pt);
		this->view->registerNodeToScene(nd_);
	}
	if (t == "Delete") {
		if (nd != nullptr) {
			view->setSelectedNode(nullptr);
			emit view->nodeDeleted(nd);
			delete nd;
		}
		else if (ed != nullptr) {
			emit view->edgeDeleted(ed);
			delete ed;
		}
	}
	for (QString name : this->view->getLabelsLibrary().keys()) {
		//Label * label = this->view->getLabelsLibrary()[name];
		if (name == t) {
			if (nd->containsLabel(name)) {
				nd->removeLabel(name);
			} else {
				nd->addLabel(name);
			}
		}
	}
	if (t == "Show all") {
		its = this->items();

		nd = nullptr;

		for (QGraphicsItem *it : its) {
			nd = dynamic_cast<Node*>(it);
			if (nd != nullptr) {
				nd->removeLabel("Hidden");
			}
		}
	}
	if (t == "Smart hide") {
		its = this->items();

		nd = nullptr;

		for (QGraphicsItem *it : its) {
			nd = dynamic_cast<Node*>(it);
			if (nd != nullptr) {
				nd->removeLabel("Hidden");
				if (nd->getIncomeEdgesCount() > 0) {
					nd->addLabel("Hidden");
				}
			}
		}
	}

}
