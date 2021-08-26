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

CourseScene::CourseScene(CourseUnitViewer * view) : dragEdge(nullptr), totalNodes(0), totalEdges(0), view(view) {
	setSceneRect(QRect(0, 0, 100, 100));
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
	QGraphicsItem *it = this->itemAt(event->lastScenePos(), QTransform());
	if (it != nullptr) {
		Node * nd = dynamic_cast<Node*>(it);
		Edge * ed = dynamic_cast<Edge*>(it);\

		if (nd != nullptr || ed != nullptr) {
			if (view->deleteModeIsOn()) {
				delete it;
				return;
			} else {
				if (nd != nullptr && event->button() == Qt::RightButton) {
					addItem(dragEdge = new Edge(nd));
					dragEdge->setTarget(event->lastScenePos());
					return;
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
