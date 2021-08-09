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
    void on_runButton_clicked();

private:
    Ui::Launcher *ui;

signals:
    void startSkillPackEditor();
};

#endif // LAUNCHER_H
