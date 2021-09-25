/*
 * Logger.cpp
 *
 *  Created on: Sep 25, 2021
 *      Author: timat
 */

#include "logger.h"

// private fields
//==================================================================
bool initialized = false;
QFile logFile("CognitiaLog.txt");
QTextStream stream(&logFile);
//==================================================================


// private functions
//==================================================================
QString formHeader(const char *file, int line) {
	 QString res = "[" + QDateTime::currentDateTime().toString() + "]";
	 res += QString("[") + QString(file) + ":" + QString::number(line) + "] ";
	 return res;
}
//==================================================================


// global functions
//==================================================================
void log_init() {
	logFile.remove();

	logFile.open(QIODevice::WriteOnly);
}

void log_say(QString mes, const char *file, int line) {
	QString m = formHeader(file, line) + (mes);

	stream << m << '\n';
	qInfo() << m;

	stream.flush();
}

void log_assert(bool cond, QString errorMessage, const char *file, int line) {
	if (!cond) {
		QString m;
		if (errorMessage.size() > 0) {
			m = (formHeader(file, line)) + ("Assertion error with message: {") + (errorMessage) + ("}");
		} else {
			m = (formHeader(file, line)) + ("Assertion error");
		}

		stream << m << '\n';
		qInfo() << m;

		stream.flush();

		exit(-1);
	}
}

void log_cleanup() {
	logFile.close();
}

void log_notnull(void *ptr, QString errorMessage, const char *file, int line) {
	if (ptr == nullptr) {
		QString m = (formHeader(file, line)) + (errorMessage);

		stream << m << '\n';
		qInfo() << m;

		stream.flush();

		exit(-1);
	}
}
//==================================================================
