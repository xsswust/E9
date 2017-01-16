#include "ad.h"
#include <sys/types.h>
#include <math.h>
//#include "ioctl.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <QFile>

static ControlReg control_reg;
static int ad_fb;   //打开 设备
// ad 初始化
int ead_init(void)
{
#if TEST_NEW
#else
    QFile f(AD_DRIVE_NAME);
    if(f.exists())
    {
        if(ad_fb){
            close(ad_fb);
        }
        ad_fb = -1;

        ad_fb = open(AD_DRIVE_NAME, O_RDONLY); // 打开文件
        if(ad_fb < 0)
        {
            qDebug()<<("open device, please insmod icsadc.ko");
            g_System_Info = ERROR_AD_OPEN_ERROR;
            return -1;
        }
    }
    else
    {
        g_System_Info = ERROR_AD_DRIVER_NOT_EXIST;
        return -1;
    }
    ead_get_control();
    control_reg.word_length = AD_WordLength_16Bit;
    control_reg.channel_selection = AD_Channel_AIN2;
    ead_set_control();
#endif
    return 0;
}
// 关闭ad
int ead_deinit(void)
{
#if TEST_NEW
#else
    if(ad_fb){
        close(ad_fb);
    }
    ad_fb = -1;
#endif
    return 0;
}

int adc_in_redy()
{
    int value;

    ioctl(ad_fb, IOCTL_EAD_DRDY, &value);

    return value;
}

/** get eac control info
 *
 * This function get ADC raw control info
 * @return 0 if get   successfully
 * @return -1 if get failed
 */
int ead_get_control(void)
{
    DWORD ret;

    ret = ioctl(ad_fb, IOCTL_EAD_GET_CONTROL, &control_reg);

	return ret;
}

// 自校验
void ead_activateselfcalibration()
{
    control_reg.operation_mode =  ACTIVATE_SELF_CALIBRATION;
    // 清空事件
//	WaitForSingleObject(ADCRdyExtEvent, 0);

    ioctl(ad_fb, IOCTL_EAD_SET_CONTROL, &control_reg);
}

// 更新控制字。按照AD ic的特性，需要一起更新
int ead_set_control(void)
{
    DWORD ret;

    // 当设置变更后，需要从新校准, 校准结束后，会自动回到 NORMAL_MODE 模式
    control_reg.operation_mode =  ACTIVATE_SELF_CALIBRATION;
    ret = ioctl(ad_fb, IOCTL_EAD_SET_CONTROL, &control_reg);
	return ret;
}

int ead_get_control_mode(void)
{
    ead_get_control();

    return control_reg.operation_mode;
}

int ead_get_control_gain(void)
{
    ead_get_control();

    return control_reg.pga_gain;
}

// return sample mode
// fast for 16bit
// normal for 24bit
int ead_get_sample_mode(void)
{
    ead_get_control();
    return control_reg.word_length;
}

int ead_get_control_bit_length(void)
{
    ead_get_control();
    return control_reg.word_length;
}

int ead_get_control_bit_bipolar(void)
{
    ead_get_control();
    return control_reg.bipolar;
}

int ead_get_control_filter(void)
{
    ead_get_control();
    return control_reg.filter_selection;
}

int ead_set_control_mode(enum AD_OperatigMode mode)
{
    control_reg.operation_mode = mode;
    return ead_set_control();
}

int ead_set_control_gain(enum AD_Gain gain)
{
    control_reg.pga_gain = gain;
    return ead_set_control();
}

int ead_set_control_bit_length(enum AD_WordLength len)
{
    control_reg.word_length = len;
    return ead_set_control();
}

// fast mode:   16bit
// normal mode: 24bit
int ead_set_sample_mode(enum AD_SAMPLE_MODE mode)
{
    control_reg.word_length = mode;
    return ead_set_control();
}

int ead_set_control_bipolar( enum AD_Bipolar bipolar)
{
    control_reg.bipolar = bipolar;
    return ead_set_control();
}

int ead_set_control_filter( WORD filter)
{
    control_reg.filter_selection = filter;
    return ead_set_control();
}

int ead_set_control_channel( enum AD_Channel channel)
{
    control_reg.channel_selection = channel;
    return ead_set_control();
}

int ead_set_channel(uint8_t channel)
{
	//printf("\n channel === %d \r\n", channel);
    ioctl(ad_fb, IOCTL_EAD_CHANNEL, channel);
    // 切换通道后需要再次校准。
    ead_set_control();
    return 0;
}

// 设置量程范围
int ead_set_sample_range(enum AD_SAMPLE_RANGE range)
{
    switch(range)
    {
    case AD_SAMPLE_RANGE_Bipolar_10V:
            control_reg.pga_gain = AD_Gain_1;
        control_reg.bipolar  = AD_Bipolar;
        break;
    case AD_SAMPLE_RANGE_Unipolar_10V:
        control_reg.pga_gain = AD_Gain_1;
        control_reg.bipolar  = AD_Unipolar;
        break;
    case AD_SAMPLE_RANGE_Bipolar_5V:
        control_reg.pga_gain = AD_Gain_2;
        control_reg.bipolar  = AD_Bipolar;
        break;
    case AD_SAMPLE_RANGE_Unipolar_5V:
        control_reg.pga_gain = AD_Gain_2;
        control_reg.bipolar  = AD_Unipolar;
        break;
    case AD_SAMPLE_RANGE_Bipolar_2V5:
        control_reg.pga_gain = AD_Gain_4;
        control_reg.bipolar  = AD_Bipolar;
        break;
    case AD_SAMPLE_RANGE_Unipolar_2V5:
        control_reg.pga_gain = AD_Gain_4;
        control_reg.bipolar  = AD_Unipolar;
        break;

    case AD_SAMPLE_RANGE_Bipolar_1V:
        control_reg.pga_gain = AD_Gain_8;
        control_reg.bipolar  = AD_Bipolar;
        break;
    case AD_SAMPLE_RANGE_Unipolar_1V:
        control_reg.pga_gain = AD_Gain_8;
        control_reg.bipolar  = AD_Unipolar;
        break;

    case AD_SAMPLE_RANGE_Bipolar_500mV:
        control_reg.pga_gain = AD_Gain_16;
        control_reg.bipolar  = AD_Bipolar;
        break;
    case AD_SAMPLE_RANGE_Unipolar_500mV:
        control_reg.pga_gain = AD_Gain_16;
        control_reg.bipolar  = AD_Unipolar;
        break;
    case AD_SAMPLE_RANGE_Bipolar_150mV:
        control_reg.pga_gain = AD_Gain_64;
        control_reg.bipolar  = AD_Bipolar;
        break;
    case AD_SAMPLE_RANGE_Unipolar_150mV:
        control_reg.pga_gain = AD_Gain_64;
        control_reg.bipolar  = AD_Unipolar;
        break;

    default:
        break;
    }
    return ead_set_control();
}

inline int ad7712_read(uint8_t *pbuf, int len)
{
    ioctl(ad_fb, IOCTL_EAD_GET_AD, pbuf);

//    printf("read   %2X %2X %2X\n", * pbuf, *( pbuf +1), *( pbuf +2) ) ;
    return len;
}

#if 0
int ead_read_raw(unsigned long *value)
{
    int ret = 0;

    uint8_t  iBuf[3]  ;//read buf
    if(control_reg.word_length)
    {
        ret = ad7712_read(iBuf, 3);
        *value = (iBuf[0] << 16) + (iBuf[1] << 8) + iBuf[2];
    }
    else
    {
        ret = ad7712_read(iBuf, 2);
        *value = (iBuf[0] << 8) + iBuf[1];
    }

	//printf("ead_read_raw ret = %d value= 0x%x \r\n", ret, *value);
	return ret;
}
#endif
double ead_convert_raw_voltage(unsigned long value)
{
    double voltage;
    // 24bit 最大值0xffffff
    int power[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    if(control_reg.bipolar == 0)
    {
        if(control_reg.word_length)
        {
            // 双极性， 24bit 计算公式， 参考电平 ±４×ＶＲＥＦ
            // 24bit 最大值0xffffff
            voltage = (( (double)value / 0xffffff ) *2 -1) * (4*2.5)/power[control_reg.pga_gain];
        }
        else
        {
            voltage = (( (double)value / 0xffff ) *2 -1) * (4*2.5)/power[control_reg.pga_gain];
        }
    }
    else
    {
        if(control_reg.word_length)
        {
            voltage = (( (double)value / 0xffffff ) ) * (4*2.5)/power[control_reg.pga_gain];
        }
        else
        {
            voltage = (( (double)value / 0xffff ) ) * (4*2.5)/power[control_reg.pga_gain];
        }
    }
	return voltage;
}
#if !TEST_NEW
/** get ad voltage value
 *
 * @param[out] voltage  point to wait single result, it can be NULL if not care
 * @return  < 0 error
 */

int ead_read_adc(double* volt)
{
    double voltage;
    int ret;

    DWORD val;
    uint8_t  iBuf[3]  ;//read buf
    if(control_reg.word_length)
    {
        ret = ad7712_read(iBuf, 3);
        val = (iBuf[0] << 16) + (iBuf[1] << 8) + iBuf[2];
    }
    else
    {
        ret = ad7712_read(iBuf, 2);
        val = (iBuf[0] << 8) + iBuf[1];
    }


    // 24bit 最大值0xffffff
    int power[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    if(control_reg.bipolar == 0)
    {
        if(control_reg.word_length)
        {
            // 双极性， 24bit 计算公式， 参考电平 ±４×ＶＲＥＦ
            // 24bit 最大值0xffffff
            voltage = (( (double)val / 0xffffff ) *2 -1) * (4*2.5)/power[control_reg.pga_gain];
        }
        else
        {
            voltage = (( (double)val / 0xffff ) *2 -1) * (4*2.5)/power[control_reg.pga_gain];
        }
    }
    else
    {
        if(control_reg.word_length)
        {
            voltage = (( (double)val / 0xffffff ) ) * (4*2.5)/power[control_reg.pga_gain];
        }
        else
        {
            voltage = (( (double)val / 0xffff ) ) * (4*2.5)/power[control_reg.pga_gain];
        }
    }
	*volt = voltage;
	return ret;
}
#endif
/** get ad voltage value
 *
 * @param[in] dwMilliseconds milli seconds to wait, 0 for wait forever
 * @param[out] voltage  point to wait single result, it can be NULL if not care
 * @return  < 0 error 
 */
int ead_read_adc_with_wait(double* voltage, DWORD dwMilliseconds)
{
    int ret;
    int fd = ad_fb;
	struct timeval tv; //store timeout 

    fd_set rfds;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
	if(dwMilliseconds == 0)
	    ret = select(fd + 1, &rfds, NULL, NULL, NULL);
	else {
		tv.tv_sec = dwMilliseconds/1000;
		tv.tv_usec = dwMilliseconds%1000 * 1000; 
	    ret = select(fd + 1, &rfds, NULL, NULL, &tv);
	}
	// 0: timeout 
	if(ret < 0)
		return ret;

    if (FD_ISSET(fd, &rfds))
    {
#if !TEST_NEW
        return ead_read_adc(voltage);
#endif
    }
	return -1;
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


