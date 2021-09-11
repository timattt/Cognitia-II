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
}

bool NodeDesign::verticalSkillsLayout() {
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
			QChar v = lst[partNumber][partIndex].toUpper();
			if (partIndex == 0) {
				v = v.toUpper();
			}
			res[partNumber] += v;
			added++;
		}

	}

	return res.join("");
}
