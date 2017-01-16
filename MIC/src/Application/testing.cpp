#include "testing.h"
#include "ui_testing.h"
#include "src/HardWare/app.h"
#include "src/HardWare/keyboard_dependence.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
#include "NumberKeyBoard.h"
#include "testmode.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QDebug>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include "common.h"

Testing *pTesting;

extern unsigned int gTest_Mode;  // 记录 测试哪一种菌


Testing::Testing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Testing)
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
#if 0
    QFile fconfiini(testParaConfigFile);
    if (!fconfiini.exists())
    {
        WriteIniPara();
    }
	 ReadCurrentPara();
#endif
	//Font Setting  返回按钮 字体
	ui->pbExit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 各瓶的按钮 字体
	ui->pb_Bottle1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Bottle2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Bottle3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Bottle4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Bottle5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Prev->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Next->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 时间 日期显示 字体
	ui->leDataTime->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	//各瓶的瓶号显示
	ui->lb_Bottle1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 各瓶样本号显示
	ui->lbSampleText1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lbSampleText2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lbSampleText3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lbSampleText4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lbSampleText5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 输入样本号显示
	ui->leSampleNo1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leSampleNo2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leSampleNo3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leSampleNo4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leSampleNo5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	// 瓶状态显示
	ui->lb_Bottle1_St->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle1_St->setAlignment(Qt::AlignCenter);

	ui->lb_Bottle2_St->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle2_St->setAlignment(Qt::AlignCenter);

	ui->lb_Bottle3_St->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle3_St->setAlignment(Qt::AlignCenter);

	ui->lb_Bottle4_St->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle4_St->setAlignment(Qt::AlignCenter);

	ui->lb_Bottle5_St->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lb_Bottle5_St->setAlignment(Qt::AlignCenter);

	ui->lb_Title->setFont(QFont(FONT_USE_WQY,24,QFont::Normal));
	//设定TextEdit中文字的对齐方式为居中显示
	ui->lb_Title->setAlignment(Qt::AlignCenter);
	// 设置字体颜色
	QPalette pe;
	pe.setColor(QPalette::WindowText,Qt::red);
	ui->lb_Title->setPalette(pe);

    //参数选择
    ui->Bottle1ParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle2ParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle3ParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle4ParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle5ParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

    ui->Bottle1_ClassNo1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle2_ClassNo1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle3_ClassNo1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle4_ClassNo1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->Bottle5_ClassNo1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->lbParaChoose->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->lbParaChoose_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->lbParaChoose_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->lbParaChoose_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->lbParaChoose_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

#if 0  // 获取桌面信息
	// 标题居中显示
	QDesktopWidget *desktop=QApplication::desktop();
	//获取桌面宽度
	int w = desktop->width();
	//获取桌面高度
	int h = desktop->height();
#endif


	ui->lbLable->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leTempDis->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->lbTemp1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->lbTempDW1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));


}
//  析构函数
Testing::~Testing()
{
    delete ui;
}



/*
	函数名称：
	函数功能：显示瓶的状态
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::show_Bottle_St(int bottleId, int st, int sampleId, int mode)
{

	//qDebug()<<"[show_Bottle_St]= g_start_num = "<<g_Start_num<<"bottleId = "<<bottleId<<"num = "<<g_Start_num*MAX_ONE_GROUP_NUM + 1;
	switch(bottleId)
	{
		case BOTTLE1_ID:
		ui->lb_Bottle1->setText(QString("%1%2").arg(g_Start_num*MAX_ONE_GROUP_NUM + 1).arg(STR_TEST_BOTTLE_NUM));
		break;

		case BOTTLE2_ID:
		ui->lb_Bottle2->setText(QString("%1%2").arg(g_Start_num*MAX_ONE_GROUP_NUM + 2).arg(STR_TEST_BOTTLE_NUM));
		break;

		case BOTTLE3_ID:
		ui->lb_Bottle3->setText(QString("%1%2").arg(g_Start_num*MAX_ONE_GROUP_NUM + 3).arg(STR_TEST_BOTTLE_NUM));
		break;

		case BOTTLE4_ID:
		ui->lb_Bottle4->setText(QString("%1%2").arg(g_Start_num*MAX_ONE_GROUP_NUM + 4).arg(STR_TEST_BOTTLE_NUM));
		break;

		case BOTTLE5_ID:
		ui->lb_Bottle5->setText(QString("%1%2").arg(g_Start_num*MAX_ONE_GROUP_NUM + 5).arg(STR_TEST_BOTTLE_NUM));
		break;

		default:
		break;
	}

	if(TEST_ST_IDEL == st)
	{
		switch(bottleId)
		{
		case BOTTLE1_ID:
			ui->lb_Bottle1_St->setText(STR_TEST_ST_IDLE);
			ui->pb_Bottle1->setText(STR_TEST_START);
			ui->leSampleNo1->setDisabled(false);
			ui->pb_Bottle1->setDisabled(false);
			//ui->leSampleNo1->setText(QString::number(sampleId));
			ui->leSampleNo1->setFocusPolicy(Qt::ClickFocus);
			ui->leSampleNo1->installEventFilter(im);\

			break;

		case BOTTLE2_ID:
			ui->lb_Bottle2_St->setText(STR_TEST_ST_IDLE);
			ui->pb_Bottle2->setText(STR_TEST_START);
			ui->leSampleNo2->setDisabled(false);
			ui->pb_Bottle2->setDisabled(false);
			//ui->leSampleNo2->setText(QString::number(sampleId));
			ui->leSampleNo2->setFocusPolicy(Qt::ClickFocus);
			ui->leSampleNo2->installEventFilter(im);

			break;

		case BOTTLE3_ID:
			ui->lb_Bottle3_St->setText(STR_TEST_ST_IDLE);
			ui->pb_Bottle3->setText(STR_TEST_START);
			ui->leSampleNo3->setDisabled(false);
			ui->pb_Bottle3->setDisabled(false);
			//ui->leSampleNo3->setText(QString::number(sampleId));
			ui->leSampleNo3->setFocusPolicy(Qt::ClickFocus);
			ui->leSampleNo3->installEventFilter(im);
			break;

		case BOTTLE4_ID:
			ui->lb_Bottle4_St->setText(STR_TEST_ST_IDLE);
			ui->pb_Bottle4->setText(STR_TEST_START);
			ui->leSampleNo4->setDisabled(false);
			ui->pb_Bottle4->setDisabled(false);
			//ui->leSampleNo4->setText(QString::number(sampleId));
			ui->leSampleNo4->setFocusPolicy(Qt::ClickFocus);
			ui->leSampleNo4->installEventFilter(im);
			break;

		case BOTTLE5_ID:
			ui->lb_Bottle5_St->setText(STR_TEST_ST_IDLE);
			ui->pb_Bottle5->setText(STR_TEST_START);
			ui->leSampleNo5->setDisabled(false);
			ui->pb_Bottle5->setDisabled(false);
			//ui->leSampleNo5->setText(QString::number(sampleId));
			ui->leSampleNo5->setFocusPolicy(Qt::ClickFocus);
			ui->leSampleNo5->installEventFilter(im);
			break;

		default:
			break;
		}
	}
	else if(TEST_ST_ZERO == st)
	{
		switch(bottleId)
		{
		case BOTTLE1_ID:
			ui->lb_Bottle1_St->setText(STR_TEST_ST_ZERO);
			ui->pb_Bottle1->setText(STR_TEST_STOP);
			ui->leSampleNo1->setDisabled(true);
			ui->leSampleNo1->setText(QString::number(sampleId));
			//ui->leSampleNo1->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo1->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle1->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle1->setDisabled(false);
			}
			break;

		case BOTTLE2_ID:
			ui->lb_Bottle2_St->setText(STR_TEST_ST_ZERO);
			ui->pb_Bottle2->setText(STR_TEST_STOP);
			ui->leSampleNo2->setDisabled(true);
			ui->leSampleNo2->setText(QString::number(sampleId));
			//ui->leSampleNo2->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo2->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle2->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle2->setDisabled(false);
			}
			break;

		case BOTTLE3_ID:
			ui->lb_Bottle3_St->setText(STR_TEST_ST_ZERO);
			ui->pb_Bottle3->setText(STR_TEST_STOP);
			ui->leSampleNo3->setDisabled(true);
			ui->leSampleNo3->setText(QString::number(sampleId));
			//ui->leSampleNo3->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo3->removeEventFilter(im);

			if(true == mode)
			{
				ui->pb_Bottle3->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle3->setDisabled(false);
			}
			break;

		case BOTTLE4_ID:
			ui->lb_Bottle4_St->setText(STR_TEST_ST_ZERO);
			ui->pb_Bottle4->setText(STR_TEST_STOP);
			ui->leSampleNo4->setDisabled(true);
			ui->leSampleNo4->setText(QString::number(sampleId));
			//ui->leSampleNo4->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo4->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle4->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle4->setDisabled(false);
			}
			break;

		case BOTTLE5_ID:
			ui->lb_Bottle5_St->setText(STR_TEST_ST_ZERO);
			ui->pb_Bottle5->setText(STR_TEST_STOP);
			ui->leSampleNo5->setDisabled(true);
			ui->leSampleNo5->setText(QString::number(sampleId));
			//ui->leSampleNo5->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo5->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle5->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle5->setDisabled(false);
			}
			break;

		default:
			break;
		}

	}
	else if(TEST_ST_TESTING == st)
	{
		//qDebug()<<"bottleId = "<<bottleId<<"mode = "<<mode;
		switch(bottleId)
		{
		case BOTTLE1_ID:
			ui->lb_Bottle1_St->setText(STR_TEST_ST_TESTING);
			ui->pb_Bottle1->setText(STR_TEST_STOP);
			ui->leSampleNo1->setDisabled(true);
			ui->leSampleNo1->setText(QString::number(sampleId));
			//ui->leSampleNo1->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo1->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle1->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle1->setDisabled(false);
			}
			break;

		case BOTTLE2_ID:
			ui->lb_Bottle2_St->setText(STR_TEST_ST_TESTING);
			ui->pb_Bottle2->setText(STR_TEST_STOP);
			ui->leSampleNo2->setDisabled(true);
			ui->leSampleNo2->setText(QString::number(sampleId));
			//ui->leSampleNo2->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo2->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle2->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle2->setDisabled(false);
			}
			break;

		case BOTTLE3_ID:
			ui->lb_Bottle3_St->setText(STR_TEST_ST_TESTING);
			ui->pb_Bottle3->setText(STR_TEST_STOP);
			ui->leSampleNo3->setDisabled(true);
			ui->leSampleNo3->setText(QString::number(sampleId));
			//ui->leSampleNo3->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo3->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle3->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle3->setDisabled(false);
			}
			break;

		case BOTTLE4_ID:
			ui->lb_Bottle4_St->setText(STR_TEST_ST_TESTING);
			ui->pb_Bottle4->setText(STR_TEST_STOP);
			ui->leSampleNo4->setDisabled(true);
			ui->leSampleNo4->setText(QString::number(sampleId));
			//ui->leSampleNo4->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo4->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle4->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle4->setDisabled(false);
			}
			break;

		case BOTTLE5_ID:
			ui->lb_Bottle5_St->setText(STR_TEST_ST_TESTING);
			ui->pb_Bottle5->setText(STR_TEST_STOP);
			ui->leSampleNo5->setDisabled(true);
			ui->leSampleNo5->setText(QString::number(sampleId));
			//ui->leSampleNo5->setFocusPolicy(Qt::TabFocus);
			ui->leSampleNo5->removeEventFilter(im);
			if(true == mode)
			{
				ui->pb_Bottle5->setDisabled(true);
			}
			else
			{
				ui->pb_Bottle5->setDisabled(false);
			}
			break;

		default:
			break;
		}

	}
}

/*
	函数名称：
	函数功能：界面初始化处理
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::Testing_Init()
{
	exit_flag = false;
	im = new Keyboard_dependence;  // 新建键盘
	ui->leSampleNo1->setFocusPolicy(Qt::ClickFocus);
	ui->leSampleNo1->installEventFilter(im);

	ui->leSampleNo2->setFocusPolicy(Qt::ClickFocus);
	ui->leSampleNo2->installEventFilter(im);

	ui->leSampleNo3->setFocusPolicy(Qt::ClickFocus);
	ui->leSampleNo3->installEventFilter(im);

	ui->leSampleNo4->setFocusPolicy(Qt::ClickFocus);
	ui->leSampleNo4->installEventFilter(im);

	ui->leSampleNo5->setFocusPolicy(Qt::ClickFocus);
	ui->leSampleNo5->installEventFilter(im);


	// 显示标题
	if(TOL_COLI_ID == gTest_Mode){
		ui->lb_Title->setText(STR_TEST_TOLCOLI);
	}
	else if(FEC_COLI_ID == gTest_Mode){
		ui->lb_Title->setText(STR_TEST_FECCOLI);
	}
	else if(TPC_COLI_ID == gTest_Mode){
		ui->lb_Title->setText(STR_TEST_TPCCOLI);
	}
	else if(E_COLI_ID == gTest_Mode){
		ui->lb_Title->setText(STR_TEST_ECOLI);
	}


	// 定时器
	pTesttimerTime = new QTimer();
	// 建立 信号和槽
	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	connect(pTesttimerTime,SIGNAL(timeout()),this,SLOT(timeUpData()));
	connect(ui->pb_Prev,SIGNAL(released()),this,SLOT(Button_Prev_clicked()));  // 上一组
	connect(ui->pb_Next,SIGNAL(released()),this,SLOT(Button_Next_clicked()));   // 下一组

	connect(ui->pb_Bottle1,SIGNAL(released()),this,SLOT(pb_Bottle1_Reless()));   // 瓶1
	connect(ui->pb_Bottle2,SIGNAL(released()),this,SLOT(pb_Bottle2_Reless()));   // 瓶2
	connect(ui->pb_Bottle3,SIGNAL(released()),this,SLOT(pb_Bottle3_Reless()));   // 瓶3
	connect(ui->pb_Bottle4,SIGNAL(released()),this,SLOT(pb_Bottle4_Reless()));   // 瓶4
	connect(ui->pb_Bottle5,SIGNAL(released()),this,SLOT(pb_Bottle5_Reless()));   // 瓶5

	ui->pb_Prev->hide();  // 隐藏
	ui->pb_Next->show();	// 显示

	g_Start_num = 0;   // 默认是第一屏
	pTesttimerTime->start(TIMER_LEN_200MS);  //500ms 定时器
	timeUpData();  // 显示时间
    showParaChoose();
}
/*
	函数名称：
	函数功能：界面初始化处理
	输入参数：
	输出参数：
	修改记录：
*/
// 界面跳转函数
void Testing::Testing_UnIinit()
{
	exit_flag = true;

	ui->leSampleNo1->removeEventFilter(im);
	ui->leSampleNo2->removeEventFilter(im);
	ui->leSampleNo3->removeEventFilter(im);
	ui->leSampleNo4->removeEventFilter(im);
	ui->leSampleNo5->removeEventFilter(im);

	disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	disconnect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	disconnect(pTesttimerTime,SIGNAL(timeout()),this,SLOT(timeUpData()));
	disconnect(ui->pb_Prev,SIGNAL(clicked()),this,SLOT(Button_Prev_clicked()));  // 上一组
	disconnect(ui->pb_Next,SIGNAL(clicked()),this,SLOT(Button_Next_clicked()));   // 下一组
	disconnect(ui->pb_Bottle1,SIGNAL(released()),this,SLOT(pb_Bottle1_Reless()));   // 瓶1
	disconnect(ui->pb_Bottle2,SIGNAL(released()),this,SLOT(pb_Bottle2_Reless()));   // 瓶2
	disconnect(ui->pb_Bottle3,SIGNAL(released()),this,SLOT(pb_Bottle3_Reless()));   // 瓶3
	disconnect(ui->pb_Bottle4,SIGNAL(released()),this,SLOT(pb_Bottle4_Reless()));   // 瓶4
	disconnect(ui->pb_Bottle5,SIGNAL(released()),this,SLOT(pb_Bottle5_Reless()));   // 瓶5

	pTesttimerTime->stop();

	delete pTesttimerTime;  // 销毁定时器
	delete im;			// 销毁 键盘
}

/*
	函数名称：
	函数功能：界面初始化处理
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::timeUpData()
{
	QDateTime time= QDateTime::currentDateTime();
	QString strDate= time.toString("yyyy-MM-dd hh:mm:ss ddd");
	ui->leDataTime->setText(strDate);
	// 上一平 下一屏 数据
	switch(g_Start_num)
	{
		case 0:
			ui->pb_Prev->hide();
			ui->pb_Next->show();
			ui->leTempDis->setText(QString::number(g_Sp1)); // 显示36.5
		break;

		case 1:
			ui->pb_Prev->show();
			ui->pb_Next->show();
			ui->leTempDis->setText(QString::number(g_Sp2)); // 显示36.5
		break;

		case 2:
			ui->pb_Prev->show();
			ui->pb_Next->show();
			ui->leTempDis->setText(QString::number(g_Sp3)); // 显示36.5
		break;

		case 3:
			ui->pb_Prev->show();
			ui->pb_Next->show();
			ui->leTempDis->setText(QString::number(g_Sp4)); // 显示36.5
		break;

		case 4:
			ui->pb_Prev->show();
			ui->pb_Next->hide();
			ui->leTempDis->setText(QString::number(g_Sp5)); // 显示36.5
		break;

		default:
		break;
	}

	// 显示瓶的状态
	if(TOL_COLI_ID == gTest_Mode)  // 总大肠
	{
		for(int i = 0; i < MAX_ONE_GROUP_NUM;i ++)
		{
//			qDebug()<<QString("gst_Test_Info[%1].Test_st = %2 ").arg(i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM).arg(gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st);
//			qDebug()<<QString("gst_Test_Info[%1].Test_st = %2 ").arg(i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM).arg(gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st);
//			qDebug()<<QString("gst_Test_Info[%1].Test_st = %2 ").arg(i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM).arg(gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st);
//			qDebug()<<QString("gst_Test_Info[%1].Test_st = %2 ").arg(i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM).arg(gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st);

			if((TEST_ST_IDEL == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
				(TEST_ST_IDEL == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
				(TEST_ST_IDEL == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
				(TEST_ST_IDEL == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				)
			{
			// 注意 瓶号是从 1开始 此处应+ 1
				show_Bottle_St(i, gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
//				qDebug()<<"11111111111111111111111111";
			}
			else if((TEST_ST_ZERO == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				if(TEST_ST_TESTING == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st){
					show_Bottle_St(i, gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_ZERO, 0, true);
				}
			}
			else
			{
//				qDebug()<<"00000000000000000000000000";
				if(TEST_ST_TESTING == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_TESTING, 0, true);
				}
			}
		}
	}
	else if(FEC_COLI_ID == gTest_Mode)  // 耐热
	{
		for(int i = 0; i < MAX_ONE_GROUP_NUM;i ++)
		{

			if((TEST_ST_IDEL == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				show_Bottle_St(i, gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
			}
			else if((TEST_ST_ZERO == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				if(TEST_ST_TESTING == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else{
					show_Bottle_St(i, TEST_ST_ZERO, 0, true);
				}
			}

			else
			{
				if(TEST_ST_TESTING == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_TESTING, 0, true);
				}
			}
		}
	}
	else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
	{
		for(int i = 0; i < MAX_ONE_GROUP_NUM;i ++)
		{

			if((TEST_ST_IDEL == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				show_Bottle_St(i, gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
			}
			else if((TEST_ST_ZERO == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				if(TEST_ST_TESTING == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_ZERO, 0, true);
				}
			}
			else
			{
				if(TEST_ST_TESTING == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_TESTING, 0, true);
				}
			}
		}
	}
	else if(E_COLI_ID == gTest_Mode)	// 埃希氏
	{
		for(int i = 0; i < MAX_ONE_GROUP_NUM;i ++)
		{

			if((TEST_ST_IDEL == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)&&
					(TEST_ST_IDEL == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				show_Bottle_St(i, gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
			}
			else if((TEST_ST_ZERO == gst_Test_Info[i+TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)||
					(TEST_ST_ZERO == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st))
			{
				if(TEST_ST_TESTING == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_ZERO, 0, true);
				}
			}
			else{
				if(TEST_ST_TESTING == gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st)
				{
					show_Bottle_St(i, gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].Test_st, gst_Test_Info[i+ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM].SampleID, false);
				}
				else
				{
					show_Bottle_St(i, TEST_ST_TESTING, 0, true);
				}
			}
		}
	}

}
/*
	函数名称：
	函数功能：键值返回 处理
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::insertValue(QString nustr)
{
	QLineEdit *p_editer;
	qDebug()<< "insertValue====11";
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->clear();
    p_editer->setText(nustr);

}
/*
	函数名称：
	函数功能：键值删除处理
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::keyboardDelete()
{
	QLineEdit *p_editer;

	qDebug()<< "keyboardDelete====11";
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->backspace();
}


/*
	函数名称：
	函数功能：界面退出
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::on_pbExit_clicked()
{
//	getFullScree("testting.jpg");
	if(false ==  exit_flag)
	{
		Testing_UnIinit();
	 this->close();
	}
}
/*
	函数名称：
	函数功能：上一组瓶
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::Button_Prev_clicked()
{

	if(g_Start_num > 0)
	{
		g_Start_num --;
	}
	else
	{
		g_Start_num = 0;
	}
	ui->leSampleNo1->setText("");
	ui->leSampleNo2->setText("");
	ui->leSampleNo3->setText("");
	ui->leSampleNo4->setText("");
	ui->leSampleNo5->setText("");
	qDebug()<<"[Button_Prev_clicked] g_Start_num === "<<g_Start_num;
     showParaChoose();
}
/*
	函数名称：
	函数功能：下一组瓶
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::Button_Next_clicked()
{
	if(g_Start_num < (MAX_ONE_GROUP_NUM -1))
	{
		g_Start_num ++;
	}
	else
	{
		g_Start_num = 0;
	}
	ui->leSampleNo1->setText("");
	ui->leSampleNo2->setText("");
	ui->leSampleNo3->setText("");
	ui->leSampleNo4->setText("");
	ui->leSampleNo5->setText("");

	qDebug()<<"g_Start_num === "<<g_Start_num;

  /*  ui->Bottle1ParaClass->setCurrentIndex(0);
    ui->Bottle2ParaClass->setCurrentIndex(0);
    ui->Bottle3ParaClass->setCurrentIndex(0);
    ui->Bottle4ParaClass->setCurrentIndex(0);
    ui->Bottle5ParaClass->setCurrentIndex(0);
    ui->Bottle1_ClassNo1->setCurrentIndex(0);
    ui->Bottle2_ClassNo1->setCurrentIndex(0);
    ui->Bottle3_ClassNo1->setCurrentIndex(0);
    ui->Bottle4_ClassNo1->setCurrentIndex(0);
    ui->Bottle5_ClassNo1->setCurrentIndex(0); */
    showParaChoose();
}

/*
	函数名称：
	函数功能：1号瓶 开始停止按键
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::pb_Bottle1_Reless()
{
	QString txt;
	int id = 0;

	txt = ui->pb_Bottle1->text();
	if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
	{

		if(TOL_COLI_ID == gTest_Mode)  // 总大肠
		{
			id = BOTTLE1_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo1->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE1_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(FEC_COLI_ID == gTest_Mode)  // 耐热
		{
			id = BOTTLE1_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo1->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE1_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
		{
			id = BOTTLE1_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo1->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE1_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(E_COLI_ID == gTest_Mode)	// 埃希氏
		{
			id = BOTTLE1_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo1->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE1_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		qDebug()<<"start gTest_Mode = "<<gTest_Mode <<"num = "<< g_Start_num*MAX_ONE_GROUP_NUM;
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:
				if(TOL_COLI_ID == gTest_Mode)  // 总大肠
				{
					id = BOTTLE1_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(FEC_COLI_ID == gTest_Mode)  // 耐热
				{
					id = BOTTLE1_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
				{
					id = BOTTLE1_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(E_COLI_ID == gTest_Mode)	// 埃希氏
				{
					id = BOTTLE1_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				break;

			case QMessageBox::Cancel:
				break;

			default:
				break;
		}

		qDebug()<<"stop";
	}
}
/*
	函数名称：
	函数功能：2号瓶 开始停止按键
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::pb_Bottle2_Reless()
{
	QString txt;
	int id = 0;

	txt = ui->pb_Bottle2->text();
	if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
	{
		if(TOL_COLI_ID == gTest_Mode)  // 总大肠
		{
			id = BOTTLE2_ID + TOLCOLI_START_NUM  + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo2->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE2_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(FEC_COLI_ID == gTest_Mode)  // 耐热
		{
			id = BOTTLE2_ID + FECCOLI_START_NUM  + g_Start_num*MAX_ONE_GROUP_NUM;

			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo2->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE2_ID+ g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
		{
			id = BOTTLE2_ID + TPCCOLI_START_NUM  + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo2->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE2_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(E_COLI_ID == gTest_Mode)	// 埃希氏
		{
			id = BOTTLE2_ID + ECOLI_START_NUM  + g_Start_num*MAX_ONE_GROUP_NUM;

			memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo2->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE2_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:
				if(TOL_COLI_ID == gTest_Mode)  // 总大肠
				{
					id = BOTTLE2_ID + TOLCOLI_START_NUM  + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(FEC_COLI_ID == gTest_Mode)  // 耐热
				{
					id = BOTTLE2_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[ id ], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
				{
					id =  BOTTLE2_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(E_COLI_ID == gTest_Mode)	// 埃希氏
				{
					id =  BOTTLE2_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				break;

			case QMessageBox::Cancel:
				break;

			default:
				break;
		}

		qDebug()<<"stop";
	}
}
/*
	函数名称：
	函数功能：3号瓶 开始停止按键
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::pb_Bottle3_Reless()
{
	QString txt;
	int id = 0;
	txt = ui->pb_Bottle3->text();
	if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
	{
		if(TOL_COLI_ID == gTest_Mode)  // 总大肠
		{
			id = BOTTLE3_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo3->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE3_ID + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(FEC_COLI_ID == gTest_Mode)  // 耐热
		{
			id = BOTTLE3_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo3->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE3_ID  + g_Start_num*MAX_ONE_GROUP_NUM;;
		}
		else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
		{
			id = BOTTLE3_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo3->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE3_ID + g_Start_num*MAX_ONE_GROUP_NUM;;
		}
		else if(E_COLI_ID == gTest_Mode)	// 埃希氏
		{
			id = BOTTLE3_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo3->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE3_ID + g_Start_num*MAX_ONE_GROUP_NUM;;
		}
		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:
				if(TOL_COLI_ID == gTest_Mode)  // 总大肠
				{
					id = BOTTLE3_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(FEC_COLI_ID == gTest_Mode)  // 耐热
				{
					id = BOTTLE3_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
				{
					id = BOTTLE3_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(E_COLI_ID == gTest_Mode)	// 埃希氏
				{
					id = BOTTLE3_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				break;

			case QMessageBox::Cancel:
				break;

			default:
				break;
		}

		qDebug()<<"stop";
	}
}
/*
	函数名称：
	函数功能：4号瓶 开始停止按键
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::pb_Bottle4_Reless()
{
	QString txt;
	int id = 0;

	txt = ui->pb_Bottle4->text();
	if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
	{
		if(TOL_COLI_ID == gTest_Mode)  // 总大肠
		{
			id = BOTTLE4_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo4->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE4_ID  + g_Start_num*MAX_ONE_GROUP_NUM;
		}
		else if(FEC_COLI_ID == gTest_Mode)  // 耐热
		{
			id = BOTTLE4_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo4->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE4_ID + g_Start_num*MAX_ONE_GROUP_NUM;

		}
		else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
		{
			id = BOTTLE4_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo4->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE4_ID  + g_Start_num*MAX_ONE_GROUP_NUM;

		}
		else if(E_COLI_ID == gTest_Mode)	// 埃希氏
		{
			id = BOTTLE4_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo4->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE4_ID + g_Start_num*MAX_ONE_GROUP_NUM;

		}
		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:
				if(TOL_COLI_ID == gTest_Mode)  // 总大肠
				{
					id = BOTTLE4_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(FEC_COLI_ID == gTest_Mode)  // 耐热
				{
					id = BOTTLE4_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
				{
					id = BOTTLE4_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(E_COLI_ID == gTest_Mode)	// 埃希氏
				{
					id = BOTTLE4_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[ id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				break;

			case QMessageBox::Cancel:
				break;

			default:
				break;
		}

		qDebug()<<"stop";
	}
}
/*
	函数名称：
	函数功能：5号瓶 开始停止按键
	输入参数：
	输出参数：
	修改记录：
*/
void Testing::pb_Bottle5_Reless()
{
	QString txt;
	int id = 0;

	txt = ui->pb_Bottle5->text();
	if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
	{
		if(TOL_COLI_ID == gTest_Mode)  // 总大肠
		{
			id = BOTTLE5_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo5->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE5_ID + g_Start_num*MAX_ONE_GROUP_NUM;

		}
		else if(FEC_COLI_ID == gTest_Mode)  // 耐热
		{
			id = BOTTLE5_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo5->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE5_ID+ g_Start_num*MAX_ONE_GROUP_NUM;

		}
		else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
		{
			id = BOTTLE5_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo5->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE5_ID+ g_Start_num*MAX_ONE_GROUP_NUM;

		}
		else if(E_COLI_ID == gTest_Mode)	// 埃希氏
		{
			id = BOTTLE5_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
			memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
			gst_Test_Info[id].Test_st = TEST_ST_ZERO;
			gst_Test_Info[id].SampleID = ui->leSampleNo5->text().toInt();
			gst_Test_Info[id].BottleId  = REAL_BOTTLE5_ID+ g_Start_num*MAX_ONE_GROUP_NUM;

		}
		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:
				if(TOL_COLI_ID == gTest_Mode)  // 总大肠
				{
					id = BOTTLE5_ID + TOLCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(FEC_COLI_ID == gTest_Mode)  // 耐热
				{
					id = BOTTLE5_ID + FECCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
				{
					id = BOTTLE5_ID + TPCCOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				else if(E_COLI_ID == gTest_Mode)	// 埃希氏
				{
					id = BOTTLE5_ID + ECOLI_START_NUM + g_Start_num*MAX_ONE_GROUP_NUM;
					memset(&gst_Test_Info[id], 0, sizeof(ST_TEST_INFO));
					gst_Test_Info[id].Test_st = TEST_ST_IDEL;
				}
				break;

			case QMessageBox::Cancel:
				break;

			default:
				break;
		}

		qDebug()<<"stop";
	}
}


void Testing::on_Bottle1ParaClass_currentIndexChanged(int index)
{   switch (g_Start_num) {
    case 0:
        Bottle1_ClassNo=index;
        ui->Bottle1ParaClass->setCurrentIndex(Bottle1_ClassNo);
        break;
    case 1:
        Bottle6_ClassNo=index;
        ui->Bottle1ParaClass->setCurrentIndex(Bottle6_ClassNo);
        break;
    case 2:
        Bottle11_ClassNo=index;
        ui->Bottle1ParaClass->setCurrentIndex(Bottle11_ClassNo);
        break;
    case 3:
        Bottle16_ClassNo=index;
        ui->Bottle1ParaClass->setCurrentIndex(Bottle16_ClassNo);
        break;
    case 4:
        Bottle21_ClassNo=index;
        ui->Bottle1ParaClass->setCurrentIndex(Bottle21_ClassNo);
        break;
    default:
        break;
    }
    
}

void Testing::on_Bottle2ParaClass_currentIndexChanged(int index)
{
    switch (g_Start_num) {
        case 0:
            Bottle2_ClassNo=index;
            ui->Bottle2ParaClass->setCurrentIndex(Bottle2_ClassNo);
            break;
        case 1:
            Bottle7_ClassNo=index;
            ui->Bottle2ParaClass->setCurrentIndex(Bottle7_ClassNo);
            break;
        case 2:
            Bottle12_ClassNo=index;
            ui->Bottle2ParaClass->setCurrentIndex(Bottle12_ClassNo);
            break;
        case 3:
            Bottle17_ClassNo=index;
            ui->Bottle2ParaClass->setCurrentIndex(Bottle17_ClassNo);
            break;
        case 4:
            Bottle22_ClassNo=index;
            ui->Bottle2ParaClass->setCurrentIndex(Bottle22_ClassNo);
            break;
        default:
            break;
        }
        
}

void Testing::on_Bottle3ParaClass_currentIndexChanged(int index)
{
    switch (g_Start_num) {
        case 0:
            Bottle3_ClassNo=index;
            ui->Bottle3ParaClass->setCurrentIndex(Bottle3_ClassNo);
            break;
        case 1:
            Bottle8_ClassNo=index;
            ui->Bottle3ParaClass->setCurrentIndex(Bottle8_ClassNo);
            break;
        case 2:
            Bottle13_ClassNo=index;
           ui->Bottle3ParaClass->setCurrentIndex(Bottle13_ClassNo);
            break;
        case 3:
            Bottle18_ClassNo=index;
            ui->Bottle3ParaClass->setCurrentIndex(Bottle18_ClassNo);
            break;
        case 4:
            Bottle23_ClassNo=index;
            ui->Bottle3ParaClass->setCurrentIndex(Bottle23_ClassNo);
            break;
        default:
            break;
        }
}

void Testing::on_Bottle4ParaClass_currentIndexChanged(int index)
{
    switch (g_Start_num) {
        case 0:
            Bottle4_ClassNo=index;
           ui->Bottle4ParaClass->setCurrentIndex(Bottle4_ClassNo);
            break;
        case 1:
            Bottle9_ClassNo=index;
            ui->Bottle4ParaClass->setCurrentIndex(Bottle9_ClassNo);
            break;
        case 2:
            Bottle14_ClassNo=index;
            ui->Bottle4ParaClass->setCurrentIndex(Bottle14_ClassNo);
            break;
        case 3:
            Bottle19_ClassNo=index;
            ui->Bottle4ParaClass->setCurrentIndex(Bottle19_ClassNo);
            break;
        case 4:
            Bottle24_ClassNo=index;
            ui->Bottle4ParaClass->setCurrentIndex(Bottle24_ClassNo);
            break;
        default:
            break;
        }
}

void Testing::on_Bottle5ParaClass_currentIndexChanged(int index)
{
    switch (g_Start_num) {
        case 0:
            Bottle5_ClassNo=index;
            ui->Bottle5ParaClass->setCurrentIndex(Bottle5_ClassNo);
            break;
        case 1:
            Bottle10_ClassNo=index;
            ui->Bottle5ParaClass->setCurrentIndex(Bottle10_ClassNo);
            break;
        case 2:
            Bottle15_ClassNo=index;
            ui->Bottle5ParaClass->setCurrentIndex(Bottle15_ClassNo);
            break;
        case 3:
            Bottle20_ClassNo=index;
            ui->Bottle5ParaClass->setCurrentIndex(Bottle20_ClassNo);
            break;
        case 4:
            Bottle25_ClassNo=index;
            ui->Bottle5ParaClass->setCurrentIndex(Bottle25_ClassNo);
            break;
        default:
            break;
        }
}

void Testing::on_Bottle1_ClassNo1_currentIndexChanged(int index)
{
     switch (g_Start_num) {
        case 0:
            Bottle1_ClassNo_1=index;
            ui->Bottle1_ClassNo1->setCurrentIndex(Bottle1_ClassNo_1);
            break;
        case 1:
            Bottle6_ClassNo_1=index;
            ui->Bottle1_ClassNo1->setCurrentIndex(Bottle6_ClassNo_1);
            break;
        case 2:
            Bottle11_ClassNo_1=index;
            ui->Bottle1_ClassNo1->setCurrentIndex(Bottle11_ClassNo_1);
            break;
        case 3:
            Bottle16_ClassNo_1=index;
            ui->Bottle1_ClassNo1->setCurrentIndex(Bottle16_ClassNo_1);
            break;
        case 4:
            Bottle21_ClassNo_1=index;
            ui->Bottle1_ClassNo1->setCurrentIndex(Bottle21_ClassNo_1);
            break;
        default:
            break;
        }
}

void Testing::on_Bottle2_ClassNo1_currentIndexChanged(int index)
{
    switch (g_Start_num) {
       case 0:
           Bottle2_ClassNo_1=index;
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle2_ClassNo_1);
           break;
       case 1:
           Bottle7_ClassNo_1=index;
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle7_ClassNo_1);
           break;
       case 2:
           Bottle12_ClassNo_1=index;
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle12_ClassNo_1);
           break;
       case 3:
           Bottle17_ClassNo_1=index;
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle17_ClassNo_1);
           break;
       case 4:
           Bottle22_ClassNo_1=index;
            ui->Bottle2_ClassNo1->setCurrentIndex(Bottle22_ClassNo_1);
           break;
       default:
           break;
       }
}

void Testing::on_Bottle3_ClassNo1_currentIndexChanged(int index)
{
    switch (g_Start_num) {
       case 0:
           Bottle3_ClassNo_1=index;
            ui->Bottle3_ClassNo1->setCurrentIndex(Bottle3_ClassNo_1);
           break;
       case 1:
           Bottle8_ClassNo_1=index;
            ui->Bottle3_ClassNo1->setCurrentIndex(Bottle8_ClassNo_1);
           break;
       case 2:
           Bottle13_ClassNo_1=index;
            ui->Bottle3_ClassNo1->setCurrentIndex(Bottle13_ClassNo_1);
           break;
       case 3:
           Bottle18_ClassNo_1=index;
            ui->Bottle3_ClassNo1->setCurrentIndex(Bottle18_ClassNo_1);
           break;
       case 4:
           Bottle23_ClassNo_1=index;
            ui->Bottle3_ClassNo1->setCurrentIndex(Bottle23_ClassNo_1);
           break;
       default:
           break;
       }
}

void Testing::on_Bottle4_ClassNo1_currentIndexChanged(int index)
{
    switch (g_Start_num) {
       case 0:
           Bottle4_ClassNo_1=index;
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle4_ClassNo_1);
           break;
       case 1:
           Bottle9_ClassNo_1=index;
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle9_ClassNo_1);
           break;
       case 2:
           Bottle14_ClassNo_1=index;
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle14_ClassNo_1);
           break;
       case 3:
           Bottle19_ClassNo_1=index;
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle19_ClassNo_1);
           break;
       case 4:
           Bottle24_ClassNo_1=index;
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle24_ClassNo_1);
           break;
       default:
           break;
       }
}

void Testing::on_Bottle5_ClassNo1_currentIndexChanged(int index)
{
    switch (g_Start_num) {
       case 0:
           Bottle5_ClassNo_1=index;
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle5_ClassNo_1);
           break;
       case 1:
           Bottle10_ClassNo_1=index;
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle10_ClassNo_1);
           break;
       case 2:
           Bottle15_ClassNo_1=index;
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle15_ClassNo_1);
           break;
       case 3:
           Bottle20_ClassNo_1=index;
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle20_ClassNo_1);
           break;
       case 4:
           Bottle25_ClassNo_1=index;
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle25_ClassNo_1);
           break;
       default:
           break;
       }
}
void Testing::showParaChoose()
{
   if(TOL_COLI_ID == gTest_Mode) //总大肠
       showParaChooseAll();
   else if(FEC_COLI_ID == gTest_Mode) //耐热
        showParaChooseAll();
   else if(TPC_COLI_ID == gTest_Mode) // 菌落总数
        showParaChooseAll();
   else if(E_COLI_ID == gTest_Mode)//埃希氏
        showParaChooseAll();
}
void Testing::showParaChooseAll()
{
    switch (g_Start_num) {
       case 0:
           ui->Bottle1ParaClass->setCurrentIndex(Bottle1_ClassNo);
           ui->Bottle2ParaClass->setCurrentIndex(Bottle2_ClassNo);
           ui->Bottle3ParaClass->setCurrentIndex(Bottle3_ClassNo);
           ui->Bottle4ParaClass->setCurrentIndex(Bottle4_ClassNo);
           ui->Bottle5ParaClass->setCurrentIndex(Bottle5_ClassNo);
           ui->Bottle1_ClassNo1->setCurrentIndex(Bottle1_ClassNo_1);
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle2_ClassNo_1);
           ui->Bottle3_ClassNo1->setCurrentIndex(Bottle3_ClassNo_1);
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle4_ClassNo_1);
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle5_ClassNo_1);
           break;
       case 1:
           ui->Bottle1ParaClass->setCurrentIndex(Bottle6_ClassNo);
           ui->Bottle2ParaClass->setCurrentIndex(Bottle7_ClassNo);
           ui->Bottle3ParaClass->setCurrentIndex(Bottle8_ClassNo);
           ui->Bottle4ParaClass->setCurrentIndex(Bottle9_ClassNo);
           ui->Bottle5ParaClass->setCurrentIndex(Bottle10_ClassNo);
           ui->Bottle1_ClassNo1->setCurrentIndex(Bottle6_ClassNo_1);
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle7_ClassNo_1);
           ui->Bottle3_ClassNo1->setCurrentIndex(Bottle8_ClassNo_1);
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle9_ClassNo_1);
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle10_ClassNo_1);
           break;
       case 2:
           ui->Bottle1ParaClass->setCurrentIndex(Bottle11_ClassNo);
           ui->Bottle2ParaClass->setCurrentIndex(Bottle12_ClassNo);
           ui->Bottle3ParaClass->setCurrentIndex(Bottle13_ClassNo);
           ui->Bottle4ParaClass->setCurrentIndex(Bottle14_ClassNo);
           ui->Bottle5ParaClass->setCurrentIndex(Bottle15_ClassNo);
           ui->Bottle1_ClassNo1->setCurrentIndex(Bottle11_ClassNo_1);
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle12_ClassNo_1);
           ui->Bottle3_ClassNo1->setCurrentIndex(Bottle13_ClassNo_1);
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle14_ClassNo_1);
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle15_ClassNo_1);
           break;
       case 3:
           ui->Bottle1ParaClass->setCurrentIndex(Bottle16_ClassNo);
           ui->Bottle2ParaClass->setCurrentIndex(Bottle17_ClassNo);
           ui->Bottle3ParaClass->setCurrentIndex(Bottle18_ClassNo);
           ui->Bottle4ParaClass->setCurrentIndex(Bottle19_ClassNo);
           ui->Bottle5ParaClass->setCurrentIndex(Bottle20_ClassNo);
           ui->Bottle1_ClassNo1->setCurrentIndex(Bottle16_ClassNo_1);
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle17_ClassNo_1);
           ui->Bottle3_ClassNo1->setCurrentIndex(Bottle18_ClassNo_1);
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle19_ClassNo_1);
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle20_ClassNo_1);
           break;
       case 4:
           ui->Bottle1ParaClass->setCurrentIndex(Bottle21_ClassNo);
           ui->Bottle2ParaClass->setCurrentIndex(Bottle22_ClassNo);
           ui->Bottle3ParaClass->setCurrentIndex(Bottle23_ClassNo);
           ui->Bottle4ParaClass->setCurrentIndex(Bottle24_ClassNo);
           ui->Bottle5ParaClass->setCurrentIndex(Bottle25_ClassNo);
           ui->Bottle1_ClassNo1->setCurrentIndex(Bottle21_ClassNo_1);
           ui->Bottle2_ClassNo1->setCurrentIndex(Bottle22_ClassNo_1);
           ui->Bottle3_ClassNo1->setCurrentIndex(Bottle23_ClassNo_1);
           ui->Bottle4_ClassNo1->setCurrentIndex(Bottle24_ClassNo_1);
           ui->Bottle5_ClassNo1->setCurrentIndex(Bottle25_ClassNo_1);
           break;
       default:
           break;
       }
}
