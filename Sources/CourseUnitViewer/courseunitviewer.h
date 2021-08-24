#ifndef COURSEUNITVIEWER_H
#define COURSEUNITVIEWER_H

#include <QtWidgets>
#include <QGraphicsScene>
#include "courseunitwidget.h"
#include "Node/node.h"
#include "Node/edge.h"

namespace Ui {
class CourseUnitViewer;
}

class CourseUnitViewer : public QWidget
{
    Q_OBJECT

public:

    // Constructor and destructor
    //----------------------------------
    CourseUnitViewer(QWidget *parent);
    ~CourseUnitViewer();
    //----------------------------------

private:

    // Fields
    //----------------------------------
    Ui::CourseUnitViewer *ui;
    QGraphicsScene * scene;
    int timerId = 0;
    qreal currentScale = 1;
    //----------------------------------

private:

    // Private functions
    //----------------------------------
    void timerEvent(QTimerEvent * event);
    qreal scrollPosToScale(int pos);
    //----------------------------------

public:

    // Public functions
    //----------------------------------
    void itemMoved();
    //----------------------------------


private slots:

    // Slots
    //----------------------------------
    void on_scaleScroll_sliderMoved(int position);
    //----------------------------------

protected:

    // Protected functions
    //----------------------------------
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    //----------------------------------

};

#endif // COURSEUNITVIEWER_H
