/*
 * gps.c
 *
 *  Created on: Oct 2, 2024
 *      Author: pauli
 */

#include "gps.h"

extern TIM_HandleTypeDef htim2;

void gps_freewheel_set_period(uint32_t period) {
	if (period < 1000000) {
		__HAL_TIM_SET_AUTORELOAD(&htim2, period);
	}
}
