#ifndef TESTING_THREAD_H
#define TESTING_THREAD_H
#include <QThread>
#include <QTimer>
#include "common.h"
#include "src/HardWare/com_trandata.h"
#define LED_POWER_TIMERLEN		(500*1000)  // 500ms
//#define LED_POWER_TIMERLEN		(2*1000*1000)  // 500ms






class testing_thread: public QThread
{
	Q_OBJECT
public:
    testing_thread();
	virtual ~testing_thread();
	void run();
	bool timer_flag;  // timer 标志
	bool timer_out_flag;  // timer out 标志

public slots:
	void timeUp_Testing();

private:
	QTimer *time_Testing;
};

extern testing_thread *ptesting_thread;
#endif // TESTING_THREAD_H
