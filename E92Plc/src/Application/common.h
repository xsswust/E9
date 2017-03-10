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
#include "e92plc_cfg.h"
#include "pwm.h"


#define DEBUD_TEST              0               // 测试

#define TEST_ONLY_TPC            0 // 只测试菌落总数
#define TEST_ONLY_TOL_FEC           1  // 只测试耐热和总大肠
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
#define SYS_PRODUCT_VER		"版本号：		V1.0.6 \r\n"
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


#define MAX_MAIN_OUT_GPIO		8 // 接口板上 输出IO数
#define MAX_MAIN_IN_GPIO		8 // 接口板上 输入IO数

#define ONE_EXT_IN_GPIO		8 // 扩展板上 输入IO数
#define ONE_EXT_OUT_GPIO		16 // 扩展板上 输出IO数

#define MAX_EXT_NUM				2 // 接口板上 扩展板数量
#define MAX_EXT_IN_GPIO			(MAX_EXT_NUM*ONE_EXT_IN_GPIO) // 扩展板上 最多输入IO数
#define MAX_EXT_OUT_GPIO		(MAX_EXT_NUM*ONE_EXT_OUT_GPIO) // 扩展板上 最多输出IO数
// 进液和排液瓶最大个数
#define MAX_JINYE_NUM		6//
#define MAX_PAIYEYE_NUM		6//


// 保存IO口状态
extern UINT8 gExt_out[MAX_EXT_OUT_GPIO + 1]; // 扩展板输出IO数
extern UINT8 gExt_in[MAX_EXT_IN_GPIO + 1];	// 扩展板输入IO数

#define FA_SHIJI		(MAX_EXT_OUT_GPIO - 1)   // 扩展输出 1号pin   试剂阀
#define FA_SHUIYANG		(MAX_EXT_OUT_GPIO - 2)   // 扩展输出 2号pin   水样阀
#define FA_HUNHEYE		(MAX_EXT_OUT_GPIO - 3)   // 扩展输出 3号pin   混合阀
#define FA_XIAODUYE		(MAX_EXT_OUT_GPIO - 4)   // 扩展输出 4号pin   消毒液阀

#define FA_WUJUNSHUI	(MAX_EXT_OUT_GPIO - 5)   // 扩展输出 5号pin  无菌水阀
#define FA_KONGQI		(MAX_EXT_OUT_GPIO - 6)   // 扩展输出 6号pin		空气阀
#define FA_JINYE1		(MAX_EXT_OUT_GPIO - 7)   // 扩展输出 7号pin		进液阀1
#define FA_JINYE2		(MAX_EXT_OUT_GPIO - 8)   // 扩展输出 8号pin		进液阀2

#define FA_JINYE3		(MAX_EXT_OUT_GPIO - 9)   // 扩展输出 9号pin		进液阀3
#define FA_JINYE4		(MAX_EXT_OUT_GPIO - 10)   // 扩展输出 10号pin		进液阀4
#define FA_JINYE5		(MAX_EXT_OUT_GPIO - 11)   // 扩展输出 11号pin		进液阀5
#define FA_JINYE6		(MAX_EXT_OUT_GPIO - 12)   // 扩展输出 12号pin		进液阀6

#define FA_PAIYE1		(MAX_EXT_OUT_GPIO - 13)   // 扩展输出 13号pin		排液阀1
#define FA_PAIYE2		(MAX_EXT_OUT_GPIO - 14)   // 扩展输出 14号pin		排液阀2
#define FA_JILIANKONGQI		(MAX_EXT_OUT_GPIO - 15)   // 扩展输出 15号pin	计量阀上
#define FA_XIGUAN		(MAX_EXT_OUT_GPIO - 16)   // 扩展输出 16号pin	计量阀下  吸管

#define FA_PAIYE3		(MAX_EXT_OUT_GPIO - 17)   // 扩展输出 17号pin		排液阀3
#define FA_PAIYE4		(MAX_EXT_OUT_GPIO - 18)   // 扩展输出 18号pin		排液阀4
#define FA_PAIYE5		(MAX_EXT_OUT_GPIO - 19)   // 扩展输出 19号pin		排液阀5
#define FA_PAIYE6		(MAX_EXT_OUT_GPIO - 20)   // 扩展输出 20号pin		排液阀6

#define FA_RUDONGBENGZHENG		(MAX_EXT_OUT_GPIO - 21)   // 扩展输出 17号pin		蠕动泵正
#define FA_RUDONGBENGFAN		(MAX_EXT_OUT_GPIO - 22)   // 扩展输出 18号pin		蠕动泵反
#define FA_WAIPAIYEBENG		(MAX_EXT_OUT_GPIO - 23)   // 扩展输出 19号pin		外排液泵
#define FA_PAIYEBENG		(MAX_EXT_OUT_GPIO - 24)   // 扩展输出 20号pin		排液泵

#define FA_ZAILIUHUAN		(MAX_EXT_OUT_GPIO - 25)   // 扩展输出 17号pin		载流环
#define FA_ZIWAIDENG		(MAX_EXT_OUT_GPIO - 26)   // 扩展输出 18号pin		紫外灯
#define FA_NULL1		(MAX_EXT_OUT_GPIO - 27)   // 扩展输出 19号pin
#define FA_NULL2		(MAX_EXT_OUT_GPIO - 28)   // 扩展输出 20号pin

#define FA_NULL3		(MAX_EXT_OUT_GPIO - 29)   // 扩展输出 19号pin
#define FA_NULL4		(MAX_EXT_OUT_GPIO - 30)   // 扩展输出 20号pin
#define FA_NULL5		(MAX_EXT_OUT_GPIO - 31)   // 扩展输出 19号pin
#define FA_NULL6		(MAX_EXT_OUT_GPIO - 32)   // 扩展输出 20号pin
//声明
extern UINT8 gJinYeFa[];
extern UINT8 gPaiYeFa[];


#define MODE_GUANLUCLEAN    1   // 管路清洗
#define MODE_JIANCECHICLEAN    2   // 检测池清洗
#define MODE_ALLCLEAN    3   // 全部清洗





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

#define CMD_PID_CHANEL5			5   // PID 1 号通道
#define CMD_PID_CHANEL6			6   // PID 2 号通道
#define CMD_PID_CHANEL7			7   // PID 3 号通道
#define CMD_PID_CHANEL8			8   // PID 4 号通道

#define CMD_PID_CHANEL1			1   // PID 1 号通道
#define CMD_PID_CHANEL2			2   // PID 2 号通道
#define CMD_PID_CHANEL3			3   // PID 3 号通道
#define CMD_PID_CHANEL4			4   // PID 4 号通道



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

#define	FILE_SYS_ST							"/opt/PDA/app/datas/sys_st.bin"   // 系统信

#define	PIC_BACKGROUND_PNG			"/home/root/appData/images/Background.png"  // 背景图片
#define	PIC_BACKGROUND2_PNG			"/home/root/appData/images/Background2.png"  // 背景图片
#define	PIC_BACKGROUND_SYSTEMTEST_JPG			"/home/root/appData/pic-plc/systemTest.jpg"  // 背景图片
#define	PIC_BACKGROUND_SYSTEMTEST_PNG			"/home/root/appData/pic-plc/systemTest.png"  // 背景图片
#define	PIC_BACKGROUND_SINGLETEST_PNG			"/home/root/appData/pic-plc/singleTest.png"  // 背景图片
#define	PIC_BACKGROUND_SINGLEZUHE_PNG			"/home/root/appData/pic-plc/comTest.png"  // 背景图片


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

// 组合动作界面
#define STR_ZUHE_ON		"开"
#define STR_ZUHE_OFF		"关"
#define	STR_STOP_ZUHE		"动作正在执行，是否停止动作？"

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
#define DATA_TABLE_W    (760-50) // 控件总宽度

#define DATA_TABLE0_W  10 //第1个table ID
//#define DATA_TABLE1_W  10 //第1个table 样本号
#define DATA_TABLE1_W  80 //第2个table 浓度
#define DATA_TABLE2_W  130 //第2个table 单位
//#define DATA_TABLE4_W  160 //第2个table 采样时间
#define DATA_TABLE3_W  130 //第2个table 检测量
#define DATA_TABLE5_W  60 //第2个table 瓶号
#define DATA_TABLE6_W  100 //第2个table 瓶号
#define DATA_TABLE4_W  (DATA_TABLE_W - DATA_TABLE1_W - DATA_TABLE2_W - DATA_TABLE3_W - DATA_TABLE0_W - DATA_TABLE5_W -DATA_TABLE6_W) //第2个table 的宽度

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
#define	STR_PARAM_ERROR		"参数错误！！"

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



//class ID
#define COIL_CLASS_NO1	0
#define COIL_CLASS_NO2	1
#define COIL_CLASS_NO3	2
#define COIL_CLASS_NO4	3


// testing 相关
#define	TEST_ST_IDEL	0	// 空闲
#define	TEST_ST_ZERO	1	// 较零
#define	TEST_ST_TESTING	2	// 正在测试

#define STR_TEST_ST_IDLE	"空闲"
#define STR_TEST_ST_ZERO	"校零"
#define STR_TEST_ST_TESTING	"测试中"

#define STR_TEST_START	"开始"
#define STR_TEST_STOP	"停止"
#define STR_TEST_BOTTLE_NUM	"号瓶"

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

#define BOTTLE_MAX_NUM			5 //在线测试相对瓶数

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
    long	Ad_LightData_Cur;			//存放各个通道暗信号当前AD数据

    long	Ad_LightData_Last;		// 上一次的变化率
	unsigned int	Ad_Max_Move_Time;		// 最大变化的时刻
    long	Ad_Max_Move_Data;		// 最大变化值

	double 	ThouData_Cur;			//当前透过率
	double 	ThouData_dis;			//当前透过率 显示
	double 	Last_ThouData;			//上一次的透过率

	int		sThouDataNum;						// 保存的数据
	//double	sThouDataTemp[MAX_THOU_DATA+1];					// 用于平滑校正

	unsigned int sThouData_Cnt;						// 检测次数 就是时间

	unsigned int threshold_time;//快速模式下根据参数计算出来的检出限时间
	//QString Time;	// 时间

}ST_TEST_INFO;
//extern ST_TEST_INFO gst_Test_Info[MAX_TEST_NUM];

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

//阀执行超时时间 300秒
#define  TIMEOUT_LIMIT              300

extern int unsigned g_TIMEOUT ;


//有效数据(判断数据的变化率不超过正负10%) 则转到完成
#define THOU_DATA_STANARD		20	// 上下波动范围

#define  FAST_TIME_LIMIT              1080//18小时  最大检测时间
//#define  FAST_TIME_LIMIT              1440//18小时  最大检测时间  test
#define  TB_FAST_TIME_LIMIT             1440//24小时 .埃希氏 最大检测时间

#define  AD_TEST_LIMIT             1000 //500//500 AD变化范围
//#define  M_OVER_TITER_VALUE_TB   		8000//消光度为80%
//#define  M_FAST_THSLD_TIME_VALUE    	MasterWorkInf.threshold_time//检出限阙值时间
//#define  M_MPN_TIME_LIMIT			    1440//24小时
//#define  M_MIX_TIME_LIMIT			    1080//18小时
#define MAX_MOVE_DATA			500 // 光度比变化最大值


#define	WORK_MODE_AUTOTESTTING		1 // 在线
#define	WORK_MODE_TEST		0	// 台式

extern int g_workmode;   // 声明 工作模式

#define MPN_TEST_IDLE       0  // 空闲
#define MPN_TEST_FEC       1  // 耐热
#define MPN_TEST_TOL       2  // 总大肠

#define MPN_MAX_NUM         15 // MPN 方法15支管


extern int g_mpn_test_st;  // 测试状态 MPN测方法状态
extern int g_mpn_test_mui; // 稀释倍数
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
void msleep(unsigned int msec);

void set_system_time(int year, uchar mon, uchar day, uchar hour, uchar min, uchar sec);

void Line_Fit(float *X, float *Y, int N, float *a, float *b);


void getFullScree(char *name);

inline QString utf82gbk(const std::string inStr);
inline std::string gbk2utf8(const QString &inStr);

void str2char(QString str, char *data);
QString UTF82GBK(const QString inStr);
void Set_Main_Gpio(int num, int val);
void UpdateExtInGpio();
int Get_Ext_Gpio(int num);
int Get_Main_Gpio(int num);
bool Set_Ext_Gpio(int num, int val);
void Set_Main_Gpio(int num, int val);
void UpdateExtOutGpio();
void SetMaDa2_Start(bool flag);
void SetMaDa1_Start(bool flag);
void SetMaDa1_Dir(bool flag);
void SetMaDa2_Dir(bool flag);
// 停止所有阀
void StopAll();
#endif // COMMON_H
