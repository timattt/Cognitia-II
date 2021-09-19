#ifndef MENTORCLIENT_H
#define MENTORCLIENT_H

#include <QtWidgets>
#include <QStringList>
#include <QTcpSocket>
#include "../Structures/SkillPack/skillpack.h"
#include "../ChooseServ/chooseserv.h"
#include "../Structures/ServerCommands/serverCommands.h"

namespace Ui {
class MentorClient;
}

class CourseUnit;
class SkillPack;
class StudentProgress;
class Node;

class MentorClient : public QMainWindow
{
    Q_OBJECT

public:

	// constructor and destructor
	//===================================================
    explicit MentorClient(QWidget *parent = nullptr);
    ~MentorClient();
    //===================================================

private:

    // private fields
    //===================================================
    Ui::MentorClient *ui;
    CourseUnit * headCourseUnit;
    SkillPack * skillPack;
    QMap<QString, StudentProgress*> students;//name -> student progress
    QString currentStudent;

    QTcpSocket* mSocket;
    quint32 nextBlockSize = 0;
    ChooseServ* chooseserv;

    QByteArray datafromServer;
    quint16 respCode;

    bool inworkingrepository = false;
    //===================================================

private:

    // private functions
    //===================================================
    //! call this when you want to extract data from courseunit and studentprogress structres into the gui.
    void display();
    //! call this when you want to extract data from gui into studentprogress.
    //! After this function they will be ready to send
    void pack();

    void sendToServer(quint16 code, const QString& str);
    void endReception();
    void confirmConnection();
    void OpenCourse();
    void handleincFile(QDataStream&);

    void LoadSkillpack();
    void LoadCourse();
    void LoadStudentsProgresses();
    //===================================================

signals:

	// signals
	//===================================================
	void onClose();
	//===================================================

private slots:

	// private slots
	//===================================================
	void on_studentChooser_currentTextChanged(const QString &arg1);

    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();
    void startConnection();
    void onChooseServClosed();
	//===================================================

    void on_actionChoose_Server_triggered();

    void on_actionReturn_to_Launcher_triggered();

    void on_actionSave_all_and_send_triggered();

public slots:

	// public slots
	//===================================================
	void nodeSelected(Node* nd);

    void onStart();
	//===================================================
};

#endif // MENTORCLIENT_H
