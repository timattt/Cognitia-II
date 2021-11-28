#pragma once

#include "nodedesign.h"

class NodeDesignFormal : public NodeDesign {

public:
	void draw(Node *nd, QPainter *painter,
			const QStyleOptionGraphicsItem *option, CourseUnitViewer *widget) override;
	bool verticalSkillsLayout() override;

	NodeDesignFormal(QObject * parent);
	~NodeDesignFormal() = default;

	QPainterPath getShape() override;
};
