#include "smarthelper.h"
#include "ui_smarthelper.h"
#include "../Core/logger.h"

SmartHelper::SmartHelper(QString path, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::SmartHelper) {
	ui->setupUi(this);

	QFile helpm = QFile(path);

	ASSERTM(helpm.exists(), "Help file does not exists!");

	helpm.open(QIODevice::ReadOnly);
	ui->textEdit->setMarkdown(helpm.readAll());
	helpm.readAll();
	helpm.close();

	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
}

SmartHelper::~SmartHelper()
{
    delete ui;
}
