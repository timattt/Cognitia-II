#include "courseunitinfo.h"
#include "ui_courseunitinfo.h"
#include "../studentclient.h"
#include "../../CourseUnitViewer/Node/node.h"
#include "../../Core/logger.h"
#include "../../CourseUnitViewer/courseunitviewer.h"
#include "outdeltaskills.h"
#include "../../Structures/StudentProgress/StudentProgress.h"

CourseUnitInfo::CourseUnitInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitInfo),
	client(nullptr)
{
    ui->setupUi(this);

    client = dynamic_cast<StudentClient*>(parent->parent());

    NOT_NULL(client);

    connect(client, SIGNAL(newStudentProgress(StudentProgress*)), ui->chat, SLOT(setStudentProgress(StudentProgress*)));
	connect(client->getCourseUnitViewer(), SIGNAL(nodeSelected(Node*)), this->ui->outDeltaSkills, SLOT(unpack(Node*)));
	connect(client->getCourseUnitViewer(), SIGNAL(nodeSelected(Node*)), this->ui->chat, SLOT(nodeSelected(Node*)));
	connect(client->getCourseUnitViewer(), SIGNAL(progressMadeToSelected(QString, double)), this->ui->outDeltaSkills, SLOT(progressMade(QString, double)));
    connect(client, SIGNAL(clearAll()), ui->chat, SLOT(clearAll()));
    connect(client, SIGNAL(newStudentName(QString)), ui->chat, SLOT(studentNameChanged(QString)));
    connect(client, SIGNAL(clearAll()), ui->outDeltaSkills, SLOT(clearAll()));
}

CourseUnitInfo::~CourseUnitInfo()
{
    delete ui;
}

void CourseUnitInfo::prepareNode(Node *nd) {
	ui->cuName->setText(nd->getName());
	ui->cuDescr->setMarkdown(nd->getDescription());
}

void CourseUnitInfo::clearAll() {
	ui->cuDescr->clear();
	ui->cuName->clear();
}

void CourseUnitInfo::on_returnBack_clicked() {
	client->setCUInfoVisible(false);
}

