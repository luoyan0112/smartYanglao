#ifndef __SEDENTARY_MONITOR_H__
#define __SEDENTARY_MONITOR_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEDENTARY_LIMIT_SEC       30U
#define PERSON_CONFIRM_COUNT      3U
#define ABSENCE_CONFIRM_COUNT     3U
#define SEDENTARY_REPORT_INTERVAL 10U

typedef enum {
    SEDENTARY_STATE_NO_PERSON = 0,
    SEDENTARY_STATE_SITTING,
    SEDENTARY_STATE_LONG_SITTING
} sedentary_state_t;

void sedentary_monitor_init(void);

void sedentary_monitor_update(bool person_present);

sedentary_state_t sedentary_monitor_get_state(void);

bool sedentary_monitor_is_person_present(void);

uint32_t sedentary_monitor_get_sitting_seconds(void);

bool sedentary_monitor_is_alarm(void);

bool sedentary_monitor_take_state_changed(void);

bool sedentary_monitor_take_report_due(void);

const char *sedentary_monitor_get_state_string(void);

#ifdef __cplusplus
}
#endif

#endif
