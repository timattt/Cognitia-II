#include "clientchat.h"
#include "ui_clientchat.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "../Core/logger.h"
#include "../CourseUnitViewer/Node/node.h"

ClientChat::ClientChat(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::ClientChat),
	currentStp(nullptr),
	currentNode(nullptr)
{
    ui->setupUi(this);
}

ClientChat::~ClientChat()
{
    delete ui;
}

void ClientChat::setStudentProgress(StudentProgress *prog) {
	clearAll();
	currentStp = prog;
}

void ClientChat::clearAll() {
	currentStp = nullptr;
	currentNode = nullptr;

	clearGui();
}

void ClientChat::nodeSelected(Node *nd) {
	clearGui();

	if (!nd) {
		currentNode = nullptr;
		return;
	}

	NOT_NULL(currentStp);
	currentNode = nd;

	for (message mes : currentStp->getMessages(nd->getName())) {
		addMessage(mes.author, mes.text);
	}
}

void ClientChat::senderNameChanged(QString name) {
	senderName = name;
}

void ClientChat::on_sendButton_clicked() {
	NOT_NULL(currentStp);
	NOT_NULL(currentNode);

	QString mes = ui->newMessage->text();
	ui->newMessage->clear();
	addMessage(senderName, mes);

	currentStp->addMessage(currentNode->getName(), senderName, mes);
}

void ClientChat::addMessage(QString author, QString text) {
	QLabel *lab = new QLabel(ui->scrollAreaWidgetContents);
	lab->setText(author + ":" + text);
	ui->verticalLayout_2->addWidget(lab);
	ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
}

void ClientChat::clearGui() {
	QList<QLabel*> toDelete;

	for (QObject *o : ui->scrollAreaWidgetContents->children()) {
		QLabel *mx = dynamic_cast<QLabel*>(o);
		if (mx != nullptr) {
			toDelete.push_back(mx);
		}
	}
	while (!toDelete.empty()) {
		delete toDelete.first();
		toDelete.pop_front();
	}
}
