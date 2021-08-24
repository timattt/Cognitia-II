#include "Launcher/launcher.h"
#include "../Structures/SkillPack/skill.h"
#include "../Structures/SkillPack/skillpack.h"
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

    QObject::connect(&launcher, SIGNAL(startSkillPackEditor()), &skillPackEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startCourseEditor()), &courseEditor, SLOT(show()));

    QObject::connect(&skillPackEditor, SIGNAL(onClose()), &skillPackEditor, SLOT(hide()));

    return a.exec();
}
