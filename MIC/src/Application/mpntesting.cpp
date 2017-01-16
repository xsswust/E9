#include "mpntesting.h"
#include "ui_mpntesting.h"
#include "common.h"
//int g_mpn_st = false;  // mpn测试状态
int mul[] ={1,10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

mpnTesting::mpnTesting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mpnTesting)
{
    ui->setupUi(this);

    QPixmap pixmap(PIC_BACKGROUND2_PNG);
    QPalette palette;

    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

    qDebug()<<"mpnTesting g_mpn_test_st =="+ g_mpn_test_st ;
    printf("[mpnTesting] g_mpn_test_st=== %d\r\n", g_mpn_test_st);
    // 获取测试状态
    if(MPN_TEST_IDLE == g_mpn_test_st){
        ui->pb_FEC->setDisabled(false);
        ui->pb_TOL->setDisabled(false);
        ui->pb_FEC->setText(STR_TEST_START);
        ui->pb_TOL->setText(STR_TEST_START);
    }
    else if(MPN_TEST_FEC == g_mpn_test_st){
        ui->pb_FEC->setDisabled(false);
        ui->pb_TOL->setDisabled(true);
        ui->pb_FEC->setText(STR_TEST_STOP);
        ui->pb_TOL->setText(STR_TEST_START);
    }
    else if(MPN_TEST_TOL == g_mpn_test_st){
        ui->pb_FEC->setDisabled(true);
        ui->pb_TOL->setDisabled(false);
        ui->pb_FEC->setText(STR_TEST_START);
        ui->pb_TOL->setText(STR_TEST_STOP);
    }
   ui->comboBox->setCurrentIndex(g_mpn_test_index);

   ptimerUpdateTime = new QTimer();

   connect(ptimerUpdateTime,SIGNAL(timeout()),this,SLOT(timeUpdate()));
}

mpnTesting::~mpnTesting()
{
    delete ui;

}


/*
    函数名称：
    函数功能：时间刷新
    输入参数：
    输出参数：
    修改记录：
*/
void mpnTesting::timeUpdate()
{
    // 获取测试状态
    if(MPN_TEST_IDLE == g_mpn_test_st){
        ui->pb_FEC->setDisabled(false);
        ui->pb_TOL->setDisabled(false);
        ui->pb_FEC->setText(STR_TEST_START);
        ui->pb_TOL->setText(STR_TEST_START);
    }
    else if(MPN_TEST_FEC == g_mpn_test_st){
        ui->pb_FEC->setDisabled(false);
        ui->pb_TOL->setDisabled(true);
        ui->pb_FEC->setText(STR_TEST_STOP);
        ui->pb_TOL->setText(STR_TEST_START);
    }
    else if(MPN_TEST_TOL == g_mpn_test_st){
        ui->pb_FEC->setDisabled(true);
        ui->pb_TOL->setDisabled(false);
        ui->pb_FEC->setText(STR_TEST_START);
        ui->pb_TOL->setText(STR_TEST_STOP);
    }
}
// 开始测试 耐热大肠
void mpnTesting::on_pb_FEC_released()
{
    QString txt;

    txt = ui->pb_FEC->text();
    if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
    {
        g_mpn_test_st = MPN_TEST_FEC;
        for(int i = 0; i < MPN_MAX_NUM; i++){  // 开始测试
           // g_work_Test_type[bootle_no-1] = TPC_COLI_ID;
            gst_Test_Info[i + TPCCOLI_START_NUM].Test_st = TEST_ST_ZERO;  //将较零状态赋值于测试状态  后台运行的线程test将先进行较零 再开始进行测试
            gst_Test_Info[i + TPCCOLI_START_NUM].BottleId = i+1;
            memset(&(gst_His_Info[i+TPCCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));   //做为初始值
            g_mpn_test_val[i] = 0;
        }
        ui->pb_FEC->setText(STR_TEST_STOP);
        ui->pb_TOL->setText(STR_TEST_START);
        ui->pb_TOL->setDisabled(true);
        ui->pb_FEC->setDisabled(false);

    }
    else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
    {
        switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
                                     QMessageBox::Ok|QMessageBox::Cancel,
                                     QMessageBox::Cancel))
        {
            case QMessageBox::Ok:
                g_mpn_test_st = MPN_TEST_IDLE;
                for(int i = 0; i < MPN_MAX_NUM; i++){  // 开始测试
                   // g_work_Test_type[bootle_no-1] = TPC_COLI_ID;
                    gst_Test_Info[i + TPCCOLI_START_NUM].Test_st = TEST_ST_IDEL;
                   // gst_Test_Info[i + TPCCOLI_START_NUM].BottleId = 0;
                   // memset(&(gst_His_Info[i+TPCCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
                }
                ui->pb_TOL->setText(STR_TEST_START);
                ui->pb_FEC->setText(STR_TEST_START);
                ui->pb_TOL->setDisabled(false);
                ui->pb_FEC->setDisabled(false);
                g_mpn_test_index=0;
                ui->comboBox->setCurrentIndex(g_mpn_test_index);
                break;

            case QMessageBox::Cancel:
                break;

            default:
                break;
        }
   }

}
//开始测试 总大肠
void mpnTesting::on_pb_TOL_released()
{
    QString txt;

    txt = ui->pb_TOL->text();
    if(operator ==(txt,QString(STR_TEST_START)))  //开始测试
    {
        g_mpn_test_st = MPN_TEST_TOL;
        for(int i = 0; i < MPN_MAX_NUM; i++){  // 开始测试
           // g_work_Test_type[bootle_no-1] = TPC_COLI_ID;
            gst_Test_Info[i + TPCCOLI_START_NUM].Test_st = TEST_ST_ZERO;
            gst_Test_Info[i + TPCCOLI_START_NUM].BottleId = i+1;
            memset(&(gst_His_Info[i+TPCCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
            g_mpn_test_val[i] = 0;
        }
        ui->pb_TOL->setText(STR_TEST_STOP);
        ui->pb_FEC->setText(STR_TEST_START);
        ui->pb_TOL->setDisabled(false);
        ui->pb_FEC->setDisabled(true);

    }
    else if(operator ==(txt, QString(STR_TEST_STOP)))// 停止测试
    {
        switch(QMessageBox::question(this,tr(STR_PROMPT),tr(STR_STOP_TESTING),
                                     QMessageBox::Ok|QMessageBox::Cancel,
                                     QMessageBox::Cancel))
        {
            case QMessageBox::Ok:
                g_mpn_test_st = MPN_TEST_IDLE;
                for(int i = 0; i < MPN_MAX_NUM; i++){  // 开始测试
                   // g_work_Test_type[bootle_no-1] = TPC_COLI_ID;
                    gst_Test_Info[i + TPCCOLI_START_NUM].Test_st = TEST_ST_IDEL;
                   // gst_Test_Info[i + TPCCOLI_START_NUM].BottleId = 0;
                   // memset(&(gst_His_Info[i+TPCCOLI_START_NUM]), 0, sizeof(ST_HISTORY_DATA));
                }
                ui->pb_TOL->setText(STR_TEST_START);
                ui->pb_FEC->setText(STR_TEST_START);
                ui->pb_TOL->setDisabled(false);
                ui->pb_FEC->setDisabled(false);
                g_mpn_test_index=0;
                ui->comboBox->setCurrentIndex(g_mpn_test_index);
                break;

            case QMessageBox::Cancel:
                break;

            default:
                break;
        }
    }

}
// 返回
void mpnTesting::on_pb_back_released()
{
    this->close();
}
//获取 改变的ID  获取稀释倍数
void mpnTesting::on_comboBox_currentIndexChanged(int index)
{
  //  ui->comboBox->setCurrentIndex(index);
    g_mpn_test_index=index;
    g_mpn_test_mui = mul[index];
}

