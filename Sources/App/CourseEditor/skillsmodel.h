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

class CourseEditor;

class SkillsModel: public QStandardItemModel {

public:

	// Constructor and destructor
	//----------------------------------------
	SkillsModel(CourseEditor * p, bool in);
	~SkillsModel();
	//----------------------------------------

public:

	// public functions
	//----------------------------------------
	bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row,
			int column, const QModelIndex &parent) const override;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row,
			int column, const QModelIndex &parent) override;
	void addSkill(QString name, int lev);
	void removeSkill(QString name);
	//----------------------------------------

private:

	// Fields
	//----------------------------------------
	CourseEditor * editor;
	bool in;
	//----------------------------------------

};

#endif /* APP_COURSEEDITOR_SKILLSMODEL_H_ */
