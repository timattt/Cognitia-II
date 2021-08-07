#include "skill.h"

// Constants
//------------------------------------
const char * SKILL_DELIMITER = "|^|";
//------------------------------------

Skill::Skill(QObject *parent) : QObject(parent), levelsDescriptions() {

}

void Skill::loadSkill(QFile * file) {
    if (!file->exists()) {
        throw QString("No such skill file [" + file->fileName() + "]");
    }

    QFileInfo info(*file);
    qDebug() << "Loading skill from file" << info.fileName();

    QString dat = nullptr;

    if (file->open(QIODevice::ReadOnly)) {
        QTextStream stream(file);
        dat = stream.readAll();
        file->close();
    } else {
        throw QString("Can not open skill file [" + file->fileName() + "]");
    }

    QStringList divs = dat.split(SKILL_DELIMITER, Qt::SkipEmptyParts);

    if (divs.size() == 0) {
        throw QString("Bad skill file [" + file->fileName() + "]");
    }

    setObjectName(divs[0]);

    levelsDescriptions.clear();
    levelsDescriptions.reserve(divs.size() - 1);

    for (int i = 0; i < divs.size() - 1; i++) {
        levelsDescriptions.push_back(divs[i + 1]);
    }
}

void Skill::saveSkill(QFile * dest) {
    qDebug() << "Saving skill" << objectName();

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

void Skill::print()
{
    qInfo() << "Skill:" << objectName() << "Total levels:" << getLevelsCount();

    for (int i = 0; i < levelsDescriptions.size(); i++) {
        qInfo() << "-> Level" << (i + 1) << ":" << levelsDescriptions[i];
    }
}
