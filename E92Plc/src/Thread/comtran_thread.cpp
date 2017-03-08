#include "comtran_thread.h"
#include "src/HardWare/posix_qextserialport.h"
#include "src/HardWare/qextserialbase.h"

#include <QFile>
#include <QTimer>
#include <QByteArray>
#include <QDataStream>
#include <QSettings>
#include <QDateTime>
#include "sql_control.h"
#include <qdatetime.h>
#include "common.h"

float g_Sp1	= 0.0;
float g_Sp2	= 0.0;
float g_Sp3	= 0.0;
float g_Sp4	= 0.0;
float g_Sp5	= 0.0;
float g_Sp6	= 0.0;
float g_Sp7	= 0.0;
float g_Sp8	= 0.0;

static bool com5_open_flag = true;
static int com5_error_num = 0;

ComTran_Thread *pcomtran_thread;  //读取温度线程

#define PLC_TEST_NEW        0

union
{
 float sub_float;
 struct  {
	 char b[4];
	  } sep_float ;
}u_float;


ComTran_Thread::ComTran_Thread()
{
	flag_1_chanel = true;
	flag_2_chanel = true;
	flag_3_chanel = true;
	flag_4_chanel = true;
	flag_5_chanel = true;
	flag_nor_chanel = true;

	flag_Tran = true;  // 可以读取温度

	dev_id = 0;

	com5_open_flag = true;
	com5_error_num = 0;
	// 新建 定时器
	timeForGetTEMP1_1Minute = new QTimer();

	//timeForGetTEMP_1Minute->start(TIMER_LEN_1MINUTE);  // 1minute
#if DEBUD_TEST
    timeForGetTEMP1_1Minute->start(15*1000);  // 1minute
#else
    timeForGetTEMP1_1Minute->start(TEMP_TIME1_OUT);  // 1s
#endif
	//timeForGetTEMP_1Minute->stop();  // 1s

	timeForGetTEMP2_100ms = new QTimer();
	timeForGetTEMP2_100ms->stop();  // ms

	timeForGetTEMP3_100ms = new QTimer();
	timeForGetTEMP3_100ms->stop();  // ms

	timeForGetTEMP4_100ms = new QTimer();
	timeForGetTEMP4_100ms->stop();  // ms

	timeForGetTEMP5_100ms = new QTimer();
	timeForGetTEMP5_100ms->stop();  // ms

	timeForGetNor_100ms = new QTimer();
	timeForGetNor_100ms->stop();  // ms

	timeForADDR_1s = new QTimer();
	//timeForADDR_1s->start(1*1000);
	timeForADDR_1s->stop();

	// 建立连接
	connect(timeForGetTEMP1_1Minute,SIGNAL(timeout()),this,SLOT(timeUp1_1Minute()));  //
	connect(timeForGetTEMP2_100ms,SIGNAL(timeout()),this,SLOT(timeUp2_100Ms()));

	connect(timeForGetTEMP3_100ms,SIGNAL(timeout()),this,SLOT(timeUp3_100Ms()));  //
	connect(timeForGetTEMP4_100ms,SIGNAL(timeout()),this,SLOT(timeUp4_100Ms()));

	connect(timeForGetTEMP5_100ms,SIGNAL(timeout()),this,SLOT(timeUp5_100Ms()));  //
	connect(timeForGetNor_100ms,SIGNAL(timeout()),this,SLOT(timeNor_100Ms()));

	connect(timeForADDR_1s,SIGNAL(timeout()),this,SLOT(time_addr_1s()));

}

ComTran_Thread::~ComTran_Thread()
{
    //delete timeupTimer;
}

// 线程运行
void ComTran_Thread::run()
{
	//quint8 data[LEN_TEMP_DATA+1];
	//int len;
	//int ch = CMD_PID_CHANEL1;
	//msleep(10*1000); // 10S
    while(1)
    {
		msleep(1000);  // 100ms
    }
}






