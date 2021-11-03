/*
 * inputabsoluteskillspanel.cpp
 *
 *  Created on: Oct 26, 2021
 *      Author: ZoomZero
 */

#include "inputabsoluteskillspanel.h"
#include "ui_inputabsoluteskillspanel.h"
#include "../../Core/logger.h"
#include "../../CourseUnitViewer/Node/node.h"
#include "circlescene.h"
#include "../../Structures/CourseUnit/courseunit.h"
#include "../../Structures/StudentProgress/StudentProgress.h"
#include "circle.h"
#include <QtWidgets>

InputAbsoluteSkillsPanel::InputAbsoluteSkillsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputAbsoluteSkillsPanel),
	currentProgress(nullptr),
    scene(nullptr)
{
    NOT_NULL(parent);

    SAY("InAbsoluteSkill init started");

    ui->setupUi(this);

    ui->view->setScene(scene = new CircleScene(this));

    scene->update();

    //ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    SAY("InAbsoluteSkill init finished");
}

InputAbsoluteSkillsPanel::~InputAbsoluteSkillsPanel()
{
    delete ui;
}

void InputAbsoluteSkillsPanel::clearAll() {
    scene->clear();
    scene->update();
    circles.clear();
}

void InputAbsoluteSkillsPanel::progressMade(QString skill, double lev) {
    if(circles.contains(skill))
        circles[skill]->setLevel(lev);
}

void InputAbsoluteSkillsPanel::unpack(Node *nd) {
    if (nd == nullptr) {
        return;
    }

    NOT_NULL(currentProgress);

    clearAll();

    QMap<QString, int> inSkills = nd->getInSkills();

    QMap<QString, int>::Iterator i;
    int size = inSkills.size();

    int elem = 0;
    for(i = inSkills.begin(); i != inSkills.end(); i++, elem++) {
        Circle * circle = nullptr;

        if (currentProgress->hasProgressInSkill(i.key())) {
            scene->addItem(circle = new Circle(currentProgress->getAbsoluteProgressForSkill(i.key()), i.value(), i.key(), size, elem));
        } else {
            scene->addItem(circle = new Circle(0, i.value(), i.key(), size, elem));
        }

        circles[i.key()] = circle;
    }

    scene->update();
    ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void InputAbsoluteSkillsPanel::newStudent(StudentProgress *prg) {
	currentProgress = prg;

	if (!currentProgress) {
		clearAll();
	}
}

void InputAbsoluteSkillsPanel::resizeEvent(QResizeEvent * event) {
    Q_UNUSED(event);
    int h = height();
    int w = width() + 50;

    scene->setSceneRect(- w / 2, - h / 2, w, h);
    ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
