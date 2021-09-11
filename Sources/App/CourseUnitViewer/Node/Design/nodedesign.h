/*
 * nodedesign.h
 *
 *  Created on: Sep 11, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGN_H_
#define APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGN_H_

#include <QtWidgets>

class Node;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

// Constants
//==================================================
#define NODE_RAD 160
#define NODE_SHADOW_SHIFT (NODE_RAD / 20 * 3)
#define MAX_SYMBOLS_PER_LINE 4
#define RECTS_PER_NAME 5
//==================================================

class NodeDesign : public QObject {
	Q_OBJECT
public:

	NodeDesign(QObject * parent);
	virtual ~NodeDesign();

	virtual void draw(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	virtual bool verticalSkillsLayout();
	virtual bool edgeToCenter();
	virtual double getEdgeThickness();

	QString rebuildStr(QString str, int max = MAX_SYMBOLS_PER_LINE);
};

#endif /* APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGN_H_ */
