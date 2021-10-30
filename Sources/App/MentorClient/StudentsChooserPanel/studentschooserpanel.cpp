#include "studentschooserpanel.h"

StudentsChooserPanel::StudentsChooserPanel(QWidget *parent) :
    QWidget(parent),
	layout(nullptr),
	groupBox(nullptr),
	lastStudent("")
{
	QVBoxLayout * la = new QVBoxLayout(this);

	QLabel * header = new QLabel(this);
	header->setText("Available students:");
	la->addWidget(header);

	groupBox = new QGroupBox(this);


	la->addWidget(groupBox);

	groupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

	groupBox->setLayout(layout = new QVBoxLayout(groupBox));
	QSpacerItem * sp = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
	la->addItem(sp);
}

QString StudentsChooserPanel::getCurrentStudent() {
	return lastStudent;
}

void StudentsChooserPanel::clearAll() {
	QList<QRadioButton*> toDelete;

	for (QObject *o : groupBox->children()) {
		QRadioButton *mx = dynamic_cast<QRadioButton*>(o);
		if (mx != nullptr) {
			toDelete.push_back(mx);
		}
	}
	while (!toDelete.empty()) {
		delete toDelete.first();
		toDelete.pop_front();
	}
}

void StudentsChooserPanel::setStudents(QList<QString> lst) {
	clearAll();

	for (QString str : lst) {
		QRadioButton * w = new QRadioButton(groupBox);
		w->setText(str);
		layout->addWidget(w);
		connect(w, SIGNAL(clicked()), this, SLOT(rb_clicked()));

		if (str == lst.first()) {
			lastStudent = str;
			w->click();
		}
	}
}

void StudentsChooserPanel::rb_clicked() {
	QRadioButton * button = dynamic_cast<QRadioButton*>( QObject::sender() );
	QString name = button->text();
	lastStudent = name;
	emit studentSelected(name);
}
