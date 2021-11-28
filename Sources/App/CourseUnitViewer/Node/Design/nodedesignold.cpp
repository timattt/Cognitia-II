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
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QString name = nd->getName();

	double side = 2*NODE_RAD;

	// paint parameters
	//========================================
	QFont f = painter->font();
	f.setPointSizeF(side /8 * 0.6);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
    painter->setPen(QPen(Qt::black, NODE_RAD / 100.0));
    //========================================

    // main background
    //========================================
    //painter->setBrush(Qt::lightGray);
    //painter->drawRect(-side/2, -side/2, side, side);
    //========================================

    // color
    //========================================
	if (nd->isSelected()) {
		painter->setBrush(SELECTED_NODE_COLOR);
	} else {
		painter->setBrush(nd->getColor());
	}
    //========================================

    // main rectangle
    //========================================
    QRect mainRect = QRect(-side/2, -side/2,side, side/8);
    painter->drawRect(mainRect);
    //========================================

    painter->setBrush(Qt::white);

    if (nd->containsLabel("Hidden")) {
    	QRect descRect = QRect(-side / 2, -side / 8 * 3, side, side / 8 * 3);
    	f = painter->font();
    	f.setPointSizeF(side / 15 * 3);
    	f.setBold(true);
    	f.setWeight(QFont::ExtraBold);
    	painter->setFont(f);
    	painter->drawRect(descRect);
    	descRect = QRect(-side / 2, -side / 8 * 3, side, side / 8 * 3 - side/10);
    	painter->drawText(descRect, Qt::AlignCenter, "?");
    } else {

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
			if (nd->isSelected()) {
				painter->setBrush(SELECTED_NODE_COLOR);
			} else {
				painter->setBrush(nd->getColor());
			}
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

		/*
		if (nd->containsProgress(key) && nd->getProgressScalar(key) > 0) {
			r = QRect(0.4*w, -h / 2, 0.1*w, h);
			painter->drawRect(r);

			r = QRect(0.4*w, -h / 2 + h * (1 - nd->getProgressScalar(key)), 0.1*w, (double)h * nd->getProgressScalar(key));
			QPen p = painter->pen();
			painter->setPen(Qt::NoPen);
			painter->setBrush(Qt::blue);
			painter->drawRect(r);
			painter->setPen(p);
			painter->setBrush(Qt::white);

			painter->setBrush(Qt::NoBrush);
			r = QRect(0.4*w, -h / 2, 0.1*w, h);
			painter->drawRect(r);
			painter->setBrush(Qt::white);
		}
		*/

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
			if (nd->isSelected()) {
				painter->setBrush(SELECTED_NODE_COLOR);
			} else {
				painter->setBrush(nd->getColor());
			}
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

		if (nd->containsProgress(key) && nd->getProgressScalar(key) > 0) {
			r = QRect(0.4*w, -h / 2, 0.1*w, h);
			painter->drawRect(r);

			r = QRect(0.4*w, -h / 2 + h * (1 - nd->getProgressScalar(key)), 0.1*w, (double)h * nd->getProgressScalar(key));
			QPen p = painter->pen();
			painter->setPen(Qt::NoPen);
			painter->setBrush(Qt::blue);
			painter->drawRect(r);
			painter->setPen(p);
			painter->setBrush(Qt::white);

			painter->setBrush(Qt::NoBrush);
			r = QRect(0.4*w, -h / 2, 0.1*w, h);
			painter->drawRect(r);
			painter->setBrush(Qt::white);
		}

		painter->translate(-x,- y );

		i++;
	}

    }

    if (nd->containsLabel("Bonus")) {
    	QRect r = QRect(-side / 2, -side / 10, side, side / 10);
    	f = painter->font();
    	f.setPointSizeF(side / 15);
    	f.setBold(true);
    	f.setWeight(QFont::ExtraBold);
    	painter->setFont(f);
    	painter->setBrush(Qt::red);
    	painter->drawRect(r);
    	painter->drawText(r, Qt::AlignCenter, "Bonus");
    }
}

bool NodeDesignOld::verticalSkillsLayout() {
	return false;
}

double NodeDesignOld::getEdgeThickness() {
	return 0.01;
}

QPainterPath NodeDesignOld::getShape() {
    QPainterPath path;
	double side = 2*NODE_RAD;
    path.addRect(-side/2, -side/2,side, side/2);
    return path;
}
