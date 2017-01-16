#include "gpio.h"


int gpio_fb = -1;  // 打开gpio驱动
// 打开驱动 返回true 打开成功 false 打开失败
bool Gpio_Init(void)
{
#if TEST_NEW
//	gpio_init()
	Gpio_UnInit();
	usleep(100*1000);   // 100ms
#if 1
	if(gpio_init() < 0) {
		g_System_Info = ERROR_GPIO_OPEN_ERROR;
		qDebug()<<"open hpio error@@@@@@@";
		return false;
	}
#endif
#else
    QFile file(GPIO_DRIVE_NAME);  //
    if(true == file.exists()) // 文件存在
    {
        gpio_fb = open(GPIO_DRIVE_NAME, O_RDWR);

        if(gpio_fb < 0)
        {
            g_System_Info = ERROR_GPIO_OPEN_ERROR;
            return false;
        }
    }
    else
    {
        g_System_Info = ERROR_GPIO_DRIVER_NOT_EXIST;
        return false;
    }
#endif
    return true;
}
//关闭 gpio驱动
bool Gpio_UnInit(void)
{
#if TEST_NEW
	gpio_deinit();

	return true;
#else
    close(gpio_fb);
    gpio_fb = -1;
#endif
}
// 设置 GPIO pin 值
bool Gpio_set(int pin, int val)
{
#if TEST_NEW
	gpio_set( pin, val);
#else
    static QMutex mutex;

    if((pin > GPIO_MAX_PIN)||(pin < 0))
    {
        return false;
    }

    if(gpio_fb < 0){
        return false;
    }

    struct io_cfg_t ct;
    ct.pin = pin;
    ct.type = val ? OUTPUT_HIGH : OUTPUT_LOW;
    mutex.lock();
    ioctl(gpio_fb, IOCTL_GPIO_CONFIG, &ct);
    mutex.unlock();
#endif
    return true;
}
// 获取pin 脚值
int Gpio_Get(int pin)
{
#if TEST_NEW
	return gpio_get( pin);
#else
    static QMutex mutex;

    if((pin > GPIO_MAX_PIN)||(pin < 0))
    {
        return -1;
    }

    if(gpio_fb < 0){
        return -1;
    }
    mutex.lock();
    // 先配置成 输入
    struct io_cfg_t ct;
    ct.pin = pin;
    ct.type = INPUT;
    ioctl(gpio_fb, IOCTL_GPIO_CONFIG, &ct);
    // 获取pin值
    struct io_st_t io;
    io.pin = pin;
    ioctl(gpio_fb, IOCTL_GPIO_IN, &io);
    mutex.unlock();

    return io.value;
#endif
}

