//
// Created by Legend on 2022/6/5.
//

#ifndef CW32F030C8T6_BLE_H
#define CW32F030C8T6_BLE_H
#include "main.h"
#include "cw32f030_rcc.h"
#include "cw32f030_gpio.h"
#include "cw32f030_uart.h"
#include "cw32f030_btim.h"
#include "stdbool.h"

void ble_init();
void ble_send_sync(uint8_t*buffer,uint16_t len);
bool ble_get_data(uint8_t **buffer,uint16_t *len);
uint8_t ble_grep_sender(uint8_t* data);
uint8_t ble_grep_receiver(uint8_t* data);
uint8_t ble_grep_command (uint8_t* data);

#endif //CW32F030C8T6_BLE_H
