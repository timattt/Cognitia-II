#ifndef SKILLSFLOWER_H
#define SKILLSFLOWER_H

#include <QWidget>

namespace Ui {
class SkillsFlower;
}

class CourseUnit;
class FlowerScene;
class StudentProgress;
class Node;

class SkillsFlower : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//====================================================
    explicit SkillsFlower(QWidget *parent = nullptr);
    ~SkillsFlower();
    //====================================================

private:

    // Fields
    //====================================================
    Ui::SkillsFlower *ui;
    FlowerScene * scene;
    //====================================================

public slots:

    // public slots
    //====================================================
    //! from CU to this gui
    void unpack(CourseUnit * cu, StudentProgress * prg);
    void unpack(Node * nd);
    //! from gui to CU
    void pack(CourseUnit * cu, StudentProgress * prg);
    //! Clears everything
    void clearAll();
    //====================================================

signals:

	// signals
	//====================================================
	void skillLevelChanged(QString skillName, double val);
	//====================================================

};

#endif // SKILLSFLOWER_H
