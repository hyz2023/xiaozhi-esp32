#pragma once

#include "driver/gpio.h"

// LCD RGB 引脚定义
#define JC8048W550_LCD_HSYNC    (gpio_num_t)39
#define JC8048W550_LCD_VSYNC    (gpio_num_t)41
#define JC8048W550_LCD_DE       (gpio_num_t)40
#define JC8048W550_LCD_PCLK     (gpio_num_t)42
#define JC8048W550_LCD_BL       (gpio_num_t)2  // 背光控制

// LCD RGB 数据引脚
#define JC8048W550_LCD_R0       (gpio_num_t)45
#define JC8048W550_LCD_R1       (gpio_num_t)48
#define JC8048W550_LCD_R2       (gpio_num_t)47
#define JC8048W550_LCD_R3       (gpio_num_t)21
#define JC8048W550_LCD_R4       (gpio_num_t)14

#define JC8048W550_LCD_G0       (gpio_num_t)5
#define JC8048W550_LCD_G1       (gpio_num_t)6
#define JC8048W550_LCD_G2       (gpio_num_t)7
#define JC8048W550_LCD_G3       (gpio_num_t)15
#define JC8048W550_LCD_G4       (gpio_num_t)16
#define JC8048W550_LCD_G5       (gpio_num_t)4

#define JC8048W550_LCD_B0       (gpio_num_t)8
#define JC8048W550_LCD_B1       (gpio_num_t)3
#define JC8048W550_LCD_B2       (gpio_num_t)46
#define JC8048W550_LCD_B3       (gpio_num_t)9
#define JC8048W550_LCD_B4       (gpio_num_t)1

// 其他引脚定义
// #define JC8048W550_BOOT_KEY     (gpio_num_t)2  // 暂时注释掉按钮定义
#define JC8048W550_STATUS_LED   (gpio_num_t)46 