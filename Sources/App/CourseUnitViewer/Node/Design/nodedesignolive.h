#pragma once

#include "nodedesign.h"

class NodeDesignOlive : public NodeDesign {

public:
	void draw(Node *nd, QPainter *painter,
			const QStyleOptionGraphicsItem *option, CourseUnitViewer *widget) override;
	bool verticalSkillsLayout() override;

	void drawSkills(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option,
			CourseUnitViewer *widget);

	NodeDesignOlive(QObject * parent);
	~NodeDesignOlive() = default;

	QPainterPath getShape() override;
};
