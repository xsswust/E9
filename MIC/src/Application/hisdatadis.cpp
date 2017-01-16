#include "hisdatadis.h"
#include "ui_hisdatadis.h"
#include "confirmclean.h"
#include "src/HardWare/app.h"
#include "hisdata.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QSettings>
#include "common.h"
#include <QSqlTableModel>
#include "sql_control.h"
#include <QSqlRecord>
#include <QMessageBox>

extern unsigned int gHisDataMode;  // 历史记录的模式

HisDataDis::HisDataDis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HisDataDis)
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
	//ui->pushPrintButton->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pushCleanButton->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
    ui->pushExitButton->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));
	ui->tableView->setFont(QFont(FONT_USE_WQY,FONT_12,QFont::Normal));

	ui->lb_Title->setAlignment(Qt::AlignCenter);
	ui->lb_Title->setFont(QFont(FONT_USE_WQY,FONT_24,QFont::Normal));
#if 0
	ui->tableWidget->setColumnCount(7);//设置列数为
    ui->tableWidget->setColumnWidth(0,DATA_TABLE1_W);
    ui->tableWidget->setColumnWidth(1,DATA_TABLE2_W);
    ui->tableWidget->setColumnWidth(2,DATA_TABLE3_W);
    ui->tableWidget->setColumnWidth(3,DATA_TABLE4_W);
    ui->tableWidget->setColumnWidth(4,DATA_TABLE5_W);
    ui->tableWidget->setColumnWidth(5,DATA_TABLE6_W);
#endif
}

HisDataDis::~HisDataDis()
{
    delete ui;
}

/*
	函数名称：
	函数功能：初始化函数
	输入参数：
	输出参数：
	修改记录：
*/
void HisDataDis::HisDataDis_init()
{
	//DisPlayDensity();  //数据显示
	exit_flag = false;
	QSqlTableModel *model;

	model = new QSqlTableModel(0, gp_sql->db);

#if 0
	ui->tableView->setModel(model);
	ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
	ui->tableView->setColumnWidth(1,DATA_TABLE1_W);
	ui->tableView->setColumnWidth(2,DATA_TABLE2_W);
	ui->tableView->setColumnWidth(3,DATA_TABLE3_W);
	ui->tableView->setColumnWidth(4,DATA_TABLE4_W);
	ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
	ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
	ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	if(TOL_COLI_ID == gHisDataMode){
		model->setTable(STR_TEST_TOLCOLI);  // 设置table 为 总大肠
		ui->lb_Title->setText(STR_TEST_TOLCOLI_HIS);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,DATA_TABLE1_W);
		ui->tableView->setColumnWidth(2,DATA_TABLE2_W);
		ui->tableView->setColumnWidth(3,DATA_TABLE3_W);
		ui->tableView->setColumnWidth(4,DATA_TABLE4_W);
		ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
        ui->tableView->setColumnWidth(7,DATA_TABLE7_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(FEC_COLI_ID ==  gHisDataMode ){
		model->setTable(STR_TEST_FECCOLI);  // 设置table 为 耐热大肠
		ui->lb_Title->setText(STR_TEST_FECCOLI_HIS);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,DATA_TABLE1_W);
		ui->tableView->setColumnWidth(2,DATA_TABLE2_W);
		ui->tableView->setColumnWidth(3,DATA_TABLE3_W);
		ui->tableView->setColumnWidth(4,DATA_TABLE4_W);
		ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
        ui->tableView->setColumnWidth(7,DATA_TABLE7_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(E_COLI_ID == gHisDataMode){
		model->setTable(STR_TEST_ECOLI);	// 设置table 为 埃希氏
		ui->lb_Title->setText(STR_TEST_ECOLI_HIS);

#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,DATA_TABLE1_W);
		ui->tableView->setColumnWidth(2,DATA_TABLE2_W);
		ui->tableView->setColumnWidth(3,DATA_TABLE3_W);
		ui->tableView->setColumnWidth(4,DATA_TABLE4_W);
		ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
        ui->tableView->setColumnWidth(7,DATA_TABLE7_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(TPC_COLI_ID ==  gHisDataMode){
		model->setTable(STR_TEST_TPCCOLI);	// 设置table为 菌落总数
		ui->lb_Title->setText(STR_TEST_TPCCOLI_HIS);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,DATA_TABLE1_W);
		ui->tableView->setColumnWidth(2,DATA_TABLE2_W);
		ui->tableView->setColumnWidth(3,DATA_TABLE3_W);
		ui->tableView->setColumnWidth(4,DATA_TABLE4_W);
		ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
        ui->tableView->setColumnWidth(7,DATA_TABLE7_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(TEMP1_ID == gHisDataMode)
	{
		model->setTable(STR_TEMP1);	// 设置table为 菌落总数
		ui->lb_Title->setText(STR_TEMP1);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,300);
		ui->tableView->setColumnWidth(2,200);
		ui->tableView->setColumnWidth(3,180);
		ui->tableView->setColumnWidth(4,200);
		//ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		//ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(TEMP2_ID == gHisDataMode)
	{
		model->setTable(STR_TEMP2);	// 设置table为 菌落总数
		ui->lb_Title->setText(STR_TEMP2);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,300);
		ui->tableView->setColumnWidth(2,200);
		ui->tableView->setColumnWidth(3,180);
		ui->tableView->setColumnWidth(4,200);
		//ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		//ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(TEMP3_ID == gHisDataMode)
	{
		model->setTable(STR_TEMP3);	// 设置table为 菌落总数
		ui->lb_Title->setText(STR_TEMP3);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,300);
		ui->tableView->setColumnWidth(2,200);
		ui->tableView->setColumnWidth(3,180);
		ui->tableView->setColumnWidth(4,200);
		//ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		//ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(TEMP4_ID == gHisDataMode)
	{
		model->setTable(STR_TEMP4);	// 设置table为 菌落总数
		ui->lb_Title->setText(STR_TEMP4);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,300);
		ui->tableView->setColumnWidth(2,200);
		ui->tableView->setColumnWidth(3,180);
		ui->tableView->setColumnWidth(4,200);
		//ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		//ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}
	else if(TEMP5_ID == gHisDataMode)
	{
		model->setTable(STR_TEMP5);	// 设置table为 菌落总数
		ui->lb_Title->setText(STR_TEMP5);
#if 1
		ui->tableView->setModel(model);
		ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
		ui->tableView->setColumnWidth(1,300);
		ui->tableView->setColumnWidth(2,200);
		ui->tableView->setColumnWidth(3,180);
		ui->tableView->setColumnWidth(4,200);
		//ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
		//ui->tableView->setColumnWidth(6,DATA_TABLE6_W);
		ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列
#endif
	}


	model->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 选中整个表的所有行
	model->select();

//	model->removeColumn(0); // don't show the ID

	//model->setSort(0,Qt::AscendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
	model->setSort(0,Qt::DescendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
	model->select();
	//model->removeColumn(0); // don't show the ID



}


/*
	函数名称：
	函数功能：清空数据处理
	输入参数：
	输出参数：
	修改记录：
*/
void HisDataDis::on_pushCleanButton_clicked()
{
//    ConfirmClean *conClean = new ConfirmClean();
//	conClean->showFullScreen();
	switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_CLEAR_DATA),
								 QMessageBox::Ok|QMessageBox::Cancel,
								 QMessageBox::Cancel))
	{
		case QMessageBox::Ok:
//			getFullScree("clean_his_data.jpg");
			gp_sql->Del_All_Data(gHisDataMode);
            if(TEMP5_ID == gHisDataMode)
            {
                gp_sql->Del_All_Data(TEMP6_ID);
            }
			HisDataDis_init();
			break;

		case QMessageBox::Cancel:
			//getFullScree("clean_his_data.jpg");
			break;

		default:
			//getFullScree("clean_his_data.jpg");
			break;
	}
static int num = 1;

	char temp[20];
	memset(temp , 0 , sizeof(temp));
	sprintf(temp, "clean_his_data%d.jpg", num);
	getFullScree(temp);
	num ++;
}

/*
	函数名称：
	函数功能：界面销毁
	输入参数：
	输出参数：
	修改记录：
*/
void HisDataDis::on_pushExitButton_clicked()
{
	getFullScree("hisdis.jpg");
	if(false == exit_flag)
	{
		exit_flag = true;
		this->close();
	}
}

/*
    函数名称：
    函数功能：历史数据显示
    输入参数：
    输出参数：
    修改记录：
*/
void HisDataDis::DisPlayDensity()
{

	QSqlTableModel *model;

	model = new QSqlTableModel(0, gp_sql->db);

	if(TOL_COLI_ID == gHisDataMode){
		model->setTable(STR_TEST_TOLCOLI);  // 设置table 为 总大肠
	}
	else if(FEC_COLI_ID ==  gHisDataMode ){
		model->setTable(STR_TEST_TOLCOLI);  // 设置table 为 耐热大肠
	}
	else if(E_COLI_ID == gHisDataMode){
		model->setTable(STR_TEST_ECOLI);	// 设置table 为 埃希氏
	}
	else if(TPC_COLI_ID ==  gHisDataMode){
		model->setTable(STR_TEST_TPCCOLI);	// 设置table为 菌落总数
	}
	else if(TEMP1_ID == gHisDataMode )
	{
		model->setTable(STR_TEMP1);	// 设置table为 菌落总数
	}


	model->setEditStrategy(QSqlTableModel::OnManualSubmit);  // 选中整个表的所有行
	model->select();

//	model->removeColumn(0); // don't show the ID

	//model->setSort(0,Qt::AscendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
	model->setSort(0,Qt::DescendingOrder); //id属性，即第0列，升序排列,Qt::DescendingOrder为降序排序
	model->select();
	//model->removeColumn(0); // don't show the ID

	ui->tableView->setModel(model);
	ui->tableView->setColumnWidth(0,DATA_TABLE0_W);
	ui->tableView->setColumnWidth(1,DATA_TABLE1_W);
	ui->tableView->setColumnWidth(2,DATA_TABLE2_W);
	ui->tableView->setColumnWidth(3,DATA_TABLE3_W);
	ui->tableView->setColumnWidth(4,DATA_TABLE4_W);
	ui->tableView->setColumnWidth(5,DATA_TABLE5_W);
	ui->tableView->setColumnHidden(0, true);  // 隐藏 iD这一列

	//delete model;
}
