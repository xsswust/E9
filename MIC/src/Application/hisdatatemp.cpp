#include "hisdatatemp.h"
#include "ui_hisdatatemp.h"
#include "src/HardWare/app.h"
#include <QTextCodec>
#include <QFont>
#include <QPalette>
#include <QSettings>
#include <QString>
#include "common.h"
#include <QSqlTableModel>
#include "sql_control.h"
#include <QMessageBox>

extern int gHisDataMode;

HisDataTemp::HisDataTemp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HisDataTemp)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND2_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
//    QTextCodec::setCodecForTr(gbk);
    QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);
    //Font Setting
    ui->pushCleanButton->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pushExitButton->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
#if 0
	ui->->setColumnWidth(0,160);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget2->setColumnWidth(0,160);
    ui->tableWidget2->setColumnWidth(1,100);
#endif
}

HisDataTemp::~HisDataTemp()
{
    delete ui;
}

/*
	函数名称：
	函数功能：返回按钮
	输入参数：
	输出参数：
	修改记录：
*/
void HisDataTemp::HisDataTemp_init()
{
	QSqlTableModel *Temp1_model;
	QSqlTableModel *Temp2_model;


	Temp1_model = new QSqlTableModel(0, gp_sql->db);
	Temp2_model = new QSqlTableModel(0, gp_sql->db);

	Temp1_model->setTable(STR_TEMP1);  // 设置table 为 总大肠
	Temp2_model->setTable(STR_TEMP2);  // 设置table 为 总大肠


	Temp1_model->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 选中整个表的所有行
	Temp1_model->select();
	Temp2_model->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 选中整个表的所有行
	Temp2_model->select();

	Temp1_model->setSort(0,Qt::DescendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
	Temp1_model->select();
	Temp2_model->setSort(0,Qt::DescendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
	Temp2_model->select();

	if(TEMP1_ID == gHisDataMode )
	{
		ui->tv_Temp365->setModel(Temp1_model);
		ui->tv_Temp445->setModel(Temp2_model);
	}
	else if(TEMP2_ID == gHisDataMode)
	{
		ui->tv_Temp365->setModel(Temp2_model);
		ui->tv_Temp445->setModel(Temp1_model);
	}

	ui->tv_Temp365->setColumnWidth(0,TEMP_DATA_TABLE0_W);
	ui->tv_Temp365->setColumnWidth(1,TEMP_DATA_TABLE1_W);
	ui->tv_Temp365->setColumnWidth(2,TEMP_DATA_TABLE2_W);

	ui->tv_Temp445->setColumnWidth(0,TEMP_DATA_TABLE0_W);
	ui->tv_Temp445->setColumnWidth(1,TEMP_DATA_TABLE1_W);
	ui->tv_Temp445->setColumnWidth(2,TEMP_DATA_TABLE2_W);

	ui->tv_Temp365->setColumnHidden(0, true);  // 隐藏 iD这一列
	ui->tv_Temp445->setColumnHidden(0, true);  // 隐藏 iD这一列

//	DisPlayTemp();
}

/*
	函数名称：
	函数功能：返回按钮
	输入参数：
	输出参数：
	修改记录：
*/
void HisDataTemp::on_pushExitButton_clicked()
{
    this->close();
}
#if 0
/*
    函数名称：
    函数功能：历史温度显示
    输入参数：
    输出参数：
    修改记录：
*/
void HisDataTemp::DisPlayTemp()
{

    if(TEMP1_ID == gHisDataMode )
    {
		showHistoryData( gHisDataMode,FILE_HIS_TEMP1,ui->tableWidget);
		showHistoryData(gHisDataMode,FILE_HIS_TEMP2,ui->tableWidget2);
    }
    else if(TEMP2_ID == gHisDataMode)
    {
		showHistoryData(gHisDataMode,FILE_HIS_TEMP2,ui->tableWidget2);
		showHistoryData(gHisDataMode,FILE_HIS_TEMP1,ui->tableWidget);
    }
}
#endif
// 数据清空处理
void HisDataTemp::on_pushCleanButton_clicked()
{

	switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_CLEAR_DATA),
								 QMessageBox::Ok|QMessageBox::Cancel,
								 QMessageBox::Cancel))
	{
		case QMessageBox::Ok:
			gp_sql->Del_All_Data(gHisDataMode);
			gp_sql->Del_All_Data(TEMP3_ID);
			gp_sql->Del_All_Data(TEMP4_ID);
			gp_sql->Del_All_Data(TEMP5_ID);
			gp_sql->Del_All_Data(TEMP6_ID);
			HisDataTemp_init();
			break;

		case QMessageBox::Cancel:
			break;

		default:
			break;
	}
}
