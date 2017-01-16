#include "debugging.h"
#include "ui_debugging.h"
#include "mainwindow.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"
#include "debug_signal.h"

Debugging::Debugging(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Debugging)
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
    ui->lbTitle->setFont(QFont(FONT_USE_WQY,22,QFont::Bold));
    ui->lbPara->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbState->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbCancle->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbCloseLight->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbCurLight->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	//ui->pbFan->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	//ui->pbIniAD->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	//ui->pbPMT->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbSignal->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
   // ui->pbUVLight->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbSingleLight->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
//    ui->pbSysState->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lePara->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leState->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
	Init();
}
Debugging::~Debugging()
{
    delete ui;
}

void Debugging::Init()   //初始化
{
	exit_flag = false;
	// 键盘
	im = new Keyboard_dependence;
	ui->lePara->setFocusPolicy(Qt::ClickFocus);
	ui->lePara->installEventFilter(im);

	ui->le_OverTime->setFocusPolicy(Qt::ClickFocus);
	ui->le_OverTime->installEventFilter(im);

	ui->le_OverTime->setText(QString::number(g_Test_Over_Time));

	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(InsertValue(QString)));
}

void Debugging::UnInit()	// 界面跳出处理
{
	ui->lePara->removeEventFilter(im);
	disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(InsertValue(QString)));
	delete im;
}


// 显示输入
void Debugging::InsertValue(QString nustr)
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
void Debugging::NumberBack()
{
	QLineEdit *p_editer;
	p_editer = qobject_cast<QLineEdit*>(focusWidget());
	p_editer->backspace ();
}

void Debugging::on_pbExit_clicked()
{
//	getFullScree("debug.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		UnInit();
		 this->close();
		pmainwin->MainWindow_Init();
	}
}
// 点亮单个LED
void Debugging::on_pbSingleLight_clicked()
{
	QString temp;

	temp = ui->lePara->text();
	qDebug()<<"on_pbSingleLight_clicked == "<<temp;
	if(temp.toInt() >= 0 && temp.toInt() < MAX_TEST_NUM)
	{
		LED_Power(temp.toInt(), true);
		//Get_ad(temp.toInt());
	}
	for(int i = 0; i< 15; i++)
	{
		LED_Power(i, true);
		sleep(1);
	}
}
// 关闭所有光源
void Debugging::on_pbCloseLight_clicked()
{
	LED_Power(0, false);
}
// 信号 显示
void Debugging::on_pbSignal_clicked()
{
	debug_signal *hDataTempDis = new debug_signal();
	//hDataTempDis->HisDataTemp_init();
	hDataTempDis->Init();
	hDataTempDis->showFullScreen();

//	debug_signal temp;

//	temp.showFullScreen();
}
// 设置 超时时间
void Debugging::on_pbCurLight_clicked()
{
	QString temp;

	temp = ui->le_OverTime->text();
	qDebug()<<"on_pbSingleLight_clicked == "<<temp;
	if(temp.toInt() >= 0 && temp.toInt() <= 2880) // 最大48小时
	{
		g_Test_Over_Time = temp.toInt();
		//LED_Power(temp.toInt(), true);
		//Get_ad(temp.toInt());
	}

}
