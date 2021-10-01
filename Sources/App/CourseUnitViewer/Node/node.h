#ifndef NODE_H
#define NODE_H

#include <QtWidgets>

// Used classes
//====================================
class Edge;
class CourseUnitViewer;
class CourseUnit;
//====================================

// Constants
//====================================
#define DEFAULT_NODE_ATT_FAC 0.01
#define DEFAULT_NODE_REP_FAC 1000
#define DEFAULT_NODE_OWN_LENGTH 600
#define NODE_PHYSICS_EPSILON 0.2
#define SELECTED_NODE_COLOR Qt::green
#define NODE_DEFAULT_NAME "New node"
//====================================

/**
 * This class is implemented from QGraphicsItem and may be used to draw on scene in CourseUnitViewer.
 * It can also be used as a plain container form course unit.
 * On most of action signal will be emited in CourseUnitViewer.
 *
 * @author timattt
 *
 * "Воевать без нефти нельзя, а кто имеет преимущество в деле нефти, тот имеет шансы на победу в грядущей войне" - И.В.Сталин
 */
class Node : public QGraphicsItem
{

public:

    // Constructor
    //--------------------------------------
	/**
	 * Initializes new node. Sets graph field to given value.
	 * Also sets some QT parameters.
	 * @param viewer - viewer that may contains this node. Can be nullptr.
	 * In this case class can be used as container for courseUnit.
	 * @author timattt
	 */
    Node(CourseUnitViewer *viewer);
    /**
     * Deletes this node.
     * Also it calls delete for all edges this node has.
     * Also if scene() function is not nullptr item will be deleted from scene.
     * @author timattt
     */
    ~Node();
    //--------------------------------------

public:

    // public functions
    //--------------------------------------
    /**
     * Connects edge to this node (adds to edgeList).
     * Calls adjust method for edge.
     * @param edge - edge to be added. May not be nullptr.
     * @author timattt
     */
    void addEdge(Edge *edge);
    /**
     * Reimplemented from QGraphicsItem
     */
    int type() const override { return UserType + 1; }
    /**
     * Updates node physics.
     * It will do nothing if:
     * 1. graph field is nullptr.
     * 2. node is not in the scene.
     * 3. node is grabbed.
     * 4. Checkbox freeze in viewer is on.
     * @author timattt
     */
    void calculateForces();
    /**
     * Reimplemented from QGraphicsItem
     */
    QRectF boundingRect() const override;
    /**
     * Reimplemented from QGraphicsItem
     */
    QPainterPath shape() const override;
    /**
     * Reimplemented from QGraphicsItem.
     * Uses graph field and its current design.
     * Draw using currentDesign.
     * @author timattt
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /**
     * Removes edge connection from this node (clears from edgeList)
     * @param e - edge object. May no be nullptr.
     * @author timattt
     */
    void removeEdge(Edge * e);
    /**
     * Checks if this node is connected by edge to given node.
     * @param nd - node to check
     * @return true - if connected to given node
     * @author timattt
     */
    bool hasEdgeToNode(Node * nd) const;
    /**
     * Adds input skill with given level.
     * If graph field is not nullptr then signal will be emited.
     * @param name - skillName
     * @param lev - level of skill
     * @author timattt
     */
    void addInSkill(QString name, int lev);
    /**
     * Adds output skill with given level.
     * If graph field is not nullptr then signal will be emited.
     * @param name - skillName
     * @param lev - level of skill
     * @author timattt
     */
    void addOutSkill(QString name, int lev);
    /**
     * Removes skill with given name from input skills map.
     * If graph field is not nullptr then signal will be emited.
     * @param name - skillName
     * @author timattt
     */
    void removeInSkill(QString name);
    /**
     * Removes skill with given name from output skills map.
     * If graph field is not nullptr then signal will be emited.
     * @param name - skillName
     * @author timattt
     */
    void removeOutSkill(QString name);
    /**
     * Gives input skills map.
     * @returns qmap with skills {QString : skill name} -> {int: skill level}
     * @author timattt
     */
	const QMap<QString, int>& getInSkills() const;
    /**
     * Gives output skills map.
     * @returns qmap with skills {QString : skill name} -> {int: skill level}
     * @author timattt
     */
	const QMap<QString, int>& getOutSkills() const;
	/**
	 * Gives node name.
	 * @returns QString - node name
	 * @author timattt
	 */
	QString getName() const;
	/**
	 * Sets node name.
	 * If name field is changed then invokes update function.
	 * @param name - node new name
	 * @author timattt
	 */
	void setName(QString name);
	/**
	 * Gives this node file name. (File where this node is stored)
	 * @returns QString - path to file
	 * @author timattt
	 */
	QString getFile() const;
	/**
	 * Sets node file name. Where it is stored.
	 * If file field is changed then invokes update function.
	 * @param file - node new file path
	 * @author timattt
	 */
	void setFile(QString file);
	/**
	 * Sets node color.
	 * If color field is changed then invokes update function.
	 * @param color - QColor, node new color
	 * @author timattt
	 */
	void setColor(QColor color);
	/**
	 * Clears input and output skills maps.
	 * If graph field is not nullptr then emits signal and calls update function.
	 * @author timattt
	 */
	void clearSkills();
	/**
	 * Gives node color.
	 * @return QColor - node current color.
	 * @author timattt
	 */
	QColor getColor() const;
	/**
	 * Gives node description.
	 * @return QColor - node current description.
	 * @author timattt
	 */
	QString getDescription() const;
	/**
	 * Sets node description.
	 * If description field is changed then invokes update function.
	 * @param color - QString, node new description
	 * @author timattt
	 */
	void setDescription(QString str);
	/**
	 * Gives value of progress in given skill.
	 * @param skill - name of requested skill
	 * @returns value of progress - level
	 * @author timattt
	 */
	double getSkillProgress(QString skill) const;
	/**
	 * Gives map of student progress.
	 * {QString : skill name} -> {double skill progress}
	 * @returns QMap - progress
	 * @author timattt
	 */
	const QMap<QString, double>& getProgress() const;
	/**
	 * Calculates value in range [0, 1].
	 * That represents progress in skill in only this node.
	 * Example:
	 * If there is input skill with level 1. And output level is 2. And progress is 1.5.
	 * Then this function will return: ( prog - from ) / (to - from) = (1.5 - 1) / (2 - 1) = 0.5
	 * @param skill - skill value must be calculated for.
	 * @returns - double in range [0, 1]
	 * @author timattt
	 */
	double getProgressScalar(QString skill) const;
	/**
	 * Checks if progress for given skill exists.
	 * @param skill name
	 * @returns true if progress exists
	 * @author timattt
	 */
	bool containsProgress(QString skill);
	/**
	 * Sets progress for given skill.
	 * If graph is not nullptr then emits signal.
	 * Calls update function.
	 * @param skill - skill name
	 * @param val - progress value
	 * @author timattt
	 */
	void setProgress(QString skill, double val);
	/**
	 * Checks this node is selected.
	 * @returns true if graph field is not null and this node is selected by it.
	 * @author timattt
	 */
	bool isSelected();
	/**
	 * Clears progress map.
	 * Calls update method.
	 * @author timattt
	 */
	void clearStudentProgress();
    //--------------------------------------

protected:

    // protected functions
    //--------------------------------------
    /**
     * Reimplemented from QGraphicsItem
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    /**
     * Reimplemented from QGraphicsItem
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /**
     * Reimplemented from QGraphicsItem
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    //--------------------------------------

private:

    // Fields
    //--------------------------------------
    /**
     * List that contains all edges that are connected with this node.
     * @autohr timattt
     */
    QList<Edge*> edgeList;
    /**
     * This field represents CourseUnitViewer that currently contains this node.
     * Can be nullptr. If it is not then it will update physics using other items from this viewer.
     * And emits signals to this viewer object.
     * @author timattt
     */
    CourseUnitViewer *graph;
    /**
     * Map {QString: skill name} - > {int: skill level} that represents input skills of this node.
     * @author timattt
     */
    QMap<QString, int> inSkills;
    /**
     * Map {QString: skill name} - > {int: skill level} that represents output skills of this node.
     * @author timattt
     */
    QMap<QString, int> outSkills;
    /**
     * Map {QString: skill name} - > {int: skill level} that represents progress in this node.
     * @author timattt
     */
    QMap<QString, double> progress;
    /**
     * File path where this node is stored.
     * @author timattt
     */
    QString file;
    /**
     * Node name.
     * @author timattt
     */
    QString name;
    /**
     * Node description.
     * @author timattt
     */
    QString description;
    /**
     * Current color of this node.
     * @author timattt
     */
    QColor color = Qt::lightGray;
    //--------------------------------------
};

// global functions
//--------------------------------------
/**
 * Transfers information from courseUnit into given node.
 * @param cu - CourseUnit object. From it info is taken.
 * @param nd - Node object. Where to place info.
 * @author timattt
 */
void fromCourseUnitToNode(CourseUnit * cu, Node * nd);
/**
 * Transfers information from node into given courseUnit.
 * @param nd - Node object. From it info is taken.
 * @param cu - CourseUnit object. Where to place info.
 * @author timattt
 */
void fromNodeToCourseUnit(Node * nd, CourseUnit * cu);
//--------------------------------------

#endif
