#ifndef ALLSKILLS_H
#define ALLSKILLS_H

#include <QWidget>

namespace Ui {
class AllSkills;
}

class AllSkills : public QWidget
{
    Q_OBJECT

public:
    explicit AllSkills(QWidget *parent = nullptr);
    ~AllSkills();

private:
    Ui::AllSkills *ui;
};

#endif // ALLSKILLS_H
