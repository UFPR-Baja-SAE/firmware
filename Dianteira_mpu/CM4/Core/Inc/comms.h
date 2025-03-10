/*
 * comms.h
 *
 *  Created on: Mar 10, 2025
 *      Author: pauli
 */

#ifndef SRC_COMMS_H_
#define SRC_COMMS_H_
#include "fdcan.h"
#include "ipcc.h"
#include "openamp.h"
#include "string.h"
#include "cmsis_os.h"

typedef enum {
	MSG_RPM,
	MSG_SPEED,
	MSG_FUEL,
	MSG_BRAKES,
	MSG_TEMP,
	MSG_IMU,
	MSG_MAG,
	MSG_GPS,
} MSG_SOURCE;

typedef enum {
	COMMS_OK,
	COMMS_RX_BUFFER_FULL,
	COMMS_RX_BUFFER_EMPTY,
	COMMS_RPMSG_TIMEOUT,
	COMMS_RPMSG_VUART_ERROR,
} COMMS_STATUS;

/*
 * to be sent via openamp
 */
typedef struct {
	MSG_SOURCE id;
	uint32_t len;
	uint8_t data[128];
} Msg;

/*
 * to be received via can
 */
typedef struct {
	FDCAN_RxHeaderTypeDef header;
	uint8_t data[8];
} CanMsgRx;

typedef struct {
	CanMsgRx buff[8];
	uint8_t index;
} CanBuffRx;

typedef struct {
	VIRT_UART_HandleTypeDef handle;
	uint32_t rxlen;
	uint32_t txlen;
	uint8_t rxdata[128];
	uint8_t txdata[128];
	//bool
	uint8_t rxflag;
} Rpmsg;

CanMsgRx comms_get_can_msg();
COMMS_STATUS comms_put_can_msg(CanMsgRx* msg);
COMMS_STATUS comms_can_rx_cb();

Msg comms_format_msg(CanMsgRx* raw_rx);

void rpmsg_cb(VIRT_UART_HandleTypeDef* huart);
COMMS_STATUS comms_start_rpmsg(uint32_t timeout);
COMMS_STATUS comms_send_rpmsg(Msg* tx);




#endif /* SRC_COMMS_H_ */
