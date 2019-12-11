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

//*****************************************************************************
//
//! Includes
//
//*****************************************************************************

#include "driverlib.h"

#include "foclib.h"
#include "device.h"
#include "pin_map.h"
#include "params.h"

#include "TMC6200.h"

//*****************************************************************************
//
//! Defines
//
//*****************************************************************************

// Pin defines
#define GPIO_PIN_LED_GREEN      (34U)
#define GPIO_PIN_LED_RED        (23U)

// Timing defines
#define DEVICE_SYSCLK_FREQ_MHz  ((uint32_t) DEVICE_SYSCLK_FREQ/1000000)
#define PWM_FREQ_kHz            ((float32_t)(10.0))
#define PWM_PERIOD_usec         ((float32_t) (1000.0/PWM_FREQ_kHz))

// Dead time defines
// DT[s]=PWMDBRED_CNT*PWM_PRESCALE/DEVICE_SYSCLK_FREQ
// DT[s]=PWMDBFED_CNT*PWM_PRESCALE/DEVICE_SYSCLK_FREQ
#define PWM_DBRED_CNT           (20)
#define PWM_DBFED_CNT           (20)

// Interrupt prescale counter
#define numPWMTicksPerISRTick   (1)
#define SOC_PREROLL_CNT         (5)

#define ADC_SAMPLE_WINDOW       (14)
#define HAL_ADC_ISENSE_ZERO     (2047)
#define HAL_SHUNT_RESISTANCE    (0.0025F)
#define HAL_ADC_AMPS_PER_BIT    (0.0322)
#define HAL_ADC_VOLTS_PER_BIT   (0.01564F)
#define ISR_LED_COUNT_MAX       (10000)

#define HAL_IER_MASK_L1         (0x0001)
#define HAL_IER_MASK_L12        (0x0FFF)
#define HAL_IER_MASK_L13        (0x1FFF)

// STRUCTURES:
typedef struct _HAL_adcValues_t
{
    uint16_t IA_value;
    uint16_t IB_value;
    uint16_t IC_value;

    uint16_t UA_value;
    uint16_t UB_value;
    uint16_t UC_value;

    uint16_t UDC_value;


    int16_t IA_offset;
    int16_t IB_offset;
    int16_t IC_offset;

    int16_t UA_offset;
    int16_t UB_offset;
    int16_t UC_offset;

    int16_t UDC_offset;

    bool     calibFlag;

} HAL_adcValues_t;

typedef struct _HAL_pwmData_t_
{
    MATH_Vec3 Vabc_pu;
    uint16_t  cmpValue[3];
    uint16_t  deadband[3];  //NOT USED
    uint16_t  noiseWindow;  //NOT USED
    uint16_t  period;
    uint16_t  socCMP;
} HAL_pwmData_t;

typedef HAL_adcValues_t*    HAL_adcValuesHandle;
typedef HAL_pwmData_t*      HAL_pwmDataHandle;

//*****************************************************************************
//
//! Function definitions
//
//*****************************************************************************
extern void HAL_initSystem(void);

extern void HAL_initEPWM(void);

extern void HAL_untripEPWM(void);

extern void HAL_tripEPWM(void);

extern void HAL_initGPIO(void);

extern HAL_adcValuesHandle HAL_initADC(HAL_adcValuesHandle handle);

extern void HAL_initSCI(void);

extern void HAL_initPeripheralClocks(void);

extern void HAL_initMotorVars(void);

extern void HAL_adcGetValues(HAL_adcValues_t* handle);

extern void HAL_adcRemoveOffsets(HAL_adcValues_t* handle);

extern void HAL_adcCalib(HAL_adcValues_t *handle);

extern void HAL_buildCurrentVectors(HAL_adcValues_t* handle,
                                    MATH_Vec2* outVectHandle);

extern void HAL_buildUnitVectorTh(float32_t angle,
                                  MATH_Vec2* unitVectorTh);

extern void HAL_calcMaxVdqVoltage(MATH_Vec2* vectHandle, float32_t UDC_V);

extern void HAL_buildVDCValue(HAL_adcValues_t* handle, float32_t* VDC_V);

extern void HAL_pwmUpdateRegisters(HAL_pwmData_t* pPWMData,
                                   MATH_Vec3* pVabc_out_pu);

static inline void HAL_setupPI(PI_Handle handle,
                               float32_t Kp,
                               float32_t Ki)
{
    PI_setKp(handle, Kp);
    PI_setKi(handle, Ki);
    PI_setUi(handle, 0.0);
    PI_setRefValue(handle, 0.0);
    PI_setFbackValue(handle, 0.0);
}

static inline void HAL_toggleLED (void)
{
    GPIO_togglePin(GPIO_PIN_LED_GREEN);
}

#endif /* INC_HAL_H_ */
