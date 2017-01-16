#ifndef PWM
#define PWM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "common.h"
//#include <ioctl.h>


#define PWM_DRIVE_NAME_ST		"/sys/class/pwm/pwmchip"   // pwm 驱动位置
//int pwm_init(int num, int period, int duty, int enbal);
int pwm_init(int num, int period, int duty, int enbal);
#endif // PWM

