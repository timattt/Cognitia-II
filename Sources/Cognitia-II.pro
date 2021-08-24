QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CourseEditor/courseeditor.cpp \
    CourseUnitViewer/courseunitviewer.cpp \
    CourseUnitViewer/courseunitwidget.cpp \
    Launcher/launcher.cpp \
    Core/main.cpp \
    SkillPackEditor/skillpackeditor.cpp \
    Structures/SkillPack/skill.cpp \
    Structures/SkillPack/skillpack.cpp \
    CourseUnitViewer/Node/node.cpp \
    CourseUnitViewer/Node/edge.cpp

HEADERS += \
    CourseEditor/courseeditor.h \
    CourseUnitViewer/courseunitviewer.h \
    CourseUnitViewer/courseunitwidget.h \
    Launcher/launcher.h \
    SkillPackEditor/skillpackeditor.h \
    Structures/SkillPack/skill.h \
    Structures/SkillPack/skillpack.h \
    CourseUnitViewer/Node/node.h \
    CourseUnitViewer/Node/edge.h

FORMS += \
    CourseEditor/courseeditor.ui \
    CourseUnitViewer/courseunitviewer.ui \
    CourseUnitViewer/courseunitwidget.ui \
    Launcher/launcher.ui \
    SkillPackEditor/skillpackeditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources/Resources.qrc
