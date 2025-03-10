/*
 * comms.c
 *
 *  Created on: Mar 10, 2025
 *      Author: pauli
 */

#include "comms.h"


CanBuffRx rx;
Rpmsg vuart;

extern FDCAN_HandleTypeDef hfdcan2;

CanMsgRx comms_get_can_msg() {
	CanMsgRx ret;
	memcpy(&ret, &rx.buff[0], sizeof(CanMsgRx));

	for (int i = 0; i < rx.index; i++) {
		memcpy(&rx.buff[i], &rx.buff[i + 1], sizeof(CanMsgRx));
	}
	rx.index--;

	return ret;
}

COMMS_STATUS comms_put_can_msg(CanMsgRx* msg) {
	if (rx.index == 7) return COMMS_RX_BUFFER_FULL;

	memcpy(&rx.buff[rx.index + 1], msg, sizeof(CanMsgRx));
	rx.index++;
	return COMMS_OK;
}

COMMS_STATUS comms_can_rx_cb() {
	CanMsgRx rx;
	HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &rx.header, rx.data);

	return(comms_put_can_msg(&rx));
}

Msg comms_format_msg(CanMsgRx* raw_rx) {
	Msg ret;
	ret.id = raw_rx->header.Identifier;

	memset(ret.data, 0, 128 * sizeof(uint8_t));

	//always 8 as it doesn't really matter apart from tx speed (which doesn't matter)
	//for us here
	ret.len = 8;
	memcpy(raw_rx->data, &ret.data, 8 *sizeof(uint8_t));

	return ret;
}

void rpmsg_cb(VIRT_UART_HandleTypeDef* huart) {
	vuart.rxlen = huart->RxXferSize;
	memcpy(vuart.rxdata, huart->pRxBuffPtr, vuart.rxlen);
	vuart.rxflag = 1;
}

COMMS_STATUS comms_start_rpmsg(uint32_t timeout) {
	if (VIRT_UART_Init(&vuart.handle) != VIRT_UART_OK) return COMMS_RPMSG_VUART_ERROR;
	if (VIRT_UART_RegisterCallback(&vuart.handle, VIRT_UART_RXCPLT_CB_ID, rpmsg_cb) != VIRT_UART_OK) return COMMS_RPMSG_VUART_ERROR;
	vuart.rxflag = 0;
	uint32_t start = osKernelGetTickCount();
	while (osKernelGetTickCount() - start < timeout && vuart.rxflag == 0) {
		OPENAMP_check_for_message();
		osDelay(1);
	}
	if (!vuart.rxflag) return COMMS_RPMSG_TIMEOUT;
	vuart.rxflag = 0;
	return COMMS_OK;
}

COMMS_STATUS comms_send_rpmsg(Msg* tx) {
	if (VIRT_UART_Transmit(&vuart.handle, tx, sizeof(Msg)) != VIRT_UART_OK) return COMMS_RPMSG_VUART_ERROR;
	return COMMS_OK;
}
