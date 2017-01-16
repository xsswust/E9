#include "testmode.h"
#include "ui_testmode.h"
#include "mainwindow.h"
#include "testing.h"
#include "testingdis.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QSettings>
#include "common.h"
#include  "stdio.h"

testmode *ptestMode;

unsigned int gTest_Mode = NULL_ID;  // 记录 测试哪一种菌

testmode::testmode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testmode)
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
    //
    testMode_flag = 0;
    //Font Setting
#if TEST_ONLY_TPC
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

    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

	exit_flag = false;
}

testmode::~testmode()
{
    delete ui;
}
/*
	函数名称：
	函数功能：界面退出
	输入参数：
	输出参数：
	修改记录：
*/
void testmode::on_pbExit_clicked()
{
//	getFullScree("testmod.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		this->close();
        //pmainwin->MainWindow_Init(); // 恢复状态
	}
}
/*
	函数名称：
	函数功能：进入测试界面
	输入参数：mode   哪一种菌的测试
	输出参数：
	修改记录：
*/
void testmode::go_test(int mode)
{

	gTest_Mode = mode;

	Testing *pTest = new Testing();
	pTest->Testing_Init();
	pTest->showFullScreen();
}

/*
    函数名称：
	函数功能：指标选择 菌落总数模式
	输入参数：NULL
    输出参数：
    修改记录：
*/
void testmode::on_pbTolColi_clicked()
{
	go_test(TOL_COLI_ID);
#if 0
	QSettings settings(TEST_MODE_FILE, QSettings::IniFormat);
    settings.beginGroup("GroupTestMode");
    settings.setValue("mode","1");
    settings.endGroup();

    Testing *pTest = new Testing();
    pTest->showFullScreen();
#endif
}
/*
	函数名称：
	函数功能：指标选择 耐热大肠模式
	输入参数：
	输出参数：
	修改记录：
*/
void testmode::on_pbFecColi_clicked()
{
	go_test(FEC_COLI_ID);
#if 0
    QSettings settings(FileTestMode, QSettings::IniFormat);
    settings.beginGroup("GroupTestMode");
    settings.setValue("mode","2");
    settings.endGroup();
    Testing *pTest = new Testing();
    pTest->showFullScreen();
#endif
}
/*
	函数名称：
	函数功能：	指标选择菌落总数 模式
	输入参数：
	输出参数：
	修改记录：
*/
void testmode::on_pbTPC_clicked()
{
	go_test(TPC_COLI_ID);

#if 0
    QSettings settings(FileTestMode, QSettings::IniFormat);
    settings.beginGroup("GroupTestMode");
    settings.setValue("mode","3");
    settings.endGroup();
    Testing *pTest = new Testing();
    pTest->showFullScreen();
#endif
}
/*
	函数名称：
	函数功能：指标选择 艾希氏菌测试模式
	输入参数：
	输出参数：
	修改记录：
*/
void testmode::on_pbEColi_clicked()
{
	go_test(E_COLI_ID);
#if 0
    TestingDis *PTestDis = new TestingDis();
	PTestDis->TestDis_Init();
    PTestDis->showFullScreen();
#endif
}
