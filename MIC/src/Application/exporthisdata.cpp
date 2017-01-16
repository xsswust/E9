#include "exporthisdata.h"
#include "ui_exporthisdata.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"

ExportHisData::ExportHisData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExportHisData)
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
    ui->lbTitle->setFont(QFont(FONT_USE_WQY,20,QFont::Bold));
    ui->lbIniDate->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbOverDate->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_5->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_6->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_7->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_8->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_9->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_10->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_11->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbStartYear_12->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leOverYear->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leOverMonth->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leOverDay->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leOverHour->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leOverMinute->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leOverSecond->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leStartYear->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leStartMonth->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leStartDay->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leStartMinute->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leStartHour->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leStartSecond->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
}

ExportHisData::~ExportHisData()
{
    delete ui;
}

void ExportHisData::on_pbExit_clicked()
{
    this->close();
}
