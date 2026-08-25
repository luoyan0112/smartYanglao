#ifndef __DRV_INFRARED_H__
#define __DRV_INFRARED_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 初始化人体红外传感器
 */
void infrared_dev_init(void);

/**
 * 获取人体红外原始状态
 *
 * 返回：
 * true  - 检测到人体
 * false - 未检测到人体
 */
bool get_infrared_state(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_INFRARED_H__ */
