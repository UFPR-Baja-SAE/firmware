#include "msg_types.h"
#include "stdint.h"
#include "event_flags.h"

#define RPM_SAMPLES 5

float rpm_calculate(uint32_t* pdata);

//add variation calculations and other stats n stuff