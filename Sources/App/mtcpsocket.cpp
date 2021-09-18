#include "mtcpsocket.h"

mTcpSocket::mTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, SIGNAL(disconnect()), SLOT(onDisconnected()));

    connect(this, SIGNAL(disconnected()), SLOT(deleteLater()));
}


void mTcpSocket::onDisconnected(){
    emit client_disconnected(this);
}
