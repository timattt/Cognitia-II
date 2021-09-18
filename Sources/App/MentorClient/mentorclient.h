#ifndef MENTORCLIENT_H
#define MENTORCLIENT_H

#include <QtWidgets>

namespace Ui {
class MentorClient;
}

class CourseUnit;
class SkillPack;
class StudentProgress;
class Node;

class MentorClient : public QMainWindow
{
    Q_OBJECT

public:

	// constructor and destructor
	//===================================================
    explicit MentorClient(QWidget *parent = nullptr);
    ~MentorClient();
    //===================================================

private:

    // private fields
    //===================================================
    Ui::MentorClient *ui;
    CourseUnit * headCourseUnit;
    SkillPack * skillPack;
    QMap<QString, StudentProgress*> students;//name -> student progress
    QString currentStudent;
    //===================================================

private:

    // private functions
    //===================================================
    //! call this when you want to extract data from courseunit and studentprogress structres into the gui.
    void display();
    //! call this when you want to extract data from gui into studentprogress.
    //! After this function they will be ready to send
    void pack();
    //===================================================

signals:

	// signals
	//===================================================
	void onClose();
	//===================================================

private slots:

	// private slots
	//===================================================
	void on_studentChooser_currentTextChanged(const QString &arg1);
	//===================================================

public slots:

	// public slots
	//===================================================
	void nodeSelected(Node* nd);
	//===================================================

};

#endif // MENTORCLIENT_H
