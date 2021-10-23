#ifndef STUDENTCLIENT_H
#define STUDENTCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QErrorMessage>
#include "../Structures/ServerCommands/serverCommands.h"

namespace Ui {
class StudentClient;
}

// used classes
//========================================================
class Node;
class SkillPack;
class StudentProgress;
class ChooseServ;
class CourseUnit;
class CourseUnitViewer;
//========================================================

/**!
 Class Student Client

 Connects to server and retrieve course files.
 After that displays them on window


 the structure of outcomming datagrams are:
 quint32 length(except this field) + name + code + data

 the structure of incomming datagrams are:
 quint32 length(code+data) + code + data


 everytime you connect to the server occurs mkdir(name+IP) that becomes
 new working directory.
 when you changing the server or leave to the launcher, the curdir is setted back


 @author - arfarafar
 */
class StudentClient: public QMainWindow {
Q_OBJECT

private:

	// private fields
	//========================================================
	Ui::StudentClient *ui;
	QTcpSocket *mSocket;
	quint32 nextBlockSize;
    /**!
	 * Current skillpack received from server.
	 * @author timattt
	 */
	SkillPack *skillpack;
    /**!
	 * Current courseunit received from server.
	 * @author timattt
	 */
	CourseUnit *courseUnit;
    /**!
	 * Current studentprogress received from server.
	 * @author timattt
	 */
	StudentProgress *progress;
	ChooseServ *chooseserv;
	QString StudentName;

	QByteArray datafromServer;
	quint16 respCode;

	bool inworkingrepository;
	//========================================================

public:

	// constructor and destructor
	//========================================================
	StudentClient();
	~StudentClient();
	//========================================================

public:

	// public functions
	//========================================================
	void setCUInfoVisible(bool v);
	//========================================================

private:

	// private functions
	//========================================================
    /**!
	 filling the header of datagramm and send it to the server
	 @param code - command to server that is choosen from serverCommands.h
	 @param str - information sent to server
	 @author - arfarafar
	 */
	void sendToServer(quint16 code, const QString &str);

    /**!
	 after package from server is accepted, we need to manage it and save what we've received
	 @author - arfarafar
	 */
	void endReception();

    /**!
	 After the success or fail code was received from the server manages it
	 @author - arfarafar
	 */
	void confirmConnection();

    /**!
	 Loading all files that were received from server

	 @author - arfarafar
	 */
	void OpenCourse();
	void LoadCourse();
	void LoadSkillpack();
	void LoadStudentsProgresses();

    /**!
	 saving as a file data from server
	 the data block is filename + filedata
	 @param QDataStream& - data received from the server, needed to save as a file
	 @author - arfarafar
	 */
	void handleincFile(QDataStream&);

    /**!
	 display Course, skillpack and Student progress on the field

	 @author - timattt
	 */
	void display();

    /**!
	 clears all widgets of the programm window
	 @author - arfarafar
	 */
	void ClearAll();

    /**!
	 * deletes old fields and make new ones
	 * called when returned to launcher or new server choosed
	 * @author - arfarafar
	 */
	void ReplaceAll();
	//========================================================

private slots:

	// private slots
	//========================================================
	void on_actionChange_Server_triggered();

    /**!
	 activates when we can read smth from the socket
	 read it into the datafromsocket field and call endReception()
	 @author - arfarafar
	 */
	void slotReadyRead();

    /**!
	 activates when socket crashes
	 shows the errorstring
	 @param - error string
	 @author - arfarafar
	 */
	void slotError(QAbstractSocket::SocketError);

    /**!
	 activates when socket connection succeed
	 sends to server username and welcome code
	 @author - arfarafar
	 */
	void slotConnected();

    /**!
	 activates when Connect button has been clicked in chooseserv wigget
	 try to connect mSocket to server
	 @author - arfarafar
	 */
	void startConnection();

	void on_actionSave_all_and_send_triggered();

	void on_actionExpand_triggered();

	void on_actionSet_course_unit_triggered();

	void on_actionSet_skill_pack_triggered();

	void on_actionReturn_to_Launcher_triggered();

    /**!
	 activates when chooseserv window is closed before the time
	 makes main window enabled
	 @author - arfarafar
	 */
	void onChooseServClosed();
	//========================================================

    void on_actionHelp_me_triggered();

public:

    // public slots
    //========================================================
    /**
     * Gives CUV instance from this UI.
     * @param CUV object
     * @author timattt
     */
    CourseUnitViewer* getCourseUnitViewer();
    //========================================================

public slots:

	// public slots
	//===================================================
    /***
	 * Signal from courseunitviewer.
	 * Sets panels for this node.
	 * @author timattt
	 */
	void nodeSelected(Node *nd);
	void onStart();
	void nodeDoubleClicked(Node * nd);
	//===================================================

signals:

	// signals
	//========================================================
	void onClose();
	//========================================================
};

#endif // STUDENTCLIENT_H
