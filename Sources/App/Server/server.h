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
#include "../Structures/fileExtensions.h"

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

private:
    Ui::Server *ui;
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
    void handleReq(QTcpSocket* client, const QByteArray& data);
    bool SendCoursetoClient(QTcpSocket* client, const QString& name);
    bool SendSkillpacktoClient(QTcpSocket* client, const QString& name);
    bool SendStudentProgresstoClient(QTcpSocket* client, const QString& name);
    bool SendFile(const QString&, QTcpSocket* client, quint16 code);
    bool handleincStudentProgressFile(QDataStream& in);

    bool CheckClient(const QString&, quint16 code, QTcpSocket* client);
    QTcpSocket* Find_Dead(const QMap<QTcpSocket*, QString>&);

private slots:
    void on_StopServ_clicked();

    void on_StartServ_clicked();

    void on_returnToL_clicked();
    void deleteFromLog();

    void on_addStudent_clicked();

    void on_chooseParent_clicked();

public slots:
    virtual void slotNewConnection();
            void slotReadClient();
            void onStart();


signals:
            void onClose();

};

#endif // SERVER_H
