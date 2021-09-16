/*
 * leaf.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: timat
 */

#include "leaf.h"
#include <math.h>
#include "skillsflower.h"

Leaf::~Leaf() {
}


QRectF Leaf::boundingRect() const {
	double len = getLen();
	return QRect( - HANDLER_RAD,  - HANDLER_RAD, 2*HANDLER_RAD, 2*HANDLER_RAD);
}

void Leaf::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget) {
	{
	double len = getLen();
	double x = scene()->width() / 2 + 0.5*(2*len) * cos(angle / 180.0 * 3.1415);
	double y = scene()->height() / 2 + 0.5*(2*len) * sin(angle / 180.0 * 3.1415);
	painter->translate(-x, -y);
	}

	QFont f = painter->font();
	f.setPointSizeF(SMALL_LENGTH / 6);
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

    //----------------
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::white);

	double len = getLen();

	double x = scene()->width() / 2 + 0.5*len * cos(angle / 180.0 * 3.1415);
	double y = scene()->height() / 2 + 0.5*len * sin(angle / 180.0 * 3.1415);

	painter->translate(x, y);
	painter->rotate(angle);

	QRect r = QRect(-len/2, -SMALL_LENGTH, len, 2 * SMALL_LENGTH);

	painter->drawEllipse(r);
	r = QRect(-len/2 + HANDLER_RAD / 2, -SMALL_LENGTH, len - 2*HANDLER_RAD, 2 * SMALL_LENGTH);
	painter->drawText(r, Qt::AlignVCenter | Qt::AlignRight, text);

	painter->rotate(-angle);
	painter->translate(-x, -y);

	//----------------
	painter->setBrush(Qt::yellow);
	painter->drawEllipse(scene()->width() / 2 -  CENTER_RAD, scene()->height() / 2 -  CENTER_RAD, 2*CENTER_RAD, 2*CENTER_RAD);


	x = scene()->width() / 2 + (len) * cos(angle / 180.0 * 3.1415);
	y = scene()->height() / 2 + (len) * sin(angle / 180.0 * 3.1415);

	painter->setBrush(Qt::red);
	r = QRect(x - HANDLER_RAD, y - HANDLER_RAD, 2 * HANDLER_RAD, 2 * HANDLER_RAD);
	painter->drawEllipse(r);
	f = painter->font();
	f.setPointSizeF(HANDLER_RAD / 1.5);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
	painter->drawText(r, Qt::AlignCenter, QString::number(value, 'g', 2));
}

void Leaf::mousePressEvent(QGraphicsSceneMouseEvent *event) {

	pressed = 1;
}

void Leaf::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	if (pressed) {
		double dx = event->scenePos().x() - scene()->width() / 2.0;
		double dy = event->scenePos().y() - scene()->height() / 2.0;
		double len = sqrt(dx * dx + dy * dy);

		if (len < CENTER_RAD + HANDLER_RAD || len > EXTRA_LEN) {
			return;
		}

		setLen(len);

		if (value < 0) {
			value = 0;
		}

		refreshPos();
		scene()->update();

		emit parent->skillLevelChanged(text, value);
	}
}

Leaf::Leaf(double from, double to, double val, QString text, double angle, SkillsFlower * parent) : from(from), to(to), value(val), text(text), angle(angle), parent(parent) {
}

double Leaf::getLen() const {
	if (to == from) {
		exit(-1);
	}
	return (value - from) / (to - from) * (MAX_LEN - MIN_LEN) + MIN_LEN;
}

void Leaf::setLen(double len) {
	value = (len - MIN_LEN) / (MAX_LEN - MIN_LEN) * (to - from) + from;
}

void Leaf::refreshPos() {
	double len = getLen();
	double x = scene()->width() / 2 + 0.5*(2*len) * cos(angle / 180.0 * 3.1415);
	double y = scene()->height() / 2 + 0.5*(2*len) * sin(angle / 180.0 * 3.1415);
	setPos(x, y);
}

void Leaf::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	pressed = 0;
}

QString Leaf::getText() {
	return text;
}

double Leaf::getValue() {
	return value;
}
