#pragma once

#include "nodedesign.h"

class NodeDesignOlive : public NodeDesign {

public:
	void draw(Node *nd, QPainter *painter,
			const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	bool verticalSkillsLayout() override;
	bool edgeToCenter() override;

	void drawSkills(Node * nd, QPainter *painter, const QStyleOptionGraphicsItem *option,
			QWidget *widget);

	NodeDesignOlive(QObject * parent);
	~NodeDesignOlive() = default;


};
