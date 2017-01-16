#include "common.h"
#include "gpio.h"
#include "ad.h"
#include <QTimer>
#include "QGuiApplication"

typedef union _CRC
{
    ushort crc16;
    uchar by[2];
}CRC;

int mpn_0[][6] = {{0,2,4,5,7,9},
                {2,4,6,7,9,11},
                {4,6,7,9,11,13},
                {6,7,9,11,13,15},
                {8,9,11,13,15,17},
                {9,11,13,15,17,19},
                };

int mpn_1[][6] = {{2,4,6,8,10,12},
                {4,6,8,10,12,14},
                {6,8,10,12,15,17},
                {8,10,12,15,17,19},
                {11,13,15,17,19,22},
                {13,15,17,19,22,24},
                };

int mpn_2[][6] = {{5,7,9,12,14,16},
                {7,9,12,14,17,19},
                {9,12,14,17,19,22},
                {12,14,17,20,22,25},
                {15,17,20,23,25,28},
                {17,20,23,26,29,32},
                };

int mpn_3[][6] = {{8,11,13,16,20,23},
                {11,14,17,20,23,27},
                {14,17,20,24,27,31},
                {17,21,24,28,32,36},
                {21,24,28,32,36,40},
                {25,29,32,37,41,45},
                };

int mpn_4[][6] = {{13,17,21,25,30,36},
                {17,21,26,31,36,42},
                {22,26,32,38,44,50},
                {27,33,39,45,52,59},
                {34,40,47,54,62,69},
                {41,48,56,64,72,81},
                };

int mpn_5[][6] = {{23,31,43,58,76,95},
                {33,46,63,84,110,130},
                {49,70,94,120,150,180},
                {79,110,140,180,210,250},
                {130,170,220,280,350,430},
                {240,350,540,920,1600,16000},
                };


int unsigned g_Test_Over_Time = FAST_TIME_LIMIT;  // 超时时间
int g_workmode = WORK_MODE_TEST;  // 工作模式

int g_test_st = false;  // 测试状态

int g_mpn_test_st = MPN_TEST_IDLE;  // 测试状态 MPN测方法状态
int g_mpn_test_mui = 1;  // 稀释倍数
int g_mpn_test_index=0; //稀释倍数索引
int g_mpn_test_val[MPN_MAX_NUM] ={0};   //记录测试结果 查表用

int g_work_Test_type[BOTTLE_MAX_NUM] ={ NULL_ID, NULL_ID,NULL_ID, NULL_ID, NULL_ID};  // 测试菌


float g_temp1 = 0.0;		// 实时温度1
float g_temp2 = 0.0;		// 实时温度2
float g_temp3 = 0.0;		// 实时温度1
float g_temp4 = 0.0;		// 实时温度2
float g_temp5 = 0.0;		// 实时温度1
float g_temp_nor = 0.0;		// 实时温度2


//每个瓶号的水类别和曲线
int Bottle1_ClassNo=0;
int Bottle2_ClassNo=0;
int Bottle3_ClassNo=0;
int Bottle4_ClassNo=0;
int Bottle5_ClassNo=0;
int Bottle6_ClassNo=0;
int Bottle7_ClassNo=0;
int Bottle8_ClassNo=0;
int Bottle9_ClassNo=0;
int Bottle10_ClassNo=0;
int Bottle11_ClassNo=0;
int Bottle12_ClassNo=0;
int Bottle13_ClassNo=0;
int Bottle14_ClassNo=0;
int Bottle15_ClassNo=0;
int Bottle16_ClassNo=0;
int Bottle17_ClassNo=0;
int Bottle18_ClassNo=0;
int Bottle19_ClassNo=0;
int Bottle20_ClassNo=0;
int Bottle21_ClassNo=0;
int Bottle22_ClassNo=0;
int Bottle23_ClassNo=0;
int Bottle24_ClassNo=0;
int Bottle25_ClassNo=0;

int Bottle1_ClassNo_1=0;
int Bottle2_ClassNo_1=0;
int Bottle3_ClassNo_1=0;
int Bottle4_ClassNo_1=0;
int Bottle5_ClassNo_1=0;
int Bottle6_ClassNo_1=0;
int Bottle7_ClassNo_1=0;
int Bottle8_ClassNo_1=0;
int Bottle9_ClassNo_1=0;
int Bottle10_ClassNo_1=0;
int Bottle11_ClassNo_1=0;
int Bottle12_ClassNo_1=0;
int Bottle13_ClassNo_1=0;
int Bottle14_ClassNo_1=0;
int Bottle15_ClassNo_1=0;
int Bottle16_ClassNo_1=0;
int Bottle17_ClassNo_1=0;
int Bottle18_ClassNo_1=0;
int Bottle19_ClassNo_1=0;
int Bottle20_ClassNo_1=0;
int Bottle21_ClassNo_1=0;
int Bottle22_ClassNo_1=0;
int Bottle23_ClassNo_1=0;
int Bottle24_ClassNo_1=0;
int Bottle25_ClassNo_1=0;

//定义斜率 截距
double XieLv=0;
double JieJu=0;
bool TOLParaCalibratedFlag=false;
bool FECParaCalibratedFlag=false;
bool TPCParaCalibratedFlag=false;
bool EParaCalibratedFlag=false;

//定义瓶测试的次数
int TOLTestNumber=0;
int FECTestNumber=0;
int TPCTestNumber=0;
int ETestNumber=0;

bool CalibrateFlag=false;//校正标志

//保存测试的瓶号  未用
QVector<int> TOLTestNumberVector(MAX_ONE_TEST_NUM,-1);
QVector<int> FECTestNumberVector(MAX_ONE_TEST_NUM,-1);
QVector<int> TPCTestNumberVector(MAX_ONE_TEST_NUM,-1);
QVector<int> ETestNumberVector(MAX_ONE_TEST_NUM,-1);


ST_HISTORY_DATA gst_His_Info[MAX_TEST_NUM];  // 浓度保存
ST_HISTORY_DATA gst_His_Info_to_plc[BOTTLE_MAX_NUM];  // 发送给 PLC 数据
int CalcMethod[MAX_TEST_NUM]={0}; //暂时未用
int g_System_Info = SYSTEM_OK;  // 系统状态
// log 记录信息
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("D:");
        break;

    case QtWarningMsg:
        text = QString("W:");
        mutex.unlock();
        return ;
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    long len = file.size();
    file.close();
    if(len > LOG_FILE_SIZE)  // 1M
    {
        QFile::remove("log.txt");//刪除文件
    }

    mutex.unlock();
}

//输入不带CRC码的数据时，返回值是CRC码
//输入带CRC码的数据时，则可以进行校验，返回0时CRC校验成功，否则CRC校验失败
ushort CRC16(uchar *ba, int size)
{
   CRC crc;
   crc.crc16 = 0xffff;
   int i, l;
   for (i=0; i<size; i++)
   {
       uchar ch = ba[i];
       crc.by[0] = crc.by[0] ^ ch;
       for (l = 0; l < 8; l++)
       {
           if (crc.by[0] & 0x01)
           {
               crc.crc16 = crc.crc16 >> 1;
               crc.crc16 = crc.crc16 ^ 0xa001;
           }
           else
           {
               crc.crc16 = crc.crc16 >> 1;
           }
       }
   }
   //uchar swap = crc.by[0];
   //crc.by[0] = crc.by[1];
   //crc.by[1] = swap;
   return crc.crc16;
}




// 硬件初始化
void HardWare_Init(void)
{
    // 打开 gpio 驱动
    Gpio_Init(); // 打开gpio 驱动
    LED_Power(0, 0); // 关闭 EN 引脚
    Gpio_set(TOL_FEC_TPC_AD_EN, GPIO_LOW);  //关闭 EN2

#if 0
    // 打开ad 驱动
    ead_init();  // ad
    ead_set_control_bit_length(AD_WordLength_16Bit); // 24 bit ad
#endif

    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
}
// 硬件解除
void HardWare_UnInit(void)
{
    Gpio_UnInit();  // 关闭gpio
    ead_deinit();   // 关闭 ad
}

//LED 控制
// num --> led 号
// val --> led 状态 0--> 全灭   1--> 亮
void LED_Power(unsigned int num, unsigned int val)
{
    unsigned char addr;

    //qDebug()<<QString("LED_Power=num = %1 val=  %2 ").arg(num).arg(val);
    //addr = num % GPIO_ZU_NUM;

//	/usleep(1000);// 1ms
    if(GPIO_HIGH == val)
    {
        //Gpio_set(LED_EN, GPIO_HIGH);  // 打开EN
        Gpio_set(LED_EN, GPIO_LOW);  // 打开EN
        addr = num % MAX_ONE_GROUP_NUM;
        if(num >= ECOLI_START_NUM) // 埃希氏
        {
            addr = addr * 3;
        }
        else if(num >= TPCCOLI_START_NUM)  // 菌落总数
        {
            addr = addr * 3 + 2;
        }
        else if(num >= FECCOLI_START_NUM)	 // 耐热
        {
            addr = addr * 3 + 1;
        }
        else if(num >= TOLCOLI_START_NUM) // 总大肠
        {
            addr = addr * 3 + 1;
        }
#if 0
        if(addr >= 15)//
        {
            addr = 5;
        }
#endif
//		qDebug()<<QString("LED_Power === addr = %1 ").arg(addr);
        //addr = addr + 1;
        if(addr&0x01){
            Gpio_set(LED_A0, GPIO_HIGH);
//			qDebug()<<"LED_A0 === 1 ";
        }
        else{
            Gpio_set(LED_A0, GPIO_LOW);
            //qDebug()<<"LED_A0 ===  0";
        }
        if(addr&0x02){
            Gpio_set(LED_A1, GPIO_HIGH);
            //qDebug()<<"LED_A1 === 1 ";
        }
        else{
            Gpio_set(LED_A1, GPIO_LOW);
            //qDebug()<<"LED_A1 === 0 ";
        }
        if(addr&0x04){
            Gpio_set(LED_A2, GPIO_HIGH);
            //qDebug()<<"LED_A2 === 1 ";
        }
        else{
            Gpio_set(LED_A2, GPIO_LOW);
            //qDebug()<<"LED_A2 === 0 ";
        }
        if(addr&0x08){
            Gpio_set(LED_A3, GPIO_HIGH);
            //qDebug()<<"LED_A3 === 1 ";
        }
        else{
            Gpio_set(LED_A3, GPIO_LOW);
            //qDebug()<<"LED_A3 === 0 ";
        }
        Gpio_set(LED_EN, GPIO_HIGH);  // 打开EN

    }
    else
    {
        Gpio_set(LED_EN, GPIO_LOW);  //关闭 EN
    }
}
//获取 AD 值
int Get_ad(unsigned int num)
{
    unsigned int chanel= 0, addr = 0;
    double ad_val = 0, ad_val1 = 0, ad_val2 = 0;

    if(num > MAX_TEST_NUM)
    {
        return  ad_val;
    }

#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_HIGH);  // dis
#endif

#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif
    usleep(1*1000); //10ms
    chanel = (num%MAX_ONE_TEST_NUM)/ MAX_ONE_GROUP_NUM;  // 需要 采集的AD 通道
    addr = num % MAX_ONE_GROUP_NUM;  // 需要 采集的是那一路
//    printf("num == %d addr = %d \r\n", num, addr);


    if(num >= ECOLI_START_NUM)  // 埃希氏
    {
        gpio_set(E_AD_EN, GPIO_HIGH);
    }
    else{
        gpio_set(E_AD_EN, GPIO_LOW);
    }
    addr = addr*2 ;

//    printf("\r\n num == %d addr = %d \r\n", num, addr);


    if(addr&0x01){
        Gpio_set(AD_A0, GPIO_HIGH);
//        qDebug()<<"AD_A0 === 1 ";
    }
    else{
        Gpio_set(AD_A0, GPIO_LOW);
//        qDebug()<<"AD_A0 === 0 ";
    }
    if(addr&0x02){
        Gpio_set(AD_A1, GPIO_HIGH);
//        qDebug()<<"AD_A1 === 1 ";
    }
    else{
        Gpio_set(AD_A1, GPIO_LOW);
//        qDebug()<<"AD_A1 === 0 ";
    }
    if(addr&0x04){
        Gpio_set(AD_A2, GPIO_HIGH);
//        qDebug()<<"AD_A2 === 1 ";
    }
    else{
        Gpio_set(AD_A2, GPIO_LOW);
//        qDebug()<<"AD_A2 === 0 ";
    }
    if(addr&0x08){
        Gpio_set(AD_A3, GPIO_HIGH);
//        qDebug()<<"AD_A3 === 1 ";
    }
    else{
        Gpio_set(AD_A3, GPIO_LOW);
//        qDebug()<<"AD_A3 === 0 ";
    }
    usleep(2*1000);
#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif
#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
#endif
    //printf("chanel =[]= %d\r\n", chanel);

#if 1//!HARDWARE_V101
    usleep(200*1000);  // 200ms
#endif


    ead_read_raw(chanel,&ad_val);
    //printf("<<1>>addr = %d ad_val == %f\r\n \r\n",addr, ad_val);
#if 1
    ead_read_raw(chanel,&ad_val1);
    //printf("<<1>>addr = %d ad_val == %f\r\n \r\n",addr, ad_val1);
    ead_read_raw(chanel,&ad_val2);
    //printf("<<1>>addr = %d ad_val == %f\r\n \r\n", addr,ad_val1);
    ad_val = (ad_val + ad_val1 + ad_val2)/3;
#endif
    //printf("<<11>>addr = %d ad_val == %f\r\n \r\n",addr, ad_val);
#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
#endif
#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif



    return (int)ad_val;
}

//获取 AD 值 电压值
double Get_val(unsigned int num)
{
    unsigned int chanel= 0, addr = 0;
    double ad_val = 0.0, ad_val1 = 0.0, ad_val2 = 0.0;

    if(num > MAX_TEST_NUM)
    {
        return  ad_val;
    }
#if 0
#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_HIGH);  // dis
#endif
#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif
#endif
    usleep(1*1000); //10ms
    chanel = (num%MAX_ONE_TEST_NUM)/ MAX_ONE_GROUP_NUM;  // 需要 采集的AD 通道
    addr = num % MAX_ONE_GROUP_NUM;  // 需要 采集的是那一路
    //printf("num == %d addr = %d \r\n", num, addr);


    if(num >= ECOLI_START_NUM)  // 埃希氏
    {
        gpio_set(E_AD_EN, GPIO_HIGH);
    }
    else{
        gpio_set(E_AD_EN, GPIO_LOW);
    }
    addr = addr*2 ;

    //printf("\r\n num == %d addr = %d \r\n", num, addr);
#if 0
#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif
#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
#endif
#endif
    usleep(1*1000); //10ms
    if(addr&0x01){
        Gpio_set(AD_A0, GPIO_HIGH);
        //qDebug()<<"AD_A0 === 1 ";
        printf("AD_A0 === 1 \r\n");
    }
    else{
        Gpio_set(AD_A0, GPIO_LOW);
        //qDebug()<<"AD_A0 === 0 ";
        printf("AD_A0 === 0 \r\n");
    }
    if(addr&0x02){
        Gpio_set(AD_A1, GPIO_HIGH);
        //qDebug()<<"AD_A1 === 1 ";
    }
    else{
        Gpio_set(AD_A1, GPIO_LOW);
        //qDebug()<<"AD_A1 === 0 ";
    }
    if(addr&0x04){
        Gpio_set(AD_A2, GPIO_HIGH);
        //qDebug()<<"AD_A2 === 1 ";
    }
    else{
        Gpio_set(AD_A2, GPIO_LOW);
        //qDebug()<<"AD_A2 === 0 ";
    }
    if(addr&0x08){
        Gpio_set(AD_A3, GPIO_HIGH);
        //qDebug()<<"AD_A3 === 1 ";
    }
    else{
        Gpio_set(AD_A3, GPIO_LOW);
        //qDebug()<<"AD_A3 === 0 ";
    }
    usleep(2*1000);
#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif
#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
#endif
    //Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
    //printf("chanel == %d\r\n", chanel);

    usleep(200*1000);  // 200ms

    ead_read_adc(chanel,&ad_val);
    //printf("<<1>>addr = %d ad_val == %f\r\n \r\n",addr, ad_val);
#if 1
    ead_read_adc(chanel,&ad_val1);
    //printf("<<1>>addr = %d ad_val == %f\r\n \r\n",addr, ad_val1);
    ead_read_adc(chanel,&ad_val2);
    //printf("<<1>>addr = %d ad_val == %f\r\n \r\n", addr,ad_val1);
    ad_val = (ad_val + ad_val1 + ad_val2)/3;
#endif
    //printf("<<11>>addr = %d ad_val == %f\r\n \r\n",addr, ad_val);
#if 1
#if HARDWARE_V101
    Gpio_set(AD_EN, GPIO_HIGH);  //使能 EN
#endif
#if HARDWARE_V100
    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
#endif
#endif

    return ad_val;
}


void set_system_time(int year, uchar mon, uchar day, uchar hour, uchar min, uchar sec)
{
    struct tm nowtime;
    time_t t;

    printf("year = %d  mon= %d, day = %d, hour = %d, min = %d, sec = %d \r\n", year, mon, day, hour, min, sec);
    nowtime.tm_sec = sec;/* Seconds.[0-60](1 leap second) 56*/
    nowtime.tm_min = min;/* Minutes.[0-59] 33*/
    nowtime.tm_hour = hour;/* Hours. [0-23]  13*/
    nowtime.tm_mday = day;/*  Day.[1-31]  7*/
    nowtime.tm_mon = mon-1;;/* Month.[0-11]  9*/
    nowtime.tm_year = year - 1900;;/* Year- 1900.   2015*/
    nowtime.tm_isdst = -1;/*DST.[-1/0/1]夏令时 */
    //nowtime.tm_isdst = 1;/*DST.[-1/0/1]夏令时 */

    t = mktime(&nowtime);
    stime(&t);
    int ret = system("hwclock -w");
    if(ret == -1)
    {
        printf("hwclock failed\n");
    }
}




//#define N 10             //N为要拟合的数据的个数

//float X[10] = {1.9,0.8,1.1,0.1,-0.1,4.4,4.6,1.6,5.5,3.4};
//float Y[10] = {0.7,-1.0,-0.2,-1.2,-0.1,3.4,0.0,0.8,3.7,2.0};



float Squre_sum(float *c, unsigned int num) ;
float Sum_Average(float *d, unsigned int num);
float X_Y_By(float *m,float *n, unsigned int num);
float Squre_sum(float *c, unsigned int num);
//void Line_Fit(void);
// 线性拟合算法
void Line_Fit(float *X, float *Y, int N, float *a, float *b)
//void Line_Fit(float *X, float *Y, )
{
    float x_sum_average = 0;   //数组 X[N] 个元素求和 并求平均值
    float y_sum_average = 0;   //数组 Y[N] 个元素求和 并求平均值
    float x_square_sum = 0;    //数组 X[N] 个个元素的平均值
    float x_multiply_y = 0;    //数组 X[N]和Y[N]对应元素的乘机
    float K = 0;               //拟合直线的斜率
    float R = 0;               //拟合直线的截距

    x_sum_average = Sum_Average(X, N); // 计算 X坐标和
    y_sum_average = Sum_Average(Y, N); // 计算 Y坐标和
    x_square_sum = Squre_sum(X, N);     // 计算X坐标平方值 X2
    x_multiply_y = X_Y_By(X,Y, N);
    //
    K = ( x_multiply_y - N * x_sum_average * y_sum_average)/( x_square_sum - N * x_sum_average*x_sum_average );
    R = y_sum_average - K * x_sum_average;

    printf("K = %f\n",K);
    printf("R = %f\n",R);

    *a = K;
    *b = R;
}
// 计算数组和值
float Sum_Average(float *d,unsigned int num)
{
    unsigned int i=0;
    float z=0;

    for(i = 0; i < num; i++)
    {
        z = z + d[i];
        printf("Sum_Average z = %f d[%d] = %f \r\n",z, i,d[i]);
    }
    z = z/num;

    return z;
}
// 计算XY乘值
float X_Y_By(float *m,float *n, unsigned int num)
{
    unsigned int i=0;
    float z = 0;

    for(i = 0; i < num; i++)
    {
        z = z + m[i]*n[i];

        printf("Sum_Average z = %f m[%d] = %f n[%d] = %f\r\n",z, i,m[i], i, n[i]);
    }
    return z;
}
// 计算数组平方值
float Squre_sum(float *c, unsigned int num)
{
    unsigned int i=0;
    float z=0;

    for(i = 0; i < num; i++)
    {
        z = z + c[i]*c[i];
        printf("Sum_Average z = %f c[%d] = %f \r\n",z, i,c[i]);
    }
    return z;
}
#if 0  //
void main(void)
{
    Line_Fit();
}
#endif

// 截屏函数
void getFullScree(char *name)
{
    return;
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(0).save(name,"jpg");
}

// GBK转UTF8
inline QString GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
    return g2u;
}
// UTF8转GBK
QString UTF82GBK(const QString inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    qDebug()<<"UTF82GBK utf2gbk=" + utf2gbk + "UTF82GBK = inStr ="+inStr ;
    return utf2gbk;
}

// GBK转UTF8
inline std::string gbk2utf8(const QString &inStr)
{
    return GBK2UTF8(inStr).toStdString();
}
// UTF8转GBK
inline QString utf82gbk(const std::string inStr)
{
    QString str = QString::fromStdString(inStr);

    return UTF82GBK(str);
}

// string 转 char
void str2char(QString str, char *data)
{
    QByteArray ba = str.toLocal8Bit();
    qDebug()<< "str2char == str ="+str;
    //printf("str2char == ");
    for(int i = 0; i < str.length(); i++)
    {
       // str.at();
        *(data + i) = ba.at(i);
        //data ++;
        printf("data + %d = 0x%x \r\n", i, *(data +i));
    }

    //data = ba.data();

    printf("data== %s \r\n",ba.data());
}
