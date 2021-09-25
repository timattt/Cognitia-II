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






/*
Class Student Client

Connects to server and retrieve course files.
After that displays them on window
@author - arfarafar
*/

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

    /*
    filling the header of datagramm and send it to the server
    @param code - command to server that is choosen from serverCommands.h
    @param str - information sent to server
    @author - arfarafar
    */
    void sendToServer(quint16 code, const QString& str);




    /*
    after package from server is accepted, we need to manage it and save what we've received
    @author - arfarafar
    */
    void endReception();



    /*
    After the success or fail code was received from the server manages it
    @author - arfarafar
    */
    void confirmConnection();



    /*
    Loading all files that were received from server

    @author - arfarafar
    */
    void OpenCourse();
    void LoadCourse();
    void LoadSkillpack();
    void LoadStudentsProgresses();



    /*
    saving as a file data from server
    @param QDataStream& - data received from the server, needed to save as a file
    @author - arfarafar
    */
    void handleincFile(QDataStream&);

    /*
    display Course, skillpack and Student progress on the field

    @author - timattt
    */
    void display();




    /*
    clears all widgets of the programm window
    @author - arfarafar
    */
    void ClearAll();

    /*
     * deletes old fields and make new ones
     * called when returned to launcher or new server choosed
     * @author-arfarafar
     */
    void ReplaceAll();


private slots:

    void on_actionChange_Server_triggered();

    /*
    activates when we can read smth from the socket
    read it into the datafromsocket field and call endReception()
    @author - arfarafar
    */
    void slotReadyRead();


    /*
    activates when socket crashes
    shows the errorstring
    @param - error string
    @author - arfarafar
    */
    void slotError(QAbstractSocket::SocketError);


    /*
    activates when socket connection succeed
    sends to server username and welcome code
    @author - arfarafar
    */
    void slotConnected();


    /*
    activates when Connect button has been clicked in chooseserv wigget
    try to connect mSocket to server
    @author - arfarafar
    */
    void startConnection();



    void on_actionSave_all_and_send_triggered();

    void on_actionReturn_to_Launcher_triggered();


    /*
    activates when chooseserv window is closed before the time
    makes main window enabled
    @author - arfarafar
    */
    void onChooseServClosed();




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
