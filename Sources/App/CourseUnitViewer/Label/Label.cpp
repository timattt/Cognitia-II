/*
 * Label.cpp
 *
 *  Created on: Nov 28, 2021
 *      Author: timat
 */

#include "Label.h"

Label::Label(QString name_, QColor color_, QObject * parent) : QObject(parent), name(name_), color(color_) {
}

Label::~Label() {
}

void Label::update(Node *node) {
	Q_UNUSED(node);
}
