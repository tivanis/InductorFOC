//#############################################################################
//
//  $Created on:    September 2019.
//  $Author:        Tomislav Ivanis
//  $Copyright:     (C) 2019 Tomislav Ivanis
//  $E-mail:        tomislav.ivanis@gmail.com
//                  tomislav.ivanis@fer.hr
//
//   Redistributions of source code must retain the above copyright notice.
//
//#############################################################################

#ifndef INC_HAL_H_
#define INC_HAL_H_

#include "driverlib.h"

typedef struct _HAL_adcValues_t
{
    uint16_t IA_value;
    uint16_t IB_value;
    uint16_t IC_value;

    uint16_t UA_value;
    uint16_t UB_value;
    uint16_t UC_value;

    uint16_t UDC_value;

} HAL_adcValues_t;

static inline void HAL_adcCalib(HAL_adcValues_t *handle)
{
    // Trip all PWM modules
    EPWM_forceTripZoneEvent(EPWM1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM4_BASE, EPWM_TZ_FORCE_EVENT_OST);

}


#endif /* INC_HAL_H_ */
