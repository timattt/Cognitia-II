#ifndef STUDENTSCHOOSERPANEL_H
#define STUDENTSCHOOSERPANEL_H

#include <QtWidgets>

class StudentsChooserPanel: public QWidget {
Q_OBJECT

public:

	// constructor and destructor
	//====================================================
	StudentsChooserPanel();
	~StudentsChooserPanel() = default;
	//====================================================

public slots:

	// public slots
	//====================================================
	void clearAll();
	//====================================================

private slots:

	// public slots
	//====================================================
	void rb_clicked();
	//====================================================

public:

	//====================================================
	QString getCurrentStudent();
	void setStudents(QList<QString> lst);
	//====================================================

private:

	//====================================================
    QVBoxLayout *layout;
    QGroupBox *groupBox;
    QString lastStudent;
	//====================================================

signals:

	// signals
	//====================================================
	void studentSelected(QString name);
	//====================================================

};

#endif // STUDENTSCHOOSERPANEL_H
