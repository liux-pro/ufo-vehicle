/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "ws2812.h"
#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"
#include "cw32f030_systick.h"
#include "cw32f030_spi.h"
#include "cw32f030_dma.h"
#include "cw32f030_flash.h"
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


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** 基于官方示例,制作本本模板，带所有外设驱动；整个项目编码转为utf-8。
 ** cw32f030_iwdt.c 编译报错，不过我用不到，不去管他了
 ** Starup里面是启动文件吧，不太懂，不要动就好
 ** Libraries里面是外设驱动库文件，用到什么就include对应的头文件，不要动
 ** User里面是代码，main.c里写程序。interrupt_cw32f030.c里面是所有的中断函数
 ** 示例电灯程序，PC13的LED闪烁。无需外部晶振。
 ** 下载器为daplink，无需额外设置，直接点下载应该就能下。
 **
 ******************************************************************************/

//spi模拟0码 1100 0000
#define CODE0  0XC0
//spi模拟1码 1111 1100
#define CODE1  0xFC

uint8_t spi_buffer[1024] = {
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,

        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,

        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,

        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,

        CODE1, CODE1, CODE1, CODE1, CODE1, CODE1, CODE1, CODE1,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,

        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE1, CODE1, CODE1, CODE1, CODE1, CODE1, CODE1, CODE1,


        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0,
        CODE1, CODE1, CODE1, CODE1, CODE1, CODE1, CODE1, CODE1,
        CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0, CODE0

};
// uint8_t spi_buffer[1024] = {
// 0xAA

// };



int32_t main(void) {
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




    ws2812_init();
    ws2812_red();
    ws2812_blue();
    ws2812_send_sync();


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

