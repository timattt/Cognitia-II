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
    void on_keepLauncherOpenCheckbox_stateChanged(int v);

private:
    Ui::Launcher *ui;
    bool mayHideWhenUnused;

signals:
    void startSkillPackEditor();
    void startCourseEditor();
    void startServer();
};

#endif // LAUNCHER_H
