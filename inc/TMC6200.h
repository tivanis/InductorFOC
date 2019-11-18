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

#ifndef INC_TMC6200_H_
#define INC_TMC6200_H_

//#define TMC6200_ADVANCED_MODE

#include "spi.h"
#include "gpio.h"
#include "pin_map.h"
#include "device.h"

// ****************************************************************************

//! \brief Default SPI parameter defines. Default transfer speed is 400 kb/s
//!        with LSPCLK of 25MHz frequency
//!
#define TMC6200_DEFAULT_SPI_BASE        (SPIB_BASE)
#define TMC6200_DEFAULT_SPI_CLOCK       (DEVICE_LSPCLK_FREQ)
#define TMC6200_DEFAULT_SPI_PROTOCOL    (SPI_PROT_POL1PHA0)
#define TMC6200_DEFAULT_SPI_MODE        (SPI_MODE_MASTER)
#define TMC6200_DEFAULT_SPI_BITRATE     400000
#define TMC6200_DEFAULT_SPI_DATAWIDTH   8

#define TMC6200_DEFAULT_SPI_EMU         (SPI_EMULATION_FREE_RUN)

#define TMC6200_SPI_FIFO_LEN            (SPI_FIFO_RX5)

#define TMC6200_SPI_TIMEOUT             0xFFFF

//! \brief Default dummy data when sending read register request at TMC6200
//!
#define TMC6200_DUMMY_DATA              0x00000000

//! \brief Default GPIO pins for enable and fault lines
//!
#define TMC6200_DEFAULT_ENABLE_PIN      12U
#define TMC6200_DEFAULT_FAULT_PIN       27U

//! \brief Default GPIO pins for SPI communication
//!
#define TMC6200_DEFAULT_SPI_MOSI_PIN    24U
#define TMC6200_DEFAULT_SPI_MISO_PIN    31U
#define TMC6200_DEFAULT_SPI_CS_PIN      33U
#define TMC6200_DEFAULT_SPI_CLK_PIN     22U

//! \brief Defines size of default communication array for TMC6200
//!
#define TMC6200_SIZEOF_COMM_ARRAY       5
#define TMC6200_8BIT_MASK               0x000000FF

// ****************************************************************************

//! \brief Defines the R/W mask
//!
#define TMC6200_RW_MASK                 0x80

//! \brief Defines the location of register with global configuration flags
//!
#define TMC6200_GCONF_REGISTER          0x00

//! \brief Defines the location of register with global status flags
//!
#define TMC6200_GSTAT_REGISTER          0x01

//! \brief Defines the location of register with input states status
//!
#define TMC6200_IOIN_REGISTER           0x04

//! \brief Defines the location of register for OTP programming access
//!
#define TMC6200_OTP_PROG_REGISTER       0x06

//! \brief Defines the location of register for OTP reading
//!
#define TMC6200_OTP_READ_REGISTER       0x07

//! \brief Defines the location of register for clock trimming
//!
#define TMC6200_FACTORY_CONF_REGISTER   0x08

//! \brief Defines the location of register for short-circuit configuration
//!
#define TMC6200_SHORT_CONF_REGISTER     0x09

//! \brief Defines the location of register for drive configuration
//!
#define TMC6200_DRV_CONF_REGISTER       0x0A

//! \brief Defines the bit locations for GCONF REGISTER
//!
#define TMC6200_GCONF_DISABLE_BITS      0x00000001 //!(1 << 0)
#define TMC6200_GCONF_IF_MODE_BITS      0x00000002 //!(1 << 1)
#define TMC6200_GCONF_FAULTDIRECT_BITS  0x00000004 //!(1 << 2)
#define TMC6200_GCONF_AMP_BITS          0x00000030 //!(3 << 4)
#define TMC6200_GCONF_AMP_OFF_BITS      0x00000040 //!(1 << 6)
#define TMC6200_GCONF_TEST_MODE_BITS    0x00000080 //!(1 << 7)

//! \brief Defines the bit locations for GSTAT REGISTER
//!
#define TMC6200_GSTAT_RST_STATUS_BITS   0x00000001 //!(1 << 0)
#define TMC6200_GSTAT_OTPW_BITS         0x00000002 //!(1 << 1)
#define TMC6200_GSTAT_OT_BITS           0x00000004 //!(1 << 2)
#define TMC6200_GSTAT_UVLO_BITS         0x00000008 //!(1 << 3)
#define TMC6200_GSTAT_SHORT_U_BITS      0x00000010 //!(1 << 4)
#define TMC6200_GSTAT_GND_SHORT_U_BITS  0x00000020 //!(1 << 5)
#define TMC6200_GSTAT_VS_SHORT_U_BITS   0x00000040 //!(1 << 6)
#define TMC6200_GSTAT_SHORT_V_BITS      0x00000100 //!(1 << 8)
#define TMC6200_GSTAT_GND_SHORT_V_BITS  0x00000200 //!(1 << 9)
#define TMC6200_GSTAT_VS_SHORT_V_BITS   0x00000400 //!(1 << 10)
#define TMC6200_GSTAT_SHORT_W_BITS      0x00001000 //!(1 << 12)
#define TMC6200_GSTAT_GND_SHORT_W_BITS  0x00002000 //!(1 << 13)
#define TMC6200_GSTAT_VS_SHORT_W_BITS   0x00004000 //!(1 << 14)

//! \brief Defines the bit locations for IOIN REGISTER
//!
#define TMC6200_IOIN_UL_BITS            0x00000001 //!(1 << 0)
#define TMC6200_IOIN_UH_BITS            0x00000002 //!(1 << 1)
#define TMC6200_IOIN_VL_BITS            0x00000004 //!(1 << 2)
#define TMC6200_IOIN_VH_BITS            0x00000008 //!(1 << 3)
#define TMC6200_IOIN_WL_BITS            0x00000010 //!(1 << 4)
#define TMC6200_IOIN_WH_BITS            0x00000020 //!(1 << 5)
#define TMC6200_IOIN_DRV_EN_BITS        0x00000040 //!(1 << 6)
#define TMC6200_IOIN_0_BITS             0x00000080 //!(1 << 7)
#define TMC6200_IOIN_OTPW_BITS          0x00000100 //!(1 << 8)
#define TMC6200_IOIN_OT136C_BITS        0x00000200 //!(1 << 9)
#define TMC6200_IOIN_OT143C_BITS        0x00000400 //!(1 << 10)
#define TMC6200_IOIN_OT150C_BITS        0x00000800 //!(1 << 11)
#define TMC6200_IOIN_VERSION_BITS       0xFF000000 //!(255 << 24)

#ifdef  TMC6200_ADVANCED_MODE
//! \brief Defines the bit locations for OTP_PROG REGISTER
//!
#define TMC6200_OTP_PROG_OTP_SEL_BITS   0x00000007 //!(7 << 0)
#define TMC6200_OTP_PROG_BYTE_BITS      0x00000030 //!(3 << 4)
#define TMC6200_OTP_PROG_OTP_MAGIC_BITS 0x00007F00 //!(127 << 8)
#endif

//! \brief Defines the bit locations for OTP_READ REGISTER
//!
#define TMC6200_OTP_READ_OTP_FCLKTRIM_BITS  0x0000001F //!(31 << 0)
#define TMC6200_OTP_READ_S2LEVEL_BITS       0x00000020 //!(1 << 5)
#define TMC6200_OTP_READ_BBM_BITS           0x000000C0 //!(3 << 6)

//! \brief Defines the bit locations for FACTORY_CONF
//!
#define TMC6200_FACTORY_CONF_BITS           0x0000001F //!(31 << 0)

//! \brief Defines the bit locations for SHORT_CONF REGISTER
//!
#define TMC6200_SHORT_CONF_VS_LVL_BITS  0x0000000F //!(15 << 0)
#define TMC6200_SHORT_CONF_GND_LVL_BITS 0x00000F00 //!(15 << 8)
#define TMC6200_SHORT_CONF_FILTER_BITS  0x00030000 //!(3 << 16)
#define TMC6200_SHORT_CONF_SH_DLY_BITS  0x00100000 //!(1 << 20)
#define TMC6200_SHORT_CONF_RETRY_BITS   0x03000000 //!(3 << 24)
#define TMC6200_SHORT_CONF_PRO_PAR_BITS 0x10000000 //!(1 << 28)
#define TMC6200_SHORT_CONF_DIS_GND_BITS 0x20000000 //!(1 << 29)
#define TMC6200_SHORT_CONF_DIS_VS_BITS  0x40000000 //!(1 << 30)

//! \brief Defines the bit locations for DRV_CONF REGISTER
//!
#define TMC6200_DRV_CONF_BBMCLK_BITS    0x0000001F //!(31 << 0)
#define TMC6200_DRV_CONF_OTSEL_BITS     0x00030000 //!(3 << 16)
#define TMC6200_DRV_CONF_DRV_STR_BITS   0x000C0000 //!(3 << 18)

// **************************************************************************

#define TMC6200_GCONF_DISABLE_SHIFT         0
#define TMC6200_GCONF_AMP_OFF_SHIFT         6
#define TMC6200_GCONF_TEST_MODE_SHIFT       7

#define TMC6200_GSTAT_RST_STATUS_SHIFT      0
#define TMC6200_GSTAT_OTPW_SHIFT            1
#define TMC6200_GSTAT_OT_SHIFT              2
#define TMC6200_GSTAT_UVLO_SHIFT            3
#define TMC6200_GSTAT_SHORT_U_SHIFT         4
#define TMC6200_GSTAT_GND_SHORT_U_SHIFT     5
#define TMC6200_GSTAT_VS_SHORT_U_SHIFT      6
#define TMC6200_GSTAT_SHORT_V_SHIFT         8
#define TMC6200_GSTAT_GND_SHORT_V_SHIFT     9
#define TMC6200_GSTAT_VS_SHORT_V_SHIFT      10
#define TMC6200_GSTAT_SHORT_W_SHIFT         12
#define TMC6200_GSTAT_GND_SHORT_W_SHIFT     13
#define TMC6200_GSTAT_VS_SHORT_W_SHIFT      14

#define TMC6200_IOIN_UL_SHIFT               0
#define TMC6200_IOIN_UH_SHIFT               1
#define TMC6200_IOIN_VL_SHIFT               2
#define TMC6200_IOIN_VH_SHIFT               3
#define TMC6200_IOIN_WL_SHIFT               4
#define TMC6200_IOIN_WH_SHIFT               5
#define TMC6200_IOIN_DRV_EN_SHIFT           6
#define TMC6200_IOIN_0_SHIFT                7
#define TMC6200_IOIN_OTPW_SHIFT             8
#define TMC6200_IOIN_OT136C_SHIFT           9
#define TMC6200_IOIN_OT143C_SHIFT           10
#define TMC6200_IOIN_OT150C_SHIFT           11
#define TMC6200_IOIN_VERSION_SHIFT          24

#ifdef  TMC6200_ADVANCED_MODE
#define TMC6200_OTP_PROG_OTP_SEL_SHIFT      0
#define TMC6200_OTP_PROG_BYTE_SHIFT         4
#define TMC6200_OTP_PROG_OTP_MAGIC_SHIFT    8
#endif

#define TMC6200_OTP_READ_BBM_SHIFT          0
#define TMC6200_OTP_READ_S2LEVEL_SHIFT      5
#define TMC6200_OTP_READ_OTP_FCLKTRIM_SHIFT 6

#define TMC6200_FACTORY_CONF_SHIFT          0

#define TMC6200_SHORT_CONF_VS_LVL_SHIFT     0
#define TMC6200_SHORT_CONF_GND_LVL_SHIFT    8
#define TMC6200_SHORT_CONF_FILTER_SHIFT     16
#define TMC6200_SHORT_CONF_SH_DLY_SHIFT     20
#define TMC6200_SHORT_CONF_RETRY_SHIFT      24
#define TMC6200_SHORT_CONF_PRO_PAR_SHIFT    28
#define TMC6200_SHORT_CONF_DIS_GND_SHIFT    29
#define TMC6200_SHORT_CONF_DIS_VS_SHIFT     30

#define TMC6200_DRV_CONF_BBMCLK_SHIFT       0
#define TMC6200_DRV_CONF_OTSEL_SHIFT        16
#define TMC6200_DRV_CONF_DRV_STR_SHIFT      18

// **************************************************************************

//! \brief Enumeration for the R/W modes
//!
typedef enum
{
    TMC6200_CTRLMODE_READ       = 0x00, //!< Read Mode,     0 << 7
    TMC6200_CTRLMODE_WRITE      = 0x80  //!< Write Mode,    1 << 7
} TMC6200_CtrlMode_e;

//! \brief Enumeration for the register addresses
//!
typedef enum
{
    TMC6200_ADDRESS_GCONF           = 0x00,
    TMC6200_ADDRESS_GSTAT           = 0x01,
    TMC6200_ADDRESS_IOIN            = 0x04,
    TMC6200_ADDRESS_OTP_PROG        = 0x06,
    TMC6200_ADDRESS_OTP_READ        = 0x07,
    TMC6200_ADDRESS_FACTORY_CONF    = 0x08,
    TMC6200_ADDRESS_SHORT_CONF      = 0x09,
    TMC6200_ADDRESS_DRV_CONF        = 0x0A

} TMC6200_Address_e;

//! \brief Structure for building the control word
//!
typedef struct _TMC6200_CtrlWord_t
{
    uint16_t     addressAndMode;
    uint32_t     data;

} TMC6200_CtrlWord_t;

// **************************************************************************

//! \brief Enumeration for the current amplification in GCONF register
//!

typedef enum
{
    TMC6200_GCONF_CURR_AMP_05             = 0x00000000,     //! 0<<4
    TMC6200_GCONF_CURR_AMP_10             = 0x00000010,     //! 1<<4
    TMC6200_GCONF_CURR_AMP_10_ALT         = 0x00000020,     //! 2<<4
    TMC6200_GCONF_CURR_AMP_20             = 0x00000030      //! 3<<4

} TMC6200_GCONF_AMP_e;

//! \brief Enumeration for the interface mode (IF_MODE) in GCONF register
//!
typedef enum
{
    TMC6200_GCONF_IF_MODE_INDIVIDUAL      = 0x00000000,     //! 0<<1
    TMC6200_GCONF_IF_MODE_L_IN_H_EN       = 0x00000002      //! 1<<1

} TMC6200_GCONF_IF_MODE_e;

//! \brief Enumeration for the faultdirect n GCONF register.
//!        0: Fault activates when one of the half-bridges shuts down
//!        1: Fault activates when any OTPW or overcurrent happens
//!
typedef enum
{
    TMC6200_GCONF_FAULTDIRECT_LOOSE       = 0x00000000,    //! 0<<2
    TMC6200_GCONF_FAULTDIRECT_STRICT      = 0x00000004     //! 1<<2

} TMC6200_GCONF_FAULTDIRECT_e;

//! \brief Structure for GCONF control & configuration register
//!
typedef struct _TMC6200_GCONF_t
{
    bool                            DISABLE;
    TMC6200_GCONF_IF_MODE_e         IF_MODE;
    TMC6200_GCONF_FAULTDIRECT_e     FAULTDIRECT;
    TMC6200_GCONF_AMP_e             AMPLIFICATION;
    bool                            AMP_OFF;
    bool                            TEST_MODE;

} TMC6200_GCONF_t;

// **************************************************************************

//! \brief Structure for GSTAT status register
//!
typedef struct _TMC6200_GSTAT_t
{
    bool                            RESET;
    bool                            OVERTEMP_PREWARNING;
    bool                            OVERTEMP;
    bool                            CHARGEPUMP_UNDERVOLTAGE;
    bool                            SHORT_PH_U;
    bool                            SHORT_GND_U;
    bool                            SHORT_VS_U;
    bool                            SHORT_PH_V;
    bool                            SHORT_GND_V;
    bool                            SHORT_VS_V;
    bool                            SHORT_PH_W;
    bool                            SHORT_GND_W;
    bool                            SHORT_VS_W;

} TMC6200_GSTAT_t;

// **************************************************************************

//! \brief Structure for IOIN control & status register
//!
typedef struct _TMC6200_IOIN_t
{
    bool                            INPUT_UL;
    bool                            INPUT_UH;
    bool                            INPUT_VL;
    bool                            INPUT_VH;
    bool                            INPUT_WL;
    bool                            INPUT_WH;
    bool                            INPUT_DRV_EN;
    bool                            OTP_PW;
    bool                            OTP_136;
    bool                            OTP_143;
    bool                            OTP_150;
    uint16_t                        TMC6200_VERSION;

} TMC6200_IOIN_t;

// **************************************************************************

#ifdef TMC6200_ADVANCED_MODE
//! \brief Enum which defines the locations of possible programmable bits
//!        WARNING: clearing bits is not possible!
//!
typedef enum _TMC6200_OTP_PROG_OTPBIT_e
{
    TMC6200_OTP_PROG_OTPBIT_00      = 0,
    TMC6200_OTP_PROG_OTPBIT_01      = 1,
    TMC6200_OTP_PROG_OTPBIT_02      = 2,
    TMC6200_OTP_PROG_OTPBIT_03      = 3,
    TMC6200_OTP_PROG_OTPBIT_04      = 4,
    TMC6200_OTP_PROG_OTPBIT_05      = 5,
    TMC6200_OTP_PROG_OTPBIT_06      = 6,
    TMC6200_OTP_PROG_OTPBIT_07      = 7

} TMC6200_OTP_PROG_OTPBIT_e;

//! \brief Structure for OTP_PROG control register. It is used to program OTP
//!        memory bit by bit.
//!        WARNING: clearing bits is not possible!
//!
typedef struct _TMC6200_OTP_PROG_t
{
    TMC6200_OTP_PROG_OTPBIT_e       OTP_BIT;
    uint16_t                        OTP_BYTE;
    uint16_t                        OTP_MAGIC;

} TMC6200_OTP_PROG_t;

#endif

// **************************************************************************

typedef enum
{
    TMC6200_OTPBIT_SHORT_LEVEL6     = 0x00000000,     //! 0<<5
    TMC6200_OTPBIT_SHORT_LEVEL12    = 0x00000020      //! 1<<5

} TMC6200_OTP_READ_SHORT_LEVEL_e;

typedef enum
{
    TMC6200_OTPBIT_BBM0             = 0x00000000,     //! 0<<6
    TMC6200_OTPBIT_BBM1             = 0x00000040,     //! 1<<6
    TMC6200_OTPBIT_BBM2             = 0x00000080,     //! 2<<6
    TMC6200_OTPBIT_BBM3             = 0x000000C0      //! 3<<6

} TMC6200_OTP_READ_BBM_e;

//! \brief Structure for OTP_READ status register
//!
typedef struct _TMC6200_OTP_READ_t
{
    uint16_t                        OTP_FCLK_TRIM;
    TMC6200_OTP_READ_SHORT_LEVEL_e  OTP_S2_LEVEL;
    TMC6200_OTP_READ_BBM_e          OTP_BBM;

} TMC6200_OTP_READ_t;

// **************************************************************************

//! \brief Structure for FACTORY_CONF status & config register
//!        Used for clock trimming
//!
typedef struct _TMC6200_FACTORY_CONF_t
{
    uint16_t                         FCLK_TRIM;

} TMC6200_FACTORY_CONF_t;

// **************************************************************************
//! \brief Enumeration for defining threshold level of short to VS
//!
typedef enum
{
    TMC6200_S2VS_LEVEL0             = 0x00000000,     //! 0<<0
    TMC6200_S2VS_LEVEL1             = 0x00000001,     //! 1<<0
    TMC6200_S2VS_LEVEL2             = 0x00000002,     //! 2<<0
    TMC6200_S2VS_LEVEL3             = 0x00000003,     //! 3<<0
    TMC6200_S2VS_LEVEL4             = 0x00000004,     //! 4<<0
    TMC6200_S2VS_LEVEL5             = 0x00000005,     //! 5<<0
    TMC6200_S2VS_LEVEL6             = 0x00000006,     //! 6<<0
    TMC6200_S2VS_LEVEL7             = 0x00000007,     //! 7<<0
    TMC6200_S2VS_LEVEL8             = 0x00000008,     //! 8<<0
    TMC6200_S2VS_LEVEL9             = 0x00000009,     //! 9<<0
    TMC6200_S2VS_LEVEL10            = 0x0000000A,    //! 10<<0
    TMC6200_S2VS_LEVEL11            = 0x0000000B,    //! 11<<0
    TMC6200_S2VS_LEVEL12            = 0x0000000C,    //! 12<<0
    TMC6200_S2VS_LEVEL13            = 0x0000000D,    //! 13<<0
    TMC6200_S2VS_LEVEL14            = 0x0000000E,    //! 14<<0
    TMC6200_S2VS_LEVEL15            = 0x0000000F     //! 15<<0

} TMC6200_SHORT_CONF_S2VS_LEVEL_e;

//! \brief Enumeration for defining threshold level of short to GND
//!
typedef enum
{
    TMC6200_S2G_LEVEL0              = 0x00000000,     //! 0<<8
    TMC6200_S2G_LEVEL1              = 0x00000100,     //! 1<<8
    TMC6200_S2G_LEVEL2              = 0x00000200,     //! 2<<8
    TMC6200_S2G_LEVEL3              = 0x00000300,     //! 3<<8
    TMC6200_S2G_LEVEL4              = 0x00000400,     //! 4<<8
    TMC6200_S2G_LEVEL5              = 0x00000500,     //! 5<<8
    TMC6200_S2G_LEVEL6              = 0x00000600,     //! 6<<8
    TMC6200_S2G_LEVEL7              = 0x00000700,     //! 7<<8
    TMC6200_S2G_LEVEL8              = 0x00000800,     //! 8<<8
    TMC6200_S2G_LEVEL9              = 0x00000900,     //! 9<<8
    TMC6200_S2G_LEVEL10             = 0x00000A00,     //! 10<<8
    TMC6200_S2G_LEVEL11             = 0x00000B00,     //! 11<<8
    TMC6200_S2G_LEVEL12             = 0x00000C00,     //! 12<<8
    TMC6200_S2G_LEVEL13             = 0x00000D00,     //! 13<<8
    TMC6200_S2G_LEVEL14             = 0x00000E00,     //! 14<<8
    TMC6200_S2G_LEVEL15             = 0x00000F00      //! 15<<8

} TMC6200_SHORT_CONF_S2G_LEVEL_e;

//! \brief Enumeration for defining short circuit spike filter bandwidth
//!
typedef enum
{
    TMC6200_SHORT_FLTR_100NS        = 0x00000000,      //! 0<<16
    TMC6200_SHORT_FLTR_1US          = 0x00010000,      //! 1<<16
    TMC6200_SHORT_FLTR_2US          = 0x00020000,      //! 2<<16
    TMC6200_SHORT_FLTR_3US          = 0x00030000       //! 3<<16

} TMC6200_SHORT_CONF_SHORT_FLTR_e;

//! \brief Enumeration for short circuit detection delay (blanking time)
//!
typedef enum
{
    TMC6200_SHORT_DELAY_750NS       = 0x00000000,    //! 0<<20
    TMC6200_SHORT_DELAY_1500NS      = 0x00100000     //! 1<<20

} TMC6200_SHORT_CONF_SHORT_DELAY_e;

//! \brief Enumeration for short circuit detection retry number
//!
typedef enum
{
    TMC6200_SHORT_RETRY0            = 0x00000000,    //! 0<<20
    TMC6200_SHORT_RETRY1            = 0x01000000,    //! 1<<24
    TMC6200_SHORT_RETRY2            = 0x02000000,    //! 2<<24
    TMC6200_SHORT_RETRY3            = 0x03000000     //! 3<<24

} TMC6200_SHORT_CONF_SHORT_RETRY_e;

//! \brief Structure for SHORT_CONF control register. Contains short circuit
//!        detection settings
//!
typedef struct _TMC6200_SHORT_CONF_t
{
    TMC6200_SHORT_CONF_S2VS_LEVEL_e         S2VS_LEVEL;
    TMC6200_SHORT_CONF_S2G_LEVEL_e          S2G_LEVEL;
    TMC6200_SHORT_CONF_SHORT_FLTR_e         SHORT_FLTR;
    TMC6200_SHORT_CONF_SHORT_DELAY_e        SHORT_DELAY;
    TMC6200_SHORT_CONF_SHORT_RETRY_e        SHORT_RETRY;
    bool                                    PROTECT_PARALLEL;
    bool                                    DISABLE_S2G;
    bool                                    DISABLE_S2VS;

} TMC6200_SHORT_CONF_t;

// **************************************************************************
//! \brief Enumeration for setting break-before-make clock cycles
//!        typically 42ns/CLK
//!
typedef enum
{
    TMC6200_BBM_CLKS0               = 0x00000000,     //! 0<<0
    TMC6200_BBM_CLKS1               = 0x00000001,     //! 1<<0
    TMC6200_BBM_CLKS2               = 0x00000002,     //! 2<<0
    TMC6200_BBM_CLKS3               = 0x00000003,     //! 3<<0
    TMC6200_BBM_CLKS4               = 0x00000004,     //! 4<<0
    TMC6200_BBM_CLKS5               = 0x00000005,     //! 5<<0
    TMC6200_BBM_CLKS6               = 0x00000006,     //! 6<<0
    TMC6200_BBM_CLKS7               = 0x00000007,     //! 7<<0
    TMC6200_BBM_CLKS8               = 0x00000008,     //! 8<<0
    TMC6200_BBM_CLKS9               = 0x00000009,     //! 9<<0
    TMC6200_BBM_CLKS10              = 0x0000000A,    //! 10<<0
    TMC6200_BBM_CLKS11              = 0x0000000B,    //! 11<<0
    TMC6200_BBM_CLKS12              = 0x0000000C,    //! 12<<0
    TMC6200_BBM_CLKS13              = 0x0000000D,    //! 13<<0
    TMC6200_BBM_CLKS14              = 0x0000000E,    //! 14<<0
    TMC6200_BBM_CLKS15              = 0x0000000F     //! 15<<0

} TMC6200_DRV_CONF_BBMCLKS_e;

//! \brief Enumeration for setting over-temperature
//!
typedef enum
{
    TMC6200_150C                    = 0x00000000,     //! 0<<16
    TMC6200_143C                    = 0x00010000,     //! 1<<16
    TMC6200_136C                    = 0x00020000,     //! 2<<16
    TMC6200_120C                    = 0x00030000      //! 3<<16

} TMC6200_DRV_CONF_OVERTEMP_e;

//! \brief Enumeration for setting driver strength
//!
typedef enum
{
    TMC6200_DRV_WEAK                = 0x00000000,     //! 0<<18
    TMC6200_DRV_WEAK_TC             = 0x00040000,     //! 1<<18
    TMC6200_DRV_MEDIUM              = 0x00080000,     //! 2<<18
    TMC6200_DRV_STRONG              = 0x000C0000      //! 3<<18

} TMC6200_DRV_CONF_DRV_STRENGTH_e;

//! \brief Structure for drive configuration in DRV_CONF control register
//!
typedef struct _TMC6200_DRV_CONF_t
{
    TMC6200_DRV_CONF_BBMCLKS_e      BBMCLKS;
    TMC6200_DRV_CONF_OVERTEMP_e     OVERTEMP;
    TMC6200_DRV_CONF_DRV_STRENGTH_e DRV_STRENGTH;

} TMC6200_DRV_CONF_t;

// **************************************************************************
//! \brief Complete structure and communication object
//!
typedef struct _TMC6200_Obj_t
{
    // TMC6200 Registers
    TMC6200_GCONF_t                 GCONF;
    TMC6200_GSTAT_t                 GSTAT;
    TMC6200_IOIN_t                  IOIN;
    TMC6200_OTP_READ_t              OTP_READ;
    TMC6200_FACTORY_CONF_t          FACTORY_CONF;
    TMC6200_SHORT_CONF_t            SHORT_CONF;
    TMC6200_DRV_CONF_t              DRV_CONF;
#ifdef TMC6200_ADVANCED_MODE
    TMC6200_OTP_PROG_t              OTP_PROG;
#endif
    // TMC6200 SPI control variables
    uint32_t                        spiBase;
    uint32_t                        spilspclkHz;
    SPI_TransferProtocol            spiProtocol;
    SPI_Mode                        spiMode;
    uint32_t                        spiBitRate;
    uint16_t                        spiDataWidth;
    uint32_t                        spiGPIONumber_CS;
    SPI_EmulationMode               spiEmu;
    uint32_t                        GPIONumber_EN;
    uint32_t                        GPIONumber_FLT;

    // TMC6200 SPI software flags
    bool                            spiWriteCmd;
    bool                            spiReadCmd;

} TMC6200_Obj_t;

typedef TMC6200_Obj_t *TMC6200_handle;

// **************************************************************************
//  TMC6200 module function prototypes and inline functions
//
// **************************************************************************

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline void TMC6200_arrayFromCtrlWord(uint16_t *txArray,
                                             uint16_t txArraySize,
                                             TMC6200_CtrlWord_t ctrlWord)
{
    uint16_t iter;
    uint16_t temp;

    // Ctrl word must be broken into an array and formated in order to write
    // ...faster to SPI using for loop and SPI_writeDataBlockingFIFO() function
    // Every member of an array must be shifted to upper 8 bits...
    // ...if 8 bit word is used for SPI

    // Shift the address and mode part of the control word
    txArray[0] = (uint16_t) ((ctrlWord.addressAndMode) << 8);

    // Shift the data part of the control word
    for(iter = 1; iter < txArraySize; iter++)
    {
        temp = (uint16_t) ((ctrlWord.data) >> ((txArraySize - iter - 1) * 8));
        txArray[iter] = (uint16_t) ((temp) << 8);

    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline TMC6200_CtrlWord_t TMC6200_ctrlWordFromArray(
                                                    uint16_t *rxArray,
                                                    uint16_t rxArraySize)
{
    TMC6200_CtrlWord_t  ctrlWord;
    uint16_t            iter;
    uint32_t            temp;

    // Make sure the variables are clear
    ctrlWord.addressAndMode = (uint16_t) 0x0000;
    ctrlWord.data           = (uint32_t) 0x00000000;

    // Only the lower N bits of the rxArray contain data
    ctrlWord.addressAndMode = (uint16_t) (rxArray[0] & TMC6200_8BIT_MASK);

    for(iter = 1; iter < rxArraySize; iter++)
    {
        // Only the lower N bits of the rxArray contain data
        temp = (uint32_t) (rxArray[iter] & TMC6200_8BIT_MASK);
        ctrlWord.data = (uint32_t) (ctrlWord.data |
                        (temp << ((rxArraySize - iter - 1) * 8) ));
    }

    return ctrlWord;
}

//! \brief     Builds the control word
//! \param[in] ctrlMode  The control mode (read or write)
//! \param[in] regAddr   The register address
//! \param[in] data      The data (uint32_t)
//! \return    The control word structure
//!
static inline TMC6200_CtrlWord_t TMC6200_buildCtrlWord(
                                            const TMC6200_CtrlMode_e ctrlMode,
                                            const TMC6200_Address_e  regAddr,
                                            const uint32_t           data)
{
    TMC6200_CtrlWord_t ctrlWord;
    ctrlWord.addressAndMode = ctrlMode | regAddr;
    ctrlWord.data           = data;

    return(ctrlWord);
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline TMC6200_CtrlWord_t TMC6200_SPI_WriteRead(
                                TMC6200_handle handle,
                                TMC6200_CtrlWord_t controlWord)
{
    uint16_t            iter;

    // MISRA-C Compliant zero array initialization
    uint16_t            txArray[TMC6200_SIZEOF_COMM_ARRAY] = {0};
    uint16_t            rxArray[TMC6200_SIZEOF_COMM_ARRAY] = {0};

    // Initialize rxFIFO wait
    volatile SPI_RxFIFOLevel    rxFIFOCnt = SPI_FIFO_RXEMPTY;

    // Counter for possible timeout while receiving
    volatile uint16_t           waitCnt   = 0;

   // Format and populate the txArray with Control Word
   TMC6200_arrayFromCtrlWord(txArray, TMC6200_SIZEOF_COMM_ARRAY, controlWord);

   // Flush receive and transmit buffers and enable FIFO
   SPI_resetRxFIFO(handle->spiBase);
   SPI_resetTxFIFO(handle->spiBase);
   SPI_enableFIFO(handle->spiBase);

   // Fill the transmit FIFO.
   // Chip select (STE) is automatically lowered to "0"
   for(iter = 0; iter < TMC6200_SIZEOF_COMM_ARRAY; iter++)
   {
       SPI_writeDataBlockingFIFO(handle->spiBase, txArray[iter]);
   }

   // Wait while transmitting
   while(SPI_isBusy(handle->spiBase))
   {
       __asm(" NOP");
   }

   // Wait to receive all of the sent messages from the TMC6200
   // Chip select (STE) is automatically raised to "1" after finishing
   while((rxFIFOCnt<TMC6200_SPI_FIFO_LEN) && (waitCnt<TMC6200_SPI_TIMEOUT))
   {
       rxFIFOCnt   = SPI_getRxFIFOStatus(handle->spiBase);
       ++waitCnt;
   }

   // If the SPI has not timed out
   if (waitCnt<TMC6200_SPI_TIMEOUT)
   {
       // Receive from receive FIFO into an array
       for(iter = 0; iter < TMC6200_SIZEOF_COMM_ARRAY; iter++)
       {
           //  CPU will not freeze if the data has not been received
           rxArray[iter] = (uint16_t) SPI_readDataNonBlocking(handle->spiBase);
       }

       // Control word needs to be rebuilt to receive array
       controlWord = (TMC6200_CtrlWord_t) TMC6200_ctrlWordFromArray(
                                       rxArray,
                                       TMC6200_SIZEOF_COMM_ARRAY);
       return controlWord;
   }

   // Else SPI has timed out or an error occurred. Return idle line bits.
   else
   {
       controlWord.addressAndMode   = (uint16_t) 0xFFFF;
       controlWord.data             = (uint32_t) 0xFFFFFFFF;
       return controlWord;
   }

}

// **************************************************************************
//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_clearReset(TMC6200_handle handle)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;

    tempData =  0x00000000;
    tempData |= (handle->GSTAT.RESET << TMC6200_GSTAT_RST_STATUS_SHIFT);
    tempData |= (handle->GSTAT.OVERTEMP_PREWARNING
                            << TMC6200_GSTAT_OTPW_SHIFT);
    tempData |= (handle->GSTAT.CHARGEPUMP_UNDERVOLTAGE
                            << TMC6200_GSTAT_OTPW_SHIFT);
    tempData |= (handle->GSTAT.OVERTEMP << TMC6200_GSTAT_OT_SHIFT);
    tempData |= (handle->GSTAT.SHORT_GND_U << TMC6200_GSTAT_GND_SHORT_U_SHIFT);
    tempData |= (handle->GSTAT.SHORT_GND_V << TMC6200_GSTAT_GND_SHORT_V_SHIFT);
    tempData |= (handle->GSTAT.SHORT_GND_W << TMC6200_GSTAT_GND_SHORT_W_SHIFT);
    tempData |= (handle->GSTAT.SHORT_VS_U << TMC6200_GSTAT_VS_SHORT_U_SHIFT);
    tempData |= (handle->GSTAT.SHORT_VS_V << TMC6200_GSTAT_VS_SHORT_V_SHIFT);
    tempData |= (handle->GSTAT.SHORT_VS_W << TMC6200_GSTAT_VS_SHORT_W_SHIFT);
    tempData |= (handle->GSTAT.SHORT_PH_U << TMC6200_GSTAT_SHORT_U_SHIFT);
    tempData |= (handle->GSTAT.SHORT_PH_V << TMC6200_GSTAT_SHORT_V_SHIFT);
    tempData |= (handle->GSTAT.SHORT_PH_W << TMC6200_GSTAT_SHORT_W_SHIFT);

    // Write the changed bit to temp word
    tempData |= (uint32_t) TMC6200_GSTAT_RST_STATUS_BITS;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GSTAT,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write to main TMC6200 structure
        handle->GSTAT.RESET = (bool) false;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }

}


//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_driverDisable(TMC6200_handle handle)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;

    tempData =  0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    // Write the changed bit to temp word
    tempData |= (uint32_t) TMC6200_GCONF_DISABLE_BITS;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write to main TMC6200 structure
        handle->GCONF.DISABLE = (bool) true;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }

}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_driverEnable(TMC6200_handle handle)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;

    tempData =  0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    // Clear the bit in temporary word by writing "1"
    tempData |= (uint32_t) TMC6200_GCONF_DISABLE_BITS;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write to main TMC6200 structure
        handle->GCONF.DISABLE = (bool) (false);
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setInterfaceMode(TMC6200_handle handle,
                                            TMC6200_GCONF_IF_MODE_e IFMODE)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_GCONF_IF_MODE_BITS));
    tempData |= (uint32_t) IFMODE;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->GCONF.IF_MODE = IFMODE;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }

}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setAmplificationMode(
                            TMC6200_handle handle,
                            TMC6200_GCONF_AMP_e AMPLIFICATION)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_GCONF_AMP_BITS));
    tempData |= (uint32_t) AMPLIFICATION;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->GCONF.AMPLIFICATION = AMPLIFICATION;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_disableAmplifiers(TMC6200_handle handle)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    tempData |= (uint32_t) (TMC6200_GCONF_AMP_OFF_BITS);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->GCONF.AMP_OFF = (bool) true;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}


//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_enableAmplifiers(TMC6200_handle handle)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    tempData &= (0xFFFFFFFF ^ (uint32_t) TMC6200_GCONF_AMP_OFF_BITS);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->GCONF.AMP_OFF = (bool) false;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setFaultDirectMode(
                            TMC6200_handle handle,
                            TMC6200_GCONF_FAULTDIRECT_e FAULTDIRECT)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->GCONF.DISABLE  << TMC6200_GCONF_DISABLE_SHIFT);
    tempData |= (handle->GCONF.IF_MODE);
    tempData |= (handle->GCONF.FAULTDIRECT);
    tempData |= (handle->GCONF.AMPLIFICATION);
    tempData |= (handle->GCONF.AMP_OFF  << TMC6200_GCONF_AMP_OFF_SHIFT);
    tempData |= (handle->GCONF.TEST_MODE<< TMC6200_GCONF_TEST_MODE_SHIFT);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_GCONF_FAULTDIRECT_BITS));
    tempData |= (uint32_t) FAULTDIRECT;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_GCONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->GCONF.FAULTDIRECT = FAULTDIRECT;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setShortToVS(
                            TMC6200_handle handle,
                            TMC6200_SHORT_CONF_S2VS_LEVEL_e S2VS)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2G
                    << TMC6200_SHORT_CONF_DIS_VS_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2VS
                    << TMC6200_SHORT_CONF_DIS_GND_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.PROTECT_PARALLEL
                    << TMC6200_SHORT_CONF_PRO_PAR_SHIFT);
    tempData |= (handle->SHORT_CONF.S2G_LEVEL);
    tempData |= (handle->SHORT_CONF.S2VS_LEVEL);
    tempData |= (handle->SHORT_CONF.SHORT_DELAY);
    tempData |= (handle->SHORT_CONF.SHORT_FLTR);
    tempData |= (handle->SHORT_CONF.SHORT_RETRY);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_SHORT_CONF_VS_LVL_BITS));
    tempData |= (uint32_t) (S2VS);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_SHORT_CONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->SHORT_CONF.S2VS_LEVEL = S2VS;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setShortToGND(
                            TMC6200_handle handle,
                            TMC6200_SHORT_CONF_S2G_LEVEL_e S2G)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2G
                    << TMC6200_SHORT_CONF_DIS_VS_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2VS
                    << TMC6200_SHORT_CONF_DIS_GND_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.PROTECT_PARALLEL
                    << TMC6200_SHORT_CONF_PRO_PAR_SHIFT);
    tempData |= (handle->SHORT_CONF.S2G_LEVEL);
    tempData |= (handle->SHORT_CONF.S2VS_LEVEL);
    tempData |= (handle->SHORT_CONF.SHORT_DELAY);
    tempData |= (handle->SHORT_CONF.SHORT_FLTR);
    tempData |= (handle->SHORT_CONF.SHORT_RETRY);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_SHORT_CONF_GND_LVL_BITS));
    tempData |= (uint32_t) (S2G);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_SHORT_CONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->SHORT_CONF.S2G_LEVEL = S2G;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setShortFilter(
                            TMC6200_handle handle,
                            TMC6200_SHORT_CONF_SHORT_FLTR_e FILTER)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2G
                    << TMC6200_SHORT_CONF_DIS_VS_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2VS
                    << TMC6200_SHORT_CONF_DIS_GND_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.PROTECT_PARALLEL
                    << TMC6200_SHORT_CONF_PRO_PAR_SHIFT);
    tempData |= (handle->SHORT_CONF.S2G_LEVEL);
    tempData |= (handle->SHORT_CONF.S2VS_LEVEL);
    tempData |= (handle->SHORT_CONF.SHORT_DELAY);
    tempData |= (handle->SHORT_CONF.SHORT_FLTR);
    tempData |= (handle->SHORT_CONF.SHORT_RETRY);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_SHORT_CONF_FILTER_BITS));
    tempData |= (uint32_t) (FILTER);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_SHORT_CONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->SHORT_CONF.SHORT_FLTR = FILTER;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }

}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setShortDelay(
                            TMC6200_handle handle,
                            TMC6200_SHORT_CONF_SHORT_DELAY_e DLY)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= ( (uint32_t) handle->SHORT_CONF.DISABLE_S2G
                    << TMC6200_SHORT_CONF_DIS_VS_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.DISABLE_S2VS
                    << TMC6200_SHORT_CONF_DIS_GND_SHIFT);
    tempData |= ((uint32_t) handle->SHORT_CONF.PROTECT_PARALLEL
                    << TMC6200_SHORT_CONF_PRO_PAR_SHIFT);
    tempData |= (handle->SHORT_CONF.S2G_LEVEL);
    tempData |= (handle->SHORT_CONF.S2VS_LEVEL);
    tempData |= (handle->SHORT_CONF.SHORT_DELAY);
    tempData |= (handle->SHORT_CONF.SHORT_FLTR);
    tempData |= (handle->SHORT_CONF.SHORT_RETRY);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_SHORT_CONF_SH_DLY_BITS));
    tempData |= (uint32_t) (DLY);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_SHORT_CONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->SHORT_CONF.SHORT_DELAY = DLY;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setDriveStrength(
                            TMC6200_handle handle,
                            TMC6200_DRV_CONF_DRV_STRENGTH_e driveStrength)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->DRV_CONF.DRV_STRENGTH);
    tempData |= (handle->DRV_CONF.BBMCLKS);
    tempData |= (handle->DRV_CONF.OVERTEMP);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_DRV_CONF_DRV_STR_BITS));
    tempData |= (uint32_t) (driveStrength);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_DRV_CONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->DRV_CONF.DRV_STRENGTH = driveStrength;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline bool TMC6200_setOvertemp(
                            TMC6200_handle handle,
                            TMC6200_DRV_CONF_OVERTEMP_e overtemp)
{
    uint32_t            tempData;
    TMC6200_CtrlWord_t  controlWord;
    TMC6200_CtrlWord_t  returnWord;


    // Clear tempData variable and get old configuration bits from the...
    // ...main TMC6200 structure
    tempData = 0x00000000;
    tempData |= (handle->DRV_CONF.DRV_STRENGTH);
    tempData |= (handle->DRV_CONF.BBMCLKS);
    tempData |= (handle->DRV_CONF.OVERTEMP);

    // Write new bits to temporary data variable
    tempData &= (uint32_t)
                ((0xFFFFFFFF ^ TMC6200_DRV_CONF_OTSEL_BITS));
    tempData |= (uint32_t) (overtemp);

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_WRITE,
                                        TMC6200_ADDRESS_DRV_CONF,
                                        tempData);

    returnWord  = TMC6200_SPI_WriteRead(handle, controlWord);

    if((returnWord.addressAndMode != 0xFFFF)
            &&  (controlWord.data != 0XFFFFFFFF))
    {
        // Write is successful, write to main TMC6200 structure
        handle->DRV_CONF.OVERTEMP = overtemp;
        return true;
    }

    else
    {
        // SPI communication failed.
        return false;
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline void TMC6200_getFaults(TMC6200_handle handle)
{
    TMC6200_CtrlWord_t controlWord;

    controlWord = TMC6200_buildCtrlWord(TMC6200_CTRLMODE_READ,
                                        TMC6200_ADDRESS_GSTAT,
                                        TMC6200_DUMMY_DATA);

    controlWord = TMC6200_SPI_WriteRead(handle, controlWord);

    ASSERT(controlWord.addressAndMode == returnWord.addressAndMode);

    // Fill the GSTAT structure with new data
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

    // Cycle enable to clear fault
    //* TO DO: PROPER FAULT HANDLING *//
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline void TMC6200_deviceEnable()
{
    uint16_t n;
    GPIO_writePin(TMC6200_DEFAULT_ENABLE_PIN, 1);
    for(n=0; n<0xFFFF; ++n)
    {
        __asm("NOP");
    }
}

//! \brief     -
//! \param[in] -
//! \return    -
//!
static inline void TMC6200_deviceDisable()
{
    GPIO_writePin(TMC6200_DEFAULT_ENABLE_PIN, 0);
}

// **************************************************************************

//! \brief     -
//! \param[in] -
//! \return    -
//!
extern void TMC6200_initSPI(TMC6200_handle handle);

//! \brief     -
//! \param[in] -
//! \return    -
//!
extern void TMC6200_readRegisters(TMC6200_handle handle);


// **************************************************************************

#endif /* INC_TMC6200_H_ */
