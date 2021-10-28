#ifndef SKILLWIDGET_H
#define SKILLWIDGET_H

#include <QWidget>

namespace Ui {
class SkillWidget;
}

class Skill;

class SkillWidget : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//====================================================
    SkillWidget(QWidget *parent, Skill * sk);
    ~SkillWidget();
    //====================================================

private:

    // private fields
    //====================================================
    Ui::SkillWidget *ui;
    //====================================================

};

#endif // SKILLWIDGET_H
