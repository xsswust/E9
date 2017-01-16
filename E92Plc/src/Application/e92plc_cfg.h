#ifndef E92PLC_CFG_H
#define E92PLC_CFG_H
#include "common.h"
// 接口板 输入定义
#define MAIN_GPIO_OUT0	GPIO_G2_P0   //输入 0 接口
#define MAIN_GPIO_OUT1	GPIO_G2_P1   //输入 1 接口
#define MAIN_GPIO_OUT2	GPIO_G2_P2   //输入 2 接口
#define MAIN_GPIO_OUT3	GPIO_G2_P3   //输入 3 接口
#define MAIN_GPIO_OUT4	GPIO_G2_P4   //输入 4 接口
#define MAIN_GPIO_OUT5	GPIO_G2_P5   //输入 5 接口
#define MAIN_GPIO_OUT6	GPIO_G2_P6   //输入 6 接口
#define MAIN_GPIO_OUT7	GPIO_G2_P7   //输入 7 接口
// 接口板输入 定义

#define MAIN_GPIO_IN0	GPIO_G6_P7   // 输入0
#define MAIN_GPIO_IN1	GPIO_G6_P8   // 输入1
#define MAIN_GPIO_IN2	GPIO_G6_P9   // 输入2
#define MAIN_GPIO_IN3	GPIO_G6_P10   // 输入3
#define MAIN_GPIO_IN4	GPIO_G6_P11   // 输入4
#define MAIN_GPIO_IN5	GPIO_G6_P14   // 输入5
#define MAIN_GPIO_IN6	GPIO_G6_P15   // 输入6
#define MAIN_GPIO_IN7	GPIO_G6_P16   // 输入7

// 扩展口
#define OUT_SDI			GPIO_G1_P16  //  扩展输入 data
#define OUT_CLK			GPIO_G1_P20   // 扩展 输出 clk
#define OUT_LE			GPIO_G3_P23   // 扩展 输出 LE
#define IN_CLK			GPIO_G3_P30   // 扩展 输出 输入 clk
#define IN_DATA			GPIO_G4_P5   // 扩展 输出 输入 data
#define IN_LE			GPIO_G3_P31   // 扩展 输出 输入 EN


#endif // E92PLC_CFG_H
