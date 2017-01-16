#include "confirmstop.h"
#include "ui_confirmstop.h"
#include "src/HardWare/com_trandata.h"
#include "testingdis.h"

ConfirmStop::ConfirmStop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmStop)
{
    ui->setupUi(this);
}

ConfirmStop::~ConfirmStop()
{
    delete ui;
}

/*
    函数名称：
    函数功能：停止
    输入参数：
    输出参数：
    修改记录：
*/
void ConfirmStop::on_pushButton_clicked()
{

    this->close();
}

void ConfirmStop::on_pushButton_2_clicked()
{
    this->close();
}
