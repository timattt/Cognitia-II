#include "courseunitviewer.h"
#include "ui_courseunitviewer.h"

#define MIDDLE_SCROOL_NUM 5
#define MIN_SCALE 0.25

CourseUnitViewer::CourseUnitViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseUnitViewer),
    scene(new QGraphicsScene())
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);

    Node * test1 = new Node(this);
    Node * test2 = new Node(this);

    test1->setPos(0, 0);

    Edge * edge = new Edge(test1, test2);

    scene->addItem(edge);

    scene->addItem(test1);
    scene->addItem(test2);

}

CourseUnitViewer::~CourseUnitViewer()
{
    delete ui;
    delete scene;
}

void CourseUnitViewer::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    const QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    for (Node *node : qAsConst(nodes))
        node->calculateForces();

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void CourseUnitViewer::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1);
}

qreal CourseUnitViewer::scrollPosToScale(int pos) {
    return qreal((MIN_SCALE * MIDDLE_SCROOL_NUM - 1) / (MIDDLE_SCROOL_NUM - 1) + qreal(pos) * (1 - MIN_SCALE) / (MIDDLE_SCROOL_NUM - 1));
}

void CourseUnitViewer::on_scaleScroll_sliderMoved(int position)
{
    qreal nsc = scrollPosToScale(position);

    ui->zoomPercentage->setText(QString::number(nsc * 100.0) + "%");

    ui->graphicsView->scale(nsc / currentScale, nsc / currentScale);
    currentScale = nsc;
}

void CourseUnitViewer::dragEnterEvent(QDragEnterEvent *event)
{
}

void CourseUnitViewer::dragMoveEvent(QDragMoveEvent *event)
{
    
}

void CourseUnitViewer::dragLeaveEvent(QDragLeaveEvent *event)
{
    
}

