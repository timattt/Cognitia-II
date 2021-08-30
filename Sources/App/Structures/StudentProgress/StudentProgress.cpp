/*
 * StudentProgress.cpp
 *
 *  Created on: Aug 29, 2021
 *      Author: timat
 */

#include "StudentProgress.h"

StudentProgress::StudentProgress() {
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
        throw QString("Can not open skill pack file [" + file->fileName() + "]");
    }

    setObjectName(info.baseName());

    QStringList divs = dat.split(STUDENT_PROGRESS_DELIMITER, Qt::SkipEmptyParts);

    if (divs.empty()) {
        throw QString("Empty skill pack file [" + file->fileName() + "]");
    }

    for (QString raw : divs) {
    	QStringList split = raw.split(STUDENT_PROGRESS_COURSEUNIT_DELIMITER, Qt::SkipEmptyParts);

    	QString courseUnitName = split[0];

    	for (int i = 1; i < split.length(); i+=2) {
    		QString skillName = split[i];
    		double lev = split[i + 1].toDouble();
    		addProgress(courseUnitName, skillName, lev);
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

            for (QString skill : skills.keys()) {
            	double level = skills[skill];

            	stream << skill << STUDENT_PROGRESS_COURSEUNIT_DELIMITER << QString::number(level) << STUDENT_PROGRESS_COURSEUNIT_DELIMITER;
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
	}

	return r;
}
