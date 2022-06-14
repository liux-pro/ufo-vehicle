//
// Created by Legend on 2022/5/28.
//
/*
 * 系统主频需为64m，pwm频率10k
 * 初始化两个pwm
 * PWM1 = GTIM1_CH4 PB1
 * PWM2 = GTIM2_CH1 PA5
 */

#include "pwm.h"

#define COUNT 6400
#define SINGLE (COUNT>>8)

void pwm_init(){
    //开启GPIO A 总线
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();


    PB01_AFx_GTIM1CH4();
    PB01_DIGTAL_ENABLE();
    PB01_DIR_OUTPUT();
    PB01_PUSHPULL_ENABLE();
    PB01_SPEED_HIGH();

    PA05_AFx_GTIM2CH1();
    PA05_DIGTAL_ENABLE();
    PA05_DIR_OUTPUT();
    PA05_PUSHPULL_ENABLE();
    PA05_SPEED_HIGH();



    GTIM_InitTypeDef GTIM_InitStruct;
    GTIM_InitStruct.Mode = GTIM_MODE_TIME;
    GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
    GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;
    //pwm一个周期6400个定时器count
    GTIM_InitStruct.ReloadValue = COUNT;
    GTIM_InitStruct.ToggleOutState = DISABLE;

    /*
     * GTIM_OC_OUTPUT_PWM_HIGH   用户手册 14.3.4.1 章节
     */

    __RCC_GTIM1_CLK_ENABLE();
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL4, GTIM_OC_OUTPUT_PWM_LOW);
    GTIM_SetCompare1(CW_GTIM1, 0);
    GTIM_Cmd(CW_GTIM1, ENABLE);

    __RCC_GTIM2_CLK_ENABLE();
    GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);
    GTIM_OCInit(CW_GTIM2, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_LOW);
    GTIM_SetCompare1(CW_GTIM2, 0);
    GTIM_Cmd(CW_GTIM2, ENABLE);

    pwm1_set_duty(0);
    pwm2_set_duty(0);
}

void pwm1_set_duty(uint8_t duty){
    GTIM_SetCompare4(CW_GTIM1, SINGLE*duty);
}

void pwm2_set_duty(uint8_t duty){
    GTIM_SetCompare1(CW_GTIM2, SINGLE*duty);
}