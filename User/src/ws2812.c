//
// Created by Legend on 2022/5/28.
//
#include "ws2812.h"

//buffer 显存 GRB888
static uint8_t buffer[TOTAL] = {0};

//记录信号是否发完
volatile bool ws2812_busy = false;


//初始化spi，参数已经自动生成了，所以这里没参数
void ws2812_init() {
    //初始化spi
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
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
    DMA_InitStructure.DMA_SrcAddress = (uint32_t) buffer;
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
    //初始化显存
    ws2812_black();
}

void ws2812_fill(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < LED_NUMBER; i++) {
        ws2812_set_color(i, r, g, b);
    }
}

void ws2812_red() {
    ws2812_fill(255, 0, 0);
}

void ws2812_green() {
    ws2812_fill(0, 255, 0);
}

void ws2812_blue() {
    ws2812_fill(0, 0, 255);
}

void ws2812_black() {
    ws2812_fill(0, 0, 0);
}

//void ws2812_fill_color(uint8_t r, uint8_t g, uint8_t b)
//{
//    for (uint16_t i = 0; i < LED_NUMBER; i++)
//    {
//
//    }
//}

void ws2812_set_color(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t *p_buffer = &buffer[n * 24];

    for (int mask = 1 << 7; mask > 0; mask = mask >> 1) {
        if ((g & mask) > 0) {
            *p_buffer = CODE1;
        } else {
            *p_buffer = CODE0;
        }
        if ((r & mask) > 0) {
            *(p_buffer + 8) = CODE1;
        } else {
            *(p_buffer + 8) = CODE0;
        }
        if ((b & mask) > 0) {
            *(p_buffer + 16) = CODE1;
        } else {
            *(p_buffer + 16) = CODE0;
        }
        p_buffer++;
    }

}


void ws2812_copy_to_buffer(uint8_t *source, uint16_t size) {
    memcpy(buffer, source, size);
}

uint8_t *ws2812_get_buffer() {
    return buffer;
}


//同步发送颜色数据
void ws2812_send_sync() {
    uint16_t counter = 0;
    while (counter < sizeof(buffer)) {
        while (SPI_GetFlagStatus(CW_SPI1, SPI_FLAG_TXE) == RESET);
        SPI_SendData(CW_SPI1, buffer[counter++]);
    }
}

bool ws2812_is_busy() {
    return ws2812_busy;
}