/*
 * nodedesign.cpp
 *
 *  Created on: Sep 11, 2021
 *      Author: timat
 */

#include "nodedesign.h"

NodeDesign::NodeDesign(QObject * parent) : QObject(parent) {}
NodeDesign::~NodeDesign() {}

void NodeDesign::draw(Node *nd, QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(nd);
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
}

bool NodeDesign::verticalSkillsLayout() {
	return false;
}

bool NodeDesign::edgeToCenter() {
	return false;
}

QString NodeDesign::rebuildStr(QString str, int max) {
	QStringList lst = str.split(" ");

	QStringList res = QStringList(lst.size());

	int added = 0;
	for (int i = 0, total = lst.join("").length(); added < max && added < total; i++) {
		int partNumber = i % lst.size();
		int partSize = lst[partNumber].length();
		int partIndex = res[partNumber].size();

		if (partIndex < partSize) {
			QChar v = lst[partNumber][partIndex];
			if (partIndex == 0) {
				v = v.toUpper();
			}
			res[partNumber] += v;
			added++;
		}

	}

	return res.join("");
}

double NodeDesign::getEdgeThickness() {
	return 0.05;
}
