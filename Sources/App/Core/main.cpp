#include "Launcher/launcher.h"
#include "../SkillPackEditor/skillpackeditor.h"
#include "../CourseEditor/courseeditor.h"
#include "../StudentClient/studentclient.h"
#include "../Server/server.h"
#include "../MentorClient/mentorclient.h"
#include "logger.h"

#include <QApplication>
#include <QObject>

#define RETURNFROM(x) \
    QObject::connect(&x, SIGNAL(onClose()), &x, SLOT(hide())); \
    QObject::connect(&x, SIGNAL(onClose()), &launcher, SLOT(show()));

int main(int argc, char *argv[])
{
	log_init();

    try{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/logos/Logos/Icon.png"));

    Launcher launcher;
    SkillPackEditor skillPackEditor;
    CourseEditor courseEditor;
    StudentClient studentClient;
    Server server;
    MentorClient mentorClient;

    // Start other widgets
    QObject::connect(&launcher, SIGNAL(startSkillPackEditor()), &skillPackEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startCourseEditor()), &courseEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startStudentClient()), &studentClient, SLOT(onStart()));
    QObject::connect(&launcher, SIGNAL(startServer()), &server, SLOT(onStart()));
    QObject::connect(&launcher, SIGNAL(startMentorClient()), &mentorClient, SLOT(onStart()));

    RETURNFROM(skillPackEditor)
    RETURNFROM(courseEditor)
    RETURNFROM(server)
    RETURNFROM(studentClient)
	RETURNFROM(mentorClient)

             return a.exec();
    }
    catch (const QString mess){
        SAY(mess);
    }
}
