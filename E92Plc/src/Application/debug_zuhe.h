#ifndef DEBUG_ZUHE_H
#define DEBUG_ZUHE_H

#include <QWidget>
#include "keyboard_dependence.h"

// 默认检测号
#define TESTID_DEFAULT		1

// 默认瓶号
#define BOTTOMID_DEFAULT		1



namespace Ui {
class debug_zuhe;
}

class debug_zuhe : public QWidget
{
    Q_OBJECT
    
public:
	explicit debug_zuhe(QWidget *parent = 0);
	~debug_zuhe();
    int testMode_flag;
	// 初始化界面需要的一些值
	void initPainter();

	bool exit_flag;

private slots:
	void timeOut();		 // 时间更新显
	void on_pbExit_clicked();   // 界面退出

	void on_pbCybjy_released();

	void on_pbSydjcc_released();

	void on_pbJccdsy_released();

	void on_pbStopAll_released();

	void on_pbDlgdjcc_released();

	void on_pbZlhdsy_released();

public slots:
   //void loginAndcancel();
   void InsertValue(QString nustr);
   void NumberBack();


private:
	Ui::debug_zuhe *ui;
	// 定义定时器
	QTimer *time_100ms;
	// 键盘输入
	Keyboard_dependence* im;
};
extern debug_zuhe *pdebug_zuhe;

#endif // TESTMODE_H
