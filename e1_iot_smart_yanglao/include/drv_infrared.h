#ifndef __DRV_INFRARED_H__
#define __DRV_INFRARED_H__

#include "stdbool.h"

void infrared_dev_init(void);
bool get_infrared_state(void); // true表示检测到人，false表示无人

#endif