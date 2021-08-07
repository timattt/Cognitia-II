#ifndef SKILLPACK_H
#define SKILLPACK_H

// Includes
//------------------------------------
#include <QObject>
#include <QVector>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "skill.h"
//------------------------------------


class SkillPack : public QObject
{
    Q_OBJECT

public:

    // Constructor
    //------------------------------------
    SkillPack(QObject *parent = nullptr);
    //------------------------------------

private:

    // Fields
    //------------------------------------
    QString name;
    QVector<Skill*> skills;
    //------------------------------------

public:

    // Methods
    //------------------------------------
    void load(QFile * file);
    void save(QDir dr);
    void print();
    int getSkillsCount();
    void addSkill(Skill * sk);
    void clear();
    //------------------------------------

};

#endif // SKILLPACK_H
