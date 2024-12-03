#include "msg.h"

#ifdef USE_CAN
#include "can.h"
#else 
#include "fdcan.h"
#endif

void msg_create_generic(msg_all* out, size_t len, MSG_TYPES type, uint8_t* in) {
	out->type = type;
	out->size = len;
	memcpy(out->pdata, in, len);
}

/*
 * you have to free pdata after
 * could look to statically allocate pdata, but it's kinda scuffed
 */
#ifdef USE_FDCAN
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
#else
#ifdef USE_CAN
void msg_unpack_can(msg_all* gen, CAN_RxHeaderTypeDef* rxheader, uint8_t* rxdata) {
	gen->pdata = malloc(rxheader->RTR);
	memcpy(gen->pdata, rxdata, gen->size);

	gen->type = rxheader->StdId;
	gen->size = rxheader->RTR;
}

void msg_pack_can(msg_all* gen, CAN_TxHeaderTypeDef* rxheader, uint8_t* txdata) {
	rxheader->RTR = gen->size;
	rxheader->StdId = gen->type;

	memcpy(txdata, gen->pdata, gen->size);
}
#endif
#endif

void msg_pack_serial(msg_all* gen, uint8_t* data) {
	data = malloc(gen->size);
	memcpy(data, gen, gen->size);
}