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

public:
    explicit ChooseServ(QWidget *parent = nullptr);
    ~ChooseServ();

    void setServ(const QString&);

private slots:

    void on_ConnectButton_clicked();

    void on_actionChoose_server_triggered();

    void on_actionSave_Server_triggered();

private:
    Ui::ChooseServ *ui;
};

#endif // CHOOSESERV_H
