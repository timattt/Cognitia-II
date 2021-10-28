#ifndef ALLSKILLS_H
#define ALLSKILLS_H

#include <QtWidgets>

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
    AllSkills(QWidget *parent = nullptr);
    ~AllSkills() = default;
    //==============================================

public slots:

	// public slots
	//==============================================
	void setSkp(SkillPack * skp);
	void clearAll();
	//==============================================

private:

    //==============================================
    SkillPack * skillPack;
    QVBoxLayout *layout;
    //==============================================

};

#endif // ALLSKILLS_H
