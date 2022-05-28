//
// Created by Legend on 2022/5/28.
//
/*
 * 调整系统时钟到64m,使用内部rc，不用外部晶振
 */
#include "clock.h"

void set_clock_64m(){
    //貌似会自动初始flash总线，这行不要也行
    RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_FLASH, ENABLE);
    //flash等会频率会上到64m，太快flash反应不过来，加这句表示加点延迟,等等flash，
    FLASH_SetLatency(FLASH_Latency_3);

    //内部rc振荡器是48M，3分频，到16M
    RCC_HSI_Enable(RCC_HSIOSC_DIV3);

    //用pll把内部rc频率提升4倍，到64M
    RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 16000000, RCC_PLL_MUL_4);    //开启PLL，PLL源为HSI
    //以下从内部时钟源HSI切换到PLL
    RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);                    //切换系统时钟到PLL
}

