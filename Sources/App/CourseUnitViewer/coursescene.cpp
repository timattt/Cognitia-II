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

CourseScene::CourseScene(CourseUnitViewer * view) : dragEdge(nullptr), totalNodes(0), totalEdges(0), view(view) {
	setSceneRect(QRect(0, 0, 300, 300));
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

		if (nd != nullptr && nd != dragEdge->sourceNode() && !nd->hasEdgeToNode(dragEdge->sourceNode())) {
			dragEdge->connectToNode(nd);
		} else {
			delete dragEdge;
		}

		dragEdge = nullptr;
	} else {
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
				view->setSelectedNode(nd);
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
				if (event->button() == Qt::RightButton) {
					addItem(dragEdge = new Edge(nd));
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

void CourseScene::drawBackground(QPainter *painter, const QRectF &rect) {
    // Shadow
    QRectF sceneRect = this->sceneRect();
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
		int lev = divs[1].toInt(&ok);

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

		emit view->nodeSkillsChanged(nd);
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

		emit view->nodeSkillsChanged(nd);
		return;
	}
}

void CourseScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
}

void CourseScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
}

void CourseScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
}

void CourseScene::stopDrag() {
	if (dragEdge != nullptr) {
		delete dragEdge;
		dragEdge = nullptr;
	}
}
