#include "mentorclient.h"
#include "ui_mentorclient.h"
#include "../CourseUnitViewer/courseunitviewer.h"
#include "../SkillsFlower/skillsflower.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/StudentProgress/StudentProgress.h"

MentorClient::MentorClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MentorClient)
{
	qInfo() << "Init mentor client";

    ui->setupUi(this);

    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->skillFlower, SLOT(unpack(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), ui->skillsMixerHolder, SLOT(nodeSelected(Node*)));
    connect(ui->courseUnitViewer, SIGNAL(nodeSelected(Node*)), this, SLOT(nodeSelected(Node*)));

    connect(ui->courseUnitViewer, SIGNAL(progressMade(QString, double)), ui->skillFlower, SLOT(progressMade(QString, double)));
    connect(ui->courseUnitViewer, SIGNAL(progressMade(QString, double)), ui->skillsMixerHolder, SLOT(progressMade(QString, double)));

    connect(ui->skillFlower, SIGNAL(skillLevelChanged(QString, double)), ui->courseUnitViewer, SLOT(makeProgressToSelected(QString, double)));

    ui->skillFlower->setEditable(true);
    ui->courseUnitViewer->setEditable(false);

    // TEST
    //------------------------------
    headCourseUnit = new CourseUnit;

    QFile f = QFile("C:/Users/timat/Desktop/dedCourse/sem1.CourseUnit");
    headCourseUnit->loadCourseUnit(&f);

    StudentProgress * tim = new StudentProgress();
    StudentProgress * andrew = new StudentProgress();

    students["tim"] = tim;
    students["andrew"] = andrew;

    tim->addProgress("Akinator", "Graph", 0.5);
    andrew->addProgress("Onegin", "IO", 1.5);

    display();
    //------------------------------

    qInfo() << "Mentor client init finished";
}

MentorClient::~MentorClient()
{
    delete ui;
}

void MentorClient::display() {
	ui->courseUnitViewer->clearAllScene();
	ui->courseUnitViewer->unpack(headCourseUnit);

	ui->studentChooser->clear();
	ui->studentChooser->addItems(students.keys());

	ui->childDescr->clear();
	ui->parentDescr->clear();

	ui->parentDescr->setMarkdown(headCourseUnit->getDescription());

	ui->childCu->clear();
	ui->parentCu->clear();

	ui->parentCu->setText(headCourseUnit->objectName());
}

void MentorClient::on_studentChooser_currentTextChanged(const QString &name) {
	if (students.contains(currentStudent)) {
		ui->courseUnitViewer->pack(students[currentStudent]);
	}
	ui->courseUnitViewer->clearStudentProgress();

	ui->courseUnitViewer->unpack(students[name]);

	currentStudent = name;

	ui->courseUnitViewer->setSelectedNode(ui->courseUnitViewer->getSelectedNode());
}

void MentorClient::pack() {
	if (students.contains(ui->studentChooser->currentText())) {
		ui->courseUnitViewer->pack(students[ui->studentChooser->currentText()]);
	}
}

void MentorClient::nodeSelected(Node *nd) {
	if (nd == nullptr) {
		ui->childDescr->clear();
		ui->childCu->clear();
	} else {
		ui->childDescr->setMarkdown(nd->getDescription());
		ui->childCu->setText(nd->getName());
	}
}
