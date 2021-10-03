/*
 * SkillsModel.h
 *
 *  Created on: Aug 27, 2021
 *      Author: timat
 */

#ifndef APP_COURSEEDITOR_SKILLSMODEL_H_
#define APP_COURSEEDITOR_SKILLSMODEL_H_

#include <QtWidgets>

// Used classes
//====================================
class CourseEditor;
//====================================

/**
 * This class reimplements QStandardItemModel and is used to implement drag and drop skills to model.
 * @author timattt
 *
 * "Мы отстали от передовых стран на 50-100 лет. Мы должны пробежать это расстояние в десять лет. Либо мы сделаем это, либо нас сомнут." - И.В.Сталин
 */
class SkillsModel: public QStandardItemModel {

public:

	// Constructor and destructor
	//----------------------------------------
	/**
	 * Basic constructor.
	 * @param p - CourseEditor where this model is used.
	 * @param in - true - if in this model will be income skills, false if outcome.
	 * @author timattt
	 */
	SkillsModel(CourseEditor * p, bool in);
	/**
	 * Basic destructor.
	 * @author timattt
	 */
	~SkillsModel() = default;
	//----------------------------------------

public:

	// public functions
	//----------------------------------------
	/**
	 * Reimplemented from QStandardItemModel.
	 * Returns true always.
	 */
	bool canDropMimeData(	const QMimeData *data,
							Qt::DropAction action,
							int row,
							int column,
							const QModelIndex &parent) const override;

	/**
	 * Reimplemented from QStandardItemModel.
	 * When something is dropped in this model this function is used to parse mime data and then add or remove skill.
	 */
	bool dropMimeData(		const QMimeData *data,
							Qt::DropAction action,
							int row,
							int column,
							const QModelIndex &parent) override;

	/**
	 * Adds given skill with level to this model.
	 * @param name - skill name
	 * @param lev - skill level.
	 * @author timattt
	 */
	void addSkill(			QString name,
							int lev);

	/**
	 * Removes skill from this model.
	 * @param name - skill to remove
	 * @author timattt
	 */
	void removeSkill(		QString name);
	//----------------------------------------

private:

	// Fields
	//----------------------------------------
	/**
	 * This is course editor where this model us used.
	 * @author timattt
	 */
	CourseEditor * editor;
	/**
	 * True if this model have income skill of course unit or false it outcome.
	 * @author timattt
	 */
	bool in;
	//----------------------------------------

};

#endif /* APP_COURSEEDITOR_SKILLSMODEL_H_ */
