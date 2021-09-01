#ifndef CHOOSESERV_H
#define CHOOSESERV_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

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


private slots:

    void on_ConnectButton_clicked();

    void on_actionChoose_server_triggered();

    void on_actionSave_Server_triggered();

signals:
    void onServConnectclicked();

private:
    Ui::ChooseServ *ui;
};

#endif // CHOOSESERV_H
