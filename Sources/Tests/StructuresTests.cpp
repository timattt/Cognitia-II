#include <QtTest/QtTest>
#include "../App/Structures/CourseUnit/courseunit.h"
#include "../App/Structures/StudentProgress/StudentProgress.h"

class Test_Structures : public QObject{
    Q_OBJECT
private slots:
    void saveAndLoadCourseUnit();
    void saveAndLoadStudentProgress();
};


void Test_Structures::saveAndLoadCourseUnit(){

    QVector<CourseUnit*> course;

    for(int i = 0; i < 8; i++){
    	CourseUnit* courseunit = new CourseUnit(10,10,20,20);
        course.push_back(courseunit);

        course[i] -> addIncome("skill", i);
        course[i] -> addIncome("skill", i+8);
        course[i] -> addOutcome("skill", i*2);
        course[i] -> addOutcome("skill", i*2+8);

        course[i] -> setDescription("Description!");
        QString name = "obj";
        course[i] -> setObjectName(name.append(static_cast<char>(i+48)));

    }

    course[0] -> addConnection(course[4]);
    course[0] -> addConnection(course[7]);
    course[1] -> addConnection(course[2]);
    course[2] -> addConnection(course[3]);

    course[0] -> addEmbedded(course[1]);
    course[0] -> addEmbedded(course[2]);
    course[0] -> addEmbedded(course[3]);

    course[4] -> addEmbedded(course[5]);
    course[5] -> addEmbedded(course[6]);

    CourseUnit courseunit;
    courseunit.addEmbedded(course[0]);
    courseunit.addEmbedded(course[4]);
    courseunit.addEmbedded(course[7]);

    QString name = "obj.cu";
    QFile filename(name);
    courseunit.saveCourseUnit(&filename);

    CourseUnit newcourseunit;
    newcourseunit.loadCourseUnit(&filename);
    qDebug() << "загрузил";
    QString u = courseunit.print();
    qDebug() << "первую строчку создал";
    QString uu = newcourseunit.print();
    QCOMPARE(u, uu);
}

void Test_Structures::saveAndLoadStudentProgress() {
	StudentProgress sp;

	sp.setObjectName("test");

	sp.addProgress("CourseUnit1", "Skill1", 0.4);
	sp.addProgress("CourseUnit2", "Skill2", 1.4);
	sp.addProgress("CourseUnit3", "Skill3", 2.4);
	sp.addProgress("CourseUnit4", "Skill4", 3.4);
	sp.addProgress("CourseUnit5", "Skill5", 4.4);

	sp.addMessage("CourseUnit1", "someone", "dfofodnddasdrt");
	sp.addMessage("CourseUnit2", "some356one", "dfofodnddasddfghrt");
	sp.addMessage("CourseUnit3", "someo2345ne", "dfofodnddfghdasdrt");
	sp.addMessage("CourseUnit4", "someo2345ne", "dfosdfghfodnddasdrt");

	QFile fl = QFile("test.cognitiaStudentProgress");

	sp.save(&fl);

	StudentProgress sp1;

	sp1.load(&fl);

    QCOMPARE(sp.toString(), sp1.toString());
}

QTEST_MAIN(Test_Structures)
#include "StructuresTests.moc"
