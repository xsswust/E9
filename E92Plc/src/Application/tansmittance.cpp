#include "tansmittance.h"
#include "ui_tansmittance.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include "common.h"

Tansmittance::Tansmittance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tansmittance)
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
    ui->pushButton->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pushButton_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pushButton_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pushButton_4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pushButton_5->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pushButton_6->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
}

Tansmittance::~Tansmittance()
{
    delete ui;
}

void Tansmittance::on_pushButton_6_clicked()
{
    this->close();
}
