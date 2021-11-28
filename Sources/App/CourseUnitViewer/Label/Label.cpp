/*
 * Label.cpp
 *
 *  Created on: Nov 28, 2021
 *      Author: timat
 */

#include "Label.h"

Label::Label(QString name_, QColor color_, QObject * parent) : QObject(parent), name(name_), color(color_), selectable(true), saveable(true) {
}

Label::~Label() {
}

const QColor& Label::getColor() const {
	return color;
}

const QString& Label::getName() const {
	return name;
}

bool Label::isSelectable() const {
	return selectable;
}

bool Label::isSaveable() const {
	return saveable;
}

void Label::setSaveable(bool saveable) {
	this->saveable = saveable;
}

void Label::setSelectable(bool selectable) {
	this->selectable = selectable;
}

void Label::update(Node *node) {
	Q_UNUSED(node);
}
