#ifndef TEMPSET_H
#define TEMPSET_H

#include <QWidget>
#include "src/HardWare/keyboard_dependence.h"
#include "common.h"
#include <QTime>

namespace Ui {
class TempSet;
}

class TempSet : public QWidget
{
	Q_OBJECT

public:
	explicit TempSet(QWidget *parent = 0);
	~TempSet();
	void Init();  // 初始化
	void UnInit();	// 界面跳转
	void Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data); //设置  通道数据
	bool Get_Check_Sum(RECEIVE_MESSAGE_TYPE *receive,quint8 addr);  // check data
	void Set_Chanel_Temp(UINT8 chanel, UINT16 data);
	bool exit_flag ;

public slots:
	void insertValue(QString nustr);
	void keyboardDelete();
	void UpdateShow();		 // 时间更新显

private slots:

	void ui_exit(void);

	//void on_pb_exit_clicked();

	void pb_Ch1_Checkup();

	void pb_Ch2_Checkup();

	void pb_Ch3_Checkup();

	void pb_Ch4_Checkup();

	void pb_Ch5_Checkup();

	void pb_Ch6_Checkup();

private:
	Ui::TempSet *ui;
	Keyboard_dependence *im;

	QTimer *pUpdateTime;
};

#endif // TEMPSET_H
