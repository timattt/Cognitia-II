/*
 * nodedesignold.cpp
 *
 *  Created on: Sep 11, 2021
 *      Author: timat
 */

#include "nodedesignold.h"
#include "../node.h"

NodeDesignOld::NodeDesignOld(QObject * parent) : NodeDesign(parent) {
}

NodeDesignOld::~NodeDesignOld() {
}

void NodeDesignOld::draw(Node *nd, QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QString name = nd->getName();

	double side = 2*NODE_RAD;

	double height = NODE_RAD / 3;
	double width = 2*NODE_RAD;


	// paint parameters
	//========================================
	QFont f = painter->font();
	f.setPointSizeF(side /8 * 0.6);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
    painter->setPen(QPen(Qt::black, NODE_RAD / 100.0));
    painter->setBrush(nd->getColor());
    //========================================

    // main rectangle
    //========================================
    QRect mainRect = QRect(-side/2, -side/2,side, side/8);
    painter->drawRect(mainRect);
    //========================================

    painter->setBrush(Qt::white);

    // text
    //========================================
    QRect textRect = QRect(-side/2 * 0.9, -side/2 * 1.01, side * 0.9, side/8);
    int maxChars = 14;
    QString nameFinal = rebuildStr(name, maxChars);
    painter->drawText(textRect, Qt::AlignCenter, nameFinal);
    //========================================

    // Description
	f = painter->font();
	f.setPointSizeF(side / 20.0);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
	QRect descRect = QRect(-side / 2, -side / 8 * 3, side, side / 8 * 3);
	painter->drawRect(descRect);
	descRect = QRect(-side / 2 * 0.9, -side / 8 * 3, side * 0.9, side / 8 * 3);
	painter->drawText(descRect, Qt::AlignLeft | Qt::TextWrapAnywhere, nd->getDescription());

    // Skills
	// In
	int total = nd->getInSkills().size() + 1;
	double h = side / 8;
	double w = side *0.45;

	double step = qMin(h, side / 2 / (double)total);

	int i = 0;
	for (QString key : nd->getInSkills().keys()) {

		double x = -side / 4;
		double y = ((double)i + 0.5) * step;

		painter->translate(x, y);

		QRect r = QRect(-w / 2, -h / 2, w, h);

		if (i == 0) {
			painter->setBrush(nd->getColor());
			painter->drawRect(r);
			painter->drawText(r, Qt::AlignCenter, "IN:");
			i++;
			painter->translate(-x,- y );
			y = ((double)i + 0.5) * step;
			painter->translate(x, y);
			painter->setBrush(Qt::white);
		}

		painter->drawRect(r);
		painter->drawText(r, Qt::AlignCenter, rebuildStr(key + " " + QString::number(nd->getInSkills()[key]), 8));

		painter->translate(-x,- y);

		i++;
	}

	// Out
	total = nd->getOutSkills().size() + 1;
	h = side / 8;
	w = side *0.45;

	step = qMin(h, side / 2 / (double)total);

	i = 0;
	for (QString key : nd->getOutSkills().keys()) {

		double x = side / 4;
		double y = ((double)i + 0.5) * step;

		painter->translate(x, y);

		QRect r = QRect(-w / 2, -h / 2, w, h);

		if (i == 0) {
			painter->setBrush(nd->getColor());
			painter->drawRect(r);
			painter->drawText(r, Qt::AlignCenter, "OUT:");
			i++;
			painter->translate(-x,- y );
			y = ((double)i + 0.5) * step;
			painter->translate(x, y);
			painter->setBrush(Qt::white);
		}

		painter->drawRect(r);
		painter->drawText(r, Qt::AlignCenter, rebuildStr(key + " " + QString::number(nd->getOutSkills()[key]), 8));

		painter->translate(-x,- y );

		i++;
	}
}

bool NodeDesignOld::verticalSkillsLayout() {
	return false;
}

bool NodeDesignOld::edgeToCenter() {
	return false;
}

double NodeDesignOld::getEdgeThickness() {
	return 0.01;
}
