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
	/**
	 * Simple init constructor.
	 * @author timattt
	 */
    SkillPackEditor();
    /**
     * Constructor delete every field.
     * @author timattt
     */
    ~SkillPackEditor();
    //--------------------------------

private slots:

    // Slots
    //--------------------------------
	///////////////////////add and remove
	/**
	 * If canEdit() function is ok,
	 * then adds new skill.
	 * @author timattt
	 */
    void on_AddSkill_clicked();
	/**
	 * If canEdit() function is ok,
	 * then adds new level to currently selected skill.
	 * @author timattt
	 */
    void on_AddLevel_clicked();
    /**
     * If canEdit() function is ok,
     * then remove selected.
     * @author timattt
     */
    void on_remove_clicked();

    ///////////////////////open/create/close/save
    /**
     * Ensures that file is nullptr.
     * Then create new file.
     * And set new skillpack.
     * @author timattt
     */
    void on_actionCreate_triggered();
    /**
     * Ensures that file is nullptr.
     * Then opens file.
     * And loads new skillpack.
     * @author timattt
     */
    void on_actionOpen_triggered();
    /**
     * If skillpack is available then calls fromGui function.
     * @author timattt
     */
    void on_actionSave_triggered();
    /**
     * Ensures that skillpack is deleted.
     */
    void on_actionClose_triggered();

    ///////////////////////tools
    /**
     * Shows help from SmartHelper
     * @author timattt
     */
    void on_actionHelp_me_triggered();
    /**
     * Calls close action.
     * Emits signal onClose.
     * @author timattt
     */
    void on_actionReturn_to_launcher_triggered();
    /**
     * Calls dialogbox to choose style CSS.
     * @author timattt
     */
    void on_actionSet_style_triggered();

    ///////////////////////autosave
    /**
     * Shows message.
     * @author timattt
     */
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
    /**
     * File that contains current skillpack.
     * @author timattt
     */
    QFile * skillPackFile;
    /**
     * Model for skills
     * @author timattt
     */
    QStandardItemModel * model;
    /**
     * Timer to update autosave.
     * @author timattt
     */
    int timerId;
    /**
     * This string represents data that lies currently in file in filesystem.
     * @author timattt
     */
    QString fileSignature;
    //--------------------------------

private:

    // Methods
    //--------------------------------
    ////////////////////////load and save
    /**
     * Saves data from gui into skillPackFile.
     * @throws QString if something went wrong
     * @author timattt
     */
    void fromGui();
    /**
     * Loads data to gui from skillPackFile.
     * @throws QString if something went wrong
     * @author timattt
     */
    void toGui();
    /**
     * Transfers data to skillpack from gui.
     * @param skp - destination
     * @author timattt
     */
    void transferToSkillPackStructure(SkillPack * skp);
    /**
     * Transfers data from skillpack to gui.
     * @param skp - soure
     * @author timattt
     */
    void transferFromSkillPackStructure(SkillPack * skp);

    ////////////////////////manipulating with file
    /**
     * Sets new skillpack file path.
     * @author timattt
     */
    void setSkillPack(QString path, bool mayExist);
    /**
     * Checks if current skillpack can be edited.
     * @returns False - if not available. True if available.
     * @author timattt
     */
    bool checkSkillPackAvailable();
    /**
     * If skillpack is available then deletes it.
     * And clears all gui.
     * @author timattt
     */
    void ensureSkillPackDeleted();
    /**
     * Checks if current gui skp data changed in compare to skp field.
     * @returns True - if gui is not equals to field.
     * @author timattt
     */
    bool isChanged();

    ////////////////////////other
    /**
     * Gives item level:
     * skill - 1.
     * level - 2.
     * description - 3.
     * WTF - -1.
     * @param ind - model index to test
     * @author timattt
     */
    int getTreeItemLevel(QModelIndex ind);
    //--------------------------------

protected:

    // protected functions
    //--------------------------------
    /**
     * Updates autosave.
     * @author timattt
     */
    void timerEvent(QTimerEvent *event) override;
    //--------------------------------

};

#endif // SKILLPACKEDITOR_H
