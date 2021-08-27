#ifndef SKILLSELECTOR_H
#define SKILLSELECTOR_H

#include <QWidget>

namespace Ui {
class SkillSelector;
}

class SkillSelector : public QWidget
{
    Q_OBJECT

public:
    explicit SkillSelector(QWidget *parent = nullptr);
    ~SkillSelector();

private:
    Ui::SkillSelector *ui;
};

#endif // SKILLSELECTOR_H
