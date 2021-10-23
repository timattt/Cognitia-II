#include "courseunitinfo.h"
#include "ui_courseunitinfo.h"
#include "../studentclient.h"
#include "../../CourseUnitViewer/Node/node.h"

CourseUnitInfo::CourseUnitInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitInfo),
	client(nullptr)
{
    ui->setupUi(this);
}

CourseUnitInfo::~CourseUnitInfo()
{
    delete ui;
}

void CourseUnitInfo::setClient(StudentClient *cl) {
	this->client = cl;
	//connect(cl->getCourseUnitViewer(), SIGNAL(nodeSelected(Node*)), this->ui->outDeltaSkills, SLOT(unpack(Node*)));
    //connect(cl->getCourseUnitViewer(), SIGNAL(progressMadeToSelected(QString, double)), this->ui->outDeltaSkills, SLOT(progressMade(QString, double)));
}

void CourseUnitInfo::prepareNode(Node *nd) {
	ui->cuName->setText(nd->getName());
	ui->cuDescr->setMarkdown(nd->getDescription());
}

void CourseUnitInfo::on_returnBack_clicked() {
	client->setCUInfoVisible(false);
}

