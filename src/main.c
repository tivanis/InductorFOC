//#############################################################################
//
//  $Created on:    September 2019.
//  $Author:        Tomislav Ivanis
//  $Copyright:     (C) 2019 Tomislav Ivanis
//  $E-mail:        tomislav.ivanis@gmail.com
//                  tomislav.ivanis@fer.hr
//
//#############################################################################
//*****************************************************************************
//! \file   uINV_280049C_003/src/main.c
//! \brief  Main file for motor control. Intended for use with uINVERTER MKII
//!
//*****************************************************************************

//*****************************************************************************
//
//! Includes
//
//*****************************************************************************

#include "device.h"
#include "driverlib.h"
#include "foclib.h"
#include "TMC6200.h"
#include "ramp.h"
#include "hal.h"
#include "math.h"

// Declaration of interrupts
__interrupt void xint1ISR(void);
__interrupt void invFaultISR(void);
__interrupt void mainISR(void);
__interrupt void cpuTimer1ISR(void);

//*****************************************************************************
//
//! Global variables
//
//*****************************************************************************

uint32_t            counterLED     =   0;

HAL_adcValues_t     adcValues;
HAL_adcValuesHandle adcValuesHandle;

HAL_pwmData_t       pwmData;
HAL_pwmDataHandle   pwmDataHandle;

MATH_Vec2           Iab_in_A;
MATH_Vec2           Ialbet_in_A;
MATH_Vec2           unitVectorTh;
MATH_Vec2           Idq_in_A;
MATH_Vec2           Idq_ref_A;
MATH_Vec2           Vdq_limit_V;
MATH_Vec3           Vabc_out_pu;
MATH_Vec2           Valbet_out_V;
MATH_Vec2           Vdq_out_V;
float32_t           VDC_V;
float32_t           oneOverVDC_V;

CLARKE_Obj          clarke;
CLARKE_Handle       clarkeHandle;

PARK_Obj            park;
PARK_Handle         parkHandle;

IPARK_Handle        iparkHandle;
IPARK_Obj           ipark;

PI_Handle           piHandle_Id;
PI_Obj              pi_Id;

PI_Handle           piHandle_Iq;
PI_Obj              pi_Iq;

SVGEN_Handle        svgenHandle;
SVGEN_Obj           svgen;

/*
SVGENCURRENT_Handle svgencurrentHandle;
SVGENCURRENT_Obj    svgencurrent;
*/

RAMP_Handle         rampHandle;
RAMP_Obj_t          ramp;

TMC6200_Obj_t       TMC6200_Obj;

//Volatile global interrupt counters
volatile uint32_t faultCount = 0;

//*****************************************************************************
//
//! Main function.
//
//*****************************************************************************

int main(void)
{
    //SETUP: Initialize system clocks
    HAL_initSystem();

    //SETUP: setup clocks for the peripherals
    HAL_initPeripheralClocks();

    //SETUP: GPIO
    HAL_initGPIO();

    //SETUP: ePWM
    HAL_initEPWM();

    //SETUP: ADC
    adcValuesHandle = HAL_initADC(&adcValues);

    //SETUP: Initialize Motor variables from constants defined in params.h
    HAL_initMotorVars();

    //SETUP: Clarke transform
    //Note: Phase A in program code is phase C on PCB (U on TMC6200)
    //      Phase B in program code is phase B on PCB (U on TMC6200)
    //      Phase C in program code is phase A on PCB (U on TMC6200)
    clarkeHandle    = CLARKE_init(&clarke, sizeof(clarke));
    CLARKE_setScaleFactors(clarkeHandle,
                           PARAMS_CLARKE_ALPHA_SF,
                           PARAMS_CLARKE_BETA_SF);
    CLARKE_setNumSensors(clarkeHandle, PARAMS_N_CURR_SENS);

    //SETUP: Park transform
    parkHandle      = PARK_init(&park, sizeof(park));

    //SETUP: Inverse Park transform
    iparkHandle     = IPARK_init(&ipark, sizeof(ipark));

    //SETUP: Space-Vector generator for voltages
    svgenHandle     = SVGEN_init(&svgen, sizeof(svgen));

    /*
    //SETUP: Space-Vector generator for currents
    svgencurrentHandle = SVGENCURRENT_init(&svgencurrent, sizeof(svgencurrent));
    */

    //SETUP: default reference values for Id and then Iq
    Idq_ref_A.value[0] = 0;
    Idq_ref_A.value[1] = 0;

    //SETUP: PI controller for d-axis current
    piHandle_Id     = PI_init(&pi_Id, sizeof(pi_Id));
    HAL_setupPI(&pi_Id,
                PARAMS_PI_KP_ID,
                PARAMS_PI_KI_ID);
    PI_setMinMax(piHandle_Id, 0.0, 0.0);

    //SETUP: PI controller for q-axis current
    piHandle_Iq     = PI_init(&pi_Iq, sizeof(pi_Iq));
    HAL_setupPI(&pi_Iq,
                PARAMS_PI_KP_IQ,
                PARAMS_PI_KI_IQ);
    PI_setMinMax(piHandle_Iq, 0.0, 0.0);

    // SETUP: Ramp generator (sets up the timer and returns pointer to
    //        ...ramp structure
    rampHandle = RAMP_init(&ramp);

    //SETUP: assign PWM data handle to PWM data structure
    pwmDataHandle = &pwmData;

    // SETUP: TMC6200 driver integrated circuit
    TMC6200_initSPI(&TMC6200_Obj);
    TMC6200_deviceEnable();
    DEVICE_DELAY_US(10000);
    TMC6200_readRegisters(&TMC6200_Obj);
    TMC6200_clearReset(&TMC6200_Obj);
    TMC6200_setInterfaceMode(&TMC6200_Obj, TMC6200_GCONF_IF_MODE_INDIVIDUAL);
    TMC6200_setAmplificationMode(&TMC6200_Obj, TMC6200_GCONF_CURR_AMP_10);
    TMC6200_setDriveStrength(&TMC6200_Obj, TMC6200_DRV_WEAK_TC);
    TMC6200_readRegisters(&TMC6200_Obj);

    // Measure the offsets after proper amplification has been set
    HAL_adcCalib(&adcValues);

    //SETUP: Interrupts
    Interrupt_initModule();
    Interrupt_initVectorTable();

    //SETUP: map ISR addresses to the vectors
    Interrupt_register(INT_XINT1, &xint1ISR);
    Interrupt_register(INT_XINT3, &invFaultISR);
    Interrupt_register(INT_ADCC1, &mainISR);
    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);

    //Enable interrupts (GPIO)
//    GPIO_enableInterrupt(GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT3);

    //Enable interrupts (ADC)
    ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER1);

    //Enable interrupts on PIEIER registers
//    Interrupt_enable(INT_XINT1);
    Interrupt_enable(INT_XINT3);
    Interrupt_enable(INT_TIMER1);
    Interrupt_enable(INT_ADCC1);


    //Untrip ePWM
    HAL_untripEPWM();

    //Global enable of interrupts, Global enable of real time debug events
    EINT;
    ERTM;

    while(1)
    {
        NOP;
    }

    return 0;
}

//*****************************************************************************
//
//! ISR of external interrupt 1. (NOT ENABLED!)
//
//*****************************************************************************

__interrupt void xint1ISR(void)
{
    //! Set interrupt priority to disable lower level interrupt preemption
    IER &= HAL_IER_MASK_L1;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    NOP;
    EINT;

    ////
    // ISR BODY
    ////

    DINT;
}

//*****************************************************************************
//
//! ISR of external interrupt 3. Inverter fault!
//
//*****************************************************************************

__interrupt void invFaultISR(void)
{
    //Mask lower priority interrupts. (IER context is stored by hardware).
    IER &= HAL_IER_MASK_L12;
    //Acknowledge the interrupt.
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);
    //Wait 1 cycle
    NOP;
    //Enable global interrupts
    EINT;

    //ISR BODY
    //Increase fault count
    ++faultCount;

    //Trip PWM
    HAL_tripEPWM();

    //! Read faults from TMC6200 to local object
    TMC6200_getFaults(&TMC6200_Obj);

    //! IMPORTANT TO DO!
    //! CREATE A STATE MACHINE WHICH WILL CHANGE WHEN ERROR IS DETERMINED!

    DINT;
    return;
}

//*****************************************************************************
//
//! TIMER 1 ISR
//! \brief
//
//*****************************************************************************
__interrupt void cpuTimer1ISR(void)
{
    //Mask lower priority interrupts. (IER context is stored by hardware).
    IER &= HAL_IER_MASK_L13;
    //CPU acknowledges interrupt, as the interrupt is only one at INT13.
    //Wait 1 cycle
    NOP;
    //Enable interrupts globally
    EINT;

    //Execute ISR body
    //Stop the timer
    RAMP_stopTimer(rampHandle);

    //Check for frequency boundaries and saturate if necessary
    //Floating points can be compared in this case.
    if((rampHandle->setpointFrequency) > RAMP_MIN_FREQ)
    {
        if((rampHandle->setpointFrequency) > RAMP_MAX_FREQ)
        {
            RAMP_setFrequency(rampHandle, RAMP_MAX_FREQ);
            RAMP_startTimer(rampHandle);
        }
        else
        {
            RAMP_setFrequency(rampHandle, rampHandle->setpointFrequency);
            RAMP_startTimer(rampHandle);
        }
    }
    else
    {
        RAMP_setFrequency(rampHandle, RAMP_MIN_FREQ);
        RAMP_startTimer(rampHandle);
    }
    DINT;
    return;
}


//*****************************************************************************
//
//! Main ISR
//! \brief ISR triggered on ADC end of conversion, calculates FOC
//
//*****************************************************************************

__interrupt void mainISR(void)
{
    //Mask lower priority interrupts. (IER context is stored by hardware).
    IER &= HAL_IER_MASK_L1;
    //Acknowledge the interrupt
    ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    //Wait 1 cycle
    NOP;
    //Enable interrupts
    EINT;

    // Execute ISR body
    //Trip EPWM
    //HAL_tripEPWM();

    // Blink LED
    counterLED++;
    if(counterLED>=ISR_LED_COUNT_MAX)
    {
        counterLED = 0;
        HAL_toggleLED();
    }

    // Get ADC values
    HAL_adcGetValues(adcValuesHandle);

    // Remove DC offsets from ADC values
    HAL_adcRemoveOffsets(adcValuesHandle);

    //Build vectors from uint16_t values of currents
    HAL_buildCurrentVectors(adcValuesHandle, &Iab_in_A);
    CLARKE_run_twoInput(clarkeHandle, &Iab_in_A, &Ialbet_in_A);

    //Get rotor angle: for now use ramp as a fictional feedback
    RAMP_getAngle(rampHandle);

    //Create vectors, utilize TMU. (TMU uses double==float==float32_t)
    //WARNING: Does not conform with ISO! See SPRU514K, page 30.
    //Note: __sin(double arg); and __cos(double arg); is used here
    PARK_setSinTh(parkHandle, __sin(rampHandle->angle));
    PARK_setCosTh(parkHandle, __cos(rampHandle->angle));

    //Run direct Park transform
    PARK_run(parkHandle, &Ialbet_in_A, &Idq_in_A);

    //Calculate maximum values for Vdq vector
    //Needs to be recalculated every PWM cycle
    HAL_buildVDCValue(&adcValues, &VDC_V);
    HAL_calcMaxVdqVoltage(&Vdq_limit_V, VDC_V);

    //Calculate Q axis voltage limit and then voltage through PI controller
    PI_setMinMax(piHandle_Iq,
                 -(Vdq_limit_V.value[1]),
                 (Vdq_limit_V.value[1]));

    PI_run_series(piHandle_Iq,
                  Idq_ref_A.value[1],
                  Idq_in_A.value[1],
                  PARAMS_PI_IQ_FF_VALUE,
                  &(Vdq_out_V.value[1]));

    //Calculate D axis voltage limit and then voltage through PI controller
    PI_setMinMax(piHandle_Id,
                 -(Vdq_limit_V.value[0]),
                 (Vdq_limit_V.value[0]));
    PI_run_series(piHandle_Id,
                  Idq_ref_A.value[0],
                  Idq_in_A.value[0],
                  PARAMS_PI_ID_FF_VALUE,
                  &(Vdq_out_V.value[0]));

    //Calculate inverse park transform. Use the same vector as for PARK for now
    IPARK_setSinTh(iparkHandle, __sin(rampHandle->angle));
    IPARK_setCosTh(iparkHandle, __cos(rampHandle->angle));
    IPARK_run(iparkHandle, &Vdq_out_V, &Valbet_out_V);

    //Calculate reference voltage space vector
    SVGEN_setOneOverDcBus_invV(svgenHandle, (1/VDC_V));
    SVGEN_run(svgenHandle, &Valbet_out_V, &Vabc_out_pu);

    //Scale and modulate (update registers) space vectors
    HAL_pwmUpdateRegisters(pwmDataHandle, &Vabc_out_pu);

    //Disable Interrupt
    DINT;
    return;
}

/*  END OF FILE */
