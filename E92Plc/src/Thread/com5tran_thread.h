#ifndef COM5TRAN_THREAD_H
#define COM5TRAN_THREAD_H

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

#define TEMP_TIME1_OUT	TIMER_LEN_1MINUTE
#define TEMP_TIME2_OUT	TIMER_LEN_1S


class Com5Tran_Thread : public QThread
{
    Q_OBJECT
public:
	Com5Tran_Thread();
	virtual ~Com5Tran_Thread();
    void run();    
    double char4ToDouble(char *arrary, float write_data);
	bool Get_Temp(int mode, quint8 *data, int len); // 解析温度值
    //void Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data);

	bool Get_Check_Sum(RECEIVE_MESSAGE_TYPE *receive,quint8 addr);
	void Get_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data);
	void Get_Chanel_addr(UINT8 chanel);
	void Set_Chanel_addr(UINT8 chanel);
    void Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data);//设置温区温度
    void Set_Bottle_Temp(UINT8 bottleId, UINT8 coilType);//设置屏温度



	//void Get_com5_data();  //获取通道4数据
	bool Get_com5_data(uchar cmd);
	bool Send_data_to_PLC(uchar cmd, int param);  // 发送数据
	int Get_system_st(int bottleno);   // 获取系统状态


public slots:
	void sys_Save_St();  // 保存系统状态

private:

	QTimer *sys_save_st;
    
};
extern Com5Tran_Thread *pcom5tran_thread;
#endif // COMTRAN_THREAD_H
