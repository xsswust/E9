#include "testingdis.h"
#include "ui_testingdis.h"
#include "confirmstop.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QSettings>
#include "common.h"


TestingDis::TestingDis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingDis)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // 中文显示
//	QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
//	QTextCodec::setCodecForCStrings(gbk);
    //Font Setting
    ui->pb1->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pb2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pb3->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pb4->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pbExit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pbFinished->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pbStop->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pbIniVal->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pbOverVal->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->label->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->leIniVal->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->leOverVal->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    //

}
// 界面销毁
TestingDis::~TestingDis()
{
    delete ui;
}
// 初始化
void TestingDis::TestDis_Init()
{
	exit_flag = false;
	testNo = 0;
	connect(pcomtran_thread,SIGNAL(emitEColiIniVal()),this,SLOT(receiveIniVal()));
	connect(pcomtran_thread,SIGNAL(emitEColiOverVal()),this,SLOT(receiveOverVal()));
}

// 界面跳出处理
void TestingDis::TestDis_Uninit()
{
	testNo = 0;
	disconnect(pcomtran_thread,SIGNAL(emitEColiIniVal()),this,SLOT(receiveIniVal()));
	disconnect(pcomtran_thread,SIGNAL(emitEColiOverVal()),this,SLOT(receiveOverVal()));
}

void TestingDis::on_pbExit_clicked()
{
//	getFullScree("testdis.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		TestDis_Uninit();
		this->close();
	}
}

void TestingDis::on_pbStop_clicked()
{
    ConfirmStop *conStop = new ConfirmStop();
    conStop->show();
}

void TestingDis::on_pb1_clicked()
{
    testNo = 1;
}

void TestingDis::on_pb2_clicked()
{
    testNo = 2;
}

void TestingDis::on_pb3_clicked()
{
    testNo = 3;
}

void TestingDis::on_pb4_clicked()
{
    testNo = 4;
}

/*
    函数名称：
    函数功能：发送测试信号
    输入参数：
    输出参数：
    修改记录：
*/
void TestingDis::on_pbIniVal_clicked()
{

}

/*
    函数名称：
    函数功能：发送停止信号
    输入参数：
    输出参数：
    修改记录：
*/
void TestingDis::on_pbOverVal_clicked()
{

}

/*
    函数名称：
    函数功能：发送测试完成信号
    输入参数：
    输出参数：
    修改记录：
*/
void TestingDis::on_pbFinished_clicked()
{


}
//
void TestingDis::receiveIniVal()
{
	QSettings settings(FILE_ACURATE_DENSITY , QSettings::IniFormat);
    settings.beginGroup("GroupEColiDensity");
    ui->leIniVal->setText(settings.value("IniVal").toString());
    settings.endGroup();
}
//
void TestingDis::receiveOverVal()
{
	QSettings settings(FILE_ACURATE_DENSITY , QSettings::IniFormat);
    settings.beginGroup("GroupEColiDensity");
    ui->leOverVal->setText(settings.value("OverVal").toString());
    settings.endGroup();
}

