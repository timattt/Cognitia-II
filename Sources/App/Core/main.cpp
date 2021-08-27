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
	/*
	QApplication a(argc, argv);

	QWidget window;

	QLabel* title = new QLabel("Custom widgets on a QListWidget");
	title->setAlignment(Qt::AlignHCenter);

	QListWidget* list = new QListWidget;
	list->addItem("foo");

	QListWidgetItem* item;
	item = new QListWidgetItem(list);
	list->addItem(item);
	QPushButton* button = new QPushButton("hey");
	item->setSizeHint(button->minimumSizeHint());
	list->setItemWidget(item, button);

	list->addItem("bar");

	QVBoxLayout* layout = new QVBoxLayout(&window);
	layout->addWidget(title);
	layout->addWidget(list);
	window.setLayout(layout);

	window.show();

	return a.exec();
	*/

    QApplication a(argc, argv);

    Launcher launcher;
    SkillPackEditor skillPackEditor;
    CourseEditor courseEditor;

    QObject::connect(&launcher, SIGNAL(startSkillPackEditor()), &skillPackEditor, SLOT(show()));
    QObject::connect(&launcher, SIGNAL(startCourseEditor()), &courseEditor, SLOT(show()));

    QObject::connect(&skillPackEditor, SIGNAL(onClose()), &skillPackEditor, SLOT(hide()));

    return a.exec();
}
