#ifndef CLIENTCHAT_H
#define CLIENTCHAT_H

#include <QWidget>

namespace Ui {
class ClientChat;
}

class ClientChat : public QWidget
{
    Q_OBJECT

public:

	// constructor and destructor
	//=======================================
    ClientChat(QWidget *parent);
    ~ClientChat();
    //=======================================

private:

    // private functions
    //=======================================
    Ui::ClientChat *ui;
    //=======================================

};

#endif // CLIENTCHAT_H
