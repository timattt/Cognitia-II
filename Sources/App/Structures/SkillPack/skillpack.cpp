#include "skillpack.h"

// Consts
//------------------------------------
const char * SKILL_PACK_FILE_EXTENSION = ".cognitiaSkillPack";
const char * SKILL_FILE_EXTENSION = ".cognitiaSkill";
const char * SKILL_PACK_DELIMITER = "|^|";
//------------------------------------

SkillPack::SkillPack(QObject *parent) : QObject(parent)
{

}

void SkillPack::load(QFile * file)
{
    if (!file->exists()) {
        throw QString("Skill pack file [" + file->fileName() + "] not exists");
    }

    QFileInfo info = QFileInfo(*file);

    qDebug() << "Loading skillPack from" << info.fileName();

    QString dat = nullptr;

    if (file->open(QIODevice::ReadOnly)) {
        QTextStream stream(file);
        dat = stream.readAll();
        file->close();
    } else {
        throw QString("Can not open skill pack file [" + file->fileName() + "]");
    }

    QStringList divs = dat.split(SKILL_PACK_DELIMITER, Qt::SkipEmptyParts);

    if (divs.empty()) {
        throw QString("Empty skill pack file [" + file->fileName() + "]");
    }

    this->setObjectName(divs[0]);
    skills.reserve(divs.size() - 1);

    QDir dr = info.dir();

    for (int i = 0; i < divs.size() - 1; i++) {
        QString skillPath = divs[i + 1];
        QFile skillFile = dr.filePath(skillPath);
        Skill * skill = new Skill(this);
        try {
            skill->loadSkill(&skillFile);
        } catch (QString err) {
            throw QString("Error while loading skill [" + skillPath + "] with error: " + err);
        }

        skills.push_back(skill);
    }
}

void SkillPack::save(QFile * file)
{
    qDebug() << "Saving skillPack" << objectName();

    QFileInfo in = QFileInfo(*file);
    QDir dr = in.dir();

    if (!dr.exists()) {
        dr.mkpath(dr.absolutePath());
    }

    if (file->open(QIODevice::WriteOnly)) {
        QTextStream stream(file);

        stream << objectName() << SKILL_PACK_DELIMITER;

        for (int i = 0; i < getSkillsCount(); i++) {
            QString skillFileName = skills[i]->objectName() + SKILL_FILE_EXTENSION;

            QFile skillFile = QFile(dr.filePath(skillFileName));

            try {
                skills[i]->saveSkill(&skillFile);
            } catch (QString err) {
                throw QString("Error while saving skill [" + skills[i]->objectName() + "] with error: " + err);
            }

            stream << skillFileName << SKILL_PACK_DELIMITER;
        }

        file->close();
    } else {
        throw QString("Can not open skill pack file [" + file->fileName() + "]");
    }
}

void SkillPack::print()
{
    qInfo() << "Skill pack" << objectName() << ", skills count" << skills.size();
    for (int i = 0; i < skills.size(); i++) {
        skills[i]->print();
    }
}

int SkillPack::getSkillsCount()
{
    return skills.size();
}

Skill *SkillPack::getSkill(int i)
{
    return skills[i];
}

void SkillPack::addSkill(Skill *sk)
{
    skills.push_back(sk);
    sk->setParent(this);
}

void SkillPack::clear()
{
    qDeleteAll(skills);
    skills.clear();
}
