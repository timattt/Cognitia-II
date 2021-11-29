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
    CourseEditor/baseinfoandskills.cpp \
    CourseEditor/courseeditor.cpp \
    CourseEditor/courseunitdescription.cpp \
    CourseEditor/skillschooser.cpp \
    CourseUnitViewer/Node/Design/nodedesignshape.cpp \
    CourseUnitViewer/courseunitviewer.cpp \
    CourseUnitViewer/courseunitviewersettings.cpp \
    Help/smarthelper.cpp \
    Launcher/launcher.cpp \
    Core/main.cpp \
    Core/logger.cpp \
    MentorClient/StudentsChooserPanel/studentschooserpanel.cpp \
    MentorClient/mentorclient.cpp \
    SkillPackEditor/skillpackeditor.cpp \
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
    StudentClient/InputAbsoluteSkillsPanel/circle.cpp \
    StudentClient/InputAbsoluteSkillsPanel/circlescene.cpp \
    StudentClient/OutputRelativeSkillsPanel/outcircle.cpp \
    StudentClient/OutputRelativeSkillsPanel/outcirclescene.cpp \
    StudentClient/StudentSkillsPanel/studentskillspanel.cpp \
    StudentClient/StudentSkillsPanel/skillwidget.cpp \
    StudentClient/CourseUnitDetailsPanel/courseunitdetailspanel.cpp \
    StudentClient/OutputRelativeSkillsPanel/outputrelativeskillspanel.cpp \
    StudentClient/InputAbsoluteSkillsPanel/inputabsoluteskillspanel.cpp \
    StudentClient/studentclient.cpp \
    CourseUnitViewer/Node/Design/nodedesign.cpp \
    CourseUnitViewer/Node/Design/nodedesignformal.cpp \
    CourseUnitViewer/Node/Design/nodedesignold.cpp \
    CourseUnitViewer/Node/Design/nodedesignolive.cpp \
    CourseUnitViewer/Label/Label.cpp \
    CourseUnitViewer/Label/LabelBonus.cpp \
    CourseUnitViewer/Label/LabelInvalid.cpp \
    CourseUnitViewer/Label/LabelHidden.cpp
    

HEADERS += \
    ChooseServ/chooseserv.h \
    ClientChat/clientchat.h \
    CourseEditor/baseinfoandskills.h \
    CourseEditor/courseunitdescription.h \
    CourseEditor/skillschooser.h \
    CourseUnitViewer/viewport.h \
    CourseEditor/courseeditor.h \
    CourseUnitViewer/Node/Design/nodedesignshape.h \
    CourseUnitViewer/courseunitviewer.h \
    CourseUnitViewer/courseunitviewersettings.h \
    Help/smarthelper.h \
    Launcher/launcher.h \
    Core/logger.h \
    MentorClient/StudentsChooserPanel/studentschooserpanel.h \
    MentorClient/mentorclient.h \
    SkillPackEditor/skillpackeditor.h \
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
    StudentClient/InputAbsoluteSkillsPanel/circle.h \
    StudentClient/InputAbsoluteSkillsPanel/circlescene.h \
    StudentClient/OutputRelativeSkillsPanel/outcircle.h \
    StudentClient/OutputRelativeSkillsPanel/outcirclescene.h \
    StudentClient/StudentSkillsPanel/studentskillspanel.h \
    StudentClient/StudentSkillsPanel/skillwidget.h \
    StudentClient/CourseUnitDetailsPanel/courseunitdetailspanel.h \
    StudentClient/OutputRelativeSkillsPanel/outputrelativeskillspanel.h \
    StudentClient/InputAbsoluteSkillsPanel/inputabsoluteskillspanel.h \
    StudentClient/studentclient.h \
    CourseUnitViewer/Node/Design/nodedesign.h \
    CourseUnitViewer/Node/Design/nodedesignolive.h \
    CourseUnitViewer/Node/Design/nodedesignold.h \
    CourseUnitViewer/Node/Design/nodedesignformal.h \
    CourseUnitViewer/Label/Label.h \
    CourseUnitViewer/Label/LabelBonus.h \
    CourseUnitViewer/Label/LabelInvalid.h \
    CourseUnitViewer/Label/LabelHidden.h

FORMS += \
    ChooseServ/chooseserv.ui \
    ClientChat/clientchat.ui \
    CourseEditor/baseinfoandskills.ui \
    CourseEditor/courseeditor.ui \
    CourseEditor/courseunitdescription.ui \
    CourseEditor/skillschooser.ui \
    CourseUnitViewer/courseunitviewer.ui \
    CourseUnitViewer/courseunitviewersettings.ui \
    Help/smarthelper.ui \
    Launcher/launcher.ui \
    MentorClient/mentorclient.ui \
    SkillPackEditor/skillpackeditor.ui\
    Server/server.ui \
    SkillsMixer/skillsmixer.ui \
    SkillsMixer/skillsmixerholder.ui \
    StudentClient/StudentSkillsPanel/skillwidget.ui \
    StudentClient/CourseUnitDetailsPanel/courseunitdetailspanel.ui \
    StudentClient/OutputRelativeSkillsPanel/outputrelativeskillspanel.ui \
    StudentClient/InputAbsoluteSkillsPanel/inputabsoluteskillspanel.ui \
    StudentClient/studentclient.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resources/Resources.qrc
