#include "paratable.h"
#include "ui_paratable.h"
#include <QTextCodec>
#include <QPalette>
#include <QFont>
#include <QSettings>
#include <QFile>
#include "common.h"


ParaTable *ParaTable;
ParaTable::ParaTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParaTable)
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
    //Para Reading
#if 0
	QFile fconfiini(FILE_CON_PARA_SETTING);
    if (!fconfiini.exists())
    {
        WriteMachineParaIniPara();   // 预先写
    }
#endif
	readCurrentCalirationParameter();
    //Font Setting
    ui->pushButton->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lb1->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lb2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lb3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->lb4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_6->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_7->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_8->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_9->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_10->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_11->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_12->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_13->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_14->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_15->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_16->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_17->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol1A->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol1A_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol1A_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol1B->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol1B_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol1B_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol2A->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol2A_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol2A_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol2B->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol2B_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol2B_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol3A->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol3A_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol3A_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol3B->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol3B_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol3B_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol4A->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol4A_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol4A_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol4B->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol4B_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leTol4B_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
}

ParaTable::~ParaTable()
{
    delete ui;
}

void ParaTable::on_pushButton_clicked()
{
    this->close();
}



/*
    函数名称：
    函数功能：读取校准参数
    输入参数：
    输出参数：
    修改记录：
*/
void ParaTable::readCurrentCalirationParameter()
{
#if 0
	QSettings settings(FILE_CON_PARA_SETTING, QSettings::IniFormat);///
	settings.beginGroup(STR_PARASETTING);

	ui->leTol1A->setText(settings.value(STR_TOLCOIL_CLASS1_A).toString());
	ui->leTol1B->setText(settings.value(STR_TOLCOIL_CLASS1_B).toString());
	ui->leTol2A->setText(settings.value(STR_TOLCOIL_CLASS2_A).toString());
	ui->leTol2B->setText(settings.value(STR_TOLCOIL_CLASS2_B).toString());
	ui->leTol3A->setText(settings.value(STR_TOLCOIL_CLASS3_A).toString());
	ui->leTol3B->setText(settings.value(STR_TOLCOIL_CLASS3_B).toString());
	ui->leTol4A->setText(settings.value(STR_TOLCOIL_CLASS4_A).toString());
	ui->leTol4B->setText(settings.value(STR_TOLCOIL_CLASS4_B).toString());

	ui->leTol1A_2->setText(settings.value(STR_FECCOIL_CLASS1_A).toString());
	ui->leTol1B_2->setText(settings.value(STR_FECCOIL_CLASS1_B).toString());
	ui->leTol2A_2->setText(settings.value(STR_FECCOIL_CLASS2_A).toString());
	ui->leTol2B_2->setText(settings.value(STR_FECCOIL_CLASS2_B).toString());
	ui->leTol3A_2->setText(settings.value(STR_FECCOIL_CLASS3_A).toString());
	ui->leTol3B_2->setText(settings.value(STR_FECCOIL_CLASS3_B).toString());
	ui->leTol4A_2->setText(settings.value(STR_FECCOIL_CLASS4_A).toString());
	ui->leTol4B_2->setText(settings.value(STR_FECCOIL_CLASS4_B).toString());

	ui->leTol1A_3->setText(settings.value(STR_TPCCOIL_CLASS1_A).toString());
	ui->leTol1B_3->setText(settings.value(STR_TPCCOIL_CLASS1_B).toString());
	ui->leTol2A_3->setText(settings.value(STR_TPCCOIL_CLASS2_A).toString());
	ui->leTol2B_3->setText(settings.value(STR_TPCCOIL_CLASS2_B).toString());
	ui->leTol3A_3->setText(settings.value(STR_TPCCOIL_CLASS3_A).toString());
	ui->leTol3B_3->setText(settings.value(STR_TPCCOIL_CLASS3_B).toString());
	ui->leTol4A_3->setText(settings.value(STR_TPCCOIL_CLASS4_A).toString());
	ui->leTol4B_3->setText(settings.value(STR_TPCCOIL_CLASS4_B).toString());

    settings.endGroup();
#endif
}
