#include "syshelp.h"
#include "ui_syshelp.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"
#include "mainwindow.h"

QString helpinfo = "\r\n\
1、使用仪器前请仔细阅读说明书。\r\n\
2、本产品使用的培养液为易耗品。一支培养液只能检测一个样品，不能重复使用。\r\n\
3、培养液的储存温度为0~4C，必须避光保存。\r\n\
4、培养液若出现浑浊或有絮状物现象，请勿使用。\r\n\
5、样品检测完毕后请将培养瓶高温灭菌后集中处理或待本公司回收。\r\n\
6、本仪器使用的培养液均由青岛佳明测控科技股份有限公司有偿供。\r\n\
7、本仪器在开机后需预热60min才能达到最佳测试温度。\r\n\
8、仪器故障可联系客服。\r\n\
\r\n\
			  售后电话：400 6527 088";
SysHelp::SysHelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysHelp)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
    QTextCodec::setCodecForLocale(gbk);
    //QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);
	ui->pb_sysInfo->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));
	ui->pushButton->setFont(QFont(FONT_USE_WQY,FONT_16,QFont::Normal));
    // 设置字体大小
	ui->textEdit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->textEdit->setReadOnly(true); // 禁止编辑
    ui->textEdit->setTextInteractionFlags(Qt::NoTextInteraction);  // 禁止选中文本
	connect(ui->pb_sysInfo, SIGNAL(released()), this, SLOT(on_pb_sysInfo_released));

	exit_flag = false;

	sysinfo_flag = false;
}

SysHelp::~SysHelp()
{
    delete ui;
}
// 界面跳出函数
void SysHelp::on_pushButton_clicked()
{
//	getFullScree("help.jpg");

	if(false == sysinfo_flag)
	{
		if(false == exit_flag)
		{
			exit_flag = true;
			this->close();
			pmainwin->MainWindow_Init(); // 恢复状态
		}
	}
	else
	{
		getFullScree("help-vi.jpg");
		ui->textEdit->setText(helpinfo);
		sysinfo_flag = false;
	}
}
// 显示 系统信息
void SysHelp::on_pb_sysInfo_released()
{


	if(false == sysinfo_flag)
	{
		ui->textEdit->setText(QString("%1\r\n%2\r\n%3\r\n%4	%5 %6\r\n")\
							  .arg(SYS_PRODUCT_NAME)\
							  .arg(SYS_PRODUCT_TYPE)\
							  .arg(SYS_PRODUCT_VER)\
							  .arg(SYS_PRODUCT_TIME)\
							  .arg(__DATE__).arg(__TIME__));
		sysinfo_flag = true;
	}
}
