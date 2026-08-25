#ifndef _IOT_H_
#define _IOT_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    double illumination;
    double temperature;
    double humidity;

    bool motor_state;
    bool light_state;
    bool auto_state;

    /*
     * 人体/久坐监测
     */
    bool person_present;
    uint32_t sitting_seconds;
    bool sedentary_alarm;

    /*
     * NO_PERSON
     * SITTING
     * LONG_SITTING
     */
    const char *sedentary_status;

} e_iot_data;

#define IOT_CMD_LIGHT_ON  0x01
#define IOT_CMD_LIGHT_OFF 0x02
#define IOT_CMD_MOTOR_ON  0x03
#define IOT_CMD_MOTOR_OFF 0x04
#define IOT_CMD_AUTO_ON   0x05
#define IOT_CMD_AUTO_OFF  0x06

int wait_message(void);
void mqtt_init(void);
unsigned int mqtt_is_connected(void);

void send_msg_to_mqtt(e_iot_data *iot_data);

#endif
