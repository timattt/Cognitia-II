/*
 * nodedesignshape.h
 *
 *  Created on: Nov 7, 2021
 *      Author: ZoomZero
 */

#pragma once

#include "nodedesign.h"

/**
 * This class is used to create shape design for nodes.
 * @author ZoomZero
 */
class NodeDesignShape : public NodeDesign {

public:
    // Public functions
    //==================================================
    /**
     * This function is called when any shape node is drawing.
     * @param nd - node to draw
     * @param painter - use this to draw
     * @author ZoomZero
     */
    void draw(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /**
     * Checks skills orientation for selection.
     * Vertical orientation is when in skills are on the left and out are on the right part of node.
     * Horizontal is when in skills are up and out skills are down.
     * @returns true if vertical orientation.
     * @author ZoomZero
     */
    bool verticalSkillsLayout() override;
    //==================================================

    // Constructor and destructor
    //==================================================
    NodeDesignShape(QObject * parent);
    ~NodeDesignShape() = default;
    //==================================================
};
