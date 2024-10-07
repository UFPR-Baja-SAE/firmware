#include "rpm.h"
#define MIN_RPM_PERIOD 12

float rpm_calculate(uint32_t* pdata) {
    float rpm = 0;
    uint8_t samples_rpt = 0;

    for (int i = 0; i < RPM_SAMPLES; i++) {
        if ((i > 0)) {
            if (pdata[i] - pdata[i - 1] < MIN_RPM_PERIOD) samples_rpt++;
        } else rpm += pdata[i];
    }
    if (samples_rpt > 1) return -1;
    rpm /= RPM_SAMPLES - samples_rpt;
    rpm *= (60/1000);
    
    return rpm;
}