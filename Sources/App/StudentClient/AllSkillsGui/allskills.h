#ifndef ALLSKILLS_H
#define ALLSKILLS_H

#include <QWidget>

namespace Ui {
class AllSkills;
}

// used classes
//==============================================
class SkillPack;
//==============================================

class AllSkills : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//==============================================
    AllSkills(QWidget *parent);
    ~AllSkills();
    //==============================================

public:

    // public functions
    //==============================================
    void clearAll();
    void setSkp(SkillPack * skp);
    //==============================================

private:

    //==============================================
    Ui::AllSkills *ui;
    SkillPack * skillPack;
    //==============================================

};

#endif // ALLSKILLS_H
