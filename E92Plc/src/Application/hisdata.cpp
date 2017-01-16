#include "hisdata.h"
#include "ui_hisdata.h"
#include "hisdatadis.h"
#include "hisdatatemp.h"
#include <QPalette>
#include <QTextCodec>
#include <QFont>
#include <QSettings>
#include <QString>
#include "common.h"
#include "mainwindow.h"

int gHisDataMode = NULL_ID; //
// 构造函数
HisData::HisData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HisData)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND2_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
//    QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);
    //Font Setting

    ui->label->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

    ui->pbTemp1->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbTemp2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	ui->pbTemp3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	ui->pbTemp4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	ui->pbTemp5->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbDataOut->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	//ui->pbTemp2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
#if TEST_ONLY_TPC
    ui->pbTPC->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	 ui->pbTPC->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
#elif TEST_ONLY_TOL_FEC
    ui->pbFecColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbTolColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
#else
    ui->pbTPC->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbEColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbFecColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbTolColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
#endif

#if 0//!TEST_ONLY_TPC
    ui->pbEColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbFecColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbTolColi->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
#endif

	ui->lb_Title->setFont(QFont(FONT_USE_WQY,FONT_24,QFont::Normal));
	// 居中显示
	ui->lb_Title->setAlignment(Qt::AlignCenter);
}
// 析构函数
HisData::~HisData()
{
    delete ui;
}

void HisData::init()
{
	exit_flag = false;
	connect(ui->pbTemp3,SIGNAL(released()),this,SLOT(pbTemp3_Reless()));  // 上一组
	connect(ui->pbTemp4,SIGNAL(released()),this,SLOT(pbTemp4_Reless()));  // 上一组
	connect(ui->pbTemp5,SIGNAL(released()),this,SLOT(pbTemp5_Reless()));  // 上一组

    connect(ui->pbDataOut,SIGNAL(released()),this,SLOT(pbDataOut_Reless()));  //建立连接 导出数据
}


void HisData::Uninit()
{
	disconnect(ui->pbTemp3,SIGNAL(released()),this,SLOT(pbTemp3_Reless()));  // 上一组
	disconnect(ui->pbTemp4,SIGNAL(released()),this,SLOT(pbTemp4_Reless()));  // 上一组
	disconnect(ui->pbTemp5,SIGNAL(released()),this,SLOT(pbTemp5_Reless()));  // 上一组
    disconnect(ui->pbDataOut,SIGNAL(released()),this,SLOT(pbDataOut_Reless()));  //建立连接 导出数据
}

/*
	函数名称：
	函数功能：界面统一跳转函数
	输入参数：mode 显示哪一种数据
	输出参数：
	修改记录：
*/
void HisData::HisData_ShowHisData(int mode)
{
    gHisDataMode = mode;

#if 0
	if((mode == TEMP1_ID) || (mode == TEMP2_ID))
	{
		HisDataTemp *hDataTempDis = new HisDataTemp();
		hDataTempDis->HisDataTemp_init();
		hDataTempDis->showFullScreen();
	}
	else
#endif
	{
		HisDataDis *hDataDis = new HisDataDis();
		hDataDis->HisDataDis_init();
		hDataDis->showFullScreen();
	}
}

/*
	函数名称：
	函数功能：界面退出函数
	输入参数：
	输出参数：
	修改记录：
*/
void HisData::on_pbExit_clicked()
{
//	getFullScree("his.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		Uninit();
		 this->close();
		pmainwin->MainWindow_Init(); // 恢复状态
	}
}

/*
    函数名称：
	函数功能：读取　菌落总数　数据
    输入参数：
    输出参数：
    修改记录：
*/
void HisData::on_pbTPC_clicked()
{
	HisData_ShowHisData(TPC_COLI_ID);   // 显示
}

/*
    函数名称：
	函数功能：读取　总大肠数据
    输入参数：
    输出参数：
    修改记录：
*/
void HisData::on_pbTolColi_clicked()
{
	HisData_ShowHisData(TOL_COLI_ID);
}

/*
    函数名称：
	函数功能：读取　耐热大肠数据
    输入参数：
    输出参数：
    修改记录：
*/
void HisData::on_pbFecColi_clicked()
{
	HisData_ShowHisData(FEC_COLI_ID);
}
/*
    函数名称：
    函数功能：读取埃希氏数据
    输入参数：
    输出参数：
    修改记录：
*/
void HisData::on_pbEColi_clicked()
{
	HisData_ShowHisData(E_COLI_ID);
}
/*
    函数名称：
    函数功能：读取温区1
    输入参数：
    输出参数：
    修改记录：
*/
void HisData::on_pbTemp1_clicked()
{
	HisData_ShowHisData(TEMP1_ID);
}
/*
    函数名称：
    函数功能：读取温区2
    输入参数：
    输出参数：
    修改记录：
*/
void HisData::on_pbTemp2_clicked()
{
	HisData_ShowHisData(TEMP2_ID);
}

//温区3数据
void HisData::pbTemp3_Reless()
{
	HisData_ShowHisData(TEMP3_ID);
}
//温区4数据
void HisData::pbTemp4_Reless()
{
	HisData_ShowHisData(TEMP4_ID);
}
//温区5数据
void HisData::pbTemp5_Reless()
{
	HisData_ShowHisData(TEMP5_ID);
}

//数据导出
void HisData::pbDataOut_Reless()
{ \
   QString path_desk = "/run/media/sda1/";

   QDateTime time= QDateTime::currentDateTime();
   QString strDate= time.toString("yyyy-MM-dd-hh-mm-ss");


   if(true == copyFileToPath(FILE_HIS_DATA,path_desk + strDate+"test.db",true))
   {\
   //QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),);
        QMessageBox::information(this,"提示","数据导出完成",NULL, NULL);
   }
   else
   {
        QMessageBox::information(this,"提示","数据导出失败",NULL, NULL);
   }
}
