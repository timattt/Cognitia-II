/*
 * nodedesignshape.cpp
 *
 *  Created on: Nov 7, 2021
 *      Author: ZoomZero
 */

#include "nodedesignshape.h"
#include "../node.h"
#include <QtWidgets>

void NodeDesignShape::draw(Node *nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
    if (nd->isSelected()) {
        painter->setBrush(SELECTED_NODE_COLOR);
    } else {
        painter->setBrush(nd->getColor());
    }
    //========================================

    // main rectangle
    //========================================
    painter->setBrush(QColorConstants::Svg::moccasin);
    painter->setPen(QColorConstants::Svg::tomato);
    QRect descRect = QRect(-side / 2, -side / 8 * 3, side, side / 6 * 3.5);
    painter->drawRoundedRect(descRect, 25, 25, Qt::AbsoluteSize);
    //========================================

    //text
    //========================================
    f = painter->font();
    f.setPointSizeF(side / 20.0);
    f.setBold(true);
    f.setWeight(QFont::ExtraBold);
    painter->setFont(f);
    int maxChars = 14;
    QString nameFinal = rebuildStr(name, maxChars);
    descRect = QRect(- side / 2 * 0.9, - side / 8 * 2.8, side * 0.9, side / 8 * 3);

    painter->setPen(Qt::black);
    painter->drawText(descRect, Qt::AlignTop | Qt::AlignHCenter, nameFinal);

    painter->setPen(QColorConstants::Svg::tomato);
    painter->drawLine(- side / 2, - side / 8 * 2, side / 2, - side / 8 * 2);
    painter->drawLine(- side / 2, - side / 8 * 1.99, side / 2, - side / 8 * 1.99);
    //========================================

    // descriptrion
    //========================================
    f = painter->font();
    f.setPointSizeF(side / 20.0);
    f.setBold(false);
    f.setWeight(QFont::Normal);
    painter->setFont(f);
    painter->setPen(Qt::black);
    descRect = QRect(- side / 2 * 0.95, - side / 8 * 2, side * 0.9, side / 8 * 3);
    painter->drawText(descRect, Qt::AlignLeft | Qt::TextWordWrap, nd->getDescription());
    //========================================

    // Skills
    // In
    int total = nd->getInSkills().size() + 1;
    double h = side / 8;
    double w = side * 0.45;

    double step = qMin(h / 2, side / 2 / (double)total);

    int i = 0;
    for (QString key : nd->getInSkills().keys()) {

        double x = -side / 4;
        double y = ((double)i + 1) * step;

        painter->translate(x, y);

        QRect r = QRect(-w / 2, -h / 2, w, h / 2);

        if (i == 0) {
            if (nd->isSelected()) {
                painter->setBrush(SELECTED_NODE_COLOR);
            } else {
                painter->setBrush(QColorConstants::Svg::coral);
            }

            painter->setPen(QColorConstants::Svg::tomato);
            painter->drawRect(r);
            painter->setPen(Qt::black);
            painter->drawText(r, Qt::AlignCenter, "IN:");
            i++;
            painter->translate(-x, -y);

            y = ((double)i + 1) * step;
            painter->translate(x, y);
            painter->setBrush(QColorConstants::Svg::moccasin);
        }

        painter->setPen(QColorConstants::Svg::tomato);
        painter->drawRect(r);
        painter->setPen(Qt::black);
        painter->drawText(r, Qt::AlignCenter, rebuildStr(key + " " + QString::number(nd->getInSkills()[key]), 8));

        painter->translate(-x,- y);

        i++;
    }

    // Out
    total = nd->getOutSkills().size() + 1;
    h = side / 8;
    w = side *0.45;

    step = qMin(h / 2, side / 2 / (double)total);

    i = 0;
    for (QString key : nd->getOutSkills().keys()) {

        double x = side / 4;
        double y = ((double)i + 1) * step;

        painter->translate(x, y);

        QRect r = QRect(-w / 2, -h / 2, w, h / 2);

        if (i == 0) {
            if (nd->isSelected()) {
                painter->setBrush(SELECTED_NODE_COLOR);
            } else {
                painter->setBrush(QColorConstants::Svg::coral);
            }

            painter->setPen(QColorConstants::Svg::tomato);
            painter->drawRect(r);
            painter->setPen(Qt::black);
            painter->drawText(r, Qt::AlignCenter, "OUT:");
            i++;
            painter->translate(-x, -y);

            y = ((double)i + 1) * step;
            painter->translate(x, y);
            painter->setBrush(QColorConstants::Svg::moccasin);
        }

        painter->setPen(QColorConstants::Svg::tomato);
        painter->drawRect(r);
        painter->setPen(Qt::black);
        painter->drawText(r, Qt::AlignCenter, rebuildStr(key + " " + QString::number(nd->getOutSkills()[key]), 8));

        painter->translate(-x,- y );

        i++;
    }
}

bool NodeDesignShape::verticalSkillsLayout() {
    return true;
}

NodeDesignShape::NodeDesignShape(QObject * parent) :
    NodeDesign(parent) {

}
