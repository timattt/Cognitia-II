#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

private:
    QTcpServer* mtcpServ;
    unsigned nPort = 1917;
    quint16 nextblocksize;


public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

    void setPort(unsigned);

private:
    void sendToClient(QTcpSocket*, const QString&);

private slots:
    void on_StopServ_clicked();

    void on_StartServ_clicked();

    void on_returnToL_clicked();

public slots:
    virtual void slotNewConnection();
            void slotReadClient();


signals:
            void onClose();

private:
    Ui::Server *ui;
};

#endif // SERVER_H
