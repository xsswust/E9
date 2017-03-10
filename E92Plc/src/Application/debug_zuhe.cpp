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

unsigned int gMode_Setup = 0;  // 记录 执行的步骤

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

	ui->le_onoff->setText(QString(STR_ZUHE_OFF));
	ui->le_setup->setText(QString::number(MODE_SETUP_1));

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
// 定量管 到 废液
void debug_zuhe::dlg2fy()
{
	gExt_out[FA_XIGUAN] = GPIO_HIGH;
	gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;

	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();
}

// 载流环 进消毒夜
void debug_zuhe::xdy2zlh()
{
	gExt_out[FA_XIAODUYE] = GPIO_HIGH;
	//gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();
}
#if 0
//  进消毒夜
void debug_zuhe::xdy2dlg()
{
	gExt_out[FA_XIAODUYE] = GPIO_HIGH;
	//gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();
}
#endif
// 载流环 进无菌水
void debug_zuhe::wjs2zlh()
{
	gExt_out[FA_WUJUNSHUI] = GPIO_HIGH;
	//gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();

}
// 载流环到定量管
void debug_zuhe::zlh2dlg()
{
	//gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;
	gExt_out[FA_SHIJI] = GPIO_HIGH;
	gExt_out[FA_JILIANKONGQI] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();

}
// 空气 到 载流环
void debug_zuhe::kq2zlh()
{
	//gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JILIANKONGQI] = GPIO_HIGH;
	gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();
}

// 管路清洗
void debug_zuhe::clean_guanlu()
{
	switch(gMode_Setup){
		case MODE_SETUP_1:  // 定量管 到 废液
			gMode_Setup = MODE_SETUP_2;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;

		case MODE_SETUP_2:   // 消毒液进载流环
			gMode_Setup = MODE_SETUP_3;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			xdy2zlh();
			break;

		case MODE_SETUP_3:  // 载流环 到 定量管
			gMode_Setup = MODE_SETUP_4;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			zlh2dlg();
			break;

		case MODE_SETUP_4:   // 消毒液进载流环
			gMode_Setup = MODE_SETUP_5;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			xdy2zlh();
			break;

		case MODE_SETUP_5:  // 等待10分钟
			gMode_Setup = MODE_SETUP_6;
			gRunTime = TIME_XIAODU;
			break;

		case MODE_SETUP_6:// 定量管到废液
			gMode_Setup = MODE_SETUP_7;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;

		case MODE_SETUP_7:// 空气到载流环
			gMode_Setup = MODE_SETUP_8;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			kq2zlh();
			break;

		case MODE_SETUP_8: // 无菌水 到 载流环
			gMode_Setup = MODE_SETUP_9;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			wjs2zlh();
			break;

		case MODE_SETUP_9:	// 载流环 到 定量管
			gMode_Setup = MODE_SETUP_10;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			zlh2dlg();
			break;

		case MODE_SETUP_10:	// 定量管 到 废液
			gMode_Setup = MODE_SETUP_11;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;

		case MODE_SETUP_11:	// 无菌水 到 载流环
			gMode_Setup = MODE_SETUP_12;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			wjs2zlh();
			break;

		case MODE_SETUP_12:	// 载流环 到 定量管
			gMode_Setup = MODE_SETUP_13;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			zlh2dlg();
			break;

		case MODE_SETUP_13:	//  定量管 到 废液
			gMode_Setup = MODE_SETUP_14;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;

		case MODE_SETUP_14:
			time_100ms->stop();
			gTest_Mode = NULL_ID;
			break;

		default:
			break;
	}
}
// 检测池 到 废液
void debug_zuhe::Jcc2Fy(int id){
	// 时间未设置  不执行
	//gExt_out[FA_WUJUNSHUI] = GPIO_HIGH;
	gExt_out[gPaiYeFa[id]] = GPIO_HIGH;
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();
}

// 无菌水 到 检测池
void debug_zuhe::Wjs2Jcc(int id){
	// 时间未设置  不执行
	if(id == 0){ // 注意载流环通往进液1是常闭的 所以要打开
		gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	}
	// 时间未设置  不执行
	gExt_out[FA_WUJUNSHUI] = GPIO_HIGH;
	gExt_out[gJinYeFa[id]] = GPIO_HIGH;
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();
}

void debug_zuhe::Xdy2Jcc(int id){
	// 时间未设置  不执行
	if(id == 0){ // 注意载流环通往进液1是常闭的 所以要打开
		gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	}
	// 时间未设置  不执行
	gExt_out[FA_XIAODUYE] = GPIO_HIGH;
	gExt_out[gJinYeFa[id]] = GPIO_HIGH;
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();
}

// 检测池清洗
void debug_zuhe::JianceshiClean()
{
	switch(gMode_Setup){
		case MODE_SETUP_1:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_2;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_2:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_3;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_3: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_4;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_4:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_5;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_5:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_6;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(5);
			break;

		case MODE_SETUP_6:// 消毒液 到 检测池1
			gMode_Setup = MODE_SETUP_7;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(0);
			break;

		case MODE_SETUP_7: // 消毒液 到 检测池2
			gMode_Setup = MODE_SETUP_8;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(1);
			break;

		case MODE_SETUP_8:	// 消毒液 到 检测池3
			gMode_Setup = MODE_SETUP_9;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(2);
			break;

		case MODE_SETUP_9:	// 消毒液 到 检测池4
			gMode_Setup = MODE_SETUP_10;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(3);
			break;

		case MODE_SETUP_10:	// 消毒液 到 检测池5
			gMode_Setup = MODE_SETUP_11;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(4);
			break;

		case MODE_SETUP_11:	// 消毒液 到 检测池6（混合池）
			gMode_Setup = MODE_SETUP_12;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(5);
			break;

		case MODE_SETUP_12:// 侵泡10分钟
			gMode_Setup = MODE_SETUP_13;
			gRunTime = TIME_XIAODU;
			break;

		case MODE_SETUP_13:	// 检测池1 到 废液
			gMode_Setup = MODE_SETUP_14;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(0);
			break;

		case MODE_SETUP_14:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_15;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_15:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_16;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_16: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_17;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_17:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_18;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_18:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_19;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(5);
			break;

		case MODE_SETUP_19:  // 无菌水 到 检测池1
			gMode_Setup = MODE_SETUP_20;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(0);
			break;

		case MODE_SETUP_20:   // 无菌水 到 检测池2
			gMode_Setup = MODE_SETUP_21;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(1);
			break;
		case MODE_SETUP_21:   // 无菌水 到 检测池3
			gMode_Setup = MODE_SETUP_22;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(2);
			break;
		case MODE_SETUP_22:   // 无菌水 到 检测池4
			gMode_Setup = MODE_SETUP_23;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(3);
			break;

		case MODE_SETUP_23:   // 无菌水 到 检测池5
			gMode_Setup = MODE_SETUP_24;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(4);
			break;

		case MODE_SETUP_25:   // 无菌水 到 检测池6 （混合池）
			gMode_Setup = MODE_SETUP_26;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(5);
			break;

		case MODE_SETUP_26:	// 检测池1 到 废液
			gMode_Setup = MODE_SETUP_27;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(0);
			break;

		case MODE_SETUP_27:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_28;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_28:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_29;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_29: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_30;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_30:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_31;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_31:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_32;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(5);
			break;

		case MODE_SETUP_32:  // 无菌水 到 检测池1
			gMode_Setup = MODE_SETUP_33;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(0);
			break;

		case MODE_SETUP_33:   // 无菌水 到 检测池2
			gMode_Setup = MODE_SETUP_34;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(1);
			break;

		case MODE_SETUP_34:   // 无菌水 到 检测池3
			gMode_Setup = MODE_SETUP_35;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(2);
			break;

		case MODE_SETUP_35:   // 无菌水 到 检测池4
			gMode_Setup = MODE_SETUP_36;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(3);
			break;

		case MODE_SETUP_36:   // 无菌水 到 检测池5
			gMode_Setup = MODE_SETUP_37;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(4);
			break;

		case MODE_SETUP_37:   // 无菌水 到 检测池6 （混合池）
			gMode_Setup = MODE_SETUP_38;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(5);
			break;

		case MODE_SETUP_38:	// 检测池1 到 废液
			gMode_Setup = MODE_SETUP_39;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(0);
			break;

		case MODE_SETUP_39:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_40;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_40:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_41;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_41: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_42;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_42:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_43;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_43:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_44;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(5);
			break;

		case MODE_SETUP_44:
			time_100ms->stop();
			gTest_Mode = NULL_ID;
			break;

		default:
			break;
	}
}
// 混合瓶到载流环
void debug_zuhe::Hhp2Zlh(){
	// 时间未设置  不执行
	gExt_out[FA_HUNHEYE] = GPIO_HIGH;
	gExt_out[FA_JINYE1] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;

	gExt_out[FA_PAIYEBENG] = GPIO_HIGH;
	gExt_out[FA_WAIPAIYEBENG] = GPIO_HIGH;
	UpdateExtOutGpio();
}

// 全部清洗
void debug_zuhe::AllClean()
{
	switch(gMode_Setup){
		// 0--7 为清空现有液体
		case MODE_SETUP_1:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_2;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_2:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_3;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_3: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_4;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_4:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_5;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_5:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_6;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(5);
			break;

		case MODE_SETUP_6:   // 清空载流环
			gMode_Setup = MODE_SETUP_7;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			kq2zlh();
			break;

		case MODE_SETUP_7:   // 清空 定量管
			gMode_Setup = MODE_SETUP_8;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;

// 所有管路 进消毒液

		case MODE_SETUP_8:// 消毒液 到 载流环
			gMode_Setup = MODE_SETUP_9;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			xdy2zlh();
			break;

		case MODE_SETUP_9: // 载流环 到 定量管
			gMode_Setup = MODE_SETUP_10;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			zlh2dlg();
			break;

		case MODE_SETUP_10: // 消毒液 到 载流环
			gMode_Setup = MODE_SETUP_11;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			xdy2zlh();
			break;

		case MODE_SETUP_11:// 消毒液 到 检测池1
			gMode_Setup = MODE_SETUP_12;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(0);
			break;

		case MODE_SETUP_12: // 消毒液 到 检测池2
			gMode_Setup = MODE_SETUP_13;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(1);
			break;

		case MODE_SETUP_13:	// 消毒液 到 检测池3
			gMode_Setup = MODE_SETUP_14;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(2);
			break;

		case MODE_SETUP_14:	// 消毒液 到 检测池4
			gMode_Setup = MODE_SETUP_15;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(3);
			break;

		case MODE_SETUP_15:	// 消毒液 到 检测池5
			gMode_Setup = MODE_SETUP_16;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(4);
			break;

		case MODE_SETUP_16:	// 消毒液 到 检测池6（混合池）
			gMode_Setup = MODE_SETUP_17;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Xdy2Jcc(5);
			break;

		case MODE_SETUP_17:	//  检测池6（混合池） 到 载流环
			gMode_Setup =MODE_SETUP_18;
			gRunTime = TIME_CLEAN_10;
			Hhp2Zlh();
			break;
// 侵泡 10分钟
		case MODE_SETUP_18:// 侵泡10分钟
			gMode_Setup = MODE_SETUP_19;
			gRunTime = TIME_XIAODU;
			break;
// 排管道中的废液
		case MODE_SETUP_19:	// 检测池1 到 废液
			gMode_Setup = MODE_SETUP_20;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(0);
			break;

		case MODE_SETUP_20:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_21;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_21:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_22;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_22: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_23;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_23:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_24;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_24:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_25;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(5);
			break;

		case MODE_SETUP_25:   // 清空载流环
			gMode_Setup = MODE_SETUP_26;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			kq2zlh();
			break;

		case MODE_SETUP_26:   // 清空 定量管
			gMode_Setup = MODE_SETUP_27;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;

		case MODE_SETUP_27:   // 清空 混合瓶到载流环管道液体
			gMode_Setup = MODE_SETUP_28;
			gRunTime = TIME_CLEAN_10*2;
			Hhp2Zlh();
			break;

//			第一遍   清洗
		case MODE_SETUP_28:  // 无菌水 到 检测池1
			gMode_Setup = MODE_SETUP_29;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(0);
			break;

		case MODE_SETUP_29:   // 无菌水 到 检测池2
			gMode_Setup = MODE_SETUP_30;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(1);
			break;
		case MODE_SETUP_30:   // 无菌水 到 检测池3
			gMode_Setup = MODE_SETUP_31;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(2);
			break;
		case MODE_SETUP_31:   // 无菌水 到 检测池4
			gMode_Setup = MODE_SETUP_32;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(3);
			break;

		case MODE_SETUP_32:   // 无菌水 到 检测池5
			gMode_Setup = MODE_SETUP_33;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(4);
			break;

		case MODE_SETUP_33:   // 无菌水 到 检测池6 （混合池）
			gMode_Setup = MODE_SETUP_34;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(5);
			break;

		case MODE_SETUP_34:   // 无菌水到载流环
			gMode_Setup = MODE_SETUP_35;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			wjs2zlh();
			break;

		case MODE_SETUP_35:   // 载流环 到 定量管
			gMode_Setup = MODE_SETUP_36;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			zlh2dlg();
			break;

		case MODE_SETUP_36:   // 无菌水到载流环
			gMode_Setup = MODE_SETUP_37;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			wjs2zlh();
			break;
// 排空所有 液体
		case MODE_SETUP_37:	// 检测池1 到 废液
			gMode_Setup = MODE_SETUP_38;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(0);
			break;

		case MODE_SETUP_38:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_39;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_39:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_40;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_40: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_41;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_41:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_42;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_42:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_43;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Hhp2Zlh();
			break;

		case MODE_SETUP_43:   // 清空载流环
			gMode_Setup = MODE_SETUP_44;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			kq2zlh();
			break;

		case MODE_SETUP_44:   // 清空 定量管
			gMode_Setup = MODE_SETUP_45;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;
//第二次清洗 再次清洗

		case MODE_SETUP_45:  // 无菌水 到 检测池1
			gMode_Setup = MODE_SETUP_46;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(0);
			break;

		case MODE_SETUP_46:   // 无菌水 到 检测池2
			gMode_Setup = MODE_SETUP_47;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(1);
			break;

		case MODE_SETUP_47:   // 无菌水 到 检测池3
			gMode_Setup = MODE_SETUP_48;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(2);
			break;

		case MODE_SETUP_48:   // 无菌水 到 检测池4
			gMode_Setup = MODE_SETUP_49;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(3);
			break;

		case MODE_SETUP_49:   // 无菌水 到 检测池5
			gMode_Setup = MODE_SETUP_50;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(4);
			break;

		case MODE_SETUP_50:   // 无菌水 到 检测池6 （混合池）
			gMode_Setup = MODE_SETUP_51;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Wjs2Jcc(5);
			break;

		case MODE_SETUP_51:   // 无菌水到载流环
			gMode_Setup = MODE_SETUP_52;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			wjs2zlh();
			break;

		case MODE_SETUP_52:   // 载流环 到 定量管
			gMode_Setup = MODE_SETUP_53;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			zlh2dlg();
			break;

		case MODE_SETUP_53:   // 无菌水到载流环
			gMode_Setup = MODE_SETUP_54;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			wjs2zlh();
			break;
// 第二次排出所有液体

		case MODE_SETUP_54:	// 检测池1 到 废液
			gMode_Setup = MODE_SETUP_55;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(0);
			break;

		case MODE_SETUP_55:  // 检测池2 到 废液
			gMode_Setup = MODE_SETUP_56;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(1);
			break;

		case MODE_SETUP_56:   // 检测池3 到 废液
			gMode_Setup = MODE_SETUP_57;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(2);
			break;

		case MODE_SETUP_57: // 检测池4 到 废液
			gMode_Setup = MODE_SETUP_58;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(3);
			break;

		case MODE_SETUP_58:   // 检测池5 到 废液
			gMode_Setup = MODE_SETUP_59;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Jcc2Fy(4);
			break;

		case MODE_SETUP_59:  // 检测池6（混合阀） 到 废液
			gMode_Setup = MODE_SETUP_60;
			gRunTime = TIME_CLEAN_JIANCECHI;
			Hhp2Zlh();
			break;

		case MODE_SETUP_60:   // 清空载流环
			gMode_Setup = MODE_SETUP_61;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			kq2zlh();
			break;

		case MODE_SETUP_61:   // 清空 定量管
			gMode_Setup = MODE_SETUP_62;
			gRunTime = TIME_CLEAN_ZAILIUHUAN;
			dlg2fy();
			break;


		case MODE_SETUP_62:
			time_100ms->stop();
			gTest_Mode = NULL_ID;
			ui->le_onoff->setText(QString(STR_ZUHE_OFF));
			ui->le_setup->setText(QString::number(MODE_SETUP_1));
			return ;
			break;

		default:
			break;
	}
	ui->le_onoff->setText(QString(STR_ZUHE_ON));
	ui->le_setup->setText(QString::number(gMode_Setup));
}

// 定时时间结束
void debug_zuhe::time_end(){
	gRunTime_tmp = 0;
	StopAll();
	if(gTest_Mode == NULL_ID){
		//StopAll();
		time_100ms->stop();
		gRunTime_tmp = 0;
		//return;
	}
	else if(gTest_Mode == MODE_GUANLUCLEAN){  // 管路清洗
		printf("\r\n [time_end]  gMode_Setup === %d \r\n", gMode_Setup);
		clean_guanlu();
	}
	else if(gTest_Mode == MODE_JIANCECHICLEAN){  // 检测池清洗
		printf("\r\n [time_end]  gMode_Setup === %d \r\n", gMode_Setup);
		JianceshiClean();
	}
	else if(gTest_Mode == MODE_ALLCLEAN){
		printf("\r\n [time_end]  gMode_Setup === %d \r\n", gMode_Setup);
		AllClean();
	}
}

// 定时器到
void debug_zuhe::timeOut(){

//	printf("\r\n[timeOut][1] gRunTime_tmp == %d gRunTime = %d \r\n", gRunTime_tmp, gRunTime);
	if(gRunTime_tmp >= (gRunTime*10)){
		time_end();
	}
	else{
//		printf("\r\n[timeOut][2] gRunTime_tmp == %d gRunTime = %d \r\n", gRunTime_tmp, gRunTime);
		gRunTime_tmp ++;
	}

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
	if(TestId == 0){ // 注意载流环通往进液1是常闭的 所以要打开
		gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	}
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
	if(TestId == 0){ // 注意载流环通往进液1是常闭的 所以要打开
		gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	}
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
	gMode_Setup = 0;
	gTest_Mode = NULL_ID;

	time_100ms->stop();
	ui->le_onoff->setText(QString(STR_ZUHE_OFF));
	ui->le_setup->setText(QString::number(MODE_SETUP_1));
}

// 定量管到检测池 ?
void debug_zuhe::on_pbDlgdjcc_released()
{

	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime = %d", playTime);
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
	if(TestId == 0){ // 注意载流环通往进液1是常闭的 所以要打开
		gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	}
	// 时间未设置  不执行
	gExt_out[FA_JILIANKONGQI] = GPIO_HIGH;
	gExt_out[FA_SHIJI] = GPIO_HIGH;
	gExt_out[gJinYeFa[TestId]] = GPIO_HIGH;

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
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
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
	gExt_out[FA_JINYE1] = GPIO_HIGH;  // 注意此处 核定在确定
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	//gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
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
// 空气到吸管
void debug_zuhe::on_pbKqdxg_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
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
	gExt_out[FA_JINYE1] = GPIO_HIGH;  // 注意此处 核定在确定
	gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[FA_XIGUAN] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	//gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	UpdateExtOutGpio();
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);

}
// 载流环到定量管
void debug_zuhe::on_pbZlhddlg_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
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
#if 0
	// 时间未设置  不执行
	gExt_out[FA_JINYE1] = GPIO_HIGH;  // 注意此处 核定在确定
	gExt_out[FA_SHIJI] = GPIO_HIGH;
	gExt_out[FA_JILIANKONGQI] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGZHENG] = GPIO_HIGH;
	//gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	UpdateExtOutGpio();
#else
	zlh2dlg();
#endif
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 消毒液到检测池 ？
void debug_zuhe::on_pbXdydjcc_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
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

	Xdy2Jcc(TestId);

	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 无菌水到检测池 ?
void debug_zuhe::on_pbWjsdjcc_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
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

	Wjs2Jcc(TestId);

	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 无菌水到载流环
void debug_zuhe::on_pbWjsdzlh_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;
#if 0
	// 时间未设置  不执行
	gExt_out[FA_WUJUNSHUI] = GPIO_HIGH;
	gExt_out[gJinYeFa[0]] = GPIO_HIGH;
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	//gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();
#else
	wjs2zlh();
#endif
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
//吸管到定量管
void debug_zuhe::on_pbXgddlg_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;

	// 时间未设置  不执行
	gExt_out[FA_XIGUAN] = GPIO_HIGH;
	gExt_out[FA_KONGQI] = GPIO_HIGH;
	gExt_out[gJinYeFa[0]] = GPIO_HIGH;
	//gExt_out[FA_JILIANGSHANG] = GPIO_HIGH;
	//gExt_out[FA_ZAILIUHUAN] = GPIO_HIGH;
	gExt_out[FA_RUDONGBENGFAN] = GPIO_HIGH;
	UpdateExtOutGpio();

	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 检测池到废液 ？
void debug_zuhe::on_pbJccdfy_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbJccdfy_released   ====111=== playTime =%d", playTime);
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;

	int TestId = ui->le_BottomID->text().toInt();
	if(TestId < 1 || TestId > MAX_JINYE_NUM){
		return;
	}
	//qDebug()<<"on_pbJccdfy_released   ====111===TestId = " + TestId;
	//printf("\r\on_pbJccdfy_released === TestId == %d \r\n", TestId);
	TestId = TestId -1;

	Jcc2Fy(TestId);

	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 消毒液到载流环
void debug_zuhe::on_pbXdydzlh_released()
{
	// 获取执行时间
	int playTime = ui->le_PlayTime->text().toInt();
	printf("\r\n  on_pbDlgdjcc_released   ====111=== playTime =%d", playTime);
	// 时间未设置  不执行
	if(playTime <= 0){
		return;
	}
	gRunTime = playTime;

	xdy2zlh();

	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 管路清洗
void debug_zuhe::on_pbGlqx_released()
{
	// 先排一下 载流环 液体
	gTest_Mode = MODE_GUANLUCLEAN;
	gMode_Setup = MODE_SETUP_1;
	kq2zlh();

	gRunTime = TIME_CLEAN_ZAILIUHUAN;  //
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 检测池清洗
void debug_zuhe::on_pbJccqx_released()
{
	gTest_Mode = MODE_JIANCECHICLEAN;
	gMode_Setup = MODE_SETUP_1;
	Jcc2Fy(0);
	gRunTime = TIME_CLEAN_JIANCECHI;  //
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 全部清洗
void debug_zuhe::on_pbQbqx_released()
{
	gTest_Mode = MODE_ALLCLEAN;
	gMode_Setup = MODE_SETUP_1;
	ui->le_onoff->setText(QString(STR_ZUHE_ON));
	ui->le_setup->setText(QString::number(gMode_Setup));

	Jcc2Fy(0);
	gRunTime = TIME_CLEAN_JIANCECHI;  //
	gRunTime_tmp = 0;
	time_100ms->start(TIMER_LEN_100MS);
}
// 界面 退出

void debug_zuhe::on_pbExit_released()
{
	if((gTest_Mode != NULL_ID) || (gMode_Setup != 0)){
		switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_ZUHE),
									 QMessageBox::Ok|QMessageBox::Cancel,
									 QMessageBox::Cancel))
		{
			case QMessageBox::Ok:
				exit_flag = true;
				this->close();
				time_100ms->stop();
				gMode_Setup = 0;
				gTest_Mode = NULL_ID;
				disconnect(time_100ms,SIGNAL(timeout()),this,SLOT(timeOut()));
				break;

			case QMessageBox::Cancel:
				//getFullScree("clean_his_data.jpg");
				break;

			default:
				//getFullScree("clean_his_data.jpg");
				break;
		}
	}
	else{
		if(false == exit_flag)
		{
			exit_flag = true;
			this->close();
			time_100ms->stop();
			gMode_Setup = 0;
			gTest_Mode = NULL_ID;
			disconnect(time_100ms,SIGNAL(timeout()),this,SLOT(timeOut()));
			//pmainwin->MainWindow_Init(); // 恢复状态
		}
	}
}
