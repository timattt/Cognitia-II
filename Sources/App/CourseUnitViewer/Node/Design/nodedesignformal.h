#pragma once

#include "nodedesign.h"

class NodeDesignFormal : public NodeDesign {

public:
	void draw(Node *nd, QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	bool verticalSkillsLayout() override;
	bool edgeToCenter() override;

	NodeDesignFormal(QObject * parent);
	~NodeDesignFormal() = default;
};
