#ifndef STUDENTCLIENT_H
#define STUDENTCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QErrorMessage>
#include "../Structures/CourseUnit/courseunit.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/StudentProgress/StudentProgress.h"
#include "../ChooseServ/chooseserv.h"
#include "../Structures/ServerCommands/serverCommands.h"

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
    StudentProgress* progress;
    ChooseServ* chooseserv;
    QString StudentName;

    QString datafromServer;
    int respCode;


public:
    explicit StudentClient(QWidget *parent = nullptr);
    ~StudentClient();


private:
    void sendToServer(int code, const QString& str);
    void endReception(int datasize);


private slots:
    void on_actionChange_Server_triggered();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void startConnection();
    void on_actionSave_all_and_send_triggered();


    void on_actionReturn_to_Launcher_triggered();

public slots:
    void onStart();

signals:
    void onClose();

private:
    Ui::StudentClient *ui;
};

#endif // STUDENTCLIENT_H
