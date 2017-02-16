#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define GPIO_OUT_INTERFACE	0			// 接口板输出
#define GPIO_IN_INTERFACE	1			//接口板输入
#define GPIO_EXT_OUT_INTERFACE 2		// 扩展板输出
#define GPIO_EXT_IN_INTERFACE 3		// 扩展板输 入
#define PWM1_INTERFACE 4		// 扩展板输 入
#define PWM2_INTERFACE 5		// 扩展板输 入


#define GPIO_EXT_OUT_NUM	16		// 扩展板输出  16

#define GPIO_EXT_IN_NUM	8		// 扩展板输入  8


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void ShowUnit();   // 指标单位显示

//private:
	void MainWindow_Init();	// 初始化
	void MainWindow_unInit();	// 界面切换

	void Show_system_Info();	// 显示系统状态


public slots:
	void timeUpdate();		 // 时间更新显
	void ShowTemp();		// 温度显示
    
private slots:
	void pbtest_Reless();

	void pbhisdata_Reless();

	void pbdebug_Reless();

	void pbset_Reless();

	void pbhelp_Reless();

	void pbst_Reless();


private:
    QTimer *ptimerUpdateTime;
    Ui::MainWindow *ui;
};
extern MainWindow *pmainwin;  // 主界面
#endif // MAINWINDOW_H
