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

	ui->le_1->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_2->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_3->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_4->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_5->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));

	ui->le_6->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_7->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_8->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_9->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_10->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));

	ui->le_11->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_12->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_13->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_14->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_15->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));

	ui->le_16->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_17->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_18->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_19->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_20->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));

	ui->le_21->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_22->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_23->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_24->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));
	ui->le_25->setFont(QFont(FONT_USE_WQY,FONT_10,QFont::Normal));


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
	QString temp;
	double vol = 0.0;
	int num_temp = 0;

#if 1
	if( Qt::Checked != ui->CB_Debug->checkState())  // debug 模式没有开启
    {
        if(Qt::Checked == ui->CB_TOL->checkState())  // 总大肠  405
        {  /* ui->CB_E->setCheckState(Qt::Unchecked);
            ui->CB_FEC->setCheckState(Qt::Unchecked);
            ui->CB_TPC->setCheckState(Qt::Unchecked); */
            ui->CB_E->setChecked(false);
            ui->CB_FEC->setChecked(false);
            ui->CB_TPC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
            num_temp = g_num + TOLCOLI_START_NUM;
        }
        else if(Qt::Checked == ui->CB_FEC->checkState())	 //  耐热 405
        {   /*ui->CB_E->setCheckState(Qt::Unchecked);
            ui->CB_TOL->setCheckState(Qt::Unchecked);
            ui->CB_TPC->setCheckState(Qt::Unchecked);*/
            ui->CB_E->setChecked(false);
            ui->CB_TOL->setChecked(false);
            ui->CB_TPC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
            num_temp = g_num + FECCOLI_START_NUM;
        }
        else if( Qt::Checked == ui->CB_TPC->checkState())  // 菌落总数 610
        { /*  ui->CB_E->setCheckState(Qt::Unchecked);
            ui->CB_TOL->setCheckState(Qt::Unchecked);
            ui->CB_FEC->setCheckState(Qt::Unchecked);*/
            ui->CB_E->setChecked(false);
            ui->CB_TOL->setChecked(false);
            ui->CB_FEC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
            num_temp = g_num + TPCCOLI_START_NUM;
        }
        else if( Qt::Checked == ui->CB_E->checkState())  // 埃希氏 365
        { /*  ui->CB_TPC->setCheckState(Qt::Unchecked);
            ui->CB_TOL->setCheckState(Qt::Unchecked);
            ui->CB_FEC->setCheckState(Qt::Unchecked);*/
            ui->CB_TPC->setChecked(false);
            ui->CB_TOL->setChecked(false);
            ui->CB_FEC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
            num_temp = g_num + ECOLI_START_NUM;
        }
        else
        {
            num_temp = g_num;
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
        }

        LED_Power( num_temp, GPIO_HIGH);
        //usleep(200*1000);

        vol = Get_val(num_temp);
	}
	else
	{
        if(Qt::Checked == ui->CB_TOL->checkState())  // 总大肠  405
        { /*  ui->CB_E->setCheckState(Qt::Unchecked);
            ui->CB_FEC->setCheckState(Qt::Unchecked);
            ui->CB_TPC->setCheckState(Qt::Unchecked);*/
            ui->CB_E->setChecked(false);
            ui->CB_FEC->setChecked(false);
            ui->CB_TPC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
			num_temp = g_num + TOLCOLI_START_NUM;
		}
        else if(Qt::Checked == ui->CB_FEC->checkState())	 //  耐热 405
         {/*   ui->CB_E->setCheckState(Qt::Unchecked);
            ui->CB_TOL->setCheckState(Qt::Unchecked);
            ui->CB_TPC->setCheckState(Qt::Unchecked);*/
            ui->CB_E->setChecked(false);
            ui->CB_TOL->setChecked(false);
            ui->CB_TPC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
			num_temp = g_num + FECCOLI_START_NUM;
		}
        else if( Qt::Checked == ui->CB_TPC->checkState())  // 菌落总数 610
        { /*  ui->CB_E->setCheckState(Qt::Unchecked);
            ui->CB_TOL->setCheckState(Qt::Unchecked);
            ui->CB_FEC->setCheckState(Qt::Unchecked);*/
            ui->CB_E->setChecked(false);
            ui->CB_TOL->setChecked(false);
            ui->CB_FEC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
			num_temp = g_num + TPCCOLI_START_NUM;
		}
        else if( Qt::Checked == ui->CB_E->checkState())  // 埃希氏 365
        { /*  ui->CB_TPC->setCheckState(Qt::Unchecked);
            ui->CB_TOL->setCheckState(Qt::Unchecked);
            ui->CB_FEC->setCheckState(Qt::Unchecked);*/
            ui->CB_TPC->setChecked(false);
            ui->CB_TOL->setChecked(false);
            ui->CB_FEC->setChecked(false);
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
			num_temp = g_num + ECOLI_START_NUM;
		}
		else
		{
			num_temp = g_num;
            qDebug()<<" ui->CB_TOL->checkState()="<<ui->CB_TOL->checkState();
            qDebug()<<" ui->CB_E->checkState()="<<ui->CB_E->checkState();
            qDebug()<<" ui->CB_FEC->checkState()="<<ui->CB_FEC->checkState();
            qDebug()<<" ui->CB_TPC->checkState()="<<ui->CB_TPC->checkState();
		}
        qDebug()<<"num_temp = "<<num_temp;

        vol = Get_val(num_temp);

	}
	//printf("\n &&&&&& g_num == %d \r\n", g_num);

	temp = QString::number((int)vol);

#if TEST
	//vol = Get_val(g_num);
//	vol = Get_val(g_num);
//	temp = QString::number(vol);
//	usleep(100*1000);
#else
	val = 0;
	val = Get_ad(num_temp);
	temp = QString::number(val);
#endif

	switch(g_num)
	{
		case 0:
			ui->le_1->setText(temp);
		break;

		case 1:
			ui->le_2->setText(temp);
		break;

		case 2:
			ui->le_3->setText(temp);
		break;

		case 3:
			ui->le_4->setText(temp);
		break;

		case 4:
			ui->le_5->setText(temp);
		break;

		case 5:
			ui->le_6->setText(temp);
		break;

		case 6:
			ui->le_7->setText(temp);
		break;

		case 7:
			ui->le_8->setText(temp);
		break;

		case 8:
			ui->le_9->setText(temp);
		break;

		case 9:
			ui->le_10->setText(temp);
		break;

		case 10:
			ui->le_11->setText(temp);
		break;

		case 11:
			ui->le_12->setText(temp);
		break;

		case 12:
			ui->le_13->setText(temp);
		break;

		case 13:
			ui->le_14->setText(temp);
		break;

		case 14:
			ui->le_15->setText(temp);
		break;

		case 15:
			ui->le_16->setText(temp);
		break;

		case 16:
			ui->le_17->setText(temp);
		break;

		case 17:
			ui->le_18->setText(temp);
		break;

		case 18:
			ui->le_19->setText(temp);
		break;

		case 19:
			ui->le_20->setText(temp);
		break;

		case 20:
			ui->le_21->setText(temp);
		break;

		case 21:
			ui->le_22->setText(temp);
		break;

		case 22:
			ui->le_23->setText(temp);
		break;

		case 23:
			ui->le_24->setText(temp);
		break;

		case 24:
			ui->le_25->setText(temp);
		break;


		default:
		break;
	}

//	LED_Power(g_num, GPIO_LOW);

	if( Qt::Checked != ui->CB_Debug->checkState())  // debug 模式没有开启
	{
        if(g_num >= 24)
        //if(g_num >= 24)
        //if(g_num >= 9)
		{
            g_num = 0;
		}
		else
		{
			g_num ++;
		}
	}
	else
	{
		if(g_num >= 24)
//        if(g_num >= 4)
		{
			g_num = 0;
		}
		else
		{
			g_num ++;
		}
	}

#else

    //usleep(100 *1000);
    g_num = 0;
    vol = Get_val(g_num);
//    Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
 //   Gpio_set(AD_A0, GPIO_HIGH);
 //   Gpio_set(AD_A1, GPIO_HIGH);
  //  Gpio_set(AD_A2, GPIO_HIGH);
  //  Gpio_set(AD_A3, GPIO_HIGH);
    temp = QString::number((int)vol);
    ui->le_1->setText(temp);
#endif

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

//365 灯1
void debug_signal::on_rb_led1_365_clicked()
{
	LED_Power(0 + ECOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_365->setChecked(true);
	ui->rb_led2_365->setChecked(false);
	ui->rb_led3_365->setChecked(false);
	ui->rb_led4_365->setChecked(false);
	ui->rb_led5_365->setChecked(false);
}
//365 灯2
void debug_signal::on_rb_led2_365_clicked()
{
	LED_Power(1+ + ECOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_365->setChecked(false);
	ui->rb_led2_365->setChecked(true);
	ui->rb_led3_365->setChecked(false);
	ui->rb_led4_365->setChecked(false);
	ui->rb_led5_365->setChecked(false);
}
//365 灯3
void debug_signal::on_rb_led3_365_clicked()
{
	LED_Power(2 + ECOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_365->setChecked(false);
	ui->rb_led2_365->setChecked(false);
	ui->rb_led3_365->setChecked(true);
	ui->rb_led4_365->setChecked(false);
	ui->rb_led5_365->setChecked(false);
}
//365 灯4
void debug_signal::on_rb_led4_365_clicked()
{
	LED_Power(3+ ECOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_365->setChecked(false);
	ui->rb_led2_365->setChecked(false);
	ui->rb_led3_365->setChecked(false);
	ui->rb_led4_365->setChecked(true);
	ui->rb_led5_365->setChecked(false);
}
//365 灯5
void debug_signal::on_rb_led5_365_clicked()
{
	LED_Power(4+ ECOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_365->setChecked(false);
	ui->rb_led2_365->setChecked(false);
	ui->rb_led3_365->setChecked(false);
	ui->rb_led4_365->setChecked(false);
	ui->rb_led5_365->setChecked(true);
}
// 405 灯1
void debug_signal::on_rb_led1_405_clicked()
{
	LED_Power(0+ TOLCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_405->setChecked(true);
	ui->rb_led2_405->setChecked(false);
	ui->rb_led3_405->setChecked(false);
	ui->rb_led4_405->setChecked(false);
	ui->rb_led5_405->setChecked(false);
}
// 405 灯2
void debug_signal::on_rb_led2_405_clicked()
{
	LED_Power(1+ TOLCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_405->setChecked(false);
	ui->rb_led2_405->setChecked(true);
	ui->rb_led3_405->setChecked(false);
	ui->rb_led4_405->setChecked(false);
	ui->rb_led5_405->setChecked(false);
}
// 405 灯3
void debug_signal::on_rb_led3_405_clicked()
{
	LED_Power(2+ TOLCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_405->setChecked(false);
	ui->rb_led2_405->setChecked(false);
	ui->rb_led3_405->setChecked(true);
	ui->rb_led4_405->setChecked(false);
	ui->rb_led5_405->setChecked(false);
}
// 405 灯4
void debug_signal::on_rb_led4_405_clicked()
{
	LED_Power(3+ TOLCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_405->setChecked(false);
	ui->rb_led2_405->setChecked(false);
	ui->rb_led3_405->setChecked(false);
	ui->rb_led4_405->setChecked(true);
	ui->rb_led5_405->setChecked(false);
}
// 405 灯5
void debug_signal::on_rb_led5_405_clicked()
{
	LED_Power(4+ TOLCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_405->setChecked(false);
	ui->rb_led2_405->setChecked(false);
	ui->rb_led3_405->setChecked(false);
	ui->rb_led4_405->setChecked(false);
	ui->rb_led5_405->setChecked(true);
}
// 610 灯1
void debug_signal::on_rb_led1_610_clicked()
{
	LED_Power(0+ TPCCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_610->setChecked(true);
	ui->rb_led2_610->setChecked(false);
	ui->rb_led3_610->setChecked(false);
	ui->rb_led4_610->setChecked(false);
	ui->rb_led5_610->setChecked(false);
}
// 610 灯2
void debug_signal::on_rb_led2_610_clicked()
{
	LED_Power(1+ TPCCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_610->setChecked(false);
	ui->rb_led2_610->setChecked(true);
	ui->rb_led3_610->setChecked(false);
	ui->rb_led4_610->setChecked(false);
	ui->rb_led5_610->setChecked(false);
}
// 610 灯3
void debug_signal::on_rb_led3_610_clicked()
{
	LED_Power(2+ TPCCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_610->setChecked(false);
	ui->rb_led2_610->setChecked(false);
	ui->rb_led3_610->setChecked(true);
	ui->rb_led4_610->setChecked(false);
	ui->rb_led5_610->setChecked(false);
}
// 610 灯4
void debug_signal::on_rb_led4_610_clicked()
{
	LED_Power(3 + TPCCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_610->setChecked(false);
	ui->rb_led2_610->setChecked(false);
	ui->rb_led3_610->setChecked(false);
	ui->rb_led4_610->setChecked(true);
	ui->rb_led5_610->setChecked(false);
}
// 610 灯5
void debug_signal::on_rb_led5_610_clicked()
{
	LED_Power(4+ TPCCOLI_START_NUM, GPIO_HIGH);
	ui->rb_led1_610->setChecked(false);
	ui->rb_led2_610->setChecked(false);
	ui->rb_led3_610->setChecked(false);
	ui->rb_led4_610->setChecked(false);
	ui->rb_led5_610->setChecked(true);
}
