#ifndef EDGE_H
#define EDGE_H

#include <QtWidgets>

// Used classes
//====================================
class Node;
class CourseUnitViewer;
//====================================

/**
 * This class is used in CourseScene class. It represents edge and connects two nodes.
 * It can be used to connect two nodes, for this you may use first constructor.
 * Also it can be used to connect node and any point.
 * For this you can use second constructor and setTarget and connectNode methods.
 * @author timattt
 */
class Edge : public QGraphicsItem
{

public:

	// public fields
	//--------------------------------------
	enum { Type = UserType + 2 };
	//--------------------------------------

public:

    // Constructors
    //--------------------------------------
	/**
	 * Initializes edge where in start point there is node and at the end there will be cursor.
	 * This edge is not connecting to source node.
	 * destNode is nullptr.
	 * @param sourceNode - edge points from this node
	 * @param view - courseUnitViewer that may contains this node.
	 * @author timattt
	 */
    Edge(Node *sourceNode, CourseUnitViewer * view);
    /**
     * Initializes new edge that connects two nodes. All you need after - add this edge to scene.
     * This edge will be putted to edges list of these nodes.
     * @param sourceNode - edge points from this node
     * @param destNode - edge points to this node
     * @param view - courseUnitViewer that may contains this node.
     * @author timattt
     */
    Edge(Node *sourceNode, Node *destNode, CourseUnitViewer * view);
    /**
     * This edge will be removed from sourceNode and destNode and from scene.
     * @author timattt
     */
    ~Edge();
    //--------------------------------------

public:

    // public functions
    //--------------------------------------
    /**
     * Gives node that edge points from.
     * @returns node object
     * @author timattt
     */
    Node *getSourceNode() const;
    /**
     * Gives node that edge points to.
     * @returns node object
     * @author timattt
     */
    Node *getDestNode() const;
    /**
     * If node source, dest or target is changed then this method may be called to change edge geometry.
     * @author timattt
     */
    void adjust();
    /**
     * Reimplemented from QGraphicsItem
     */
    int type() const override { return Type; }
    /**
     * Checks if this edge is currently has no destNode. So it uses target field as destination.
     * @returns true if dest is nullptr.
     * @author timattt
     */
    bool isDragable() const;
    /**
     * Sets new target for edge (where it may point to).
     * Calls djust function.
     * This method can be used only for edge that has no dest node.
     * @param p - destination point in scene coords.
     * @author timattt
     */
    void setTarget(QPointF p);
    /**
     * Gives target for edge (where it may point to).
     * This method can be used only for edge that has no dest node.
     * @returns destination point in scene coords.
     * @author timattt
     */
    QPointF getTarget() const;
    /**
     * This method connects this edge to new destination node.
     * It will add this edge into source and dest node adges lists.
     * This function can not be used if dest node is not nullptr.
     * It is recomended to use Edge(Node *sourceNode, CourseUnitViewer * view) constructor before using this function.
     * @param dest - dest node value. dest node field may be nullptr.
     * @author timattt
     */
	void connectToNode(Node * dest);
    //--------------------------------------

private:

	// private functions
	//--------------------------------------
	/**
	 * Calculates arrow size
	 * @returns value of arrow size in pixels
	 * @author timattt
	 */
	double getArrowSize() const;
	//--------------------------------------

protected:

    // protected functions
    //--------------------------------------
    /**
     * Reimplemented from QGraphicsItem
     */
    QRectF boundingRect() const override;
    /**
     * Reimplemented from QGraphicsItem
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //--------------------------------------

private:

    // Fields
    //--------------------------------------
    /**
     * Edge points from this node. May no be nullptr.
     * @author timattt
     */
    Node *source;
    /**
     * Edge points to this node. Can be nullptr, so target field is used to determine end coordinates,
     * You can use connectToNode() method to connect this node to dest.
     * @author timattt
     */
    Node *dest;
    /**
     * Point where edge is starting in pixels in scene coords.
     * Changing in adjust function.
     * @author timattt
     */
    QPointF sourcePoint;
    /**
     * Point where edge is ending in pixels in scene coords.
     * Changing in adjust function.
     * @author timattt
     */
    QPointF destPoint;
    /**
     * This point will be used to determine end destPoint if dest node is nullptr.
     * @author timattt
     */
    QPointF target;
    /**
     * Parent to which this object belongs.
     * @author timattt
     */
    CourseUnitViewer * viewer;
    //--------------------------------------

};
#endif
