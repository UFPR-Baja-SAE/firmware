#include "msg.h"

/*
 * you have to free pdata after
 * could look to statically allocate pdata, but it's kinda scuffed
 */

void msg_unpack_can(msg_all* gen, FDCAN_RxHeaderTypeDef* rxheader, uint8_t* rxdata) {
	gen->pdata = malloc(rxheader->DataLength);
	memcpy(gen->pdata, rxdata);

	gen->type = rxheader->Identifier;
	gen->size = rxheader->DataLength;
}

void msg_pack_can(msg_all* gen, FDCAN_TxHeaderTypeDef* rxheader, uint8_t* txdata) {
	rxheader->DataLength = gen->size;
	rxheader->Identifier = gen->type;

	memcpy(txdata, gen->pdata);
}

void msg_pack_serial(msg_all* gen, uint8_t* data) {
	memcpy(data, gen);
}
