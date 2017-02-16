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


private:
	Ui::debug_signal *ui;

	QTimer *timeForGetTEMP_1Minute;
	QTimer *timeForGetTEMP_100ms;
};

#endif // DEBUG_SIGNAL_H
