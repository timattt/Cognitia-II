#include "skillsflower.h"
#include "ui_skillsflower.h"
#include "flowerscene.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "leaf.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Core/logger.h"
#include <QtWidgets>

SkillsFlower::SkillsFlower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillsFlower),
	scene(nullptr),
	editable(0),
	leafs()
{
	NOT_NULL(parent);

	SAY("SkillFlower init started");

    ui->setupUi(this);

    ui->view->setScene(scene = new FlowerScene(this));

    ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    SAY("SkillFlower init finished");
}

SkillsFlower::~SkillsFlower()
{
    delete ui;
}

/**
 * @Deprecated
 */
void SkillsFlower::unpackEmbed(CourseUnit *cu, StudentProgress * prg) {
	clearAll();

	if (!cu || !prg) {
		return;
	}

	QVector<CourseUnit*> units;

	units.push_back(cu);

	QMap<QString, double> skills;

	while (!units.isEmpty()) {
		CourseUnit * cur = units.first();
		units.pop_front();
		for (CourseUnit * c : cur->getEmbedded()) {
			units.push_back(c);
		}

		QString name = cur->objectName();

		prg->collectAbsolute(cur, skills);
	}

	double anglePerSkill = 360.0 / (double)( skills.keys().size());

	double max = 0;
	for (QString name : skills.keys()) {
		max = qMax(max, skills[name]);
	}

	int i = 0;
	for (QString name : skills.keys()) {
		double from = 0;
		double to = max;
		double val = skills[name];
		Leaf * le = nullptr;
		scene->addItem(le = new Leaf(from, to, val, name, i * anglePerSkill, this));
		le->refreshPos();
		leafs[name] = le;
		i++;
	}

	scene->update();
	ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void SkillsFlower::clearAll() {
	ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	scene->clear();
	scene->update();
	leafs.clear();
}

void SkillsFlower::pack(CourseUnit *cu, StudentProgress *prg) const {
	NOT_NULL(cu);
	NOT_NULL(prg);

	QString courseUnitName = cu->objectName();
	for (QGraphicsItem *it : scene->items()) {
		Leaf *lf = (Leaf*) dynamic_cast<Leaf*>(it);

		if (lf == nullptr) {
			continue;
		}

		QString skill = lf->getText();
		double val = lf->getValue();
		prg->addProgress(courseUnitName, skill, val);
	}
}

void SkillsFlower::unpack(Node *nd) {
	clearAll();

	if (!nd) {
		return;
	}

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

	double anglePerSkill = 360.0 / (double)( min.keys().size());

	int i = 0;
	for (QString name : min.keys()) {
		double from = min[name];
		double to = max[name];
		double val = value[name];
		Leaf * le = nullptr;
		scene->addItem(le = new Leaf(from, to, val, name, i * anglePerSkill, this));
		le->refreshPos();
		leafs[name] = le;
		i++;
	}

	scene->update();
	ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

bool SkillsFlower::isEditable() const {
	return editable;
}

void SkillsFlower::setEditable(bool v) {
	editable = v;
}

void SkillsFlower::progressMade(QString name, double v) {
	if (leafs.contains(name)) {
		leafs[name]->setValue(v);
	}
}

void SkillsFlower::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);
	ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
