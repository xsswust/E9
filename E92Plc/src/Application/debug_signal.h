#ifndef DEBUG_SIGNAL_H
#define DEBUG_SIGNAL_H

#include <QWidget>

namespace Ui {
class debug_signal;
}

class debug_signal : public QWidget
{
	Q_OBJECT

public:
	explicit debug_signal(QWidget *parent = 0);
	~debug_signal();

	int g_num;
	bool exit_flag ;


public slots:
	void timeUp_1Minute();
	void timeUp_100Ms();
	void Init();  // 初始化
	void Uninit();  // 初始化



private slots:
	void on_pB_Exit_clicked();

	// 试剂阀
	void on_pB_shiji_released();
	// 水样阀
	void on_pB_shuiyangfa_released();

	void on_pB_cilvsuan_released();

	void on_pB_hunhefa_released();

	void on_pB_wujunshuifa_released();

	void on_pB_kongqifa_released();

	void on_pB_jinyefa_released();

	void on_pB_jinyefa2_released();

	void on_pB_jinyefa3_released();

	void on_pB_jinyefa4_released();

	void on_pB_jinyefa5_released();

	void on_pB_jinyefa6_released();

	void on_pB_zailiufa_released();

	void on_pB_payefa1_released();

	void on_pB_paiyefa2_released();

	void on_pB_paiyefa3_released();

	void on_pB_paiyefa4_released();

	void on_pB_paiyefa5_released();

	void on_pB_paiyefa6_released();

	void on_pB_rudongbengzheng_released();

	void on_pB_waipaifa_released();

	void on_pB_paiyebeng_released();

	void on_pB_jiliangfashag_released();

	void on_pB_jiliangdaxia_released();

	void on_pB_shajundeng_released();

	void on_pB_rongdongbengfan_released();

private:
	Ui::debug_signal *ui;

	QTimer *timeForGetTEMP_1Minute;
	QTimer *timeForGetTEMP_100ms;
};

#endif // DEBUG_SIGNAL_H
