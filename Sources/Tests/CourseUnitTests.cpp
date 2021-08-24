#include <QtTest/QtTest>
#include "../App/Structures/CourseUnit/courseunit.h"

class Test_courseUnit : public QObject{
    Q_OBJECT
private slots:
    void saveAndLoad();
};


void Test_courseUnit::saveAndLoad(){

    QVector<courseUnit*> course;

    for(int i = 0; i < 8; i++){
        courseUnit* courseunit = new courseUnit(10,10,20,20);
        course.push_back(courseunit);

        course[i] -> addIncome(std::make_pair("skill", i));
        course[i] -> addIncome(std::make_pair("skill", i+8));
        course[i] -> addOutcome(std::make_pair("skill", i*2));
        course[i] -> addOutcome(std::make_pair("skill", i*2+8));

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

    courseUnit courseunit;
    courseunit.addEmbedded(course[0]);
    courseunit.addEmbedded(course[4]);
    courseunit.addEmbedded(course[7]);

    QString name = "obj.cu";
    QFile filename(name);
    courseunit.saveCourseUnit(&filename);

    courseUnit newcourseunit;
    newcourseunit.loadCourseUnit(&filename);
    qDebug() << "загрузил";
    QString u = courseunit.print();
    qDebug() << "первую строчку создал";
    QString uu = newcourseunit.print();
    QCOMPARE(u, uu);
}

QTEST_MAIN(Test_courseUnit)
#include "CourseUnitTests.moc"
