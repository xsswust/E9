#include "timecalibrate.h"
#include "ui_timecalibrate.h"
#include "src/HardWare/app.h"
#include "src/HardWare/keyboard_dependence.h"
#include "src/HardWare/com_trandata.h"
#include "NumberKeyBoard.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QDebug>
#include <QString>
#include <QTime>
#include "common.h"

TimeCalibrate::TimeCalibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeCalibrate)
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
    ui->lbTitle->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbCalibrate->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

	ui->lbDateTime->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));
	ui->lbSysDateTime->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));

	ui->lbYear->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));
	ui->lbMonth->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));
	ui->lbDay->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));
	ui->lbHour->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));
	ui->lbMinute->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));
	ui->lbSecond->setFont(QFont(FONT_USE_WQY,16,QFont::Normal));

    ui->leYear->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leMonth->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leDay->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leHour->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leMinute->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leSecond->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));



//   connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
//   connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
//   connect(ptimerTime,SIGNAL(timeout()),this,SLOT(timeUpData()));
}

TimeCalibrate::~TimeCalibrate()
{
	//disconnect(im->v_key, SIGNAL(emitNustr(QString)), this, SLOT(insertValue(QString)));
	disconnect(im->v_key, SIGNAL(emitNustr(QString)), this, SLOT(insertValue(QString)));
	disconnect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	disconnect(ptimerTime,SIGNAL(timeout()),this,SLOT(timeUpData()));

	ptimerTime->stop();  // timer 停止
	//ptimerTime->del;  // 销毁
    delete ui;
}
// 界面 初始化  显示
void TimeCalibrate::init()
{
	exit_flag = false;
	im = new Keyboard_dependence;
	ui->leYear->setFocusPolicy (Qt::ClickFocus);
	ui->leYear->installEventFilter (im);
	ui->leMonth->setFocusPolicy (Qt::ClickFocus);
	ui->leMonth->installEventFilter (im);
	ui->leDay->setFocusPolicy (Qt::ClickFocus);
	ui->leDay->installEventFilter (im);
	ui->leHour->setFocusPolicy (Qt::ClickFocus);
	ui->leHour->installEventFilter (im);
	ui->leMinute->setFocusPolicy (Qt::ClickFocus);
	ui->leMinute->installEventFilter (im);
	ui->leSecond->setFocusPolicy (Qt::ClickFocus);
	ui->leSecond->installEventFilter (im);

//   QDateTime time =   QDateTime::currentDateTime ();

//   ui->leYear->setText (time.toString ("yyyy"));
//   ui->leMonth->setText (time.toString ("MM"));
//   ui->leDay->setText (time.toString ("dd"));
//   ui->leHour->setText (time.toString ("hh"));
//   ui->leMinute->setText (time.toString ("mm"));
//   ui->leSecond->setText (time.toString ("ss"));

   ptimerTime = new QTimer();
//   ptimerTime->start(1000);  //1s 定时器


	ptimerTime->start(TIMER_LEN_500MS);  //1s 定时器 开始定时
// 建立 信号链接
	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	connect(ptimerTime,SIGNAL(timeout()),this,SLOT(timeUpData()));
//
	QDateTime time= QDateTime::currentDateTime();  // 获取目前时间
	QString year= time.toString("yyyy");  // 获取年
	QString month= time.toString("MM");	// 获取月份
	QString day= time.toString("dd");		// 获取日
	QString hour= time.toString("hh");		// 获取小时
	QString min= time.toString("mm");		// 获取分钟
	QString ss= time.toString("ss");			// 获取秒

	ui->leYear->setText(year);
	ui->leMonth->setText(month);
	ui->leDay->setText(day);
	ui->leHour->setText(hour);
	ui->leMinute->setText(min);
	ui->leSecond->setText(ss);

	timeUpData();  // 时间显示
}

void TimeCalibrate::Un_Init()
{

	ui->leYear->removeEventFilter(im);
	ui->leMonth->removeEventFilter(im);
	ui->leDay->removeEventFilter(im);
	ui->leHour->removeEventFilter(im);
	ui->leMinute->removeEventFilter(im);
	ui->leSecond->removeEventFilter(im);

	ptimerTime->stop();  // timer 停止
	disconnect(im->v_key,SIGNAL(emitNustr(QString)), this, SLOT(insertValue(QString)));
	disconnect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	disconnect(ptimerTime,SIGNAL(timeout()),this,SLOT(timeUpData()));

	delete ptimerTime;

	delete im;
}

void TimeCalibrate::on_pbExit_clicked()
{
//	getFullScree("timecalibrate.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		Un_Init();
		this->close();
	}
}

/*
    函数名称：
    函数功能：时间校准
    输入参数：
    输出参数：
    修改记录：
*/
void TimeCalibrate::on_pbCalibrate_clicked()
{
#if 0
    QString myString = ui->leYear->text ()+"-"+ui->leMonth->text ()+"-"+ui->leDay->text ()+" "+ui->leHour->text ()+":"+ui->leMinute->text ()+":"+ui->leSecond->text ();
     qDebug ()<<myString;
     char* timeCalibrate = QstringTostring(myString);
     if(!setSysTime(timeCalibrate))
     {
         qDebug ()<<" Been  Calibrated Successfully !";
     }
     else
     {
         qDebug ()<<" Been  Calibrated Failed !";
     }
#else //时间设置测试 ok --9-7


	QString time_temp;
	int sec= 0,min = 0, hour = 0, day = 0, mon = 0, year = 0;

	// 秒
	time_temp = ui->leSecond->text();
	sec = time_temp.toInt();
	if(sec > 59){
		sec = 59;
	}
	else if(sec < 0){
		sec = 0;
	}
	// 分
	time_temp = ui->leMinute->text();
	min = time_temp.toInt();
	if(min > 59){
		min = 59;
	}
	else if(min < 0){
		min = 0;
	}
	// 时
	time_temp = ui->leHour->text();
	hour = time_temp.toInt();
	if(hour > 23){
		hour = 23;
	}
	else if(hour < 0){
		hour = 0;
	}
	// 日
	time_temp = ui->leDay->text();
	day = time_temp.toInt();
	if(day > 31){
		day = 31;
	}
	else if(day < 1){
		day = 1;
	}
	// 月
	time_temp = ui->leMonth->text();
	mon = time_temp.toInt();
	if(mon > 12){
		mon = 12;
	}
	else if(mon < 1){
		mon = 1;
	}

	// 年
	time_temp = ui->leYear->text();
	year = time_temp.toInt();
	if(year > 2055){
		year = 2055;
	}
	else if(year < 1900){
		year = 1900;
	}


	set_system_time(year, mon, day, hour, min, sec);
#endif

}

/*
    函数名称：
    函数功能：系统时间更新
    输入参数：
    输出参数：
    修改记录：
*/
void TimeCalibrate::timeUpData()
{
	QDateTime time= QDateTime::currentDateTime();
	QString strDate= time.toString("yyyy-MM-dd hh:mm:ss ddd");
	ui->lbSysDateTime->setText(strDate);
}

void TimeCalibrate::insertValue(QString nustr)
{
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->clear();
    p_editer->setText(nustr);

}
void TimeCalibrate::keyboardDelete()
{
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->backspace();
}
