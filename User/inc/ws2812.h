//
// Created by Legend on 2022/5/28.
//
/*
 * cw32通过spi控制ws2812
 *
 * spi需要被设置成8m，参数采用8分频
 * 所以要求系统时钟位64m
 */

#ifndef CW32F030C8T6_WS2812_H
#define CW32F030C8T6_WS2812_H

#include <stdbool.h>
#include "main.h"
#include "cw32f030_gpio.h"
#include "cw32f030_spi.h"
#include "cw32f030_dma.h"
#include "cw32f030_flash.h"
#include "cw32f030_rcc.h"

//使用cw32 spi1控制ws2812
//从fsp中配置spi0，速率设为8M，MSB,并自动生成代码
//SPI1——MOSI（PB15） 接ws2812


//LED 灯珠数量 不要超内存
#define LED_NUMBER 1
//显存需要TOTAl个字节的内存
#define TOTAL   LED_NUMBER * 24


//spi模拟0码 1100 0000
#define CODE0  0XC0
//spi模拟1码 1111 1100
#define CODE1  0xFC

extern volatile bool ws2812_busy;


void ws2812_init();

void ws2812_set_color(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

void ws2812_fill(uint8_t r, uint8_t g, uint8_t b);

void ws2812_red();

void ws2812_green();

void ws2812_blue();

void ws2812_black();

void ws2812_copy_to_buffer(uint8_t *source, uint16_t size);

uint8_t *ws2812_get_buffer();

bool ws2812_is_busy();

void ws2812_send_sync();


#endif //CW32F030C8T6_WS2812_H
