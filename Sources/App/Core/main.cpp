#include "Launcher/launcher.h"
#include "../Structures/SkillPack/skill.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../SkillPackEditor/skillpackeditor.h"
#include "../CourseEditor/courseeditor.h"
#include "../StudentClient/studentclient.h"
#include "../Server/server.h"

#include <QApplication>
#include <QObject>

#define RETURNFROM(x) \
    QObject::connect(&x, SIGNAL(onClose()), &x, SLOT(hide())); \
    QObject::connect(&x, SIGNAL(onClose()), &launcher, SLOT(show()));

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Launcher launcher;
    SkillPackEditor skillPackEditor;
    CourseEditor courseEditor;
    StudentClient studentClient;
    Server server;

    // Start other widgets
    QObject::connect(&launcher, SIGNAL(startSkillPackEditor()), &skillPackEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startCourseEditor()), &courseEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startStudentClient()), &studentClient, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startServer()), &server, SLOT(show()));


    RETURNFROM(skillPackEditor)
    RETURNFROM(courseEditor)
    RETURNFROM(server)


    return a.exec();
}
