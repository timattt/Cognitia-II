TARGET = Cognitia-II

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChooseServ/chooseserv.cpp \
    ClientChat/clientchat.cpp \
    CourseEditor/courseeditor.cpp \
    CourseUnitViewer/Node/Design/nodedesignshape.cpp \
    CourseUnitViewer/courseunitviewer.cpp \
    CourseUnitViewer/courseunitviewersettings.cpp \
    Help/smarthelper.cpp \
    Launcher/launcher.cpp \
    Core/main.cpp \
    Core/logger.cpp \
    MentorClient/mentorclient.cpp \
    SkillPackEditor/skillpackeditor.cpp \
    SkillsFlower/skillsflower.cpp \
    SkillsMixer/skillsmixer.cpp \
    SkillsMixer/skillsmixerholder.cpp \
    Structures/CourseUnit/courseunit.cpp \
    Structures/SkillPack/skill.cpp \
    Structures/SkillPack/skillpack.cpp \
    CourseUnitViewer/Node/node.cpp \
    CourseUnitViewer/Node/edge.cpp \
    Structures/StudentProgress/StudentProgress.cpp \
    CourseUnitViewer/coursescene.cpp \
    CourseUnitViewer/viewport.cpp \
    CourseEditor/skillsmodel.cpp \
    Server/server.cpp \
    StudentClient/AllSkillsGui/allskills.cpp \
    StudentClient/AllSkillsGui/skillwidget.cpp \
    StudentClient/CourseUnitInfoPanel/courseunitinfo.cpp \
    StudentClient/CourseUnitInfoPanel/outdeltaskills.cpp \
    StudentClient/InAbsoluteSkillGui/inabsoluteskill.cpp \
    StudentClient/studentclient.cpp \
    CourseUnitViewer/Node/Design/nodedesign.cpp \
    CourseUnitViewer/Node/Design/nodedesignformal.cpp \
    CourseUnitViewer/Node/Design/nodedesignold.cpp \
    SkillsFlower/flowerscene.cpp \
    SkillsFlower/leaf.cpp \
        CourseUnitViewer/Node/Design/nodedesignolive.cpp

HEADERS += \
    ChooseServ/chooseserv.h \
    ClientChat/clientchat.h \
    CourseUnitViewer/viewport.h \
    CourseEditor/courseeditor.h \
    CourseUnitViewer/Node/Design/nodedesignshape.h \
    CourseUnitViewer/courseunitviewer.h \
    CourseUnitViewer/courseunitviewersettings.h \
    Help/smarthelper.h \
    Launcher/launcher.h \
    Core/logger.h \
    MentorClient/mentorclient.h \
    SkillPackEditor/skillpackeditor.h \
    SkillsFlower/skillsflower.h \
    SkillsMixer/skillsmixer.h \
    SkillsMixer/skillsmixerholder.h \
    Structures/CourseUnit/courseunit.h \
    Structures/ServerCommands/serverCommands.h \
    Structures/SkillPack/skill.h \
    Structures/SkillPack/skillpack.h \
    CourseUnitViewer/Node/node.h \
    CourseUnitViewer/Node/edge.h \
    CourseUnitViewer/coursescene.h \
    Structures/StudentProgress/StudentProgress.h \
    CourseEditor/skillsmodel.h\
    Server/server.h \
    Structures/fileExtensions.h \
    StudentClient/AllSkillsGui/allskills.h \
    StudentClient/AllSkillsGui/skillwidget.h \
    StudentClient/CourseUnitInfoPanel/courseunitinfo.h \
    StudentClient/CourseUnitInfoPanel/outdeltaskills.h \
    StudentClient/InAbsoluteSkillGui/inabsoluteskill.h \
    StudentClient/studentclient.h \
    CourseUnitViewer/Node/Design/nodedesign.h \
    CourseUnitViewer/Node/Design/nodedesignolive.h \
    CourseUnitViewer/Node/Design/nodedesignold.h \
    SkillsFlower/flowerscene.h \
    SkillsFlower/leaf.h \
    CourseUnitViewer/Node/Design/nodedesignformal.h

FORMS += \
    ChooseServ/chooseserv.ui \
    ClientChat/clientchat.ui \
    CourseEditor/courseeditor.ui \
    CourseUnitViewer/courseunitviewer.ui \
    CourseUnitViewer/courseunitviewersettings.ui \
    Help/smarthelper.ui \
    Launcher/launcher.ui \
    MentorClient/mentorclient.ui \
    SkillPackEditor/skillpackeditor.ui\
    Server/server.ui \
    SkillsFlower/skillsflower.ui \
    SkillsMixer/skillsmixer.ui \
    SkillsMixer/skillsmixerholder.ui \
    StudentClient/AllSkillsGui/allskills.ui \
    StudentClient/AllSkillsGui/skillwidget.ui \
    StudentClient/CourseUnitInfoPanel/courseunitinfo.ui \
    StudentClient/CourseUnitInfoPanel/outdeltaskills.ui \
    StudentClient/InAbsoluteSkillGui/inabsoluteskill.ui \
    StudentClient/studentclient.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources/Resources.qrc
