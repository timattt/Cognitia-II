#ifndef SKILLSMIXER_H
#define SKILLSMIXER_H

#include <QWidget>

namespace Ui {
class SkillsMixer;
}

class Node;

class SkillsMixer : public QWidget
{
    Q_OBJECT

public:
    SkillsMixer(QWidget *parent, double from, double to, double val, QString name, Node *nd);
    ~SkillsMixer();

private:

    Node * node;
    Ui::SkillsMixer *ui;

private slots:
	void on_horizontalSlider_valueChanged(int a);

public:
	void setValue(double v);
};

#endif // SKILLSMIXER_H
