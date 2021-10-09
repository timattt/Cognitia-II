/*
 * Logger.h
 *
 *  Created on: Sep 25, 2021
 *      Author: timat
 */

#ifndef APP_CORE_LOGGER_H_
#define APP_CORE_LOGGER_H_
#include <QtWidgets>

#define ASSERT(COND) log_assert(COND, "", __FILE__, __LINE__);
#define ASSERTM(COND, MESSAGE) log_assert(COND, MESSAGE, __FILE__, __LINE__);
#define NOT_NULL(PTR) log_notnull(PTR, QString("Pointer ") + QString(#PTR) + QString(" is null!"), __FILE__, __LINE__);
#define SAY(MESSAGE) log_say(MESSAGE, __FILE__, __LINE__);

void log_init();
void log_cleanup();
void log_say(QString mes, const char * file, int line);
void log_assert(bool cond, QString errorMessage, const char * file, int line);
void log_notnull(void * ptr, QString errorMes, const char * file, int line);

#endif /* APP_CORE_LOGGER_H_ */
