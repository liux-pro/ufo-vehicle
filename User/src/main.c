/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"
#include "cw32f030_systick.h"
#include "cw32f030_spi.h"
#include "cw32f030_dma.h"
#include "cw32f030_flash.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

#define LED_GPIO_PORT CW_GPIOC
#define LED_GPIO_PINS GPIO_PIN_13

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
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    uint16_t counter = 0;

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



    //尝试初始化spi，控制ws2812

    //打开各种总线，貌似cw32要用任何功能都要先打开它对应的总线，但是示例代理管这个叫打开时钟。爱叫什么叫什么把
    // AHB   内部总线
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_DMA_CLK_ENABLE();
    // APB  外设总线
    __RCC_SPI1_CLK_ENABLE();
    // AF 配置pin端口复用
    PB12_AFx_SPI1CS();
    PB13_AFx_SPI1SCK();
    PB14_AFx_SPI1MISO();
    PB15_AFx_SPI1MOSI();
    //设置gpio的推挽或者开漏，以适应spi，这一步为什么不设计成自动的？
    //时钟线
    GPIO_InitStructure.Pins = GPIO_PIN_13;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    //mosi
    GPIO_InitStructure.Pins = GPIO_PIN_15;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    //miso
    GPIO_InitStructure.Pins = GPIO_PIN_14;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);


    //配置SPI1 DMA TX
    DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
    DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
    DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Increase;
    DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Fix;
    DMA_InitStructure.TrigMode = DMA_HardTrig;
    DMA_InitStructure.HardTrigSource = DMA_HardTrig_SPI1_TXBufferE;
    DMA_InitStructure.DMA_TransferCnt = 1024;
    DMA_InitStructure.DMA_SrcAddress = (uint32_t) spi_buffer;
    DMA_InitStructure.DMA_DstAddress = (uint32_t) &CW_SPI1->DR;
    DMA_Init(CW_DMACHANNEL1, &DMA_InitStructure);
    DMA_Cmd(CW_DMACHANNEL1, ENABLE);


    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    // 双线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // 帧数据长度为8bit
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            // 时钟空闲电平为低
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          // 第一个边沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // 片选信号由SSI寄存器控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;    // 波特率为PCLK的8分频。64/8=8M
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // 最高有效位 MSB 收发在前
    SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // 低速SPI

    SPI_Init(CW_SPI1, &SPI_InitStructure);
    SPI_Cmd(CW_SPI1, ENABLE);

    while (counter < sizeof(spi_buffer)) {
        while (SPI_GetFlagStatus(CW_SPI1, SPI_FLAG_TXE) == RESET);
        SPI_SendData(CW_SPI1, spi_buffer[counter++]);
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

