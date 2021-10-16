/*
 * viewport.h
 *
 *  Created on: Oct 15, 2021
 *      Author: timat
 */

#ifndef APP_COURSEUNITVIEWER_VIEWPORT_H_
#define APP_COURSEUNITVIEWER_VIEWPORT_H_

#include <QtWidgets>

/**
 * This class is used to add scene move and zoom functions.
 *
 * @author timattt
 *
 * "Люди вместо выбора представителя интересов поставлены перед выбором интересов представителя" - А.С.Маслов
 */
class Viewport: public QGraphicsView {
public:

	// constructor and destructor
	//==============================================
	/**
	 * Simple constructor.
	 * @author timattt
	 */
	Viewport(QWidget * parent);
	~Viewport() = default;
	//==============================================

private:

	// private fields
	//==============================================
	/**
	 * If mouse button is pressed then this field will be initialized with mouse pos on previous update.
	 * @author timattt
	 */
	QPointF prev;
	/**
	 * Checks if left button is pressed currently.
	 * @author timattt
	 */
	bool pressed;
	/**
	 * Current camera position in scene coords.
	 * @author timattt
	 */
	QPointF cameraPos;
	/**
	 * Current scale in scene coords.
	 * @author timattt
	 */
	float curScale;
	//==============================================

public:

	// public functions
	//==============================================
	/**
	 * Adds given value to current scale field.
	 * Makes all necesarry action to rescale:
	 * @param v - scale delta
	 * @author timattt
	 */
	void addScale(float v);
	/**
	 * Moves camera position on the given value.
	 * Warning! It moves not scene rect. It moves all items!
	 * @param delta - vector to move camera for.
	 * @author timattt
	 */
	void moveCamera(QPointF delta);
	/**
	 * Focuses on all items.
	 * Resizes nicely.
	 * @author timattt
	 */
	void focusOn();
	/**
	 * Sets camera pos.
	 * Work just like moveCamera function.
	 * @param p - new camera pos value in scene coords
	 * @author timattt
	 */
	void setCameraPos(QPointF p);
	/**
	 * Sets current scale.
	 * Works just like addScale function.
	 * @param v - scale value. May be greater then zero.
	 * @author timattt
	 */
	void setScale(float v);
	/**
	 * Gives camera pos.
	 * Or all scene items shift.
	 * @returns vector - camera pos
	 * @author timattt
	 */
	QPointF getCameraPos() const;
	/**
	 * Gives current scale.
	 * @returns current scale.
	 * @author timattt
	 */
	float getCurScale() const;
	//==============================================

protected:

	// protected functions
	//==============================================
	void resizeEvent(QResizeEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	//==============================================

};

#endif /* APP_COURSEUNITVIEWER_VIEWPORT_H_ */
