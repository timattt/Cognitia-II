#ifndef COURSEUNITVIEWERSETTINGS_H
#define COURSEUNITVIEWERSETTINGS_H

#include <QDialog>

namespace Ui {
class CourseUnitViewerSettings;
}

/**
 * Simple dialog box for changing CUV settings.
 * @author timattt
 */
class CourseUnitViewerSettings: public QDialog {
Q_OBJECT

public:

	// constructor and destructor
	//================================================
	/**
	 * Simple init contructor.
	 * Give it totalNodes, totalEdges, totalItems, att factor, rep factor, own length
	 * @author timattt
	 */
	CourseUnitViewerSettings(QWidget *parent, int totalN, int totalE, int totalI, double att, double rep, double own);
	~CourseUnitViewerSettings();
	//================================================

private slots:

	// private slots
	//================================================
	void on_attFact_textEdited(QString v);
	void on_repFact_textEdited(QString v);
	void on_ownLength_textEdited(QString v);
	//================================================

public:

	// public functions
	//================================================
	/**
	 * Tmp value of repulsion.
	 * @author timattt
	 */
	double repulsion;
	/**
	 * Tmp value of attraction.
	 * @author timattt
	 */
	double attraction;
	/**
	 * Tmp value of own length.
	 * @author timattt
	 */
	double ownLength;
	//================================================

private:

	// private fields
	//================================================
	Ui::CourseUnitViewerSettings *ui;
	//================================================
};

#endif // COURSEUNITVIEWERSETTINGS_H
