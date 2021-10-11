#ifndef CHOOSESERV_H
#define CHOOSESERV_H

#include <QtWidgets>

namespace Ui {
class ChooseServ;
}

/**!
 * Window to connect to the server
 * Opens immediatly after the launch of client
 * Shows every time user want to change current server
 * after the connect button emit signal to client.
 * author - Arfarafar
 */


class ChooseServ : public QMainWindow
{
    Q_OBJECT

private:
    /**
     *
    open server file and fill out all fields

    @arg1 filename
    author - Arfarafar
    */
      void setServ(const QString&);

public:
    explicit ChooseServ(QWidget *parent = nullptr);
    ~ChooseServ();


    /**
    returns ip from the textedit field
    @return - IP adress
    author - Arfarafar
    */
    QString getIP();

    /**
    returns port from the textedit field
    @return - port number as string
    author - Arfarafar
    */
    QString getPort();

    /**
    returns login name that will be sent to server
    author - Arfarafar
    */
    QString getName();

    /**
    disables the connect button while waiting server response
    author - Arfarafar
    */
    void setButtonDisabled();


    /**
    enables the connect button
    author - Arfarafar
    */
    void setButtonEnabled();


    /**
    when chooseserv window closes without connection(is closed by user)
    returns client back to life by emiting signal
    author - Arfarafar
    */
    void closeEvent(QCloseEvent *event) override;


private slots:

    /**
    emit signal to client to inform about the action
    author - Arfarafar
    */
    void on_ConnectButton_clicked();

    /**
    open a dialog window to pick up the server file
    author - Arfarafar
    */
    void on_actionChoose_server_triggered();


    /**
    open a dialog window to save information of the fields to the server file
    author - Arfarafar
    */
    void on_actionSave_Server_triggered();

signals:
    void onServConnectclicked();
    void chooseServClosed();

private:
    Ui::ChooseServ *ui;
};

#endif // CHOOSESERV_H
