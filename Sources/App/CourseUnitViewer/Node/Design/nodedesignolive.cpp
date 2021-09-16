#include "nodedesignolive.h"
#include "../node.h"
#include <QtWidgets>

#define PI 3.14159265359

void NodeDesignOlive::drawSkills(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget) {
	double rad = NODE_RAD / 4;

	QFont f = painter->font();
	f.setPointSizeF(rad / 2);
	f.setBold(true);
	f.setWeight(QFont::ExtraBold);
	painter->setFont(f);

	painter->setPen(QPen(Qt::black, NODE_RAD / 25.0));

    double anglePerSkill = 30;
    double startAngle = 180 -nd->getInSkills().size() * anglePerSkill / 2;
    double endAngle = 180 + nd->getInSkills().size() * anglePerSkill / 2;

    if (endAngle > 270) {
        startAngle = 90;
        endAngle = 270;
        anglePerSkill = (endAngle - startAngle) / (double) nd->getInSkills().size();
    }

	int i = -1;
	for (QString key : nd->getInSkills().keys()) {
		i++;
		double alpha = endAngle - anglePerSkill * ((double)(i) + 0.5);

		double x = cos(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;
		double y = sin(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;

        painter->setBrush(Qt::black);
        painter->drawLine(0, 0, x, y);

        painter->translate(x, y);
        painter->rotate(alpha - 180);

        QRect r = QRect(0 - rad, 0 - rad / 2, 2.0 * rad, rad);

		QRadialGradient gradient(0, 0, rad);
        gradient.setColorAt(1, QColor(Qt::darkRed));
        gradient.setColorAt(0, QColor(Qt::red));
        painter->setBrush(gradient);
        painter->drawRect(0 - rad, - rad, 2.0 * rad, 2.0 * rad);

        painter->drawText(QRectF(0 - rad, 0 - rad, 2.0 * rad, rad), Qt::AlignCenter, rebuildStr(key));
        painter->drawText(QRectF(0 - rad, 0, 2.0 * rad, rad), Qt::AlignLeft, rebuildStr(QString::number(nd->getInSkills()[key])));

        painter->rotate(-alpha +180);
        painter->translate(-x, -y);
	}

	anglePerSkill = 30;
    startAngle = -nd->getOutSkills().size() * anglePerSkill / 2;
    endAngle = nd->getOutSkills().size() * anglePerSkill / 2;

    if (endAngle > 90) {
        startAngle = -90;
        endAngle = 90;
        anglePerSkill = (endAngle - startAngle) / (double) nd->getOutSkills().size();
    }

    i = -1;
	for (QString key : nd->getOutSkills().keys()) {
		i++;
		double alpha = startAngle + anglePerSkill * ((double)(i) + 0.5);

		double x = cos(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;
		double y = sin(alpha / 180.0 * PI) * NODE_RAD / 5.0 * 4.0;

        painter->setBrush(Qt::black);
        painter->drawLine(0, 0, x, y);

        painter->translate(x, y);
        painter->rotate( alpha);

        QRect r = QRect(0 - rad, 0 - rad / 2, 2.0 * rad, rad);

		QRadialGradient gradient(0, 0, rad);
        gradient.setColorAt(1, QColor(Qt::darkGreen));
        gradient.setColorAt(0, QColor(Qt::green));
        painter->setBrush(gradient);
        painter->drawRect(0 - rad, - rad, 2.0 * rad, 2.0 * rad);

        painter->drawText(QRectF(0 - rad, 0 - rad, 2.0 * rad, rad), Qt::AlignCenter, rebuildStr(key));
        painter->drawText(QRectF(0 - rad, 0, 2.0 * rad, rad), Qt::AlignRight, rebuildStr(QString::number(nd->getOutSkills()[key])));

        painter->rotate( -alpha);
        painter->translate(-x, -y);
	}

	// center
	QRadialGradient gr(0, 0, rad * 1.5);
	if (nd->isSelected()) {
		gr.setColorAt(1, SELECT_COLOR);
		gr.setColorAt(0, SELECT_COLOR);
	} else {
		gr.setColorAt(1, QColor(nd->getColor()).darker(200));
		gr.setColorAt(0, QColor(nd->getColor()));
	}
	painter->setBrush(gr);
	painter->drawEllipse(QPointF(0, 0), rad * 1.5, rad * 1.5);

	// Name bACKGROUND
	startAngle = -180;
	endAngle = 0;
	double anglePerSymbol = (endAngle - startAngle) / RECTS_PER_NAME;

	double dist = 1.5 * rad;

	QRadialGradient gradient(0, 0, (dist + rad));
    gradient.setColorAt(1, QColor(Qt::darkMagenta));
    gradient.setColorAt(0, QColor(Qt::magenta));
    painter->setBrush(gradient);
    QPainterPath p = QPainterPath();

	for (int i = 0; i < RECTS_PER_NAME + 1; i++) {
		double alpha = (startAngle + (i) * anglePerSymbol);
		double x = cos(alpha / 180.0 * PI) * (dist + rad);
		double y = sin(alpha / 180.0 * PI) * (dist + rad);

		if (i == 0) {
			p.moveTo(x, y);
		} else {
			p.lineTo(x, y);
		}
	}

	for (int i = RECTS_PER_NAME; i != -1; i--) {
		double alpha = (startAngle + (i) * anglePerSymbol);
		double x = cos(alpha / 180.0 * PI) * dist * 0.9;
		double y = sin(alpha / 180.0 * PI) * dist * 0.9;

		p.lineTo(x, y);
	}

	// Name
	p.closeSubpath();
	painter->drawPath(p);
	startAngle = -170;
	endAngle = -10;
	anglePerSymbol = (endAngle - startAngle) / nd->getName().length();

	painter->setPen(QPen(Qt::black, NODE_RAD / 25.0));
	for (int i = 0; i < nd->getName().length(); i++) {
		QChar c = nd->getName()[i];

		double alpha = (startAngle + (i + 0.5) * anglePerSymbol);

		double x = cos(alpha / 180.0 * PI) * dist ;
		double y = sin(alpha / 180.0 * PI) * dist ;


		painter->translate(x, y);
		painter->rotate(alpha - 180);
		painter->rotate(-90);

		painter->drawText(QRect(-rad / 2, -rad, rad, rad), Qt::AlignCenter, QString(c));

		painter->rotate(90);
		painter->rotate(-alpha + 180);
		painter->translate(-x, -y);

	}

}

void NodeDesignOlive::draw(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

	// Shadow
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor(50, 50, 50, 128)));
    painter->drawEllipse(-NODE_RAD + NODE_SHADOW_SHIFT, -NODE_RAD + NODE_SHADOW_SHIFT, NODE_RAD * 2, NODE_RAD * 2);

    QRadialGradient gradient(-NODE_SHADOW_SHIFT, -NODE_SHADOW_SHIFT, NODE_RAD);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).lighter(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, NODE_RAD / 25.0));

    painter->drawEllipse(-NODE_RAD, -NODE_RAD, 2 * NODE_RAD, 2 * NODE_RAD);

    gradient.setCenter(0, 0);
    gradient.setFocalPoint(0, 0);

    drawSkills(nd, painter, option, widget);

}

bool NodeDesignOlive::verticalSkillsLayout() {
	return false;
}

bool NodeDesignOlive::edgeToCenter() {
	return false;
}

NodeDesignOlive::NodeDesignOlive(QObject *parent) : NodeDesign(parent) {
}
