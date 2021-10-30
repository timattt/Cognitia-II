/*
 * StudentProgress.h
 *
 *  Created on: Aug 29, 2021
 *      Author: timat
 */

#ifndef APP_STRUCTURES_STUDENTPROGRESS_H_
#define APP_STRUCTURES_STUDENTPROGRESS_H_

#include <QtWidgets>

#define STUDENT_PROGRESS_DELIMITER "|%|"
#define STUDENT_PROGRESS_COURSEUNIT_DELIMITER "&&&"
#include "../fileExtensions.h"

struct message {
	QString author;
	QString text;
};

class CourseUnit;

class StudentProgress: public QObject {
public:

	// constructor and destructor
	//----------------------------------
    StudentProgress(QObject* parent = nullptr);
	~StudentProgress();
	//----------------------------------

private:

	// fields
	//----------------------------------
	QMap<QString, QMap<QString, double>> progress;
	QMap<QString, QVector<message>> messages;
	//----------------------------------

public:

	// public functions
	//----------------------------------
	void load(QFile * fl);
	void save(QFile * fl);
	void addMessage(QString cu, QString author, QString text);
	QVector<message>& getMessages(QString cu);
	void addProgress(QString courseUnit, QString skill, double level);
	double getLevel(QString courseUnit, QString skill);
	bool containsLevel(QString courseUnit, QString skill);
	QString toString();
	void clearProgress();
	/**
	 * @Deprecated
	 */
	void collectAbsolute(CourseUnit * cu, QMap<QString, double> & res);
	//----------------------------------
};

#endif /* APP_STRUCTURES_STUDENTPROGRESS_H_ */
