#include "testmodeselect.h"
#include "ui_testmodeselect.h"
#include "common.h"
#include "testmode.h"
#include "mainwindow.h"
#include "mpntesting.h"

testModeSelect::testModeSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testModeSelect)
{
    ui->setupUi(this);

    ui->setupUi(this);
    //Backgroud Setting
    QPixmap pixmap(PIC_BACKGROUND2_PNG);
    QPalette palette;

    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

    qDebug()<<"testModeSelect    ====   ";
    connect(ui->pb_Fast,SIGNAL(released()),this,SLOT(on_pb_Fast_released()));
    connect(ui->pb_Mpn,SIGNAL(released()),this,SLOT(on_pb_Mpn_released()));
    connect(ui->pb_back,SIGNAL(released()),this,SLOT(on_pb_back_released()));
}

testModeSelect::~testModeSelect()
{
    delete ui;
}
// MPN方法
void testModeSelect::on_pb_Mpn_released()
{
    qDebug()<<"on_pb_Mpn_released =============";
    mpnTesting *pmpnTesting = new mpnTesting();
    pmpnTesting->showFullScreen();

}
// 快速方法
void testModeSelect::on_pb_Fast_released()
{
    qDebug()<<"on_pb_Fast_released =============";
    testmode *tmode = new testmode();
    tmode->showFullScreen();
}
// 界面跳出
void testModeSelect::on_pb_back_released()
{
    qDebug()<<"on_pb_back_released =============";
    this->close();
    pmainwin->MainWindow_Init(); // 恢复状态
}
