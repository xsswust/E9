#include "paracalibrate.h"
#include "ui_paracalibrate.h"
#include "NumberKeyBoard.h"
#include <QTextCodec>
#include <QFont>
#include <QPalette>
#include <QSettings>
#include <math.h>
//#include "common.h"


ParaCalibrate::ParaCalibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParaCalibrate)
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
    //button
    ui->pbCalibrate->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->pbExit->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

//	ui->pb_calc->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    // label
    ui->label->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_2->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_3->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_4->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_5->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_6->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_7->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_8->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_9->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_10->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->label_11->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
//	ui->lb_time->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
//	ui->lb_tnongdu->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));

    // line edit
    //ui->leParaSerial->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->le_XieLv->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->leCount->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->le_JieJu->setFont(QFont(FONT_USE_WQY,12,QFont::Normal));
    ui->le_NongDu1->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_NongDu2->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_NongDu3->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_NongDu4->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_TestTime1->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_TestTime2->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_TestTime3->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
    ui->le_TestTime4->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));

//	ui->le_nongdu->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));
//	ui->le_time->setFont(QFont(FONT_USE_WQY,8,QFont::Normal));

}
// 析构函数
ParaCalibrate::~ParaCalibrate()
{
    delete ui;
}
/*
    函数名称：
    函数功能：界面初始化
    输入参数：
    输出参数：
    修改记录：
*/
void ParaCalibrate::ParaCalibrate_Init()
{
    exit_flag = false;
    //Keyboard Setting
    im = new Keyboard_dependence;
    ui->le_NongDu1->setFocusPolicy (Qt::ClickFocus);
    ui->le_NongDu1->installEventFilter (im);

    ui->le_NongDu2->setFocusPolicy (Qt::ClickFocus);
    ui->le_NongDu2->installEventFilter (im);

    ui->le_NongDu3->setFocusPolicy (Qt::ClickFocus);
    ui->le_NongDu3->installEventFilter (im);

    ui->le_NongDu4->setFocusPolicy (Qt::ClickFocus);
    ui->le_NongDu4->installEventFilter (im);

    ui->le_TestTime1->setFocusPolicy (Qt::ClickFocus);
    ui->le_TestTime1->installEventFilter (im);

    ui->le_TestTime2->setFocusPolicy (Qt::ClickFocus);
    ui->le_TestTime2->installEventFilter (im);

    ui->le_TestTime3->setFocusPolicy (Qt::ClickFocus);
    ui->le_TestTime3->installEventFilter (im);

    ui->le_TestTime4->setFocusPolicy (Qt::ClickFocus);
    ui->le_TestTime4->installEventFilter (im);

    ui->leCount->setFocusPolicy(Qt::ClickFocus);
    ui->leCount->installEventFilter(im);

//	ui->le_time->setFocusPolicy(Qt::ClickFocus);
//	ui->le_time->installEventFilter(im);

    connect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
    connect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
}
/*
    函数名称：
    函数功能：界面跳出 处理
    输入参数：
    输出参数：
    修改记录：
*/
void ParaCalibrate::ParaCalibrate_UnInit()
{

    ui->le_NongDu1->removeEventFilter(im);
    ui->le_NongDu2->removeEventFilter(im);
    ui->le_NongDu3->removeEventFilter(im);
    ui->le_NongDu4->removeEventFilter(im);
    ui->le_TestTime1->removeEventFilter(im);
    ui->le_TestTime2->removeEventFilter(im);
    ui->le_TestTime3->removeEventFilter(im);
    ui->le_TestTime4->removeEventFilter(im);

    disconnect(im->v_key,SIGNAL(emitNustr(QString)),this,SLOT(insertValue(QString)));
    disconnect(im->v_key,SIGNAL(del()),this,SLOT(keyboardDelete()));
    delete im;  // 删除
}

/*
    函数名称：
    函数功能：界面跳出函数
    输入参数：
    输出参数：
    修改记录：
*/
void ParaCalibrate::on_pbExit_clicked()
{
//	getFullScree("paracalibratr.jpg");
    if(false == exit_flag)
    {
        exit_flag = true;
        ParaCalibrate_UnInit();
        this->close();
    }
}
// 计算斜率和截距
void ParaCalibrate::on_pbCalibrate_clicked()
{
    float X[MAX_COUNT+1];
    float Y[MAX_COUNT+1];
    float a= 0.0, b = 0.0;

    int count = ui->leCount->text().toInt();  // 点个数

    if(count > MAX_COUNT)
    {
        QMessageBox::warning(
            this,
            tr(STR_PROMPT),
            tr(STR_PARAM_ERROR) );

        return;
        count = MAX_COUNT;
    }
    else if(count <= 1) // 无法计算
    {
        QMessageBox::warning(
            this,
            tr(STR_PROMPT),
            tr(STR_PARAM_ERROR) );
        return;
    }
    // 算法 浓度取 log10 在赋值计算
    if(2 == count)
    {
        X[0] = ui->le_TestTime1->text().toFloat();
        X[1] = ui->le_TestTime2->text().toFloat();

        Y[0] = -log10(ui->le_NongDu1->text().toFloat());
        Y[1] = -log10(ui->le_NongDu2->text().toFloat());
    }
    else if(3 == count)
    {
        X[0] = ui->le_TestTime1->text().toFloat();
        X[1] = ui->le_TestTime2->text().toFloat();
        X[2] = ui->le_TestTime3->text().toFloat();

        Y[0] = -log10(ui->le_NongDu1->text().toFloat());
        Y[1] = -log10(ui->le_NongDu2->text().toFloat());
        Y[2] = -log10(ui->le_NongDu3->text().toFloat());
    }
    else if(4 == count)
    {
        X[0] = ui->le_TestTime1->text().toFloat();
        X[1] = ui->le_TestTime2->text().toFloat();
        X[2] = ui->le_TestTime3->text().toFloat();
        X[3] = ui->le_TestTime4->text().toFloat();

        Y[0] = -log10(ui->le_NongDu1->text().toFloat());
        Y[1] = -log10(ui->le_NongDu2->text().toFloat());
        Y[2] = -log10(ui->le_NongDu3->text().toFloat());
        Y[3] = -log10(ui->le_NongDu4->text().toFloat());
    }

    Line_Fit(X, Y, count, &a, &b);

    ui->le_XieLv->setText(QString("%1").arg(a));

    ui->le_JieJu->setText(QString("%1").arg(b));

    XieLv=qAbs(a); //输入都为正值
    JieJu=qAbs(b);
    qDebug()<<"XieLv="<<XieLv;
    qDebug()<<"JieJu="<<JieJu;
    if(isnan(XieLv) || isnan(JieJu) || isinf(XieLv) || isinf(JieJu) ) //如果为nan返回非零值 当x是正无穷是返回1，当x是负无穷时返回-1
    {
        QMessageBox::warning(
            this,
            tr(STR_PROMPT),
            "参数错误" );
    }
   int num = ui->comboBox->currentIndex();
    qDebug()<<"num="<<num;
    qDebug()<<"修正后的曲线***************";
    qDebug()<<"校正前  TOLParaCalibratedFlag="<<TOLParaCalibratedFlag;
    qDebug()<<"校正前  FECParaCalibratedFlag="<<FECParaCalibratedFlag;
    qDebug()<<"校正前  TPCParaCalibratedFlag="<<TPCParaCalibratedFlag;
    qDebug()<<"校正前  EParaCalibratedFlag="<<EParaCalibratedFlag;
    if(isnan(XieLv)==0 && isnan(JieJu)==0 && isinf(XieLv)==0 && isinf(JieJu)==0 && XieLv!=0 && JieJu!=0)
 {   if(num==0)  //参数输入总大肠
    {
        ST_PARA_SETTING temp_set;

        temp_set.para_A = XieLv;
        temp_set.para_B = JieJu;
        temp_set.para_C = 1;
        temp_set.para_D = 0;
        temp_set.para_Q = 1;
        temp_set.para_R = 1;
        temp_set.Unit = 0;

        printf("@@ temp_set.para_A = %f temp_set.para_B= %f, temp_set.para_C= %f\r\n", temp_set.para_A, temp_set.para_B, temp_set.para_C);
        printf("@@ temp_set.para_D = %f temp_set.para_Q = %f, temp_set.para_R= %f\r\n", temp_set.para_D, temp_set.para_Q, temp_set.para_R);

        QFile file(FILE_QX_PARA_SETTING);  //
        file.open(QIODevice::ReadWrite);  // 打开文件 只读
        file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_TOL_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9) = %d  \r\n", PRAR_TOL_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.close();
        TOLParaCalibratedFlag=true;
        qDebug()<<"校正后  TOLParaCalibratedFlag="<<TOLParaCalibratedFlag;

    }
   else if(num==1)  //参数输入耐热大肠
    {
        ST_PARA_SETTING temp_set;

        temp_set.para_A = XieLv;
        temp_set.para_B = JieJu;
        temp_set.para_C = 1;
        temp_set.para_D = 0;
        temp_set.para_Q = 1;
        temp_set.para_R = 1;
        temp_set.Unit = 0;

        printf("@@ temp_set.para_A = %f temp_set.para_B= %f, temp_set.para_C= %f\r\n", temp_set.para_A, temp_set.para_B, temp_set.para_C);
        printf("@@ temp_set.para_D = %f temp_set.para_Q = %f, temp_set.para_R= %f\r\n", temp_set.para_D, temp_set.para_Q, temp_set.para_R);

        QFile file(FILE_QX_PARA_SETTING);  //
        file.open(QIODevice::ReadWrite);  // 打开文件 只读
        file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_FEC_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9) = %d  \r\n", PRAR_FEC_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.close();
        FECParaCalibratedFlag=true;
        qDebug()<<"校正后  FECParaCalibratedFlag="<<FECParaCalibratedFlag;

    }
   else if(num==2)  //参数输入细菌总数
    {
        ST_PARA_SETTING temp_set;

        temp_set.para_A = XieLv;
        temp_set.para_B = JieJu;
        temp_set.para_C = 1;
        temp_set.para_D = 0;
        temp_set.para_Q = 1;
        temp_set.para_R = 1;
        temp_set.Unit = 2;

        printf("@@ temp_set.para_A = %f temp_set.para_B= %f, temp_set.para_C= %f\r\n", temp_set.para_A, temp_set.para_B, temp_set.para_C);
        printf("@@ temp_set.para_D = %f temp_set.para_Q = %f, temp_set.para_R= %f\r\n", temp_set.para_D, temp_set.para_Q, temp_set.para_R);

        QFile file(FILE_QX_PARA_SETTING);  //
        file.open(QIODevice::ReadWrite);  // 打开文件 只读
        file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_TPC_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9) = %d  \r\n", PRAR_TPC_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.close();
        TPCParaCalibratedFlag=true;
        qDebug()<<"校正后  TPCParaCalibratedFlag="<<TPCParaCalibratedFlag;

    }
   else if(num==3)  //参数输入埃希氏
    {
        ST_PARA_SETTING temp_set;

        temp_set.para_A = XieLv;
        temp_set.para_B = JieJu;
        temp_set.para_C = 1;
        temp_set.para_D = 0;
        temp_set.para_Q = 1;
        temp_set.para_R = 1;
        temp_set.Unit = 0;

        printf("@@ temp_set.para_A = %f temp_set.para_B= %f, temp_set.para_C= %f\r\n", temp_set.para_A, temp_set.para_B, temp_set.para_C);
        printf("@@ temp_set.para_D = %f temp_set.para_Q = %f, temp_set.para_R= %f\r\n", temp_set.para_D, temp_set.para_Q, temp_set.para_R);

        QFile file(FILE_QX_PARA_SETTING);  //
        file.open(QIODevice::ReadWrite);  // 打开文件 只读
        file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.write((char*)&temp_set, PRAR_ST_DATA_LEN);
        printf("PRAR_E_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9) = %d  \r\n", PRAR_E_START + PRAR_ST_DATA_LEN*(9*PRAR_MAX_CLASS_NUM + 9));
        file.close();
        EParaCalibratedFlag=true;
        qDebug()<<"校正后  EParaCalibratedFlag="<<EParaCalibratedFlag;
    }
    else    //未选择菌种
    {
        QMessageBox::warning(
            this,
            tr(STR_PROMPT),
            "未选择菌种" );
           }
   }

}
/*
    函数名称：
    函数功能：键值 处理
    输入参数：
    输出参数：
    修改记录：
*/
void ParaCalibrate::insertValue(QString nustr)
{
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->clear();
    p_editer->setText(nustr);

}
/*
    函数名称：
    函数功能：键值 删除
    输入参数：
    输出参数：
    修改记录：
*/
void ParaCalibrate::keyboardDelete()
{
    p_editer = qobject_cast<QLineEdit*>(focusWidget());
    p_editer->backspace();
}

#if 0
/*
    函数名称：
    函数功能：修改配置参数
    输入参数：
    输出参数：
    修改记录：
*/
void ParaCalibrate::on_le_NongDu1_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("D1",arg1);
    settings.endGroup();
}

void ParaCalibrate::on_le_NongDu2_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("D2",arg1);
    settings.endGroup();
}

void ParaCalibrate::on_le_NongDu3_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("D3",arg1);
    settings.endGroup();
}

void ParaCalibrate::on_le_NongDu4_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("D4",arg1);
    settings.endGroup();
}


void ParaCalibrate::on_le_TestTime1_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("Q1",arg1);
    settings.endGroup();
}

void ParaCalibrate::on_le_TestTime2_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("Q2",arg1);
    settings.endGroup();
}

void ParaCalibrate::on_le_TestTime3_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("Q3",arg1);
    settings.endGroup();
}

void ParaCalibrate::on_le_TestTime4_textChanged(const QString &arg1)
{
    QSettings settings(FILE_CON_PARA_CALIBRATE, QSettings::IniFormat);//写所需配置参数
    settings.beginGroup("Para Calibrate");
    settings.setValue("Q4",arg1);
    settings.endGroup();
}
#endif



/*
//计算浓度

void ParaCalibrate::on_pb_calc_clicked()
{
    int num = 0, len = 0;
    int threshold_time = 0;
    double A = 0.0, B = 0.0, yyy = 0.0 ,zzz = 0.0;

    ST_PARA_SETTING temp;

    num = ui->comboBox->currentIndex();

    qDebug()<<QString(" num == %1\r\n").arg(num);
    memset(&temp, 0 , sizeof(temp));
    Get_Param(num +1, &temp);
    qDebug()<<QString(" temp.para_A == %1 temp.para_B = %2\r\n").arg(temp.para_A).arg(temp.para_B);
#if 0
    if(temp.para_A != 0.0)  // 根据参数计算出 时间
    {
        threshold_time = (long)((temp.para_B - 1.00)/temp.para_A);
        qDebug()<<QString(" threshold_time == %1 \r\n").arg(threshold_time);
    }

    if(g_Test_Over_Time > threshold_time) // 最大时间 大于 计算时间
    {
        A = 1.00/(g_Test_Over_Time - threshold_time);
        B = A * g_Test_Over_Time;
    }
#else
    A = temp.para_A;
    B = temp.para_B;
#endif
    qDebug()<<QString(" A == %1  b = %2\r\n").arg(A).arg(B);
    QString str_len = ui->le_time->text();
    len = str_len.toInt();
    yyy = A * len - B;//公式可以改变
    zzz = exp(-1*log(10)*yyy);//计数浓度
    qDebug()<<QString(" zzz == %1  yyy = %2\r\n").arg(zzz).arg(yyy);
    qDebug()<<QString(" log == %1  len = %2\r\n").arg(log(10)).arg(len);

    ui->le_nongdu->setText(QString::number(zzz));
}
*/

//mode -->那种菌
void ParaCalibrate::Get_Param(int mode, ST_PARA_SETTING *st)
{

    //ST_PARA_SETTING temp_set;
    int TargetNo = 0, ClassNo = 0, ClassNo_1 = 0;
    QFile file(FILE_QX_PARA_SETTING);  //

    file.open(QIODevice::ReadOnly);  // 打开文件 只读

    file.seek(PRAR_TARGET_START);
    file.read((char*)&TargetNo, PRAR_DATA_LEN);
    file.seek(PRAR_CLASS_START);
    file.read((char*)&ClassNo, PRAR_DATA_LEN);
    file.seek(PRAR_CLASS1_START);
    file.read((char*)&ClassNo_1, PRAR_DATA_LEN);

    printf("TargetNo = %d ClassNo= %d, ClassNo_1= %d\r\n", TargetNo, ClassNo, ClassNo_1);

    if(TOL_COLI_ID == mode)  // 总大肠
    {
        file.seek(PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
        memset(st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)st, PRAR_ST_DATA_LEN);
//		printf("PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1) = %d  \r\n", PRAR_TOL_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
    }
    else if(FEC_COLI_ID == mode)  // 耐热大肠
    {
        file.seek(PRAR_FEC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
//                file.seek(PRAR_FEC_START+PRAR_ST_DATA_LEN*ClassNo);
        memset(st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)&st, PRAR_ST_DATA_LEN);
    }
    else if(TPC_COLI_ID == mode)  // 菌落总数
    {
        file.seek(PRAR_TPC_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
        //file.seek(PRAR_TPC_START+PRAR_ST_DATA_LEN*ClassNo);
        memset(&st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)&st, PRAR_ST_DATA_LEN);
    }
    else if(E_COLI_ID == mode)  // 埃希氏
    {
        file.seek(PRAR_E_START + PRAR_ST_DATA_LEN*(ClassNo*PRAR_MAX_CLASS_NUM + ClassNo_1));
       // file.seek(PRAR_E_START+PRAR_ST_DATA_LEN*ClassNo);
        memset(&st, 0,PRAR_ST_DATA_LEN);
        file.read((char*)&st, PRAR_ST_DATA_LEN);
    }
    file.close();

#if 0
    int class_no;

    QSettings settings(FILE_CON_PARA_SETTING, QSettings::IniFormat);
    settings.beginGroup(STR_PARASETTING);
    if(TOL_COLI_ID == mode)
    {
        class_no = settings.value(STR_TOl_CLASS).toInt();
        st->Unit = settings.value(STR_TOl_UNIT).toInt();
        if(COIL_CLASS_NO1 == class_no)
        {
            st->para_A = settings.value(STR_TOLCOIL_CLASS1_A).toDouble();
            st->para_B = settings.value(STR_TOLCOIL_CLASS1_B).toDouble();
            st->para_C = settings.value(STR_TOLCOIL_CLASS1_C).toDouble();
            st->para_D = settings.value(STR_TOLCOIL_CLASS1_D).toDouble();
            st->para_R = settings.value(STR_TOLCOIL_CLASS1_R).toDouble();
            st->para_Q = settings.value(STR_TOLCOIL_CLASS1_Q).toDouble();
        }
        else if(COIL_CLASS_NO2 == class_no)
        {
            st->para_A = settings.value(STR_TOLCOIL_CLASS2_A).toDouble();
            st->para_B = settings.value(STR_TOLCOIL_CLASS2_B).toDouble();
            st->para_C = settings.value(STR_TOLCOIL_CLASS2_C).toDouble();
            st->para_D = settings.value(STR_TOLCOIL_CLASS2_D).toDouble();
            st->para_R = settings.value(STR_TOLCOIL_CLASS2_R).toDouble();
            st->para_Q = settings.value(STR_TOLCOIL_CLASS2_Q).toDouble();
        }
        else if(COIL_CLASS_NO3 == class_no)
        {
            st->para_A = settings.value(STR_TOLCOIL_CLASS3_A).toDouble();
            st->para_B = settings.value(STR_TOLCOIL_CLASS3_B).toDouble();
            st->para_C = settings.value(STR_TOLCOIL_CLASS3_C).toDouble();
            st->para_D = settings.value(STR_TOLCOIL_CLASS3_D).toDouble();
            st->para_R = settings.value(STR_TOLCOIL_CLASS3_R).toDouble();
            st->para_Q = settings.value(STR_TOLCOIL_CLASS3_Q).toDouble();
        }
        else if(COIL_CLASS_NO4 == class_no)
        {
            st->para_A = settings.value(STR_TOLCOIL_CLASS4_A).toDouble();
            st->para_B = settings.value(STR_TOLCOIL_CLASS4_B).toDouble();
            st->para_C = settings.value(STR_TOLCOIL_CLASS4_C).toDouble();
            st->para_D = settings.value(STR_TOLCOIL_CLASS4_D).toDouble();
            st->para_R = settings.value(STR_TOLCOIL_CLASS4_R).toDouble();
            st->para_Q = settings.value(STR_TOLCOIL_CLASS4_Q).toDouble();
        }
    }
    else if(FEC_COLI_ID == mode)
    {
        class_no = settings.value(STR_FEC_CLASS).toInt();
        st->Unit = settings.value(STR_FEC_UNIT).toInt();
        if(COIL_CLASS_NO1 == class_no)
        {
            st->para_A = settings.value(STR_FECCOIL_CLASS1_A).toDouble();
            st->para_B = settings.value(STR_FECCOIL_CLASS1_B).toDouble();
            st->para_C = settings.value(STR_FECCOIL_CLASS1_C).toDouble();
            st->para_D = settings.value(STR_FECCOIL_CLASS1_D).toDouble();
            st->para_R = settings.value(STR_FECCOIL_CLASS1_R).toDouble();
            st->para_Q = settings.value(STR_FECCOIL_CLASS1_Q).toDouble();
        }
        else if(COIL_CLASS_NO2 == class_no)
        {
            st->para_A = settings.value(STR_FECCOIL_CLASS2_A).toDouble();
            st->para_B = settings.value(STR_FECCOIL_CLASS2_B).toDouble();
            st->para_C = settings.value(STR_FECCOIL_CLASS2_C).toDouble();
            st->para_D = settings.value(STR_FECCOIL_CLASS2_D).toDouble();
            st->para_R = settings.value(STR_FECCOIL_CLASS2_R).toDouble();
            st->para_Q = settings.value(STR_FECCOIL_CLASS2_Q).toDouble();
        }
        else if(COIL_CLASS_NO3 == class_no)
        {
            st->para_A = settings.value(STR_FECCOIL_CLASS3_A).toDouble();
            st->para_B = settings.value(STR_FECCOIL_CLASS3_B).toDouble();
            st->para_C = settings.value(STR_FECCOIL_CLASS3_C).toDouble();
            st->para_D = settings.value(STR_FECCOIL_CLASS3_D).toDouble();
            st->para_R = settings.value(STR_FECCOIL_CLASS3_R).toDouble();
            st->para_Q = settings.value(STR_FECCOIL_CLASS3_Q).toDouble();
        }
        else if(COIL_CLASS_NO4 == class_no)
        {
            st->para_A = settings.value(STR_FECCOIL_CLASS4_A).toDouble();
            st->para_B = settings.value(STR_FECCOIL_CLASS4_B).toDouble();
            st->para_C = settings.value(STR_FECCOIL_CLASS4_C).toDouble();
            st->para_D = settings.value(STR_FECCOIL_CLASS4_D).toDouble();
            st->para_R = settings.value(STR_FECCOIL_CLASS4_R).toDouble();
            st->para_Q = settings.value(STR_FECCOIL_CLASS4_Q).toDouble();
        }

    }
    else if(TPC_COLI_ID == mode)
    {
        class_no = settings.value(STR_TPC_CLASS).toInt();
        st->Unit = settings.value(STR_TPC_UNIT).toInt();
        if(COIL_CLASS_NO1 == class_no)
        {
            st->para_A = settings.value(STR_TPCCOIL_CLASS1_A).toDouble();
            st->para_B = settings.value(STR_TPCCOIL_CLASS1_B).toDouble();
            st->para_C = settings.value(STR_TPCCOIL_CLASS1_C).toDouble();
            st->para_D = settings.value(STR_TPCCOIL_CLASS1_D).toDouble();
            st->para_R = settings.value(STR_TPCCOIL_CLASS1_R).toDouble();
            st->para_Q = settings.value(STR_TPCCOIL_CLASS1_Q).toDouble();
        }
        else if(COIL_CLASS_NO2 == class_no)
        {
            st->para_A = settings.value(STR_TPCCOIL_CLASS2_A).toDouble();
            st->para_B = settings.value(STR_TPCCOIL_CLASS2_B).toDouble();
            st->para_C = settings.value(STR_TPCCOIL_CLASS2_C).toDouble();
            st->para_D = settings.value(STR_TPCCOIL_CLASS2_D).toDouble();
            st->para_R = settings.value(STR_TPCCOIL_CLASS2_R).toDouble();
            st->para_Q = settings.value(STR_TPCCOIL_CLASS2_Q).toDouble();
        }
        else if(COIL_CLASS_NO3 == class_no)
        {
            st->para_A = settings.value(STR_TPCCOIL_CLASS3_A).toDouble();
            st->para_B = settings.value(STR_TPCCOIL_CLASS3_B).toDouble();
            st->para_C = settings.value(STR_TPCCOIL_CLASS3_C).toDouble();
            st->para_D = settings.value(STR_TPCCOIL_CLASS3_D).toDouble();
            st->para_R = settings.value(STR_TPCCOIL_CLASS3_R).toDouble();
            st->para_Q = settings.value(STR_TPCCOIL_CLASS3_Q).toDouble();
        }
        else if(COIL_CLASS_NO4 == class_no)
        {
            st->para_A = settings.value(STR_TPCCOIL_CLASS4_A).toDouble();
            st->para_B = settings.value(STR_TPCCOIL_CLASS4_B).toDouble();
            st->para_C = settings.value(STR_TPCCOIL_CLASS4_C).toDouble();
            st->para_D = settings.value(STR_TPCCOIL_CLASS4_D).toDouble();
            st->para_R = settings.value(STR_TPCCOIL_CLASS4_R).toDouble();
            st->para_Q = settings.value(STR_TPCCOIL_CLASS4_Q).toDouble();
        }

    }
    else if(E_COLI_ID == mode)
    {
        class_no = settings.value(STR_E_CLASS).toInt();
        st->Unit = settings.value(STR_E_UNIT).toInt();
        if(COIL_CLASS_NO1 == class_no)
        {
            st->para_A = settings.value(STR_ECOIL_CLASS1_A).toDouble();
            st->para_B = settings.value(STR_ECOIL_CLASS1_B).toDouble();
            st->para_C = settings.value(STR_ECOIL_CLASS1_C).toDouble();
            st->para_D = settings.value(STR_ECOIL_CLASS1_D).toDouble();
            st->para_R = settings.value(STR_ECOIL_CLASS1_R).toDouble();
            st->para_Q = settings.value(STR_ECOIL_CLASS1_Q).toDouble();
        }
        else if(COIL_CLASS_NO2 == class_no)
        {
            st->para_A = settings.value(STR_ECOIL_CLASS2_A).toDouble();
            st->para_B = settings.value(STR_ECOIL_CLASS2_B).toDouble();
            st->para_C = settings.value(STR_ECOIL_CLASS2_C).toDouble();
            st->para_D = settings.value(STR_ECOIL_CLASS2_D).toDouble();
            st->para_R = settings.value(STR_ECOIL_CLASS2_R).toDouble();
            st->para_Q = settings.value(STR_ECOIL_CLASS2_Q).toDouble();
        }
        else if(COIL_CLASS_NO3 == class_no)
        {
            st->para_A = settings.value(STR_ECOIL_CLASS3_A).toDouble();
            st->para_B = settings.value(STR_ECOIL_CLASS3_B).toDouble();
            st->para_C = settings.value(STR_ECOIL_CLASS3_C).toDouble();
            st->para_D = settings.value(STR_ECOIL_CLASS3_D).toDouble();
            st->para_R = settings.value(STR_ECOIL_CLASS3_R).toDouble();
            st->para_Q = settings.value(STR_ECOIL_CLASS3_Q).toDouble();
        }
        else if(COIL_CLASS_NO4 == class_no)
        {
            st->para_A = settings.value(STR_ECOIL_CLASS4_A).toDouble();
            st->para_B = settings.value(STR_ECOIL_CLASS4_B).toDouble();
            st->para_C = settings.value(STR_ECOIL_CLASS4_C).toDouble();
            st->para_D = settings.value(STR_ECOIL_CLASS4_D).toDouble();
            st->para_R = settings.value(STR_ECOIL_CLASS4_R).toDouble();
            st->para_Q = settings.value(STR_ECOIL_CLASS4_Q).toDouble();
        }
    }

    settings.endGroup();
#endif
}
