/*
 * circle.h
 *
 *  Created on: Nov 26, 2021
 *      Author: ZoomZero
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include <QtWidgets>

// Used classes
//====================================
class InputAbsoluteSkillsPanel;
//====================================

/**
 * This class represents circle in inabsoluteskill.
 * Circle represents certain in skill for this node.
 * Ring shows how much progress i made in given skill.
 * Field to and from represents level bounds in which value field (which represents current user progress) may be.
 * This class may be used only by inabsoluteskill.
 *
 * @author ZoomZero
 */
class Circle: public QGraphicsItem {

public:

    // constructor and destructor
    //=====================================
    /**
     * Simple constructor that inits all class fields.
     * @author ZoomZero
     */
    Circle(double level, double levelTo, QString text, int skillsCount, int elem);
    ~Circle() = default;
    //=====================================

private:

    // fields
    //=====================================
    /**
     * Level of student skill.
     * @author ZoomZero
     */
    double level;
    /**
     * Required to start module level of skill.
     * @author ZoomZero
     */
    double levelTo;
    /**
     * Text that will be draw on this circle.
     * It is skill name and level.
     * @author ZoomZero
     */
    QString text;
    /**
     * Number of overall in skills
     * @author ZoomZero
     */
    int skillsCount;
    /**
     * Number of current skill overall
     * @author ZoomZero
     */
    int elem;
    /**
     * Parent object.
     * @author ZoomZero
     */
    InputAbsoluteSkillsPanel * parent;
    //=====================================

private:

    // private function
    //=====================================

    //=====================================

public:

    // public functions
    //=====================================
    /**
     * Reimplemented from QGraphicsItem.
     */
    QRectF boundingRect() const override;
    /**
     * Reimplemented from QGraphicsItem.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * Sets new progress level.
     * Calls refrsh function.
     * @param double may be from from field to to field.
     * @author ZoomZero
     */
    void setLevel(double lev);
    //=====================================

protected:

    // protected functions
    //=====================================

    //=====================================

};

#endif // CIRCLE_H
