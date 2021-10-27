#include "allskills.h"
#include "../../Structures/SkillPack/skillpack.h"
#include "skillwidget.h"

AllSkills::AllSkills(QWidget *parent) :
	QWidget(parent),
	skillPack(nullptr),
	layout(nullptr)
{
	layout = new QVBoxLayout(this);
}

void AllSkills::clearAll() {
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

void AllSkills::setSkp(SkillPack *skp) {
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
