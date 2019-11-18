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

#ifndef INC_INIT_H_
#define INC_INIT_H_

//*****************************************************************************
//
//! Includes
//
//*****************************************************************************
#include "device.h"
#include "epwm.h"
#include "gpio.h"
#include "pin_map.h"
#include "TMC6200.h"

//*****************************************************************************
//
//! Defines
//
//*****************************************************************************

// Timing defines
#define DEVICE_SYSCLK_FREQ_MHz  ((uint32_t) DEVICE_SYSCLK_FREQ/1000000)
#define PWM_FREQ_kHz            ((float32_t)(10.0))
#define PWM_PERIOD_usec         ((float32_t) (1000.0/PWM_FREQ_kHz))

// Dead time defines
// DT[s]=PWMDBRED_CNT*PWM_PRESCALE/DEVICE_SYSCLK_FREQ
// DT[s]=PWMDBFED_CNT*PWM_PRESCALE/DEVICE_SYSCLK_FREQ
#define PWM_DBRED_CNT           (15)
#define PWM_DBFED_CNT           (15)

// Interrupt prescale counter
#define numPWMTicksPerISRTick   (1)
#define SOC_PREROLL_CNT         (5)

#define ADC_SAMPLE_WINDOW       (14)
#define ISR_LED_ITER_MAX        (10000)

//*****************************************************************************
//
//! Function definitions
//
//*****************************************************************************
extern void INIT_epwm(void);

extern void INIT_gpio(void);

extern void INIT_adc(void);

extern void INIT_sci(void);

#endif /* INC_INIT_H_ */

/*  END OF FILE */
