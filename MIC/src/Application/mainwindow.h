#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



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

  //  void on_lbTempValue2_linkActivated(const QString &link);
  //  void on_lbTemp2_linkActivated(const QString &link);

private:
    QTimer *ptimerUpdateTime;
    Ui::MainWindow *ui;
};
extern MainWindow *pmainwin;  // 主界面
#endif // MAINWINDOW_H
