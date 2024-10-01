#ifndef MSG_H
#define MSG_H
//ADC1 = battery and fuel
//ADC2 = front and rear break pressure
//ADC2 will eventually be removed since those sensors will go on the front pcb

typedef enum {
    MSG_RPM,
    MSG_VELOCITY,
    MSG_TEMPERATURE,
	MSG_GPS,
    MSG_FUEL,
    MSG_WARNING,
    MSG_ERROR
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
    FREQ_REALTIME = 0
} FREQ_VALUES;

typedef enum {
    ERROR_CAN_QUEUE_FULL = 25,
    ERROR_NO_RPM,
    ERROR_NO_VELOCITY,
	ERROR_NO_TEMP,
} ERROR_MSG;

typedef enum {
    WARNING_RPM_HIGH = 50,
    WARNING_RPM_LOW,
    WARNING_FUEL,
    WARNING_VELOCITY_HIGH,
} WARNING_MSG;

typedef struct {
  MSG_TYPES type;
  uint8_t size;
  void* pdata;
} msg_all;

typedef struct {
	uint8_t bfuel		//this is a boolean
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
	uint8_t code;
	uint32_t timestamp;
} msg_error;




#endif
