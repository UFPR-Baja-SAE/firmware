#ifndef TEMPCVT_H
#define TEMPCVT_H

#include "spi.h"
#include "msg.h"

#define TEMP_RAW_TO_REAL 0.25

uint16_t temp_read();
float temp_convert(uint16_t raw);


#endif