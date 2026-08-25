#include <stdio.h>
#include "drv_infrared.h"
#include "iot_gpio.h"

/*
 * 根据你的实际接线修改。
 *
 * 当前仓库已有修改：
 * GPIO0_PA3
 */
#define INFRARED_GPIO GPIO0_PA3

/*
 * 如果你的 PIR 模块：
 *
 * HIGH = 有人
 * LOW  = 无人
 *
 * 保持 1。
 *
 * 如果实际测试发现：
 *
 * HIGH = 无人
 * LOW  = 有人
 *
 * 改成 0。
 */
#define INFRARED_ACTIVE_HIGH 1

void infrared_dev_init(void)
{
    unsigned int ret;

    ret = IoTGpioInit(INFRARED_GPIO);
    if (ret != 0) {
        printf("[INFRARED] IoTGpioInit failed: %u\n", ret);
        return;
    }

    ret = IoTGpioSetDir(INFRARED_GPIO, IOT_GPIO_DIR_IN);
    if (ret != 0) {
        printf("[INFRARED] IoTGpioSetDir failed: %u\n", ret);
        return;
    }

    printf("[INFRARED] init success, gpio=%d\n", INFRARED_GPIO);
}

bool get_infrared_state(void)
{
    IotGpioValue value;
    unsigned int ret;

    ret = IoTGpioGetInputVal(INFRARED_GPIO, &value);
    if (ret != 0) {
        printf("[INFRARED] read failed: %u\n", ret);
        return false;
    }

#if INFRARED_ACTIVE_HIGH
    return value == IOT_GPIO_VALUE1;
#else
    return value == IOT_GPIO_VALUE0;
#endif
}
