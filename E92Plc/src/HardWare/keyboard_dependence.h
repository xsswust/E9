#ifndef KEYBOARD_DEPENDENCE_H
#define KEYBOARD_DEPENDENCE_H

#include <QWidget>
//#include "ui_keyboard_dependence.h"
//#include <QWSInputMethod>
#include <QSignalMapper>
#include "../Application/NumberKeyBoard.h"

class Keyboard_dependence : public QWidget
{
    Q_OBJECT

public:
    Keyboard_dependence(QWidget *parent = 0);
    virtual ~Keyboard_dependence();
    class NumberKeyBoard* v_key;
	bool eventFilter(QObject *watched,QEvent *event);
	void  showkeyboard_d();

private:
//    Ui::Keyboard_dependenceClass ui;
};

#endif // KEYBOARD_DEPENDENCE_H
