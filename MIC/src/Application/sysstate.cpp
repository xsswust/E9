#include "sysstate.h"
#include "ui_sysstate.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"

SysState::SysState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysState)
{
    ui->setupUi(this);
    //Backgroud Setting
	QPixmap pixmap(PIC_BACKGROUND_PNG);
    QPalette palette;
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);
    //Chinese Display
    QTextCodec *gbk = QTextCodec::codecForName("utf-8");   // Chinese Display
//    QTextCodec::setCodecForTr(gbk);
	QTextCodec::setCodecForLocale(gbk);
//    QTextCodec::setCodecForCStrings(gbk);
    //Font Setting
    ui->lbSystate->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR1->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR5->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR6->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR7->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR8->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR9->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR10->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR11->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR12->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR13->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR14->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR15->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR16->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR17->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR18->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR19->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR20->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR21->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR22->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR23->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR24->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR25->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR26->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR27->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR28->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR29->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR30->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR31->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR32->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR33->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR34->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR35->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbR36->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_1->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_5->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_6->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_7->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_8->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_9->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_10->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_11->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_12->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_13->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_14->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_15->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_16->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_17->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_18->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_19->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_20->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_21->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_22->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_23->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_24->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_25->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_26->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_27->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_28->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_29->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_30->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_31->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_32->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_33->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_34->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_35->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lineEdit_36->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

}

SysState::~SysState()
{
    delete ui;
}

void SysState::on_pbExit_clicked()
{
    this->close();
}
