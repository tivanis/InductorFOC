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

//! \file   uINV_280049C_003/src/main.c
//! \brief  Main file for motor control. Intended for use with uINVERTER MKI
//!
//*****************************************************************************

//*****************************************************************************
//
//! Includes
//
//*****************************************************************************

#include "device.h"
#include "driverlib.h"
#include "init.h"
#include "hal.h"

// Declaration of interrupts
__interrupt void xint1ISR(void);
__interrupt void invFaultISR(void);
__interrupt void mainISR(void);

//*****************************************************************************
//
//! Global variables
//
//*****************************************************************************

const float32_t sineTableA[24] =
{
     0.5000, 0.6294, 0.7500, 0.8536, 0.9330, 0.9830,
     1.0000, 0.9830, 0.9330, 0.8536, 0.7500, 0.6294,
     0.5000, 0.3706, 0.2500, 0.1464, 0.0670, 0.0170,
     0.0000, 0.0170, 0.0670, 0.1464, 0.2500, 0.3706
};

const float32_t sineTableB[24] =
{
     0.9330, 0.9830, 1.0000, 0.9830, 0.9330, 0.8536,
     0.7500, 0.6294, 0.5000, 0.3706, 0.2500, 0.1464,
     0.0670, 0.0170, 0.0000, 0.0170, 0.0670, 0.1464,
     0.2500, 0.3706, 0.5000, 0.6294, 0.7500, 0.8536
};

const float32_t sineTableC[24] =
{
     0.0670, 0.1464, 0.2500, 0.3706, 0.5000, 0.6294,
     0.7500, 0.8536, 0.9330, 0.9830, 1.0000, 0.9830,
     0.9330, 0.8536, 0.7500, 0.6294, 0.5000, 0.3706,
     0.2500, 0.1464, 0.0670, 0.0170, 0.0000, 0.0170
};

uint16_t tableIter      =   0;
uint32_t isrLEDIter     =   0;

HAL_adcValues_t  adcValues;

TMC6200_Obj_t TMC6200_Obj;

//Volatile global interrupt counters
volatile uint32_t xint1Count = 0;
volatile uint32_t xint3Count = 0;

//*****************************************************************************
//
//! Main function.
//
//*****************************************************************************

int main(void)
{
    //  Device_init(); //REMOVED, it is advised to move device init function to
    //  device.h instead of init.h

    //Disable Watchdog
    SysCtl_disableWatchdog();

    //Set main system clock to 100MHZ
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);

    // Disable DC-DC controller
    ASysCtl_disableDCDC();

    // Enable sync and clock to SPI module
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIB);

    ASSERT(SysCtl_getClock(DEVICE_OSCSRC_FREQ) == DEVICE_SYSCLK_FREQ);
    ASSERT(SysCtl_getLowSpeedClock(DEVICE_OSCSRC_FREQ) == DEVICE_LSPCLK_FREQ);

    //Enable sync and clock to all PWM modules
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);

    //Enable sync and clock to all ADC modules
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCC);

    //SETUP: GPIO
    INIT_gpio();

    //SETUP: ePWM
    INIT_epwm();

    //SETUP: ADC
    INIT_adc();

    // SETUP: TMC6200
    TMC6200_initSPI(&TMC6200_Obj);
    TMC6200_deviceEnable();
    DEVICE_DELAY_US(10000);
    TMC6200_readRegisters(&TMC6200_Obj);
    TMC6200_clearReset(&TMC6200_Obj);
    TMC6200_setInterfaceMode(&TMC6200_Obj, TMC6200_GCONF_IF_MODE_INDIVIDUAL);
    TMC6200_setAmplificationMode(&TMC6200_Obj, TMC6200_GCONF_CURR_AMP_10);
    TMC6200_setDriveStrength(&TMC6200_Obj, TMC6200_DRV_WEAK_TC);

    // Measure the offsets after proper amplification has been set
    HAL_adcCalib(&adcValues);

    //SETUP: Interrupts
    Interrupt_initModule();
    Interrupt_initVectorTable();

    //SETUP: map ISR addresses to the vectors
    Interrupt_register(INT_XINT1, &xint1ISR);
    Interrupt_register(INT_XINT3, &invFaultISR);
    Interrupt_register(INT_ADCC1, &mainISR);

    //Enable interrupts (GPIO)
//    GPIO_enableInterrupt(GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT3);

    //Enable interrupts (ADC)
    ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER1);

    //Enable interrupts on PIEIER registers
//    Interrupt_enable(INT_XINT1);
//    Interrupt_enable(INT_XINT3);

    Interrupt_enable(INT_ADCC1);


    //Untrip ePWM

    EPWM_clearTripZoneFlag(EPWM1_BASE, (EPWM_TZ_INTERRUPT_DCBEVT2 |
                                        EPWM_TZ_INTERRUPT_DCBEVT1 |
                                        EPWM_TZ_INTERRUPT_DCAEVT2 |
                                        EPWM_TZ_INTERRUPT_DCAEVT1 |
                                        EPWM_TZ_INTERRUPT_OST |
                                        EPWM_TZ_INTERRUPT_CBC));

    EPWM_clearTripZoneFlag(EPWM2_BASE, (EPWM_TZ_INTERRUPT_DCBEVT2 |
                                        EPWM_TZ_INTERRUPT_DCBEVT1 |
                                        EPWM_TZ_INTERRUPT_DCAEVT2 |
                                        EPWM_TZ_INTERRUPT_DCAEVT1 |
                                        EPWM_TZ_INTERRUPT_OST |
                                        EPWM_TZ_INTERRUPT_CBC));
    EPWM_clearTripZoneFlag(EPWM4_BASE, (EPWM_TZ_INTERRUPT_DCBEVT2 |
                                        EPWM_TZ_INTERRUPT_DCBEVT1 |
                                        EPWM_TZ_INTERRUPT_DCAEVT2 |
                                        EPWM_TZ_INTERRUPT_DCAEVT1 |
                                        EPWM_TZ_INTERRUPT_OST |
                                        EPWM_TZ_INTERRUPT_CBC));

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
//! ISR of external interrupt 1.
//
//*****************************************************************************

__interrupt void xint1ISR(void)
{
    //! Set interrupt priority to disable lower level interrupt preemption
    IER &= 0b0000000000000001;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    NOP;
    EINT;

    /*
    //! Force trip all ePWM modules
    EPWM_forceTripZoneEvent(EPWM1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM4_BASE, EPWM_TZ_FORCE_EVENT_OST);
    */
}

//*****************************************************************************
//
//! ISR of external interrupt 3. Inverter fault
//
//*****************************************************************************

__interrupt void invFaultISR(void)
{
    //! Set interrupt priority to disable lower level interrupt preemption
    IER &= 0b0000011111111111;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
    NOP;
    EINT;

    //! Force trip all ePWM modules
    EPWM_forceTripZoneEvent(EPWM1_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM2_BASE, EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(EPWM4_BASE, EPWM_TZ_FORCE_EVENT_OST);

    //! Read faults from TMC6200 to local object
    TMC6200_getFaults(&TMC6200_Obj);

    //! IMPORTANT TO DO!
    //! CREATE A STATE MACHINE WHICH WILL CHANGE WHEN ERROR IS DETERMINED!

}

//*****************************************************************************
//
//! Main ISR
//! \brief ISR triggered on ADC end of conversion, calculates FOC
//
//*****************************************************************************

__interrupt void mainISR(void)
{
    //! Set interrupt priority to disable lower level interrupt preemption
    IER &= 0b0000000000000111;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);
    NOP;
    EINT;

    float32_t value;

    float32_t current_sf =  1;
    float32_t voltage_sf =  1;

    uint32_t period      =  0;

    uint16_t load1       =  0;
    uint16_t load2       =  0;
    uint16_t load3       =  0;

    //Acknowledge interrupt
    ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

    // Blink LED
    isrLEDIter++;
    if(isrLEDIter>=ISR_LED_ITER_MAX)
    {
        isrLEDIter = 0;
        GPIO_togglePin(34);
    }

    // Get ADC values
    // Read converted phase C current
    adcValues.IC_value = (float32_t)ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0);

    // Read converted phase B current
    adcValues.IB_value = (float32_t)ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

    // Read converted phase A voltage
    adcValues.UA_value = (float32_t)ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);

    // Read converted phase B voltage
    adcValues.UB_value = (float32_t)ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER1);

    // Read converted phase C voltage
    adcValues.UC_value = (float32_t)ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER1);

    // Read converted dcBus voltage
    adcValues.UDC_value = (float32_t)ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER2);


    // Iterate over tables
    tableIter = (tableIter+1) % 24;
    period  = (uint32_t) EPWM_getTimeBasePeriod(EPWM1_BASE);
    load1   = (uint16_t) (sineTableA[tableIter]*period);
    load2   = (uint16_t) (sineTableB[tableIter]*period);
    load3   = (uint16_t) (sineTableC[tableIter]*period);
    EPWM_setCounterCompareValue(EPWM1_BASE,
                                EPWM_COUNTER_COMPARE_A,
                                load1);
    EPWM_setCounterCompareValue(EPWM2_BASE,
                                EPWM_COUNTER_COMPARE_A,
                                load2);
    EPWM_setCounterCompareValue(EPWM4_BASE,
                                EPWM_COUNTER_COMPARE_A,
                                load3);

}

/*  END OF FILE */
