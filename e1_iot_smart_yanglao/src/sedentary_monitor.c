#include <stdio.h>
#include <string.h>

#include "sedentary_monitor.h"

typedef struct {
    sedentary_state_t state;

    bool person_present;
    bool alarm;

    /*
     * 连续检测计数
     */
    uint32_t person_count;
    uint32_t absence_count;

    /*
     * 已经连续存在的秒数
     */
    uint32_t sitting_seconds;

    /*
     * MQTT周期上报计数
     */
    uint32_t report_seconds;

    /*
     * 状态是否发生变化
     */
    bool state_changed;
} sedentary_monitor_t;

static sedentary_monitor_t g_monitor;

void sedentary_monitor_init(void)
{
    memset(&g_monitor, 0, sizeof(g_monitor));

    g_monitor.state = SEDENTARY_STATE_NO_PERSON;
    g_monitor.person_present = false;
    g_monitor.alarm = false;

    printf("[SEDENTARY] monitor init\n");
}

void sedentary_monitor_update(bool person_present)
{
    /*
     * ==========================================================
     * 第一阶段：确认“有人”
     * ==========================================================
     */
    if (person_present) {
        g_monitor.absence_count = 0;

        if (g_monitor.person_count < PERSON_CONFIRM_COUNT) {
            g_monitor.person_count++;
        }

        /*
         * 连续检测到人体
         */
        if (!g_monitor.person_present &&
            g_monitor.person_count >= PERSON_CONFIRM_COUNT) {

            g_monitor.person_present = true;

            /*
             * 新的一次坐着开始
             */
            g_monitor.sitting_seconds = 0;
            g_monitor.report_seconds = 0;
            g_monitor.alarm = false;

            g_monitor.state = SEDENTARY_STATE_SITTING;
            g_monitor.state_changed = true;

            printf("[SEDENTARY] person detected\n");
        }
    }

    /*
     * ==========================================================
     * 第二阶段：确认“无人”
     * ==========================================================
     */
    else {
        g_monitor.person_count = 0;

        if (g_monitor.absence_count < ABSENCE_CONFIRM_COUNT) {
            g_monitor.absence_count++;
        }

        /*
         * 连续检测不到人体
         */
        if (g_monitor.person_present &&
            g_monitor.absence_count >= ABSENCE_CONFIRM_COUNT) {

            printf("[SEDENTARY] person left, sitting time=%u sec\n",
                   g_monitor.sitting_seconds);

            g_monitor.person_present = false;
            g_monitor.sitting_seconds = 0;
            g_monitor.report_seconds = 0;
            g_monitor.alarm = false;

            g_monitor.state = SEDENTARY_STATE_NO_PERSON;
            g_monitor.state_changed = true;
        }
    }

    /*
     * ==========================================================
     * 第三阶段：有人状态下累计时间
     * ==========================================================
     */
    if (g_monitor.person_present) {

        /*
         * 每次 update() 代表 1 秒
         */
        if (g_monitor.sitting_seconds < 0xFFFFFFFFU) {
            g_monitor.sitting_seconds++;
        }

        if (g_monitor.report_seconds < 0xFFFFFFFFU) {
            g_monitor.report_seconds++;
        }

        /*
         * ======================================================
         * 达到久坐阈值
         * ======================================================
         */
        if (!g_monitor.alarm &&
            g_monitor.sitting_seconds >= SEDENTARY_LIMIT_SEC) {

            g_monitor.alarm = true;

            g_monitor.state = SEDENTARY_STATE_LONG_SITTING;
            g_monitor.state_changed = true;

            printf("[SEDENTARY] WARNING! sitting time=%u sec\n",
                   g_monitor.sitting_seconds);
        }
    }
}

sedentary_state_t sedentary_monitor_get_state(void)
{
    return g_monitor.state;
}

bool sedentary_monitor_is_person_present(void)
{
    return g_monitor.person_present;
}

uint32_t sedentary_monitor_get_sitting_seconds(void)
{
    return g_monitor.sitting_seconds;
}

bool sedentary_monitor_is_alarm(void)
{
    return g_monitor.alarm;
}

bool sedentary_monitor_take_state_changed(void)
{
    bool changed = g_monitor.state_changed;

    g_monitor.state_changed = false;

    return changed;
}

bool sedentary_monitor_take_report_due(void)
{
    if (g_monitor.report_seconds >= SEDENTARY_REPORT_INTERVAL) {
        g_monitor.report_seconds = 0;
        return true;
    }

    return false;
}

const char *sedentary_monitor_get_state_string(void)
{
    switch (g_monitor.state) {

        case SEDENTARY_STATE_NO_PERSON:
            return "NO_PERSON";

        case SEDENTARY_STATE_SITTING:
            return "SITTING";

        case SEDENTARY_STATE_LONG_SITTING:
            return "LONG_SITTING";

        default:
            return "UNKNOWN";
    }
}
