#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>
#include "QMutex"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <fcntl.h>
#include "UdpCommunication.h"
#include "app.h"
#include <QFile>
#include "gpio.h"
#include "libFoo.h"
#include "QDesktopWidget"
#include "QScreen"

#define HARDWARE_V101			1                  //版本 V1.0.1
#define HARDWARE_V100			0                //版本 V1.0.0

#define DEBUD_TEST              0               // 测试

#define TEST_ONLY_TPC            0 // 只测试菌落总数
#define TEST_ONLY_TOL_FEC           0  // 只测试耐热和总大肠
#define HAVE_PRINTF   0   // 有无打印机

// 此文件为公用文件请将全局变量放在此处定义

#define UINT32 unsigned int
#define UINT16 unsigned short
#define UINT8 unsigned char
#define UINT32 unsigned int
#define BOOL	UINT8

#define DWORD	UINT32
#define WORD	UINT16

//长用 Font大小

#define FONT_8				8  //8 号字
#define FONT_10				10  //12 号字
#define FONT_12				12  //12 号字
#define FONT_16				16  //16 号字
#define FONT_20				20  //20 号字
#define FONT_24				24  //24 号字
#define FONT_32				32  //32 号字

#define FONT_USE_WQY   "wenquanyi"  // 用到的字库

// 系统信息
#if  !TEST_ONLY_TOL_FEC

#define SYS_PRODUCT_NAME	"产品名称：	微生物四项检测仪 \r\n"
#define SYS_PRODUCT_TYPE	"产品型号：	JMA-SZS-MP Ⅲ \r\n"
#if HARDWARE_V100
#define SYS_PRODUCT_VER		"版本号：		V1.0.6 \r\n"
#endif
#if HARDWARE_V101
#define SYS_PRODUCT_VER		"版本号：		V1.3.6 \r\n"
#endif

#else
#define SYS_PRODUCT_NAME	"产品名称：	大肠菌群快速检测仪 \r\n"
#define SYS_PRODUCT_TYPE	"产品型号：	JMS-CPM II \r\n"
#define SYS_PRODUCT_VER		"版本号：		V1.0.6 \r\n"

#endif
#define SYS_PRODUCT_TIME	"编译时间："


#define RS232_DRIVE_COM0_NAME    "/dev/ttymxc0"  //com0
#define RS232_DRIVE_COM1_NAME    "/dev/ttymxc1"  // com1
#define RS232_DRIVE_COM2_NAME    "/dev/ttymxc2"  //com2
#define RS232_DRIVE_COM3_NAME    "/dev/ttymxc3"  // com3
#define RS232_DRIVE_COM4_NAME    "/dev/ttymxc4"  //com4

#define RS485_DRIVE_COM5_NAME    "/dev/ttySC0"  //com5
#define RS485_DRIVE_COM6_NAME    "/dev/ttySC1"  // com6

#define RS485_DRIVE_COM6_NAME1    "ttySC1"  // com6
#define RS485_DRIVE_COM5_NAME1    "ttySC0"  // com5
#define RS232_DRIVE_COM2_NAME1    "ttymxc2"  // com2
#define RS232_DRIVE_COM3_NAME1    "ttymxc3"  // com3

#define GPIO_DRIVE_NAME		"/dev/gpio-ext"   // gpio 驱动位置
#define AD_DRIVE_NAME		"/dev/icsadc"   // ad 驱动位置
// 系统状态
extern int g_System_Info;  // 系统状态
#define SYSTEM_OK			0   // 系统正常
#define	ERROR_GPIO_DRIVER_NOT_EXIST		-1  // gpio 驱动不存在
#define	ERROR_GPIO_OPEN_ERROR           -2  // gpio 驱动打开失败

#define	ERROR_AD_DRIVER_NOT_EXIST		-3  // AD 驱动不存在
#define	ERROR_AD_OPEN_ERROR             -4  // AD驱动打开失败

#define	ERROR_COM6_DRIVER_NOT_EXIST		-5  // AD 驱动不存在
#define	ERROR_COM6_OPEN_ERROR             -6  // AD驱动打开失败

#define	ERROR_COM7_DRIVER_NOT_EXIST		-7  // AD 驱动不存在
#define	ERROR_COM7_OPEN_ERROR             -8  // AD驱动打开失败

#define	STR_SYSTEM_OK		"系统正常"
#define	STR_GPIO_DRVER_NOT_EXIST		"GPIO驱动不存在"
#define	STR_GPIO_OPEN_ERROR				"GPIO驱动打开失败"

#define	STR_AD_DRVER_NOT_EXIST		"AD驱动不存在"
#define	STR_AD_OPEN_ERROR				"AD驱动打开失败"

#define	STR_COM6_DRVER_NOT_EXIST		"RS485驱动不存在"
#define	STR_COM6_OPEN_ERROR				"RS485驱动打开失败"
// 时间定义
#define TIMER_LEN_100MS		100  // 100m秒定时
#define TIMER_LEN_200MS		200  // 200m秒定时
#define TIMER_LEN_500MS		500  // 500m秒定时
#define TIMER_LEN_1S		1000  // 1秒定时
#define TIMER_LEN_5S		(5*TIMER_LEN_1S)  // 1秒定时

#define TIMER_LEN_1MINUTE 	(60*TIMER_LEN_1S)  // 1分钟定时
#define TIMER_LEN_5MINUTE 	(5*TIMER_LEN_1MINUTE)  // 5分钟定时


#define THREAD_DELEAY_100MS			100  // msleep // 100 ms
// 获取温度时 需要发送数据的长度
#define CMD_GET_TEMP_SEND_DATA_LEN		8  // 获取温度是发送数据长度
#define CMD_GET_TEMP_DATA_LEN		10  // 收到数据长度

#define CMD_PID_SET_SP			0x00   // PID 4 号通道

#define CMD_PID_ADDR			0x80  // 基地址
#define CMD_PID_READ			0x52  // 读命令
#define CMD_PID_WRITE			0x43  // 写命令

#define CMD_PID__GET_ADDR			0x16  // 写命令

#if HARDWARE_V100
#define CMD_PID_CHANEL5			1   // PID 1 号通道
#define CMD_PID_CHANEL6			2   // PID 2 号通道
#define CMD_PID_CHANEL7			3   // PID 3 号通道
#define CMD_PID_CHANEL8			4   // PID 4 号通道

#define CMD_PID_CHANEL1			10   // PID 1 号通道
#define CMD_PID_CHANEL2			11   // PID 2 号通道
#define CMD_PID_CHANEL3			12   // PID 3 号通道
#define CMD_PID_CHANEL4			13   // PID 4 号通道
#endif

#if HARDWARE_V101
#define CMD_PID_CHANEL5			5   // PID 1 号通道
#define CMD_PID_CHANEL6			6   // PID 2 号通道
#define CMD_PID_CHANEL7			7   // PID 3 号通道
#define CMD_PID_CHANEL8			8   // PID 4 号通道

#define CMD_PID_CHANEL1			1   // PID 1 号通道
#define CMD_PID_CHANEL2			2   // PID 2 号通道
#define CMD_PID_CHANEL3			3   // PID 3 号通道
#define CMD_PID_CHANEL4			4   // PID 4 号通道
#endif


//AIJ仪表通讯协议请求帧帧格式  发送命令格式
typedef struct tagREQ_FRM_T
{
		quint8	u8DevAddr1;		//设备地址
		quint8	u8DevAddr2;		//设备地址
		quint8	u8ReqType;		//请求类型
		quint8	u8DevPara;		//设备参数
		quint8	u8LoParam;		//参数数据-低字节
		quint8	u8HiParam;		//参数数据-高字节
		quint8	u8LoSumCheck;           //累加和校验码-低字节
		quint8	u8HiSumCheck;           //累加和校验码-高字节
}SEND_MESSAGE_TYPE;


//AIJ仪表通讯协议应答帧帧格式
typedef struct tagANS_FRM_T
{
		quint8	u8LoPV;			//应答PV值的低字节
		quint8	u8HiPV;			//应答PV值的高字节
		quint8	u8LoSV;			//应答SV值的低字节
		quint8	u8HiSV;			//应答SV值的高字节
		quint8	u8MV;			//应答MV值
		quint8	u8ALm;		//应答报警状态
		quint8	u8LoParam;		//参数数据-低字节
		quint8	u8HiParam;		//参数数据-高字节
		quint8	u8LoSumCheck;           //累加和校验码-低字节
		quint8	u8HiSumCheck;           //累加和校验码-高字节
}RECEIVE_MESSAGE_TYPE;



// for sql
#define	FILE_HIS_DATA	"/opt/PDA/app/datas/his.db"  // 历史数据

#define STR_TEMP1	"温区一"  // 注意 table中不可以有数字
#define STR_TEMP2	"温区二"
#define STR_TEMP3	"温区三"  // 注意 table中不可以有数字
#define STR_TEMP4	"温区四"
#define STR_TEMP5	"温区五"
#define STR_TEMP6	"温区六"
#if 1
#define STR_TEMP_32		"制冷区"  // 注意 table中不可以有数字
#define STR_TEMP_nor	"常温"
#endif
extern float g_temp1;		// 实时温度1
extern float g_temp2;		// 实时温度2
extern float g_temp3;		// 实时温度1
extern float g_temp4;		// 实时温度2
extern float g_temp5;		// 实时温度1
extern float g_temp_nor;		// 实时温度2


extern float g_Sp1	;
extern float g_Sp2	;
extern float g_Sp3	;
extern float g_Sp4	;
extern float g_Sp5	;
extern float g_Sp6	;
extern float g_Sp7	;
extern float g_Sp8	;


//每个瓶号的水类别和曲线
extern int Bottle1_ClassNo;
extern int Bottle2_ClassNo;
extern int Bottle3_ClassNo;
extern int Bottle4_ClassNo;
extern int Bottle5_ClassNo;
extern int Bottle6_ClassNo;
extern int Bottle7_ClassNo;
extern int Bottle8_ClassNo;
extern int Bottle9_ClassNo;
extern int Bottle10_ClassNo;
extern int Bottle11_ClassNo;
extern int Bottle12_ClassNo;
extern int Bottle13_ClassNo;
extern int Bottle14_ClassNo;
extern int Bottle15_ClassNo;
extern int Bottle16_ClassNo;
extern int Bottle17_ClassNo;
extern int Bottle18_ClassNo;
extern int Bottle19_ClassNo;
extern int Bottle20_ClassNo;
extern int Bottle21_ClassNo;
extern int Bottle22_ClassNo;
extern int Bottle23_ClassNo;
extern int Bottle24_ClassNo;
extern int Bottle25_ClassNo;

extern int Bottle1_ClassNo_1;
extern int Bottle2_ClassNo_1;
extern int Bottle3_ClassNo_1;
extern int Bottle4_ClassNo_1;
extern int Bottle5_ClassNo_1;
extern int Bottle6_ClassNo_1;
extern int Bottle7_ClassNo_1;
extern int Bottle8_ClassNo_1;
extern int Bottle9_ClassNo_1;
extern int Bottle10_ClassNo_1;
extern int Bottle11_ClassNo_1;
extern int Bottle12_ClassNo_1;
extern int Bottle13_ClassNo_1;
extern int Bottle14_ClassNo_1;
extern int Bottle15_ClassNo_1;
extern int Bottle16_ClassNo_1;
extern int Bottle17_ClassNo_1;
extern int Bottle18_ClassNo_1;
extern int Bottle19_ClassNo_1;
extern int Bottle20_ClassNo_1;
extern int Bottle21_ClassNo_1;
extern int Bottle22_ClassNo_1;
extern int Bottle23_ClassNo_1;
extern int Bottle24_ClassNo_1;
extern int Bottle25_ClassNo_1;

//定义斜率 截距
extern double XieLv;
extern double JieJu;
extern bool TOLParaCalibratedFlag;
extern bool FECParaCalibratedFlag;
extern bool TPCParaCalibratedFlag;
extern bool EParaCalibratedFlag;
extern bool CalibrateFlag;

//保存测试的瓶号
extern QVector<int> TOLTestNumberVector;
extern QVector<int> FECTestNumberVector;
extern QVector<int> TPCTestNumberVector;
extern QVector<int> ETestNumberVector;
//定义瓶测试的次数
extern int TOLTestNumber;
extern int FECTestNumber;
extern int TPCTestNumber;
extern int ETestNumber;


#define TEMP_RS485_DELEY	(500*1000)  // usleep 200ms


typedef struct Para_setting
{
    double para_A;  // 参数A
    double para_B;	// 参数B
    double para_C;	// 参数C
    double para_D;	// 参数D
    double para_Q;	// 参数Q
    double para_R;	// 参数R
    int Unit;	// 单位
}ST_PARA_SETTING;

//QString conParaCalibrareFile = "/opt/PDA/app/datas/ParaCalibrate.ini";
#if 1
//#define FILE_CON_PARA_SETTING			"/opt/PDA/app/datas/ParaSetting.ini"  // 指标单位参数 文件
//#else
#define FILE_QX_PARA_SETTING			"/home/root/appData/ParaSetting.bin"  // 指标单位参数 文件
#endif
//#define FILE_CON_PARA_SETTING			"/opt/PDA/app/datas/ParaSetting.ini"  // 指标单位参数 文件
#define FILE_ACURATE_DENSITY			"/opt/PDA/app/datas/AcurateDensity.ini"  // 温区 指标 文件

#define PRAR_MAX_NUM        100 // 最大曲线数
#define PRAR_MAX_CLASS_NUM        10 // 最大曲线数

#define PRAR_ST_DATA_LEN    sizeof(ST_PARA_SETTING) // 数据长度
//#define PRAR_ST_DATA_LEN    sizeof(ST_PARA_SETTING) // 数据长度
#define PRAR_DATA_LEN       sizeof(int) // 数据长度
#define PRAR_TARGET_START   0  // 菌种记录开始位置
#define PRAR_CLASS_START   (PRAR_DATA_LEN +  PRAR_TARGET_START) // 类别开始位置
#define PRAR_CLASS1_START   (PRAR_DATA_LEN +  PRAR_CLASS_START) //系列

#define PRAR_TOL_START   (PRAR_CLASS1_START + PRAR_DATA_LEN)  // 总大肠曲线开始位置
#define PRAR_FEC_START   (PRAR_TOL_START + PRAR_MAX_NUM*PRAR_ST_DATA_LEN)  // 耐热曲线开始位置
#define PRAR_TPC_START   (PRAR_FEC_START + PRAR_MAX_NUM*PRAR_ST_DATA_LEN)  // 菌落总数曲线开始位置
#define PRAR_E_START   (PRAR_TPC_START + PRAR_MAX_NUM*PRAR_ST_DATA_LEN)  //埃希氏菌曲线开始位置

#define FILE_TOL_HIS					"/opt/PDA/app/datas/HistaryDensityTol.ini"   //总大肠历史浓度数据
#define FILE_FEC_HIS					"/opt/PDA/app/datas/HistaryDensityFec.ini"   //耐热大肠历史浓度数据
#define FILE_E_HIS						"/opt/PDA/app/datas/HistaryDensityE.ini"   //艾希氏菌历史浓度数据
#define FILE_TPC_HIS					"/opt/PDA/app/datas/HistaryDensityTPC.ini"   //菌落总数历史浓度数据


#define  FILE_CON_PARA_CALIBRATE		"/opt/PDA/app/datas/ParaCalibrate.ini"   //
//#define	FILE_HIS_DATA_MODE				"/opt/PDA/app/datas/HisDataMode.ini"  //历史数据模式
#define	FILE_HIS_TEMP1							"/opt/PDA/app/datas/HistaryTemp1.ini"   // 温区１历史数据
#define	FILE_HIS_TEMP2							"/opt/PDA/app/datas/HistaryTemp2.ini"   // 温区１历史数据

#define	FILE_SYS_ST							"/opt/PDA/app/datas/sys_st.bin"   // 系统信息

#define	PIC_BACKGROUND_PNG			"/home/root/appData/images/Background.png"  // 背景图片
#define	PIC_BACKGROUND2_PNG			"/home/root/appData/images/Background2.png"  // 背景图片


#define LEN_TEMP_DATA			10 // 温度数据长度
#define LEN_TEMP1_DATA			100 // 温度数据长度

#define PLC_BOTTLE_1			1 //
#define PLC_BOTTLE_2			2 //
#define PLC_BOTTLE_3			3 //
#define PLC_BOTTLE_4			4 //
#define PLC_BOTTLE_5			5 //

// for PLC com
#define PLC_CMD_HEAD_1				0x4A		// J
#define PLC_CMD_HEAD_2				0x4D		// M

#define PLC_CMD_GET_PARAM				0x01		// 获取状态
#define PLC_CMD_GET_ST				0x02		// 获取状态
#define PLC_CMD_GET_SYSYTEM_PARAM				0x11		// 获取系统参数
#define PLC_CMD_GET_NONGDU				0x12		// 获取浓度值
#define PLC_CMD_GET_AD				0x13		// 获取AD值
#define PLC_CMD_GET_TEMP				0x14		// 获取AD值
#define PLC_CMD_GET_IN_PARAM				0x15		// 获取AD值

#define PLC_CMD_SET_TIME				0x21		// 对时
#define PLC_CMD_SET_START				0x22		// 停止
#define PLC_CMD_SET_TEMP				0x23		//set temp
#define PLC_CMD_SET_PARAM				0x24		// 设置测量参数
#define PLC_CMD_SET_IN_PARAM			0x25		// 设置内部参数


#define PLC_DATA_TYPE_8B				0x00  //数据类型 8bit
#define PLC_DATA_TYPE_16B				0x01  //数据类型	16bit
#define PLC_DATA_TYPE_32B				0x02  //数据类型   32bit


#define PLC_SET_OK				0x00 // ok
#define PLC_START_TESTTING				0x11 // 开始测试
#define PLC_STOP_TESTTING				0x10 // 停止测试

#define PLC_TOLCOIL_ID				0x00 // 总大肠
#define PLC_ECOIL_ID				0x01 // 埃希氏
#define PLC_FECCOIL_ID				0x02 // 耐热
#define PLC_TPCCOIL_ID				0x03 // 菌落总数

// for PLC nongdu

#define PLC_TIME_FLAG				0xD3		//时间
#define PLC_FEC_NONGDU_FLAG				0xD9	// 耐热浓度
#define PLC_TOL_NONGDU_FLAG				0xDA	// 总大肠浓度
#define PLC_E_NONGDU_FLAG				0xDB	//  埃希氏浓度
#define PLC_TPC_NONGDU_FLAG				0xE0	//  菌落总数

#define PLC_FEC_TIME_FLAG				0xDC	//  耐热因子
#define PLC_TOL_TIME_FLAG				0xDD	//  总大肠因子
#define PLC_E_TIME_FLAG				0xDE	//  埃希氏因子
#define PLC_TPC_TIME_FLAG				0xE1	//  菌落总数因子

#define PLC_BOTTLE_FLAG				0xDF	//  瓶号
#define PLC_UNIT_FLAG				0xE3	//  瓶号

// 指标单位
#define	UNIT_MPN_100ML				0  //  MPN/100mL
#define	UNIT_MPN_L						1	// MPN/L
#define	UNIT_CFU_ML						2	// CFU/mL

#define	STR_UNIT_MPN_100ML				 "MPN/100mL"
#define	STR_UNIT_MPN_L					"MPN/L"
#define	STR_UNIT_CFU_ML					"CFU/mL"

//#define	PIC_BACKGROUND_PNG			"/opt/PDA/app/images/Background.png"  // 背景图片


//#define	PIC_BACKGROUND_PNG			"/image/Background.png"  // 背景图片
#define	MAX_MODE_LEN    10		//测试 mode 最大支付长度

#define MAX_HIS_ID					5000 // 最大记录数

#define NULL_ID			0 // 默认值
#define TOL_COLI_ID			1 //总大肠
#define FEC_COLI_ID				2 //耐热大肠
#define TPC_COLI_ID			3 //细菌总数
#define E_COLI_ID					4 //艾希氏

#define TEMP1_ID					5 // 温区１
#define TEMP2_ID					6	// 温区２

#define TEMP3_ID					7 // 温区１
#define TEMP4_ID					8	// 温区２

#define TEMP5_ID					9 // 温区１
#define TEMP6_ID					10	// 温区２

// login 界面
#define	STR_LOGIN_ADMIN			"管理员"   //
#define	STR_LOGIN_OP				"操作员"   //
#define	STR_LOGIN_PASSWORD				"1234"		//默认密码

// netsetting

#define	STR_UPD							"UDP"  // udp  协议
#define	STR_WORK_MODE1							"实验室模式"  //
#define	STR_WORK_MODE2							"在线模式"  //
#define	FILE_NET_SETTING			"/opt/PDA/app/datas/netSetting.bin"   // 网络设置 文件


//testting 界面
//#define STR_TEST_START	"开始检测！"   //
//#define STR_TEST_STOP	"检测结束！"
#if 1
#define BOTTLE1_ID		0  //瓶号
#define BOTTLE2_ID		1  //瓶号
#define BOTTLE3_ID		2  //瓶号
#define BOTTLE4_ID		3  //瓶号
#define BOTTLE5_ID		4  //瓶号
#define BOTTLE6_ID		5  //瓶号
#define BOTTLE7_ID		6  //瓶号
#define BOTTLE8_ID		7  //瓶号
//#else
#define REAL_BOTTLE1_ID		(BOTTLE1_ID+1)  //瓶号
#define REAL_BOTTLE2_ID		(BOTTLE2_ID+1)  //瓶号
#define REAL_BOTTLE3_ID		(BOTTLE3_ID+1)  //瓶号
#define REAL_BOTTLE4_ID		(BOTTLE4_ID+1)  //瓶号
#define REAL_BOTTLE5_ID		(BOTTLE5_ID+1)  //瓶号
#define REAL_BOTTLE6_ID		(BOTTLE6_ID+1)  //瓶号
#define REAL_BOTTLE7_ID		(BOTTLE7_ID+1)  //瓶号
#define REAL_BOTTLE8_ID		(BOTTLE8_ID+1)  //瓶号
#endif
#define STR_TEST_TOLCOLI	"总大肠杆菌"
#define STR_TEST_FECCOLI	"耐热大肠杆菌"
#define STR_TEST_TPCCOLI	"菌落总数"
#define STR_TEST_ECOLI		"埃希氏菌"


#define STR_TEST_TOLCOLI_HIS	"总大肠杆菌历史数据"
#define STR_TEST_FECCOLI_HIS	"耐热大肠杆菌历史数据"
#define STR_TEST_TPCCOLI_HIS	"菌落总数历史数据"
#define STR_TEST_ECOLI_HIS		"埃希氏菌历史数据"


// hisdis  历史数据界面
#define DATA_TABLE_W    (760-30) // 控件总宽度
#define DATA_TABLE0_W  10 //第1个table ID
//#define DATA_TABLE1_W  10 //第1个table 样本号
#define DATA_TABLE1_W  80 //第2个table 样本号
#define DATA_TABLE2_W  130 //第2个table 浓度
//#define DATA_TABLE4_W  160 //第2个table 采样时间
#define DATA_TABLE3_W  100 //第2个table 单位 未修改前130
#define DATA_TABLE5_W  50 //第2个table 瓶号 未修改前60
#define DATA_TABLE6_W  100 //第2个table 检测因子
#define DATA_TABLE7_W  60 //修正
#define DATA_TABLE4_W  (DATA_TABLE_W - DATA_TABLE1_W - DATA_TABLE2_W - DATA_TABLE3_W - DATA_TABLE0_W - DATA_TABLE5_W -DATA_TABLE6_W-DATA_TABLE7_W) //第2个table 的宽度

// temp dis
#define TEMP_DATA_TABLE0_W  10 //第1个table ID
#define TEMP_DATA_TABLE1_W  160 //第2个table 浓度
#define TEMP_DATA_TABLE2_W  120 //第2个table 单位


#define STR_BOTTLE_ST   "状态"
#define STR_BOTTLE_TR   "透过率"

#define	STR_PROMPT		"提示:"
#define	STR_SET_OK		"设置成功"
#define	STR_SET_ERROR		"设置失败"
#define	STR_CLEAR_DATA		"是否要清空数据？"
#define	STR_STOP_TESTING		"是否停止检测？"
#define	STR_PARAM_ERROR		"参数错误，请输入点数！！"

// parasetting 相关
#define STR_PARASETTING			"Para Setting"  // 文本标志

#define STR_PARASETTING_UNIT	"Unit"  // 单位 标志
#define STR_PARASETTING_CLASS	"CLASS"  // 系列
#define STR_PARASETTING_COIL	"Coil"  // 菌种

#define STR_TOl_CLASS	"Tol_Class"  // 系列
#define STR_FEC_CLASS	"Fec_Class"  // 系列
#define STR_TPC_CLASS	"TPC_Class"  // 系列
#define STR_E_CLASS	"E_Class"  // 系列

#define STR_TOl_UNIT	"Tol_Unit"  // 单位
#define STR_FEC_UNIT	"Fec_Unit"  // 单位
#define STR_TPC_UNIT	"TPC_Unit"  // 单位
#define STR_E_UNIT	"E_Unit"  // 单位

// 总大肠参数
#define	STR_TOLCOIL_CLASS1_A	"Tol_Class1_A"
#define	STR_TOLCOIL_CLASS1_B	"Tol_Class1_B"
#define	STR_TOLCOIL_CLASS1_C	"Tol_Class1_C"
#define	STR_TOLCOIL_CLASS1_D	"Tol_Class1_D"
#define	STR_TOLCOIL_CLASS1_Q	"Tol_Class1_Q"  // 合格线
#define	STR_TOLCOIL_CLASS1_R	"Tol_Class1_R"	// 相关系数

#define	STR_TOLCOIL_CLASS2_A	"Tol_Class2_A"
#define	STR_TOLCOIL_CLASS2_B	"Tol_Class2_B"
#define	STR_TOLCOIL_CLASS2_C	"Tol_Class2_C"
#define	STR_TOLCOIL_CLASS2_D	"Tol_Class2_D"
#define	STR_TOLCOIL_CLASS2_Q	"Tol_Class2_Q"  // 合格线
#define	STR_TOLCOIL_CLASS2_R	"Tol_Class2_R"	// 相关系数

#define	STR_TOLCOIL_CLASS3_A	"Tol_Class3_A"
#define	STR_TOLCOIL_CLASS3_B	"Tol_Class3_B"
#define	STR_TOLCOIL_CLASS3_C	"Tol_Class3_C"
#define	STR_TOLCOIL_CLASS3_D	"Tol_Class3_D"
#define	STR_TOLCOIL_CLASS3_Q	"Tol_Class3_Q"  // 合格线
#define	STR_TOLCOIL_CLASS3_R	"Tol_Class3_R"	// 相关系数

#define	STR_TOLCOIL_CLASS4_A	"Tol_Class4_A"
#define	STR_TOLCOIL_CLASS4_B	"Tol_Class4_B"
#define	STR_TOLCOIL_CLASS4_C	"Tol_Class4_C"
#define	STR_TOLCOIL_CLASS4_D	"Tol_Class4_D"
#define	STR_TOLCOIL_CLASS4_Q	"Tol_Class4_Q"  // 合格线
#define	STR_TOLCOIL_CLASS4_R	"Tol_Class4_R"	// 相关系数
// 耐热大肠参数
#define	STR_FECCOIL_CLASS1_A	"Fec_Class1_A"
#define	STR_FECCOIL_CLASS1_B	"Fec_Class1_B"
#define	STR_FECCOIL_CLASS1_C	"Fec_Class1_C"
#define	STR_FECCOIL_CLASS1_D	"Fec_Class1_D"
#define	STR_FECCOIL_CLASS1_Q	"Fec_Class1_Q"  // 合格线
#define	STR_FECCOIL_CLASS1_R	"Fec_Class1_R"	// 相关系数

#define	STR_FECCOIL_CLASS2_A	"Fec_Class2_A"
#define	STR_FECCOIL_CLASS2_B	"Fec_Class2_B"
#define	STR_FECCOIL_CLASS2_C	"Fec_Class2_C"
#define	STR_FECCOIL_CLASS2_D	"Fec_Class2_D"
#define	STR_FECCOIL_CLASS2_Q	"Fec_Class2_Q"  // 合格线
#define	STR_FECCOIL_CLASS2_R	"Fec_Class2_R"	// 相关系数

#define	STR_FECCOIL_CLASS3_A	"Fec_Class3_A"
#define	STR_FECCOIL_CLASS3_B	"Fec_Class3_B"
#define	STR_FECCOIL_CLASS3_C	"Fec_Class3_C"
#define	STR_FECCOIL_CLASS3_D	"Fec_Class3_D"
#define	STR_FECCOIL_CLASS3_Q	"Fec_Class3_Q"  // 合格线
#define	STR_FECCOIL_CLASS3_R	"Fec_Class3_R"	// 相关系数

#define	STR_FECCOIL_CLASS4_A	"Fec_Class4_A"
#define	STR_FECCOIL_CLASS4_B	"Fec_Class4_B"
#define	STR_FECCOIL_CLASS4_C	"Fec_Class4_C"
#define	STR_FECCOIL_CLASS4_D	"Fec_Class4_D"
#define	STR_FECCOIL_CLASS4_Q	"Fec_Class4_Q"  // 合格线
#define	STR_FECCOIL_CLASS4_R	"Fec_Class4_R"	// 相关系数

// 菌落总数
#define	STR_TPCCOIL_CLASS1_A	"TPC_Class1_A"
#define	STR_TPCCOIL_CLASS1_B	"TPC_Class1_B"
#define	STR_TPCCOIL_CLASS1_C	"TPC_Class1_C"
#define	STR_TPCCOIL_CLASS1_D	"TPC_Class1_D"
#define	STR_TPCCOIL_CLASS1_Q	"TPC_Class1_Q"  // 合格线
#define	STR_TPCCOIL_CLASS1_R	"TPC_Class1_R"	// 相关系数

#define	STR_TPCCOIL_CLASS2_A	"TPC_Class2_A"
#define	STR_TPCCOIL_CLASS2_B	"TPC_Class2_B"
#define	STR_TPCCOIL_CLASS2_C	"TPC_Class2_C"
#define	STR_TPCCOIL_CLASS2_D	"TPC_Class2_D"
#define	STR_TPCCOIL_CLASS2_Q	"TPC_Class2_Q"  // 合格线
#define	STR_TPCCOIL_CLASS2_R	"TPC_Class2_R"	// 相关系数

#define	STR_TPCCOIL_CLASS3_A	"TPC_Class3_A"
#define	STR_TPCCOIL_CLASS3_B	"TPC_Class3_B"
#define	STR_TPCCOIL_CLASS3_C	"TPC_Class3_C"
#define	STR_TPCCOIL_CLASS3_D	"TPC_Class3_D"
#define	STR_TPCCOIL_CLASS3_Q	"TPC_Class3_Q"  // 合格线
#define	STR_TPCCOIL_CLASS3_R	"TPC_Class3_R"	// 相关系数

#define	STR_TPCCOIL_CLASS4_A	"TPC_Class4_A"
#define	STR_TPCCOIL_CLASS4_B	"TPC_Class4_B"
#define	STR_TPCCOIL_CLASS4_C	"TPC_Class4_C"
#define	STR_TPCCOIL_CLASS4_D	"TPC_Class4_D"
#define	STR_TPCCOIL_CLASS4_Q	"TPC_Class4_Q"  // 合格线
#define	STR_TPCCOIL_CLASS4_R	"TPC_Class4_R"	// 相关系数


// 埃希氏
#define	STR_ECOIL_CLASS1_A	"E_Class1_A"
#define	STR_ECOIL_CLASS1_B	"E_Class1_B"
#define	STR_ECOIL_CLASS1_C	"E_Class1_C"
#define	STR_ECOIL_CLASS1_D	"E_Class1_D"
#define	STR_ECOIL_CLASS1_Q	"E_Class1_Q"  // 合格线
#define	STR_ECOIL_CLASS1_R	"E_Class1_R"	// 相关系数

#define	STR_ECOIL_CLASS2_A	"E_Class2_A"
#define	STR_ECOIL_CLASS2_B	"E_Class2_B"
#define	STR_ECOIL_CLASS2_C	"E_Class2_C"
#define	STR_ECOIL_CLASS2_D	"E_Class2_D"
#define	STR_ECOIL_CLASS2_Q	"E_Class2_Q"  // 合格线
#define	STR_ECOIL_CLASS2_R	"E_Class2_R"	// 相关系数

#define	STR_ECOIL_CLASS3_A	"E_Class3_A"
#define	STR_ECOIL_CLASS3_B	"E_Class3_B"
#define	STR_ECOIL_CLASS3_C	"E_Class3_C"
#define	STR_ECOIL_CLASS3_D	"E_Class3_D"
#define	STR_ECOIL_CLASS3_Q	"E_Class3_Q"  // 合格线
#define	STR_ECOIL_CLASS3_R	"E_Class3_R"	// 相关系数

#define	STR_ECOIL_CLASS4_A	"E_Class4_A"
#define	STR_ECOIL_CLASS4_B	"E_Class4_B"
#define	STR_ECOIL_CLASS4_C	"E_Class4_C"
#define	STR_ECOIL_CLASS4_D	"E_Class4_D"
#define	STR_ECOIL_CLASS4_Q	"E_Class4_Q"  // 合格线
#define	STR_ECOIL_CLASS4_R	"E_Class4_R"	// 相关系数

//class ID
#define COIL_CLASS_NO1	0
#define COIL_CLASS_NO2	1
#define COIL_CLASS_NO3	2
#define COIL_CLASS_NO4	3


// testing 相关
#define MAX_TEST_NUM	100//32	 // 最大测试值
#define MAX_ONE_TEST_NUM	25	 // 一种菌最大测试个数

#define ONE_TEST_NUM	20	 // 一种菌最大测试个数

#define MAX_ONE_GROUP_NUM	5	 // 一组个数

#define TOLCOLI_START_NUM	0	 //总大肠开始序号
#define FECCOLI_START_NUM	25	 //耐热 开始序号
#define TPCCOLI_START_NUM	50	 //菌落总数 开始序号
#define ECOLI_START_NUM		75	 //埃希氏 开始序号

#define	TEST_ST_IDEL	0	// 空闲
#define	TEST_ST_ZERO	1	// 较零
#define	TEST_ST_TESTING	2	// 正在测试

#define STR_TEST_ST_IDLE	"空闲"
#define STR_TEST_ST_ZERO	"校零"
#define STR_TEST_ST_TESTING	"测试中"

#define STR_TEST_START	"开始"
#define STR_TEST_STOP	"停止"
#define STR_TEST_BOTTLE_NUM	"号瓶"

#define MAX_THOU_DATA	10 // 平滑 保存数据


// 一条历史记录信息
typedef struct History
{
	QString SampleID;  // 样本号
	QString NongDu;			// 浓度
	QString Unit;	// 单位
	QString Time;	// 时间
	QString BottleId;	// 瓶号
	QString Param_time;	// 检测时间

	ushort year;
	uchar mon;
	uchar day;
	uchar hour;
	uchar min;
	uchar sec;

	uchar bt_id; // 数字表示 瓶号
	uchar unit_n ;	// 数字表示 单位
	float NongD ;	// 数字表示 浓度
	uint	test_len;	//测试 时间

}ST_HISTORY_DATA;

typedef struct History1
{
     QString    SampleID;       //时间
     QString	Ad_DarkData_Begin;		//存放各个通道暗信号初始AD数据
     QString	Ad_LightData_Begin;		//存放各个通道亮信号初始AD数据
     QString	Ad_DarkData_Cur;			//存放各个通道暗信号当前AD数据
     QString	Ad_LightData_Cur;			//存放各个通道亮信号当前AD数据
     QString	Ad_LightData_Last;		// 上一次的变化率
     QString	Ad_Max_Move_Time;		// 最大变化的时刻
     QString	Ad_Max_Move_Data;		// 最大变化值
     QString 	ThouData_Cur;			//当前透过率 变成吸光度
     QString 	ThouData_dis;			//当前透过率 显示
     QString 	Last_ThouData;			//上一次的透过率
     QString    sThouData_Cnt;			// 检测次数 就是时间

}ST_HISTORY_DATA1;
#define BOTTLE_MAX_NUM			5 //在线测试相对瓶数
extern ST_HISTORY_DATA gst_His_Info[MAX_TEST_NUM];  //

extern ST_HISTORY_DATA gst_His_Info_to_plc[BOTTLE_MAX_NUM];  //
//#define MAX__TITER_VALUE_MAX
// 测试相关的 变量
typedef struct test_info
{
	int Save_st;	// 测试状态

	int Test_st;	// 测试状态
	int SampleID;  // 样本号
	int BottleId;  // 瓶号

    long	Ad_DarkData_Begin;		//存放各个通道暗信号初始AD数据
    long	Ad_LightData_Begin;		//存放各个通道亮信号初始AD数据

    long	Ad_DarkData_Cur;			//存放各个通道暗信号当前AD数据
    long	Ad_LightData_Cur;			//存放各个通道亮信号当前AD数据

    long	Ad_LightData_Last;		// 上一次的变化率
	unsigned int	Ad_Max_Move_Time;		// 最大变化的时刻
    long	Ad_Max_Move_Data;		// 最大变化值

	double 	ThouData_Cur;			//当前透过率
	double 	ThouData_dis;			//当前透过率 显示
	double 	Last_ThouData;			//上一次的透过率

	int		sThouDataNum;						// 保存的数据
	double	sThouDataTemp[MAX_THOU_DATA+1];					// 用于平滑校正

	unsigned int sThouData_Cnt;						// 检测次数 就是时间

	unsigned int threshold_time;//快速模式下根据参数计算出来的检出限时间

     bool  TestFlag;
     int CalcMethod;


	//QString Time;	// 时间

}ST_TEST_INFO;
extern ST_TEST_INFO gst_Test_Info[MAX_TEST_NUM];
extern ST_TEST_INFO pSt_CalibrateInfo[MAX_TEST_NUM];
extern int CalcMethod[MAX_TEST_NUM];

#define LOG_FILE_SIZE		(10*1024*1024)  // 5M log信息 文件大小

#define MAX_LED_GROUP_NUM		15  //  LED 一组的个数



#define GPIO_HIGH		1  // 高电平
#define GPIO_LOW		0  // 低电平

// 功能引脚定义
#define LED_EN		GPIO_G5_P21	// 405nm
#define AD_EN		GPIO_G5_P18	// 405nm

//led addr
#define LED_A0		GPIO_G3_P31
#define LED_A1		GPIO_G4_P5
#define LED_A2		GPIO_G5_P2
#define LED_A3		GPIO_G5_P19
//ad addr
#define AD_A0		GPIO_G1_P16
#define AD_A1		GPIO_G1_P20
#define AD_A2		GPIO_G3_P23
#define AD_A3		GPIO_G3_P30

#define E_AD_EN		GPIO_G6_P17


#define ECOIL_EN		GPIO_G3_P30 //埃希氏 EN
#define TOL_FEC_TPC_LED_EN		GPIO_G3_P31 //剩余3项LED  EN
#define TOL_FEC_TPC_AD_EN		GPIO_G3_P23 //剩余3项AD  EN
// 地址定义
#define ECOIL_A0		GPIO_G6_P18 //埃希氏 地址0
#define ECOIL_A1		GPIO_G6_P31 //埃希氏 地址1
#define ECOIL_A2		GPIO_G1_P28 //埃希氏 地址2

#define TOL_FEC_TPC_LED_A0		GPIO_G5_P18 //剩余3项LED 地址0
#define TOL_FEC_TPC_LED_A1		GPIO_G5_P21 //剩余3项LED 地址1
#define TOL_FEC_TPC_LED_A2		GPIO_G6_P17 //剩余3项LED 地址2

#define TOL_FEC_TPC_AD_A0		GPIO_G4_P5 //剩余3项AD 地址0
#define TOL_FEC_TPC_AD_A1		GPIO_G5_P2 //剩余3项AD 地址1
#define TOL_FEC_TPC_AD_A2		GPIO_G5_P19 //剩余3项AD 地址2

#define ECOIL_AD_CHANEL				3//4   //AD chanel

#define FEC_AD_CHANEL				1 //2   //AD chanel
#define TPC_AD_CHANEL				2 //3   //AD chanel
#define TOL_AD_CHANEL				0 //1   //AD chanel

#define AD_ZERO						0x8000  //
#define AD_GET_TIME						(200*1000)  // 200ms
#define AD_GET_TIME2						(20*1000)  // 10ms
//有效数据(判断数据的变化率不超过正负10%) 则转到完成
#define ZERO_STANDARD			10 //
#define FULL_STANDARD			100 // 100%

#define	 STANDARD_PARAM			100  // 倍数
#define START_STANDARD			(30*STANDARD_PARAM) //	30%
#define END_STANDARD			(70*STANDARD_PARAM) //	70%
//#define END_STANDARD			(60*STANDARD_PARAM) //	60%
#define END_MAX_STANDARD		(70*STANDARD_PARAM) //	70%



#define END_MAX_STANDARD_60		(60*STANDARD_PARAM) //	60%
#define END_MAX_STANDARD_50		(50*STANDARD_PARAM) //	50%
#define END_MAX_STANDARD_40		(40*STANDARD_PARAM) //	40%
#define END_MAX_STANDARD_30		(30*STANDARD_PARAM) //	30%
#define END_MAX_STANDARD_20		(20*STANDARD_PARAM) //	20%
#define END_MAX_STANDARD_10		(10*STANDARD_PARAM) //	10%
#define END_MAX_STANDARD_5		(5*STANDARD_PARAM)	//	5%




#define THOU_DATA_STANARD		20	// 上下波动范围

#define  FAST_TIME_LIMIT              1080//18小时  最大检测时间
//#define  FAST_TIME_LIMIT              2//2分钟 测试时用  最大检测时间
//#define  FAST_TIME_LIMIT              1440//18小时  最大检测时间  test
#define  TB_FAST_TIME_LIMIT             3 //1440//24小时 .埃希氏 最大检测时间

#define  AD_TEST_LIMIT             1000 //500//500 AD变化范围
//#define  M_OVER_TITER_VALUE_TB   		8000//消光度为80%
//#define  M_FAST_THSLD_TIME_VALUE    	MasterWorkInf.threshold_time//检出限阙值时间
//#define  M_MPN_TIME_LIMIT			    1440//24小时
//#define  M_MIX_TIME_LIMIT			    1080//18小时
#define MAX_MOVE_DATA			500 // 光度比变化最大值


#define	WORK_MODE_AUTOTESTTING		1 // 在线
#define	WORK_MODE_TEST		0	// 台式

extern int g_workmode;   // 声明 工作模式
extern int mpn_0[][6];
extern int mpn_1[][6] ;
extern int mpn_2[][6] ;
extern int mpn_3[][6] ;
extern int mpn_4[][6] ;
extern int mpn_5[][6] ;


#define MPN_TEST_IDLE       0  // 空闲
#define MPN_TEST_FEC       1  // 耐热
#define MPN_TEST_TOL       2  // 总大肠

#define MPN_MAX_NUM         15 // MPN 方法15支管


extern int g_mpn_test_st;  // 测试状态 MPN测方法状态
extern int g_mpn_test_mui; // 稀释倍数
extern int g_mpn_test_index;//稀释倍数索引
extern int g_mpn_test_val[MPN_MAX_NUM];  // 保存瓶状态
extern int g_work_Test_type[BOTTLE_MAX_NUM] ;  // 测试菌
extern int g_test_st;  // 测试状态
extern unsigned int g_Test_Over_Time ;  // 超时时间
  //  crc 16 算法
ushort CRC16(uchar *ba, int size);
//LED 控制
// num --> led 号
// val --> led 状态 0--> 全灭   1--> 亮
void LED_Power(unsigned int num, unsigned int val);
// 获取 AD值
int Get_ad(unsigned int num);
// 获取 AD值 电压值
double Get_val(unsigned int num);
// log 信息保存
extern void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// for hardware **************************
void HardWare_Init(void);  // 硬件初始化
void HardWare_UnInit(void); //接触硬件

void set_system_time(int year, uchar mon, uchar day, uchar hour, uchar min, uchar sec);

void Line_Fit(float *X, float *Y, int N, float *a, float *b);


void getFullScree(char *name);

inline QString utf82gbk(const std::string inStr);
inline std::string gbk2utf8(const QString &inStr);

void str2char(QString str, char *data);
QString UTF82GBK(const QString inStr);

#endif // COMMON_H
