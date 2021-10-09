TARGET = Cognitia-II-tests

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += StructuresTests.cpp \
           ../App/Structures/CourseUnit/courseunit.cpp \
           ../App/Core/logger.cpp \
           ../App/Structures/StudentProgress/StudentProgress.cpp \
           
HEADERS = ../App/Structures/CourseUnit/courseunit.h \
    		../App/Core/logger.h \
			../App/Structures/StudentProgress/StudentProgress.h
QT += testlib

