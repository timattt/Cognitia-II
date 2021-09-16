#include "skillsflower.h"
#include "ui_skillsflower.h"
#include "flowerscene.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "leaf.h"

SkillsFlower::SkillsFlower(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillsFlower)
{
    ui->setupUi(this);

    ui->view->setScene(scene = new FlowerScene(this));

    QFile test = QFile("C:/Users/timat/Desktop/dedCourse/Akinator.CourseUnit");
    CourseUnit cu;
    cu.loadCourseUnit(&test);
    StudentProgress prg;
    unpack(&cu, &prg);
}

SkillsFlower::~SkillsFlower()
{
    delete ui;
}

void SkillsFlower::unpack(CourseUnit *cu, StudentProgress * prg) {
	clearAll();

	QMap<QString, double> min;
	QMap<QString, double> max;
	QMap<QString, double> value;

	for (int i = 0; i < cu->getIncome().size(); i++) {
		QString name = cu->getIncome()[i].first;
		double val = cu->getIncome()[i].second;

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
	for (int i = 0; i < cu->getOutcome().size(); i++) {
		QString name = cu->getOutcome()[i].first;
		double val = cu->getOutcome()[i].second;

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
		if (prg->containsLevel(cu->objectName(), name)) {
			value[name] = prg->getLevel(cu->objectName(), name);
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
		i++;
	}

	scene->update();
}

void SkillsFlower::pack(CourseUnit *cu, StudentProgress * prg) {
	QString courseUnitName = cu->objectName();
	for (QGraphicsItem * it : scene->items()) {
		Leaf * lf = (Leaf*)dynamic_cast<Leaf*>(it);

		if (lf == nullptr) {
			continue;
		}

		QString skill = lf->getText();
		double val = lf->getValue();

		prg->addProgress(courseUnitName, skill, val);
	}
}

void SkillsFlower::clearAll() {
	scene->clear();
}
