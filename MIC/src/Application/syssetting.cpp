#include "syssetting.h"
#include "ui_syssetting.h"
#include "paracalibrate.h"
#include "parasetting.h"
#include "timecalibrate.h"
#include "src/HardWare/app.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"
#include "netsetting.h"
#include "tempset.h"
#include "mainwindow.h"

SysSetting::SysSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysSetting)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND2_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // 中文显示
//    QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);
    //Font Setting
	ui->pbUpDate->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));   // 系统更新的字体
	ui->pbParaCali->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));	// 参数校验的字体
	ui->pbParaSet->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));	// 参数设置的字体
	ui->pbTimeCali->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));	// 时间设置的字体
	ui->pbNetSet->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));	// 时间设置的字体
	ui->pbExit->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));			// 返回 字体

	ui->pbTempSet->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));			// 返回 字体

	ui->lb_Title->setFont(QFont(FONT_USE_WQY,FONT_24,QFont::Normal));			// 返回 字体
	ui->lb_Title->setAlignment(Qt::AlignCenter);
	ui->lb_Title->setText("用户设置"); //
	connect(ui->pbTempSet,SIGNAL(clicked(bool)),this,SLOT(goto_TempSet()));
	exit_flag = false;
}

SysSetting::~SysSetting()
{
    delete ui;
}
// 参数设置界面
void SysSetting::on_pbParaSet_clicked()
{
    ParaSetting *pSet = new ParaSetting();
	pSet->Init();
    pSet->showFullScreen();
}
// 参数校准界面
void SysSetting::on_pbParaCali_clicked()
{
    ParaCalibrate *pCali = new ParaCalibrate();
    pCali->ParaCalibrate_Init();
    pCali->showFullScreen();
}
// 时间设置界面
void SysSetting::on_pbTimeCali_clicked()
{
    TimeCalibrate *timeCali = new TimeCalibrate();
	timeCali->init();  // 初始化
	timeCali->showFullScreen();  // 界面显示
}
// 界面退出
void SysSetting::on_pbExit_clicked()
{

//	getFullScree("sysset.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		this->close();
		usleep(10*1000);// 10ms
		pmainwin->MainWindow_Init(); // 恢复状态
	}
}
// 系统更新界面
void SysSetting::on_pbUpDate_clicked()
{
    QString path_udisk = "/run/media/sda1/";
    if(true == copyFileToPath(path_udisk+"MICROBIAL","/home/root/MICROBIAL",true))
    {\

         QMessageBox::information(this,"提示","程序更新成功",NULL, NULL);
    }
    else
    {
         QMessageBox::information(this,"提示","程序更新失败",NULL, NULL);
    }
}
// 网络设置
void SysSetting::on_pbNetSet_clicked()
{
	NetSetting *Netset = new NetSetting();
	Netset->NetSetting_init();  // 初始化
	Netset->showFullScreen();  // 界面显示

	//delete Netset;
}

void SysSetting::goto_TempSet()
{
	TempSet *ptempSet = new TempSet();
	ptempSet->showFullScreen();

	//delete ptempSet;
}
