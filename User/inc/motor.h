//
// Created by Legend on 2022/6/14.
//

#ifndef CW32F030C8T6_MOTOR_H
#define CW32F030C8T6_MOTOR_H
#include "main.h"
#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"
#include "pwm.h"
void motor_init();
void motor_set_speed(uint8_t *speed_data, uint16_t len);
#endif //CW32F030C8T6_MOTOR_H
