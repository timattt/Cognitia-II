#ifndef SKILLSMIXER_H
#define SKILLSMIXER_H

#include <QWidget>

namespace Ui {
class SkillsMixer;
}

// Used classes
//====================================
class Node;
class SkillsMixerHolder;
//====================================

/**
 * This class is used by SkillsMixerHolder.
 * It represents one skill and its progress.
 * It may not be used outside skillsmixerholder.
 *
 * @author timattt
 */
class SkillsMixer : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//====================================
	/**
	 * Inits slider with given from and to bounds.
	 * Sets name to label.
	 * @param parent - holder object
	 * @param from - start skill level bound.
	 * @param to - end skill level bound.
	 * @param val - current level value.
	 * @param name - skill name.
	 * @author timattt
	 */
    SkillsMixer(SkillsMixerHolder *parent, double from, double to, double val, QString name);
    /**
     * Classical widget destructor.
     */
    ~SkillsMixer();
    //====================================

private:

    // private fields
    //====================================
    /**
     * Parent object.
     * @author timattt
     */
    SkillsMixerHolder * holder;
    Ui::SkillsMixer *ui;
    //====================================

private slots:

	// private slots
	//====================================
	void on_horizontalSlider_valueChanged(int a);
	//====================================

public:

	// public functions
	//====================================
	/**
	 * Sets new skill level progress value.
	 * @param v - level value in bounds [from, to].
	 * @author timattt
	 */
	void setValue(double v);
	//====================================

};

#endif // SKILLSMIXER_H
