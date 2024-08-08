#ifndef MSG_H
#define MSG_H

//ADC1 = battery and fuel
//ADC2 = front and rear break pressure
//ADC2 will eventually be removed since those sensors will go on the front pcb

typedef enum MSG_TYPES {
    MSG_RPM,
    MSG_ADC1,
    MSG_ADC2,
    MSG_VELOCITY,
    MSG_TEMPERATURE,
    MSG_WARNING,
    MSG_ERROR
} MSG_TYPES;

typedef enum MSG_FREQ {
    FREQ_ITR = 75,
    FREQ_POLLING,
    FREQ_COMMS
} MSG_FREQ;

//just put this in osDelay(ms)
//not a good idea to send stuff at 100hz or more I think, should test this out
//can also just send a delay value directly, this is made so it is more legible

typedef enum FREQ_VALUES {
    FREQ_1_HZ = 1000,
    FREQ_2_HZ = 500,
    FREQ_3_HZ = 333,
    FREQ_4_HZ = 250,
    FREQ_10_HZ = 100,
    FREQ_20_HZ = 50,
    FREQ_40_HZ = 25,
    FREQ_100_HZ = 10,
    FREQ_REALTIME = 0
} FREQ_VALUES;

typedef enum ERROR_MSG {
    ERROR_CAN_QUEUE_FULL = 25,
    ERROR_NO_RPM,
    ERROR_NO_BATTERY,
    ERROR_NO_FUEL,
    ERROR_NO_VELOCITY,
} ERROR_MSG;

typedef enum WARNING_MSG {
    WARNING_RPM_HIGH = 50,
    WARNING_RPM_LOW,
    WARNING_BATTERY,
    WARNING_FUEL,
    WARNING_VELOCITY_HIGH,
    WARNING_VELOCITY_LOW
} WARNING_MSG;

#endif