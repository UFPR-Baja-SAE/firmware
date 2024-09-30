#ifndef LORA_E22
#define LORA_E22

#include <stm32mp1xx_hal.h>

typedef enum
{
	LORA_MODE_NORMAL,
	LORA_MODE_WOR,
	LORA_MODE_CONFIG,
	LORA_MODE_SLEEP
} LORA_MODE;

typedef enum
{
	LORA_REG_ADDRESS = 0x00,
	LORA_REG_NETID = 0x02,
	LORA_REG_REG0 = 0x03,
	LORA_REG_REG1 = 0x04,
	LORA_REG_REG2 = 0x05,
	LORA_REG_REG3 = 0x06,
	LORA_REG_CRYPTH = 0x07,
	LORA_REG_CRYPTL = 0x08,
	LORA_REG_PID = 0x80
} LORA_REG;

typedef enum
{
	LORA_UART_1200 = 0x000 << 5,	//bit shifting to bits 5,6,7 so it ends up on the correct place in the register
	LORA_UART_2400 = 0x001 << 5,	//no need to bit shift in later functions
	LORA_UART_4800 = 0x010 << 5,
	LORA_UART_9600 = 0x011 << 5,	//default
	LORA_UART_19200 = 0x100 << 5,
	LORA_UART_38400 = 0x101 << 5,
	LORA_UART_57600 = 0x110 << 5,
	LORA_UART_115200 = 0x111 << 5
} LORA_UART_BPS;

typedef enum 					//has to be the same between devices
{
	LORA_AIR_300,
	LORA_AIR_1200,
	LORA_AIR_2400,					//default
	LORA_AIR_4800,
	LORA_AIR_9600,
	LORA_AIR_19200,
	LORA_AIR_38400,
	LORA_AIR_62500
} LORA_AIR_RATE;

typedef enum 			//minimum data lenght (you have to fill the rest of it before sending)
{
	LORA_PACKET_240 = 0x00 << 6,	//default
	LORA_PACKET_128 = 0x01 << 6,
	LORA_PACKET_64 = 0x10 << 6,
	LORA_PACKET_32 = 0x11 << 6
} LORA_SUBPACKET_SIZE;

typedef enum 				//enables reading rssi data with C0 C1 C2 C3 command
{
	LORA_RSSI_READ_DISABLE,
	LORA_RSSI_READ_ENABLE = 1 << 5
} LORA_RSSI_READ;

typedef enum
{
	LORA_POWER_22,						//default, probably always use this
	LORA_POWER_17,
	LORA_POWER_13,
	LORA_POWER_10

}  LORA_POWER;

typedef enum 							//sends rssi as 8 bytes after the regular message
{
	LORA_RSSI_DISABLE,					//default
	LORA_RSSI_ENABLE = 0x1 << 7
} LORA_ENABLE_RSSI;

typedef enum 								//enables fixed point transmission mode, refer to datasheet
{
	LORA_FIXED_POINT_DISABLE,		//default
	LORA_FIXED_POINT_ENABLE = 0x1 << 6
} LORA_FIXED_POINT;

typedef enum 								//will forward received messages once, kinda useless for us
{
	LORA_REPLY_DISABLE,				//doesn't say in the datasheet but probably default
	LORA_REPLY_ENABLE = 0x1 << 5
} LORA_REPLY;

typedef enum 						//monitors data before sending, no clue how this works, datasheet doesn't say
{
	LORA_LBT_DISABLE,
	LORA_LBT_ENABLE = 0x1 << 4
} LORA_LBT;

typedef enum 										//configures the wake on receive function, will not use since receiver will
{																		//be connected to pc directly
	LORA_WOR_RX,					//default
	LORA_WOR_TX = 0x1 << 3
} LORA_WOR_CONTROL;

typedef enum 					//doesn't matter + i don't understand
{
	LORA_WOR_500,
	LORA_WOR_1000,
	LORA_WOR_1500,
	LORA_WOR_2000,					//default
	LORA_WOR_2500,
	LORA_WOR_3000,
	LORA_WOR_3500,
	LORA_WOR_4000
} LORA_WOR_CYCLE;

typedef struct {
	uint16_t addr;
	uint8_t netid;
	uint8_t channel;
	uint16_t crypt_key;

	LORA_UART_BPS bps;
	LORA_AIR_RATE air_rate;
	LORA_SUBPACKET_SIZE pkt_size;
	LORA_RSSI_READ rssi_read_en;
	LORA_POWER pwr;
	LORA_ENABLE_RSSI rssi_en;
	LORA_FIXED_POINT fixed_point;
	LORA_REPLY reply_en;
	LORA_LBT lbt_en;
	LORA_WOR_CONTROL wor_ctrl;
	LORA_WOR_CYCLE wor_cycle;
} E22_config;

typedef struct
{
	uint16_t m0;
	uint16_t m1;
	uint16_t aux;
	GPIO_TypeDef* GPIOx;

	UART_HandleTypeDef* huart;

	E22_config* cfg;
	LORA_MODE mode;

} E22;

HAL_StatusTypeDef lora_init( E22* module, E22_config* cfg, GPIO_TypeDef* GPIOX,
							uint16_t m0, uint16_t m1, uint16_t aux, UART_HandleTypeDef* huart);

HAL_StatusTypeDef lora_setmode(E22* module, LORA_MODE mode);

//use the enums as defined above for the value variable DO NOT BIT SHIFT ANYTHING
HAL_StatusTypeDef lora_write_register(E22* module, LORA_REG reg, uint8_t* value, uint8_t len);

HAL_StatusTypeDef lora_write_message(E22* module, uint8_t* data, uint16_t len);

HAL_StatusTypeDef lora_read_message(E22* module, uint8_t* data, uint16_t len);




#endif
