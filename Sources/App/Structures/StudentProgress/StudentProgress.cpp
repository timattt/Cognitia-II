/*
 * StudentProgress.cpp
 *
 *  Created on: Aug 29, 2021
 *      Author: timat
 */

#include "StudentProgress.h"
#include "../CourseUnit/courseunit.h"

StudentProgress::StudentProgress(QObject *parent) : QObject(parent)
{
}

StudentProgress::~StudentProgress() {
}

void StudentProgress::load(QFile *file) {
    if (!file->exists()) {
        throw QString("Student progress file [" + file->fileName() + "] not exists");
    }

    QFileInfo info = QFileInfo(*file);

    qDebug() << "Loading student progress from" << info.fileName();

    QString dat = nullptr;

    if (file->open(QIODevice::ReadOnly)) {
        QTextStream stream(file);
        dat = stream.readAll();
        file->close();
    } else {
        throw QString("Can not open student progress file [" + file->fileName() + "]");
    }

    setObjectName(info.baseName());

    QStringList divs = dat.split(STUDENT_PROGRESS_DELIMITER, Qt::SkipEmptyParts);

    if (divs.empty()) {
        throw QString("Empty student progress file [" + file->fileName() + "]");
    }

    for (QString raw : divs) {
    	QStringList split = raw.split(STUDENT_PROGRESS_COURSEUNIT_DELIMITER, Qt::SkipEmptyParts);

    	QString courseUnitName = split[0];

    	int totalSkills = split[1].toInt();

    	for (int i = 0; i < totalSkills * 2; i+=2) {
    		QString skillName = split[i + 2];
    		double lev = split[i + 3].toDouble();
    		addProgress(courseUnitName, skillName, lev);
    	}

    	int totalMes = split[2 + totalSkills * 2].toInt();

    	for (int i = 0; i < totalMes * 2; i += 2) {
    		QString author = split[3 + totalSkills * 2 + i];
    		QString text = split[3 + totalSkills * 2 + i + 1];

    		messages[courseUnitName].push_back({author, text});
    	}
    }
}

void StudentProgress::save(QFile *file) {
    qDebug() << "Saving student progress" << objectName();

    QFileInfo in = QFileInfo(*file);
    QDir dr = in.dir();

    if (!dr.exists()) {
        dr.mkpath(dr.absolutePath());
    }

    if (file->open(QIODevice::WriteOnly)) {
        QTextStream stream(file);

        for (QString courseUnitName : progress.keys()) {
        	QMap<QString, double> skills = progress[courseUnitName];

            stream << courseUnitName << STUDENT_PROGRESS_COURSEUNIT_DELIMITER;

            stream << skills.size() << STUDENT_PROGRESS_COURSEUNIT_DELIMITER;

            for (QString skill : skills.keys()) {
            	double level = skills[skill];

            	stream << skill << STUDENT_PROGRESS_COURSEUNIT_DELIMITER << QString::number(level) << STUDENT_PROGRESS_COURSEUNIT_DELIMITER;
            }

            int totalMes = messages[courseUnitName].size();

            stream << totalMes << STUDENT_PROGRESS_COURSEUNIT_DELIMITER;

            for (message mes : messages[courseUnitName]) {
            	stream << mes.author << STUDENT_PROGRESS_COURSEUNIT_DELIMITER << mes.text << STUDENT_PROGRESS_COURSEUNIT_DELIMITER;
            }

            stream << STUDENT_PROGRESS_DELIMITER;

        }

        file->close();
    } else {
        throw QString("Can not open student progress file [" + file->fileName() + "]");
    }
}

void StudentProgress::addProgress(QString courseUnit, QString skill, double level) {
	progress[courseUnit][skill] = level;
}

QString StudentProgress::toString() {
	QString r = "";

	r += "Student progress name: " + objectName() + "\n";

	for (QString courseUnitName : progress.keys()) {
		QMap<QString, double> skills = progress[courseUnitName];

		r += "CourseUnit: " + courseUnitName + "\n";

		for (QString skill : skills.keys()) {
			double lev = skills[skill];
			r += "---> Skill:" + skill + " level: " + QString::number(lev) + "\n";
		}

		for (message m : messages[courseUnitName]) {
			r += "Message from " + m.author + " with text: " + m.text + "\n";
		}
	}

	return r;
}

double StudentProgress::getLevel(QString courseUnit, QString skill) {
	return progress[courseUnit][skill];
}

bool StudentProgress::containsLevel(QString courseUnit, QString skill) {
	return progress.contains(courseUnit) && progress[courseUnit].contains(skill);
}

void StudentProgress::collectAbsolute(CourseUnit *cu, QMap<QString, double> &res) {
	if (!progress.contains(cu->objectName())) {
		return;
	}

	for (QString sk : progress[cu->objectName()].keys()) {
		double val = progress[cu->objectName()][sk];

		if (cu->containsInSkill(sk)) {
			if (qAbs(val - cu->getInSkillLevel(sk)) < 0.001) {
				continue;
			}
		} else if (cu->containsOutSkill(sk)) {
			if (qAbs(val - (cu->getOutSkillLevel(sk) - 1)) < 0.001) {
				continue;
			}
		}
 		if (!res.contains(sk)) {
			res[sk] = 0;
		}
		res[sk] = qMax(res[sk], progress[cu->objectName()][sk]);
	}

}

void StudentProgress::addMessage(QString cu, QString author, QString text) {
	messages[cu].push_back({author, text});
}

QVector<message>& StudentProgress::getMessages(QString cu) {
	return messages[cu];
}

void StudentProgress::clearProgress() {
	progress.clear();
}
