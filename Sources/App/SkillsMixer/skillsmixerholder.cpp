#include "skillsmixerholder.h"
#include "ui_skillsmixerholder.h"
#include "../CourseUnitViewer/Node/node.h"
#include "skillsmixer.h"
#include <QtWidgets>
#include "../Core/logger.h"

SkillsMixerHolder::SkillsMixerHolder(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::SkillsMixerHolder) {
	NOT_NULL(parent);

	SAY("SkillMixerHolder init started");
	ui->setupUi(this);
	SAY("SkillMixerHolder init finished");
}

SkillsMixerHolder::~SkillsMixerHolder() {
	delete ui;
}

void SkillsMixerHolder::nodeSelected(Node *nd) {
	QList<SkillsMixer*> toDelete;

	for (QObject *o : this->children()) {
		SkillsMixer *mx = dynamic_cast<SkillsMixer*>(o);
		if (mx != nullptr) {
			toDelete.push_back(mx);
		}
	}
	while (!toDelete.empty()) {
		delete toDelete.first();
		toDelete.pop_front();
	}

	mixers.clear();

	if (nd != nullptr) {
		QMap<QString, double> min;
		QMap<QString, double> max;
		QMap<QString, double> value;

		for (QString name : nd->getInSkills().keys()) {
			double val = nd->getInSkills()[name];

			if (!nd->getOutSkills().contains(name)) {
				continue;
			}

			if (!min.contains(name)) {
				min[name] = val;
			} else {
				min[name] = qMin(min[name], val);
			}
			if (!max.contains(name)) {
				max[name] = val + 1;
			} else {
				max[name] = qMax(max[name], val + 1);
			}
		}

		for (QString name : nd->getOutSkills().keys()) {
			double val = nd->getOutSkills()[name];

			if (!min.contains(name)) {
				min[name] = val - 1;
			} else {
				min[name] = qMin(min[name], val - 1);
			}
			if (!max.contains(name)) {
				max[name] = val;
			} else {
				max[name] = qMax(max[name], val);
			}
		}
		for (QString name : min.keys()) {
			if (nd->containsProgress(name)) {
				value[name] = nd->getSkillProgress(name);
			} else {
				value[name] = min[name];
			}
		}

		int i = 0;
		for (QString name : min.keys()) {
			double from = min[name];
			double to = max[name];
			double val = value[name];
			SkillsMixer * mx = new SkillsMixer(this, from, to ,val, name);
			ui->verticalLayout_2->addWidget(mx);
			mixers[name] = mx;
			i++;
		}

		this->updateGeometry();
		this->update();
	}
}

void SkillsMixerHolder::progressMade(QString name, double v) {
	if (mixers.contains(name)) {
		mixers[name]->setValue(v);
	}
}
