#include "Launcher/launcher.h"
#include "../Structures/SkillPack/skill.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../SkillPackEditor/skillpackeditor.h"
#include "../CourseEditor/courseeditor.h"

#include <QApplication>
#include <QObject>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Launcher launcher;
    SkillPackEditor skillPackEditor;
    CourseEditor courseEditor;

    // Start other widgets
    QObject::connect(&launcher, SIGNAL(startSkillPackEditor()), &skillPackEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startCourseEditor()), &courseEditor, SLOT(show()));

    // Return from skillPackEditor to launcher
    QObject::connect(&skillPackEditor, SIGNAL(onClose()), &skillPackEditor, SLOT(hide()));
    QObject::connect(&skillPackEditor, SIGNAL(onClose()), &launcher, SLOT(show()));

    // Return from courseEditor to launcher
    QObject::connect(&courseEditor, SIGNAL(onClose()), &courseEditor, SLOT(hide()));
    QObject::connect(&courseEditor, SIGNAL(onClose()), &launcher, SLOT(show()));

    return a.exec();
}
