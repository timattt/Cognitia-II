#ifndef SKILLPACKEDITOR_H
#define SKILLPACKEDITOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QModelIndex>
#include <QItemSelectionModel>

#include "../Structures/SkillPack/skillpack.h"

namespace Ui {
class SkillPackEditor;
}

class SkillPackEditor : public QMainWindow
{
    Q_OBJECT

public:

    // Constructor
    //--------------------------------
    SkillPackEditor();
    ~SkillPackEditor();
    //--------------------------------

private slots:

    // Slots
    //--------------------------------
    void on_actionCreate_triggered();
    void on_AddSkill_clicked();
    void on_AddLevel_clicked();
    void on_levelsList_clicked(const QModelIndex &index);
    void on_descrEdit_textChanged();
    void on_RemoveSkill_clicked();
    void on_RemoveLevel_clicked();
    //--------------------------------

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

private:

    // Fields
    //--------------------------------
    Ui::SkillPackEditor *ui;
    QFile * skillPackFile;
    QStandardItemModel * model;
    QItemSelectionModel * skillsSelection;
    QItemSelectionModel * levelsSelection;
    //--------------------------------

private:

    // Methods
    //--------------------------------
    void fromGui();
    void toGui();
    void transferToSkillPackStructure(SkillPack * skp);
    void transferFromSkillPackStructure(SkillPack * skp);
    //--------------------------------
};

#endif // SKILLPACKEDITOR_H
