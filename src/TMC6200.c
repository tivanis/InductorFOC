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

#include "TMC6200.h"

void TMC6200_initSPI(TMC6200_handle handle)
{
    // Write the default parameters to structure
    handle->spiBase     = (uint32_t) TMC6200_DEFAULT_SPI_BASE;
    handle->spilspclkHz = (uint32_t) TMC6200_DEFAULT_SPI_CLOCK;
    handle->spiProtocol = (SPI_TransferProtocol) TMC6200_DEFAULT_SPI_PROTOCOL;
    handle->spiMode     = (SPI_Mode) TMC6200_DEFAULT_SPI_MODE;
    handle->spiBitRate  = (uint32_t) TMC6200_DEFAULT_SPI_BITRATE;
    handle->spiDataWidth= (uint16_t) TMC6200_DEFAULT_SPI_DATAWIDTH;
    handle->spiEmu      = (SPI_EmulationMode) TMC6200_DEFAULT_SPI_EMU;

    handle->spiGPIONumber_CS = TMC6200_DEFAULT_SPI_CS_PIN;
    handle->GPIONumber_EN    = TMC6200_DEFAULT_ENABLE_PIN;
    handle->GPIONumber_FLT   = TMC6200_DEFAULT_FAULT_PIN;

    // Assignment of GPIOs and pin configuration
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);

    // Set SPIB MOSI pin on GPIO 24
    GPIO_setMasterCore(TMC6200_DEFAULT_SPI_MOSI_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_24_SPISIMOB);
    GPIO_setDirectionMode(TMC6200_DEFAULT_SPI_MOSI_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(TMC6200_DEFAULT_SPI_MOSI_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(TMC6200_DEFAULT_SPI_MOSI_PIN,GPIO_QUAL_ASYNC);

    // Set SPIB MISO pin on GPIO 31
    GPIO_setMasterCore(TMC6200_DEFAULT_SPI_MISO_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_31_SPISOMIB);
    GPIO_setDirectionMode(TMC6200_DEFAULT_SPI_MISO_PIN, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(TMC6200_DEFAULT_SPI_MISO_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(TMC6200_DEFAULT_SPI_MISO_PIN,GPIO_QUAL_ASYNC);

    // Set SPIB CLK pin on GPIO 22
    GPIO_setMasterCore(TMC6200_DEFAULT_SPI_CLK_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_22_SPICLKB);
    GPIO_writePin(TMC6200_DEFAULT_SPI_CLK_PIN, 0);
    GPIO_setDirectionMode(TMC6200_DEFAULT_SPI_CLK_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(TMC6200_DEFAULT_SPI_CLK_PIN, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(TMC6200_DEFAULT_SPI_CLK_PIN,GPIO_QUAL_ASYNC);
    GPIO_setAnalogMode(22, GPIO_ANALOG_DISABLED);

    // Set SPIB nCS pin on GPIO 33
    GPIO_setMasterCore(TMC6200_DEFAULT_SPI_CS_PIN, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_33_SPISTEB);
    GPIO_setDirectionMode(TMC6200_DEFAULT_SPI_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(TMC6200_DEFAULT_SPI_CS_PIN, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(TMC6200_DEFAULT_SPI_CS_PIN,GPIO_QUAL_ASYNC);
//    GPIO_writePin(handle->spiGPIONumber_CS, 1);

    GPIO_lockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_lockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_lockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);

    // Put SPI into reset before configuring
    SPI_disableModule(handle->spiBase);

    // Initialize SPI with set parameters
    SPI_setConfig(handle->spiBase, handle->spilspclkHz, handle->spiProtocol,
                  handle->spiMode, handle->spiBitRate, handle->spiDataWidth);

    // Disable the loopback
    SPI_disableLoopback(handle->spiBase);

    // Set default mode when debugging
    SPI_setEmulationMode(handle->spiBase, TMC6200_DEFAULT_SPI_EMU);

    // Hardware FIFO for Rx ant Tx will be
    SPI_enableFIFO(handle->spiBase);

    // Set default transmit delay at zero (hardcoded)
    SPI_setTxFifoTransmitDelay(handle->spiBase, 0);

    // Clear interrupt status
    SPI_clearInterruptStatus(handle->spiBase, SPI_INT_TXFF);

    // Configuration complete. Enable the module
    SPI_enableModule(handle->spiBase);

}

void TMC6200_readRegisters(TMC6200_handle handle)
{
    TMC6200_CtrlWord_t  controlWord;

    // Build the control word with dummy data to read GCONF register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                        TMC6200_ADDRESS_GCONF,
                                        TMC6200_DUMMY_DATA);

    // Send dummy data over SPI, then read GCONF over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_GCONF);

    // Save data to GCONF register
    // Fill the control structures
    handle->GCONF.DISABLE                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GCONF_DISABLE_BITS));

    handle->GCONF.IF_MODE                   = (TMC6200_GCONF_IF_MODE_e)
            (controlWord.data & ((uint32_t) TMC6200_GCONF_IF_MODE_BITS));

    handle->GCONF.FAULTDIRECT               = (TMC6200_GCONF_FAULTDIRECT_e)
            (controlWord.data & ((uint32_t) TMC6200_GCONF_FAULTDIRECT_BITS));

    handle->GCONF.AMPLIFICATION             = (TMC6200_GCONF_AMP_e)
            (controlWord.data & ((uint32_t) TMC6200_GCONF_AMP_BITS));

    handle->GCONF.AMP_OFF                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GCONF_AMP_OFF_BITS));

    handle->GCONF.TEST_MODE                 = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GCONF_TEST_MODE_BITS));

    // Build the control word with dummy data to read GCONF register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                        TMC6200_ADDRESS_GSTAT,
                                        TMC6200_DUMMY_DATA);

    // Send dummy data over SPI, then read over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_GSTAT);

    // Fill the GSTAT structure
    handle->GSTAT.RESET                     = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_RST_STATUS_BITS));

    handle->GSTAT.OVERTEMP_PREWARNING       = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_OTPW_BITS));

    handle->GSTAT.OVERTEMP                  = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_OT_BITS));

    handle->GSTAT.CHARGEPUMP_UNDERVOLTAGE   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_UVLO_BITS));

    handle->GSTAT.SHORT_PH_U                = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_SHORT_U_BITS));

    handle->GSTAT.SHORT_GND_U               = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_GND_SHORT_U_BITS));

    handle->GSTAT.SHORT_VS_U                = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_VS_SHORT_U_BITS));

    handle->GSTAT.SHORT_PH_V                = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_SHORT_V_BITS));

    handle->GSTAT.SHORT_GND_V               = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_GND_SHORT_V_BITS));

    handle->GSTAT.SHORT_VS_V                = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_VS_SHORT_V_BITS));

    handle->GSTAT.SHORT_PH_W                = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_SHORT_W_BITS));

    handle->GSTAT.SHORT_GND_W               = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_GND_SHORT_W_BITS));

    handle->GSTAT.SHORT_VS_W                = (bool)
            (controlWord.data & ((uint32_t) TMC6200_GSTAT_VS_SHORT_W_BITS));

    // Build the control word with dummy data to read IOIN register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                        TMC6200_ADDRESS_IOIN,
                                        TMC6200_DUMMY_DATA);

    // Send dummy data over SPI, then read over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_IOIN);

    // Read the INPUT status register
    handle->IOIN.INPUT_UH                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_UH_BITS));

    handle->IOIN.INPUT_UL                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_UL_BITS));

    handle->IOIN.INPUT_VH                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_VH_BITS));

    handle->IOIN.INPUT_VL                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_VL_BITS));

    handle->IOIN.INPUT_WH                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_WH_BITS));

    handle->IOIN.INPUT_WL                   = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_WL_BITS));

    handle->IOIN.INPUT_DRV_EN               = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_DRV_EN_BITS));

    handle->IOIN.OTP_PW                     = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_UL_BITS));

    handle->IOIN.OTP_136                    = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_OT136C_BITS));

    handle->IOIN.OTP_143                    = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_OT143C_BITS));

    handle->IOIN.OTP_150                    = (bool)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_OT150C_BITS));

    handle->IOIN.TMC6200_VERSION            = (uint16_t)
            (controlWord.data & ((uint32_t) TMC6200_IOIN_VERSION_BITS));

    // Build the control word with dummy data to read OTP_READ register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                        TMC6200_ADDRESS_OTP_READ,
                                        TMC6200_DUMMY_DATA);

    // Send dummy data over SPI, then read over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_OTP_READ);

    // Fill the OTP_READ register structure
    handle->OTP_READ.OTP_FCLK_TRIM          = (uint16_t)
            (controlWord.data & ((uint32_t) TMC6200_OTP_READ_OTP_FCLKTRIM_BITS));

    handle->OTP_READ.OTP_S2_LEVEL           = (TMC6200_OTP_READ_SHORT_LEVEL_e)
            (controlWord.data & ((uint32_t) TMC6200_OTP_READ_S2LEVEL_BITS));

    handle->OTP_READ.OTP_BBM                = (TMC6200_OTP_READ_BBM_e)
            (controlWord.data & ((uint32_t) TMC6200_OTP_READ_BBM_BITS));

    // Build the control word with dummy data to read FACTORY_CONF register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                        TMC6200_ADDRESS_FACTORY_CONF,
                                        TMC6200_DUMMY_DATA);

    // Send dummy data over SPI, then read over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_FACTORY_CONF);

    // Fill the FACTORY_CONF register structure
    handle->FACTORY_CONF.FCLK_TRIM          = (uint16_t)
            (controlWord.data & ((uint32_t) TMC6200_FACTORY_CONF_BITS));

    // Build the control word with dummy data to read SHORT_CONF register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                          TMC6200_ADDRESS_SHORT_CONF,
                                          TMC6200_DUMMY_DATA);
    // Send dummy data over SPI, then read over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_SHORT_CONF);

    // Fill the SHORT_CONF register structure
    handle->SHORT_CONF.S2VS_LEVEL           = (TMC6200_SHORT_CONF_S2VS_LEVEL_e)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_VS_LVL_BITS));

    handle->SHORT_CONF.S2G_LEVEL            = (TMC6200_SHORT_CONF_S2G_LEVEL_e)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_GND_LVL_BITS));

    handle->SHORT_CONF.SHORT_FLTR           = (TMC6200_SHORT_CONF_SHORT_FLTR_e)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_FILTER_BITS));

    handle->SHORT_CONF.SHORT_DELAY          = (TMC6200_SHORT_CONF_SHORT_DELAY_e)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_SH_DLY_BITS));

    handle->SHORT_CONF.SHORT_RETRY          = (TMC6200_SHORT_CONF_SHORT_RETRY_e)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_RETRY_BITS));

    handle->SHORT_CONF.PROTECT_PARALLEL     = (bool)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_PRO_PAR_BITS));

    handle->SHORT_CONF.DISABLE_S2G          = (bool)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_DIS_GND_BITS));

    handle->SHORT_CONF.DISABLE_S2VS         = (bool)
            (controlWord.data & ((uint32_t) TMC6200_SHORT_CONF_DIS_VS_BITS));


    // Build the control word with dummy data to read DRV_CONF register
    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                          TMC6200_ADDRESS_DRV_CONF,
                                          TMC6200_DUMMY_DATA);
    // Send dummy data over SPI, then read over SPI. Return the ctrlWrd
    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    // Check that the received address is the same as receive address
    ASSERT(
    (controlWord.addressAndMode & TMC6200_8BIT_MASK)
        == TMC6200_ADDRESS_DRV_CONF);

    // Fill the DRV_CONF register structure
    handle->DRV_CONF.BBMCLKS                = (TMC6200_DRV_CONF_BBMCLKS_e)
            (controlWord.data & ((uint32_t) TMC6200_DRV_CONF_BBMCLK_BITS));

    handle->DRV_CONF.OVERTEMP               = (TMC6200_DRV_CONF_OVERTEMP_e)
            (controlWord.data & ((uint32_t) TMC6200_DRV_CONF_OTSEL_BITS));

    handle->DRV_CONF.DRV_STRENGTH           = (TMC6200_DRV_CONF_DRV_STRENGTH_e)
            (controlWord.data & ((uint32_t) TMC6200_DRV_CONF_DRV_STR_BITS));
}
