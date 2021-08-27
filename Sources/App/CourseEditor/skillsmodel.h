/*
 * SkillsModel.h
 *
 *  Created on: Aug 27, 2021
 *      Author: timat
 */

#ifndef APP_COURSEEDITOR_SKILLSMODEL_H_
#define APP_COURSEEDITOR_SKILLSMODEL_H_

#include <QtWidgets>
#include <qstringlistmodel.h>

class SkillsModel: public QStandardItemModel {

public:

	SkillsModel(QWidget * p);
	~SkillsModel();
	bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row,
			int column, const QModelIndex &parent) const override;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
			int column, const QModelIndex &parent) override;
};

#endif /* APP_COURSEEDITOR_SKILLSMODEL_H_ */
