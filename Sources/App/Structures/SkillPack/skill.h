#ifndef SKILL_H
#define SKILL_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QFileInfo>

// Constants
//------------------------------------
#define SKILL_DELIMITER  "|^|"
//------------------------------------


class Skill : public QObject {
    Q_OBJECT

public:

    // Constructor
    //------------------------------------
    Skill(QObject *parent = nullptr);
    //------------------------------------

private:

    // Fields
    //------------------------------------
    QVector<QString> levelsDescriptions;
    //------------------------------------

public:

    // Global methods
    //------------------------------------
    void loadSkill(QFile * file);
    void saveSkill(QFile * dest);
    void addLevel(QString descr);
    void clear();
    int getLevelsCount();
    QString getLevelDescription(int i);
    QString toString();
    //------------------------------------
};

#endif // SKILL_H
