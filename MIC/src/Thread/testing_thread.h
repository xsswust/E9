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
 //   bool CalibrateFlag;//校正标志
	bool Check_SelfSt(ST_TEST_INFO *pSt_TestInfo, unsigned int num);  // 自检
	void Zero_Calibration(ST_TEST_INFO *pSt_TestInfo);// 较零
	void ThruRate_Calculate(ST_TEST_INFO *pSt_TestInfo); // 透过率 计算
	void ThouData_Smooth(ST_TEST_INFO *pSt_TestInfo);	// 透过率平滑处理
	bool Check_OverTime(void);		//检查是否已经可以计算浓度
	void Time_Calculate(ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_ParaSetting);	//时间校正
	void Titer_Cal(int mode, ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_ParaSetting);  // 浓度计算
	void Save_Data(int num , double dat, ST_TEST_INFO *pSt_TestInfo, ST_PARA_SETTING *pst_Parasetting);  // 保存数据
	void Testing();  //测试
    void Get_Param(int mode, ST_PARA_SETTING *st);
    void Get_Param1(int mode, ST_PARA_SETTING *st,int Bottle_ClassNo,int Bottle_ClassNo_1);

	void E_Test_Cal(ST_TEST_INFO *pSt_TestInfo);
    void setHz();
    void setchar();

   long long GetMpnVal(int a, int b, int c);
 //   void CalcMethodChoose(ST_TEST_INFO *gst_Test_Info,int i,ST_PARA_SETTING *st_temp);
    void CalcMethodChoose(ST_TEST_INFO *gst_Test_Info,int i,ST_PARA_SETTING st_temp);
public slots:
	void timeUp_Testing();

private:
	QTimer *time_Testing;
};

extern testing_thread *ptesting_thread;
#endif // TESTING_THREAD_H
