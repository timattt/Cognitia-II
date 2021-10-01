#include "nodedesignformal.h"
#include <QtWidgets>
#include "../node.h"

void NodeDesignFormal::draw(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QString name = nd->getName();

	double height = NODE_RAD / 3;
	double width = 2*NODE_RAD;

	// paint parameters
	//========================================
	QFont f = painter->font();
	f.setPointSizeF(height * 0.7);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
    painter->setPen(QPen(Qt::black, NODE_RAD / 25.0));
    painter->setBrush(Qt::white);
    //========================================

    QPen p = painter->pen();
	if (nd->isSelected()) {
		p.setColor(SELECTED_NODE_COLOR);
	} else {
		p.setColor(nd->getColor());
	}
    p.setStyle(Qt::DashLine);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(-NODE_RAD, -NODE_RAD, NODE_RAD * 2, NODE_RAD * 2);

	// paint parameters
	//========================================
	f = painter->font();
	f.setPointSizeF(height * 0.7);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
    painter->setPen(QPen(Qt::black, NODE_RAD / 25.0));
    painter->setBrush(Qt::white);
    //========================================

    // main rectangle
    //========================================
    QRect mainRect = QRect(-width / 2, -height / 2, width, height);
    painter->drawRoundedRect(mainRect, height / 8, height / 8);
    //========================================

    // text
    //========================================
    QRect textRect = QRect(-width / 2 * 0.95, -height / 2, width * 0.95, height * 0.9);
    int maxChars = width / 11;
    QString nameFinal = rebuildStr(name, maxChars);
    painter->drawText(textRect, Qt::AlignCenter, nameFinal);
    //========================================

    // Skills
	// In
	int total = nd->getInSkills().size();
	double w = qMin(width / (double) total, width / 8);
	double h = NODE_RAD * 5 / 6;
	f = painter->font();
	f.setPointSizeF(w * 0.6);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
	int i = 0;
	for (QString key : nd->getInSkills().keys()) {

		double x = (w) * ((double) i + 0.5) - total * w / 2;

		painter->translate(x, -h + height / 2);
		painter->rotate(-90);

		QRect r = QRect(-h / 2, -w / 2, h, w);
		painter->drawRoundedRect(r, w / 8, h / 8);
		painter->drawText(r, Qt::AlignCenter, rebuildStr(key + " " + QString::number(nd->getInSkills()[key]), 5));

		painter->rotate(90);
		painter->translate(-x, h - height / 2);

		i++;
	}

	// Out
	total = nd->getOutSkills().size();
	w = qMin(width / (double) total, width / 8);
	h = NODE_RAD * 5 / 6;
	f = painter->font();
	f.setPointSizeF(w * 0.6);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);
	i = 0;
	for (QString key : nd->getOutSkills().keys()) {

		double x = (w) * ((double) i + 0.5) - total * w / 2;

		painter->translate(x, h - height / 2);
		painter->rotate(-90);

		QRect r = QRect(-h / 2, -w / 2, h, w);
		painter->drawRoundedRect(r, w / 8, h / 8);
		painter->drawText(r, Qt::AlignCenter, rebuildStr(key + " " + QString::number(nd->getOutSkills()[key]), 5));

		painter->rotate(90);
		painter->translate(-x, -h + height / 2);

		i++;
	}
}

bool NodeDesignFormal::verticalSkillsLayout() {
	return true;
}

NodeDesignFormal::NodeDesignFormal(QObject *parent) :
		NodeDesign(parent) {
}

bool NodeDesignFormal::edgeToCenter() {
	return true;
}
