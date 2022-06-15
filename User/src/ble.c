//
// Created by Legend on 2022/6/5.
//

/*
 * 通过串口与蓝牙芯片ch571k通讯。
 *
 * 定时器超时时间设为n毫秒（这里实际设置为n=1），超时后计时器停止
 *
 * 串口接收数据时，收到第一个数据时启动定时器，以后只要接到数据，
 * 就重置定时器，保证定时器在接收数据时一直不会溢出。
 * 超过n毫秒没收到数据后，定时器就会溢出进到定时器中断。
 * 代表接收完成。
 *
 *     PA02 UART3 TXD
 *     PA03 UART3 RXD
 *     115200
 *     64M pclock
 *
 */

#include "ble.h"

#define BAUD_RATE 115200

//定义缓冲区大小，256字节+1个校验位
#define BLE_RECEIVE_BUFFER (256+1)
#define BLE_SEND_BUFFER (256+1)


uint8_t ble_receive_buffer[BLE_RECEIVE_BUFFER];
uint8_t ble_send_buffer[BLE_SEND_BUFFER];


volatile bool ble_receiving = false;
volatile uint16_t ble_receive_count = 0;

volatile bool ble_received = false;


uint8_t calc_xor(uint8_t *data, uint16_t len);

void ble_send_sync(uint8_t *buffer, uint16_t len) {
    if (buffer != ble_send_buffer) {
        memcpy(ble_send_buffer, buffer, len);
    }
    ble_send_buffer[len] = calc_xor(ble_send_buffer, len);
    uint8_t *p = ble_send_buffer;
    for (int i = 0; i < len + 1; ++i) {
        USART_SendData_8bit(CW_UART3, ble_send_buffer[i]);
        while (USART_GetFlagStatus(CW_UART3, USART_FLAG_TXE) == RESET);
    }
    while (USART_GetFlagStatus(CW_UART3, USART_FLAG_TXBUSY) == SET);
}

//初始化定时器并且装填，启动
void timer_reset(){
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;
    // 1ms
    BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TIMER;
    BTIM_TimeBaseInitStruct.BTIM_Period = 1000;
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV64;

    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);
}

//关闭定时器
void timer_stop(){
    BTIM_Cmd(CW_BTIM1, DISABLE);
}


void ble_init() {
    /*
     * 初始化串口。uart3
     */
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_UART3_CLK_ENABLE();
    PA02_AFx_UART3TXD();
    PA03_AFx_UART3RXD();


    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);


    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = BAUD_RATE;
    USART_InitStructure.USART_Over = USART_Over_16;
    USART_InitStructure.USART_Source = USART_Source_PCLK;
    USART_InitStructure.USART_UclkFreq = RCC_Sysctrl_GetHClkFreq();
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(CW_UART3, &USART_InitStructure);
    //开启串口中断
    NVIC_SetPriority(UART3_IRQn, 0);
    NVIC_EnableIRQ(UART3_IRQn);
    //开启接收中断
    USART_ITConfig(CW_UART3, USART_IT_RC, ENABLE);

    /*
     * 初始化定时器btim1
     */
    __RCC_BTIM_CLK_ENABLE();
    NVIC_EnableIRQ(BTIM1_IRQn);
}



/**
 * @brief This funcation handles BTIM1
 */
void BTIM1_IRQHandler(void) {
    /* USER CODE BEGIN */
    //关闭定时器
    timer_stop();
    //定时器超时了，代表接收完成
    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV)) {
        //清除定时器中断标志位
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
        //标记接收完成
        ble_receiving = false;
        //进行异或校验。
        //发的时候把所有数据异或运算结果拼在数据最后
        //校验的时候把所有数据和校验值一起异或，结果应该是0
        if (calc_xor(ble_receive_buffer, ble_receive_count) == 0) {
            ble_received = true;
        }
    }
    /* USER CODE END */
}



//主循环轮询获取数据。
bool ble_get_data(uint8_t **buffer, uint16_t *len) {
    if (ble_received) {
        //被读取后自动清除标志
        ble_received = false;
        *buffer = ble_receive_buffer;
        //减去校验位。
        *len = ble_receive_count - 1;
        return true;
    };
    return false;
}


/**
 * @brief This funcation handles UART3
 */
void UART3_IRQHandler(void) {
    /* USER CODE BEGIN */

    //收到一个字节
    if (USART_GetITStatus(CW_UART3, USART_IT_RC) != RESET) {
        uint8_t byte;
        byte = USART_ReceiveData_8bit(CW_UART3);
        USART_ClearITPendingBit(CW_UART3, USART_IT_RC);

        if (ble_receiving) {    //正在接收，以前已经收到过数据了
            //能到这里接收计数应该大于0
            //接收不能超过缓冲区。
            if (ble_receive_count > 0 &&
                ble_receive_count < BLE_RECEIVE_BUFFER) {
                ble_receive_buffer[ble_receive_count++] = byte;
                //重置定时器
                timer_reset();
            } else {
                //正常应该不会走到这里
                //除非发的数据超长，挤爆buffer
                ble_receiving = false;
                ble_receive_count = 0;
                timer_stop();
            }
        } else {       //收到第一个数据
            //标记数据接收开始。
            ble_receiving = true;
            //开启定时器
            timer_reset();
            //初始化接收计数器
            ble_receive_count = 0;
            //保存这第一个数据到缓冲区
            ble_receive_buffer[ble_receive_count++] = byte;
        }
    }
    /* USER CODE END */
}


//异或校验
uint8_t calc_xor(uint8_t *data, uint16_t len) {
    uint8_t result = data[0];
    for (uint16_t i = 1; i < len; i++) {
        result ^= data[i];
    }
    return result;
}

uint8_t ble_grep_sender(uint8_t *data) {
    return data[0];
}

uint8_t ble_grep_receiver(uint8_t *data) {
    return data[1];
}

uint8_t ble_grep_command(uint8_t *data) {
    return data[2];
}