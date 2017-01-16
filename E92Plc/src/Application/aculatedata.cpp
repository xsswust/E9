#include "aculatedata.h"
#include "ui_aculatedata.h"
//#include "mainwindow.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
#include <stdio.h>
#include <QFile>
#include <QTextCodec>
#include <QFont>
#include <QPalette>
#include <QSettings>
#include <QDebug>
#include "common.h"

#include "testing_thread.h"
#include "sql_control.h"


AculateData::AculateData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AculateData)
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
  //  QTextCodec::setCodecForCStrings(gbk);
    //Initial Para
	//acuratestate_flag = false;
	//targetType = 0;
	//bottleNo = 0;
    //Read Para
#if 0
	QFile fconfiini(FILE_ACURATE_DENSITY);
    if (!fconfiini.exists())
    {
        WriteIniPara();
    }
	ReadCurrentAcurate();
#endif
    //Font Setting
	ui->lb_Title->setFont(QFont(FONT_USE_WQY,FONT_20,QFont::Normal));  // 标题
	//ui->lb_Title->setDisabled(true);

	ui->lb_BottleID->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); // 瓶号
	ui->lb_Group1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  1
	ui->lb_Group2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  2
	ui->lb_Group3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  3
	ui->lb_Group4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  4
	ui->lb_Group5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  5
	// 瓶号
	ui->lb_Bottle1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  1
	ui->lb_Bottle2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  2
	ui->lb_Bottle3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  3
	ui->lb_Bottle4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  4
	ui->lb_Bottle5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal)); //  5

	//1 group
	ui->le_Group1_1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group1_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group1_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group1_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group1_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	//2 group
	ui->le_Group2_1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group2_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group2_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group2_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group2_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	//3 group
	ui->le_Group3_1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group3_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group3_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group3_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group3_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 4 group
	ui->le_Group4_1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group4_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group4_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group4_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group4_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 5 group
	ui->le_Group5_1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group5_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group5_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group5_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Group5_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	//ui->pbDensity->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	//ui->pbStatus->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	// button 字体
	ui->pbExit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbStat->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    //Sigles and Slots
	//connect(pcomtran_thread,SIGNAL(emitAcurateDensity()),this,SLOT(ReademitAcurate()));
    //connect(pcomtran_thread,SIGNAL(emitAcurateStatus()),this,SLOT(ReademitStatus()));

	Init();
}

AculateData::~AculateData()
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
void AculateData::Init()
{
	exit_flag = false;
	st_flag = false;        // 默认是 状态
	ui->pbStat->setText(STR_BOTTLE_TR);
	ptimerUpdateStat = new QTimer();

	connect(ptimerUpdateStat,SIGNAL(timeout()),this,SLOT(timeUpdate()));
	connect(ui->pbStat,SIGNAL(released()),this,SLOT(pb_change_st()));

	ShowStat();
	ptimerUpdateStat->start(TIMER_LEN_500MS);   // 时间显示 500MS
}


void AculateData::UnInit()
{

	disconnect(ptimerUpdateStat,SIGNAL(timeout()),this,SLOT(timeUpdate()));


	ptimerUpdateStat->stop();   // 时间显示 500MS
	qDebug()<<("=====de_init============\r\n");
	delete ptimerUpdateStat;

}


void AculateData::timeUpdate()
{
	if(false == st_flag)
	{
		ShowStat(); // 显示 状态
	}
	else
	{
		showTransmission();
	}
}

void AculateData::pb_change_st()		 // 更新状态
{
	if(operator ==(ui->pbStat->text(),QString(STR_BOTTLE_TR)))  //开始测试
	{
		st_flag = true;
		ui->pbStat->setText(STR_BOTTLE_ST);
	}
	else
	{
		st_flag = false;
		ui->pbStat->setText(STR_BOTTLE_TR);
	}
}

// 显示透过率
void AculateData::showTransmission()
{
		// group 1

		if(TEST_ST_TESTING ==  gst_Test_Info[0 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_1->setText(QString::number(gst_Test_Info[0 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[0 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_1->setText(QString::number(gst_Test_Info[0 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[0 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_1->setText(QString::number(gst_Test_Info[0 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[0 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group1_1->setText(QString::number(gst_Test_Info[0 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group1_1->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[1 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_2->setText(QString::number(gst_Test_Info[1 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[1 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_2->setText(QString::number(gst_Test_Info[1 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[1 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_2->setText(QString::number(gst_Test_Info[1 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[1 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group1_2->setText(QString::number(gst_Test_Info[1 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group1_2->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[2 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_3->setText(QString::number(gst_Test_Info[2 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[2 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_3->setText(QString::number(gst_Test_Info[2 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[2 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_3->setText(QString::number(gst_Test_Info[2 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[2 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group1_3->setText(QString::number(gst_Test_Info[2 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group1_3->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[3 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_4->setText(QString::number(gst_Test_Info[3 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[3 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_4->setText(QString::number(gst_Test_Info[3 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[3 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_4->setText(QString::number(gst_Test_Info[3 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[3 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group1_4->setText(QString::number(gst_Test_Info[3 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group1_4->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[4 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_5->setText(QString::number(gst_Test_Info[4 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[4 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_5->setText(QString::number(gst_Test_Info[4 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[4 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group1_5->setText(QString::number(gst_Test_Info[4 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[4 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group1_5->setText(QString::number(gst_Test_Info[4 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group1_5->setText("100");
		}


		// group 2
		if(TEST_ST_TESTING ==  gst_Test_Info[5 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_1->setText(QString::number(gst_Test_Info[5 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[5 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_1->setText(QString::number(gst_Test_Info[5 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[5 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_1->setText(QString::number(gst_Test_Info[5 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[5 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group2_1->setText(QString::number(gst_Test_Info[5 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group2_1->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[6 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_2->setText(QString::number(gst_Test_Info[6 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[6 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_2->setText(QString::number(gst_Test_Info[6 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[6 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_2->setText(QString::number(gst_Test_Info[6 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[6 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group2_2->setText(QString::number(gst_Test_Info[6 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group2_2->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[7 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_3->setText(QString::number(gst_Test_Info[7 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[7 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_3->setText(QString::number(gst_Test_Info[7 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[7 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_3->setText(QString::number(gst_Test_Info[7 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[7 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group2_3->setText(QString::number(gst_Test_Info[7 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group2_3->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[8 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_4->setText(QString::number(gst_Test_Info[8 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[8 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_4->setText(QString::number(gst_Test_Info[8 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[8 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_4->setText(QString::number(gst_Test_Info[8 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[8 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group2_4->setText(QString::number(gst_Test_Info[8 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group2_4->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[9 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_5->setText(QString::number(gst_Test_Info[9 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[9 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_5->setText(QString::number(gst_Test_Info[9 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[9 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group2_5->setText(QString::number(gst_Test_Info[9 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[9 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group2_5->setText(QString::number(gst_Test_Info[9 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group2_5->setText("100");
		}


		// group 3
		if(TEST_ST_TESTING ==  gst_Test_Info[10 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_1->setText(QString::number(gst_Test_Info[10 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[10 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_1->setText(QString::number(gst_Test_Info[10 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[10 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_1->setText(QString::number(gst_Test_Info[10 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[10 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group3_1->setText(QString::number(gst_Test_Info[10 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group3_1->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[11 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_2->setText(QString::number(gst_Test_Info[11 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[11 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_2->setText(QString::number(gst_Test_Info[11 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[11 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_2->setText(QString::number(gst_Test_Info[11 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[11 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group3_2->setText(QString::number(gst_Test_Info[11 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group3_2->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[12 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_3->setText(QString::number(gst_Test_Info[12 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[12 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_3->setText(QString::number(gst_Test_Info[12 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[12 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_3->setText(QString::number(gst_Test_Info[12 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[12 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group3_3->setText(QString::number(gst_Test_Info[12 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group3_3->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[13 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_4->setText(QString::number(gst_Test_Info[13 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[13 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_4->setText(QString::number(gst_Test_Info[13 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[13 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_4->setText(QString::number(gst_Test_Info[13 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[13 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group3_4->setText(QString::number(gst_Test_Info[13 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group3_4->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[14 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_5->setText(QString::number(gst_Test_Info[14 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[14 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_5->setText(QString::number(gst_Test_Info[14 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[14 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group3_5->setText(QString::number(gst_Test_Info[14 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[14 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group3_5->setText(QString::number(gst_Test_Info[14 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group3_5->setText("100");
		}


		// group 4
		if(TEST_ST_TESTING ==  gst_Test_Info[15 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_1->setText(QString::number(gst_Test_Info[15 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[15 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_1->setText(QString::number(gst_Test_Info[15 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[15 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_1->setText(QString::number(gst_Test_Info[15 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[15 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group4_1->setText(QString::number(gst_Test_Info[15 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group4_1->setText("100");
		}


		if(TEST_ST_TESTING ==  gst_Test_Info[16 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_2->setText(QString::number(gst_Test_Info[16 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[16 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_2->setText(QString::number(gst_Test_Info[16 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[16 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_2->setText(QString::number(gst_Test_Info[16 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[16 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group4_2->setText(QString::number(gst_Test_Info[16 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group4_2->setText("100");
		}


		if(TEST_ST_TESTING ==  gst_Test_Info[17 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_3->setText(QString::number(gst_Test_Info[17 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[17 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_3->setText(QString::number(gst_Test_Info[17 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[17 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_3->setText(QString::number(gst_Test_Info[17 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[17 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group4_3->setText(QString::number(gst_Test_Info[17 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group4_3->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[18 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_4->setText(QString::number(gst_Test_Info[18 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[18 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_4->setText(QString::number(gst_Test_Info[18 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[18 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_4->setText(QString::number(gst_Test_Info[18 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[18 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group4_4->setText(QString::number(gst_Test_Info[18 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group4_4->setText("100");
		}

		if(TEST_ST_TESTING ==  gst_Test_Info[19 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_5->setText(QString::number(gst_Test_Info[19 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[19 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_5->setText(QString::number(gst_Test_Info[19 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[19 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group4_5->setText(QString::number(gst_Test_Info[19 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[19 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group4_5->setText(QString::number(gst_Test_Info[19 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group4_5->setText("100");
		}

		// group 5
		if(TEST_ST_TESTING ==  gst_Test_Info[20 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_1->setText(QString::number(gst_Test_Info[20 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[20 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_1->setText(QString::number(gst_Test_Info[20 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[20 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_1->setText(QString::number(gst_Test_Info[20 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[20 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group5_1->setText(QString::number(gst_Test_Info[20 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group5_1->setText("100");
		}


		if(TEST_ST_TESTING ==  gst_Test_Info[21 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_2->setText(QString::number(gst_Test_Info[21 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[21 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_2->setText(QString::number(gst_Test_Info[21 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[21 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_2->setText(QString::number(gst_Test_Info[21 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[21 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group5_2->setText(QString::number(gst_Test_Info[21 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group5_2->setText("100");
		}


		if(TEST_ST_TESTING ==  gst_Test_Info[22 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_3->setText(QString::number(gst_Test_Info[22 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[22 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_3->setText(QString::number(gst_Test_Info[22 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[22 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_3->setText(QString::number(gst_Test_Info[22 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[22 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group5_3->setText(QString::number(gst_Test_Info[22 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group5_3->setText("100");
		}


		if(TEST_ST_TESTING ==  gst_Test_Info[23 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_4->setText(QString::number(gst_Test_Info[23 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[23 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_4->setText(QString::number(gst_Test_Info[23 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[23 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_4->setText(QString::number(gst_Test_Info[23 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[23 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group5_4->setText(QString::number(gst_Test_Info[23 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group5_4->setText("100");
		}


		if(TEST_ST_TESTING ==  gst_Test_Info[24 + TOLCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_5->setText(QString::number(gst_Test_Info[24 + TOLCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[24 + FECCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_5->setText(QString::number(gst_Test_Info[24 + FECCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[24 + TPCCOLI_START_NUM].Test_st)
		{
			ui->le_Group5_5->setText(QString::number(gst_Test_Info[24 + TPCCOLI_START_NUM].ThouData_dis));
		}
		else if(TEST_ST_TESTING ==  gst_Test_Info[24 + ECOLI_START_NUM].Test_st)
		{
			ui->le_Group5_5->setText(QString::number(gst_Test_Info[24 + ECOLI_START_NUM].ThouData_dis));
		}
		else
		{
			ui->le_Group5_5->setText("100");
		}


}
/*
    函数名称：
    函数功能：返回主页面
    输入参数：
    输出参数：
    修改记录：
*/
void AculateData::on_pbExit_clicked()
{
	//QScreen *screen = QGuiApplication::primaryScreen();
	//screen->grabWindow(0).save("st.jpg","jpg");
//	getFullScree("st.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		UnInit();
		this->close();
	}
}

//显示状态
void AculateData::ShowStat(void)
{
	// group 1
	if((TEST_ST_TESTING ==  gst_Test_Info[0+TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[0 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[0 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[0 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group1_1->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[0+TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[0 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[0 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[0 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group1_1->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group1_1->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[1 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[1 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[1 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[1 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group1_2->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[1 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[1 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[1 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[1 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group1_2->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group1_2->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[4 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[4 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[4 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[4 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group1_5->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[4 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[4 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[4 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[4 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group1_5->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group1_5->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[2 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[2 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[2 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[2 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group1_3->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[2 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[2 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[2 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[2 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group1_3->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group1_3->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[3 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[3 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[3 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[3 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group1_4->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[3 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[3 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[3 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[3 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group1_4->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group1_4->setText(STR_TEST_ST_IDLE);
	}

	// group 2
	if((TEST_ST_TESTING ==  gst_Test_Info[5 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[5 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[5 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[5 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group2_1->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[5 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[5 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[5 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[5 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group2_1->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group2_1->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[6 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[6 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[6 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[6 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group2_2->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[6 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[6 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[6 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[6 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group2_2->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group2_2->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[7 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[7 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[7 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[7 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group2_3->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[7 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[7 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[7 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[7 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group2_3->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group2_3->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[8 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[8 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[8 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[8 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group2_4->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[8 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[8 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[8 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[8 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group2_4->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group2_4->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[9 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[9 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[9 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[9 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group2_5->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[9 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[9 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[9 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[9 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group2_5->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group2_5->setText(STR_TEST_ST_IDLE);
	}

	// group 3
	if((TEST_ST_TESTING ==  gst_Test_Info[10 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[10 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[10 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[10 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group3_1->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[10 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[10 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[10 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[10 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group3_1->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group3_1->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[11 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[11 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[11 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[11 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group3_2->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[11 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[11 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[11 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[11 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group3_2->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group3_2->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[12 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[12 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[12 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[12 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group3_3->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[12 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[12 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[12 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[12 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group3_3->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group3_3->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[13 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[13 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[13 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[13 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group3_4->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[13 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[13 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[13 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[13 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group3_4->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group3_4->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[14 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[14 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[14 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[14 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group3_5->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[14 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[14 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[14 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[14 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group3_5->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group3_5->setText(STR_TEST_ST_IDLE);
	}
	// group 4
	if((TEST_ST_TESTING ==  gst_Test_Info[15 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[15 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[15 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[15 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group4_1->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[15 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[15 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[15 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[15 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group4_1->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group4_1->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[16 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[16 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[16 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[16 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group4_2->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[16 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[16 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[16 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[16 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group4_2->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group4_2->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[17 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[17 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[17 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[17 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group4_3->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[17 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[17 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[17 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[17 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group4_3->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group4_3->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[18 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[18 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[18 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[18 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group4_4->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[18 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[18 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[18 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[18 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group4_4->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group4_4->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[19 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[19 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[19 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[19 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group4_5->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[19 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[19 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[19 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[19 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group4_5->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group4_5->setText(STR_TEST_ST_IDLE);
	}

	// group 5
	if((TEST_ST_TESTING ==  gst_Test_Info[20 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[20 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[20 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[20 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group5_1->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[20 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[20 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[20 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[20 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group5_1->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group5_1->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[21 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[21 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[21 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[21 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group5_2->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[21 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[21 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[21 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[21 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group5_2->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group5_2->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[22 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[22 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[22 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[22 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group5_3->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[22 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[22 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[22 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[22 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group5_3->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group5_3->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[23 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[23 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[23 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[23 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group5_4->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[23 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[23 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[23 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[23 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group5_4->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group5_4->setText(STR_TEST_ST_IDLE);
	}

	if((TEST_ST_TESTING ==  gst_Test_Info[24 + TOLCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[24 + FECCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[24 + TPCCOLI_START_NUM].Test_st)||
		(TEST_ST_TESTING ==  gst_Test_Info[24 + ECOLI_START_NUM].Test_st)	)
	{
		ui->le_Group5_5->setText(STR_TEST_ST_TESTING);
	}
	else if((TEST_ST_ZERO ==  gst_Test_Info[24 + TOLCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[24 + FECCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[24 + TPCCOLI_START_NUM].Test_st)||
			(TEST_ST_ZERO ==  gst_Test_Info[24 + ECOLI_START_NUM].Test_st))
	{
		ui->le_Group5_5->setText(STR_TEST_ST_ZERO);
	}
	else
	{
		ui->le_Group5_5->setText(STR_TEST_ST_IDLE);
	}
}


/*
    函数名称：
    函数功能：读取当前浓度
    输入参数：
    输出参数：
    修改记录：
*/
void AculateData::ReademitAcurate()
{

}

/*
    函数名称：
    函数功能：读取当前浓度
    输入参数：
    输出参数：
    修改记录：
*/
void AculateData::ReadCurrentAcurate()
{
	//gp_sql->Get_One_Data(TOL_COLI_ID, , )
	//ui->le_EColi_1->setText();
}

/*
    函数名称：
    函数功能：读取当前状态
    输入参数：
    输出参数：
    修改记录：
*/
void AculateData::ReadCurrentStatus()
{

}

