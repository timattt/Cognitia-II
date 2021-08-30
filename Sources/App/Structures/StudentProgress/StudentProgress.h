/*
 * StudentProgress.h
 *
 *  Created on: Aug 29, 2021
 *      Author: timat
 */

#ifndef APP_STRUCTURES_STUDENTPROGRESS_H_
#define APP_STRUCTURES_STUDENTPROGRESS_H_

#include <qobject.h>

#include <QMap>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#define STUDENT_PROGRESS_DELIMITER "|%|"
#define STUDENT_PROGRESS_COURSEUNIT_DELIMITER "&&&"

class StudentProgress: public QObject {
public:

	// constructor and destructor
	//----------------------------------
	StudentProgress();
	~StudentProgress();
	//----------------------------------

private:

	// fields
	//----------------------------------
	QMap<QString, QMap<QString, double>> progress;
	//----------------------------------

public:

	// public functions
	//----------------------------------
	void load(QFile * fl);
	void save(QFile * fl);
	void addProgress(QString courseUnit, QString skill, double level);
	QString toString();
	//----------------------------------
};

#endif /* APP_STRUCTURES_STUDENTPROGRESS_H_ */
