/*
 * init.c
 *
 *  Created on: 16. kol 2019.
 *      Author: Tomislav Ivanis
 */

#include "init.h"


//*****************************************************************************
//
//! \brief Function initializes gpio module.
//
//*****************************************************************************

void INIT_gpio(void)
{
    //! Unlock ports (enable GPIO registers to be modified)
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);

    //! SETUP GPIO 34 (GREEN LED)
    GPIO_setPinConfig(GPIO_34_GPIO34);
    GPIO_setPadConfig(34U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(34U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 23 (RED LED)
    GPIO_setPinConfig(GPIO_23_GPIO23);
    GPIO_setAnalogMode(23U, GPIO_ANALOG_DISABLED);
    GPIO_setPadConfig(23U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(23U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 8 (INTERRUPT 1)
    GPIO_setPinConfig(GPIO_8_GPIO8);
    GPIO_setDirectionMode(8U, GPIO_DIR_MODE_IN);
    GPIO_setInterruptPin(8U, GPIO_INT_XINT1);
    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_RISING_EDGE);
    GPIO_setQualificationMode(8U, GPIO_QUAL_6SAMPLE);
    GPIO_setQualificationPeriod(8U, 510);

    //! SETUP GPIO 27 (FAULT): Floating input (INTERRUPT 3)
    GPIO_setDirectionMode(TMC6200_DEFAULT_FAULT_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPinConfig(GPIO_27_GPIO27);
    GPIO_setPadConfig(TMC6200_DEFAULT_FAULT_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setInterruptPin(TMC6200_DEFAULT_FAULT_PIN, GPIO_INT_XINT3);
    GPIO_setInterruptType(GPIO_INT_XINT3, GPIO_INT_TYPE_RISING_EDGE);
    GPIO_setQualificationMode(TMC6200_DEFAULT_FAULT_PIN, GPIO_QUAL_6SAMPLE);
    GPIO_setQualificationPeriod(TMC6200_DEFAULT_FAULT_PIN, 510);

    //! SETUP GPIO 12 (ENABLE)
    GPIO_setPinConfig(GPIO_12_GPIO12);
    GPIO_setPadConfig(TMC6200_DEFAULT_ENABLE_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(TMC6200_DEFAULT_ENABLE_PIN, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 0 (ePWM1A - PWM_AH)
    GPIO_setPinConfig(GPIO_0_EPWM1A);
    GPIO_setPadConfig(0U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(0U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 1 (ePWM1B - PWM_AL)
    GPIO_setPinConfig(GPIO_1_EPWM1B);
    GPIO_setPadConfig(1U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(1U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 2 (ePWM2A - PWM_CH)
    GPIO_setPinConfig(GPIO_2_EPWM2A);
    GPIO_setPadConfig(2U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(2U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 3 (ePWM2B - PWM_CL)
    GPIO_setPinConfig(GPIO_3_EPWM2B);
    GPIO_setPadConfig(3U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(2U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 6 (ePWM4A - PWM_BH)
    GPIO_setPinConfig(GPIO_6_EPWM4A);
    GPIO_setPadConfig(6U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(6U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 7 (ePWM4B - PWM_BL)
    GPIO_setPinConfig(GPIO_7_EPWM4B);
    GPIO_setPadConfig(7U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(7U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 4 (ePWM3A)
    GPIO_setPinConfig(GPIO_4_EPWM3A);
    GPIO_setPadConfig(4U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(4U, GPIO_DIR_MODE_OUT);

    //! SETUP GPIO 5 (ePWM3B)
    GPIO_setPinConfig(GPIO_5_EPWM3B);
    GPIO_setPadConfig(5U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(5U, GPIO_DIR_MODE_OUT);

    //! Lock ports (disable further GPIO configuration)
    GPIO_lockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_lockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_lockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);
}

//*****************************************************************************
//
//! \brief Function initializes all used epwm modules.
//
//*****************************************************************************

void INIT_epwm(void)
{
    //! Handle variable for initialization
    const uint32_t pwmHandle[3] = {EPWM1_BASE, EPWM2_BASE, EPWM4_BASE};
    uint16_t cnt;

    //! Calculate period
    uint16_t periodCount = (uint16_t) (DEVICE_SYSCLK_FREQ_MHz *
                                        PWM_PERIOD_usec / 2.0);

    //! Freeze clock to all ePWM modules
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //! Trip out all ePWM modules: turns off the outputs (sets MOSFETS to Hi-Z)
    EPWM_forceTripZoneEvent(pwmHandle[0], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(pwmHandle[1], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(pwmHandle[2], EPWM_TZ_FORCE_EVENT_OST);

    for(cnt=0; cnt<3; ++cnt)
    {
        //! Set time base counter mode to up-down (in TBCTL register)
        EPWM_setTimeBaseCounterMode(pwmHandle[cnt], EPWM_COUNTER_MODE_UP_DOWN);

        //! Disable phase shift (in TBCTL register)
        EPWM_disablePhaseShiftLoad(pwmHandle[cnt]);

        //! Set period load mode to load from shadow register (in TBCTL register)
        EPWM_setPeriodLoadMode(pwmHandle[cnt],  EPWM_PERIOD_DIRECT_LOAD);

        //! Set sync pulse to be software generated (typically on interrupt)...
        //...(in TBCTL register)
        EPWM_setSyncOutPulseMode(pwmHandle[cnt],
                                 EPWM_SYNC_OUT_PULSE_ON_SOFTWARE);

        //! Set the prescaler (in TBCTL register)
        EPWM_setClockPrescaler(pwmHandle[cnt],
                               EPWM_CLOCK_DIVIDER_1,
                               EPWM_HSCLOCK_DIVIDER_1);

        //! Set to count up after sync pulse (in TBCTL register)
        EPWM_setCountModeAfterSync(pwmHandle[cnt], EPWM_COUNT_MODE_UP_AFTER_SYNC);

        //! Set emulation mode (in TBCTL register)
        EPWM_setEmulationMode(pwmHandle[cnt], EPWM_EMULATION_FREE_RUN);

        //! Set the Timer-Based Phase Register (TBPHS)
        EPWM_setPhaseShift(pwmHandle[cnt], 0);

        //! Reset the ePWM3 counter register (TBCTR)
        EPWM_setTimeBaseCounter(pwmHandle[cnt], 0U);

        //! Set the time base period register (TBPRD)
        //! Initially set to zero to prevent interrupts
        EPWM_setTimeBasePeriod(pwmHandle[cnt], 0U);

        //! Set on which event will compare registers load from their respective...
        //! ...shadow load registers. Will be set for CMPA (with CMPCTL register)
        EPWM_setCounterCompareShadowLoadMode(pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_A,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);
        EPWM_disableCounterCompareShadowLoadMode(pwmHandle[cnt],
                                                 EPWM_COUNTER_COMPARE_B);
        EPWM_disableCounterCompareShadowLoadMode(pwmHandle[cnt],
                                                 EPWM_COUNTER_COMPARE_C);
        EPWM_disableCounterCompareShadowLoadMode(pwmHandle[cnt],
                                                 EPWM_COUNTER_COMPARE_D);

        //! Set action qualifier action (with AQCTLA)
        EPWM_setActionQualifierAction(pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_HIGH,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
        EPWM_setActionQualifierAction(pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_LOW,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

        //! Set the Action-qualifier Continuous Software Force Register...
        // ... (AQCSFRC)
        EPWM_setActionQualifierContSWForceAction(pwmHandle[cnt],
                                                 EPWM_AQ_OUTPUT_B,
                                                 EPWM_AQ_SW_OUTPUT_HIGH);

        //! Set the Dead-Band Generator Control Register (DBCTL) to apply...
        //...deadband on rising and falling edge
        EPWM_setDeadBandDelayMode(pwmHandle[cnt], EPWM_DB_RED, true);
        EPWM_setDeadBandDelayMode(pwmHandle[cnt], EPWM_DB_FED, true);

        //! Set EPWMA as the input to the dead band generator
        EPWM_setRisingEdgeDeadBandDelayInput(pwmHandle[cnt],
                                             EPWM_DB_INPUT_EPWMA);

        //! Configure the right polarity for active high complementary config.
        //! Also, ePWMxB here becomes don't care signal with DBCTL[IN_MODE]=00
        EPWM_setDeadBandDelayPolarity(pwmHandle[cnt],
                                      EPWM_DB_RED,
                                      EPWM_DB_POLARITY_ACTIVE_HIGH);
        EPWM_setDeadBandDelayPolarity(pwmHandle[cnt],
                                      EPWM_DB_FED,
                                      EPWM_DB_POLARITY_ACTIVE_LOW);

        //! Set the Dead-Band Rising Edge Delay Register (DBRED)
        EPWM_setRisingEdgeDelayCount(pwmHandle[cnt], PWM_DBRED_CNT);

        //! Set the Dead-Band Falling Edge Delay Register (DBFED)
        EPWM_setFallingEdgeDelayCount(pwmHandle[cnt], PWM_DBFED_CNT);

        //! Disable Chopper by writing toPWM-Chopper Control Register (PCCTL)
        EPWM_disableChopper(pwmHandle[cnt]);

        //! Setup the Trip Zone Select Register (TZSEL)
        EPWM_disableTripZoneSignals(pwmHandle[cnt],
                                    EPWM_TZ_SIGNAL_CBC1 |
                                    EPWM_TZ_SIGNAL_CBC2 |
                                    EPWM_TZ_SIGNAL_CBC3 |
                                    EPWM_TZ_SIGNAL_CBC4 |
                                    EPWM_TZ_SIGNAL_CBC5 |
                                    EPWM_TZ_SIGNAL_CBC6 |
                                    EPWM_TZ_SIGNAL_DCAEVT2 |
                                    EPWM_TZ_SIGNAL_DCBEVT2 |
                                    EPWM_TZ_SIGNAL_OSHT1 |
                                    EPWM_TZ_SIGNAL_OSHT2 |
                                    EPWM_TZ_SIGNAL_OSHT3 |
                                    EPWM_TZ_SIGNAL_OSHT4 |
                                    EPWM_TZ_SIGNAL_OSHT5 |
                                    EPWM_TZ_SIGNAL_OSHT6 |
                                    EPWM_TZ_SIGNAL_DCAEVT1 |
                                    EPWM_TZ_SIGNAL_DCBEVT1);

    }

    //! Set the Event Trigger Selection Register (ETSEL)
    //! Disable interrupts for ePWM. Main ISR will be on ADC end of conversion
    EPWM_disableInterrupt(pwmHandle[0]);
    EPWM_disableInterrupt(pwmHandle[1]);
    EPWM_disableInterrupt(pwmHandle[2]);

    //! Set ADC stert of conversion pulse source to be down-count of CMPC
    EPWM_setADCTriggerSource(pwmHandle[0],
                             EPWM_SOC_A,
                             EPWM_SOC_TBCTR_D_CMPC);

    //! Enable event trigger (ETSEL register, SOCASELCMP=1)
    EPWM_enableADCTrigger(pwmHandle[0], EPWM_SOC_A);

    //! Set ADC trigger prescale (ETPS register) and interrupt prescale
    ASSERT((numPWMTicksPerISRTick >= 1) && (numPWMTicksPerISRTick <= 15));
    EPWM_setInterruptEventCount(pwmHandle[0], numPWMTicksPerISRTick);
    EPWM_setADCTriggerEventPrescale(pwmHandle[0],
                                    EPWM_SOC_A,
                                    numPWMTicksPerISRTick);

    //! Set the Event Trigger Clear Register (ETCLR)
    EPWM_clearEventTriggerInterruptFlag(pwmHandle[0]);
    EPWM_clearADCTriggerFlag(pwmHandle[0], EPWM_SOC_A);

    //! Set periods of all used ePWM modules PWM is configured as...
    //...up/down counter, the period register is 0.5* of the desired PWM period
    EPWM_setTimeBasePeriod(pwmHandle[0], periodCount);
    EPWM_setTimeBasePeriod(pwmHandle[1], periodCount);
    EPWM_setTimeBasePeriod(pwmHandle[2], periodCount);

    //! Write the PWM data value  for ADC SOC trigger (Pre-roll count of 5)
    EPWM_setCounterCompareValue(pwmHandle[0],
                                EPWM_COUNTER_COMPARE_C,
                                SOC_PREROLL_CNT);

    //! Set initial Compare Values to help start Driver. D=50%.
    EPWM_setCounterCompareValue(pwmHandle[0],
                                EPWM_COUNTER_COMPARE_A,
                                (uint16_t) (0.5*periodCount));
    EPWM_setCounterCompareValue(pwmHandle[1],
                                EPWM_COUNTER_COMPARE_A,
                                (uint16_t) (0.5*periodCount));
    EPWM_setCounterCompareValue(pwmHandle[2],
                                EPWM_COUNTER_COMPARE_A,
                                (uint16_t) (0.5*periodCount));
    //Unfreeze ePWM clock
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

}

void INIT_adc(void)
{
    uint32_t adcHandle[3] = {ADCA_BASE, ADCB_BASE, ADCC_BASE};

    //! Set ADC references to internal (ANAREFCTL registers)
    SysCtl_delay(100U);
    ADC_setVREF(adcHandle[2], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(adcHandle[1], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    ADC_setVREF(adcHandle[0], ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    SysCtl_delay(100U);

    //! Configure internal reference as 1.65V*2 = 3.3V (ANAREFCTL register)
    ASysCtl_setAnalogReference1P65(ASYSCTL_VREFHIA |
                                   ASYSCTL_VREFHIB |
                                   ASYSCTL_VREFHIC);

    //! Enable internal voltage reference (ANAREFCTL)
    ASysCtl_setAnalogReferenceInternal(ASYSCTL_VREFHIA |
                                       ASYSCTL_VREFHIB |
                                       ASYSCTL_VREFHIC);

    //! Set main clock scaling factor (50MHz max clock for the ADC module)
    ADC_setPrescaler(adcHandle[0], ADC_CLK_DIV_2_0);
    ADC_setPrescaler(adcHandle[1], ADC_CLK_DIV_2_0);
    ADC_setPrescaler(adcHandle[2], ADC_CLK_DIV_2_0);

    //! Set the ADC interrupt pulse generation to end of conversion
    ADC_setInterruptPulseMode(adcHandle[0], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(adcHandle[1], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(adcHandle[2], ADC_PULSE_END_OF_CONV);

    //! Enable the ADCs
    ADC_enableConverter(adcHandle[0]);
    ADC_enableConverter(adcHandle[1]);
    ADC_enableConverter(adcHandle[2]);

    //! Set priority of SOCs
    ADC_setSOCPriority(adcHandle[0], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(adcHandle[1], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(adcHandle[2], ADC_PRI_ALL_HIPRI);

    //! Delay to allow ADCs to power up
    SysCtl_delay(1000U);

    //! Configure the interrupt when SOC 2 sampling sequence finishes
    //! Configure the sample window to 15 system clocks (assigning 14...
    //! ...to the ACQPS of ADCSOCxCTL Register).
    ADC_setInterruptSource(adcHandle[2], ADC_INT_NUMBER1, ADC_SOC_NUMBER2);

    //! Configure SOCs (setup hardware controlled queue: SOC0->SOC1->SOC2)

    //! SOC0 group:
    // ISENSE_C: set to sample with ADCC, Channel 3 (PIN31)
    ADC_setupSOC(adcHandle[2], ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN3, ADC_SAMPLE_WINDOW);

    // ISENSE_B: set to sample with ADCA, Channel 3 (PIN10)
    ADC_setupSOC(adcHandle[0], ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN3, ADC_SAMPLE_WINDOW);

    //! SOC1 group:
    // VSENSE_A: set to sample with ADCA, Channel 6 (PIN6)
    ADC_setupSOC(adcHandle[0], ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN6, ADC_SAMPLE_WINDOW);

    // VSENSE_B: set to sample with ADCB, Channel 6 (PIN9)
    ADC_setupSOC(adcHandle[1], ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN6, ADC_SAMPLE_WINDOW);

    // VSENSE_C: set to sample with ADCC, Channel 14 (PIN44)
    ADC_setupSOC(adcHandle[2], ADC_SOC_NUMBER1, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN14, ADC_SAMPLE_WINDOW);

    //! SOC2 group:
    // VDC SENSE: set to sample with ADCC, Channel 1 (PIN29)
    ADC_setupSOC(adcHandle[2], ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA,
                 ADC_CH_ADCIN1, ADC_SAMPLE_WINDOW);

}


void INIT_sci()
{
    SCI_setParityMode(SCIA_BASE, SCI_CONFIG_PAR_NONE);

}
/*  END OF FILE */
