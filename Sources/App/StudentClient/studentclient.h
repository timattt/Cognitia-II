#ifndef STUDENTCLIENT_H
#define STUDENTCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include "../Structures/CourseUnit/courseunit.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "../ChooseServ/chooseserv.h"

namespace Ui {
class StudentClient;
}

class StudentClient : public QMainWindow
{
    Q_OBJECT

private:
    QTcpSocket* mSocket;
    quint16 nextBlockSize = 0;
    SkillPack skillpack;
    CourseUnit courseUnit;
    StudentProgress progress;
    ChooseServ* chooseserv;

public:
    explicit StudentClient(QWidget *parent = nullptr);
    ~StudentClient();

private slots:
    void on_actionChange_Server_triggered();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotSendToServer();
    void startConnection();
    void on_actionSave_all_and_send_triggered();

public slots:
    void onStart();

signals:
    void onClose();

private:
    Ui::StudentClient *ui;
};

#endif // STUDENTCLIENT_H
