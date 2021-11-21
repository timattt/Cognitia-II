/*
 * circle.cpp
 *
 *  Created on: Nov, 19, 2021
 *      Author: ZoomZero
 */

#include "outcircle.h"
#include <QtGlobal>
#include "outputrelativeskillspanel.h"
#include "../../CourseUnitViewer/Node/Design/nodedesign.h"
#include "../../Core/logger.h"

QRectF OutCircle::boundingRect() const {
    int w = scene()->width();
    int h = scene()->height();
    int y = h / 2;
    double radi = qMin(w / (1.2 * skillsCount), y / 1.2);

    QRectF r = QRectF(0,  0, radi, radi);

    return r;
}

void OutCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int w = scene()->width();
    int h = scene()->height();
    painter->translate(-w/2, -h/2);

    int x = w / 2;
    int y = h * (2*(elem + 1) - 1) / (2 * skillsCount);
    double radi = qMin(h / (1.4 * skillsCount), x / 1.4);

    painter->translate(x - radi / 2, y - radi / 2);

    QRectF r = QRectF(0,  0, radi, radi);

    double scalar = level / levelTo;
    double penSize = radi / 10;

    if (scalar <= (1.0 / 4))
        painter->setPen(QPen(QColorConstants::Svg::darkseagreen, penSize));
    else if (scalar <= (1.0 / 2))
        painter->setPen(QPen(QColorConstants::Svg::palegreen, penSize));
    else if (scalar <= (3.0 / 4))
        painter->setPen(QPen(QColorConstants::Svg::mediumspringgreen, penSize));
    else
        painter->setPen(QPen(QColorConstants::Svg::lime, penSize));

    painter->setRenderHint(QPainter::Antialiasing);
    QFont f = painter->font();
    f.setPointSizeF(radi / 8.0);
    f.setStyle(QFont::StyleNormal);
    f.setWeight(QFont::DemiBold);
    painter->setFont(f);
    painter->setBrush(Qt::white);

    int startAngle = 360 * 4;
    int spanAngle = - scalar * 360 * 16;

    painter->drawArc(r, startAngle, spanAngle);

    painter->setPen(QPen(QColorConstants::Svg::darkturquoise, 3));
    r = QRect(4, 4, radi-8, radi-8);
    painter->drawArc(r, startAngle, spanAngle);

    painter->setPen(QPen(Qt::black, 1));
    r = QRect(0, - radi * 0.12, radi, radi);
    painter->drawText(r, Qt::AlignCenter, rebuildStr(text, 10));

    painter->setPen(QPen(QColorConstants::Svg::orangered, 3));
    painter->drawLine(radi / 8, radi / 2, radi * 7 / 8, radi / 2);

    QString progress = QString::number(level);
    progress.append(" / ");
    progress.append(QString::number(levelTo));

    painter->setPen(QPen(Qt::black, 1));
    r = QRect(0, radi * 0.12, radi, radi);
    painter->drawText(r, Qt::AlignCenter, progress);

    painter->translate(-x + radi / 2 , - y + radi / 2);
}

void OutCircle::setLevel(double lev)
{
    level = lev;
    scene()->update();
}


OutCircle::OutCircle(double l, double lt, QString te, int sc, int e) :
            level(l),
            levelTo(lt),
            text(te),
            skillsCount(sc),
            elem(e),
			parent(nullptr) {
}
