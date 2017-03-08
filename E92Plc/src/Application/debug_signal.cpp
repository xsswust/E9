#include "debug_signal.h"
#include "ui_debug_signal.h"
#include	"common.h"

#define TEST 1

debug_signal::debug_signal(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::debug_signal)
{
	ui->setupUi(this);

	QPixmap pixmap(PIC_BACKGROUND_SINGLETEST_PNG);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);
	g_num = 0;

//	Init();
}

debug_signal::~debug_signal()
{
	delete ui;
}
// 初始化 界面
void debug_signal::Init()
{
	exit_flag = false;
	timeForGetTEMP_1Minute = new QTimer();
	timeForGetTEMP_1Minute->stop();  // ms

	timeForGetTEMP_100ms = new QTimer();
	timeForGetTEMP_100ms->stop();  // ms


	// 建立连接
	connect(timeForGetTEMP_1Minute,SIGNAL(timeout()),this,SLOT(timeUp_1Minute()));  //
	connect(timeForGetTEMP_100ms,SIGNAL(timeout()),this,SLOT(timeUp_100Ms()));

	//connect(pb_,SIGNAL(timeout()),this,SLOT(timeUp_100Ms()));

	//timeForGetTEMP_1Minute->start(TIMER_LEN_1MINUTE);

     timeForGetTEMP_1Minute->start(250);

    //timeUp_1Minute();
}

// 初始化
void debug_signal::Uninit()
{
	exit_flag = true;
	// 建立连接
	disconnect(timeForGetTEMP_1Minute,SIGNAL(timeout()),this,SLOT(timeUp_1Minute()));  //
	disconnect(timeForGetTEMP_100ms,SIGNAL(timeout()),this,SLOT(timeUp_100Ms()));

	timeForGetTEMP_1Minute->stop();
	timeForGetTEMP_100ms->stop();
	//timeUp_1Minute();

	delete timeForGetTEMP_1Minute;
	delete timeForGetTEMP_100ms;
}


// 定时获取数据
void debug_signal::timeUp_1Minute()
{

}

void debug_signal::timeUp_100Ms()
{

}

void debug_signal::on_pB_Exit_clicked()
{
//	getFullScree("signal.jpg");
	if(false == exit_flag)
	{
//		Uninit();
//		this->close();
	}
}

// 设置 试剂阀 状态
void debug_signal::on_pB_shiji_released()
{
	if(gExt_out[FA_SHIJI] == 0){  // 关闭的
		Set_Ext_Gpio(FA_SHIJI, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(FA_SHIJI, GPIO_LOW);
	}
	SetMaDa2_Start(true);
	//SetMaDa1_Start(true);
	SetMaDa2_Dir(true);
	//SetMaDa1_Dir(true);
}
// 设置水样阀状态
void debug_signal::on_pB_shuiyangfa_released()
{
	if(gExt_out[FA_SHUIYANG] == 0){  // 关闭的
		Set_Ext_Gpio(FA_SHUIYANG, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(FA_SHUIYANG, GPIO_LOW);
	}
}
// 次氯酸阀
void debug_signal::on_pB_cilvsuan_released()
{
	int pin = FA_XIAODUYE;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}

// 设置混合液阀状态
void debug_signal::on_pB_hunhefa_released()
{
	int pin = FA_HUNHEYE;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}

}
// 无菌水
void debug_signal::on_pB_wujunshuifa_released()
{
	int pin = FA_WUJUNSHUI;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 空气阀
void debug_signal::on_pB_kongqifa_released()
{
	int pin = FA_KONGQI;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 进液阀1
void debug_signal::on_pB_jinyefa_released()
{
	int pin = FA_JINYE1;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 进液阀2
void debug_signal::on_pB_jinyefa2_released()
{
	int pin = FA_JINYE2;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 进液阀3
void debug_signal::on_pB_jinyefa3_released()
{
	int pin = FA_JINYE3;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 进液阀4
void debug_signal::on_pB_jinyefa4_released()
{
	int pin = FA_JINYE4;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 进液阀5
void debug_signal::on_pB_jinyefa5_released()
{
	int pin = FA_JINYE5;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 进液阀6
void debug_signal::on_pB_jinyefa6_released()
{
	int pin = FA_JINYE6;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 载流环
void debug_signal::on_pB_zailiufa_released()
{
	int pin = FA_ZAILIUHUAN;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}

}
// 排液泵1
void debug_signal::on_pB_payefa1_released()
{
	int pin = FA_PAIYE1;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}

// 排液泵2
void debug_signal::on_pB_paiyefa2_released()
{
	int pin = FA_PAIYE2;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 排液泵3
void debug_signal::on_pB_paiyefa3_released()
{
	int pin = FA_PAIYE3;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 排液泵4
void debug_signal::on_pB_paiyefa4_released()
{
	int pin = FA_PAIYE4;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 排液泵5
void debug_signal::on_pB_paiyefa5_released()
{
	int pin = FA_PAIYE5;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 排液泵6
void debug_signal::on_pB_paiyefa6_released()
{
	int pin = FA_PAIYE6;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 蠕动泵正转
void debug_signal::on_pB_rudongbengzheng_released()
{
	int pin = FA_RUDONGBENGZHENG;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
		Set_Ext_Gpio(FA_RUDONGBENGFAN, GPIO_LOW);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 蠕动泵反转
void debug_signal::on_pB_rongdongbengfan_released()
{
	int pin = FA_RUDONGBENGFAN;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
		Set_Ext_Gpio(FA_RUDONGBENGZHENG, GPIO_LOW);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
	printf("\r\n [on_pB_rongdongbengfan_released]   === gExt_out[pin] = %d \r\n", gExt_out[pin]);
}
// 外排液泵
void debug_signal::on_pB_waipaifa_released()
{
	int pin = FA_WAIPAIYEBENG;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
	printf("\r\n [on_pB_waipaifa_released]   === gExt_out[pin] = %d \r\n", gExt_out[pin]);
}
// 排液泵
void debug_signal::on_pB_paiyebeng_released()
{
	int pin = FA_PAIYEBENG;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
	printf("\r\n [on_pB_paiyebeng_released]   === gExt_out[pin] = %d \r\n", gExt_out[pin]);
}
//计量上
void debug_signal::on_pB_jiliangfashag_released()
{
	int pin = FA_JILIANGSHANG;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 计量下
void debug_signal::on_pB_jiliangdaxia_released()
{
	int pin = FA_JILIANGXIA;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}
// 灭菌等
void debug_signal::on_pB_shajundeng_released()
{
	int pin = FA_ZIWAIDENG;

	if(gExt_out[pin] == 0){  // 关闭的
		Set_Ext_Gpio(pin, GPIO_HIGH);
	}
	else{
		Set_Ext_Gpio(pin, GPIO_LOW);
	}
}


// 全部停止
void debug_signal::on_pB_StopAll_released()
{
	StopAll();
}
// 返回上一级界面
void debug_signal::on_pB_Exit_released()
{
	//	getFullScree("signal.jpg");
		//if(false == exit_flag)
		{
	//		Uninit();
			this->close();
		}
}
