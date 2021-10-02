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

// Global functions
//==================================================
/**
 * This function may be use to make your string shorter and fits given length.
 * It is abbreviation generator.
 * Examples: max = 4
 * 1. Graph course -> GrCo
 * 2. Something -> Some
 * 3. HELP ME -> HeMe
 * @param str - string to rebuild
 * @param max - maximum symbols in new line
 * @returns rebuilt string with given length.
 * @author timattt
 */
QString rebuildStr(QString str, int max = MAX_SYMBOLS_PER_LINE);
//==================================================

/**
 * This class is used to create multiple designs for nodes.
 * Inherit your design from this class.
 * To add design to courseUnitViewer you may in its constructor add your class instance to qmap.
 * @author timattt
 */
class NodeDesign : public QObject {
	Q_OBJECT

public:

	// constructor and desctructor
	//==================================================
	/**
	 * Classical qobject constructor.
	 * @author timattt
	 */
	NodeDesign(QObject * parent);
	~NodeDesign() = default;
	//==================================================

public:

	// public functions
	//==================================================
	/**
	 * This function is called when any node is drawing.
	 * @param nd - node to draw
	 * @param painter - use this to draw
	 * @author timattt
	 */
	virtual void draw(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	/**
	 * Checks skills orientation for selection.
	 * Vertical orientation is when in skills are on the left and out are on the right part of node.
	 * Horizontal is when in skills are up and out skills are down.
	 * @returns true if vertical orientation.
	 * @author timattt
	 */
	virtual bool verticalSkillsLayout();
	/**
	 * Gives edge thickness in pixels for this design.
	 * @returns thickness in pixels
	 * @author timattt
	 */
	virtual double getEdgeThickness();
	//==================================================

};

#endif /* APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGN_H_ */
