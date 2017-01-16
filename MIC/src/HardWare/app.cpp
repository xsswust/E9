#include "app.h"
#include <QFile>
#include <QString>
#include <stdio.h>
#include <unistd.h>
#include <QDebug>
#include <QDir>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QTextCodec>
#include <time.h>
#include <sys/time.h>
#include <QStringList>
#include <QTextStream>
#include <QByteArray>
#include <stdlib.h>
#include <QtCore/qmath.h>
#include <math.h>
#include "common.h"
#include "sql_control.h"
/*
    函数名称：saveWarningMessage
    函数功能：保存警告记录
    输入参数：FileNameErr要保存的错误文件路径；object警告对象； warningType 警告类型；warningEvent警告事件；currentValue 当前值；marginValue界限值
    输出参数：无
    修改记录：
*/
void saveWarningMessage(QString FileNameErr,QString object,QString warningType,QString warningEvent,QString currentValue,QString marginValue)
{
   // QTextCodec *codec = QTextCodec::codecForName("GBK");

   // QString FileNameErr ="HisDatalist.data";
    QFile file(FileNameErr);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        return;
    QTextStream in(&file);
   // in.setCodec(codec);
    in <<QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")<<","<<object<<","<<warningType<<","<<warningEvent<<","<<currentValue<<","<<marginValue<<";";
    file.close();
}

/*
    函数名称：saveCODWarningMessage
    函数功能：保存警告记录
    输入参数：FileNameErr要保存的错误文件路径；warningEvent警告事件信息
    输出参数：无
    修改记录：
*/
void saveCODWarningMessage(QString FileNameErr,QString warningEvent)
{
   // QTextCodec *codec = QTextCodec::codecForName("GBK");

   // QString FileNameErr ="Errorlist.data";
    QFile file(FileNameErr);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        return;
    QTextStream in(&file);
   // in.setCodec(codec);
    in <<QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")<<","<<warningEvent<<";";
    file.close();
}

/*
    函数名称：showWarningData
    函数功能：显示错误记录
    输入参数：FileNameErr保存错误文件的路径；tableWarningandLog显示错误信息的QTableWidget名
    输出参数：无
    修改记录：
*/
void showWarningMessage(QString FileNameErr,QTableWidget *tableWarningandLog)
{

    /***********显示错误记录文件里的内容***************************/
    int i = 0,j = 0;
    //QString FileNameErr =Errorlist;
    QString tmpData;
    QStringList warningDataList;
    QFile file(FileNameErr);
    if(file.open(QFile::ReadOnly))//打开文件
    {
        QTextStream out(&file);
        tmpData = out.readAll();
        warningDataList = tmpData.split(";");//以分号作为分割符，将数据分为若干字符串
        tableWarningandLog->setRowCount(warningDataList.count());//设置列数
        for(i  = warningDataList.count();i>0;i--)
        {
            if(warningDataList.value(i) == "")//去掉空格行
                continue;

            QString warningString = warningDataList.value(i);
            QStringList messList = warningString.split(",");//以逗号为分隔符，将字符串分隔成具体的报警信息
            for(j = 0;j<messList.count();++j)
            {
                QTableWidgetItem *newItem = new QTableWidgetItem(messList.value(j));
                tableWarningandLog->setItem(i, j, newItem);//将每个字否串打印到相应的表格
            }
        }

    }
    else
    {
        qDebug()<<"open warning message failed";
    }
    file.close();//关闭文件

    /**********************************************************/
}




/*
    函数名称：copyFileToPath
    函数功能：复制文件
    输入参数：sourceDir 源文件路径；toDir 目标文件路径；coverFileIfExist 文件同名是否覆盖，true覆盖，false返回false
    输出参数：成功true，失败false
    修改记录：
*/
bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    qDebug()<<"copyFileToPath sourceDir ="+sourceDir+"    toDir ="+toDir;
    toDir.replace("\\","/");//将“\\”转换为“/”

    if (sourceDir == toDir)
    {
         qDebug()<<"11111";
        return true;
    }

    if (!QFile::exists(sourceDir)) //
    {
         qDebug()<<"22222";
        return false;
    }

    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist)
    {
        qDebug()<<"5555";
        if(coverFileIfExist)
        {
            createfile->remove(toDir);
            qDebug()<<"7777";
        }
        else{
             qDebug()<<"66666";
            return false;
        }
    }

    /*进行文件copy*/
    if(!QFile::copy(sourceDir, toDir))
    {
         qDebug()<<"44444";
        return false;
    }
    qDebug()<<"before sync";
    //system("sync");//同步一下
    sync();
    return true;
}

/*
    函数名称：copyDirectoryFiles
    函数功能：复制文件
    输入参数：fromDir 源文件夹路径；toDir 目标文件夹路径；coverFileIfExist 文件同名是否覆盖，true覆盖，false返回false
    输出参数：成功true，失败false
    修改记录：
*/
bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{

    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    qDebug()<<"fromDir ="+fromDir+"    toDir ="+toDir;
    if(!targetDir.exists())
    {    /* 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
           {
            qDebug()<<"1111-1";
            return false;
            }
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {    qDebug()<<"fileName()="<<fileInfo.fileName();
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
           {
            qDebug()<<"2222-1";
            continue;
          }
        qDebug()<<"进入foreach";
        if(fileInfo.isDir())
        {    qDebug()<<"6666-1";
            /* 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName()),coverFileIfExist))
               {
                qDebug()<<"3333-1";
                return false;
               }
        }
        else
        {    /* 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName()))
            {
                 targetDir.remove(fileInfo.fileName());
                 qDebug()<<"4444-1";
            }
             /* 进行文件copy*/
            if(!QFile::copy(fileInfo.filePath(),targetDir.filePath(fileInfo.fileName())))
            {        qDebug()<<"5555-1";
                    return false;
            }
        }
     }
     //system("sync");//同步一下
     sync();
     return true;
}

/*
    函数名称：saveDataTofile
    函数功能：保存数据到文件
    输入参数：date 要保存的数据；dataFiLeDirectory 要保存到的文件路径
    输出参数：成功true，失败false
    修改记录：
*/
bool saveHistoryDataTofile(QString date,QString dataFiLeDirectory)
{
    QFile file (dataFiLeDirectory);

     if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))//打开文件
         return false;
     QDateTime time = QDateTime::currentDateTime();//获取当前时间
     QString str = time.toString("yyyy-MM-dd-hh-mm-ss");
     QTextStream out(&file);
     out<<str<< date<<"\n";
     file.close();//关闭文件
     return true;
}

/*
    函数名称：saveHistoryData
    函数功能：保存错误记录
    输入参数：FileNameErr要保存的错误文件路径；errorData要保存的错误记录
    输出参数：无
    修改记录：
*/
void saveHistoryData(QString FileNameErr,QString DateTime,QString CODValue,QString detectValue,QString referVale,QString blankValue,QString absorbValue,QString dataStatus,QString initialT,QString degestinT,QString detectT)
{    
    QFile file(FileNameErr);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        return;
    QTextStream in(&file);
    in <<DateTime<<","<<CODValue<<","<<detectValue<<","<<referVale<<","<<blankValue<<","<<absorbValue<<","<<dataStatus<<","<<initialT<<","<<degestinT<<","<<detectT<<";";
    file.close();
}

/*
    函数名称：saveColiHistoryData
    函数功能：保存错误记录
    输入参数：FileNameErr要保存的错误文件路径；errorData要保存的错误记录
    输出参数：无
    修改记录：
*/
void saveColiHistoryData(QString FileNameErr,QString SampleNo,QString Density,QString Unit,QString DateTime,QString testQual,QString bottleNo)
{
    QFile file(FileNameErr);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        return;
    QTextStream in(&file);
    in <<SampleNo<<","<<Density<<","<<Unit<<","<<DateTime<<","<<testQual<<","<<bottleNo<<";";
    file.close();
}

/*
    函数名称：saveTempHistoryData
    函数功能：保存错误记录
    输入参数：FileNameErr要保存的错误文件路径；errorData要保存的错误记录
    输出参数：无
    修改记录：
*/
void saveTempHistoryData(QString FileNameErr,QString Temp)
{
    QFile file(FileNameErr);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        return;
    QDateTime time = QDateTime::currentDateTime();//获取当前时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QTextStream in(&file);
    in <<str<<","<<Temp<<";";
    file.close();
}

/*
    函数名称：clearHistoryData
    函数功能：清除历史数据
    输入参数：FileNameErr要清楚的历史数据文件路径
    输出参数：无
    修改记录：
*/
void clearHistoryData(QString FileNameErr)
{
    QFile file(FileNameErr);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append))
        return;
    file.resize(0);
}


/*
    函数名称：showHistoryData
    函数功能：显示历史数据
    输入参数：FileNameErr保存错误文件的路径；tableWarningandLog显示错误信息的QTableWidget名
    输出参数：无
    修改记录：
*/
void showHistoryData( QString FileNameErr,QTableWidget *tableWarningandLog)
{

    /***********显示错误记录文件里的内容***************************/
    int i = 0,j = 0;

	QString temp_str;
	//int ii = 0; jj = 0;
    //QString FileNameErr =Errorlist;
    QString tmpData;
    QStringList HstoryDataList;
    QFile file(FileNameErr);
    if(file.open(QFile::ReadOnly))//打开文件
    {
        QTextStream out(&file);
        tmpData = out.readAll();
        HstoryDataList = tmpData.split(";");
        tableWarningandLog->setRowCount(HstoryDataList.count());
        for(i  = 0;i< HstoryDataList.count();i++)
        {
			//qDebug()<<errData.value(i);
            if(HstoryDataList.value(i) == "")
                continue;
            QString historyDataString = HstoryDataList.value(HstoryDataList.count()-(i + 2));
            QStringList dataList = historyDataString.split(",");
            for(j = 0;j<dataList.count();++j)
            {
                QTableWidgetItem *qtwi = new QTableWidgetItem(dataList.value (j));
				//qDebug<<QObject::tr("i = %1 j = %2 text = %3\r\n").arg(i).arg(j).arg(dataList.value (j));
//				printf("i = %d j = %d \r\n", i, j);
//				qDebug()<<dataList.value(j);
                qtwi->setText(dataList.value(j));

                tableWarningandLog->setItem(i,j,qtwi);
				temp_str = QString(dataList.value(j));
//				qDebug()<<temp_str;

			}
        }
    }
    else
    {
        qDebug()<<"open warning message failed";
    }
    file.close();//关闭文件

    /**********************************************************/
}

/*
    函数名称：setSysTime
    函数功能：设置系统时间
    输入参数：char *t_string 格式为"yyyy-mm-dd hh:mm:ss"
    输出参数：成功返回0，失败返回1
    修改记录：
*/
int setSysTime(char *t_string)
{
    printf("t_string = %s\n",t_string);
    struct tm time_tm;
    struct timeval time_tv;
    time_t timep;
    int ret = 0;

    sscanf(t_string, "%d-%d-%d %d:%d:%d", &time_tm.tm_year, &time_tm.tm_mon, &time_tm.tm_mday, &time_tm.tm_hour, &time_tm.tm_min, &time_tm.tm_sec);
    time_tm.tm_year -= 1900;
    time_tm.tm_mon -= 1;
    time_tm.tm_wday = 0;
    time_tm.tm_yday = 0;
    time_tm.tm_isdst = 0;

    timep = mktime(&time_tm);
    time_tv.tv_sec = timep;
    time_tv.tv_usec = 0;

    ret = settimeofday(&time_tv, NULL);
    if(ret != 0)
    {
        fprintf(stderr, "settimeofday failed\n");
        return -1;
    }
    else
    {
        printf("setSysTime success\n");
    }
    ret = system("hwclock -w");
    if(ret == -1)
    {
        printf("hwclock failed\n");
        return -1;
    }

    return 0;
}

/*
    函数名称：QstringTostring
    函数功能：将QT的QString类转换为字符串
    输入参数：需转换的QString类
    输出参数：转换完成的字符串
    修改记录：
*/
char* QstringTostring(QString qs)
{
    char *ch;
    QByteArray ba = qs.toLatin1();

    ch=ba.data();
    return ch;
}



/*
    函数名称：stringToQstring()
    函数功能：将Q字符串转换为T的QString类
    输入参数：需转换的字符串
    输出参数：转换完成的QString类
    修改记录：
*/
QString stringToQstring(char* str)
{
    QString qs;
    qs = QString(QLatin1String(str));
    return qs;
}

/*
    函数名称：CoefficientCaculate()
    函数功能：计算校零系数
    输入参数：测量光强、参比光强
    输出参数：校零系数K0
    修改记录：
*/
float Coefficient0Caculate(float detectedValue,float referedValue)
{
    float K0;
    if(referedValue)
    {
        K0=detectedValue/referedValue;
        return K0;
    }else
    return 0;
}

/*
    函数名称：AbsorbanceValue()
    函数功能：计算吸光度
    输入参数：校标浓度、校标吸光度
    输出参数：COD浓度值
    修改记录：
*/
float AbsorbanceValue(float detectedValue,float referedValue)
{
    float absorb;
    float median_1;
    if(detectedValue)
    {
        median_1 = referedValue/detectedValue;
        if(median_1)
        {
            absorb = log10(median_1);
            if(absorb>=0)
            {return absorb;}
            else
            {
                absorb=0-absorb;
                return absorb;
            }
        }else
        {return 0;}
    }else
    {return 0;}
}

/*
    函数名称：Coefficient1Caculate()
    函数功能：计算校标系数
    输入参数：测量光强、参比光强、校零系数、校标浓度
    输出参数：校标系数K1
    修改记录：
*/
float Coefficient1Caculate(float detectedValue,float referedValue,float density)
{
    float K1;
    float median_1;
    median_1 = AbsorbanceValue(detectedValue,referedValue);
    if(median_1)
    {
        K1 = density/median_1;
        return K1;
    }else
    {
        return 0;
    }
}

/*
    函数名称：DensityCaculate()
    函数功能：计算COD浓度值
    输入参数：测量光强、参比光强、校零系数、校标系数、修正因子、介质因子
    输出参数：COD浓度值
    修改记录：
*/
float DensityCaculate(float detectedValue,float referedValue,float K1,float b,float medium)
{
    float density;
    float median_1;
    float median_2;
    median_1 = AbsorbanceValue(detectedValue,referedValue);
    median_2 = K1*median_1+b;
    density = medium*median_2;
    return density;

}

/*
    函数名称：ParaCaculate()
    函数功能：多点校标求K、R2、B
    输入参数：校标浓度、校标吸光度
    输出参数：K、R2、B
    修改记录：
    备注说明：x-absorbance,y-density
*/
float ParaCaculate(float x1,float x2,float x3,float x4, float y1, float y2, float y3, float y4)
{
    float A[3];
	float xx,xy,yy,xx1,yy1;//,xp,yp;
    float D1,D2;
    float R1;
 //   xp = (x1+x2+x3+x4)/4;
 //   yp = (y1+y2+y3+y4)/4;
    xx = (x1*x1+x2*x2+x3*x3+x4*x4)/4;
    yy = (y1*y1+y2*y2+y3*y3+y4*y4)/4;
    xy = (x1*y1+x2*y2+x3*y3+x4*y4)/4;
    xx1 = x1*x1;
    yy1 = y1*y1;
    D1 = (xx - xx1)*(yy - yy1);
    D2 = sqrt(D1);
    R1 = xy - x1*y1;
    if(D2)
    {
        A[2]=-1*R1/D2;//R2
        A[0]=(xy-x1*y1)/(xx-xx1);  //K
        A[1]=y1-A[0]*x1;//B
        int i;
        for(i = 0;i<3;i++)
        {
            return A[i];
        }
	}
	else{
        return 0;
	}
}

/*
    函数名称：saveCurveDataForDis
    函数功能：保存测量时间、数据，以便历史曲线调用
    输入参数：FileNameErr要保存的错误文件路径；errorData要保存的错误记录
    输出参数：无
    修改记录：
*/
//void saveCurveDataForDis(QString FileNameErr, QString Density)
//{
//    QFile file(FileNameErr);
//    if(!file.open(QIODevice::WriteOnly) | QIODevice::Text|QIODevice::Append)
//        return;
//    QTextStream in(&file);
//    in << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "," << Density << ";";
//}


