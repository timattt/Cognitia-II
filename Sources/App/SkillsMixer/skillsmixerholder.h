#ifndef SKILLSMIXERHOLDER_H
#define SKILLSMIXERHOLDER_H

#include <QWidget>
#include <QMap>

class Node;
class SkillsMixer;

namespace Ui {
class SkillsMixerHolder;
}

class SkillsMixerHolder : public QWidget
{
    Q_OBJECT

public:
    explicit SkillsMixerHolder(QWidget *parent = nullptr);
    ~SkillsMixerHolder();

private:
    Ui::SkillsMixerHolder *ui;
    QMap<QString, SkillsMixer*> mixers;

public slots:
	void nodeSelected(Node * nd);
    //! Call this it progress current node is changed
    void progressMade(QString name, double v);
};

#endif // SKILLSMIXERHOLDER_H
