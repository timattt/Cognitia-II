/*
 * circle.h
 *
 *  Created on: Nov, 19, 2021
 *      Author: ZoomZero
 */

#ifndef OUTCIRCLE_H
#define OUTCIRCLE_H

#include <QtWidgets>

// Used classes
//====================================
class OutputRelativeSkillsPanel;
//====================================

/**
 * This class represents circle in outputrelativeskill.
 * Circle represents reltive progress in out skill.
 * Ring shows how much progress i made in given skill.
 * Field to and from represents level bounds in which value field (which represents current user progress) may be.
 * This class may be used only by outputrelativeskill.
 *
 * @author ZoomZero
 */
class OutCircle: public QGraphicsItem {

public:

    // constructor and destructor
    //=====================================
    /**
     * Simple constructor that inits all class fields.
     * @author ZoomZero
     */
    OutCircle(double level, double levelTo, QString text, int skillsCount, int elem);
    ~OutCircle() = default;
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
    OutputRelativeSkillsPanel * parent;
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

#endif // OUTCIRCLE_H
