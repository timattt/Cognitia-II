#ifndef SKILLSMIXERHOLDER_H
#define SKILLSMIXERHOLDER_H

#include <QWidget>
#include <QMap>

// Used classes
//====================================
class Node;
class SkillsMixer;
//====================================

namespace Ui {
class SkillsMixerHolder;
}


/**
 * This class is used to create mixers for skills in node you want to make progress in.
 * Promote this widget into your editor, connect slots and signals and that all.
 *
 * @author timattt
 */
class SkillsMixerHolder : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//====================================
	/**
	 * Simple init constructor.
	 * @author timattt
	 */
    SkillsMixerHolder();
    /**
     * Classical widget destructor.
     * @author timattt
     */
    ~SkillsMixerHolder();
    //====================================

private:

    // private fields
    //====================================
    Ui::SkillsMixerHolder *ui;
    /**
     * This is mixers library.
     * {QString : skillname} -> {SkillMixer object for this skill}
     * @author timattt
     */
    QMap<QString, SkillsMixer*> mixers;
    //====================================

public slots:

	// public slots
	//====================================
	/**
	 * This may be called when node may be set to this mixer holder.
	 * @param nd - node to set
	 * @author timattt
	 */
	void nodeSelected(Node * nd);
    /**
     * This may be called when progress is made for node that is represented by this widget.
     * @param name - name of skill where progress made
     * @param v - new skill level progress
     * @author timattt
     */
    void progressMade(QString name, double v);
    //====================================

signals:

	// signals
	//====================================
	/**
	 * This will be emited when user draged skill and changed progress.
	 * So it may be connected to course unit viewer to update data there.
	 * @param skill - skill that is changed.
	 * @param val - new skill progress.
	 * @author timattt
	 */
	void skillLevelChanged(QString skillName, double val);
	//====================================

};

#endif // SKILLSMIXERHOLDER_H
