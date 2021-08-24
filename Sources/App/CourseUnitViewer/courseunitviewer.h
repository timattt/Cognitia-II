#ifndef COURSEUNITVIEWER_H
#define COURSEUNITVIEWER_H

#include <QtWidgets>

#include "nodesdefs.h"

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
    Edge* dragEdge;
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
    bool nodesCanMove();
    bool deleteModeIsOn();
    //----------------------------------

private slots:

    // Slots
    //----------------------------------
    void on_scaleScroll_sliderMoved(int position);
    void on_freezeCheckbox_stateChanged(int arg1);
    void on_pushButton_2_clicked();
    //----------------------------------

};
#endif
