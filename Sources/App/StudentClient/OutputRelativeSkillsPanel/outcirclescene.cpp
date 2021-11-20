/*
 * circlescene.cpp
 *
 *  Created on: Nov, 19, 2021
 *      Author: ZoomZero
 */

#include "outcirclescene.h"
#include "outputrelativeskillspanel.h"
#include "outcircle.h"
#include "../../Core/logger.h"
#include <QtWidgets>

OutCircleScene::OutCircleScene(OutputRelativeSkillsPanel * v) : QGraphicsScene(v), view(v) {
    NOT_NULL(v);
    setSceneRect(QRect(0, 0, 400, 600));
}

void OutCircleScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);
}

void OutCircleScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
}

void OutCircleScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
}

void OutCircleScene::drawBackground(QPainter *painter, const QRectF &rect) {
    // Shadow
    QRectF sceneRect = this->sceneRect();
    sceneRect = QRectF(sceneRect.x() - 20, sceneRect.y() - 20, sceneRect.width() + 40, sceneRect.height() + 40);
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
