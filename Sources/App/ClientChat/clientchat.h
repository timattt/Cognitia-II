#ifndef CLIENTCHAT_H
#define CLIENTCHAT_H

#include <QWidget>

namespace Ui {
class ClientChat;
}

class StudentProgress;
class Node;

class ClientChat : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//=======================================
    ClientChat(QWidget *parent);
    ~ClientChat();
    //=======================================

public slots:

    void setStudentProgress(StudentProgress * prog);
    void clearAll();
    void nodeSelected(Node * nd);
    void senderNameChanged(QString name);

private:

    // private functions
    //=======================================
    Ui::ClientChat *ui;
    StudentProgress * currentStp;
    Node * currentNode;
    QString senderName;
    //=======================================

private slots:

	// private slots
	//=======================================
	void on_sendButton_clicked();
	//=======================================

private:

	// private functions
	//=======================================
	void addMessage(QString author, QString text);
	void clearGui();
	//=======================================

};

#endif // CLIENTCHAT_H
