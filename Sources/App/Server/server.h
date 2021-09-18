#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <mtcpsocket.h>
#include <QDir>
#include <QMessageBox>
#include    <QMap>

#include <QFileDialog>

#include "../Structures/ServerCommands/serverCommands.h"

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

private:
    QTcpServer* mtcpServ;
    unsigned nPort = 1917;
    quint32 nextblocksize;
    QMap<mTcpSocket*, QString> Users;

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

    void setPort(unsigned);

private:
    void sendToClient(mTcpSocket*, quint16, const QString&);
    void handleReq(mTcpSocket* client, quint32 block, const QByteArray& data);
    bool SendCoursetoClient(mTcpSocket* client, const QString& name);
    bool SendSkillpacktoClient(mTcpSocket* client, const QString& name);
    bool SendStudentProgresstoClient(mTcpSocket* client, const QString& name);
    bool SendFile(const QString&, mTcpSocket* client, quint16 code);
    bool CheckClient(const QString&);

private slots:
    void on_StopServ_clicked();

    void on_StartServ_clicked();

    void on_returnToL_clicked();
    void deleteFromLog(mTcpSocket*);

    void on_addStudent_clicked();

public slots:
    virtual void slotNewConnection();
            void slotReadClient();


signals:
            void onClose();

private:
    Ui::Server *ui;
};

#endif // SERVER_H
