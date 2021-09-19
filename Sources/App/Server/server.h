#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
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
    QMap<QTcpSocket*, QString> Users;
    QMap<QTcpSocket*, QString> Mentors;

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

    void setPort(unsigned);

private:
    void sendToClient(QTcpSocket*, quint16, const QString&);
    void handleReq(QTcpSocket* client, quint32 block, const QByteArray& data);
    bool SendCoursetoClient(QTcpSocket* client, const QString& name);
    bool SendSkillpacktoClient(QTcpSocket* client, const QString& name);
    bool SendStudentProgresstoClient(QTcpSocket* client, const QString& name);
    bool SendFile(const QString&, QTcpSocket* client, quint16 code);
    bool CheckClient(const QString&, quint16 code);
    QTcpSocket* Find_Dead(const QMap<QTcpSocket*, QString>&);

private slots:
    void on_StopServ_clicked();

    void on_StartServ_clicked();

    void on_returnToL_clicked();
    void deleteFromLog();

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
