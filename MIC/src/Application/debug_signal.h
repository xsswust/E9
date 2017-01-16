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

	void on_rb_led1_365_clicked();

	void on_rb_led2_365_clicked();

	void on_rb_led3_365_clicked();

	void on_rb_led4_365_clicked();

	void on_rb_led5_365_clicked();

	void on_rb_led1_405_clicked();

	void on_rb_led2_405_clicked();

	void on_rb_led3_405_clicked();

	void on_rb_led4_405_clicked();

	void on_rb_led5_405_clicked();

	void on_rb_led1_610_clicked();

	void on_rb_led2_610_clicked();

	void on_rb_led3_610_clicked();

	void on_rb_led4_610_clicked();

	void on_rb_led5_610_clicked();

private:
	Ui::debug_signal *ui;

	QTimer *timeForGetTEMP_1Minute;
	QTimer *timeForGetTEMP_100ms;
};

#endif // DEBUG_SIGNAL_H
