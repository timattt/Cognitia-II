/*
 * LabelInvalid.cpp
 *
 *  Created on: Nov 28, 2021
 *      Author: timat
 */

#include "LabelInvalid.h"
#include "../Node/edge.h"
#include "../Node/node.h"

LabelInvalid::LabelInvalid(QObject * parent) : Label("Invalid", QColor(255, 100, 255), parent) {
	setSelectable(false);
	setSaveable(false);
}

LabelInvalid::~LabelInvalid() {
}

void LabelInvalid::update(Node *node) {
	QMap<QString, int> sks = QMap<QString, int>(node->getInSkills());
	for (Edge * e : node->getEdges()) {
		if (e->getDestNode() != node) {
			continue;
		}

		Node * src = e->getSourceNode();

		for (QString sk : src->getOutSkills().keys()) {
			if (sks.contains(sk) && sks[sk] <= src->getOutSkills()[sk]) {
				sks.remove(sk);
			}
		}
	}

	if (sks.size() != 0 && !node->containsLabel("Invalid")) {
		node->addLabel("Invalid");
	}
	if (sks.size() == 0 && node->containsLabel("Invalid")) {
		node->removeLabel("Invalid");
	}
}
