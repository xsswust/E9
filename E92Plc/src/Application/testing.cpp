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

	ui->leSampleNo1->setText("");
	ui->leSampleNo2->setText("");
	ui->leSampleNo3->setText("");
	ui->leSampleNo4->setText("");
	ui->leSampleNo5->setText("");

	qDebug()<<"g_Start_num === "<<g_Start_num;
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
		//qDebug()<<"start gTest_Mode = "<<gTest_Mode <<"num = "<< g_Start_num*MAX_ONE_GROUP_NUM;
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:

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

		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:


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

	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:


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

		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:


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

		qDebug()<<"start";
	}
	else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
	{
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:

				break;

			case QMessageBox::Cancel:
				break;

			default:
				break;
		}

		qDebug()<<"stop";
	}
}

