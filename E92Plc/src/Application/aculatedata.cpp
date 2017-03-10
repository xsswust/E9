#include "aculatedata.h"
#include "ui_aculatedata.h"
//#include "mainwindow.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
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

