#ifndef NODESCONVERTER_H
#define NODESCONVERTER_H

#include <QtWidgets>

class CourseUnit;
class Node;
class StudentProgress;

class NodesConverter
{

	static void convertCourseUnitsTreeToNodesTree(CourseUnit * cuHead, Node *& nodesHead);
	static void convertNodesTreeToCourseUnitsTree(Node * nodesHead, CourseUnit *& cuHead);

	static void setStudentProgress(Node * nodesHead, StudentProgress * prog);
	static void extractStudentProgress(Node * nodeHead, StudentProgress * destProg);

};

#endif // NODESCONVERTER_H
