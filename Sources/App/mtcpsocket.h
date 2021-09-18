#ifndef MTCPSOCKET_H
#define MTCPSOCKET_H

#include <QTcpSocket>

class mTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit mTcpSocket(QObject *parent = nullptr);


public slots:
    void onDisconnected();

signals:
    void client_disconnected(mTcpSocket*);
};

#endif // MTCPSOCKET_H
