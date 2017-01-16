#include "src/HardWare/keyboard_dependence.h"
#include <QWidget>
//#include <QWSInputMethod>
#include <QSignalMapper>

Keyboard_dependence::Keyboard_dependence(QWidget *parent)
    : QWidget(parent)
{
//	ui.setupUi(this);
	v_key = new NumberKeyBoard(this);

}
bool Keyboard_dependence::eventFilter(QObject *watched,QEvent *event)
{
	if ((event->type()==QEvent::MouseButtonPress) )

	{
		showkeyboard_d();

	}

	return QObject::eventFilter(watched,event);

}

void  Keyboard_dependence::showkeyboard_d()
{
	QWidget *widget=QApplication::focusWidget();//获取当前焦点所在控件
	if (!widget)
		return;
	QRect widgetRect=widget->rect();//获取控件的几何信息，即宽高
	QPoint panelPos=QPoint(-200 ,widgetRect.bottom());//以控件的左上角为原点，空间的左下角的坐标
	panelPos=widget->mapToGlobal(panelPos);//转换为全局即以整个屏幕的左上角为原点的坐标
	v_key-> setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
//	inputDialog -> move(panelPos);
    v_key-> move(200,60);
    v_key->showNormal();
}

Keyboard_dependence::~Keyboard_dependence()
{

}
