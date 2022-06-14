//
// Created by Legend on 2022/6/14.
//
/*
 * 高低电平控制方向
 *   方向控制1   FR1   PB11
 *   方向控制2   FR2   PA4
 */
#include "motor.h"

#define FR1_GPIO CW_GPIOB
#define FR1_PIN GPIO_PIN_11
#define FR2_GPIO CW_GPIOA
#define FR2_PIN GPIO_PIN_4

void motor_init(){
    GPIO_InitTypeDef GPIO_InitStructure;


    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();


    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

    GPIO_InitStructure.Pins = FR1_PIN;
    GPIO_Init(FR1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = FR2_PIN;
    GPIO_Init(FR2_GPIO, &GPIO_InitStructure);

}


//第一个字节按位表示正反转，后边字节表示每个每个电机的速度。
void motor_set_speed(uint8_t *speed_data, uint16_t len){
    //取最低位置，~按位取反，因为两个电机是反装的，所以要把其中一个方向控制取反，方便控制端写代码
    GPIO_WritePin(FR1_GPIO,FR1_PIN,(~speed_data[0])|0x01);
    GPIO_WritePin(FR2_GPIO,FR2_PIN,(speed_data[0]|0x02)>>1);

    pwm1_set_duty(speed_data[1]);
    pwm2_set_duty(speed_data[2]);
}