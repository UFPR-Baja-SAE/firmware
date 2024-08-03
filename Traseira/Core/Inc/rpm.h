#include "msg_types.h"
#include "stdint.h"
#define RPM_ITR_FLAG 0x0000001

typedef struct rpm_msg {
    MSG_TYPE type;
    uint32_t rpm;
} rpm_msg;