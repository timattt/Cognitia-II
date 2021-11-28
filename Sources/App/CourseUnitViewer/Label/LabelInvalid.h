/*
 * LabelInvalid.h
 *
 *  Created on: Nov 28, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_LABEL_LABELINVALID_H_
#define APP_COURSEUNITVIEWER_LABEL_LABELINVALID_H_

#include <QtWidgets>
#include "Label.h"

class LabelInvalid: public Label {
public:
	LabelInvalid(QObject * parent);
	~LabelInvalid();
	void update(Node *node) override;
};

#endif /* APP_COURSEUNITVIEWER_LABEL_LABELINVALID_H_ */
