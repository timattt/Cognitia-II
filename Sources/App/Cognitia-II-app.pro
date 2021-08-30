TARGET = Cognitia-II

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

OUTPUT += Console
CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChooseServ/chooseserv.cpp \
    CourseEditor/courseeditor.cpp \
    CourseUnitViewer/courseunitviewer.cpp \
    Help/smarthelper.cpp \
    Launcher/launcher.cpp \
    Core/main.cpp \
    SkillPackEditor/skillpackeditor.cpp \
    Structures/CourseUnit/courseunit.cpp \
    Structures/SkillPack/skill.cpp \
    Structures/SkillPack/skillpack.cpp \
    CourseUnitViewer/Node/node.cpp \
    CourseUnitViewer/Node/edge.cpp \
    Structures/StudentProgress/StudentProgress.cpp \
    CourseUnitViewer/coursescene.cpp \
    CourseEditor/skillsmodel.cpp \
    Server/server.cpp \
    StudentClient/studentclient.cpp


HEADERS += \
    ChooseServ/chooseserv.h \
    CourseEditor/courseeditor.h \
    CourseUnitViewer/courseunitviewer.h \
    Help/smarthelper.h \
    Launcher/launcher.h \
    SkillPackEditor/skillpackeditor.h \
    Structures/CourseUnit/courseunit.h \
    Structures/SkillPack/skill.h \
    Structures/SkillPack/skillpack.h \
    CourseUnitViewer/Node/node.h \
    CourseUnitViewer/Node/edge.h \
    CourseUnitViewer/coursescene.h \
    Structures/StudentProgress/StudentProgress.h \
    CourseEditor/skillsmodel.h\
    Server/server.h \
    StudentClient/studentclient.h

FORMS += \
    ChooseServ/chooseserv.ui \
    CourseEditor/courseeditor.ui \
    CourseUnitViewer/courseunitviewer.ui \
    Help/smarthelper.ui \
    Launcher/launcher.ui \
    SkillPackEditor/skillpackeditor.ui\
    Server/server.ui \
    StudentClient/studentclient.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources/Resources.qrc