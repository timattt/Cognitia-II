#include "ui_outputrelativeskillspanel.h"
#include "outputrelativeskillspanel.h"
#include "../../Core/logger.h"
#include "outcirclescene.h"
#include "outcircle.h"
#include "../../CourseUnitViewer/Node/node.h"
#include "../../Structures/CourseUnit/courseunit.h"
#include "../../Structures/StudentProgress/StudentProgress.h"
#include <QtWidgets>

OutputRelativeSkillsPanel::OutputRelativeSkillsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputRelativeSkillsPanel),
    currentProgress(nullptr),
    scene(nullptr)
{
    NOT_NULL(parent);

    SAY("OutputRelativeSkill init started");

    ui->setupUi(this);

    ui->view->setScene(scene = new OutCircleScene(this));

    scene->update();

    SAY("OutputRelativeSkill init finished");
}

OutputRelativeSkillsPanel::~OutputRelativeSkillsPanel()
{
    delete ui;
}

void OutputRelativeSkillsPanel::clearAll() {
    scene->clear();
    scene->update();
    circles.clear();
}

void OutputRelativeSkillsPanel::progressMade(QString skill, double lev) {
    if(circles.contains(skill))
        circles[skill]->setLevel(lev);
}

void OutputRelativeSkillsPanel::nodeSelected(Node *nd) {

    if (nd == nullptr) {
        return;
    }

    clearAll();

    QMap<QString, int> outSkills = nd->getOutSkills();

    QMap<QString, int>::Iterator i;
    int size = outSkills.size();

    int elem = 0;
    for(i = outSkills.begin(); i != outSkills.end(); i++, elem++) {
        OutCircle * circle = nullptr;

        scene->addItem(circle = new OutCircle(nd->getProgressScalar(i.key()), 1, i.key(), size, elem));

        circles[i.key()] = circle;
    }

    scene->update();
    ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void OutputRelativeSkillsPanel::resizeEvent(QResizeEvent * event) {
    Q_UNUSED(event);
    int h = height() + 20;
    int w = width();

    scene->setSceneRect(- w / 2, - h / 2, w, h);
    ui->view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
