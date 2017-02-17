#include "testing_thread.h"
#include "common.h"
#include "sql_control.h"

testing_thread *ptesting_thread;

testing_thread::testing_thread()
{
	timer_flag = false;  //timer 没有启动
	time_Testing = new QTimer();
	timer_out_flag = false;  // 时间到
//	time_Testing->start(TIMER_LEN_1MINUTE);  // 定时一分钟
//	time_Testing->start(TIMER_LEN_1MINUTE);  // 定时一分钟
//	timer_flag = true;  //timer 没有启动
	// 建立连接
	connect(time_Testing,SIGNAL(timeout()),this,SLOT(timeUp_Testing()));  //

}


testing_thread::~testing_thread()
{
	//delete timeupTimer;
}


// 线程运行
void testing_thread::run()
{
	int i= 0, idle_num = 0;

	msleep(10*1000); // 10S
	while(1)
	{
		msleep(1000); //100ms
		idle_num = 0;
	}
}

void testing_thread::timeUp_Testing(){

}
