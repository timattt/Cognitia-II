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



// Consts
//------------------------------------
#include "../fileExtensions.h"
#define SKILL_PACK_DELIMITER  "|^|"
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
    void save(QFile * file);
    QString toString();
    int getSkillsCount();
    Skill* getSkill(int i);
    void addSkill(Skill * sk);
    void clear();
    //------------------------------------

};

#endif // SKILLPACK_H
