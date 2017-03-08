#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <QWidget>
#include "keyboard_dependence.h"

namespace Ui {
class Debugging;
}

class Debugging : public QWidget
{
    Q_OBJECT
    
public:
    explicit Debugging(QWidget *parent = 0);
    ~Debugging();

	void Init();   //初始化
	void UnInit();	// 界面跳出处理

	bool exit_flag;

public slots:
   //void loginAndcancel();
   void InsertValue(QString nustr);
   void NumberBack();

private slots:
    void on_pbExit_clicked();

	// 开关量
	void on_pb_single_clicked();
	//组合 开关
	void on_pb_com_released();

private:
    Ui::Debugging *ui;
	Keyboard_dependence* im;
};

#endif // DEBUGGING_H
