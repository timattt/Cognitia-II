#ifndef OUTDELTASKILLS_H
#define OUTDELTASKILLS_H

#include <QWidget>

namespace Ui {
class OutputRelativeSkillsPanel;
}

class Node;

class OutputRelativeSkillsPanel : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//===============================================
    OutputRelativeSkillsPanel(QWidget *parent);
    ~OutputRelativeSkillsPanel();
    //===============================================

private:

    // private fields
    //===============================================
    Ui::OutputRelativeSkillsPanel *ui;
    //===============================================

public slots:

	// public slots
	//==============================================
	/**
	 * Clears everything from this widget.
	 * @author timattt
	 */
	void clearAll();
	/**
	 * This will be called when progress of current course unit is changed.
	 * @param skill - skill that is changed.
	 * @param lev - new skill progress.
	 * @author timattt
	 */
	void progressMade(QString skill, double lev);

	/**
	 * Unpacks single node to this gui.
	 * This will be called when some node is selected.
	 * Can be nullptr if no node is selected.
	 * @param nd - node that will be unpacked. Can be nullptr.
	 * @author timattt
	 */
	void nodeSelected(Node *nd);
	//==============================================
};

#endif // OUTDELTASKILLS_H
