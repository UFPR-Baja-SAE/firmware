#include "msg.h"


void msg_create_generic(msg_all* out, size_t len, MSG_TYPES type, uint8_t* in) {
	out->type = type;
	out->size = len;
	memcpy(out->pdata, in, len);
}

/*
 * you have to free pdata after
 * could look to statically allocate pdata, but it's kinda scuffed
 */

void msg_unpack_can(msg_all* gen, FDCAN_RxHeaderTypeDef* rxheader, uint8_t* rxdata) {
	memcpy(gen->pdata, rxdata, gen->size);

	gen->type = rxheader->Identifier;
	gen->size = 8;				//TODO: switch statement for each msg type since dlc gets corrupted for some reason
}

void msg_pack_can(msg_all* gen, FDCAN_TxHeaderTypeDef* rxheader, uint8_t* txdata) {
	rxheader->DataLength = gen->size;
	rxheader->Identifier = gen->type;

	memcpy(txdata, gen->pdata, gen->size);
}

void msg_pack_serial(msg_all* gen, uint8_t* data) {
	data = malloc(gen->size);
	memcpy(data, gen, gen->size);
}
