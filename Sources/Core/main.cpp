#include "Launcher/launcher.h"
#include "../Structures/SkillPack/skill.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../SkillPackEditor/skillpackeditor.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Launcher launcher;

    SkillPackEditor skillPackEditor;

    QObject::connect(&launcher, SIGNAL(startSkillPackEditor()), &skillPackEditor, SLOT(show()));
    QObject::connect(&skillPackEditor, SIGNAL(onClose()), &launcher, SLOT(show()));
    QObject::connect(&skillPackEditor, SIGNAL(onClose()), &skillPackEditor, SLOT(hide()));

    return a.exec();
}
