#include "skill.h"
#include "../../Core/logger.h"


Skill::Skill(QObject *parent) : QObject(parent), levelsDescriptions() {

}

void Skill::loadSkill(QFile * file) {
    if (!file->exists()) {
        throw QString("No such skill file [" + file->fileName() + "]");
    }

    QFileInfo info(*file);
    SAY("Loading skill from file " + info.fileName());

    QString dat = nullptr;

    if (file->open(QIODevice::ReadOnly)) {
        QTextStream stream(file);
        dat = stream.readAll();
        file->close();
    } else {
        throw QString("Can not open skill file [" + file->fileName() + "]");
    }

    QStringList divs = dat.split(SKILL_DELIMITER);

    if (divs.size() < 2) {
        throw QString("Bad skill file [" + file->fileName() + "]");
    }

    setObjectName(divs[0]);

    levelsDescriptions.clear();
    levelsDescriptions.reserve(divs.size() - 2);

    for (int i = 0; i < divs.size() - 2; i++) {
        levelsDescriptions.push_back(divs[i + 1]);
    }
}

void Skill::saveSkill(QFile * dest) {
    SAY("Saving skill " + objectName());

    if (dest->open(QIODevice::WriteOnly)) {
        QTextStream stream(dest);

        stream << objectName() << SKILL_DELIMITER;

        for (int i = 0; i < getLevelsCount(); i++) {
            stream << levelsDescriptions[i] << SKILL_DELIMITER;
        }

        dest->close();
    } else {
        throw QString("Can not open skill file [" + dest->fileName() + "]");
    }

}

void Skill::addLevel(QString descr)
{
    levelsDescriptions.push_back(descr);
}

void Skill::clear()
{
    levelsDescriptions.clear();
}

int Skill::getLevelsCount()
{
    return levelsDescriptions.size();
}

QString Skill::getLevelDescription(int i)
{
    return levelsDescriptions[i];
}

QString Skill::toString()
{
    QString res = QString("Skill:") + objectName() + "Total levels:" + QString::number(getLevelsCount());

    for (int i = 0; i < levelsDescriptions.size(); i++) {
        res +=  QString("-> Level") + QString::number(i + 1) + ":" + levelsDescriptions[i];
    }

    return res;
}
