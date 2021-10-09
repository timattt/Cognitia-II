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


/* Server
 *
 *
 * When launcher starts mkdir(Server) occurs that becomes new workdir
 *
 * --------------------------------------------------------------------------------
 * To proper work You need to create directories with names of students, who would
 * be allowed to attend the course with the Add student button
 * And pick the main course with set course button
 * --------------------------------------------------------------------------------
 *
 * its crusial that all course files and student dirs are in /Server direcrory
 *
 *
 * the structure of incomming datagrams are:
    quint32 length(except this field) + name + code + data

   the structure of outcomming datagrams are:
    quint32 length + code + data
 *
 *
 *

@author - arfarafar
*/





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

   // void setPort(unsigned);

private:

    /*
    filling the header of datagramm and send it to the client
    @param client - receiver
    @param code - command to server that is choosen from serverCommands.h
    @param str - information to be sent
    @author - arfarafar
    */
    void sendToClient(QTcpSocket* client, quint16, const QString&);



    /*
    after request from client is accepted, we need to reply in an apropriate way
    @param data - request (name + code + data)
    @author - arfarafar
    */
    void handleReq(QTcpSocket* client, const QByteArray& data);


    /*
    sends to client and append info to the log
    uses SendFile()
    param1 - client
    param2 - name of the clientto append to log
    @author - arfarafar
    */
    bool SendCoursetoClient(QTcpSocket* client, const QString& name);
    bool SendSkillpacktoClient(QTcpSocket* client, const QString& name);
    bool SendStudentProgresstoClient(QTcpSocket* client, const QString& name);



    /*
    sends file with @filename with @code to @client

    the block data consist of filename + filedata
    @author - arfarafar
    */
    bool SendFile(const QString& filename, QTcpSocket* client, quint16 code);



    /*
    Stores studentproggress from datastream @in to file and save it
    into the dir with students name
    the data block consist of filename + filedata
    @author - arfarafar
    */
    bool handleincStudentProgressFile(QDataStream& in);


    /*
    check whether there is a dir with student @name or the name belongs to mentor
    @return - true or false, if true - the course will be transmitted by previous function
    @author - arfarafar
    */
    bool CheckClient(const QString& name, quint16 code, QTcpSocket* client);


private slots:
    /*
    close server socket
    @author - arfarafar
    */
    void on_StopServ_clicked();

    /*
    start server socket
    @author - arfarafar
    */
    void on_StartServ_clicked();


    /*
    return
    @author - arfarafar
    */
    void on_returnToL_clicked();


    /*
    when somesocket died delete it from active clients and logs
    @author - arfarafar
    */
    void deleteFromLog();

    /*
    Creates a new dir with name of the student
    student then will be allowed to connect
    @author - arfarafar
    */
    void on_addStudent_clicked();

    /*
    pick the main courseunit
    @author - arfarafar
    */
    void on_chooseParent_clicked();

public slots:

    /*
    when new connection appears, signal is catched and socket creates
    @author - arfarafar
    */
    virtual void slotNewConnection();

    /*
    activates when client socket becomes ready to read from.
    reads the block and call hendler
    @author - arfarafar
    */
            void slotReadClient();


            /*
            current dir is set ./Server
            @author - arfarafar
            */
            void onStart();


signals:
            void onClose();

};

#endif // SERVER_H
