#include "pwm.h"
#include <sys/types.h>
#include <math.h>
//#include "ioctl.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <QFile>

#define MAX_NUM  64   // 最大条数

#define MAX_CLK_NUM   1000000000
static int pwm_fb;   //打开 设备
// 打开设备
const char pwm_open[][MAX_NUM] ={"/sys/class/pwm/pwmchip0/export",
				 "/sys/class/pwm/pwmchip1/export",
				 "/sys/class/pwm/pwmchip2/export",
				 "/sys/class/pwm/pwmchip3/export"
					   };
// 判断设备是否 打开
const char pwm_have_open[][MAX_NUM] ={"/sys/class/pwm/pwmchip0/pwm0",
				 "/sys/class/pwm/pwmchip1/pwm0",
				 "/sys/class/pwm/pwmchip2/pwm0",
				 "/sys/class/pwm/pwmchip3/pwm0"};
//pwm 打开
const char pwm_en[][MAX_NUM] ={"/sys/class/pwm/pwmchip0/pwm0/enable",
				 "/sys/class/pwm/pwmchip1/pwm0/enable",
				 "/sys/class/pwm/pwmchip2/pwm0/enable",
				 "/sys/class/pwm/pwmchip3/pwm0/enable"};

//pwm 波特率
const char pwm_period[][MAX_NUM] ={"/sys/class/pwm/pwmchip0/pwm0/period",
				 "/sys/class/pwm/pwmchip1/pwm0/period",
				 "/sys/class/pwm/pwmchip2/pwm0/period",
				 "/sys/class/pwm/pwmchip3/pwm0/period"};

//pwm 占空比
const char pwm_duty[][MAX_NUM] ={"/sys/class/pwm/pwmchip0/pwm0/duty_cycle",
				 "/sys/class/pwm/pwmchip1/pwm0/duty_cycle",
				 "/sys/class/pwm/pwmchip2/pwm0/duty_cycle",
				 "/sys/class/pwm/pwmchip3/pwm0/duty_cycle"};

// pwm 初始化  num--.pwm 口   period 波特率  duty 占空比
int pwm_init(int num, int period, int duty, int enbal)
{
	char buffer[MAX_NUM];
	int port = num - 1;
	int len = 0;

	printf( "pwm_init num = %d, period= %d, duty = %d, enbal= %d \r\n", num, period, duty, enbal);
	if(port > 3){
		qDebug() << "num error ==============\r\n";
		return -1;
	}
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, PWM_DRIVE_NAME_ST);
//	sprintf();
	QFile f(pwm_have_open[port]);
	if(!f.exists())  // 文件存在说明没有打开过
    {
		if(pwm_fb){
			close(pwm_fb);
        }
		pwm_fb = -1;

		pwm_fb = open(pwm_open[port], O_WRONLY); // 打开文件
		if(pwm_fb < 0)
        {
            qDebug()<<("open device, please insmod icsadc.ko");
		 //   g_System_Info = ERROR_AD_OPEN_ERROR;
            return -1;
        }
		len = snprintf(buffer, sizeof(buffer), "%d", 0);

		if(write(pwm_fb, buffer, len) < 0) {
			   printf("Fail to export !");
			   return -1;
		}
		close(pwm_fb);
		pwm_fb = 0;

    }
	else{

		if(pwm_fb){
			close(pwm_fb);
		}
		pwm_fb = -1;
		// 设置 en
		pwm_fb = open(pwm_en[port], O_WRONLY);
		if(pwm_fb < 0)
		{
			qDebug()<<("open deviceerror , en error\r\n");
			return -1;
		}
		len = snprintf(buffer, sizeof(buffer), "%d", enbal);

		if(write(pwm_fb, buffer, len) < 0) {
			   printf("Fail to export !");
			   return -1;
		}
		close(pwm_fb);
		pwm_fb = 0;
		// 设置 波特率
		pwm_fb = open(pwm_period[port], O_WRONLY);
		if(pwm_fb < 0)
		{
			qDebug()<<("open deviceerror , en error\r\n");
			return -1;
		}
		len = snprintf(buffer, sizeof(buffer), "%d", MAX_CLK_NUM/period);

		if(write(pwm_fb, buffer, len) < 0) {
			   printf("Fail to export !");
			   return -1;
		}
		close(pwm_fb);
		pwm_fb = 0;


		// 设置 波特率
		pwm_fb = open(pwm_duty[port], O_WRONLY);
		if(pwm_fb < 0)
		{
			qDebug()<<("open deviceerror , en error\r\n");
			return -1;
		}
		len = snprintf(buffer, sizeof(buffer), "%d", (MAX_CLK_NUM/period)*duty/100);

		if(write(pwm_fb, buffer, len) < 0) {
			   printf("Fail to export !");
			   return -1;
		}
		close(pwm_fb);
		pwm_fb = 0;

	}


    return 0;
}
// 关闭ad
int pwm_deinit(void)
{

	if(pwm_fb){
		close(pwm_fb);
    }
	pwm_fb = -1;

    return 0;
}








#if 0 //test
int main(void)
{
	int i;
	double vol;
	
	ead_init();

//	ead_set_control_bit_length(AD_WordLength_24Bit); // 24 bit ad 
	ead_set_control_bit_length(AD_WordLength_16Bit); // 24 bit ad 

//	ead_set_sample_mode(AD_WordLength_24Bit);
	while(1)
	{
		for(i = 1; i <= 8; i++)
		{
			ead_set_channel(i);
			usleep(160*1000);
			ead_read_adc(&vol);
			printf("\n vol[%d] = %f", i, vol);
//			usleep(200*1000); // 10ms
		}
		printf("\n ");
		sleep(2);
	}
	ead_deinit();
}
	
#endif


