/*
 * leaf.h
 *
 *  Created on: Sep 16, 2021
 *      Author: timat
 */

#ifndef APP_SKILLSFLOWER_LEAF_H_
#define APP_SKILLSFLOWER_LEAF_H_

#include <QtWidgets>

// ellipse
#define SMALL_LENGTH 60.0

//
#define MIN_LEN 180.0
#define MAX_LEN 260.0
#define EXTRA_LEN MAX_LEN

#define HANDLER_RAD MIN_LEN / 5
#define CENTER_RAD MAX_LEN / 8

class SkillsFlower;

class Leaf: public QGraphicsItem {

public:

	// constructor and destructor
	//=====================================
	Leaf(double from, double to, double val, QString text, double angle, SkillsFlower * graph);
	~Leaf();
	//=====================================

private:

	// fields
	//=====================================
	bool pressed = 0;
	QPointF startPoint;
	double from;
	double to;
	double value;
	QString text;
	double angle;
	SkillsFlower * parent;
	//=====================================

private:

	// private function
	//=====================================
	double getLen() const;
	void setLen(double len);
	//=====================================

public:

	// public functions
	//=====================================
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void refreshPos();
	QString getText();
	double getValue();
	void setValue(double v);
	//=====================================

protected:

	// protected functions
	//=====================================
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	//=====================================

};

#endif /* APP_SKILLSFLOWER_LEAF_H_ */
