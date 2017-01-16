#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testmode.h"
#include "aculatedata.h"
#include "hisdata.h"
#include "syssetting.h"
#include "syshelp.h"
#include "debugging.h"
#include "login.h"
#include "comtran.h"
#include "parasetting.h"
#include "src/Thread/comtran_thread.h"
#include <QString>
#include <QSettings>
#include <QFont>
#include <QTextCodec>
#include <QPalette>
#include <QDateTime>
#include <QTimer>
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
#include <QDebug>
#include<time.h>
#include<QProcess>
#include "common.h"
#include "testmodeselect.h"
#include "e92plc_cfg.h"
#include "pwm.h"

int gFlag = GPIO_OUT_INTERFACE;

MainWindow *pmainwin;  // 主界面

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

    //Chinese Display
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //Font Setting
	ptimerUpdateTime = new QTimer();
	ptimerUpdateTime->setInterval(500);  // 500 ms


	connect(ptimerUpdateTime,SIGNAL(timeout()),this,SLOT(timeUpdate()));

	ptimerUpdateTime->start(TIMER_LEN_1S);   // 时间显示 500MS
}

/*
	函数名称：
	函数功能：删除界面
	输入参数：
	输出参数：
	修改记录：
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*
	函数名称：
	函数功能：界面初始化
	输入参数：
	输出参数：
	修改记录：
*/
void MainWindow::MainWindow_Init()
{

	ptimerUpdateTime = new QTimer();



	timeUpdate();
	ShowTemp();
	ShowUnit();
	connect(ui->pb_gpio_ex_in, SIGNAL(released()),this,SLOT(on_pb_gpio_ex_in_released()));
	//connect(pb->,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	ptimerUpdateTime->start(TIMER_LEN_1S);   // 时间显示 500MS
	qDebug()<<("==***===init============\r\n");

}
/*
	函数名称：
	函数功能：界面切换 处理
	输入参数：
	输出参数：
	修改记录：
*/
void MainWindow::MainWindow_unInit()
{

}

/*
    函数名称：
	函数功能：切换到 开始测试 界面
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::pbtest_Reless()
{

}

/*
    函数名称：
	函数功能：历史数据 显示历史数据界面
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::pbhisdata_Reless()
{

}

/*
    函数名称：
    函数功能：系统调试
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::pbdebug_Reless()
{

}

/*
    函数名称：
    函数功能：用户设置
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::pbset_Reless()
{
	MainWindow_unInit();
    SysSetting *sysSet = new SysSetting();
    sysSet->showFullScreen();
	//MainWindow_Init();
}

/*
    函数名称：
    函数功能：系统帮助
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::pbhelp_Reless()
{
//	getFullScree("main.jpg");

	MainWindow_unInit();
    SysHelp *sHelp = new SysHelp();
    sHelp->showFullScreen();
	//MainWindow_Init();
}
/*
	函数名称： 界面切换
	函数功能：切换实时浓度界面
	输入参数：
	输出参数：
	修改记录：
*/
void MainWindow::pbst_Reless()
{
	MainWindow_unInit();
    AculateData *aData = new AculateData();
    aData->showFullScreen();
	MainWindow_Init();
}

/*
    函数名称：
    函数功能：时间刷新
    输入参数：
    输出参数：
    修改记录：
*/
int gpio_out[] = {MAIN_GPIO_OUT0, MAIN_GPIO_OUT1,MAIN_GPIO_OUT2, MAIN_GPIO_OUT3,MAIN_GPIO_OUT4, MAIN_GPIO_OUT5,MAIN_GPIO_OUT6, MAIN_GPIO_OUT7};
int gpio_in[] = {MAIN_GPIO_IN0, MAIN_GPIO_IN1,MAIN_GPIO_IN2, MAIN_GPIO_IN3,MAIN_GPIO_IN4, MAIN_GPIO_IN5,MAIN_GPIO_IN6, MAIN_GPIO_IN7};
bool gpio_flag = true;
void MainWindow::timeUpdate()
{
//	printf("\r\ntimeUpdate == []gFlag == %d gpio_flag === %d\r\n ", gFlag, gpio_flag);
	// gpio 测试输出
	if(gFlag == GPIO_OUT_INTERFACE){
		for(int i = 0; i < 8; i++){
			if(true == gpio_flag){
				Gpio_set(gpio_out[i], 1);
			}
			else {
				Gpio_set(gpio_out[i], 0);
			}
		}
		if(true == gpio_flag){
			gpio_flag = false;
		}
		else{
			gpio_flag = true;
		}

	}
	else if(gFlag == GPIO_IN_INTERFACE){  // 输入接口
		for(int i = 0; i < 8; i++){
			printf("gpio_in[%d] === %d\r\n", i, Gpio_get(gpio_in[i]));
		}
	}
	else if(gFlag == GPIO_EXT_OUT_INTERFACE)  // 扩展IO 输出  OK
	{
		Gpio_set(OUT_CLK, 0);  // 注意 clk 为上升沿时有效
		Gpio_set(OUT_LE, 0);
		Gpio_set(OUT_SDI, 0);
#if 0
		for(int i = 0; i < GPIO_EXT_OUT_NUM; i++)
		{
			if(true == gpio_flag){
				if(i%2 == 0){
					Gpio_set(OUT_SDI, 1);
				}
				else {
					Gpio_set(OUT_SDI,0);
				}
			}
			else{
				if(i% 2 == 0){
					Gpio_set(OUT_SDI, 0);
				}
				else{
					Gpio_set(OUT_SDI, 1);
				}
			}

//			Sleep(1);
			Gpio_set(OUT_CLK, 1);
//			Sleep(1);
			Gpio_set(OUT_CLK, 0);

		}
#else  // 先进先出原则， 先进入的为 高位
		for(int i = 0; i < 32; i++)
		{
			if(i < 16 ){
				if(i < 1){
					Gpio_set(OUT_SDI, 0);
				}
				else{
					Gpio_set(OUT_SDI,1);
				}
			}
			else{
				if(i < 30){
					Gpio_set(OUT_SDI, 1);
				}
				else{
					Gpio_set(OUT_SDI, 0);
				}
			}

//			Sleep(1);
			Gpio_set(OUT_CLK, 1);
//			Sleep(1);
			Gpio_set(OUT_CLK, 0);

		}
#endif
		Gpio_set(OUT_LE, 1);

		if(true == gpio_flag){
			gpio_flag = false;
		}
		else{
			gpio_flag = true;
		}
	}
	else if(gFlag == GPIO_EXT_IN_INTERFACE){
		Gpio_set(IN_CLK, 0);
		Gpio_set(IN_LE, 1);  // 先置高在置地进行本芯片的数据保存
	//	Sleep(1);
		Gpio_set(IN_LE, 0);
		printf(" get[%d] == %d \r\n", 0, (bool)Gpio_get(IN_DATA));
		for(int i = 0; i < GPIO_EXT_IN_NUM*2 -1; i++)
		{

//			Sleep(1);
			Gpio_set(IN_CLK, 1);
			printf(" get[%d] == %d \r\n", i+1, (bool)Gpio_get(IN_DATA));
//			Sleep(1);
			Gpio_set(IN_CLK, 0);

		}
	}
	else if(gFlag == PWM1_INTERFACE)
	{

	}
	else if(gFlag == PWM2_INTERFACE)
	{

	}

}

/*
    函数名称：
	函数功能：温度显示
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::ShowTemp()
{

}
/*
	函数名称：
	函数功能：显示系统状态
	输入参数：
	输出参数：
	修改记录：
*/
void MainWindow::Show_system_Info()
{

}

/*
    函数名称：
	函数功能：指标 单位显示
    输入参数：
    输出参数：
    修改记录：
*/
void MainWindow::ShowUnit()
{

}

// 接口板 gpio 测试  out

void MainWindow::on_pb_gpio_released()
{
	gFlag = GPIO_OUT_INTERFACE;
}
// 接口板 gpio 测试  in
void MainWindow::on_pb_gpio_in_released()
{
	gFlag = GPIO_IN_INTERFACE;
}
// 接口板 gpio ext 测试  out
void MainWindow::on_pb_gpio_ex_out_released()
{
	gFlag = GPIO_EXT_OUT_INTERFACE;
}
// 扩展板 gpio ext in
void MainWindow::on_pb_gpio_ex_in_released()
{
	printf("\r\n on_pb_gpio_ex_in_released ==== gFlag = %d \r\n", gFlag);
	gFlag = GPIO_EXT_IN_INTERFACE;
}
// pwm1
void MainWindow::on_pb_pwm1_released()
{
	gFlag = PWM1_INTERFACE;

    if(true == gpio_flag){
        pwm_init(1, 1000, 50, 1);
        gpio_flag = false;
    }
    else{
        pwm_init(1, 1000, 50, 0);
        gpio_flag = true;
    }
}
// pwm2

void MainWindow::on_pb_pwm2_released()
{
	gFlag = PWM2_INTERFACE;

    if(true == gpio_flag){
        pwm_init(2, 1000, 50, 1);
        gpio_flag = false;
    }
    else{
        pwm_init(2, 1000, 50, 0);
        gpio_flag = true;
    }
}


int gPwm1_flag = false;
//电机1 转向控制

void MainWindow::on_pb_pwm1_DIR_released()
{
    if(gPwm1_flag == true){
        Gpio_set(gpio_out[0], 1);
        gPwm1_flag = false;
    }
    else{
        Gpio_set(gpio_out[0], 0);
        gPwm1_flag = true;
    }
}

int gPwm2_flag = false;
//电机2 转向控制
void MainWindow::on_pb_pwm2_DIR_released()
{
    if(gPwm2_flag == true){
        Gpio_set(gpio_out[1], 1);
        gPwm2_flag = false;
    }
    else{
        Gpio_set(gpio_out[1], 0);
        gPwm2_flag = true;
    }
}

