#ifndef STUDENTCLIENT_H
#define STUDENTCLIENT_H

#include <QMainWindow>

namespace Ui {
class StudentClient;
}

class StudentClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentClient(QWidget *parent = nullptr);
    ~StudentClient();

private slots:
    void on_actionChange_Server_triggered();

    void on_actionSave_all_and_send_triggered();

private:
    Ui::StudentClient *ui;
};

#endif // STUDENTCLIENT_H
