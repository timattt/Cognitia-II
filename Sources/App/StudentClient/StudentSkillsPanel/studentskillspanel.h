#ifndef ALLSKILLS_H
#define ALLSKILLS_H

#include <QtWidgets>

// used classes
//==============================================
class SkillPack;
//==============================================

class StudentSkillsPanel : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//==============================================
    StudentSkillsPanel(QWidget *parent = nullptr);
    ~StudentSkillsPanel() = default;
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
