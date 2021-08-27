/*
 * SkillsModel.cpp
 *
 *  Created on: Aug 27, 2021
 *      Author: timat
 */

#include "skillsmodel.h"
#include "../Structures/SkillPack/skillpack.h"

SkillsModel::SkillsModel(QWidget * p) : QStandardItemModel(0, 2, p) {
	setHorizontalHeaderLabels(QStringList() << "Skill" << "Level");
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

	int r = this->rowCount();
	this->insertRow(r);

	this->setData(this->index(r, 0), name);
	this->setData(this->index(r, 1), QString::number(lev));
	return true;
}
