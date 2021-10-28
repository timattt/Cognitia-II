#include "outdeltaskills.h"
#include "ui_outdeltaskills.h"

OutDeltaSkills::OutDeltaSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutDeltaSkills)
{
    ui->setupUi(this);
}

OutDeltaSkills::~OutDeltaSkills()
{
    delete ui;
}

void OutDeltaSkills::clearAll() {
}

void OutDeltaSkills::progressMade(QString skill, double lev) {
}

void OutDeltaSkills::unpack(Node *nd) {
}
