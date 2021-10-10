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

	// constructor and destructor
	//====================================================
    Launcher();
    ~Launcher();
    //====================================================

private slots:

	// private slots
	//====================================================
    void on_runButton_clicked();
    void on_keepLauncherOpenCheckbox_stateChanged(int v);
    //====================================================

private:

    // private fields
    //====================================================
    Ui::Launcher *ui;
    bool mayHideWhenUnused;
    //====================================================

signals:

	// signals
	//====================================================
    void startSkillPackEditor();
    void startCourseEditor();
    void startServer();
    void startStudentClient();
    void startMentorClient();
    //====================================================
};

#endif // LAUNCHER_H
