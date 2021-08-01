#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QWidget>

namespace Ui {
class Launcher;
}

class Launcher : public QWidget
{
    Q_OBJECT

public:
    explicit Launcher();
    ~Launcher();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Launcher *ui;
};

#endif // LAUNCHER_H
