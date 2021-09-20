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

class Node;

class StudentClient : public QMainWindow
{
    Q_OBJECT

private:
	Ui::StudentClient *ui;
    QTcpSocket* mSocket;
    quint32 nextBlockSize;
    SkillPack* skillpack;
    CourseUnit* courseUnit;
    StudentProgress* progress;
    ChooseServ* chooseserv;
    QString StudentName;

    QByteArray datafromServer;
    quint16 respCode;

    bool inworkingrepository;

public:
    explicit StudentClient(QWidget *parent = nullptr);
    ~StudentClient();


private:
    void sendToServer(quint16 code, const QString& str);
    void endReception();
    void confirmConnection();
    void OpenCourse();
    void handleincFile(QDataStream&);

    void LoadCourse();
    void LoadSkillpack();
    void LoadStudentsProgresses();
    void display();
    void ClearAll();


private slots:
    void on_actionChange_Server_triggered();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void startConnection();
    void on_actionSave_all_and_send_triggered();
    void onChooseServClosed();
    void on_actionReturn_to_Launcher_triggered();

public slots:
    public slots:

    	// public slots
    	//===================================================
    	void nodeSelected(Node* nd);
    	void onStart();

signals:
    void onClose();
};

#endif // STUDENTCLIENT_H
