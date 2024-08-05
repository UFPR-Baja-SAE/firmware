#include "rpm.h"

float rpm_calculate(uint32_t* pdata) {
    float rpm = 0;

    for (int i = 0; i < RPM_SAMPLES; i++) {
        rpm += pdata[i];
    }
    rpm /= RPM_SAMPLES;
    rpm *= (60/1000);
    
    return rpm;
}