#include "nodesconverter.h"
#include "../Core/logger.h"
#include "../CourseUnitViewer/Node/node.h"
#include "../Structures/CourseUnit/courseunit.h"
#include "../CourseUnitViewer/Node/edge.h"

void NodesConverter::convertCourseUnitsTreeToNodesTree(CourseUnit *cuHead, Node *&nodesHead) {
	NOT_NULL(cuHead);

	QMap<QString, Node*> nodes;

	for (CourseUnit * u : cuHead->getEmbedded()) {
		if (nodes.contains(u->objectName())) {
			throw QString("Some course units have same name!");
		}

		Node * nd = new Node(nullptr);
		nd->fromCourseUnitToNode(u);
		nodes[nd->getName()] = nd;
	}

	for (CourseUnit * u : cuHead->getEmbedded()) {
		for (QString v : u->getConnections()) {
			new Edge(nodes[u->objectName()], nodes[v], nullptr);
		}
	}
}

void NodesConverter::convertNodesTreeToCourseUnitsTree(Node *nodesHead,
		CourseUnit *&cuHead) {
}

void NodesConverter::setStudentProgress(Node *nodesHead,
		StudentProgress *prog) {
}

void NodesConverter::extractStudentProgress(Node *nodeHead,
		StudentProgress *destProg) {
}
