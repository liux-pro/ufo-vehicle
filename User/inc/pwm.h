//
// Created by Legend on 2022/5/28.
//

#ifndef CW32F030C8T6_PWM_H
#define CW32F030C8T6_PWM_H
#include "main.h"
#include "cw32f030_gtim.h"
#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"

void pwm_init();
/*
 * 设置占空比，0-255
 */
void pwm1_set_duty(uint8_t duty);
/*
 * 设置占空比，0-255
 */
void pwm2_set_duty(uint8_t duty);
#endif //CW32F030C8T6_PWM_H
