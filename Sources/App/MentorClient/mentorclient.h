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

/**!
 Class Mentor Client

 Connects to server and retrieve course files and studentprogresses
 After that displays them on window
 allows to change the marks and levels of the student and send them back to server
 without the connection u cant do anything

 everytime you connect to the server occurs mkdir(name+IP) that becomes
 new working directory.
 when you changing the server or leave to the launcher, the curdir is setted back


 the structure of outcomming datagrams are:
 quint32 length(except this field) + name + code + data

 the structure of incomming datagrams are:
 quint32 length + code + data

 @author - arfarafar
 */
class MentorClient: public QMainWindow {
Q_OBJECT

public:

	// constructor and destructor
	//===================================================
	MentorClient();
	~MentorClient();
	//===================================================

private:

	// private fields
	//===================================================
	Ui::MentorClient *ui;
    /**!
	 * Main courseunit pointer received from server.
	 * Its children will be shown in courseunitviewer.
	 * @author timattt
	 */
	CourseUnit *headCourseUnit;
    /**!
	 * Skillpack received from server.
	 * @author timattt
	 */
	SkillPack *skillPack;
    /**!
	 * QMap representing students.
	 * {QString: studentName } -> {StudentProgress pointer : progress of this student}
	 * @author timattt
	 */
	QMap<QString, StudentProgress*> students;
    /**!
	 * Currently selected student.
	 * @author timattt
	 */
	QString currentStudent;

	QTcpSocket *mSocket;
	quint32 nextBlockSize = 0;
	ChooseServ *chooseserv;

	QByteArray datafromServer;
	quint16 respCode;

	bool inworkingrepository = false;
	//===================================================

private:

	// private functions
	//===================================================
    /**!
	 * Display Course, skillpack and Student progress on the field.
	 * Call this when you have new headCourseUnit, skillPack and students progresses.
	 * @author timattt
	 */
	void display();

    /**! call this when you want to extract data from gui into studentprogress.
	 * After this function they will be ready to send.
	 * @author timattt
	 */
	void pack();

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
	 if the code is fail then close the socket
	 otherwise opens course
	 @author - arfarafar
	 */
	void confirmConnection();

    /**!
	 Loading all files that were received from server

	 @author - arfarafar
	 */
	void OpenCourse();
	void LoadSkillpack();
	void LoadCourse();
	void LoadStudentsProgresses();

    /**!
	 saving as a file data from server
	 the data block is filename + filedata
	 @param QDataStream& - data received from the server, needed to save as a file
	 @author - arfarafar
	 */
	void handleincFile(QDataStream&);

    /**!
	 sends file to server
	 @param code - code of request to the server
	 @author - arfarafar
	 */
	bool SendFile(QFile *file, quint16 code);

    /**!
	 send all studentprogresses from localsave dir to server
	 @author - arfarafar
	 */
	void sendAll();
	//===================================================

signals:

	// signals
	//===================================================
    /**!
	 * This is used by launcher. When this widget is closed launcher is opened again.
	 */
	void onClose();
	//===================================================

private slots:

	// private slots
	//===================================================
    /**!
	 * Here student progress is set to all editors.
	 * When user changed current student.
	 * @author timattt
	 */
	void on_studentChooser_currentTextChanged(const QString &arg1);

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

    /**!
	 activates when chooseserv window is closed before the time
	 makes main window enabled
	 @author - arfarafar
	 */
	void onChooseServClosed();

    /**!
	 leave the current working dir and shows choseserv window
	 @author - arfarafar
	 */
	void on_actionChoose_Server_triggered();

    /**!
	 Clears and replace all fields, closes socket, change workingrep
	 emit onClose
	 @author - arfarafar
	 */
	void on_actionReturn_to_Launcher_triggered();

    /**!
	 mkdir localsave and stores there all studentprogresses
	 then call sendAll()
	 @author - arfarafar
	 */
	void on_actionSave_all_and_send_triggered();
	//===================================================

    void on_actionHelp_triggered();

public slots:

	// public slots
	//===================================================
    /**!
	 * This is called when user selects node in courseunitviewer.
	 * It updates all panels.
	 * @param nd - new node. Or nullptr if nothing is selected, so head is set to all panels.
	 * @author timattt
	 */
	void nodeSelected(Node *nd);
    /**!
	 * This is called when any progress is made for current student.
	 * @author timattt
	 */
	void progressMade(QString skill, double val);

    /**!
	 shows chooseserv window
	 @author - arfarafar
	 */
	void onStart();
	//===================================================
};

#endif // MENTORCLIENT_H
