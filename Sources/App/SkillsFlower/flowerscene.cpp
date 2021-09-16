/*
 * FlowerScene.cpp
 *
 *  Created on: Aug 25, 2021
 *      Author: timat
 */

#include "flowerscene.h"
#include "skillsflower.h"
#include "leaf.h"

FlowerScene::FlowerScene(SkillsFlower * v) : QGraphicsScene(v), view(v) {
	setSceneRect(QRect(0, 0, 550, 550));
}

void FlowerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseMoveEvent(event);
}

void FlowerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseReleaseEvent(event);
}

void FlowerScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mousePressEvent(event);
}

void FlowerScene::drawBackground(QPainter *painter, const QRectF &rect) {
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

    // circles
	QFont f = painter->font();
	f.setPointSizeF(5);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::white);

	QPen p = painter->pen();
	p.setStyle(Qt::DashLine);
	p.setColor(Qt::gray);
	painter->setPen(p);
	painter->setBrush(Qt::NoBrush);

	painter->drawEllipse(width() / 2 - MIN_LEN, height() / 2 - MIN_LEN, 2*MIN_LEN, 2*MIN_LEN);
	painter->drawEllipse(width() / 2 - MAX_LEN, height() / 2 - MAX_LEN, 2*MAX_LEN, 2*MAX_LEN);
	painter->drawEllipse(width() / 2 - EXTRA_LEN, height() / 2 - EXTRA_LEN, 2*EXTRA_LEN, 2*EXTRA_LEN);
}


void FlowerScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
}

void FlowerScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
}

void FlowerScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
}


