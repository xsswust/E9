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

int unsigned g_TIMEOUT= TIMEOUT_LIMIT;  // 超时时间

int unsigned g_Test_Over_Time = FAST_TIME_LIMIT;  // 超时时间
int g_workmode = WORK_MODE_TEST;  // 工作模式

int g_test_st = false;  // 测试状态

int g_mpn_test_st = MPN_TEST_IDLE;  // 测试状态 MPN测方法状态
int g_mpn_test_mui = 1;  // 稀释倍数
int g_mpn_test_val[MPN_MAX_NUM] ={0};   //记录测试结果 查表用

int g_work_Test_type[BOTTLE_MAX_NUM] ={ NULL_ID, NULL_ID,NULL_ID, NULL_ID, NULL_ID};  // 测试菌
// 接口板 IO
int gpio_out[] = {MAIN_GPIO_OUT0, MAIN_GPIO_OUT1,MAIN_GPIO_OUT2, MAIN_GPIO_OUT3,MAIN_GPIO_OUT4, MAIN_GPIO_OUT5,MAIN_GPIO_OUT6, MAIN_GPIO_OUT7};
int gpio_in[] = {MAIN_GPIO_IN0, MAIN_GPIO_IN1,MAIN_GPIO_IN2, MAIN_GPIO_IN3,MAIN_GPIO_IN4, MAIN_GPIO_IN5,MAIN_GPIO_IN6, MAIN_GPIO_IN7};

UINT8 gExt_out[MAX_EXT_OUT_GPIO + 1];
UINT8 gExt_in[MAX_EXT_IN_GPIO + 1];
// 进液阀
UINT8 gJinYeFa[] ={ FA_JINYE1, FA_JINYE2,FA_JINYE3, FA_JINYE4, FA_JINYE5, FA_JINYE6};
UINT8 gPaiYeFa[] ={ FA_PAIYE1, FA_PAIYE2,FA_PAIYE3, FA_PAIYE4, FA_PAIYE5, FA_PAIYE6};

//ST_HISTORY_DATA gst_His_Info[MAX_TEST_NUM];  // 浓度保存
ST_HISTORY_DATA gst_His_Info_to_plc[BOTTLE_MAX_NUM];  // 发送给 PLC 数据
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
	int i = 0;
	// 打开 gpio 驱动
	Gpio_Init(); // 打开gpio 驱动
//	LED_Power(0, 0); // 关闭 EN 引脚
//	Gpio_set(TOL_FEC_TPC_AD_EN, GPIO_LOW);  //关闭 EN2

#if 0
	// 打开ad 驱动
	ead_init();  // ad
	ead_set_control_bit_length(AD_WordLength_16Bit); // 24 bit ad
#endif

    Gpio_set(AD_EN, GPIO_LOW);  //使能 EN
// 初始化扩展板输入状态
	for(i = 0 ; i < MAX_EXT_IN_GPIO; i++){   // 默认全部为低
		gExt_in[i] = 0;
	}
// 初始化扩展板输出状态
	for(i = 0 ; i < MAX_EXT_OUT_GPIO; i++){  // 默认输入全为0
		gExt_out[i] = 0;
	}
// 初始化 接口板输出状态
	for(i = 0; i < MAX_MAIN_OUT_GPIO; i++){
		Set_Main_Gpio(i, GPIO_LOW);
	}
	UpdateExtOutGpio();
}
// 硬件解除
void HardWare_UnInit(void)
{
	Gpio_UnInit();  // 关闭gpio
	ead_deinit();   // 关闭 ad
}
// 设置系统时间
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

int gPwm1_flag = false;
//电机1 转向控制
// flag -> true  正转    flag--->false 反转
void SetMaDa1_Dir(bool flag)
{
	printf("\r\n [SetMaDa1_Dir]  flag = %d \r\n ", flag);
	if(flag == true){
		Gpio_set(gpio_out[1], 1);
		gPwm1_flag = false;
	}
	else{
		Gpio_set(gpio_out[1], 0);
		gPwm1_flag = true;
	}
}


int gPwm2_flag = false;
//电机2 转向控制
void SetMaDa2_Dir(bool flag)
{
	printf("\r\n [SetMaDa2_Dir]  flag = %d \r\n ", flag);
	if(flag == true){
		Gpio_set(gpio_out[3], 1);
		gPwm2_flag = false;
	}
	else{
		Gpio_set(gpio_out[3], 0);
		gPwm2_flag = true;
	}
}


// pwm1  设置转速  机械臂
// flag --> true 开始转  flag---> false 停止转
void SetMaDa1_Start(bool flag)
{
	if(true == flag){
		pwm_init(1, 1000, 50, 1);
	//	gpio_flag = false;
	}
	else{
		pwm_init(1, 1000, 50, 0);
	//	gpio_flag = true;
	}
}
// pwm2

// pwm2  设置转速  转盘
// flag --> true 开始转  flag---> false 停止转
void SetMaDa2_Start(bool flag)
{
//	gFlag = PWM2_INTERFACE;

	if(true == flag){
		pwm_init(2, 1000, 50, GPIO_HIGH);
	//	gpio_flag = false;
	}
	else{
		pwm_init(2, 1000, 50, GPIO_LOW);
	//	gpio_flag = true;
	}
}
// 设置接口板GPIO值
//num ---> 接口板gpio号 0----7
// flag --> 数值  true --> 1   false --> 0

void Set_Main_Gpio(int num, int val){

	if(num < 0 || num >= MAX_MAIN_OUT_GPIO){
		return ;
	}

	Gpio_set(gpio_out[num], val);
}

// 获取接口板GPIO值
//num ---> 接口板gpio号 0----7
// val --> 数值

int Get_Main_Gpio(int num){

	if(num < 0 || (num >= MAX_MAIN_IN_GPIO)){
		return -1;
	}

	return Gpio_get(gpio_in[num]);
}

// 设置扩展口输出
bool Set_Ext_Gpio(int num, int val){
	printf("\r\n[1] [S]   num == %d  val = %d \r\n", num, val);
	if((num < 0) || (num >= MAX_EXT_OUT_GPIO)){
		return false;
	}
	gExt_out[num] = val;
	printf("\r\n[2] [Set_Ext_Gpio]   num == %d  val = %d \r\n", num, val);
	UpdateExtOutGpio();
	return true;
}

// 更新全部扩展板输出状态
void UpdateExtOutGpio(){
	int i = 0;

	//qDebug<<gExt_out;
//	Gpio_set(OUT_CLK, GPIO_LOW);
	//sleep(1);
	Gpio_set(OUT_LE, GPIO_LOW);
	Gpio_set(OUT_SDI, gExt_out[i]);
	for(i = 0; i < MAX_EXT_OUT_GPIO; i++)
	{
		Gpio_set(OUT_CLK, GPIO_LOW);
		Gpio_set(OUT_SDI, gExt_out[i]);
		Gpio_set(OUT_CLK, GPIO_HIGH);
		//sleep(1);

		//printf("gExt_out[%d] === %d \r\n", i, gExt_out[i]);
	}
	//sleep(1);
	//Gpio_set(OUT_CLK, GPIO_HIGH);
	Gpio_set(OUT_LE, GPIO_HIGH);

}

// 获取扩展口输入值
int Get_Ext_Gpio(int num){

	if((num < 0) || (num >= MAX_EXT_OUT_GPIO)){
		return false;
	}
	UpdateExtInGpio();
	return gExt_in[num] ;
}

// 更新全部扩展板输入状态
void UpdateExtInGpio(){
	int i = 0;

	Gpio_set(IN_CLK, GPIO_LOW);
	//sleep(1);
	Gpio_set(IN_LE, GPIO_HIGH);  // 先置高在置地进行本芯片的数据保存
	//sleep(1);
	Gpio_set(IN_LE, GPIO_LOW);
	gExt_in[0] = (bool)Gpio_get(IN_DATA);
	for(i = 1; i < MAX_EXT_IN_GPIO ; i++)
	{
		Gpio_set(IN_CLK, GPIO_HIGH);
		gExt_in[i] = (bool)Gpio_get(IN_DATA);
		//sleep(1);
		Gpio_set(IN_CLK, GPIO_LOW);
	}
	//sleep(1);
	Gpio_set(IN_CLK, GPIO_HIGH);
}

// 停止所有阀
void StopAll(){
// 初始化扩展板输出状态
	int i = 0;

	for(i = 0 ; i < MAX_EXT_OUT_GPIO; i++){  // 默认输入全为0
		gExt_out[i] = 0;
	}
// 初始化 接口板输出状态
//	for(i = 0; i < MAX_MAIN_OUT_GPIO; i++){
//		Set_Main_Gpio(i, GPIO_LOW);
//	}
	UpdateExtOutGpio();// 初始化扩展板输出状态

}


