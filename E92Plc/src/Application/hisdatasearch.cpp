#include "hisdatasearch.h"
#include "ui_hisdatasearch.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"

HisDataSearch::HisDataSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HisDataSearch)
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
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbSearch->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbDataTime->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbYear->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbMonth->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbDay->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbHour->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbMinute->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lbSecond->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leYear->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leMonth->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leDay->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leHour->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leMinute->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leSecond->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
}

HisDataSearch::~HisDataSearch()
{
    delete ui;
}

void HisDataSearch::on_pbExit_clicked()
{
    this->close();
}
