/*
 * Label.h
 *
 *  Created on: Nov 28, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_LABEL_LABEL_H_
#define APP_COURSEUNITVIEWER_LABEL_LABEL_H_

#include <QtWidgets>

// used classes
//=========================================
class Node;
//=========================================

class Label : public QObject {
	Q_OBJECT

public:

	//=========================================
	Label(QString name_, QColor color_, QObject * parent);
	~Label();
	//=========================================

private:

	//=========================================
	QString name;
	QColor color;
	//=========================================

public:

	//=========================================
	virtual void update(Node * node);
	//=========================================

};

#endif /* APP_COURSEUNITVIEWER_LABEL_LABEL_H_ */
