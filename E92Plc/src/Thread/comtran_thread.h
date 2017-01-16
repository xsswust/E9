#ifndef COMTRAN_THREAD_H
#define COMTRAN_THREAD_H

#include "src/HardWare/posix_qextserialport.h"
#include "src/HardWare/qextserialbase.h"
#include <QTimer>
#include <QFile>
#include <QThread>
#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "common.h"
#include "src/HardWare/com_trandata.h"

#if DEBUD_TEST
#define TEMP_TIME1_OUT	TIMER_LEN_1MINUTE//TIMER_LEN_1MINUTE
#else
#define TEMP_TIME1_OUT	TIMER_LEN_1MINUTE  //TIMER_LEN_1MINUTE
#endif

#define TEMP_TIME2_OUT	TIMER_LEN_1S
#if 0
#define CMD_PID_365_CHANEL		(CMD_PID_CHANEL2 -1)  // 获取36。5 通道温度
#define CMD_PID_445_CHANEL		(CMD_PID_CHANEL3 -1)	// 获取44。5 通道温度
#define CMD_PID_32_CHANEL		(CMD_PID_CHANEL4 -1)  // 获取36。5 通道温度
#define CMD_PID_nor_CHANEL		(CMD_PID_CHANEL1 -1)	// 获取44。5 通道温度
#else
#define CMD_PID_1_CHANEL		CMD_PID_CHANEL1   // 获取36。5 通道温度
#define CMD_PID_2_CHANEL		CMD_PID_CHANEL2 	// 获取44。5 通道温度
#define CMD_PID_3_CHANEL		CMD_PID_CHANEL3   // 获取36。5 通道温度
#define CMD_PID_4_CHANEL		CMD_PID_CHANEL4 	// 获取44。5 通道温度
#define CMD_PID_5_CHANEL		CMD_PID_CHANEL5   // 获取36。5 通道温度
#define CMD_PID_NOR_CHANEL		CMD_PID_CHANEL6 	// 获取44。5 通道温度

#endif






#define DEV_ID			1   // 设备号



#define TEMP_ERROR		-1000  // 温度读取错误


class ComTran_Thread : public QThread
{
    Q_OBJECT
public:
    ComTran_Thread();
    virtual ~ComTran_Thread();
    void run();    
	void Send_Data_ForGetTemper(UINT8 chanel); // 发送获取温度通道的命令
    double char4ToDouble(char *arrary, float write_data);
	bool flag_Tran;   // 标志 true 可以发送  false 不可以发送数据

	int flag_1_chanel;   // 标志 开始获取36.5 通道温度
	int flag_2_chanel;   //标志 开始获取44.5 通道温度
	int flag_3_chanel;   // 标志 开始获取36.5 通道温度
	int flag_4_chanel;   //标志 开始获取44.5 通道温度
	int flag_5_chanel;   // 标志 开始获取36.5 通道温度
	int flag_nor_chanel;   //标志 开始获取44.5 通道温度
	int dev_id; // 设备号

	bool Get_Temp(int mode, quint8 *data, int len); // 解析温度值
	void Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data);

	bool Get_Check_Sum(RECEIVE_MESSAGE_TYPE *receive,quint8 addr);
	void Get_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data);
	void Get_Chanel_addr(UINT8 chanel);
	void Set_Chanel_addr(UINT8 chanel);
	void Get_Ch1_data();  //获取通道1数据
	void Get_Ch2_data();  //获取通道2数据
	void Get_Ch3_data();  //获取通道3数据
	void Get_Ch4_data();  //获取通道4数据
	void Get_Ch5_data();  //获取通道4数据
	void Get_Nor_data();  //获取通道4数据

	//void Get_com5_data();  //获取通道4数据
	bool Get_com5_data(uchar cmd);
	bool Send_data_to_PLC(uchar cmd, int param);  // 发送数据
	int Get_system_st(int bottleno);   // 获取系统状态

#if 0
// for com
	QSerialPort *g_pCom6;
	void com6_send(const char *c,int len);
	int com6_receive(quint8 *tmp_data, int l);

	qint32 BaudRate;
	QSerialPort::DataBits DataBit;
	QSerialPort::Parity Parity;
	QSerialPort::StopBits StopBit;
	QSerialPort::FlowControl FlowControl;
#endif
// end for com

public slots:
	void timeUp1_1Minute();
	void timeUp2_100Ms();
	void timeUp3_100Ms();
	void timeUp4_100Ms();
	void timeUp5_100Ms();
	void timeNor_100Ms();
	void time_addr_1s();


private:
	QTimer *timeForGetTEMP1_1Minute;
	QTimer *timeForGetTEMP2_100ms;
	QTimer *timeForGetTEMP3_100ms;
	QTimer *timeForGetTEMP4_100ms;
	QTimer *timeForGetTEMP5_100ms;
	QTimer *timeForGetNor_100ms;
	QTimer *timeForADDR_1s;

    
};
extern ComTran_Thread *pcomtran_thread;
#endif // COMTRAN_THREAD_H
