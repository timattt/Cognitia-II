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
#include "../Core/logger.h"

SkillsModel::SkillsModel(CourseEditor * p, bool in_) : QStandardItemModel(0, 2, p), editor(p), in(in_) {
	NOT_NULL(p);
}

bool SkillsModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent) const {
	Q_UNUSED(data);
	Q_UNUSED(action);
	Q_UNUSED(row);
	Q_UNUSED(column);
	Q_UNUSED(parent);
	return true;
}

bool SkillsModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
		int row, int column, const QModelIndex &parent) {
	Q_UNUSED(action);
	Q_UNUSED(row);
	Q_UNUSED(column);
	Q_UNUSED(parent);
	if (data->text().contains(QString(SKILL_PACK_DELIMITER) + QString(SKILL_PACK_DELIMITER))) {
		QStringList divs = data->text().split(QString(SKILL_PACK_DELIMITER) + QString(SKILL_PACK_DELIMITER));

		bool ok = 0;

		QString name = divs[0];
		divs[1].toInt(&ok);

		if (!ok) {
			return false;
		}

		Node * n = editor->getCurrentNode();

		if (in) {
			if (n->getInSkills().contains(name)) {
				n->removeInSkill(name);
			} else {
				return true;
			}
		} else {
			if (n->getOutSkills().contains(name)) {
				n->removeOutSkill(name);
			} else {
				return true;
			}
		}

		removeSkill(name);
		return true;
	}

	if (data->text().contains(QString(SKILL_PACK_DELIMITER))) {
		QStringList divs = data->text().split(SKILL_PACK_DELIMITER);

		bool ok = 0;

		QString name = divs[0];
		int lev = divs[1].toInt(&ok);

		if (!ok) {
			return false;
		}

		Node *n = editor->getCurrentNode();

		if (in) {
			n->addInSkill(name, lev);
		} else {
			n->addOutSkill(name, lev);
		}

		addSkill(name, lev);
		return true;

	}

	return false;
}

void SkillsModel::addSkill(QString name, int lev) {
	int was = -1;

	for (int i = 0; i < this->rowCount(); i++) {
		if (this->data(this->index(i, 0)) == name) {
			was = i;
		}
	}

	int r = 0;

	if (was == -1) {
		r = this->rowCount();
		this->insertRow(r);
	} else {
		r = was;
	}

	this->setData(this->index(r, 0), name);
	this->setData(this->index(r, 1), QString::number(lev));
}

void SkillsModel::removeSkill(QString name) {
	for (int i = 0; i < this->rowCount(); i++) {
		if (this->data(this->index(i, 0)) == name) {
			this->removeRow(i);
			return;
		}
	}
}
