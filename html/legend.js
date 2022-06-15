//组装控制命令
const SPEED_MAX = 0xFF


const LEGEND_DEVICE_KEY_SERVER = 0x00
const LEGEND_DEVICE_SUPER_CLOCK = 0x01
const LEGEND_DEVICE_UFO_VEHICLE = 0x02

const LEGEND_CMD_SET_TIME = 0x00
const LEGEND_CMD_GET_TIME = 0x01
const LEGEND_CMD_ADV_TEMPERATURE_HUMIDITY = 0x02
const LEGEND_CMD_SET_SPEED = 0x03
const LEGEND_CMD_SET_LED = 0x04

function build_speed_data(speed1, speed2) {
    let data = new Uint8Array(6);
    data[0] = 0xff
    data[1] = LEGEND_DEVICE_UFO_VEHICLE
    data[2] = LEGEND_CMD_SET_SPEED
    data[3] = 0
    data[4] = speed1 * SPEED_MAX
    data[5] = speed2 * SPEED_MAX
    return data;
}
//0-255 色调，设置8个led的颜色。
function build_led_data(led0, led1, led2, led3, led4, led5, led6, led7) {
    let data = new Uint8Array(11);
    data[0] = 0xff
    data[1] = LEGEND_DEVICE_UFO_VEHICLE
    data[2] = LEGEND_CMD_SET_LED
    data[3] = led0
    data[4] = led1
    data[5] = led2
    data[6] = led3
    data[7] = led4
    data[8] = led5
    data[9] = led6
    data[10] = led7
    return data;
}
