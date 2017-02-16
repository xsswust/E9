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
	//connect(ui->pb_gpio_ex_in, SIGNAL(released()),this,SLOT(on_pb_gpio_ex_in_released()));
	//connect(pb->,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	ptimerUpdateTime->start(TIMER_LEN_1S);   // 时间显示 500MS
	qDebug()<<("==***===init============\r\n");

	connect(ui->pb_debug, SIGNAL(released()),this,SLOT(pbdebug_Reless()));
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
	Debugging *debug = new Debugging();
	debug->showFullScreen();
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

bool gpio_flag = true;
void MainWindow::timeUpdate()
{
//	printf("\r\ntimeUpdate == []gFlag == %d gpio_flag === %d\r\n ", gFlag, gpio_flag);

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

