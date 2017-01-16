#include "netsetting.h"
#include "ui_netsetting.h"
#include "common.h"
#include <QString>
#include <QTextCodec>

NetSetting::NetSetting(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::NetSetting)
{
	ui->setupUi(this);

	//Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND2_PNG);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(pixmap));
	setPalette(palette);
	// text
	//ui->lb_NetSet->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //本地IP

	ui->lb_LocalIP->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //本地IP
	ui->lb_ServerIP->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //服务器IP
	ui->lb_Protocol->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //协议
	ui->lb_Prot->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //本地 端口

    ui->lb_NetSet->setFont(QFont(FONT_USE_WQY, FONT_24, QFont::Normal));  // title
	// lineedit
	ui->le_LocalIP->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //本地IP
	ui->le_ServerIP->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //服务器IP

	ui->le_Port->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //端口
	// button
    ui->pb_Set->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  // 设置按钮
    ui->pb_Exit->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  // 返回按钮
	//combox
	ui->cb_Protocol->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //协议
	ui->cb_Protocol->addItem(tr(STR_UPD));   //  udp

	ui->cb_Work_Mode->setFont(QFont(FONT_USE_WQY, FONT_16, QFont::Normal));  //协议
	ui->cb_Protocol->addItem(tr(STR_WORK_MODE1));   //  udp
	ui->cb_Protocol->addItem(tr(STR_WORK_MODE2));   //  udp
	ui->cb_Work_Mode->setCurrentIndex(0);  // 默认是 0

}

NetSetting::~NetSetting()
{
	delete ui;
}
// 界面初始化处理函数
void NetSetting::NetSetting_init()
{

	exit_flag = false;
	//Keyboard Setting
	im = new Keyboard_dependence;
	ui->le_LocalIP->setFocusPolicy (Qt::ClickFocus);
	ui->le_LocalIP->installEventFilter (im);

	ui->le_ServerIP->setFocusPolicy (Qt::ClickFocus);
	ui->le_ServerIP->installEventFilter (im);

	ui->le_Port->setFocusPolicy (Qt::ClickFocus);
	ui->le_Port->installEventFilter (im);
	// 显示本机IP
	ui->le_LocalIP->setText(pUDPCommunication->getClientIP());
	// 服务器 IP 显示  服务器端口号显示
	pUDPCommunication->getServeIP();
	ui->le_ServerIP->setText((pUDPCommunication->serverAddress).toString());
	ui->le_Port->setText(QString::number(pUDPCommunication->serverPort));

	// 工作模式
	ui->cb_Work_Mode->setCurrentIndex(g_workmode);

	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(NetSetting_KeyValue(QString)));
	//connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));

}

// 界面跳转处理函数
void NetSetting::NetSetting_Uninit()
{
	ui->le_LocalIP->removeEventFilter(im);
	ui->le_ServerIP->removeEventFilter (im);
	ui->le_Port->removeEventFilter (im);
	//del Keyboard Setting
	disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(NetSetting_KeyValue(QString)));

	delete im;
}
// 键盘回传函数 显示
void NetSetting::NetSetting_KeyValue(QString str)
{
	QLineEdit *p_editer;

	p_editer = qobject_cast<QLineEdit*>(focusWidget());
	p_editer->clear();
	p_editer->setText(str);

}

// 界面退出函数
void NetSetting::on_pb_Exit_clicked()
{
//	getFullScree("netsetting.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		NetSetting_Uninit();
		this->close();
	}
}
// 界面 设置 保存按钮
void NetSetting::on_pb_Set_clicked()
{
	UDPCommunicationInfo.localIP = ui->le_LocalIP->text();
	UDPCommunicationInfo.serveIP = ui->le_ServerIP->text();
	UDPCommunicationInfo.servePort = ui->le_Port->text();
	// 获取工作模式
	g_workmode = ui->cb_Work_Mode->currentIndex();

//	QMutex mutex;
//	mutex.lock();

	QSettings settings(FILE_NET_SETTING, QSettings::IniFormat);
	settings.beginGroup("CommunicationNerwork");
	settings.setValue("ClientIP",UDPCommunicationInfo.localIP);
	settings.setValue("ServiceIP",UDPCommunicationInfo.serveIP);
	settings.setValue("NetPort",UDPCommunicationInfo.servePort);
	settings.setValue("workmode",QString::number(g_workmode));
	settings.endGroup();

	//设定本机IP
	QString strifconfigset="ifconfig eth0 "+UDPCommunicationInfo.localIP;
	system(QstringTostring(strifconfigset));
//	mutex.unlock();
}
