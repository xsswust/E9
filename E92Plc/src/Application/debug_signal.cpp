#include "debug_signal.h"
#include "ui_debug_signal.h"
#include	"common.h"

#define TEST 1

debug_signal::debug_signal(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::debug_signal)
{
	ui->setupUi(this);

	QPixmap pixmap(PIC_BACKGROUND2_PNG);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);
	g_num = 0;

//	Init();
}

debug_signal::~debug_signal()
{
	delete ui;
}
// 初始化
void debug_signal::Init()
{
	exit_flag = false;
	timeForGetTEMP_1Minute = new QTimer();
	timeForGetTEMP_1Minute->stop();  // ms

	timeForGetTEMP_100ms = new QTimer();
	timeForGetTEMP_100ms->stop();  // ms


	// 建立连接
	connect(timeForGetTEMP_1Minute,SIGNAL(timeout()),this,SLOT(timeUp_1Minute()));  //
	connect(timeForGetTEMP_100ms,SIGNAL(timeout()),this,SLOT(timeUp_100Ms()));

	connect(pb,SIGNAL(timeout()),this,SLOT(timeUp_100Ms()));

	//timeForGetTEMP_1Minute->start(TIMER_LEN_1MINUTE);

#if HARDWARE_V101
     timeForGetTEMP_1Minute->start(250);
#endif
#if HARDWARE_V100
    timeForGetTEMP_1Minute->start(500);
#endif
    //timeUp_1Minute();
}

// 初始化
void debug_signal::Uninit()
{
	exit_flag = true;
	// 建立连接
	disconnect(timeForGetTEMP_1Minute,SIGNAL(timeout()),this,SLOT(timeUp_1Minute()));  //
	disconnect(timeForGetTEMP_100ms,SIGNAL(timeout()),this,SLOT(timeUp_100Ms()));

	timeForGetTEMP_1Minute->stop();
	timeForGetTEMP_100ms->stop();
	//timeUp_1Minute();

	delete timeForGetTEMP_1Minute;
	delete timeForGetTEMP_100ms;
}


// 定时获取数据
void debug_signal::timeUp_1Minute()
{

}

void debug_signal::timeUp_100Ms()
{

}

void debug_signal::on_pB_Exit_clicked()
{
//	getFullScree("signal.jpg");
	if(false == exit_flag)
	{
		Uninit();
		this->close();
	}
}

