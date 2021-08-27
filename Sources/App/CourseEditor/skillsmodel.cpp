/*
 * SkillsModel.cpp
 *
 *  Created on: Aug 27, 2021
 *      Author: timat
 */

#include "skillsmodel.h"
#include "../Structures/SkillPack/skillpack.h"
#include "../CourseUnitViewer/Node/node.h"
#include "courseeditor.h"

SkillsModel::SkillsModel(CourseEditor * p, bool in) : QStandardItemModel(0, 2, p), editor(p), in(in) {
}

SkillsModel::~SkillsModel() {
}

bool SkillsModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent) const {
	return true;
}

bool SkillsModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent) {

	QStringList divs = data->text().split(SKILL_PACK_DELIMITER);

	bool ok = 0;

	QString name = divs[0];
	int lev = divs[1].toInt(&ok);

	if (!ok) {
		return false;
	}

	Node * n = editor->getCurrent();

	if (in) {
		if (!n->getInSkills().contains(name)) {
			n->addInSkill(name, lev);
		} else {
			return true;
		}
	} else {
		if (!n->getOutSkills().contains(name)) {
			n->addOutSkill(name, lev);
		} else {
			return true;
		}
	}

	addSkill(name, lev);
	return true;
}

void SkillsModel::addSkill(QString name, int lev) {
	int r = this->rowCount();
	this->insertRow(r);

	this->setData(this->index(r, 0), name);
	this->setData(this->index(r, 1), QString::number(lev));
}
