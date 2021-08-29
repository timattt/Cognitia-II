#ifndef SKILLPACKEDITOR_H
#define SKILLPACKEDITOR_H

#include <QtWidgets>

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
    void on_AddSkill_clicked();
    void on_AddLevel_clicked();
    void on_remove_clicked();
    void on_actionCreate_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionClose_triggered();
    void on_actionHelp_me_triggered();
    void on_actionReturn_to_launcher_triggered();
    void on_actionSet_style_triggered();
    void on_autoSave_stateChanged(int v);
    //--------------------------------

signals:

    // Signals
    //--------------------------------
    void onClose();
    //--------------------------------

private:

    // Fields
    //--------------------------------
    Ui::SkillPackEditor *ui;
    QFile * skillPackFile;
    QStandardItemModel * model;
    QItemSelectionModel * skillsSelection;
    QItemSelectionModel * levelsSelection;
    int timerId;
    //--------------------------------

private:

    // Methods
    //--------------------------------
    void fromGui();
    void toGui();
    void transferToSkillPackStructure(SkillPack * skp);
    void transferFromSkillPackStructure(SkillPack * skp);
    int getTreeItemLevel(QModelIndex ind);
    void setSkillPackFile(QString path, bool mayExist);
    bool ensureFileIsValid();
    void ensureFileIsNull();
    //--------------------------------

protected:

    // protected functions
    //--------------------------------
    void timerEvent(QTimerEvent *event) override;
    //--------------------------------

};

#endif // SKILLPACKEDITOR_H
