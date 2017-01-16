#include "parasetting.h"
#include "ui_parasetting.h"
#include "paratable.h"
#include "src/HardWare/com_trandata.h"
#include "src/Thread/comtran_thread.h"
#include <QTextCodec>
#include <QFont>
#include <QPalette>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include "common.h"


ParaSetting::ParaSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParaSetting)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND2_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
	QTextCodec::setCodecForLocale(gbk);

	//Initial Para 默认值

    TargetNo = 0;

    ClassNo = 0;
    UnitNo = 0;
	ClassNo_1 = 0;


    //Font Setting
	ui->pbParaA->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbExit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbParaB->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbParaC->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbParaD->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
//	ui->pbParaTable->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbQualim->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbTestTarget->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbUnit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leParaA->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leParaB->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leParaC->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leParaD->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->cbParaClass->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leQualim->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->cbTestTarget->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->cbUnit->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->cbParaClass_2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->pbSave->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->pbR2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->leR2->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->lb_Title->setFont(QFont(FONT_USE_WQY,FONT_24,QFont::Normal));
	// 居中显示
	ui->lb_Title->setAlignment(Qt::AlignCenter);
	ui->lb_Title->setText("设置参数");
}

ParaSetting::~ParaSetting()
{
    delete ui;
}
// 显示参数
void ParaSetting::showPara(void)
{
	readCurrentCalirationParameter();

	//Combox Displaying

    //ui->cbTestTarget->setCurrentIndex(0);  // 测试指标
    ui->cbTestTarget->setCurrentIndex(TargetNo);  // 测试指标

    ui->cbParaClass->setCurrentIndex(ClassNo);    // 参数类别
	ui->cbParaClass_2->setCurrentIndex(ClassNo_1);    // 参数类别
	ui->cbUnit->setCurrentIndex(UnitNo);			// 单位

	ui->leParaA->setText(Para_A);
	ui->leParaB->setText(Para_B);
	ui->leParaC->setText(Para_C);
	ui->leParaD->setText(Para_D);
	ui->leQualim->setText(Para_Q);
	ui->leR2->setText(Para_R);

}

//初始化
void ParaSetting::Init()
{
	exit_flag = false;
	//Keyboard Setting
	im = new Keyboard_dependence;
	ui->leParaA->setFocusPolicy (Qt::ClickFocus);
	ui->leParaA->installEventFilter (im);

	ui->leParaB->setFocusPolicy (Qt::ClickFocus);
	ui->leParaB->installEventFilter (im);

	ui->leParaC->setFocusPolicy (Qt::ClickFocus);
	ui->leParaC->installEventFilter (im);

	ui->leParaD->setFocusPolicy (Qt::ClickFocus);
	ui->leParaD->installEventFilter (im);

	ui->leQualim->setFocusPolicy (Qt::ClickFocus);
	ui->leQualim->installEventFilter (im);

	ui->leR2->setFocusPolicy (Qt::ClickFocus);
	ui->leR2->installEventFilter (im);
#if 0
	//Para Reading
	QFile fconfiini(FILE_CON_PARA_SETTING);
	if (!fconfiini.exists())
	{
		WriteMachineParaIniPara();   // 预先写
	}
#else
        QFile file(FILE_QX_PARA_SETTING);  //
        // 文件不存在
        if(!file.exists())
        {
            int i = 0;
            ST_PARA_SETTING temp_set;
            file.open(QIODevice::ReadWrite);  // 打开文件

            file.seek(PRAR_TARGET_START);  // 放在开头
            file.write((char*)&TargetNo, sizeof(int));
            file.seek(PRAR_CLASS_START);  //
            file.write((char*)&ClassNo, sizeof(int));
			file.seek(PRAR_CLASS1_START);
			file.write((char*)&ClassNo_1, sizeof(int));
			// 初始值
			memset(&temp_set, 0, PRAR_ST_DATA_LEN);
			temp_set.para_A = 0.0;
			temp_set.para_B = 0.0;
			temp_set.para_C = 1;
			temp_set.para_D = 0;
			temp_set.para_Q = 1;
			temp_set.para_R = 1;
			temp_set.Unit = 0;

			file.seek(PRAR_TOL_START);  //
			for(i= 0; i < PRAR_MAX_NUM ; i++)
            {
                file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
            }

            file.seek(PRAR_FEC_START);  //

            for(i= 0; i < PRAR_MAX_NUM ; i++)
            {
                file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
            }

            file.seek(PRAR_TPC_START);  //
            for(i= 0; i < PRAR_MAX_NUM ; i++)
            {
                file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
            }

            file.seek(PRAR_E_START);  //
            for(i= 0; i <
                PRAR_MAX_NUM ; i++)
            {
                file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
            }
            file.close();
        }

#endif

	showPara();

	//Signals and Slots
	connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));


}
// 界面跳转 处理
void ParaSetting::UnInit()
{

	ui->leParaA->removeEventFilter(im);
	ui->leParaB->removeEventFilter(im);
	ui->leParaC->removeEventFilter(im);
	ui->leParaD->removeEventFilter(im);
	ui->leQualim->removeEventFilter(im);
	ui->leR2->removeEventFilter(im);

	disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
	disconnect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));

	delete im;
}
// 界面退出
void ParaSetting::on_pbExit_clicked()
{
//	getFullScree("parasetting.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		UnInit();
		this->close();
	}
}
#if 0//TEST_ONLY_TOL_FEC
// 参数表
void ParaSetting::on_pbParaTable_clicked()
{
	UnInit();
    ParaTable *pTable = new ParaTable();
    pTable->showFullScreen();
	Init();
}
#endif
void ParaSetting::insertValue(QString nustr)
{
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->clear();
    p_editer->setText(nustr);

}

void ParaSetting::keyboardDelete()
{
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->backspace();
}

/*
    函数名称：
    函数功能：初始化参数
    输入参数：
    输出参数：
    修改记录：
*/
void ParaSetting:: WriteMachineParaIniPara()
{
#if 0
	QSettings settings(FILE_CON_PARA_SETTING, QSettings::IniFormat);///写初校准参数
	settings.beginGroup(STR_PARASETTING);
	// 总大肠
	settings.setValue(STR_TOLCOIL_CLASS1_A,"0");
	settings.setValue(STR_TOLCOIL_CLASS2_A,"0");
	settings.setValue(STR_TOLCOIL_CLASS3_A,"0");
	settings.setValue(STR_TOLCOIL_CLASS4_A,"0");
	settings.setValue(STR_TOLCOIL_CLASS1_B,"0");
	settings.setValue(STR_TOLCOIL_CLASS2_B,"0");
	settings.setValue(STR_TOLCOIL_CLASS3_B,"0");
	settings.setValue(STR_TOLCOIL_CLASS4_B,"0");
	settings.setValue(STR_TOLCOIL_CLASS1_C,"0");
	settings.setValue(STR_TOLCOIL_CLASS2_C,"0");
	settings.setValue(STR_TOLCOIL_CLASS3_C,"0");
	settings.setValue(STR_TOLCOIL_CLASS4_C,"0");
	settings.setValue(STR_TOLCOIL_CLASS1_D,"0");
	settings.setValue(STR_TOLCOIL_CLASS2_D,"0");
	settings.setValue(STR_TOLCOIL_CLASS3_D,"0");
	settings.setValue(STR_TOLCOIL_CLASS4_D,"0");
	settings.setValue(STR_TOLCOIL_CLASS1_Q,"0");
	settings.setValue(STR_TOLCOIL_CLASS2_Q,"0");
	settings.setValue(STR_TOLCOIL_CLASS3_Q,"0");
	settings.setValue(STR_TOLCOIL_CLASS4_Q,"0");
	settings.setValue(STR_TOLCOIL_CLASS1_R,"0");
	settings.setValue(STR_TOLCOIL_CLASS2_R,"0");
	settings.setValue(STR_TOLCOIL_CLASS3_R,"0");
	settings.setValue(STR_TOLCOIL_CLASS4_R,"0");
	//耐热
	settings.setValue(STR_FECCOIL_CLASS1_A,"0");
	settings.setValue(STR_FECCOIL_CLASS2_A,"0");
	settings.setValue(STR_FECCOIL_CLASS3_A,"0");
	settings.setValue(STR_FECCOIL_CLASS4_A,"0");
	settings.setValue(STR_FECCOIL_CLASS1_B,"0");
	settings.setValue(STR_FECCOIL_CLASS2_B,"0");
	settings.setValue(STR_FECCOIL_CLASS3_B,"0");
	settings.setValue(STR_FECCOIL_CLASS4_B,"0");
	settings.setValue(STR_FECCOIL_CLASS1_C,"0");
	settings.setValue(STR_FECCOIL_CLASS2_C,"0");
	settings.setValue(STR_FECCOIL_CLASS3_C,"0");
	settings.setValue(STR_FECCOIL_CLASS4_C,"0");
	settings.setValue(STR_FECCOIL_CLASS1_D,"0");
	settings.setValue(STR_FECCOIL_CLASS2_D,"0");
	settings.setValue(STR_FECCOIL_CLASS3_D,"0");
	settings.setValue(STR_FECCOIL_CLASS4_D,"0");
	settings.setValue(STR_FECCOIL_CLASS1_Q,"0");
	settings.setValue(STR_FECCOIL_CLASS2_Q,"0");
	settings.setValue(STR_FECCOIL_CLASS3_Q,"0");
	settings.setValue(STR_FECCOIL_CLASS4_Q,"0");
	settings.setValue(STR_FECCOIL_CLASS1_R,"0");
	settings.setValue(STR_FECCOIL_CLASS2_R,"0");
	settings.setValue(STR_FECCOIL_CLASS3_R,"0");
	settings.setValue(STR_FECCOIL_CLASS4_R,"0");
	// 菌落总数
	settings.setValue(STR_TPCCOIL_CLASS1_A,"0");
	settings.setValue(STR_TPCCOIL_CLASS2_A,"0");
	settings.setValue(STR_TPCCOIL_CLASS3_A,"0");
	settings.setValue(STR_TPCCOIL_CLASS4_A,"0");
	settings.setValue(STR_TPCCOIL_CLASS1_B,"0");
	settings.setValue(STR_TPCCOIL_CLASS2_B,"0");
	settings.setValue(STR_TPCCOIL_CLASS3_B,"0");
	settings.setValue(STR_TPCCOIL_CLASS4_B,"0");
	settings.setValue(STR_TPCCOIL_CLASS1_C,"0");
	settings.setValue(STR_TPCCOIL_CLASS2_C,"0");
	settings.setValue(STR_TPCCOIL_CLASS3_C,"0");
	settings.setValue(STR_TPCCOIL_CLASS4_C,"0");
	settings.setValue(STR_TPCCOIL_CLASS1_D,"0");
	settings.setValue(STR_TPCCOIL_CLASS2_D,"0");
	settings.setValue(STR_TPCCOIL_CLASS3_D,"0");
	settings.setValue(STR_TPCCOIL_CLASS4_D,"0");
	settings.setValue(STR_TPCCOIL_CLASS1_Q,"0");
	settings.setValue(STR_TPCCOIL_CLASS2_Q,"0");
	settings.setValue(STR_TPCCOIL_CLASS3_Q,"0");
	settings.setValue(STR_TPCCOIL_CLASS4_Q,"0");
	settings.setValue(STR_TPCCOIL_CLASS1_R,"0");
	settings.setValue(STR_TPCCOIL_CLASS2_R,"0");
	settings.setValue(STR_TPCCOIL_CLASS3_R,"0");
	settings.setValue(STR_TPCCOIL_CLASS4_R,"0");
	// 埃希氏
	settings.setValue(STR_ECOIL_CLASS1_A,"0");
	settings.setValue(STR_ECOIL_CLASS2_A,"0");
	settings.setValue(STR_ECOIL_CLASS3_A,"0");
	settings.setValue(STR_ECOIL_CLASS4_A,"0");
	settings.setValue(STR_ECOIL_CLASS1_B,"0");
	settings.setValue(STR_ECOIL_CLASS2_B,"0");
	settings.setValue(STR_ECOIL_CLASS3_B,"0");
	settings.setValue(STR_ECOIL_CLASS4_B,"0");
	settings.setValue(STR_ECOIL_CLASS1_C,"0");
	settings.setValue(STR_ECOIL_CLASS2_C,"0");
	settings.setValue(STR_ECOIL_CLASS3_C,"0");
	settings.setValue(STR_ECOIL_CLASS4_C,"0");
	settings.setValue(STR_ECOIL_CLASS1_D,"0");
	settings.setValue(STR_ECOIL_CLASS2_D,"0");
	settings.setValue(STR_ECOIL_CLASS3_D,"0");
	settings.setValue(STR_ECOIL_CLASS4_D,"0");
	settings.setValue(STR_ECOIL_CLASS1_Q,"0");
	settings.setValue(STR_ECOIL_CLASS2_Q,"0");
	settings.setValue(STR_ECOIL_CLASS3_Q,"0");
	settings.setValue(STR_ECOIL_CLASS4_Q,"0");
	settings.setValue(STR_ECOIL_CLASS1_R,"0");
	settings.setValue(STR_ECOIL_CLASS2_R,"0");
	settings.setValue(STR_ECOIL_CLASS3_R,"0");
	settings.setValue(STR_ECOIL_CLASS4_R,"0");
	settings.setValue(STR_PARASETTING_COIL,"0");
	settings.setValue(STR_PARASETTING_UNIT,"0");
	settings.setValue(STR_PARASETTING_CLASS,"0");

	settings.setValue(STR_TOl_CLASS,"0"); // 默认是 系列1
	settings.setValue(STR_FEC_CLASS,"0");
	settings.setValue(STR_TPC_CLASS,"0");
	settings.setValue(STR_E_CLASS,"0");

	settings.setValue(STR_TOl_UNIT,"0"); // 默认是 单位1
	settings.setValue(STR_FEC_UNIT,"0");
	settings.setValue(STR_TPC_UNIT,"0");
	settings.setValue(STR_E_UNIT,"0");


    settings.endGroup();
#endif
}

/*
    函数名称：
    函数功能：读取校准参数
    输入参数：
    输出参数：
    修改记录：
*/
void ParaSetting::readCurrentCalirationParameter()
{

	ST_PARA_SETTING temp_set;
	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadOnly);  // 打开文件 只读

	file.seek(PRAR_TARGET_START);
	file.read((char*)&TargetNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS_START);
	file.read((char*)&ClassNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS1_START);
	file.read((char*)&ClassNo_1, PRAR_DATA_LEN);
    qDebug()<<"读参数输出****";
	printf("TargetNo = %d ClassNo= %d, ClassNo_1= %d\r\n", TargetNo, ClassNo, ClassNo_1);

    if(1 == TEST_ONLY_TPC)
    {
        TargetNo = (TPC_COLI_ID - 1);
    }

	if((TOL_COLI_ID - 1) == TargetNo)  // 总大肠
	{
		file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
		printf("PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if((FEC_COLI_ID - 1) == TargetNo)  // 耐热大肠
	{
		file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
//                file.seek(PRAR_FEC_START+PRAR_ST_DATA_LEN*ClassNo);
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if((TPC_COLI_ID - 1) == TargetNo)  // 菌落总数
	{
		file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		//file.seek(PRAR_TPC_START+PRAR_ST_DATA_LEN*ClassNo);
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if((E_COLI_ID - 1) == TargetNo)  // 埃希氏
	{
		file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	   // file.seek(PRAR_E_START+PRAR_ST_DATA_LEN*ClassNo);
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	file.close();

	printf("temp_set.para_A = %f temp_set.para_B= %f, temp_set.para_C= %f\r\n", temp_set.para_A, temp_set.para_B, temp_set.para_C);
	printf("temp_set.para_D = %f temp_set.para_Q = %f, temp_set.para_R= %f\r\n", temp_set.para_D, temp_set.para_Q, temp_set.para_R);

	Para_A = QString::number(temp_set.para_A);
	Para_B = QString::number(temp_set.para_B);
	Para_C = QString::number(temp_set.para_C);
	Para_D = QString::number(temp_set.para_D);
	Para_Q = QString::number(temp_set.para_Q);
	Para_R = QString::number(temp_set.para_R);
	UnitNo = temp_set.Unit;

	qDebug()<<Para_A<<Para_B<<Para_C<<Para_D<<Para_Q<<Para_R<<UnitNo;

    if(temp_set.para_A == 0 || temp_set.para_B == 0 )
         QMessageBox::information(this,STR_PROMPT,tr("读取参数为0，请重新输入"));
  /*  else
         QMessageBox::information(this,STR_PROMPT,tr("读取成功"));*/
}

/*
    函数名称：
    函数功能：选择指标
    输入参数：
    输出参数：
    修改记录：
*/
void ParaSetting::on_cbTestTarget_currentIndexChanged(int index)
{
	TargetNo = index;
#if 1
	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadWrite);  // 打开文件 只读

	file.seek(PRAR_TARGET_START);
	file.write((char*)&TargetNo, PRAR_DATA_LEN);

	file.close();
#else
	// 保存 下次进来后 进入此项
	QSettings settings(FILE_CON_PARA_SETTING, QSettings::IniFormat);///写初校准参数
	settings.beginGroup(STR_PARASETTING);
	settings.setValue(STR_PARASETTING_COIL,QString::number(index));
	settings.endGroup();
#endif
	showPara();
}

/*
    函数名称：
    函数功能：选择单位
    输入参数：
    输出参数：
    修改记录：
*/
void ParaSetting::on_cbUnit_currentIndexChanged(int index)
{

	UnitNo = index;

/*	ST_PARA_SETTING temp_set;

	temp_set.para_A = Para_A.toDouble();
	temp_set.para_B = Para_B.toDouble();
	temp_set.para_C = Para_C.toDouble();
	temp_set.para_D = Para_D.toDouble();
	temp_set.para_Q = Para_Q.toDouble();
	temp_set.para_R = Para_R.toDouble();
	temp_set.Unit = UnitNo;

	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadWrite);  // 打开文件 只读

	file.seek(PRAR_TARGET_START);
	file.write((char*)&TargetNo, PRAR_DATA_LEN);

    if(1 == TEST_ONLY_TPC)
    {
        TargetNo = TPC_COLI_ID - 1;
    }

	if((TOL_COLI_ID - 1) == TargetNo)  // 总大肠
	{
		file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
	}
	else if((FEC_COLI_ID - 1) == TargetNo)  // 耐热大肠
	{
		file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
//                file.seek(PRAR_FEC_START+PRAR_ST_DATA_LEN*ClassNo);
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
	}
	else if((TPC_COLI_ID - 1) == TargetNo)  // 菌落总数
	{
		file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		//file.seek(PRAR_TPC_START+PRAR_ST_DATA_LEN*ClassNo);
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
	}
	else if((E_COLI_ID - 1) == TargetNo)  // 埃希氏
	{
		file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	   // file.seek(PRAR_E_START+PRAR_ST_DATA_LEN*ClassNo);
		memset(&temp_set, 0,PRAR_ST_DATA_LEN);
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
	}
    file.close();*/


	//showPara();
}
// 选择曲线  水类别
void ParaSetting::on_cbParaClass_currentIndexChanged(int index)
{

	ClassNo = index;

	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadWrite);  // 打开文件 只读

	file.seek(PRAR_CLASS_START);
	file.write((char*)&ClassNo, PRAR_DATA_LEN);

	file.close();

	showPara();

}

/*
    函数名称：
	函数功能：保存数据
    输入参数：
    输出参数：
    修改记录：
*/
void ParaSetting::on_pbSave_clicked()
{
	ST_PARA_SETTING temp_set;

	temp_set.para_A = ui->leParaA->text().toDouble();
    temp_set.para_B = ui->leParaB->text().toDouble();
    temp_set.para_C = ui->leParaC->text().toDouble();
    temp_set.para_D = ui->leParaD->text().toDouble();
    temp_set.para_Q = ui->leQualim->text().toDouble();
    temp_set.para_R = ui->leR2->text().toDouble();
	temp_set.Unit = UnitNo;

    qDebug()<<"保存按钮输出参数***";
	printf("@@ temp_set.para_A = %f temp_set.para_B= %f, temp_set.para_C= %f\r\n", temp_set.para_A, temp_set.para_B, temp_set.para_C);
	printf("@@ temp_set.para_D = %f temp_set.para_Q = %f, temp_set.para_R= %f\r\n", temp_set.para_D, temp_set.para_Q, temp_set.para_R);

	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadWrite);  // 打开文件 只读

	file.seek(PRAR_TARGET_START);
	file.write((char*)&TargetNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS_START);
	file.write((char*)&ClassNo, PRAR_DATA_LEN);
	file.seek(PRAR_CLASS1_START);
	file.write((char*)&ClassNo_1, PRAR_DATA_LEN);

	printf("TargetNo = %d ClassNo= %d, ClassNo_1= %d\r\n", TargetNo, ClassNo, ClassNo_1);

    if(1 == TEST_ONLY_TPC)
    {
        TargetNo = TPC_COLI_ID - 1;
    }

	if((TOL_COLI_ID - 1) == TargetNo)  // 总大肠
	{
		file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);

		printf("PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if((FEC_COLI_ID - 1) == TargetNo)  // 耐热大肠
	{
		file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if((TPC_COLI_ID - 1) == TargetNo)  // 菌落总数
	{
		file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}
	else if((E_COLI_ID - 1) == TargetNo)  // 埃希氏
	{
		file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
		file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
	}

	file.close();
    if((TOL_COLI_ID - 1) == TargetNo)  // 总大肠
    {
        file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
        file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
    }
    else if((FEC_COLI_ID - 1) == TargetNo)  // 耐热大肠
    {
        file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
        file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
    }
    else if((TPC_COLI_ID - 1) == TargetNo)  // 菌落总数
    {
        file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
        file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
    }
    else if((E_COLI_ID - 1) == TargetNo)  // 埃希氏
    {
        file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
        file.read((char*)&temp_set, PRAR_ST_DATA_LEN);
    }
    file.close();
    if(temp_set.para_A == 0 || temp_set.para_B == 0 )
         QMessageBox::information(this,STR_PROMPT,tr("保存失败"));
    else
         QMessageBox::information(this,STR_PROMPT,tr("保存成功"));

 /*   QString fileName = QFileDialog::getSaveFileName(this,
            tr("文件保存"),FILE_QX_PARA_SETTING,0);
        if (!fileName.isNull())
        {
           QMessageBox::information(this,STR_PROMPT,tr("保存成功"));
        }
        else
        {
            QMessageBox::information(this,STR_PROMPT,tr("保存失败"));
        }*/

	qDebug()<< "save ok ================";

}

union
{
 float sub_float;
 struct  {
     char b[4];
      } sep_float ;
}u_float;
/*
    函数名称：
    函数功能：将4个字节转换成float
    输入参数：
    输出参数：
    修改记录：
*/
float ParaSetting::char4ToFloat(char *arrary, float write_data)
{
    u_float.sep_float.b[0] = arrary[3];
    u_float.sep_float.b[1] = arrary[2];
    u_float.sep_float.b[2] = arrary[1];
    u_float.sep_float.b[3] = arrary[0];

    write_data = u_float.sub_float;
    return write_data;
}
/*
    函数名称：
    函数功能：将float转换成4个字节
    输入参数：
    输出参数：
    修改记录：
*/
void ParaSetting::floatToChar4(char *arrary, float write_data)
{

    u_float.sub_float = write_data;

    arrary[0] = u_float.sep_float.b[3];
    arrary[1] = u_float.sep_float.b[2];
    arrary[2] = u_float.sep_float.b[1];
    arrary[3] = u_float.sep_float.b[0];

}


//曲线
void ParaSetting::on_cbParaClass_2_currentIndexChanged(int index)
{
	ClassNo_1 = index;

	QFile file(FILE_QX_PARA_SETTING);  //

	file.open(QIODevice::ReadWrite);  // 打开文件 只读

	file.seek(PRAR_CLASS1_START);
	file.write((char*)&ClassNo_1, PRAR_DATA_LEN);

	file.close();

	showPara();
}


