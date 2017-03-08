#include "debug_zuhe.h"
#include "ui_debug_zuhe.h"
#include "mainwindow.h"
#include "testing.h"
#include "testingdis.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QSettings>
#include "common.h"

debug_zuhe *pdebug_zuhe;

unsigned int gTest_Mode = NULL_ID;  // 记录 测试哪一种菌
// 记录 测试ID
unsigned int gTestId = TESTID_DEFAULT;
// 记录 测试瓶ID
unsigned int gBottomId = BOTTOMID_DEFAULT;
// 记录 执行时间
unsigned int gRunTime = 0;
unsigned int gRunTime_tmp = 0;

debug_zuhe::debug_zuhe(QWidget *parent) :
    QWidget(parent),
	ui(new Ui::debug_zuhe)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND_SINGLEZUHE_PNG);
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
	ui->pbExit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbStopAll->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbCy->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbCybjy->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbDlgdjcc->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbGlqx->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbJccdfy->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbJccdsy->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbJccqx->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbKqdxg->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbQbqx->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbSydjcc->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbWjsdjcc->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbWjsdzlh->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbXdydjcc->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbXdydzlh->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbXgddlg->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbZdcsw->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbZdpyw->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbZdxqw->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbZlhddlg->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbZlhdsy->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbZpdpw->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbZpdpyw->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));



	// 输入框值
	ui->le_TestID->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_BottomID->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_PlayTime->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Max_Time->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	exit_flag = false;

	initPainter();
}

debug_zuhe::~debug_zuhe()
{
    delete ui;
}

/*
	函数名称：
	函数功能：界面initPainter
	输入参数：
	输出参数：
	修改记录：
*/
void debug_zuhe::initPainter()
{
	// 新建键盘
	im = new Keyboard_dependence;
	// 设置键盘作用域
	ui->le_TestID->setFocusPolicy (Qt::ClickFocus);
	ui->le_TestID->installEventFilter (im);

	ui->le_BottomID->setFocusPolicy (Qt::ClickFocus);
	ui->le_BottomID->installEventFilter (im);

	ui->le_PlayTime->setFocusPolicy (Qt::ClickFocus);
	ui->le_PlayTime->installEventFilter (im);

	ui->le_Max_Time->setFocusPolicy (Qt::ClickFocus);
	ui->le_Max_Time->installEventFilter (im);

	ui->le_TestID->setText(QString::number(gTestId));
	ui->le_BottomID->setText(QString::number(gBottomId));
	ui->le_PlayTime->setText(QString::number(gRunTime));
	ui->le_Max_Time->setText(QString::number(g_TIMEOUT));

	//Signals and Slots
	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(InsertValue(QString)));
	connect(im->v_key,SIGNAL(del()),this,SLOT(NumberBack()));

	//定时器
	time_100ms = new QTimer();
	time_100ms->setInterval(TIMER_LEN_100MS);  // 500 ms


	connect(time_100ms,SIGNAL(timeout()),this,SLOT(timeOut()));
	time_100ms->stop();

}

/*
	函数名称：
	函数功能：界面退出
	输入参数：
	输出参数：
	修改记录：
*/
void debug_zuhe::on_pbExit_clicked()
{
//	getFullScree("testmod.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		this->close();
        //pmainwin->MainWindow_Init(); // 恢复状态
	}
}


// 显示输入
void debug_zuhe::InsertValue(QString nustr)
{
	QLineEdit *p_editer;
	qDebug()<<nustr;

	p_editer = qobject_cast<QLineEdit*>(focusWidget());
	p_editer->clear();
	p_editer->setText(nustr);
}

/*
	函数名称：
	函数功能：键盘返回
	输入参数：
	输出参数：
	修改记录：
*/
void debug_zuhe::NumberBack()
{
	QLineEdit *p_editer;
	p_editer = qobject_cast<QLineEdit*>(focusWidget());
	p_editer->backspace ();
}
// 定时器到
void debug_zuhe::timeOut(){

//	printf("\r\n[timeOut][1] gRunTime_tmp == %d gRunTime = %d \r\n", gRunTime_tmp, gRunTime);
	if(gRunTime_tmp >= (gRunTime*10)){
		StopAll();
		time_100ms->stop();
		gRunTime_tmp = 0;
		return;
	}
//	printf("\r\n[timeOut][2] gRunTime_tmp == %d gRunTime = %d \r\n", gRunTime_tmp, gRunTime);
	gRunTime_tmp ++;

}

//采样泵近样  目前没有接
void debug_zuhe::on_pbCybjy_released()
{

}

// 水样到检测池？
void debug_zuhe::on_pbSydjcc_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	qDebug()<<"on_pbSydjcc_released =======" << playTime;
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;
	int TestId = ui->le_BottomID->text().toInt();
	if(TestId < 1 || TestId > MAX_JINYE_NUM){
		return;
	}
	TestId = TestId -1;
	// 时间未设置  不执行
	gExt_out[FA_SHUIYANG] = GPIO_HIGH;
	gExt_out[gJinYeFa[TestId]] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 检测池到水样
void debug_zuhe::on_pbJccdsy_released()
{

	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	qDebug()<<"on_pbJccdsy_released   ====111==="<< playTime;
	gRunTime = playTime;
	int TestId = ui->le_BottomID->text().toInt();
	if(TestId < 1 || TestId > MAX_JINYE_NUM){
		return;
	}
	qDebug()<<"on_pbJccdsy_released   ====111===TestId = " << TestId;
	TestId = TestId -1;
	// 时间未设置  不执行
	gExt_out[FA_SHUIYANG] = GPIO_HIGH;
	gExt_out[gJinYeFa[TestId]] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	UpdateExtOutGpio();
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}

// 全部停止
void debug_zuhe::on_pbStopAll_released()
{
	StopAll();
}

// 定量管到检测池
void debug_zuhe::on_pbDlgdjcc_released()
{

	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =", playTime);
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;
	int TestId = ui->le_BottomID->text().toInt();
	if(TestId < 1 || TestId > MAX_JINYE_NUM){
		return;
	}
	//qDebug()<<"on_pbDlgdjcc_released   ====111===TestId = " + TestId;
	printf("\r\non_pbDlgdjcc_released === TestId == %d \r\n", TestId);
	TestId = TestId -1;
	// 时间未设置  不执行
	gExt_out[FA_SHIJI] = GPIO_HIGH;
	gExt_out[gJinYeFa[TestId]] = GPIO_HIGH;
	gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);

	//ui->pbDlgdjcc->setBackgroundRole(QPalette::Background);
	//ui->pbDlgdjcc->setBackgroundRole(QPalette::Dark);
	ui->pbDlgdjcc->setBackgroundRole(QPalette::Dark);
	ui->pbDlgdjcc->setAutoFillBackground(true);
	QPalette testpalette = ui->pbDlgdjcc->palette();
	//    testpalette.setBrush(ui->TH_2->backgroundRole(), QBrush(QColor(255, 0, 0)));
	testpalette.setColor(QPalette::Background, QColor(255,0,0));
	ui->pbDlgdjcc->setPalette(testpalette);
}
//载流环到水样
void debug_zuhe::on_pbZlhdsy_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =", playTime);
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;

	/*int TestId = ui->le_BottomID->text().toInt();
	if(TestId < 1 || TestId > MAX_JINYE_NUM){
		return;
	}
	//qDebug()<<"on_pbDlgdjcc_released   ====111===TestId = " + TestId;
	printf("\r\non_pbDlgdjcc_released === TestId == %d \r\n", TestId);
	TestId = TestId -1;
	*/
	// 时间未设置  不执行
	gExt_out[FA_SHUIYANG] = GPIO_HIGH;
	gExt_out[gJinYeFa[5]] = GPIO_HIGH;  // 注意此处 核定在确定
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	UpdateExtOutGpio();
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);

	//ui->pbDlgdjcc->setBackgroundRole(QPalette::Background);
	ui->pbZlhdsy->setBackgroundRole(QPalette::Dark);
	ui->pbZlhdsy->setAutoFillBackground(true);
	QPalette testpalette = ui->pbZlhdsy->palette();
	//    testpalette.setBrush(ui->TH_2->backgroundRole(), QBrush(QColor(255, 0, 0)));
	testpalette.setColor(QPalette::Background, QColor(0,255,0));
	ui->pbZlhdsy->setPalette(testpalette);
}
