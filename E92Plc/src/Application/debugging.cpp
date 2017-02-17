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
	QPixmap pixmap(PIC_BACKGROUND_SYSTEMTEST_PNG);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
//    QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);
    //Font Setting
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

	Init();
}
Debugging::~Debugging()
{
    delete ui;
}

void Debugging::Init()   //初始化
{
	exit_flag = false;
	//connect();
}

void Debugging::UnInit()	// 界面跳出处理
{

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
//	p_editer = qobject_cast<QLineEdit*>(focusWidget());
//	p_editer->backspace ();
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



// 进入 开关量界面
void Debugging::on_pb_single_clicked()
{
	debug_signal *mDebugsingle = new debug_signal();
	mDebugsingle->showFullScreen();
}
