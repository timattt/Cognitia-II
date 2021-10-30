#include "courseunitdetailspanel.h"

#include "ui_courseunitdetailspanel.h"
#include "../studentclient.h"
#include "../../CourseUnitViewer/Node/node.h"
#include "../../Core/logger.h"
#include "../../CourseUnitViewer/courseunitviewer.h"
#include "../../Structures/StudentProgress/StudentProgress.h"

CourseUnitDetailsPanel::CourseUnitDetailsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitInfo)
{
    ui->setupUi(this);

    StudentClient * client = dynamic_cast<StudentClient*>(parent->parent());

    NOT_NULL(client);

    connect(client, SIGNAL(newStudentProgress(StudentProgress*)), ui->chat, SLOT(setStudentProgress(StudentProgress*)));
	connect(client->getCourseUnitViewer(), SIGNAL(nodeSelected(Node*)), this->ui->outDeltaSkills, SLOT(unpack(Node*)));
	connect(client->getCourseUnitViewer(), SIGNAL(nodeSelected(Node*)), this->ui->chat, SLOT(nodeSelected(Node*)));
	connect(client->getCourseUnitViewer(), SIGNAL(progressMadeToSelected(QString, double)), this->ui->outDeltaSkills, SLOT(progressMade(QString, double)));
    connect(client, SIGNAL(clearAll()), ui->chat, SLOT(clearAll()));
    connect(client, SIGNAL(newStudentName(QString)), ui->chat, SLOT(senderNameChanged(QString)));
    connect(client, SIGNAL(clearAll()), ui->outDeltaSkills, SLOT(clearAll()));
    connect(ui->returnBack, SIGNAL(clicked()), client, SLOT(hideInfoPanel()));

}

CourseUnitDetailsPanel::~CourseUnitDetailsPanel()
{
    delete ui;
}

void CourseUnitDetailsPanel::nodeSelected(Node *nd) {
	ui->cuName->setText(nd->getName());
	ui->cuDescr->setMarkdown(nd->getDescription());
}

void CourseUnitDetailsPanel::clearAll() {
	ui->cuDescr->clear();
	ui->cuName->clear();
}


