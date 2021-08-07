#include "Launcher/launcher.h"
#include "../Structures/SkillPack/skill.h"
#include "../Structures/SkillPack/skillpack.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Launcher launcher;
    return a.exec();
}
