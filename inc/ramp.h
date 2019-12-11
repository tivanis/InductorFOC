
#ifndef RAMP_H
#define RAMP_H

#include "gpio.h"
#include "cputimer.h"
#include "device.h"
#include "libraries/math/include/math.h"

//WARNING: Do not change clock source
#define RAMP_TIMER_BASE          CPUTIMER1_BASE
#define RAMP_TIMER_CLOCK_SOURCE  CPUTIMER_CLOCK_SOURCE_SYS
#define RAMP_TIMER_PRESCALE      CPUTIMER_CLOCK_PRESCALER_1
#define RAMP_TIMER_EMU_MODE      CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT

#define RAMP_MIN_FREQ            (0.5F)
#define RAMP_MAX_FREQ            (1000.0F)

#define RAMP_TIMER_DEFAULT_FREQ  (50.0F)
#define RAMP_TIMER_DEFAULT_ANGLE (0.0F)

typedef struct _RAMP_Obj_t
{
    float32_t               setpointFrequency;
    uint32_t                setpointTicks;
    float32_t               angle;

    uint32_t                timerBase;
    CPUTimer_ClockSource    timerClockSource;
    CPUTimer_Prescaler      timerPrescaler;
    CPUTimer_EmulationMode  timerEmuMode;


} RAMP_Obj_t;

typedef RAMP_Obj_t* RAMP_Handle;

static inline RAMP_Obj_t* RAMP_init(RAMP_Obj_t* handle)
{
    //Get timer default settings
    handle->timerBase           = RAMP_TIMER_BASE;
    handle->timerClockSource    = RAMP_TIMER_CLOCK_SOURCE;
    handle->timerPrescaler      = RAMP_TIMER_PRESCALE;
    handle->timerEmuMode        = RAMP_TIMER_EMU_MODE;

    //Get default timer frequency and angle
    handle->angle               = RAMP_TIMER_DEFAULT_ANGLE;
    handle->setpointFrequency   = RAMP_TIMER_DEFAULT_FREQ;

    //Disable interrupts before changing timer registers
    CPUTimer_disableInterrupt(handle->timerBase);

    //Write settings to the timer
    CPUTimer_selectClockSource(handle->timerBase,
                               handle->timerClockSource,
                               handle->timerPrescaler);
    //Set emulation mode
    CPUTimer_setEmulationMode(handle->timerBase, handle->timerEmuMode);

    //Set the period from which counter counts down
    handle->setpointTicks = (uint32_t)
         (DEVICE_SYSCLK_FREQ/(handle->setpointFrequency) /
                 (handle->timerPrescaler+1));

    CPUTimer_setPeriod(handle->timerBase,
                       handle->setpointTicks);

    CPUTimer_startTimer(handle->timerBase);
    CPUTimer_enableInterrupt(handle->timerBase);

    return (handle);
}

static inline uint32_t RAMP_getFrequency(RAMP_Obj_t* handle)
{
    return (handle->setpointFrequency);
}

static inline void RAMP_setFrequency(RAMP_Obj_t* handle, float32_t frequency)
{
    //Frequency range checked in ISR

    //Write new frequency setpoint to ramp handling data structure
    (handle->setpointFrequency) = frequency;

    //Calculate required number of ticks
    handle->setpointTicks   =     (uint32_t)
        (DEVICE_SYSCLK_FREQ/(handle->setpointFrequency) /
                (handle->timerPrescaler+1));

    //Write the number of ticks to the period register
    CPUTimer_setPeriod(handle->timerBase,
                       handle->setpointTicks);

    //Reload the timer
    CPUTimer_reloadTimerCounter(handle->timerBase);

}

static inline float32_t RAMP_getAngle(RAMP_Obj_t* handle)
{
    //This angle calculation is set for count down timer
    handle->angle = (float32_t)
      (((     (float32_t)handle->setpointTicks -
              (float32_t) CPUTimer_getTimerCount(handle->timerBase)) /
              ((float32_t) handle->setpointTicks))) * MATH_TWO_PI;

      return (handle->angle);
}


static inline void RAMP_startTimer(RAMP_Obj_t* handle)
{
    CPUTimer_startTimer(handle->timerBase);
}

static inline void RAMP_stopTimer(RAMP_Obj_t* handle)
{
    CPUTimer_stopTimer(handle->timerBase);
}

static inline void RAMP_reloadAndStartTimer(RAMP_Obj_t* handle)
{
    if((handle->setpointFrequency>RAMP_MIN_FREQ)
     && (handle->setpointFrequency<RAMP_MAX_FREQ))
    {
        //Calculate required number of ticks
        handle->setpointTicks   =     (uint32_t)
            (DEVICE_SYSCLK_FREQ/(handle->setpointFrequency) /
                    (handle->timerPrescaler+1));

        //Write the number of ticks to the period register
        CPUTimer_setPeriod(handle->timerBase,
                           handle->setpointTicks);
    }

    CPUTimer_startTimer(handle->timerBase);
}

#endif
