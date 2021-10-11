#ifndef SMARTHELPER_H
#define SMARTHELPER_H

#include <QtWidgets>

namespace Ui {
class SmartHelper;
}

/**
 * This dialogbox is used to show help to users.
 * To use it you must have file with help.
 * Then you insert code in to your widget:
 *
 * -------
 * SmartHelper helper(YOUR_HELP_FILE_PATH, this);
 * helper.exec();
 * -------
 *
 * @author timattt
 */
class SmartHelper : public QDialog
{
    Q_OBJECT

public:

	// Constructor and destructor
	//----------------------------------------
	/**
	 * Constructor for this dialogbox.
	 * @param path - path to file where your help is. Help may be in markdown format.
	 * @parent - parent widget.
	 * @author timattt
	 */
    SmartHelper(QString path, QWidget *parent);
    /**
     * Simple widget destructor.
     */
    ~SmartHelper();
    //----------------------------------------

private:

    // Fields
    //----------------------------------------
    Ui::SmartHelper *ui;
    //----------------------------------------

};

#endif // SMARTHELPER_H
