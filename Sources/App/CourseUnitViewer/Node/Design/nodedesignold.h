/*
 * nodedesignold.h
 *
 *  Created on: Sep 11, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGNOLD_H_
#define APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGNOLD_H_

#include "nodedesign.h"

class NodeDesignOld: public NodeDesign {
public:
	NodeDesignOld(QObject * parent);
	~NodeDesignOld();
	void draw(Node *nd, QPainter *painter,
			const QStyleOptionGraphicsItem *option, CourseUnitViewer *widget) override;
	bool verticalSkillsLayout() override;
	double getEdgeThickness() override;
	QPainterPath getShape() override;
};

#endif /* APP_COURSEUNITVIEWER_NODE_DESIGN_NODEDESIGNOLD_H_ */
