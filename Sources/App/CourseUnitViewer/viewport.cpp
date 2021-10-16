/*
 * viewport.cpp
 *
 *  Created on: Oct 15, 2021
 *      Author: timat
 */

#include "viewport.h"
#include "../Core/logger.h"
#include "Node/Design/nodedesign.h"
#include "Node/node.h"

Viewport::Viewport(QWidget * parent) : QGraphicsView(parent), prev(-1, -1), pressed(0), cameraPos(0, 0), curScale(1) {}

void Viewport::resizeEvent(QResizeEvent *event) {
	focusOn();
	QGraphicsView::resizeEvent(event);
}

void Viewport::mouseMoveEvent(QMouseEvent *event) {
	if (pressed) {
		QPointF pos = mapToScene(event->pos());
		QPointF delta = pos - prev;

		moveCamera(delta);

		prev = pos;
	}
	QGraphicsView::mouseMoveEvent(event);
}

void Viewport::mousePressEvent(QMouseEvent *event) {
	prev = mapToScene(event->pos());

	if (event->button() == Qt::LeftButton && scene()->items(prev).size() == 0) {
		pressed = true;
	} else {
		pressed = false;
	}

	QGraphicsView::mousePressEvent(event);
}

void Viewport::addScale(float v) {
	QRectF r = scene()->sceneRect();

	float w = r.width() / curScale;
	float h = r.height() / curScale;

	curScale += v;
	curScale = qMax(0.1f, curScale);

	w *= curScale;
	h *= curScale;

	scene()->setSceneRect(QRectF(-w / 2, -h / 2, w, h));
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	scene()->update();
}

void Viewport::mouseReleaseEvent(QMouseEvent *event) {
	pressed = false;
	QGraphicsView::mouseReleaseEvent(event);
}

void Viewport::moveCamera(QPointF delta) {
	for (QGraphicsItem *it : scene()->items()) {
		Node * nd = dynamic_cast<Node*>(it);
		if (nd != nullptr) {
			nd->moveBy(delta.x(), delta.y());
		}
	}
	cameraPos += delta;
}

void Viewport::focusOn() {
	double minX = 100000;
	double minY = 100000;
	double maxX = -100000;
	double maxY = -100000;

	setCameraPos({0, 0});

	for (QGraphicsItem *it : scene()->items()) {
		minX = qMin(minX, it->pos().x());
		minY = qMin(minY, it->pos().y());
		maxX = qMax(maxX, it->pos().x());
		maxY = qMax(maxY, it->pos().y());
	}
	if (scene()->items().size() == 0) {
		minX = minY = maxX = maxY = 0;
	}

	double factor = qMax(qMax(1.0, (double)(maxX - minX+ 6 * NODE_RAD) /(double)width()), (double)(maxY - minY+ 6 * NODE_RAD) /(double)height());

	double w = width() * factor;
	double h = height() * factor;

	double x = (minX + maxX) / 2;
	double y = (minY + maxY) / 2;

	setCameraPos({-x, -y});

	scene()->setSceneRect(QRectF(-w/2, -h/2, w, h));
	scene()->update();
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	curScale = 1;
}

void Viewport::setCameraPos(QPointF p) {
	for (QGraphicsItem *it : scene()->items()) {
		Node * nd = dynamic_cast<Node*>(it);
		if (nd != nullptr) {
			nd->moveBy(-cameraPos.x(), -cameraPos.y());
			nd->moveBy(p.x(), p.y());
		}
	}
	cameraPos = p;
}

void Viewport::setScale(float v) {
	ASSERT(v > 0);
	addScale(v - curScale);
}

QPointF Viewport::getCameraPos() const {
	return cameraPos;
}

float Viewport::getCurScale() const {
	return curScale;
}

void Viewport::wheelEvent(QWheelEvent *event) {
	float v = event->angleDelta().y() / 750.0;
	addScale(v);

	QPointF d = {event->position().x() - width() / 2, event->position().y() - height() / 2};

	d *= curScale;

	moveCamera((v / (1+v)) * d);

	QGraphicsView::wheelEvent(event);
}
