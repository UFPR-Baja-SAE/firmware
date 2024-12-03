#ifndef MSG_H
#define MSG_H

#include "stm32mp1xx_hal.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"
#include "stdint.h"

typedef enum {
    MSG_RPM,
    MSG_VELOCITY,
    MSG_TEMPERATURE,
	MSG_GPS,
    MSG_FUEL,
    MSG_BATTERY,
    MSG_BRAKE_R,
    MSG_BRAKE_F,
    MSG_WARNING,
    MSG_ERROR,
    MSG_CONTROL
} MSG_TYPES;

typedef enum {
    FREQ_ITR = 75,
    FREQ_POLLING,
    FREQ_COMMS
} MSG_FREQ;

//just put this in osDelay(ms)
//not a good idea to send stuff at 100hz or more I think, should test this out
//can also just send a delay value directly, this is made so it is more legible

typedef enum {
    FREQ_1_HZ = 1000,
    FREQ_2_HZ = 500,
    FREQ_3_HZ = 333,
    FREQ_4_HZ = 250,
    FREQ_10_HZ = 100,
    FREQ_20_HZ = 50,
    FREQ_40_HZ = 25,
    FREQ_100_HZ = 10,
    FREQ_200_HZ = 5,
    FREQ_500_HZ = 2,
    FREQ_REALTIME = 0
} FREQ_VALUES;

typedef enum {
    ERROR_CAN_QUEUE_FULL = 25,
    ERROR_FAULTY_RPM,
    ERROR_NO_RPM,
    ERROR_FAULTY_VELOCITY,
    ERROR_NO_VELOCITY,
    ERROR_FAULTY_TEMP,
	ERROR_NO_TEMP
} ERROR_MSG;

typedef enum {
    WARNING_RPM_HIGH = 50,
    WARNING_RPM_LOW,
    WARNING_RPM_DEBOUNCE,
    WARNING_FUEL,
    WARNING_VELOCITY_HIGH,
    WARNING_TEMP_SPI,
    WARNING_TEMP_FAULTY,
    WARNING_TEMP_HIGH
} WARNING_MSG;

typedef enum {
    CONTROL_RPM,
    CONTROL_VELOCITY,
    CONTROL_TEMPERATURE,
    CONTROL_FUEL,
    CONTROL_CAN
} CONTROL_UNIT;

typedef enum {
    CONTROL_CHANGE_STATE,
    CONTROL_CHANGE_FREQ
} CONTROL_ACTION;

typedef struct {
  MSG_TYPES type;
  uint8_t size;
  uint8_t pdata[8];
} msg_all;

typedef struct {
	uint32_t fuel;
	uint32_t timestamp;
} msg_fuel;

typedef struct {
	uint32_t spd;
	uint32_t timestamp;
} msg_vel;

typedef struct {
    uint32_t rpm;
    uint32_t timestamp;
} msg_rpm;

typedef struct {
    uint32_t temp;
    uint32_t timestamp;
} msg_tempcvt;

typedef struct {
    uint32_t batv;
    uint32_t timestamp;
} msg_bat;

typedef struct {
    uint32_t press;
    uint32_t timestamp;
} msg_brake;

typedef struct {
    uint8_t code;
    uint32_t timestamp;
} msg_warning;

typedef struct {
	uint8_t code;
	uint32_t timestamp;
} msg_error;

typedef struct {
    CONTROL_UNIT unit;
    CONTROL_ACTION action;
    uint8_t info
} msg_control;

typedef struct {
    uint8_t data[4];
    uint32_t timestamp;
} msg_uif;


void msg_create_generic(msg_all* out, size_t len, MSG_TYPES type, uint8_t* in);
void msg_unpack_can(msg_all* gen, FDCAN_RxHeaderTypeDef* rxheader, uint8_t* rxdata);
void msg_pack_can(msg_all* gen, FDCAN_TxHeaderTypeDef* rxheader, uint8_t* txdata);

void msg_pack_serial(msg_all* in, uint8_t* out);

void msg_unpack_serial(msg_all* out, uint8_t* in);

#endif
void msg_pack_serial(msg_all* gen, uint8_t* data);
#endif
