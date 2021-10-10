/*
 * leaf.h
 *
 *  Created on: Sep 16, 2021
 *      Author: timat
 */

#ifndef APP_SKILLSFLOWER_LEAF_H_
#define APP_SKILLSFLOWER_LEAF_H_

#include <QtWidgets>

// Constants
//====================================
// ellipse
#define SMALL_LENGTH 60.0
//
#define MIN_LEN 180.0
#define MAX_LEN 260.0
#define EXTRA_LEN MAX_LEN

#define HANDLER_RAD MIN_LEN / 5
#define CENTER_RAD MAX_LEN / 8
//====================================

// Used classes
//====================================
class SkillsFlower;
//====================================

/**
 * This class represents skill in flower.
 * So it flower represents node then leaf is single node skill.
 * Leaf shows how much progress i made in given skill.
 * Field to and from represents level bounds in which value field (which represents current user progress) may be.
 * This class may be used only by skill flower.
 *
 * @author timattt
 */
class Leaf: public QGraphicsItem {

public:

	// constructor and destructor
	//=====================================
	/**
	 * Simple constructor that inits all class fields.
	 * @author timattt
	 */
	Leaf(double from, double to, double val, QString text, double angle, SkillsFlower * graph);
	~Leaf() = default;
	//=====================================

private:

	// fields
	//=====================================
	/**
	 * Checks if user pressed this leaf but still not released.
	 * Field is used in drag action.
	 * @author timatt
	 */
	bool pressed = 0;
	/**
	 * When user presses mouse button on this leaf this field inits with mouse cursor scene coords.
	 * @author timatt
	 */
	QPointF startPoint;
	/**
	 * Starting skill level value.
	 * So value lies between from and to.
	 * @author timatt
	 */
	double from;
	/**
	 * End skill level value.
	 * So value lies between from and to.
	 * @author timatt
	 */
	double to;
	/**
	 * Current skill level value.
	 * @author timatt
	 */
	double value;
	/**
	 * Text that will be draw on this leaf.
	 * It may be skill name.
	 * @author timatt
	 */
	QString text;
	/**
	 * This leaf rotation angle in flower.
	 * @author timatt
	 */
	double angle;
	/**
	 * Flower object.
	 * @author timatt
	 */
	SkillsFlower * parent;
	//=====================================

private:

	// private function
	//=====================================
	/**
	 * Gives length of this leaf in scene coordinates.
	 * Calculates it using value field.
	 * @returns length from flower center to this leaf border
	 * @author timatt
	 */
	double getLen() const;
	/**
	 * Sets new length to this leaf in scene coordinates.
	 * It translates it into new value field value. And then sets value field.
	 * @param len - length from flower center to this leaf border.
	 * @author timatt
	 */
	void setLen(double len);
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
	 * This function moves this leaf to correct place.
	 * Call this always when value or length is changed.
	 * @author timatt
	 */
	void refreshPos();
	/**
	 * Gives current leaf text.
	 * @returns QString, leaf or skill name.
	 * @author timatt
	 */
	QString getText();
	/**
	 * Gives current skill progress value.
	 * @returns value between from field and to field.
	 * @author timatt
	 */
	double getValue();
	/**
	 * Sets new progress value.
	 * Calls refrsh function.
	 * @param double may be from from field to to field.
	 * @author timatt
	 */
	void setValue(double v);
	//=====================================

protected:

	// protected functions
	//=====================================
	/**
	 * Reimplemented from QGraphicsItem.
	 */
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	/**
	 * Reimplemented from QGraphicsItem.
	 */
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	/**
	 * Reimplemented from QGraphicsItem.
	 */
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	//=====================================

};

#endif /* APP_SKILLSFLOWER_LEAF_H_ */
