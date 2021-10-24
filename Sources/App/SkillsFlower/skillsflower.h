#ifndef SKILLSFLOWER_H
#define SKILLSFLOWER_H

#include <QWidget>
#include <QMap>

namespace Ui {
class SkillsFlower;
}

// Used classes
//====================================
class CourseUnit;
class FlowerScene;
class StudentProgress;
class Node;
class Leaf;
//====================================

/**
 * This class is used to display student progress in single node or in whole course.
 * You can add this widget to your gui and then connect its signals with course unit viewer.
 * Or with other editors.
 *
 * In this flower user can move leafs to change progress. Or you can just view progress.
 *
 * @Deprecated
 *
 * @author timattt
 */
class SkillsFlower : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//====================================================
	/**
	 * Classical widget constructor.
	 * @author timattt
	 */
    SkillsFlower(QWidget *parent);
	/**
	 * Classical widget destructor.
	 * @author timattt
	 */
    ~SkillsFlower();
    //====================================================

private:

    // Fields
    //====================================================
    /**
     * Basic ui field.
     */
    Ui::SkillsFlower *ui;
    /*
     * Scene for this flower.
     * @author timattt
     */
    FlowerScene * scene;
    /**
     * Checks if this flower can be edited (leafs can be dragged)
     * @author timattt
     */
    bool editable;
    /**
     * Leafs library.
     * {QString : leaf or skill name} -> {Leaf widget object}
     * @author timattt
     */
    QMap<QString, Leaf*> leafs;
    //====================================================

public slots:

    // public slots
    //====================================================
    /**
     * Unpacks to flower all course units for given. Not only itself but children and their children too.
     * If arguments are nullptr then it will only clears everything.
     * @param cu - course unit and its children will be placed on this flower. Can be nullptr.
     * @param prg - progress for course units. Can be nullptr.
     * @author timattt
     */
    void unpackEmbed(CourseUnit * cu, StudentProgress * prg);
    /**
     * Unpacks to this gui single node.
     * If argument are nullptr then it will only clears everything.
     * @param nd - node that will be unpacked. Can be nullptr.
     * @author timattt
     */
    void unpack(Node * nd);
    /**
     * Packs this flower into given course unit and student progress structure.
     * @param cu - object where skills from this flower will be stored.
     * @param prg - object where skills progresses will be stored.
     * @author timattt
     */
    void pack(CourseUnit * cu, StudentProgress * prg) const;
    /**
     * Clears everything from this widget.
     * @author timattt
     */
    void clearAll();
    /**
     * Sets this widget editable. If its true user can drag leafs to change student progress in several skills.
     * @param v - editable or not
     * @author timattt
     */
    void setEditable(bool v);
    /**
     * Checks if this widget is editable. So user can or can not drag leafs to change progress.
     * @returns true is flowers leafs can be dragged.
     * @author timattt
     */
    bool isEditable() const;
    /**
     * This may be called when progress of current course unit is changed.
     * @param skill - skill that is changed.
     * @param lev - new skill progress.
     * @author timattt
     */
    void progressMade(QString skill, double lev);
    //====================================================

signals:

	// signals
	//====================================================
	/**
	 * This will be emited when user draged skill and changed progress.
	 * So it may be connected to course unit viewer to update data there.
	 * @param skill - skill that is changed.
     * @param val - new skill progress.
	 * @author timattt
	 */
	void skillLevelChanged(QString skillName, double val);
	//====================================================

protected:

	// protected functions
	//====================================================
	/**
	 * Reiplemented from QWidget.
	 * It is used to fit scene into widget after resize.
	 * @author timattt
	 */
	void resizeEvent(QResizeEvent *event) override;
	//====================================================

};

#endif // SKILLSFLOWER_H
