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
// for test
	Get_Ch1_data();

	Get_Ch1_data();

//	Get_com5_data(0);
	pTranData->com5_send("JM\r\n", 4);
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
		msleep(100);  // 100ms
#if 1
		if(false == flag_Tran)  // 不可以读取数据
		{
			continue;
		}
        
		if(true == flag_1_chanel)
		{
			Get_Ch1_data();
			flag_1_chanel = false;
		}
		else if(true == flag_2_chanel)
		{
			Get_Ch2_data();
			flag_2_chanel = false;
			//pTranData->com6_receive_all();
		}
		else if(true == flag_3_chanel)
		{
			Get_Ch3_data();
			flag_3_chanel = false;
			//pTranData->com6_receive_all();
		}
		else if(true == flag_4_chanel)
		{
			Get_Ch4_data();
			flag_4_chanel = false;
			//pTranData->com6_receive_all();
		}
		else if(true == flag_5_chanel)
		{
			Get_Ch5_data();
			flag_5_chanel = false;
			//pTranData->com6_receive_all();
		}
		else if(true == flag_nor_chanel)
		{
			Get_Nor_data();
			flag_nor_chanel = false;
			//pTranData->com6_receive_all();
		}
#endif
#if 0  // 单独 运行
		if(WORK_MODE_AUTOTESTTING == g_workmode){
			Get_com5_data(0);
		}
#endif
    }
}
// 发送 数据 获取通道温度值  chanel 通道值
void ComTran_Thread::Send_Data_ForGetTemper(UINT8 chanel)
{
	SEND_MESSAGE_TYPE tx_data;
	qint32 checksum = 0;
	//char tx_data[CMD_GET_TEMP_SEND_DATA_LEN];

	memset(&tx_data, 0, sizeof(SEND_MESSAGE_TYPE));

	tx_data.u8DevAddr1 = CMD_PID_ADDR + chanel;
	tx_data.u8DevAddr2 = CMD_PID_ADDR + chanel;
	tx_data.u8ReqType = CMD_PID_READ;
	tx_data.u8DevPara = 0x00;
	tx_data.u8LoParam = 0x00;
	tx_data.u8HiParam = 0x00;
	checksum = chanel + (tx_data.u8DevPara << 8) + tx_data.u8ReqType + ((tx_data.u8HiParam << 8) + tx_data.u8LoParam);
	tx_data.u8LoSumCheck = checksum & 0xff;
	tx_data.u8HiSumCheck = (checksum >> 8) & 0xff;;
#if 0
	for(int i; i < sizeof(tx_data); i++)
	{
		printf("tx_data[%d] = 0x%x \r\n", i, (char *)tx_data[i]);
	}
#endif
	//printf("tx_data.u8DevAddr1 = 0x%x \r\n", tx_data.u8DevAddr1);
	//com6_send((const char *) (&tx_data),CMD_GET_TEMP_SEND_DATA_LEN);
	pTranData->com6_send((const char *) (&tx_data),CMD_GET_TEMP_SEND_DATA_LEN);


}

// 发送 数据 获取通道温度值  chanel 通道值
void ComTran_Thread::Get_Chanel_addr(UINT8 chanel)
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	printf("\n chanel == %d\r\n", chanel);
	Get_Chanel_Param(chanel, CMD_PID__GET_ADDR, 0);
	usleep(TEMP_RS485_DELEY);
#if 1
	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);

	if(len > 0)
	{
#if 0
		for(int i = 0;i < len;i ++)
		{
			printf("data[%d] = 0x%x \r\n", i, data[i]);
		}
#endif
		//len = data[1] << 8 | data[0];//(data[1]<<8 + data[0]);
		printf("pv  = %d \r\n",  (data[1]<<8 | data[0])/10);
		printf("sv  = %d \r\n",  (data[3]<<8 | data[2])/10);
		printf("param  = %d \r\n",  (data[7]<<8 | data[6]));
		dev_id = (data[7]<<8 | data[6]);
		//chanel = 0;
		Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, chanel );
		//Get_Temp(TEMP4_ID, data, len);
	}
#endif
}


// 发送 数据 获取通道温度值  chanel 通道值
void ComTran_Thread::Set_Chanel_addr(UINT8 chanel)
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	printf("\n chanel == %d\r\n", chanel);
	Set_Chanel_Param(chanel, CMD_PID__GET_ADDR, DEV_ID);
	usleep(TEMP_RS485_DELEY);
#if 1
	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);

	if(len > 0)
	{
#if 0
		for(int i = 0;i < len;i ++)
		{
			printf("data[%d] = 0x%x \r\n", i, data[i]);
		}
#endif
		//len = data[1] << 8 | data[0];//(data[1]<<8 + data[0]);
		printf("pv  = %d \r\n",  (data[1]<<8 | data[0])/10);
		printf("sv  = %d \r\n",  (data[3]<<8 | data[2])/10);
		printf("param  = %d \r\n",  (data[7]<<8 | data[6]));
		dev_id = (data[7]<<8 | data[6]);
		//chanel = 0;
		Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, chanel );
		//Get_Temp(TEMP4_ID, data, len);
	}
#endif
}


// 发送 数据 获取通道温度值  chanel 通道值
void ComTran_Thread::Get_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data)
{
	SEND_MESSAGE_TYPE tx_data;
	qint32 checksum = 0;
	//char tx_data[CMD_GET_TEMP_SEND_DATA_LEN];

	memset(&tx_data, 0, sizeof(SEND_MESSAGE_TYPE));

	//tx_data.u8DevAddr1 = CMD_PID_ADDR + chanel +dev_id;
	//tx_data.u8DevAddr2 = CMD_PID_ADDR + chanel +dev_id;
	tx_data.u8DevAddr1 = CMD_PID_ADDR + chanel;
	tx_data.u8DevAddr2 = CMD_PID_ADDR + chanel;

	tx_data.u8ReqType = CMD_PID_READ;
	tx_data.u8DevPara = param;
	tx_data.u8LoParam = data&0xff;
	tx_data.u8HiParam = (data>>8)&0xff;
	checksum = chanel + (tx_data.u8DevPara << 8) + tx_data.u8ReqType + ((tx_data.u8HiParam << 8) + tx_data.u8LoParam);
	tx_data.u8LoSumCheck = checksum & 0xff;
	tx_data.u8HiSumCheck = (checksum >> 8) & 0xff;;
#if 0
	for(int i; i < sizeof(tx_data); i++)
	{
		printf("tx_data[%d] = 0x%x \r\n", i, tx_data[i]);
	}
#endif
	//printf("tx_data.u8DevAddr1 = 0x%x \r\n", tx_data.u8DevAddr1);
	pTranData->com6_send((const char *) (&tx_data),CMD_GET_TEMP_SEND_DATA_LEN);


}



// 发送 数据 获取通道温度值  chanel 通道值
void ComTran_Thread::Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data)
{
	SEND_MESSAGE_TYPE tx_data;
	qint32 checksum = 0;
	//char tx_data[CMD_GET_TEMP_SEND_DATA_LEN];

	memset(&tx_data, 0, sizeof(SEND_MESSAGE_TYPE));

	tx_data.u8DevAddr1 = CMD_PID_ADDR + chanel ;
	tx_data.u8DevAddr2 = CMD_PID_ADDR + chanel ;
	tx_data.u8ReqType = CMD_PID_WRITE;
	tx_data.u8DevPara = param;
	tx_data.u8LoParam = data&0xff;
	tx_data.u8HiParam = (data>>8)&0xff;
	checksum = chanel + (tx_data.u8DevPara << 8) + tx_data.u8ReqType + ((tx_data.u8HiParam << 8) + tx_data.u8LoParam);
	tx_data.u8LoSumCheck = checksum & 0xff;
	tx_data.u8HiSumCheck = (checksum >> 8) & 0xff;;
#if 0
	for(int i; i < sizeof(tx_data); i++)
	{
		char *p = &tx_data;
		qDebug()<<QString("[com6]tx_data[%1] = %2 \r\n").arg(i).arg(p[i]);
	}
#endif
	//printf("tx_data.u8DevAddr1 = 0x%x \r\n", tx_data.u8DevAddr1);
	pTranData->com6_send((const char *) (&tx_data),CMD_GET_TEMP_SEND_DATA_LEN);


}

//
void ComTran_Thread::time_addr_1s()
{
	Get_Chanel_addr(CMD_PID_NOR_CHANEL);
}

// 获取通道 1的温度值
void ComTran_Thread::Get_Ch1_data()
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	Send_Data_ForGetTemper(CMD_PID_1_CHANEL);
	usleep(TEMP_RS485_DELEY);
#if 1
	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	//len = com6_receive(data, LEN_TEMP_DATA);
    //printf("\n len == %d \r\n", len);
	if(len == LEN_TEMP_DATA)
	{
		g_Sp1 = ( (data[3]<<8)|data[2])/10.0;
		//printf("g_Sp1 = %f \r\n", g_Sp1);
		Get_Temp(TEMP1_ID, data, len);
	}
	//timeForGetTEMP_100ms->start(TIMER_LEN_100MS); // 100ms 后获取 第二路
	timeForGetTEMP2_100ms->start(TEMP_TIME2_OUT); // 100ms 后获取 第二路
#endif
}

// 获取通道 2的温度值
void ComTran_Thread::Get_Ch2_data()
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	Send_Data_ForGetTemper(CMD_PID_2_CHANEL);
	usleep(TEMP_RS485_DELEY);

	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	//len = com6_receive(data, LEN_TEMP_DATA);
    //printf("\n len =2= %d \r\n", len);

	if(len == LEN_TEMP_DATA)
	{
		g_Sp2 = ((data[3]<<8)|data[2])/10.0;
		//printf("g_Sp2 = %f \r\n", g_Sp2);
		Get_Temp(TEMP2_ID, data, len);
	}

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	//Send_Data_ForGetTemper(CMD_PID_445_CHANEL);
	timeForGetTEMP2_100ms->stop(); // 停止定时
	timeForGetTEMP3_100ms->start(TEMP_TIME2_OUT); // 100ms 后获取 第二路
}
// 获取通道 3的温度值
void ComTran_Thread::Get_Ch3_data()
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	Send_Data_ForGetTemper(CMD_PID_3_CHANEL);
	usleep(TEMP_RS485_DELEY);

	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	//len = com6_receive(data, LEN_TEMP_DATA);
    //printf("\n len =3= %d \r\n", len);
	if(len == LEN_TEMP_DATA)
	{
		g_Sp3 = ( (data[3]<<8)|data[2])/10.0;
		//printf("g_Sp3 = %f \r\n", g_Sp3);
		Get_Temp(TEMP3_ID, data, len);
	}

	//Send_Data_ForGetTemper(CMD_PID_32_CHANEL);
	//Send_Data_ForGetTemper(CMD_PID_445_CHANEL);
	timeForGetTEMP3_100ms->stop(); // 停止定时
	timeForGetTEMP4_100ms->start(TEMP_TIME2_OUT); // 100ms 后获取 第二路
}
// 获取通道 4的温度值
void ComTran_Thread::Get_Ch4_data()
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	Send_Data_ForGetTemper(CMD_PID_4_CHANEL);
	usleep(TEMP_RS485_DELEY);

	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	//len = com6_receive(data, LEN_TEMP_DATA);

	if(len == LEN_TEMP_DATA)
	{
		g_Sp4 = ( (data[3]<<8)|data[2])/10.0;
//        printf("g_Sp4 = %f \r\n", g_Sp4);
		Get_Temp(TEMP4_ID, data, len);

	}
	//Send_Data_ForGetTemper(CMD_PID_nor_CHANEL);
	//Send_Data_ForGetTemper(CMD_PID_32_CHANEL);
	timeForGetTEMP4_100ms->stop(); // 停止定时
	timeForGetTEMP5_100ms->start(TEMP_TIME2_OUT);
}

// 获取通道 5的温度值
void ComTran_Thread::Get_Ch5_data()
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	Send_Data_ForGetTemper(CMD_PID_5_CHANEL);
	usleep(TEMP_RS485_DELEY);

	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	//len = com6_receive(data, LEN_TEMP_DATA);
	//printf("Get_Ch5_data == len = %d \r\n", len);
	if(len == LEN_TEMP_DATA)
	{
		g_Sp5 = ( (data[3]<<8)|data[2])/10.0;
//        printf("g_Sp5 = %f \r\n", g_Sp5);
		Get_Temp(TEMP5_ID, data, len);

	}
	//Send_Data_ForGetTemper(CMD_PID_nor_CHANEL);
	//Send_Data_ForGetTemper(CMD_PID_32_CHANEL);
	timeForGetTEMP5_100ms->stop(); // 停止定时
	timeForGetNor_100ms->start(TEMP_TIME2_OUT);
}

// 获取通道 5的温度值
void ComTran_Thread::Get_Nor_data()
{
	quint8 data[LEN_TEMP_DATA+1];

	int len = 0;

	//Send_Data_ForGetTemper(CMD_PID_365_CHANEL);
	Send_Data_ForGetTemper(CMD_PID_NOR_CHANEL);
	usleep(TEMP_RS485_DELEY);

	memset(data, 0, sizeof(data));
	len = 0;
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	//len = com6_receive(data, LEN_TEMP_DATA);
//    printf("Get_Ch6_data == len = %d \r\n", len);
	if(len == LEN_TEMP_DATA)
	{
		g_Sp6 = ( (data[3]<<8)|data[2])/10.0;
		//printf("g_Sp6 = %f \r\n", g_Sp6);
		Get_Temp(TEMP6_ID, data, len);

	}
	//Send_Data_ForGetTemper(CMD_PID_nor_CHANEL);
	//Send_Data_ForGetTemper(CMD_PID_32_CHANEL);
	timeForGetNor_100ms->stop(); // 停止定时
}


// 获取系统状态
int ComTran_Thread::Get_system_st(int bottleno)
{
	int i = 0, id = 0;
	bool flag = false;

		//bottleno  = 5;
	//printf("\n bottleno ==== %d\r\n", bottleno);

	if(bottleno < 1 || bottleno > 5)
	{
		return -1;
	}
	for(i = 0; i < MAX_ONE_GROUP_NUM; i++)
	{
		id = i + MAX_ONE_GROUP_NUM*(bottleno -1);
//		printf("\n gst_Test_Info[%d] = %d \r\n", id + TOLCOLI_START_NUM, gst_Test_Info[id + TOLCOLI_START_NUM]);
//		printf("\n gst_Test_Info[%d] = %d \r\n", id + FECCOLI_START_NUM, gst_Test_Info[id + TOLCOLI_START_NUM]);
//		printf("\n gst_Test_Info[%d] = %d \r\n", id + TPCCOLI_START_NUM, gst_Test_Info[id + TOLCOLI_START_NUM]);
//		printf("\n gst_Test_Info[%d] = %d \r\n", id + ECOLI_START_NUM, gst_Test_Info[id + TOLCOLI_START_NUM]);

		if((TEST_ST_TESTING == gst_Test_Info[id + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_TESTING == gst_Test_Info[id + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_TESTING == gst_Test_Info[id + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_TESTING == gst_Test_Info[id + ECOLI_START_NUM].Test_st)
			)
		{
			flag = true;
		}
	}

#if 0
	for(i = 0; i < MAX_TEST_NUM; i++)
	{
		if( (TEST_ST_ZERO == gst_Test_Info[i].Test_st) || (TEST_ST_TESTING == gst_Test_Info[i].Test_st))
		{
			flag = true;

			break;
		}
	}
#endif
	//printf("flag === %d \r\n", flag);
	return flag;
}
// 发送数据
bool ComTran_Thread::Send_data_to_PLC(uchar cmd, int param)
{
	unsigned char data[LEN_TEMP1_DATA+1];
	ushort crc_16 = 0xffff;
	int bit = 0;
	//float temp_nondu = 0.0;
	int bottleid = 0;


	memset(data, 0, sizeof(data));

	data[bit++] = PLC_CMD_HEAD_1;
	data[bit++] = PLC_CMD_HEAD_2;

	switch(cmd)
	{
		case PLC_CMD_GET_PARAM:

		break;

		case PLC_CMD_GET_ST:
			data[bit++] = PLC_CMD_GET_ST; // cmd
			data[bit++] = 0;// 数据长度
			data[bit++] = 2;// 数据长度
			data[bit++] = param;//  瓶号
			if(true == Get_system_st(param))
			{
				data[bit++] = 0x02;// 数据 测量
			}
			else if(-1 == Get_system_st(param))
			{
				data[bit++] = 0x03;// 参数错误
			}
			else
			{
				data[bit++] = 0x01;// 数据 待机
			}

		break;

		case PLC_CMD_GET_AD:
		break;

		case PLC_CMD_GET_IN_PARAM:
		break;

		case PLC_CMD_GET_SYSYTEM_PARAM:
		break;

		case PLC_CMD_GET_TEMP:
		break;

		case PLC_CMD_GET_NONGDU:
			bottleid = param -1;
			data[bit++] = PLC_CMD_GET_NONGDU; // cmd
			data[bit++] = 0;// 数据长度
			data[bit++] = 1;// 数据长度
//			qDebug()<<" botle id == "<<bottleid;
			if((bottleid >= 0) && (bottleid < BOTTLE_MAX_NUM))
			{
                //data[bit++] = PLC_TIME_FLAG;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].year>>8;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].year&0xff;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].mon;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].day;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].hour;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].min;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].sec;// 时间

				if(FEC_COLI_ID == g_work_Test_type[bottleid]) // 测试耐热
				{
					data[bit++] = PLC_FEC_NONGDU_FLAG;//耐热浓度
					//nongdu.sub_float = gst_His_Info_to_plc[param].NongD;
					u_float.sub_float = gst_His_Info_to_plc[bottleid].NongD;
					data[bit++] = (uchar)u_float.sep_float.b[3];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[2];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[1];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[0];//耐热浓度
#if PLC_TEST_NEW
					data[bit++] = PLC_TOL_NONGDU_FLAG;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
					data[bit++] = 0;//总大肠浓度

					data[bit++] = PLC_E_NONGDU_FLAG;//埃希氏浓度
					data[bit++] = 0;//埃希氏浓度
					data[bit++] = 0;//埃希氏浓度
					data[bit++] = 0;//埃希氏浓度
					data[bit++] = 0;//埃希氏浓度

                    data[bit++] = PLC_FEC_TIME_FLAG;//耐热 因子
#endif
					data[bit++] = (gst_His_Info_to_plc[bottleid].test_len >> 8)&0xff;//耐热 因子
					data[bit++] = gst_His_Info_to_plc[bottleid].test_len&0xff;//耐热 因子
#if PLC_TEST_NEW
					data[bit++] = PLC_TOL_TIME_FLAG;//总大肠 因子
					data[bit++] = 0;//总大肠 因子
					data[bit++] = 0;//总大肠 因子

					data[bit++] = PLC_E_TIME_FLAG;//埃希氏 因子
					data[bit++] = 0;//埃希氏 因子
					data[bit++] = 0;//埃希氏 因子
#endif
				}
				else if(TOL_COLI_ID == g_workmode)  // 总大肠
				{
#if PLC_TEST_NEW
					data[bit++] = PLC_FEC_NONGDU_FLAG;//耐热浓度
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;
#endif
					data[bit++] = PLC_TOL_NONGDU_FLAG;//总大肠
					u_float.sub_float = gst_His_Info_to_plc[bottleid].NongD;
					data[bit++] = (uchar)u_float.sep_float.b[3];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[2];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[1];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[0];//耐热浓度
#if PLC_TEST_NEW
					data[bit++] = PLC_E_NONGDU_FLAG;//埃希氏浓度
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;

					data[bit++] = PLC_FEC_TIME_FLAG;// 耐热 因子
					data[bit++] = 0;// 因子
					data[bit++] = 0;// 因子

					data[bit++] = PLC_TOL_TIME_FLAG;//总大肠 因子
#endif
                    data[bit++] = (gst_His_Info_to_plc[bottleid].test_len >> 8)&0xff;// 因子
					data[bit++] = gst_His_Info_to_plc[bottleid].test_len&0xff;// 因子
#if PLC_TEST_NEW
					data[bit++] = PLC_E_TIME_FLAG;//埃希氏 因子
					data[bit++] = 0;// 因子
					data[bit++] = 0;// 因子
#endif
				}
				else if(E_COLI_ID == g_workmode)
				{
#if PLC_TEST_NEW
					data[bit++] = PLC_FEC_NONGDU_FLAG;//耐热浓度
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;

					data[bit++] = PLC_TOL_NONGDU_FLAG;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
					data[bit++] = 0;//总大肠浓度
#endif
					data[bit++] = PLC_E_NONGDU_FLAG;//埃希氏浓度
					u_float.sub_float = gst_His_Info_to_plc[bottleid].NongD;
					data[bit++] = (uchar)u_float.sep_float.b[3];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[2];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[1];//耐热浓度
					data[bit++] = (uchar)u_float.sep_float.b[0];//耐热浓度
#if PLC_TEST_NEW
					data[bit++] = PLC_FEC_TIME_FLAG;//耐热 因子
					data[bit++] = 0;// 因子
					data[bit++] = 0;//因子

					data[bit++] = PLC_TOL_TIME_FLAG;//总大肠 因子
					data[bit++] = 0;//总大肠 因子
					data[bit++] = 0;//总大肠 因子

					data[bit++] = PLC_E_TIME_FLAG;//埃希氏 因子
#endif
                    data[bit++] = (gst_His_Info_to_plc[bottleid].test_len >> 8)&0xff;// 因子
					data[bit++] = gst_His_Info_to_plc[bottleid].test_len&0xff;// 因子

				}
                //data[bit++] = PLC_BOTTLE_FLAG;// 瓶号
				data[bit++] = gst_His_Info_to_plc[bottleid].bt_id;// 瓶号

                //data[bit++] = PLC_UNIT_FLAG;// 单位
				data[bit++] = gst_His_Info_to_plc[bottleid].unit_n;// 瓶号

				data[3] = ((bit - 3)>>8)*0xff;// 长度  去除 头
				data[4] = (bit-3)&0xff;
			}

		break;

		case PLC_CMD_SET_IN_PARAM:
		break;

		case PLC_CMD_SET_PARAM:
		break;

		case PLC_CMD_SET_TEMP:
		break;

		case PLC_CMD_SET_TIME:
			data[bit++] = PLC_CMD_SET_TIME; // cmd
			data[bit++] = 0;// 数据长度
			data[bit++] = 1;// 数据长度
			data[bit++] = PLC_SET_OK;// ok

		break;

		case PLC_CMD_SET_START:
			data[bit++] = PLC_CMD_SET_START; // cmd
			data[bit++] = 0;// 数据长度
			data[bit++] = 1;// 数据长度
			data[bit++] = PLC_SET_OK;// ok
		break;


		default:
			break;

	}
	crc_16 = CRC16( data + 2, bit - 2);
	data[bit++] = (crc_16 >> 8)&0xff ;// 数据长度
	data[bit++] = crc_16&0xff ;// 数据长度
#if 1
	for(int j = 0; j < bit; j++)
	{
		printf("send data[%d] = 0x%x \r\n", j, data[j]);
	}
#endif
	pTranData->com5_send((char *)data, bit);
	//usleep(500*1000);
#if 0  //
	if(Get_com5_data(data[2]))
	{
		Send_data_to_PLC(data[2], param);
	}
#endif

	return true;

}

//获取 com5的数据
bool ComTran_Thread::Get_com5_data(uchar cmd)
{
	quint8 data[LEN_TEMP1_DATA+1];
	int len = 0;
	ushort cheaksum = 0;
	uchar data_cmd = 0;
	ushort data_crc = 0;
	int data_len = 0;

	memset(data,0, sizeof(data));
	len = pTranData->com5_receive(data, LEN_TEMP1_DATA);
	if(len > 0)
	{

		if((PLC_CMD_HEAD_1== data[0]) && (PLC_CMD_HEAD_2 == data[1]))
		{
#if 1
	for(int i = 0; i < len; i++)
	{
		printf("data[%d] == 0x%x \r\n", i, data[i]);
	}
		printf("\r\n \r\n");
#endif

			data_crc = data[len-2]<<8|data[len-1];
			cheaksum = CRC16(data+2,  len -4);
			data_cmd = data[2];  // 命令
			data_len = (data[3] >> 8)|data[4]; // 数据长度
//			printf("data_len === %d", data_len);
//			printf("data_crc === 0x%x cheaksum = 0x%x \r\n", data_crc, cheaksum);
			if(data_crc == cheaksum)  // 测试 时为1
			{
				//QDateTime tpast;// = QDateTime::fromMSecsSinceEpoch(time_len);
				//QString temp;
				//uchar data_type = 0; // 数据类型
				//ulong time_len = 0;
				int bootle_no = 0;
				int param_type = 0;
				ushort year = 0;
				uchar mon = 0, day = 0, hour = 0, min = 0, sec = 0;

				uchar test_type = 0;

				switch(data_cmd)
				{
					case PLC_CMD_GET_PARAM:

					break;

					case PLC_CMD_GET_ST:
						bootle_no = data[5];
						if(cmd == PLC_CMD_GET_ST){
							return true;
						}
						else
						{
							//printf("\n data[5] === 0x%x\r\n", data[5]);
							Send_data_to_PLC(PLC_CMD_GET_ST, bootle_no);
						}

					break;

					case PLC_CMD_GET_AD:		// 获取AD值
					break;

					case PLC_CMD_GET_IN_PARAM:	// 获取内部参数
					break;

					case PLC_CMD_GET_SYSYTEM_PARAM:  // 获取系统参数
					break;

					case PLC_CMD_GET_TEMP:			// 获取温度
					break;

					case PLC_CMD_GET_NONGDU:   // 获取浓度

						bootle_no = data[5];
						Send_data_to_PLC(PLC_CMD_GET_NONGDU, bootle_no);
					break;

					case PLC_CMD_SET_IN_PARAM:  // 设置内部参数
					break;

					case PLC_CMD_SET_PARAM:  // 设置参数
					break;

					case PLC_CMD_SET_TEMP:  // 设置温度

					break;

					case PLC_CMD_SET_TIME:  // 设置时间
#if 0
						data_type = data[5];
						if(PLC_DATA_TYPE_32B == data_type)  // 4个字节
						{
							time_len = data[7]<<24|data[8]<<16|data[9]<<8|data[10];
							printf("time_len === 0x%lx \r\n", time_len);
						}

						temp = tpast.fromTime_t(time_len, Qt::LocalTime).toString("yyyy-MM-dd hh:mm:ss[:zzz]");
						//QDateTime::fromMSecsSinceEpoch(m_maxx, Qt::UTC);
						qDebug() << "time(time_len):" << temp;
						// 时间设置
						set_system_time(tpast.fromTime_t(time_len, Qt::LocalTime).toString("yyyy").toInt(),
										tpast.fromTime_t(time_len, Qt::LocalTime).toString("MM").toInt(),
										tpast.fromTime_t(time_len, Qt::LocalTime).toString("dd").toInt(),
										tpast.fromTime_t(time_len, Qt::LocalTime).toString("hh").toInt(),
										tpast.fromTime_t(time_len, Qt::LocalTime).toString("mm").toInt(),
										tpast.fromTime_t(time_len, Qt::LocalTime).toString("ss").toInt());
#else
						year = data[5]<<8 | data[6];  //年
						mon = data[7];
						day = data[8];

						hour = data[9];
						min = data[10];
						sec = data[11];

						set_system_time( year, mon, day, hour, min , sec);
#endif
						Send_data_to_PLC(PLC_CMD_SET_TIME, data[5]);

					break;

					case PLC_CMD_SET_START:  // 开始停止
						bootle_no = data[8];		// 瓶号
						test_type = data[6];  // 菌的种类
						param_type = data[5];
						//printf("bootle_no == %d test_type = %d\r\n", bootle_no, test_type);
						if(PLC_START_TESTTING == param_type)  // 开始测试
						{
							int id = 0;
							memset(&(gst_His_Info_to_plc[bootle_no-1]), 0, sizeof(ST_HISTORY_DATA));
							for(int i = 0; i < MAX_ONE_GROUP_NUM; i++)
							{

								id = i + MAX_ONE_GROUP_NUM*(bootle_no -1);

								if(PLC_TOLCOIL_ID == test_type)
								{
									g_work_Test_type[bootle_no-1] = TOL_COLI_ID;
                                    qDebug()<<"com传输****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									gst_Test_Info[id + TOLCOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + TOLCOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+TOLCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								else if(PLC_FECCOIL_ID == test_type)
                                {
                                     g_work_Test_type[bootle_no-1] = FEC_COLI_ID;
                                     qDebug()<<"com传输1****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									//g_work_Test_type = FEC_COLI_ID;
									gst_Test_Info[id + FECCOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + FECCOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+FECCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								else if(PLC_TPCCOIL_ID == test_type)
								{
									g_work_Test_type[bootle_no-1] = TPC_COLI_ID;
                                    qDebug()<<"com传输2****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									gst_Test_Info[id + TPCCOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + TPCCOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+TPCCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								else if(PLC_ECOIL_ID == test_type)
								{
									g_work_Test_type[bootle_no-1] = E_COLI_ID;
                                     qDebug()<<"com传输3****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									gst_Test_Info[id + ECOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + ECOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+ECOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
#if 0
								printf("\n gst_Test_Info[%d] = %d \r\n", id + TOLCOLI_START_NUM, gst_Test_Info[id + TOLCOLI_START_NUM].Test_st);
								printf("\n gst_Test_Info[%d] = %d \r\n", id + FECCOLI_START_NUM, gst_Test_Info[id + FECCOLI_START_NUM].Test_st);
								printf("\n gst_Test_Info[%d] = %d \r\n", id + TPCCOLI_START_NUM, gst_Test_Info[id + TPCCOLI_START_NUM].Test_st);
								printf("\n gst_Test_Info[%d] = %d \r\n", id + ECOLI_START_NUM, gst_Test_Info[id + ECOLI_START_NUM].Test_st);
#endif
							}

						}
						else if(PLC_STOP_TESTTING == data[5])  // 停止测试
						{
							for(int i = 0; i < MAX_TEST_NUM; i++)
							{
								gst_Test_Info[i].Test_st = TEST_ST_IDEL;
								memset(&(gst_His_Info[i]), 0, sizeof(ST_HISTORY_DATA));
							}
							memset(&(gst_His_Info_to_plc), 0, sizeof(ST_HISTORY_DATA)*BOTTLE_MAX_NUM);
							memset(g_work_Test_type, 0 , sizeof(g_work_Test_type));

						}
						Send_data_to_PLC(PLC_CMD_SET_START, bootle_no);
					break;


					default:
						break;
				}
			}
		}
	}

	return true;
}

// 时间轮训 读数据 1 minute 定时
void ComTran_Thread::timeUp1_1Minute()
{
	pTranData->com6_receive_all();
	flag_1_chanel = true;

	if(true == com5_open_flag)
	{
		com5_open_flag = false;
		com5_error_num = 0;
	}
	else
	{
		com5_error_num ++;
	}

    if(com5_error_num > 10)  // 10分钟数据错误
	{
		pTranData->com6_close();
		usleep(300*1000);  // 300ms
		pTranData->com6_open();
		com5_error_num = 0;
		com5_open_flag = true;

		//设定本机IP
//		QString reboot ="reboot";
//		system(QstringTostring(reboot));
    }
//	qDebug()<<"timeUp1_1Minute ==== \r\n";
}

// 时间轮训 读数据 100Ms定时
void ComTran_Thread::timeUp2_100Ms()
{
	pTranData->com6_receive_all();
	flag_2_chanel = true;
}

// 时间轮训 读数据 100Ms定时
void ComTran_Thread::timeUp3_100Ms()
{
	pTranData->com6_receive_all();
	flag_3_chanel = true;
}

// 时间轮训 读数据 100Ms定时
void ComTran_Thread::timeUp4_100Ms()
{
	pTranData->com6_receive_all();
	flag_4_chanel = true;
}
// 时间轮训 读数据 100Ms定时
void ComTran_Thread::timeUp5_100Ms()
{
	pTranData->com6_receive_all();
	flag_5_chanel = true;
}
// 时间轮训 读数据 100Ms定时
void ComTran_Thread::timeNor_100Ms()
{
	pTranData->com6_receive_all();
	flag_nor_chanel = true;
}



// 解析温度值 并保存

bool ComTran_Thread::Get_Check_Sum(RECEIVE_MESSAGE_TYPE *receive,quint8 addr)
{
	quint16  u16CrcVal = 0;

	quint16 u16TempCheck;
	bool bResult = false;

	u16TempCheck = (receive->u8HiSumCheck << 8) + receive->u8LoSumCheck;
//    u8CrcVal = ((stAnsFrm.u8HiPV + stAnsFrm.u8LoPV + stAnsFrm.u8HiSV + stAnsFrm.u8LoSV +
//                 stAnsFrm.u8LoMV + stAnsFrm.u8HiMV + stAnsFrm.u8HiParam + stAnsFrm.u8LoParam +
//                 stAnsFrm.u8ALm + u8TempCheck) % 256) / 16;

//    u8CalCrcVal = stAnsFrm.u8SumCheck / 16;
	u16CrcVal = ((receive->u8HiPV << 8) + receive->u8LoPV)
				+((receive->u8HiSV << 8) + receive->u8LoSV)
				+((receive->u8ALm << 8) + receive->u8MV)
				+((receive->u8HiParam << 8) + receive->u8LoParam) + addr;

	//qDebug()<<QString("u16TempCheck = 0x%x")
	//printf("u16TempCheck = 0x%x   u16CrcVal = 0x%x addr = %d\r\n", u16TempCheck, u16CrcVal, addr);
	//printf("u16TempCheck - u16CrcVal = %d addr = %d\r\n", (u16TempCheck - u16CrcVal), addr);
	if(u16TempCheck == u16CrcVal)
	{
		bResult = true;
	}
	//bResult = true;

	return bResult;
}


bool ComTran_Thread::Get_Temp(int mode, quint8 *data, int len)
{
	int i;
	//long crc;
	float t = TEMP_ERROR;

//	static QMutex mutex;
//	mutex.lock();  // 加锁保护
//	qDebug()<<QString(" mode = %1 len = %2\r\n").arg(mode).arg(len);
	if(CMD_GET_TEMP_DATA_LEN == len)
	{
#if 0
		crc = 0;
		for(i = 0;i < 7;i += 2)
		{
			crc += (data[i+1] << 8) | data[i];
		}
#if 1
		if(TEMP1_ID == mode){
			crc = crc + CMD_PID_365_CHANEL ;
		}
		else if(TEMP2_ID == mode)
		{
			crc += CMD_PID_445_CHANEL ;
		}
		else if(TEMP3_ID == mode)
		{
			crc += CMD_PID_32_CHANEL ;
		}
		else if(TEMP4_ID == mode)
		{
			crc += CMD_PID_nor_CHANEL ;
		}
#endif
		// crc 值在最后两个字节

		if(crc == ((data[CMD_GET_TEMP_DATA_LEN-1] << 8)|data[CMD_GET_TEMP_DATA_LEN-2]))
		{
			t = ( (data[1]<<8)|data[0])/10.0;
			//qDebug()<<QString(" temp = %1").arg(t);
		}
		else
		{
			printf("crc = 0x%x crc1 = 0x%x\r\n", crc, ((data[CMD_GET_TEMP_DATA_LEN-1] << 8)|data[CMD_GET_TEMP_DATA_LEN-2]));
		}
#else
		if(TEMP1_ID == mode){
			if(true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, CMD_PID_1_CHANEL))
			{
				t = ( (data[1]<<8)|data[0])/10.0;
			}
		}
		else if(TEMP2_ID == mode)
		{
			if(true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, CMD_PID_2_CHANEL))
			{
				t = ( (data[1]<<8)|data[0])/10.0;
			}
		}
		else if(TEMP3_ID == mode)
		{
			if(true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, CMD_PID_3_CHANEL))
			{
				t = ( (data[1]<<8)|data[0])/10.0;
			}
		}
		else if(TEMP4_ID == mode)
		{
			if(true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, CMD_PID_4_CHANEL))
			{
				t = ( (data[1]<<8)|data[0])/10.0;
			}
		}
		else if(TEMP5_ID == mode)
		{
			if(true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, CMD_PID_5_CHANEL))
			{
				t = ( (data[1]<<8)|data[0])/10.0;
			}
		}
		else if(TEMP6_ID == mode)
		{
			if(true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, CMD_PID_NOR_CHANEL))
			{
				t = ( (data[1]<<8)|data[0])/10.0;
			}
		}

#endif
	}
	else
	{
		for(i = 0;i < len;i ++)
		{
			printf("data[%d] = 0x%x \r\n", i, data[i]);
		}
		return false;
	}

	if(t == TEMP_ERROR)
	{
		return false;
	}

	ST_HISTORY_DATA st_his;
	QDateTime time= QDateTime::currentDateTime();
	QString strDate= time.toString("yyyy-MM-dd hh:mm:ss");
//	QString strTime= time.toString("hh:mm:ss");
	st_his.SampleID = strDate;
	//qDebug()<<QString("mode = %1 temp = %2").arg(mode).arg(t);
	if(TEMP1_ID == mode)
	{
		st_his.NongDu = QString::number(t);
		gp_sql->Add_One_Data(mode,&st_his);
		g_temp1 = t;  //保存最新的温度
		com5_open_flag = true;
	}
	else if(TEMP2_ID == mode)
	{
		st_his.NongDu = QString::number(t);
		gp_sql->Add_One_Data(mode,&st_his);
		g_temp2 = t; //保存最新的温度
	}
	else if(TEMP3_ID == mode)
	{
		st_his.NongDu = QString::number(t);
		gp_sql->Add_One_Data(mode,&st_his);

		g_temp3 = t;  //保存最新的温度
	}
	else if(TEMP4_ID == mode)
	{
		st_his.NongDu = QString::number(t);
		gp_sql->Add_One_Data(mode,&st_his);
		g_temp4 = t;  //保存最新的温度
	}
	else if(TEMP5_ID == mode)
	{
		st_his.NongDu = QString::number(t);
		gp_sql->Add_One_Data(mode,&st_his);
		g_temp5 = t; //保存最新的温度
	}
	else if(TEMP6_ID == mode)
	{
		st_his.NongDu = QString::number(t);
		gp_sql->Add_One_Data(mode,&st_his);
		g_temp_nor = t; //保存最新的温度
	}

	//mutex.unlock();  // 加锁保护
	if(-100 == t)
	{
		return false;
	}
	else{
		return true;
	}
}

#if 0
union
{
 int sub_double;
 struct  {
     char l[4];
      } sep_double ;
}u_double;

/*
    函数名称：
    函数功能：将4个字节转换成int
    输入参数：
    输出参数：
    修改记录：
*/
double ComTran_Thread::char4ToDouble(char *arrary, float write_data)
{
    u_double.sep_double.l[0] = arrary[3];
    u_double.sep_double.l[1] = arrary[2];
    u_double.sep_double.l[2] = arrary[1];
    u_double.sep_double.l[3] = arrary[0];

    write_data = u_double.sub_double;
    return write_data;
}
#endif
