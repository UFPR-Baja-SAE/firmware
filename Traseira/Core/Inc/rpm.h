#include "msg_types.h"
#include "stdint.h"
typedef struct rpm_msg {
    MSG_TYPE type;
    uint32_t rpm;
} rpm_msg;