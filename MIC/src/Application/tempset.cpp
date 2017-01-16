#include "tempset.h"
#include "ui_tempset.h"
#include "common.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"



TempSet::TempSet(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TempSet)
{
	ui->setupUi(this);
	QPixmap pixmap(PIC_BACKGROUND2_PNG);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);

	// 字体设置
	ui->lb_Ch1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_Ch2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_Ch3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_Ch4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_Ch5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_Nor->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	// 温度显示
	ui->lb_temp_1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_temp_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_temp_3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_temp_4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_temp_5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //
	ui->lb_temp_nor->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));  //

	ui->le_Ch1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Ch2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Ch3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Ch4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Ch5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->le_Ch6->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pb_Ch1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Ch2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Ch3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Ch4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Ch5->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pb_Ch6->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->lb_Title->setFont(QFont(FONT_USE_WQY,FONT_24,QFont::Normal));
	ui->pb_exit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	// 居中显示
	ui->lb_Ch1->setAlignment(Qt::AlignCenter);
	ui->lb_Ch2->setAlignment(Qt::AlignCenter);
	ui->lb_Ch3->setAlignment(Qt::AlignCenter);
	ui->lb_Ch4->setAlignment(Qt::AlignCenter);
	ui->lb_Ch5->setAlignment(Qt::AlignCenter);
	ui->lb_Nor->setAlignment(Qt::AlignCenter);

	ui->lb_temp_1->setAlignment(Qt::AlignCenter);
	ui->lb_temp_2->setAlignment(Qt::AlignCenter);
	ui->lb_temp_3->setAlignment(Qt::AlignCenter);
	ui->lb_temp_4->setAlignment(Qt::AlignCenter);
	ui->lb_temp_5->setAlignment(Qt::AlignCenter);
	ui->lb_temp_nor->setAlignment(Qt::AlignCenter);

	ui->le_Ch1->setAlignment(Qt::AlignCenter);
	ui->le_Ch2->setAlignment(Qt::AlignCenter);
	ui->le_Ch3->setAlignment(Qt::AlignCenter);
	ui->le_Ch4->setAlignment(Qt::AlignCenter);
	ui->le_Ch5->setAlignment(Qt::AlignCenter);
    ui->le_Ch6->setAlignment(Qt::AlignCenter);

	ui->lb_Title->setAlignment(Qt::AlignCenter);

	Init();
}

TempSet::~TempSet()
{
	delete ui;
}

void TempSet::Init()  // 初始化
{

	exit_flag = false;
	//Keyboard Setting
	im = new Keyboard_dependence;

	ui->le_Ch1->setFocusPolicy(Qt::ClickFocus);
	ui->le_Ch1->installEventFilter(im);

	ui->le_Ch2->setFocusPolicy(Qt::ClickFocus);
	ui->le_Ch2->installEventFilter(im);

	ui->le_Ch3->setFocusPolicy(Qt::ClickFocus);
	ui->le_Ch3->installEventFilter(im);

	ui->le_Ch4->setFocusPolicy(Qt::ClickFocus);
	ui->le_Ch4->installEventFilter(im);

	ui->le_Ch5->setFocusPolicy(Qt::ClickFocus);
	ui->le_Ch5->installEventFilter(im);

	ui->le_Ch6->setFocusPolicy(Qt::ClickFocus);
	ui->le_Ch6->installEventFilter(im);

	ui->le_Ch1->setText(QString::number(g_Sp1));
	ui->le_Ch2->setText(QString::number(g_Sp2));
	ui->le_Ch3->setText(QString::number(g_Sp3));
	ui->le_Ch4->setText(QString::number(g_Sp4));
	ui->le_Ch5->setText(QString::number(g_Sp5));
	ui->le_Ch6->setText(QString::number(g_Sp6));
	//ui->le_Ch1->removeEventFilter(im);

	ui->lb_temp_1->setText(QString::number(g_temp1));
	ui->lb_temp_2->setText(QString::number(g_temp2));
	ui->lb_temp_3->setText(QString::number(g_temp3));
	ui->lb_temp_4->setText(QString::number(g_temp4));
	ui->lb_temp_5->setText(QString::number(g_temp5));
	ui->lb_temp_nor->setText(QString::number(g_temp_nor));

	// 1s 定时
	pUpdateTime = new QTimer();
	pUpdateTime->start(TIMER_LEN_1S);

	//Signals and Slots
	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	connect(ui->pb_exit, SIGNAL(released()), this, SLOT(ui_exit()));
	connect(ui->pb_Ch1, SIGNAL(released()), this, SLOT(pb_Ch1_Checkup())); // 抬起操作
	connect(ui->pb_Ch2, SIGNAL(released()), this, SLOT(pb_Ch2_Checkup())); // 抬起操作
	connect(ui->pb_Ch3, SIGNAL(released()), this, SLOT(pb_Ch3_Checkup())); // 抬起操作
	connect(ui->pb_Ch4, SIGNAL(released()), this, SLOT(pb_Ch4_Checkup())); // 抬起操作
	connect(ui->pb_Ch5, SIGNAL(released()), this, SLOT(pb_Ch5_Checkup())); // 抬起操作
	connect(ui->pb_Ch6, SIGNAL(released()), this, SLOT(pb_Ch6_Checkup())); // 抬起操作


	connect(pUpdateTime,SIGNAL(timeout()),this,SLOT(UpdateShow()));
}


void TempSet::UnInit()  // 界面跳转
{
	exit_flag = true;
	pUpdateTime->stop();  // 停止计时
	ui->le_Ch1->removeEventFilter(im);
	ui->le_Ch2->removeEventFilter(im);
	ui->le_Ch3->removeEventFilter(im);
	ui->le_Ch4->removeEventFilter(im);
	ui->le_Ch5->removeEventFilter(im);

	disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	disconnect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
	disconnect(ui->pb_exit, SIGNAL(clicked(bool)), this, SLOT(ui_exit()));
	disconnect(pUpdateTime,SIGNAL(timeout()),this,SLOT(UpdateShow()));
	disconnect(ui->pb_Ch1, SIGNAL(released()), this, SLOT(pb_Ch1_Checkup())); // 抬起操作
	disconnect(ui->pb_Ch2, SIGNAL(released()), this, SLOT(pb_Ch2_Checkup())); // 抬起操作
	disconnect(ui->pb_Ch3, SIGNAL(released()), this, SLOT(pb_Ch3_Checkup())); // 抬起操作
	disconnect(ui->pb_Ch4, SIGNAL(released()), this, SLOT(pb_Ch4_Checkup())); // 抬起操作
	disconnect(ui->pb_Ch5, SIGNAL(released()), this, SLOT(pb_Ch5_Checkup())); // 抬起操作
	disconnect(ui->pb_Ch6, SIGNAL(released()), this, SLOT(pb_Ch6_Checkup())); // 抬起操作


	delete im;
	delete pUpdateTime;
}
//更新显示
void TempSet::UpdateShow()
{
	ui->lb_temp_1->setText(QString::number(g_temp1));
	ui->lb_temp_2->setText(QString::number(g_temp2));
	ui->lb_temp_3->setText(QString::number(g_temp3));
	ui->lb_temp_4->setText(QString::number(g_temp4));
	ui->lb_temp_5->setText(QString::number(g_temp5));
	ui->lb_temp_nor->setText(QString::number(g_temp_nor));

	double t;

#if 1
	t = (ui->le_Ch1->text()).toDouble();
	if(t == 0.0)
	{
		ui->le_Ch1->setText(QString::number(g_Sp1));
	}

	t = (ui->le_Ch2->text()).toDouble();
	if(t == 0.0)
	{
		ui->le_Ch2->setText(QString::number(g_Sp2));
	}

	t = (ui->le_Ch3->text()).toDouble();
	if(t == 0.0)
	{
		ui->le_Ch3->setText(QString::number(g_Sp3));
	}

	t = (ui->le_Ch4->text()).toDouble();
	if(t == 0.0)
	{
		ui->le_Ch4->setText(QString::number(g_Sp4));
	}

	t = (ui->le_Ch5->text()).toDouble();
	if(t == 0.0)
	{
		ui->le_Ch5->setText(QString::number(g_Sp5));
	}

	t = (ui->le_Ch6->text()).toDouble();
	if(t == 0.0)
	{
		ui->le_Ch6->setText(QString::number(g_Sp6));
	}
#else
	ui->le_Ch1->setText(QString::number(g_Sp1));
	ui->le_Ch2->setText(QString::number(g_Sp2));
	ui->le_Ch3->setText(QString::number(g_Sp3));
	ui->le_Ch4->setText(QString::number(g_Sp4));
	ui->le_Ch5->setText(QString::number(g_Sp5));
	ui->le_Ch6->setText(QString::number(g_Sp6));

#endif
}

// 退出
void TempSet::ui_exit()
{
	getFullScree("tempset.jpg");
	//qDebug()<<"ui_exit ===============111==========";
	if(false ==  exit_flag)
	{
		UnInit();
//		qDebug()<<"ui_exit ===============222==========";
		usleep(1000);
//		qDebug()<<"ui_exit ===============333==========";
		this->close();
//		qDebug()<<"ui_exit ===============444==========";
	}
}


// 显示
void TempSet::insertValue(QString nustr)
{
	QLineEdit *p_editer;

	p_editer = qobject_cast<QLineEdit*>(focusWidget());
	p_editer->clear();
	p_editer->setText(nustr);
}
// 删除
void TempSet::keyboardDelete()
{
	QLineEdit *p_editer;

	p_editer = qobject_cast<QLineEdit*>(focusWidget());
	p_editer->backspace();
}


// 发送 数据 获取通道温度值  chanel 通道值
void TempSet::Set_Chanel_Param(UINT8 chanel, UINT8 param, UINT16 data)
{
	SEND_MESSAGE_TYPE tx_data;
	qint32 checksum = 0;
	int len = 0;
	//char tx_data[CMD_GET_TEMP_SEND_DATA_LEN];

	memset(&tx_data, 0, sizeof(SEND_MESSAGE_TYPE));

	tx_data.u8DevAddr1 = CMD_PID_ADDR + chanel;
	tx_data.u8DevAddr2 = CMD_PID_ADDR + chanel;
	tx_data.u8ReqType = CMD_PID_WRITE;
	tx_data.u8DevPara = param;
	tx_data.u8LoParam = data&0xff;
	tx_data.u8HiParam = (data>>8)&0xff;
	checksum = chanel + (tx_data.u8DevPara << 8) + tx_data.u8ReqType + ((tx_data.u8HiParam << 8) + tx_data.u8LoParam);
	tx_data.u8LoSumCheck = checksum & 0xff;
	tx_data.u8HiSumCheck = (checksum >> 8) & 0xff;;
#if 0
	for(int i; i < sizeof(tx_data); i++)
	{
		printf("tx_data[%d] = 0x%x \r\n", i, tx_data[i]);
	}
#endif
	len = pTranData->com6_send((const char *) (&tx_data),CMD_GET_TEMP_SEND_DATA_LEN);
	if(len != CMD_GET_TEMP_SEND_DATA_LEN)
	{
		qDebug()<<"send data error   +++++";
	}
	return ;
}

bool TempSet::Get_Check_Sum(RECEIVE_MESSAGE_TYPE *receive,quint8 addr)
{
	quint16  u16CrcVal = 0;

	quint16 u16TempCheck;
	bool bResult = false;

	u16TempCheck = (receive->u8HiSumCheck << 8) + receive->u8LoSumCheck;
//    u8CrcVal = ((stAnsFrm.u8HiPV + stAnsFrm.u8LoPV + stAnsFrm.u8HiSV + stAnsFrm.u8LoSV +
//                 stAnsFrm.u8LoMV + stAnsFrm.u8HiMV + stAnsFrm.u8HiParam + stAnsFrm.u8LoParam +
//                 stAnsFrm.u8ALm + u8TempCheck) % 256) / 16;

//    u8CalCrcVal = stAnsFrm.u8SumCheck / 16;
	u16CrcVal = ((receive->u8HiPV << 8) + receive->u8LoPV)
				+((receive->u8HiSV << 8) + receive->u8LoSV)
				+((receive->u8ALm << 8) + receive->u8MV)
				+((receive->u8HiParam << 8) + receive->u8LoParam) + addr;

	//qDebug()<<QString("u16TempCheck = 0x%x")
	printf("u16TempCheck = 0x%x   u16CrcVal = 0x%x addr = %d\r\n", u16TempCheck, u16CrcVal, addr);
	if(u16TempCheck == u16CrcVal)
	{
		bResult = true;
	}
	bResult = true;

	return bResult;
}

// 设置数据
void TempSet::Set_Chanel_Temp(UINT8 chanel, UINT16 val)
{
	quint8 data[LEN_TEMP_DATA+1];
	int len, flag = 0;

	//static QMutex mutex;
	//mutex.lock();  // 加锁保护

	pcomtran_thread->flag_Tran = false;

	Set_Chanel_Param(chanel, CMD_PID_SET_SP, val);
	usleep(TEMP_RS485_DELEY);
	memset(data, 0, sizeof(data));
	len = pTranData->com6_receive(data, LEN_TEMP_DATA);
	printf("\n len  = %d \r\n ", len);
#if 0
	while(len <= 0)
	{
		flag ++;
		printf("\n 重发  =  \r\n ");
		usleep(1000*1000);  // 1S
		Set_Chanel_Param(chanel, CMD_PID_SET_SP, val);
		usleep(TEMP_RS485_DELEY);
		memset(data, 0, sizeof(data));
		len = pTranData->com6_receive(data, LEN_TEMP_DATA);
		printf("\n len  = %d \r\n ", len);

		flag ++;
		if(flag > 3){  // 重发3次
			break;
		}
	}
#endif
	for(int i = 0; i < len; i++)
	{
		printf("\ndata[%d] = 0x%x \r\n", i, data[i]);
	}

	if(len > 0)
	{
		if(val == (data[3] << 8 | data[2]))
//		if( true == Get_Check_Sum((RECEIVE_MESSAGE_TYPE *)data, chanel))
		{
			QMessageBox::information(this,STR_PROMPT,tr(STR_SET_OK));
		}
		else{
			QMessageBox::information(this,STR_PROMPT,tr(STR_SET_ERROR));

		}
	}
	pcomtran_thread->flag_Tran = true;
	//mutex.unlock();  // 加锁保护
}

#if 1
// 通道1 设置  抬起操作
void TempSet::pb_Ch1_Checkup()
{
	double t;
	int temp = 0;

	t = (ui->le_Ch1->text()).toDouble();
	temp = int(t*10);
	if(0 != temp)
	{
		Set_Chanel_Temp(CMD_PID_CHANEL1, temp);
	}

}
#endif
// 通道2 设置
void TempSet::pb_Ch2_Checkup()
{
	double t;

	t = (ui->le_Ch2->text()).toDouble();
	Set_Chanel_Temp(CMD_PID_CHANEL2, int(t*10));
}
// 通道3 设置
void TempSet::pb_Ch3_Checkup()
{
	double t;

	t = (ui->le_Ch3->text()).toDouble();
	Set_Chanel_Temp(CMD_PID_CHANEL3, int(t*10));
}
// 通道4 设置
void TempSet::pb_Ch4_Checkup()
{
	double t;

	t = (ui->le_Ch4->text()).toDouble();
	Set_Chanel_Temp(CMD_PID_CHANEL4, int(t*10));
}
// 通道5 设置
void TempSet::pb_Ch5_Checkup()
{
	double t;

	t = (ui->le_Ch5->text()).toDouble();
	Set_Chanel_Temp(CMD_PID_CHANEL5, int(t*10));
}
// 通道5 设置
void TempSet::pb_Ch6_Checkup()
{
	double t;

	t = (ui->le_Ch6->text()).toDouble();
	Set_Chanel_Temp(CMD_PID_CHANEL6, int(t*10));
}
