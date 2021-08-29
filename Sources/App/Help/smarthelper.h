#ifndef SMARTHELPER_H
#define SMARTHELPER_H

#include <QtWidgets>

namespace Ui {
class SmartHelper;
}

class SmartHelper : public QDialog
{
    Q_OBJECT

public:

	// Constructor and destructor
	//----------------------------------------
    SmartHelper(QString path, QWidget *parent);
    ~SmartHelper();
    //----------------------------------------

private:

    // Fields
    //----------------------------------------
    Ui::SmartHelper *ui;
    //----------------------------------------

public slots:

	// public slots
	//----------------------------------------

	//----------------------------------------

};

#endif // SMARTHELPER_H
