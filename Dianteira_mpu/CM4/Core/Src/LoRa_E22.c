#include "LoRa_E22.h"

#define LORA_CREATE_DEFAULT_CONFIG(NAME) E22_config NAME = {		\
	.addr = 0x7272,													\
	.netid = 0x27,													\
	.channel = 0x07,												\
	.crypt_key = 0x1324,											\
	.bps = LORA_UART_9600,											\
	.air_rate = LORA_AIR_2400,										\
	.pkt_size = LORA_PACKET_240,									\
	.rssi_read_en = LORA_RSSI_READ_DISABLE,							\
	.pwr = LORA_POWER_22,											\
	.rssi_en = LORA_RSSI_DISABLE,									\
	.lbt_en = LORA_LBT_DISABLE,										\
	.wor_ctrl = LORA_WOR_RX,										\
	.wor_cycle = LORA_WOR_2000										\
};

HAL_StatusTypeDef lora_setmode(E22* module, LORA_MODE mode)
{

	enum lora_mode prevmode = module->mode;

	if (prevmode != mode)
	{
		if(HAL_GPIO_ReadPin(module->GPIOx, module->aux) == RESET)
		{
			uint32_t starttim = HAL_GetTick();
			uint32_t currtim = starttim;

			while (HAL_GPIO_ReadPin(module->GPIOx, module->aux) == RESET)
			{
				currtim = HAL_GetTick();
				HAL_Delay(2);
				if ((currtim - starttim) > 500)
				{
					return HAL_BUSY;
				}
			}
		}
		switch (mode)
		{
		case LORA_MODE_NORMAL:
			HAL_GPIO_WritePin(module->GPIOx, module->m0, RESET);
			HAL_GPIO_WritePin(module->GPIOx, module->m1, RESET);
			module->mode = LORA_MODE_NORMAL;
			break;
		case LORA_MODE_CONFIG:
			HAL_GPIO_WritePin(module->GPIOx, module->m0, RESET);
			HAL_GPIO_WritePin(module->GPIOx, module->m1, SET);
			module->mode = LORA_MODE_CONFIG;
			break;
		case LORA_MODE_WOR:
			HAL_GPIO_WritePin(module->GPIOx, module->m0, SET);
			HAL_GPIO_WritePin(module->GPIOx, module->m1, RESET);
			module->mode = LORA_MODE_WOR;
			break;
		case LORA_MODE_SLEEP:
			HAL_GPIO_WritePin(module->GPIOx, module->m0, SET);
			HAL_GPIO_WritePin(module->GPIOx, module->m1, SET);
			module->mode = LORA_MODE_SLEEP;
		}
	}



	return HAL_OK;

}
/*
 * you need to send the entire register config to this
 * do not set a single parameter on a multi parameter register
 * it will break shit
 */
HAL_StatusTypeDef lora_write_register(E22* module, LORA_REG reg, uint8_t* value, uint8_t len)
{
	LORA_MODE old_mode = module->mode;
	lora_setmode(module, LORA_MODE_CONFIG);

	uint8_t cmd[3 + len];

	cmd[0] = 0xC0;
	cmd[1] = reg;
	cmd[2] = len;
	for (int i = 3; i < 3 + len; i++) {
		cmd[i] = value[i - 3];
	}

	if (HAL_UART_Transmit(module->huart, cmd, 3 + len, 10) != HAL_OK)
	{
		return HAL_ERROR;
	}

	lora_set_mode(module, old_mode);

	return HAL_OK;
}

/*
 * sets the config passed in cfg and inits the module
 */

HAL_StatusTypeDef lora_init(E22* module, E22_config* cfg, GPIO_TypeDef* GPIOX,
							uint16_t m0, uint16_t m1, uint16_t aux, UART_HandleTypeDef* huart)
{
	module->GPIOx = GPIOX;
	module->aux = aux;
	module->m0 = m0;
	module->m1 = m1;
	module->huart = huart;

	lora_write_register(module, LORA_REG_ADDRESS, cfg-->addr, sizeof(cfg->addr));
	lora_write_register(module, LORA_REG_NETID, cfg->netid, sizeof(cfg->netid));
	uint8_t temp_reg = cfg->air_rate | cfg->bps;	//serial parity at 00, dont mess with it
	lora_write_register(module, LORA_REG_REG0, temp_reg, sizeof(temp_reg));
	temp_reg = 0;
	temp_reg = cfg->pkt_size | cfg->rssi_en | cfg->pwr;
	lora_write_register(module, LORA_REG_REG1, temp_reg, sizeof(temp_reg));
	temp_reg = 0;
	temp_reg = cfg->channel;
	lora_write_register(module, LORA_REG_REG2, temp_reg, sizeof(temp_reg));
	temp_reg = 0;
	temp_reg = cfg->rssi_en | cfg->fixed_point | cfg->reply_en | cfg->lbt_en | cfg->wor_ctrl | cfg->wor_cycle;
	lora_write_register(module, LORA_REG_REG3, temp_reg, sizeof(temp_reg));
	lora_write_register(module, LORA_REG_CRYPTH, cfg->crypt_key, sizeof(cfg->crypt_key));

	module->cfg = cfg;
	//set to configuration mode




	return HAL_OK;
}

HAL_StatusTypeDef lora_write_message(E22* module, uint8_t* data, uint16_t len)
{
	if (module->mode != LORA_MODE_NORMAL) {
		lora_setmode(module, LORA_MODE_NORMAL);
	}

	return HAL_UART_Transmit(module->huart, data, len, 0);
}

/*
 * need to check if this works on interrupts, I think it should
 */

HAL_StatusTypeDef lora_read_message(E22* module, uint8_t* data, uint16_t len)
{
	return HAL_UART_Receive(module->huart, data, len, 0);
}
