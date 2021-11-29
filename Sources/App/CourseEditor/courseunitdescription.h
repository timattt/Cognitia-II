#ifndef COURSEUNITDESCRIPTION_H
#define COURSEUNITDESCRIPTION_H

#include <QWidget>

namespace Ui {
class CourseUnitDescription;
}

class CourseEditor;
class Node;

class CourseUnitDescription : public QWidget
{
    Q_OBJECT

public:
    explicit CourseUnitDescription(QWidget *parent = nullptr);
    ~CourseUnitDescription();

private:
    Ui::CourseUnitDescription *ui;
    CourseEditor * editor;

public slots:

	void nodeSelected(Node * nd);
	void ensureLocked();
	void unlock();

private slots:

	/**
	 * This function update currentNode decription.
	 * And sets new description to markdown panel.
	 * @author timattt
	 */
	void on_descrPanel_textChanged();
    /**
     * Hides or shows markdown preview panel.
     *
     * @author timattt
     */
    void on_markDownShow_stateChanged(int v);
};

#endif // COURSEUNITDESCRIPTION_H
