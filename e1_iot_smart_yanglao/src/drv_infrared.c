#include "drv_infrared.h"
#include "iot_gpio.h"

#define INFRARED_GPIO GPIO0_PA3 

//红外传感初始化
void infrared_dev_init(void) {
    IoTGpioInit(INFRARED_GPIO);
    IoTGpioSetDir(INFRARED_GPIO, IOT_GPIO_DIR_IN); // 设置为输入模式
}

//获取是否有人
bool get_infrared_state(void) {
    IotGpioValue value;
    IoTGpioGetInputVal(INFRARED_GPIO, &value);
    return (value == IOT_GPIO_VALUE1); // 假设高电平表示有人
}