#include "studentskillspanel.h"

#include "../../Structures/SkillPack/skillpack.h"
#include "../StudentSkillsPanel/skillwidget.h"

StudentSkillsPanel::StudentSkillsPanel(QWidget *parent) :
	QWidget(parent),
	skillPack(nullptr),
	layout(nullptr)
{
	layout = new QVBoxLayout(this);
}

void StudentSkillsPanel::clearAll() {
	QList<SkillWidget*> toDelete;

	for (QObject *o : this->children()) {
		SkillWidget *mx = dynamic_cast<SkillWidget*>(o);
		if (mx != nullptr) {
			toDelete.push_back(mx);
		}
	}
	while (!toDelete.empty()) {
		delete toDelete.first();
		toDelete.pop_front();
	}
}

void StudentSkillsPanel::setSkp(SkillPack *skp) {
	for (int i = 0; i < skp->getSkillsCount(); i++) {
		Skill * sk = skp->getSkill(i);
		SkillWidget * w = new SkillWidget(this, sk);
		layout->addWidget(w);
	}
	for (int i = 0; i < skp->getSkillsCount(); i++) {
		Skill * sk = skp->getSkill(i);
		SkillWidget * w = new SkillWidget(this, sk);
		layout->addWidget(w);
	}
}
