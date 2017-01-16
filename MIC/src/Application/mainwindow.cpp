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
	ui->lbTest->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));  // 开始测试
	ui->lbSet->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));	  // 用户设置
	ui->lbHelp->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));	// 帮助
	ui->lbHisdata->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));	//历史数据
	ui->lbDebug->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));	//系统调试
	ui->lbDensity->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));	//实时浓度
	ui->groupBox->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));
	ui->groupBox_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));
	ui->label->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));		//当前日期
	ui->label_2->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));	//当前时间
	ui->lbData->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));		//显示日期
	ui->lbTime->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Bold));		// 显示时间
	ui->lbState->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));		// 显示状态
	ui->label_6->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	// 单位显示
#if TEST_ONLY_TPC
    ui->lbTPC->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_7->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));
#elif TEST_ONLY_TOL_FEC
    ui->lbTolColi->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->lbFecColi->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_3->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_4->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
#else
    ui->lbTPC->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->lbTolColi->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->lbFecColi->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->lbEColi->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_3->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_4->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_5->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
    ui->label_7->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));
#endif

	// 温度显示
	ui->lbTemp1->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));
	ui->lbTemp2->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	ui->leTemp1Dis->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	ui->leTemp2Dis->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));

	ui->lbTemp3->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));
	ui->lbTemp4->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	ui->leTemp3Dis->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	ui->leTemp4Dis->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));

	ui->lbTemp5->setFont(QFont(FONT_USE_WQY, FONT_12, QFont::Bold));
	ui->lbTemp6->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	ui->leTemp5Dis->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));
	ui->leTemp6Dis->setFont(QFont(FONT_USE_WQY, FONT_12,QFont::Bold));

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

	connect(ptimerUpdateTime,SIGNAL(timeout()),this,SLOT(timeUpdate()));
	connect(ui->pbdebug,SIGNAL(released()),this,SLOT(pbdebug_Reless()));
	connect(ui->pbst,SIGNAL(released()),this,SLOT(pbst_Reless()));

	connect(ui->pbhelp,SIGNAL(released()),this,SLOT(pbhelp_Reless()));
	connect(ui->pbhisdata,SIGNAL(released()),this,SLOT(pbhisdata_Reless()));
	connect(ui->pbset,SIGNAL(released()),this,SLOT(pbset_Reless()));
	connect(ui->pbtest,SIGNAL(released()),this,SLOT(pbtest_Reless()));


	timeUpdate();
	ShowTemp();
	ShowUnit();
#if 1
	ui->pbst->blockSignals(false); // 实时浓度
	ui->pbhisdata->blockSignals(false);
	ui->pbtest->blockSignals(false);
	ui->pbset->blockSignals(false);
	ui->pbdebug->blockSignals(false);
	ui->pbhelp->blockSignals(false);
#endif
	ptimerUpdateTime->start(TIMER_LEN_500MS);   // 时间显示 500MS
	//qDebug()<<("==***===init============\r\n");

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
	disconnect(ui->pbdebug,SIGNAL(released()),this,SLOT(pbdebug_Reless()));
	disconnect(ui->pbst,SIGNAL(released()),this,SLOT(pbst_Reless()));
	disconnect(ui->pbhelp,SIGNAL(released()),this,SLOT(pbhelp_Reless()));
	disconnect(ui->pbhisdata,SIGNAL(released()),this,SLOT(pbhisdata_Reless()));
	disconnect(ui->pbset,SIGNAL(released()),this,SLOT(pbset_Reless()));
	disconnect(ui->pbtest,SIGNAL(released()),this,SLOT(pbtest_Reless()));
	disconnect(ptimerUpdateTime,SIGNAL(timeout()),this,SLOT(timeUpdate()));

	ui->pbst->blockSignals(true); // 实时浓度
	ui->pbhisdata->blockSignals(true);
	ui->pbtest->blockSignals(true);
	ui->pbset->blockSignals(true);
	ui->pbdebug->blockSignals(true);
	ui->pbhelp->blockSignals(true);

	ptimerUpdateTime->stop();   // 时间显示 500MS
	//qDebug()<<("=***====de_init============\r\n");
	delete ptimerUpdateTime;
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
	MainWindow_unInit();
#if 0
    testmode *tmode = new testmode();
    tmode->showFullScreen();
	//MainWindow_Init();
#else
    testModeSelect * ptestModeSelect = new testModeSelect();
    ptestModeSelect->showFullScreen();
#endif
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
	MainWindow_unInit();
    HisData *hData = new HisData();
	hData->init();
    hData->showFullScreen();
	//MainWindow_Init();
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
	MainWindow_unInit();
#if 1
	Login *pLog = new Login();
	pLog->Login_Init();
	//pLog->show();
	pLog->showNormal();
#else
	Debugging *pDebug = new Debugging();
	pDebug->showFullScreen();

	MainWindow_Init();
#endif
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
void MainWindow::timeUpdate()
{
    QDateTime time= QDateTime::currentDateTime();
    QString strDate= time.toString("yyyy-MM-dd");
    QString strTime= time.toString("hh:mm:ss");

    ui->lbData->setText(strDate);
    ui->lbTime->setText(strTime);
	ShowTemp(); // 显示温度
	Show_system_Info(); // 系统状态
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
	//QString str_temp1 = QString::number(g_temp1);
	//QString str_temp2 = QString::number(g_temp2);

	ui->leTemp1Dis->setText(QString::number(g_temp1));
	ui->leTemp2Dis->setText(QString::number(g_temp2));
	ui->leTemp3Dis->setText(QString::number(g_temp3));
	ui->leTemp4Dis->setText(QString::number(g_temp4));
	ui->leTemp5Dis->setText(QString::number(g_temp5));
	ui->leTemp6Dis->setText(QString::number(g_temp_nor));


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
	switch(g_System_Info)
	{
		case SYSTEM_OK:
			ui->lbState->setText(STR_SYSTEM_OK);
			break;

		case ERROR_GPIO_DRIVER_NOT_EXIST:
			ui->lbState->setText(STR_GPIO_DRVER_NOT_EXIST);
			break;

		case ERROR_GPIO_OPEN_ERROR:
			ui->lbState->setText(STR_GPIO_OPEN_ERROR);
			break;

		case ERROR_AD_DRIVER_NOT_EXIST:
			ui->lbState->setText(STR_AD_DRVER_NOT_EXIST);
			break;

		case ERROR_AD_OPEN_ERROR:
			ui->lbState->setText(STR_AD_OPEN_ERROR);
			break;

		case ERROR_COM6_DRIVER_NOT_EXIST:
			ui->lbState->setText(STR_COM6_DRVER_NOT_EXIST);
			break;

		case ERROR_COM6_OPEN_ERROR:
			ui->lbState->setText(STR_COM6_OPEN_ERROR);
			break;
#if 0
		case :
			break;

		case :
			break;
#endif
		default:
			break;
	}
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


	ST_PARA_SETTING temp_set;
	int TargetNo = 0, ClassNo = 0, ClassNo_1;
	int int_unit;

	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadOnly);  // 打开文件 只读

	file.seek(PRAR_TARGET_START);
	file.read((char*)&TargetNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS_START);
	file.read((char*)&ClassNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS1_START);
	file.read((char*)&ClassNo_1, PRAR_DATA_LEN);

#if TEST_ONLY_TPC
    ui->label_7->setText(STR_UNIT_CFU_ML);
#elif TEST_ONLY_TOL_FEC
    // 获取总大肠单位
    file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
    memset(&temp_set, 0,PRAR_ST_DATA_LEN);
    file.read((char*)&temp_set, PRAR_ST_DATA_LEN);

    int_unit = temp_set.Unit;
    if(int_unit == UNIT_MPN_100ML)  //
    {
        ui->label_3->setText(STR_UNIT_MPN_100ML);
    }
    else if(int_unit == UNIT_MPN_L)  //
    {
        ui->label_3->setText(STR_UNIT_MPN_L);
    }
    else
    {
        ui->label_3->setText(STR_UNIT_MPN_100ML);
    }

    // 获取耐热大肠单位
    file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
    memset(&temp_set, 0,PRAR_ST_DATA_LEN);
    file.read((char*)&temp_set, PRAR_ST_DATA_LEN);

    int_unit = temp_set.Unit;
    if(int_unit == UNIT_MPN_100ML)  //
    {
        ui->label_4->setText(STR_UNIT_MPN_100ML);
    }
    else if(int_unit == UNIT_MPN_L)  //
    {
        ui->label_4->setText(STR_UNIT_MPN_L);
    }
    else
    {
        ui->label_4->setText(STR_UNIT_MPN_100ML);
    }
#else
    // 获取总大肠单位
    file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
    memset(&temp_set, 0,PRAR_ST_DATA_LEN);
    file.read((char*)&temp_set, PRAR_ST_DATA_LEN);

    int_unit = temp_set.Unit;
    if(int_unit == UNIT_MPN_100ML)  //
    {
        ui->label_3->setText(STR_UNIT_MPN_100ML);
    }
    else if(int_unit == UNIT_MPN_L)  //
    {
        ui->label_3->setText(STR_UNIT_MPN_L);
    }
    else
    {
        ui->label_3->setText(STR_UNIT_MPN_100ML);
    }

    // 获取耐热大肠单位
    file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
    memset(&temp_set, 0,PRAR_ST_DATA_LEN);
    file.read((char*)&temp_set, PRAR_ST_DATA_LEN);

    int_unit = temp_set.Unit;
    if(int_unit == UNIT_MPN_100ML)  //
    {
        ui->label_4->setText(STR_UNIT_MPN_100ML);
    }
    else if(int_unit == UNIT_MPN_L)  //
    {
        ui->label_4->setText(STR_UNIT_MPN_L);
    }
    else
    {
        ui->label_4->setText(STR_UNIT_MPN_100ML);
    }
    // 获取埃希氏菌单位
    file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
    memset(&temp_set, 0,PRAR_ST_DATA_LEN);
    file.read((char*)&temp_set, PRAR_ST_DATA_LEN);

    int_unit = temp_set.Unit;
    if(int_unit == UNIT_MPN_100ML)  //
    {
        ui->label_5->setText(STR_UNIT_MPN_100ML);
    }
    else if(int_unit == UNIT_MPN_L)  //
    {
        ui->label_5->setText(STR_UNIT_MPN_L);
    }
    else
    {
        ui->label_5->setText(STR_UNIT_MPN_100ML);
    }

    ui->label_7->setText(STR_UNIT_CFU_ML);
#endif

    file.close();
#if 0
    int int_unit;

	QSettings settings(FILE_CON_PARA_SETTING, QSettings::IniFormat);  // 读取指标单位
	settings.beginGroup(STR_PARASETTING);  // 找到开始
    int_unit = settings.value("U").toInt();
    settings.endGroup();

	if(int_unit == UNIT_MPN_100ML)  //
    {
		ui->label_3->setText(STR_UNIT_MPN_100ML);
		ui->label_4->setText(STR_UNIT_MPN_100ML);
		ui->label_5->setText(STR_UNIT_MPN_100ML);
		ui->label_7->setText(STR_UNIT_MPN_100ML);
    }
	else if(int_unit == UNIT_MPN_L)  //
    {
		ui->label_3->setText(STR_UNIT_MPN_L);
		ui->label_4->setText(STR_UNIT_MPN_L);
		ui->label_5->setText(STR_UNIT_MPN_L);
		ui->label_7->setText(STR_UNIT_MPN_L);
    }
	else if(int_unit == UNIT_CFU_ML)  //
    {
		ui->label_3->setText(STR_UNIT_CFU_ML);
		ui->label_4->setText(STR_UNIT_CFU_ML);
		ui->label_5->setText(STR_UNIT_CFU_ML);
		ui->label_7->setText(STR_UNIT_CFU_ML);
    }

    if(1 /*int_unit == UNIT_MPN_100ML*/)  //
    {
        ui->label_3->setText(STR_UNIT_MPN_100ML);
        ui->label_4->setText(STR_UNIT_MPN_100ML);
        ui->label_5->setText(STR_UNIT_MPN_100ML);
        ui->label_7->setText(STR_UNIT_MPN_100ML);
    }
#endif
}
/*
void MainWindow::on_lbTempValue2_linkActivated(const QString &link)
{
    return 0;
}
*/

