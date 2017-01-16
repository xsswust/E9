#ifndef TESTING_H
#define TESTING_H

#include <QWidget>
#include <QTimer>
#include "src/HardWare/keyboard_dependence.h"

namespace Ui {
class Testing;
}

class Testing : public QWidget
{
    Q_OBJECT

public:
    explicit Testing(QWidget *parent = 0);
    ~Testing();
	void Testing_Init();   //界面初始化
	void Testing_UnIinit();  // 界面跳转处理
	int g_Start_num;   // 记录当前屏显示 开始的序号
	bool exit_flag;
    
    void showParaChoose();
    void showParaChooseAll();
public slots:
	void show_Bottle_St(int bottleId, int st, int sampleId, int mode); // 时间更新

	void timeUpData(); // 时间更新
	void insertValue(QString nustr); // 键值处理
	void keyboardDelete();		//s删除键处理

private slots:
	void on_pbExit_clicked();		// 返回键处理

	void pb_Bottle1_Reless();   // 1号瓶 按钮

	void pb_Bottle2_Reless();

	void pb_Bottle3_Reless();

	void pb_Bottle4_Reless();

	void pb_Bottle5_Reless();



	void Button_Prev_clicked();  // 上一组

	void Button_Next_clicked();	 // 下一组

    void on_Bottle1ParaClass_currentIndexChanged(int index);

    void on_Bottle2ParaClass_currentIndexChanged(int index);

    void on_Bottle3ParaClass_currentIndexChanged(int index);

    void on_Bottle4ParaClass_currentIndexChanged(int index);

    void on_Bottle5ParaClass_currentIndexChanged(int index);

    void on_Bottle1_ClassNo1_currentIndexChanged(int index);

    void on_Bottle2_ClassNo1_currentIndexChanged(int index);

    void on_Bottle3_ClassNo1_currentIndexChanged(int index);

    void on_Bottle4_ClassNo1_currentIndexChanged(int index);

    void on_Bottle5_ClassNo1_currentIndexChanged(int index);

private:
    QTimer *pTesttimerTime;
    Keyboard_dependence *im;
	//QLineEdit *p_editer;
    Ui::Testing *ui;
};
extern Testing *pTesting;
#endif // TESTING_H
