#ifndef GPIO
#define GPIO
#include <stdint.h>
///#include "ioctl.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "common.h"
#include "libFoo.h"
//ioctl.h

#define TEST_NEW	1  // 2015-1-4 F501 主板

#ifdef __cplusplus
extern "C" {
#endif


/** use gpio, first gpio_init, and set state use gpio_set
 * get gpio state use gpio_get.
 *
 * exampe use GPIO
 *
 * gpio_init();
 * gpio_set(0, 0);
 * gpio_set(0, 1);
 * gpio_get(3);
 * gpio_deinit();
 *
 * watch gpio input
 *
 * char input[40]
 * gpio_init();
 * gpio_read(input, sizeof(input), NULL);
 * gpio_deinit();
 *
 * the gpio index from 0.
 * when finished, use gpio_deinit
 */



// 目前可以用的IO
// 干接点输入
#define GPIO_G6_P7      0   // group6 pin7
#define GPIO_G6_P8      1   // group6 pin8
#define GPIO_G6_P9      2   // group6 pin9
#define GPIO_G6_P10     3   // group6 pin10
#define GPIO_G6_P11     4   // group6 pin11
#define GPIO_G6_P14     5    // group6 pin14
#define GPIO_G6_P15     6   // group6 pin15
#define GPIO_G6_P16     7   // group6 pin16
// 干接点输出
#define GPIO_G2_P0     8   // group2 pin0
#define GPIO_G2_P1     9   // group2 pin1
#define GPIO_G2_P2     10   // group2 pin2
#define GPIO_G2_P3     11   // group2 pin3
#define GPIO_G2_P4     12   // group2 pin4
#define GPIO_G2_P5     13   // group2 pin5
#define GPIO_G2_P6     14   // group2 pin6
#define GPIO_G2_P7     15   // group2 pin7
// 普通GPIO
#define GPIO_G1_P16     16   // group1 pin16
#define GPIO_G1_P20     17   // group1 pin20

#define GPIO_G3_P23     18   // group3 pin23
#define GPIO_G3_P30     19   // group3 pin30
#define GPIO_G3_P31     20   // group3 pin31

#define GPIO_G4_P5     21   // group4 pin5

#define GPIO_G5_P2     22   // group5 pin2
#define GPIO_G5_P19     23   // group5 pin19
#define GPIO_G5_P18     24   // group5 pin18
#define GPIO_G5_P21     25   // group5 pin21

#define GPIO_G6_P17     26   // group6 pin17
#define GPIO_G6_P18     27   // group6 pin18
#define GPIO_G6_P31     28   // group6 pin31

#define GPIO_G1_P28     29   // group1 pin28

#define GPIO_G4_P10     30   // group4 pin10

#define GPIO_G1_P27     31   // group1 pin27
#define GPIO_G1_P30     32   // group1 pin30

#define GPIO_G4_P20     33   // group4 pin20

#define GPIO_G7_P0     34   // group7 pin0
#define GPIO_G7_P1     35   // group7 pin1
#define GPIO_G7_P7     36   // group7 pin7
#define GPIO_G7_P12     37   // group7 pin12
#define GPIO_G7_P13     38   // group7 pin13

#define GPIO_MAX_PIN	31  // 最大只有31个pin 可用

#define TEST            1

#define AD_CHANNELG_0     0   //  channel 0
#define AD_CHANNELG_1     1   //  channel 1
#define AD_CHANNELG_2     2   //  channel 2
#define AD_CHANNELG_3     3   //  channel 3
#define AD_CHANNELG_4     4   //  channel 4
#define AD_CHANNELG_5     5   //  channel 5
#define AD_CHANNELG_6     6   //  channel 6
#define AD_CHANNELG_7     7   //  channel 7

 // GPIO 定义
#define GPIO_IOC_MAGIC 'k'
#define IOCTL_GPIO_IN                   _IO(GPIO_IOC_MAGIC, 1 )
#define IOCTL_GPIO_OUT                  _IO(GPIO_IOC_MAGIC, 2 )
#define IOCTL_GPIO_CONFIG				_IO(GPIO_IOC_MAGIC, 3 )

#define INPUT           0
#define OUTPUT_HIGH     0xf1
#define OUTPUT_LOW      0xf0


#define UINT16 unsigned short

struct io_st_t
{
    UINT16 pin;
    UINT16 value;
};

struct io_cfg_t {
    UINT16 pin;
    UINT16 type;
};

// 打开驱动 返回true 打开成功 false 打开失败
bool Gpio_Init(void);
//关闭 gpio驱动
bool Gpio_UnInit(void);
// 获取pin 脚值
int gpio_get(int pin);
// 设置 GPIO pin 值
bool Gpio_set(int pin, int val);

#if 0
/** initial gpio
 * @return less 0 if failed
 */
int gpio_init(void);

/** deinitial gpio
 */
int gpio_deinit(void);
/** set gpio state
 * @param[in] num  gpio index from 0..MAX
 * @param[in] value 1 set gpio high, and 0 set gpio low
 *
 * @return 0 if successfully
 */
int gpio_set(int num, int value);

/** set gpio state
 * @param[in] num  gpio index from 0..MAX
 *
 * @return 1 if gpio high, 0 if gpio low
 */
int gpio_get(int num);

/* read gpio value
 * gpio_read will block till input change or timeout.
 *
 * param[in] buf point to io value
 * @param[in] milliseconds milli seconds to wait, 0 wait forever
 * @return  length of read. less 0 indication error
 */
int gpio_read(char *buf, int len, int milliseconds);
#endif

#ifdef __cplusplus
}
#endif

#endif // GPIO

