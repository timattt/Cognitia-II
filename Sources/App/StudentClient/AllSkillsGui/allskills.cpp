#include "allskills.h"
#include "ui_allskills.h"
#include "../../Structures/SkillPack/skillpack.h"
#include "skillwidget.h"

AllSkills::AllSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllSkills),
	skillPack(nullptr)
{
    ui->setupUi(this);
}

AllSkills::~AllSkills()
{
    delete ui;
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
		this->ui->verticalLayout->addWidget(w);
	}
}
