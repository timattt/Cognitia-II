/*
 * circlescene.cpp
 *
 *  Created on: Oct 26, 2021
 *      Author: ZoomZero
 */

#include "circlescene.h"
#include "inputabsoluteskillspanel.h"
#include "circle.h"
#include "../../Core/logger.h"
#include <QtWidgets>

CircleScene::CircleScene(InputAbsoluteSkillsPanel * v) : QGraphicsScene(v), view(v) {
    NOT_NULL(v);
    setSceneRect(QRect(0, 0, 900, 500));
}

void CircleScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);
}

void CircleScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
}

void CircleScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
}

void CircleScene::drawBackground(QPainter *painter, const QRectF &rect) {
    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, QColorConstants::Svg::whitesmoke);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, QColorConstants::Svg::whitesmoke);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, QColorConstants::Svg::oldlace);
    gradient.setColorAt(1, QColorConstants::Svg::white);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}
