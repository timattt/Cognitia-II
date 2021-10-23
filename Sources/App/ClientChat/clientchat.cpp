#include "clientchat.h"
#include "ui_clientchat.h"

ClientChat::ClientChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientChat)
{
    ui->setupUi(this);
}

ClientChat::~ClientChat()
{
    delete ui;
}
