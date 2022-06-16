/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "clock.h"
#include "ws2812.h"
#include "pwm.h"
#include "motor.h"
#include "ble.h"
#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"
#include "cw32f030_systick.h"
#include "cw32f030_spi.h"
#include "cw32f030_dma.h"
#include "cw32f030_flash.h"
#include "cw32f030_btim.h"
#include "LEGEND.H"
#include "fast_hsv2rgb.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/*
 * 流水灯
 */
void rolling() {
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;
    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER;
    BTIM_TimeBaseInitStruct.BTIM_Period = 800;
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV32768;

    BTIM_TimeBaseInit(CW_BTIM2, &BTIM_TimeBaseInitStruct);
    BTIM_ITConfig(CW_BTIM2, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM2, ENABLE);
    __RCC_BTIM_CLK_ENABLE();
    NVIC_EnableIRQ(BTIM2_IRQn);
}

volatile bool roll = false;

void BTIM2_IRQHandler(void) {
    /* USER CODE BEGIN */
    if (BTIM_GetITStatus(CW_BTIM2, BTIM_IT_OV)) {
        BTIM_ClearITPendingBit(CW_BTIM2, BTIM_IT_OV);
        roll = true;
    }
    /* USER CODE END */
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** Libraries里面是外设驱动库文件，用到什么就include对应的头文件，不要动
 ** 下载器为daplink，无需额外设置，直接点下载应该就能下。
 **
 ******************************************************************************/
int32_t main(void) {
    set_clock_64m();

    ble_init();
    pwm_init();
    motor_init();


    ws2812_init();
    uint8_t r, g, b;
    for (int i = 0; i < LED_NUMBER; ++i) {
        fast_hsv2rgb_8bit((HSV_HUE_MAX >> 3) * i, HSV_SAT_MAX, HSV_VAL_MAX / 4, &r,
                          &g, &b);
        ws2812_set_color((i) % LED_NUMBER, r, g, b);
    }
    rolling();

    while (1) {
        if (roll) {
            roll = false;
            uint8_t old[24];
            //TODO debug 一用memcpy串口就什么都收不到了 暂时用for循环去写
//            memcpy(old, ws2812_get_buffer(), 24);
//            memmove(ws2812_get_buffer() + 24, ws2812_get_buffer(), (LED_NUMBER - 1) * 24);
//            memcpy(ws2812_get_buffer() + (LED_NUMBER - 1) * 24, old, 24);
            for (int i = 0; i < 24; ++i) {
                old[i] = ws2812_get_buffer()[i];
            }
            for (int i = 0; i < LED_NUMBER - 1; ++i) {
                for (int j = 0; j < 24; ++j) {
                    ws2812_get_buffer()[i * 24 + j] = ws2812_get_buffer()[i * 24 + j + 24];
                }
            }
            for (int i = 0; i < 24; ++i) {
                ws2812_get_buffer()[(LED_NUMBER - 1) * 24 + i] = old[i];
            }
            ws2812_send_sync();
        }
        static uint8_t *ble_data;
        static uint16_t ble_data_len;
        if (ble_get_data(&ble_data, &ble_data_len)) {
            if (ble_grep_receiver(ble_data) == LEGEND_DEVICE_CURRENT) {
                switch (ble_grep_command(ble_data)) {
                    case LEGEND_CMD_SET_SPEED:
                        motor_set_speed(&ble_data[3], 2);
                        break;
                    case LEGEND_CMD_SET_LED: {
                        //3是通信协议的前三位 数据太少就跳过
                        if (ble_data_len < LED_NUMBER + 3) {
                            continue;
                        }
                        uint8_t *hue = &ble_data[3];
                        for (int i = 0; i < LED_NUMBER; ++i) {
                            fast_hsv2rgb_8bit((*(hue++)) * 6, HSV_SAT_MAX, HSV_VAL_MAX / 4, &r,
                                              &g, &b);
                            //应该不会有线程冲突吧
                            ws2812_set_color(i, r, g, b);
                        }
                        break;
                    }
                }
            }
        }

    }

}



/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

