#include "com5tran_thread.h"
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

static int g_connet_flag = true;  // 用于判断通讯是否正常

static int g_connet_num = 0;  // 记录通讯中断时间

Com5Tran_Thread *pcom5tran_thread;

union
{
 float sub_float;
 struct  {
	 char b[4];
	  } sep_float ;
}u_float;


Com5Tran_Thread::Com5Tran_Thread()
{
	// 定义 timer
	sys_save_st = new QTimer();
	sys_save_st->stop();

	// 注意 ; 此处不可少 否则无法 发送数据
	pTranData->com5_send("JM\r\n", 4);

#if HAVE_PRINTF
    pTranData->com3_send("\r\n", 2);
#endif

	connect(sys_save_st, SIGNAL(timeout()), this, SLOT(sys_Save_St()));

	sys_save_st->start(TIMER_LEN_1MINUTE);  // 1分钟记录一次数据
}

Com5Tran_Thread::~Com5Tran_Thread()
{
    //delete timeupTimer;
}

// 线程运行
void Com5Tran_Thread::run()
{
	//quint8 data[LEN_TEMP_DATA+1];
	//int len;
	//int ch = CMD_PID_CHANEL1;
	msleep(10*1000); // 10S
    while(1)
    {
		msleep(100);  // 100ms
		if(WORK_MODE_AUTOTESTTING == g_workmode){
			Get_com5_data(0);
		}

    }
}
// 保存数据
void Com5Tran_Thread::sys_Save_St()
{
	QFile file(FILE_SYS_ST);

	file.open(QIODevice::WriteOnly);

	file.seek(0);
	// 保存状态
	file.write((char*)&g_test_st, sizeof(g_test_st));
	file.write((char*)&g_Test_Over_Time, sizeof(g_Test_Over_Time));
    file.write((char*)&g_mpn_test_st, sizeof(g_mpn_test_st));
    file.write((char*)&g_mpn_test_mui, sizeof(g_mpn_test_mui));
    file.write((char*)&g_mpn_test_val, sizeof(g_mpn_test_val));
    printf("\n<><><><><><><>< g_test_st ===== %d \r\n", g_test_st);
    printf("\n[sys_Save_St] g_mpn_test_st == %d g_mpn_test_mui = %d", g_mpn_test_st, g_mpn_test_mui);
	file.write((char*)&gst_Test_Info, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
    file.write((char*)&pSt_CalibrateInfo, sizeof(ST_TEST_INFO)*MAX_TEST_NUM);
	file.write((char*)&gst_His_Info_to_plc, sizeof(ST_HISTORY_DATA)*BOTTLE_MAX_NUM);
	file.write((char*)&g_work_Test_type, sizeof(g_work_Test_type));
	file.write((char*)&gst_His_Info, sizeof(gst_His_Info));


	QDateTime time= QDateTime::currentDateTime();
	QString strDate= time.toString("yyyy-MM-dd hh:mm:ss");
	QByteArray ba = strDate.toLatin1();

	file.write(ba.data(), strlen(ba.data()));

	file.close();
	// 在线模式
	if(WORK_MODE_AUTOTESTTING == g_workmode)
	{
		if(false == g_connet_flag)
		{
			g_connet_num ++;
		}
		else
		{
			g_connet_flag = false;
			g_connet_num = 0;
		}
		printf("\n g_connet_flag = %d  g_connet_num == %d \r\n", g_connet_flag, g_connet_num);
		if( g_connet_num >= 5) // 5分钟 中断
		{

			pTranData->com5_close();
			//pTranData->com6_close();
			usleep(300*1000);  // 300ms
			pTranData->com5_open();
			//pTranData->com6_open();

			g_connet_num = 0;
			g_connet_flag = true;
#if 0
			QString reboot ="reboot";
			system(QstringTostring(reboot));
#endif
		}
	}
}

// 获取系统状态
int Com5Tran_Thread::Get_system_st(int bottleno)
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
bool Com5Tran_Thread::Send_data_to_PLC(uchar cmd, int param)
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
            qDebug()<<"## botle id == "<<bottleid;
			if((bottleid >= 0) && (bottleid < BOTTLE_MAX_NUM))
			{
#if PLC_TEST_NEW
				data[bit++] = PLC_TIME_FLAG;// 时间
#endif
				data[bit++] = gst_His_Info_to_plc[bottleid].year>>8;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].year&0xff;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].mon;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].day;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].hour;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].min;// 时间
				data[bit++] = gst_His_Info_to_plc[bottleid].sec;// 时间
                qDebug()<<"## g_work_Test_type[bottleid]  == "<<g_work_Test_type[bottleid];
				if(FEC_COLI_ID == g_work_Test_type[bottleid]) // 测试耐热
				{
#if PLC_TEST_NEW
					data[bit++] = PLC_FECCOIL_ID;
#endif
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

					data[bit++] = PLC_TPC_NONGDU_FLAG;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
\
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

					data[bit++] = PLC_TPC_TIME_FLAG;//菌落总数因子
					data[bit++] = 0;//菌落总数 因子
					data[bit++] = 0;//菌落总数 因子
#endif
				}
                else if(TOL_COLI_ID == g_work_Test_type[bottleid])  // 总大肠
				{
#if PLC_TEST_NEW
					data[bit++] = PLC_TOLCOIL_ID;
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

					data[bit++] = PLC_TPC_NONGDU_FLAG;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度

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

					data[bit++] = PLC_TPC_TIME_FLAG;//菌落总数因子
					data[bit++] = 0;//菌落总数 因子
					data[bit++] = 0;//菌落总数
#endif
				}
				else if(E_COLI_ID == g_work_Test_type[bottleid])  // 埃希氏
				{
#if PLC_TEST_NEW
					data[bit++] = PLC_ECOIL_ID;
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
					data[bit++] = PLC_TPC_NONGDU_FLAG;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度
					data[bit++] = 0;//菌落总数浓度

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
#if PLC_TEST_NEW
					data[bit++] = PLC_TPC_TIME_FLAG;//菌落总数因子
					data[bit++] = 0;//菌落总数 因子
					data[bit++] = 0;//菌落总数
#endif
				}
                else if(TPC_COLI_ID == g_work_Test_type[bottleid])
				{
#if PLC_TEST_NEW
					data[bit++] = TPC_COLI_ID;
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

					data[bit++] = PLC_E_NONGDU_FLAG;//埃希氏浓度
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;
					data[bit++] = 0;
#endif
					data[bit++] = PLC_TPC_NONGDU_FLAG;//菌落总数浓度
					u_float.sub_float = gst_His_Info_to_plc[bottleid].NongD;
					data[bit++] = (uchar)u_float.sep_float.b[3];//菌落总数浓度
					data[bit++] = (uchar)u_float.sep_float.b[2];//菌落总数浓度
					data[bit++] = (uchar)u_float.sep_float.b[1];//菌落总数浓度
					data[bit++] = (uchar)u_float.sep_float.b[0];//菌落总数浓度
#if PLC_TEST_NEW
					data[bit++] = PLC_FEC_TIME_FLAG;//耐热 因子
					data[bit++] = 0;// 因子
					data[bit++] = 0;//因子

					data[bit++] = PLC_TOL_TIME_FLAG;//总大肠 因子
					data[bit++] = 0;//总大肠 因子
					data[bit++] = 0;//总大肠 因子


					data[bit++] = PLC_E_TIME_FLAG;//埃希氏 因子
					data[bit++] = 0;// 因子
					data[bit++] = 0;// 因子

					data[bit++] = PLC_TPC_TIME_FLAG;//菌落总数 因子
#endif
                    data[bit++] = (gst_His_Info_to_plc[bottleid].test_len >> 8)&0xff;// 因子
					data[bit++] = gst_His_Info_to_plc[bottleid].test_len&0xff;// 因子

				}
#if PLC_TEST_NEW
				data[bit++] = PLC_BOTTLE_FLAG;// 瓶号
#endif
				data[bit++] = gst_His_Info_to_plc[bottleid].bt_id;// 瓶号

#if PLC_TEST_NEW
				data[bit++] = PLC_UNIT_FLAG;// 单位
#endif
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
#if 0
	for(int j = 0; j < bit; j++)
	{
        printf("send data[%d] = 0x%x \r\n", j, data[j]);
        //qDebug()<< QString("send data[%1] = %2 \r\n").arg(j).arg(data[j]);
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
bool Com5Tran_Thread::Get_com5_data(uchar cmd)
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
		//printf("data[%d] == 0x%x \r\n", i, data[i]);
		qDebug()<<QString("from plc data[%1] == 0x%2 \r\n").arg(i).arg(data[i]);
	}
//		printf("\r\n \r\n");
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

						g_connet_flag = true; // 通讯正常
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
						printf("bootle_no == %d test_type = %d\r\n", bootle_no, test_type);
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
                                     qDebug()<<"com5传输****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									gst_Test_Info[id + TOLCOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + TOLCOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+TOLCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								else if(PLC_FECCOIL_ID == test_type)
								{
									g_work_Test_type[bootle_no-1] = FEC_COLI_ID;
                                     qDebug()<<"com5传输1****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									//g_work_Test_type = FEC_COLI_ID;
									gst_Test_Info[id + FECCOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + FECCOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+FECCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								else if(PLC_TPCCOIL_ID == test_type)
								{
									g_work_Test_type[bootle_no-1] = TPC_COLI_ID;
                                     qDebug()<<"com5传输2****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									gst_Test_Info[id + TPCCOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + TPCCOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+TPCCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								else if(PLC_ECOIL_ID == test_type)
								{
									g_work_Test_type[bootle_no-1] = E_COLI_ID;
                                     qDebug()<<"com5传输3****g_work_Test_type="<<g_work_Test_type[bootle_no-1];
									gst_Test_Info[id + ECOLI_START_NUM].Test_st = TEST_ST_ZERO;
									gst_Test_Info[id + ECOLI_START_NUM].BottleId = id+1;
									memset(&(gst_His_Info[id+ECOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
								}
								printf("\n gst_Test_Info[%d] = %d \r\n", id + TOLCOLI_START_NUM, gst_Test_Info[id + TOLCOLI_START_NUM].Test_st);
								printf("\n gst_Test_Info[%d] = %d \r\n", id + FECCOLI_START_NUM, gst_Test_Info[id + FECCOLI_START_NUM].Test_st);
								printf("\n gst_Test_Info[%d] = %d \r\n", id + TPCCOLI_START_NUM, gst_Test_Info[id + TPCCOLI_START_NUM].Test_st);
								printf("\n gst_Test_Info[%d] = %d \r\n", id + ECOLI_START_NUM, gst_Test_Info[id + ECOLI_START_NUM].Test_st);
							}
                            Set_Bottle_Temp(bootle_no, test_type);  // 设置温度
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
double Com5Tran_Thread::char4ToDouble(char *arrary, float write_data)
{
    u_double.sep_double.l[0] = arrary[3];
    u_double.sep_double.l[1] = arrary[2];
    u_double.sep_double.l[2] = arrary[1];
    u_double.sep_double.l[3] = arrary[0];

    write_data = u_double.sub_double;
    return write_data;
}
#endif

void Com5Tran_Thread::Set_Bottle_Temp(UINT8 bottleId, UINT8 coilType)//设置屏温度
{
    if(REAL_BOTTLE1_ID == bottleId)
    {
        if((PLC_ECOIL_ID == coilType) || (PLC_FECCOIL_ID == coilType))
        {
            Set_Chanel_Param(CMD_PID_CHANEL1,CMD_PID_SET_SP, 445);
        }
        else{
            Set_Chanel_Param(CMD_PID_CHANEL1,CMD_PID_SET_SP, 365);
        }

    }
    else if(REAL_BOTTLE2_ID == bottleId)
    {
        if((PLC_ECOIL_ID == coilType) || (PLC_FECCOIL_ID == coilType))
        {
            Set_Chanel_Param(CMD_PID_CHANEL2,CMD_PID_SET_SP, 445);
        }
        else{
            Set_Chanel_Param(CMD_PID_CHANEL2,CMD_PID_SET_SP, 365);
        }
    }
    else if(REAL_BOTTLE3_ID == bottleId)
    {
        if((PLC_ECOIL_ID == coilType) || (PLC_FECCOIL_ID == coilType))
        {
            Set_Chanel_Param(CMD_PID_CHANEL3,CMD_PID_SET_SP, 445);
        }
        else{
            Set_Chanel_Param(CMD_PID_CHANEL3,CMD_PID_SET_SP, 365);
        }
    }
    else if(REAL_BOTTLE4_ID == bottleId)
    {
        if((PLC_ECOIL_ID == coilType) || (PLC_FECCOIL_ID == coilType))
        {
            Set_Chanel_Param(CMD_PID_CHANEL4,CMD_PID_SET_SP, 445);
        }
        else{
            Set_Chanel_Param(CMD_PID_CHANEL4,CMD_PID_SET_SP, 365);
        }
    }
    else if(REAL_BOTTLE5_ID == bottleId)
    {
        if((PLC_ECOIL_ID == coilType) || (PLC_FECCOIL_ID == coilType))
        {
            Set_Chanel_Param(CMD_PID_CHANEL5,CMD_PID_SET_SP, 445);
        }
        else{
            Set_Chanel_Param(CMD_PID_CHANEL5,CMD_PID_SET_SP, 365);
        }
    }
}


// 发送 数据 获取通道温度值  chanel 通道值
void Com5Tran_Thread::Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data)
{
    SEND_MESSAGE_TYPE tx_data;
    qint32 checksum = 0;
    //char tx_data[CMD_GET_TEMP_SEND_DATA_LEN];

    memset(&tx_data, 0, sizeof(SEND_MESSAGE_TYPE));

    tx_data.u8DevAddr1 = CMD_PID_ADDR + chanel;
    tx_data.u8DevAddr2 = CMD_PID_ADDR + chanel;
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
        printf("tx_data[%d] = 0x%x \r\n", i, tx_data[i]);
    }
#endif

	pTranData->com6_send((const char *) (&tx_data),CMD_GET_TEMP_SEND_DATA_LEN);

	return ;

}
