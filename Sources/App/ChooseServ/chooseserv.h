#ifndef CHOOSESERV_H
#define CHOOSESERV_H

#include <QtWidgets>

namespace Ui {
class ChooseServ;
}

class ChooseServ : public QMainWindow
{
    Q_OBJECT

private:
      void setServ(const QString&);

public:
    explicit ChooseServ(QWidget *parent = nullptr);
    ~ChooseServ();

    QString getIP();
    QString getPort();
    QString getName();
    void closeEvent(QCloseEvent *event) override;


private slots:

    void on_ConnectButton_clicked();

    void on_actionChoose_server_triggered();

    void on_actionSave_Server_triggered();

signals:
    void onServConnectclicked();
    void chooseServClosed();

private:
    Ui::ChooseServ *ui;
};

#endif // CHOOSESERV_H
