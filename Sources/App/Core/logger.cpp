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
	 if (file != nullptr && strlen(file) > 0) {
		 res += QString("[") + QString(file) + ":" + QString::number(line) + "] ";
	 }
	 return res;
}
//==================================================================


// global functions
//==================================================================
void cognitiaMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	switch (type) {
	case QtDebugMsg:
		log_say(localMsg.constData(), context.file, context.line);
		break;
	case QtInfoMsg:
		log_say(localMsg.constData(), context.file, context.line);
		break;
	case QtWarningMsg:
		log_say(localMsg.constData(), context.file, context.line);
		break;
	case QtCriticalMsg:
		log_say(localMsg.constData(), context.file, context.line);
		break;
	case QtFatalMsg:
		log_say(localMsg.constData(), context.file, context.line);
		abort();
	}
}
void log_init() {
	qInstallMessageHandler(cognitiaMessageOutput);
	logFile.remove();

	logFile.open(QIODevice::WriteOnly);
}

void log_say(QString mes, const char *file, int line) {
	QString m = formHeader(file, line) + (mes);

	stream << m << '\n';
	printf("%s\n", m.toStdString().c_str());
	fflush(stdout);

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
