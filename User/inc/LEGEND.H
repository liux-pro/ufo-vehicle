/*
 * LEGEND.H
 * 统一管理设备编号，以及控制命令
 * Author: Legend
 */

#ifndef LEGEND_H_
#define LEGEND_H_

#define LEGEND_DEVICE_CURRENT LEGEND_DEVICE_UFO_VEHICLE

#define LEGEND_DEVICE_KEY_SERVER 0x00
#define LEGEND_DEVICE_SUPER_CLOCK 0x01
#define LEGEND_DEVICE_UFO_VEHICLE 0x02


#define LEGEND_CMD_SET_TIME 0x00
#define LEGEND_CMD_GET_TIME 0x01
#define LEGEND_CMD_ADV_TEMPERATURE_HUMIDITY 0x02
//第一个字节按位表示正反转，后边字节表示每个每个电机的速度。
#define LEGEND_CMD_SET_SPEED 0x03
//0-255 色调，设置8个led的颜色。
#define LEGEND_CMD_SET_LED 0x04

#endif /* LEGEND_H_ */