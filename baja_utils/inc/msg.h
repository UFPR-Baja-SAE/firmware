#ifndef MSG_H
#define MSG_H

typedef enum MSG_TYPES {
    MSG_RPM,
    MSG_FUEL,
    MSG_VELOCITY,
    MSG_BRAKES,
    MSG_WARNING,
    MSG_ERROR
} MSG_TYPES;

typedef enum ERROR_TYPES {
    ERROR_CAN_QUEUE_FULL = 100,
    ERROR_NO_RPM,
    ERROR_NO_BATTERY,
    ERROR_NO_FUEL,
    ERROR_NO_VELOCITY,
} ERROR_TYPES;

typedef enum WARNING_TYPES {
    WARNING_RPM_HIGH = 200,
    WARNING_RPM_LOW,
    WARNING_BATTERY,
    WARNING_FUEL,
    WARNING_VELOCITY_HIGH,
    WARNING_VELOCITY_LOW
} WARNING_TYPES;

#endif