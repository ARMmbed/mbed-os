/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__FSL_SIM_HAL_H__)
#define __FSL_SIM_HAL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_sim_features.h"

/*! @addtogroup sim_hal*/
/*! @{*/

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum _sim_clock_names {
 
   /* default clocks*/
   kSimCoreClock,                         /**/
   kSimSystemClock,                       /**/
   kSimPlatformClock,                     /**/
   kSimBusClock,                          /**/
   kSimFlexBusClock,                      /**/
   kSimFlashClock,                        /**/
 
   /* other internal clocks used by peripherals*/
   /* osc clock*/
   kSimOsc32kClock,
   kSimOsc0ErClock,
   kSimOsc1ErClock,

   /* irc 48Mhz clock */
   kSimIrc48mClock,

   /* rtc clock*/
   kSimRtc32kClock,
   kSimRtc1hzClock,

   /* lpo clcok*/
   kSimLpoClock,
 
   /* mcg clocks*/
   kSimMcgFllClock,
   kSimMcgPll0Clock,
   kSimMcgPll1Clock,
   kSimMcgOutClock,
   kSimMcgIrClock,
   
   /* constant clocks (provided in other header files?)*/
   kSimSDHC0_CLKIN,
   kSimENET_1588_CLKIN,
   kSimEXTAL_Clock,
   kSimEXTAL1_Clock,
   kSimUSB_CLKIN,
   
   /* reserved value*/
   kSimReserved,
 
   kSimClockNameCount
} sim_clock_names_t;

/*! @brief Clock gate module names */
typedef enum _sim_clock_gate_module_names {

    /* System modules*/
    kSimClockModuleDMA,                    /**/
    kSimClockModuleDMAMUX,                 /*   instance 0, 1*/
    kSimClockModulePORT,                   /*   instance 0 - 5 (A - F)*/
    kSimClockModuleMPU,                    /**/
    kSimClockModuleLLWU,                   /**/
    kSimClockModuleEWM,                    /**/

    /* Clocks*/
    kSimClockModuleOSC1,                   /**/

    /* Memory and memory interfaces*/
    kSimClockModuleFTF,                    /*  Flash memory control clock*/
    kSimClockModuleNFC,                    /*  NAND flash control clock*/
    kSimClockModuleFLEXBUS,                /**/
    kSimClockModuleDDR,                    /**/

    /* Security*/
    kSimClockModuleCRC,                    /**/
    kSimClockModuleRNGA,                   /**/
    kSimClockModuleREGFILE,                /**/
    kSimClockModuleDRYICESECREG,           /**/
    kSimClockModuleDRYICE,                 /**/

    /* Analog*/
    kSimClockModuleADC,                    /*   instance 0 - 3*/
    kSimClockModuleCMP,                    /* */
    kSimClockModuleDAC,                    /*   instance 0, 1*/
    kSimClockModuleVREF,                   /* */
    kSimClockModuleSAI,                    /*   instance 0, 1*/

    /* Timers*/
    kSimClockModuleTPM,                    /*   TPM timers 0 - 2*/
    kSimClockModulePDB,                    /* */
    kSimClockModuleFTM,                    /*   instance 0 - 3*/
    kSimClockModulePIT,                    /**/
    kSimClockModuleLPTIMER,                /**/
    kSimClockModuleCMT,                    /**/
    kSimClockModuleRTC,                    /**/

    /* Communication Interfaces*/
    kSimClockModuleENET,                   /**/
    kSimClockModuleUSBHS,                  /**/
    kSimClockModuleUSBFS,                  /**/
    kSimClockModuleUSBDCD,                 /**/
    kSimClockModuleFLEXCAN,                /*   instance 0, 1*/
    kSimClockModuleSPI,                    /*   instance 0 - 2*/
    kSimClockModuleI2C,                    /*   instance 0, 1*/
    kSimClockModuleUART,                   /*   instance 0 - 5*/
    kSimClockModuleESDHC,                  /**/
    kSimClockModuleLPUART,                 /**/

    /* Human-machine Interfaces*/
    kSimClockModuleTSI,                    /**/
    kSimClockModuleLCDC,                   /**/
    kSimClockModuleMax
} sim_clock_gate_module_names_t;

/*! @brief Clock source and sel names */
typedef enum _sim_clock_source_names {
    kSimClockNfcSrc,                   /* NFCSRC*/
    kSimClockEsdhcSrc,                 /* ESDHCSRC K70*/
    kSimClockSdhcSrc,                  /* SDHCSRC  K64*/
    kSimClockLcdcSrc,                  /* LCDCSRC*/
    kSimClockTimeSrc,                  /* TIMESRC*/
    kSimClockRmiiSrc,                  /* RMIISRC*/
    kSimClockUsbfSrc,                  /* USBFSRC  K70*/
    kSimClockUsbSrc,                   /* USBSRC   K64, KL25, KV31, and K22*/
    kSimClockUsbhSrc,                  /* USBHSRC*/
    kSimClockUart0Src,                 /* UART0SRC*/
    kSimClockTpmSrc,                   /* TPMSRC*/
    kSimClockOsc32kSel,                /* OSC32KSEL*/
    kSimClockUsbfSel,                  /* USBF_CLKSEL*/
    kSimClockPllfllSel,                /* PLLFLLSEL*/
    kSimClockNfcSel,                   /* NFC_CLKSEL*/
    kSimClockLcdcSel,                  /* LCDC_CLKSEL*/
    kSimClockTraceSel,                 /* TRACE_CLKSEL*/
    kSimClockClkoutSel,                /* CLKOUTSEL*/
    kSimClockRtcClkoutSel,             /* RTCCLKOUTSEL */
    kSimClockSourceMax
} sim_clock_source_names_t;

/*! @brief Clock Divider names*/
typedef enum _sim_clock_divider_names {
    kSimClockDividerOutdiv1,           /* OUTDIV1*/
    kSimClockDividerOutdiv2,           /* OUTDIV2*/
    kSimClockDividerOutdiv3,           /* OUTDIV3*/
    kSimClockDividerOutdiv4,           /* OUTDIV4*/
    kSimClockDividerUsbFrac,           /* (USBFRAC + 1) / (USBDIV + 1)*/
    kSimClockDividerUsbDiv,
    kSimClockDividerUsbfsFrac,         /* (USBFSFRAC + 1) / (USBFSDIV) + 1)*/
    kSimClockDividerUsbfsDiv,
    kSimClockDividerUsbhsFrac,         /* (USBHSFRAC + 1) / (USBHSDIV + 1)*/
    kSimClockDividerUsbhsDiv,
    kSimClockDividerLcdcFrac,          /* (LCDCFRAC + 1) / (LCDCDIV + 1)*/
    kSimClockDividerLcdcDiv,  
    kSimClockDividerNfcFrac,           /* (NFCFRAC + 1) / (NFCDIV + 1)*/
    kSimClockDividerNfcDiv,    
    kSimClockDividerSpecial1,          /* special divider 1*/
    kSimClockDividerMax
} sim_clock_divider_names_t;

/*! @brief SIM USB voltage regulator in standby mode setting during stop modes */
typedef enum _sim_usbsstby_stop
{
    kSimUsbsstbyNoRegulator,        /* regulator not in standby during Stop modes */
    kSimUsbsstbyWithRegulator       /* regulator in standby during Stop modes */
} sim_usbsstby_stop_t;

/*! @brief SIM USB voltage regulator in standby mode setting during VLPR and VLPW modes */
typedef enum _sim_usbvstby_stop
{
    kSimUsbvstbyNoRegulator,        /* regulator not in standby during VLPR and VLPW modes */
    kSimUsbvstbyWithRegulator       /* regulator in standby during VLPR and VLPW modes */
} sim_usbvstby_stop_t;

/*! @brief SIM CMT/UART pad drive strength */
typedef enum _sim_cmtuartpad_strengh
{
    kSimCmtuartSinglePad,           /* Single-pad drive strength for CMT IRO or UART0_TXD */
    kSimCmtuartDualPad              /* Dual-pad drive strength for CMT IRO or UART0_TXD */
} sim_cmtuartpad_strengh_t;

/*! @brief SIM PTD7 pad drive strength */
typedef enum _sim_ptd7pad_strengh
{
    kSimPtd7padSinglePad,           /* Single-pad drive strength for PTD7 */
    kSimPtd7padDualPad              /* Dual-pad drive strength for PTD7 */
} sim_ptd7pad_strengh_t;

/*! @brief SIM FlexBus security level */
typedef enum _sim_flexbus_security_level
{
    kSimFbslLevel0,                 /* All off-chip accesses (op code and data) via the FlexBus */
                                    /* and DDR controller are disallowed */
    kSimFbslLevel1,                 /* Undefined */
    kSimFbslLevel2,                 /* Off-chip op code accesses are disallowed. Data accesses */
                                    /* are allowed */
    kSimFbslLevel3                  /* Off-chip op code accesses and data accesses are allowed */
} sim_flexbus_security_level_t;

/*! @brief SIM ADCx pre-trigger select */
typedef enum _sim_pretrgsel
{
    kSimAdcPretrgselA,              /* Pre-trigger A selected for ADCx */
    kSimAdcPretrgselB               /* Pre-trigger B selected for ADCx */
} sim_pretrgsel_t;

/*! @brief SIM ADCx trigger select */
typedef enum _sim_trgsel
{
    kSimAdcTrgselExt,               /* External trigger */
    kSimAdcTrgSelHighSpeedComp0,    /* High speed comparator 0 asynchronous interrupt */
    kSimAdcTrgSelHighSpeedComp1,    /* High speed comparator 1 asynchronous interrupt */
    kSimAdcTrgSelHighSpeedComp2,    /* High speed comparator 2 asynchronous interrupt */
    kSimAdcTrgSelPit0,              /* PIT trigger 0 */
    kSimAdcTrgSelPit1,              /* PIT trigger 1 */
    kSimAdcTrgSelPit2,              /* PIT trigger 2 */
    kSimAdcTrgSelPit3,              /* PIT trigger 3 */
    kSimAdcTrgSelFtm0,              /* FTM0 trigger */
    kSimAdcTrgSelFtm1,              /* FTM1 trigger */
    kSimAdcTrgSelFtm2,              /* FTM2 trigger */
    kSimAdcTrgSelFtm3,              /* FTM3 trigger */
    kSimAdcTrgSelRtcAlarm,          /* RTC alarm */
    kSimAdcTrgSelRtcSec,            /* RTC seconds */
    kSimAdcTrgSelLptimer,           /* Low-power timer trigger */
    kSimAdcTrgSelHigSpeedComp3      /* High speed comparator 3 asynchronous interrupt */
} sim_trgsel_t;

/*! @brief SIM receive data source select */
typedef enum _sim_uart_rxsrc
{
    kSimUartRxsrcPin,               /* UARTx_RX Pin */
    kSimUartRxsrcCmp0,              /* CMP0 */
    kSimUartRxsrcCmp1,              /* CMP1 */
    kSimUartRxsrcReserved           /* Reserved */
} sim_uart_rxsrc_t;

/*! @brief SIM transmit data source select */
typedef enum _sim_uart_txsrc
{
    kSimUartTxsrcPin,               /* UARTx_TX Pin */
    kSimUartTxsrcCmp0,              /* UARTx_TX pin modulated with FTM1 channel 0 output */
    kSimUartTxsrcCmp1,              /* UARTx_TX pin modulated with FTM2 channel 0 output */
    kSimUartTxsrcReserved           /* Reserved */
} sim_uart_txsrc_t;

/*! @brief SIM FlexTimer x trigger y select */
typedef enum _sim_ftm_trg_src
{
    kSimFtmTrgSrc0,                 /* FlexTimer x trigger y select 0 */
    kSimFtmTrgSrc1                  /* FlexTimer x trigger y select 1 */
} sim_ftm_trg_src_t;

/*! @brief SIM FlexTimer external clock select */
typedef enum _sim_ftm_clk_sel
{
    kSimFtmClkSel0,                 /* FTM CLKIN0 pin. */
    kSimFtmClkSel1                  /* FTM CLKIN1 pin. */
} sim_ftm_clk_sel_t;

/*! @brief SIM FlexTimer x channel y input capture source select */
typedef enum _sim_ftm_ch_src
{
    kSimFtmChSrc0,                 /* See RM for details of each selection for each channel */
    kSimFtmChSrc1,                 /* See RM for details of each selection for each channel */
    kSimFtmChSrc2,                 /* See RM for details of each selection for each channel */
    kSimFtmChSrc3                  /* See RM for details of each selection for each channel */
} sim_ftm_ch_src_t;

/*! @brief SIM FlexTimer x Fault y select */
typedef enum _sim_ftm_flt_sel
{
    kSimFtmFltSel0,                 /* FlexTimer x fault y select 0 */
    kSimFtmFltSel1                  /* FlexTimer x fault y select 1 */
} sim_ftm_flt_sel_t;

/*! @brief SIM Timer/PWM external clock select */
typedef enum _sim_tpm_clk_sel
{
    kSimTpmClkSel0,                 /* Timer/PWM TPM_CLKIN0 pin. */
    kSimTpmClkSel1                  /* Timer/PWM TPM_CLKIN1 pin. */
} sim_tpm_clk_sel_t;

/*! @brief SIM Timer/PWM x channel y input capture source select */
typedef enum _sim_tpm_ch_src
{
    kSimTpmChSrc0,                 /* TPMx_CH0 signal */
    kSimTpmChSrc1                  /* CMP0 output */
} sim_tpm_ch_src_t;

/*! @brief SIM HAL API return status*/
typedef enum _sim_hal_status {
    kSimHalSuccess,
    kSimHalFail,
    kSimHalNoSuchModule,
    kSimHalNoSuchClockSrc,
    kSimHalNoSuchDivider
} sim_hal_status_t;

/*! @brief Clock gate module configuration table structure*/
typedef struct SimClockGateModuleConfig {
    sim_clock_gate_module_names_t   clockGateModuleName;  /*!< clock module name*/
    uint8_t                         deviceInstance;       /*!< device instance*/
    uint32_t                        scgcRegAddress;       /*!< clock gate control register address*/
    uint32_t                        deviceMask;           /*!< device mask in control register*/
} sim_clock_gate_module_config_t;

/*! @brief clock source value table structure*/
typedef struct SimClockSourceValue {
    union{
    uint8_t                         name;
    sim_clock_names_t               clockName;            /*!< clock name*/
    sim_clock_source_names_t        clockSelName;
    };
    bool                            isSel;                /*!< clock sel flag*/
    bool                            hasDivider;           /*!< has divider*/
    sim_clock_divider_names_t       dividerName;          /*!< divider name*/
} sim_clock_source_value_t;

/*! @brief Clock name configuration table structure*/
typedef struct SimClockNameConfig {
    sim_clock_names_t                   clockName;            /*!< clock name*/
    bool                                useOtherRefClock;     /*!< if it  uses the other ref clock*/
    sim_clock_names_t                   otherRefClockName;    /*!< other ref clock name*/
    sim_clock_divider_names_t           dividerName;          /*!< clock divider name*/
} sim_clock_name_config_t;

/*! @brief clock name configuration table for specified CPU defined in fsl_clock_module_names_Kxxx.h*/
extern const sim_clock_name_config_t kSimClockNameConfigTable[];

/*! @brief SIM configuration table for clock module names*/
extern const sim_clock_gate_module_config_t kSimClockGateModuleConfigTable[];

/*! @brief clock source value table for specified CPU */
extern const sim_clock_source_value_t *kClockSourceValueTable[];

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name clock related feature APIs*/
/*@{*/

/*!
 * @brief Enables or disables the clock for a specified clock module.
 *
 * This function  enables/disables the clock for a specified clock module and instance.
 *
 * @param clockModule Clock module name defined in the sim_clock_gate_module_names_t
 * @param instance    Module instance  
 * @param enable      Enable or disable the clock
 *                    - true:  enable the clock
 *                    - false: disable the clock
 * @return status     If the clock module name doesn't exist, it  returns an error.
 */
sim_hal_status_t clock_hal_set_gate(sim_clock_gate_module_names_t clockModule, uint8_t instance,
                                    bool enable);

/*!
 * @brief Gets the clock enabled or disabled state.
 *
 * This function  gets the current clock gate status of the specified clock
 * module and instance.
 *
 * @param clockModule Clock module name defined in sim_clock_gate_module_names_t
 * @param instance    Module instance
 * @param isEnabled   returned status, clock is enabled or disabled for the module.
 *                    - true: enabled
 *                    - false: disabled
 * @return status     if the clock module name doesn't exist, it  returns an error.
 */
sim_hal_status_t clock_hal_get_gate(sim_clock_gate_module_names_t clockModule, uint8_t instance, 
                                    bool *isEnabled);

/*!
 * @brief Sets the clock source setting.
 *
 * This function  sets the settings for a specified clock source. Each clock 
 * source has its own clock selection settings. See the chip reference manual for 
 * clock source detailed settings and the sim_clock_source_names_t 
 * for clock sources.
 *
 * @param clockSource Clock source name defined in sim_clock_source_names_t
 * @param setting     Setting value
 * @return status     If the clock source doesn't exist, it  returns an error.
 */
sim_hal_status_t clock_hal_set_clock_source(sim_clock_source_names_t clockSource, uint8_t setting);

/*!
 * @brief Gets the clock source setting.
 *
 * This function  gets the settings for a specified clock source. Each clock 
 * source has its own clock selection settings. See the reference manual for 
 * clock source detailed settings and the sim_clock_source_names_t
 * for clock sources.
 *
 * @param clockSource Clock source name
 * @param setting     Current setting for the clock source
 * @return status     If the clock source doesn't exist, it  returns an error.
 */
sim_hal_status_t clock_hal_get_clock_source(sim_clock_source_names_t clockSource, 
                                            uint8_t *setting);

/*!
 * @brief Sets the clock divider setting.
 *
 * This function  sets the setting for a specified clock divider. See the
 * reference manual for a supported clock divider and value range and the 
 * sim_clock_divider_names_t for dividers.
 *
 * @param clockDivider Clock divider name
 * @param divider      Divider setting
 * @return status      If the clock divider doesn't exist, it  returns an error.
 */
sim_hal_status_t clock_hal_set_clock_divider(sim_clock_divider_names_t clockDivider, 
                                             uint32_t setting);

/*!
 * @brief Sets the clock out dividers setting.
 *
 * This function  sets the setting for all clock out dividers at the same time.
 * See the reference manual for a supported clock divider and value range and the 
 * sim_clock_divider_names_t for clock out dividers.
 *
 * @param outdiv1      Outdivider1 setting
 * @param outdiv2      Outdivider2 setting
 * @param outdiv3      Outdivider3 setting
 * @param outdiv4      Outdivider4 setting
 */
void clock_hal_set_clock_out_dividers(uint32_t outdiv1, uint32_t outdiv2,
                                      uint32_t outdiv3, uint32_t outdiv4);

/*!
 * @brief Gets the clock divider setting.
 *
 * This function  gets the setting for a specified clock divider. See the
 * reference manual for a supported clock divider and value range and the 
 * sim_clock_divider_names_t for dividers.
 *
 * @param clockDivider Clock divider name
 * @param divider      Divider value pointer
 * @return status      If the clock divider doesn't exist, it  returns an error.
 */
sim_hal_status_t clock_hal_get_clock_divider(sim_clock_divider_names_t clockDivider,
                                             uint32_t *setting);

/*@}*/

/*! @name individual field access APIs*/
/*@{*/

/*!
 * @brief Sets the USB voltage regulator enabled setting.
 *
 * This function  controls whether the USB voltage regulator is enabled. This bit
 * can only be written when the SOPT1CFG[URWE] bit is set. 
 *
 * @param enable   USB voltage regulator enable setting
 *                  - true: USB voltage regulator is enabled.
 *                  - false: USB voltage regulator is disabled.
 */
static inline void sim_set_usbregen(bool enable)
{
    BW_SIM_SOPT1_USBREGEN(enable ? 1 : 0);
}

/*!
 * @brief Gets the USB voltage regulator enabled setting.
 *
 * This function  gets the USB voltage regulator enabled setting.
 *
 * @return enabled True if the USB voltage regulator is enabled.
 */
static inline bool sim_get_usbregen(void)
{
    return BR_SIM_SOPT1_USBREGEN;
}

/*!
 * @brief Sets the USB voltage regulator in a standby mode setting during Stop, VLPS, LLS, and VLLS.
 *
 * This function  controls whether the USB voltage regulator is placed in a standby
 * mode during Stop, VLPS, LLS, and VLLS modes. This bit can only be written when the
 * SOPT1CFG[USSWE] bit is set. 
 *
 * @param setting   USB voltage regulator in standby mode setting
 *                  - 0: USB voltage regulator not in standby during Stop, VLPS, LLS and
 *                       VLLS modes.
 *                  - 1: USB voltage regulator in standby during Stop, VLPS, LLS and VLLS
 *                       modes.
 */
static inline void sim_set_usbsstby(sim_usbsstby_stop_t setting)
{
    BW_SIM_SOPT1_USBSSTBY(setting);
}

/*!
 * @brief Gets the USB voltage regulator in a standby mode setting.
 *
 * This function  gets the USB voltage regulator in a standby mode setting.
 *
 * @return setting  USB voltage regulator in a standby mode setting
 */
static inline sim_usbsstby_stop_t sim_get_usbsstby(void)
{
    return (sim_usbsstby_stop_t)BR_SIM_SOPT1_USBSSTBY;
}

/*!
 * @brief Sets the USB voltage regulator in a standby mode during the VLPR or the VLPW.
 *
 * This function  controls whether the USB voltage regulator is placed in a standby
 * mode during the VLPR and the VLPW modes. This bit can only be written when the 
 * SOPT1CFG[UVSWE] bit is set. 
 *
 * @param setting   USB voltage regulator in standby mode setting
 *                  - 0: USB voltage regulator not in standby during VLPR and VLPW modes.
 *                  - 1: USB voltage regulator in standby during VLPR and VLPW modes.
 */
static inline void sim_set_usbvstby(sim_usbvstby_stop_t setting)
{
    BW_SIM_SOPT1_USBVSTBY(setting);
}

/*!
 * @brief Gets the USB voltage regulator in a standby mode during the VLPR or the VLPW.
 *
 * This function  gets the USB voltage regulator in a standby mode during the VLPR or the VLPW.
 *
 * @return setting  USB voltage regulator in a standby mode during the VLPR or the VLPW
 */
static inline sim_usbvstby_stop_t sim_get_usbvstby(void)
{
    return (sim_usbvstby_stop_t)BR_SIM_SOPT1_USBVSTBY;
}

#if FSL_FEATURE_SIM_OPT_HAS_RAMSIZE
/*!
 * @brief Gets the RAM size.
 *
 * This function  gets the RAM size. The field specifies the amount of system RAM, which is 
 * available on the device.
 *
 * @return size  RAM size on the device
 */
static inline uint32_t sim_get_ramsize(void)
{
    return BR_SIM_SOPT1_RAMSIZE;
}
#endif /* FSL_FEATURE_SIM_OPT_HAS_RAMSIZE */

/*!
 * @brief Sets the USB voltage regulator stop standby write enable setting.
 *
 * This function  controls whether the USB voltage regulator stop  standby write
 * feature is enabled. Writing one to this bit allows the SOPT1[USBSSTBY] bit to be written. This
 * register bit clears after a write to SOPT1[USBSSTBY]. 
 *
 * @param enable  USB voltage regulator stop standby write enable setting
 *                  - true: SOPT1[USBSSTBY] can be written.
 *                  - false: SOPT1[USBSSTBY] cannot be written.
 */
static inline void sim_set_usswe(bool enable)
{
    BW_SIM_SOPT1CFG_USSWE(enable ? 1 : 0);
}

/*!
 * @brief Gets the USB voltage regulator stop standby write enable setting.
 *
 * This function  gets the USB voltage regulator stop standby write enable setting.
 *
 * @return enabled True if the USB voltage regulator stop standby write is enabled.
 */
static inline bool sim_get_usswe(void)
{
    return BR_SIM_SOPT1CFG_USSWE;
}

/*!
 * @brief Sets the USB voltage regulator VLP standby write enable setting.
 *
 * This function  controls whether USB voltage regulator VLP standby write
 * feature is enabled. Writing one to this bit allows the SOPT1[USBVSTBY] bit to be written. This
 * register bit clears after a write to SOPT1[USBVSTBY]. 
 *
 * @param enable   USB voltage regulator VLP standby write enable setting
 *                  - true: SOPT1[USBSSTBY] can be written.
 *                  - false: SOPT1[USBSSTBY] cannot be written.
 */
static inline void sim_set_uvswe(bool enable)
{
    BW_SIM_SOPT1CFG_UVSWE(enable ? 1 : 0);
}

/*!
 * @brief Gets the USB voltage regulator VLP standby write enable setting.
 *
 * This function  gets the USB voltage regulator VLP standby write enable setting.
 *
 * @return enabled True if the USB voltage regulator VLP standby write is enabled.
 */
static inline bool sim_get_uvswe(void)
{
    return BR_SIM_SOPT1CFG_UVSWE;
}

/*!
 * @brief Sets the USB voltage regulator enable write enable setting.
 *
 * This function  controls whether the USB voltage regulator write enable
 * feature is enabled. Writing one to this bit allows the SOPT1[USBREGEN] bit to be written. 
 * This register bit clears after a write to SOPT1[USBREGEN]. 
 *
 * @param enable   USB voltage regulator enable write enable setting
 *                  - true: SOPT1[USBSSTBY] can be written.
 *                  - false: SOPT1[USBSSTBY] cannot be written.
 */
static inline void sim_set_urwe(bool enable)
{
    BW_SIM_SOPT1CFG_URWE(enable ? 1 : 0);
}

/*!
 * @brief Gets the USB voltage regulator enable write enable setting.
 *
 * This function  gets the USB voltage regulator enable write enable setting.
 *
 * @return enabled True if USB voltage regulator enable write is enabled.
 */
static inline bool sim_get_urwe(void)
{
    return BR_SIM_SOPT1CFG_URWE;
}

#if FSL_FEATURE_SIM_OPT_HAS_CMTUARTPAD
/*!
 * @brief Sets the CMT/UART pad drive strength setting.
 *
 * This function  controls the output drive strength of the CMT IRO signal or 
 * UART0_TXD signal on PTD7 pin by selecting either one or two pads to drive it. 
 *
 * @param setting   CMT/UART pad drive strength setting
 *                  - 0: Single-pad drive strength for CMT IRO or UART0_TXD.
 *                  - 1: Dual-pad drive strength for CMT IRO or UART0_TXD.
 */
static inline void sim_set_cmtuartpad(sim_cmtuartpad_strengh_t setting)
{
    BW_SIM_SOPT2_CMTUARTPAD(setting);
}

/*!
 * @brief Gets the CMT/UART pad drive strength setting.
 *
 * This function  gets the CMT/UART pad drive strength setting.
 *
 * @return setting CMT/UART pad drive strength setting
 */
static inline sim_cmtuartpad_strengh_t sim_get_cmtuartpad(void)
{
    return (sim_cmtuartpad_strengh_t)BR_SIM_SOPT2_CMTUARTPAD;
}
#endif /* FSL_FEATURE_SIM_OPT_HAS_CMTUARTPAD */

#if FSL_FEATURE_SIM_OPT_HAS_PTD7PAD
/*!
 * @brief Sets the PTD7 pad drive strength setting.
 *
 * This function  controls the output drive strength of the PTD7 pin by selecting
 * either one or two pads to drive it. 
 *
 * @param setting   PTD7 pad drive strength setting
 *                  - 0: Single-pad drive strength for PTD7.
 *                  - 1: Double pad drive strength for PTD7.
 */
static inline void sim_set_ptd7pad(sim_ptd7pad_strengh_t setting)
{
    BW_SIM_SOPT2_PTD7PAD(setting);
}

/*!
 * @brief Gets the PTD7 pad drive strength setting.
 *
 * This function  gets the PTD7 pad drive strength setting.
 *
 * @return setting PTD7 pad drive strength setting
 */
static inline sim_ptd7pad_strengh_t sim_get_ptd7pad(void)
{
    return (sim_ptd7pad_strengh_t)BR_SIM_SOPT2_PTD7PAD;
}
#endif /* FSL_FEATURE_SIM_OPT_HAS_PTD7PAD */

#if FSL_FEATURE_SIM_OPT_HAS_FBSL
/*!
 * @brief Sets the FlexBus security level setting.
 *
 * This function  sets the FlexBus security level setting. If the security is enabled,
 * this field affects which CPU operations can access the off-chip via the FlexBus 
 * and DDR controller interfaces. This field has no effect if the security is not enabled. 
 *
 * @param setting   FlexBus security level setting
 *                  - 00: All off-chip accesses (op code and data) via the FlexBus and 
 *                        DDR controller are disallowed.
 *                  - 10: Off-chip op code accesses are disallowed. Data accesses are
 *                        allowed.
 *                  - 11: Off-chip op code accesses and data accesses are allowed.
 */
static inline void sim_set_fbsl(sim_flexbus_security_level_t setting)
{
    BW_SIM_SOPT2_FBSL(setting);
}

/*!
 * @brief Gets the FlexBus security level setting.
 *
 * This function  gets the FlexBus security level setting.
 *
 * @return setting FlexBus security level setting
 */
static inline sim_flexbus_security_level_t sim_get_fbsl(void)
{
    return (sim_flexbus_security_level_t)BR_SIM_SOPT2_FBSL;
}
#endif /* FSL_FEATURE_SIM_OPT_HAS_FBSL */

#if FSL_FEATURE_SIM_OPT_HAS_PCR
/*!
 * @brief Sets the PCR setting.
 *
 * This function  sets the PCR setting. This is the FlexBus hold cycles before 
 * FlexBus can release bus to NFC or to IDLE.
 *
 * @param setting   PCR setting
 */
static inline void sim_set_pcr(uint32_t setting)
{
    BW_SIM_SOPT6_PCR(setting);
}

/*!
 * @brief Gets the PCR setting.
 *
 * This function  gets the PCR setting.
 *
 * @return setting PCR setting
 */
static inline uint32_t sim_get_pcr(void)
{
    return BR_SIM_SOPT6_PCR;
}
#endif /* FSL_FEATURE_SIM_OPT_HAS_PCR */

#if FSL_FEATURE_SIM_OPT_HAS_MCC
/*!
 * @brief Sets the MCC setting. 
 *
 * This function  sets  the MCC setting. This is the NFC hold cycle in case the
 * FlexBus request during NFC is granted.
 *
 * @param setting   MCC setting
 */
static inline void sim_set_mcc(uint32_t setting)
{
    BW_SIM_SOPT6_MCC(setting);
}

/*!
 * @brief Gets the MCC setting.
 *
 * This function  gets the MCC setting.
 *
 * @return setting MCC setting
 */
static inline uint32_t sim_get_mcc(void)
{
    return BR_SIM_SOPT6_MCC;
}
#endif /* FSL_FEATURE_SIM_OPT_HAS_MCC */

/*!
 * @brief Sets the ADCx alternate trigger enable setting.
 *
 * This function  enables/disables the alternative conversion triggers for ADCx.
 *
 * @param enable Enable alternative conversion triggers for ADCx
 *               - true: Select alternative conversion trigger.
 *               - false: Select PDB trigger.
 */
void sim_set_alttrgen(uint8_t instance, bool enable);

/*!
 * @brief Gets the  ADCx alternate trigger enable setting.
 *
 * This function  gets the  ADCx alternate trigger enable setting.
 *
 * @return enabled True if  ADCx alternate trigger is enabled
 */
bool sim_get_alttrgen(uint8_t instance);

/*!
 * @brief Sets the ADCx pre-trigger select setting.
 *
 * This function  selects the ADCx pre-trigger source when the alternative triggers
 * are enabled through ADCxALTTRGEN.
 *
 * @param select pre-trigger select setting for ADCx
 *               - 0: Pre-trigger A selected for ADCx.
 *               - 1: Pre-trigger B selected for ADCx.
 */
void sim_set_pretrgsel(uint8_t instance, sim_pretrgsel_t select);

/*!
 * @brief Gets the ADCx pre-trigger select setting.
 *
 * This function  gets the ADCx pre-trigger select setting.
 *
 * @return select ADCx pre-trigger select setting
 */
sim_pretrgsel_t sim_get_pretrgsel(uint8_t instance);

/*!
 * @brief Sets the ADCx trigger select setting.
 *
 * This function  selects the ADCx trigger source when alternative triggers
 * are enabled through ADCxALTTRGEN.
 *
 * @param select trigger select setting for ADCx
 *               - 0000: External trigger
 *               - 0001: High speed comparator 0 asynchronous interrupt
 *               - 0010: High speed comparator 1 asynchronous interrupt
 *               - 0011: High speed comparator 2 asynchronous interrupt 
 *               - 0100: PIT trigger 0
 *               - 0101: PIT trigger 1
 *               - 0110: PIT trigger 2
 *               - 0111: PIT trigger 3
 *               - 1000: FTM0 trigger
 *               - 1001: FTM1 trigger
 *               - 1010: FTM2 trigger
 *               - 1011: FTM3 trigger
 *               - 1100: RTC alarm
 *               - 1101: RTC seconds
 *               - 1110: Low-power timer trigger
 *               - 1111: High speed comparator 3 asynchronous interrupt
*/
void sim_set_trgsel(uint8_t instance, sim_trgsel_t select);

/*!
 * @brief Gets the ADCx trigger select setting.
 *
 * This function  gets the ADCx trigger select setting.
 *
 * @return select ADCx trigger select setting
 */
sim_pretrgsel_t sims_get_trgsel(uint8_t instance);

/*!
 * @brief Sets the UARTx receive data source select setting.
 *
 * This function  selects the source for the UARTx receive data.
 *
 * @param select the source for the UARTx receive data
 *               - 00: UARTx_RX pin.
 *               - 01: CMP0.
 *               - 10: CMP1.
 *               - 11: Reserved.
 */
void sim_set_uart_rxsrc(uint8_t instance, sim_uart_rxsrc_t select);

/*!
 * @brief Gets the UARTx receive data source select setting. 
 *
 * This function  gets the UARTx receive data source select setting.
 *
 * @return select UARTx receive data source select setting
 */
sim_uart_rxsrc_t sim_get_uart_rxsrc(uint8_t instance);

/*!
 * @brief Sets the UARTx transmit data source select setting.
 *
 * This function  selects the source for the UARTx transmit data.
 *
 * @param select the source for the UARTx transmit data
 *               - 00: UARTx_TX pin.
 *               - 01: UARTx_TX pin modulated with FTM1 channel 0 output.
 *               - 10: UARTx_TX pin modulated with FTM2 channel 0 output.
 *               - 11: Reserved.
 */
void sim_set_uart_txsrc(uint8_t instance, sim_uart_txsrc_t select);

/*!
 * @brief Gets the UARTx transmit data source select setting.
 *
 * This function  gets the UARTx transmit data source select setting.
 *
 * @return select UARTx transmit data source select setting
 */
sim_uart_txsrc_t sim_get_uart_txsrc(uint8_t instance);

#if FSL_FEATURE_SIM_OPT_HAS_ODE
/*!
 * @brief Sets the UARTx Open Drain Enable setting.
 *
 * This function  enables/disables the UARTx Open Drain.
 *
 * @param enable Enable/disable UARTx Open Drain
 *               - True: Enable UARTx Open Drain
 *               - False: Disable UARTx Open Drain
 */
void sim_set_uart_ode(uint8_t instance, bool enable);

/*!
 * @brief Gets the UARTx Open Drain Enable setting.
 *
 * This function  gets the UARTx Open Drain Enable setting. 
 *
 * @return enabled True if UARTx Open Drain is enabled.
 */
bool sim_get_uart_ode(uint8_t instance);
#endif

#if FSL_FEATURE_SIM_OPT_HAS_FTM
/*!
 * @brief Sets the FlexTimer x hardware trigger y source select setting.
 *
 * This function  selects  the source of FTMx hardware trigger y.
 *
 * @param select FlexTimer x hardware trigger y
 *               - 0: Pre-trigger A selected for ADCx.
 *               - 1: Pre-trigger B selected for ADCx.
 */
void sim_set_ftm_trg_src(uint8_t instance, uint8_t trigger, sim_ftm_trg_src_t select);

/*!
 * @brief Gets the FlexTimer x hardware trigger y source select setting.
 *
 * This function  gets the FlexTimer x hardware trigger y source select setting. 
 *
 * @return select FlexTimer x hardware trigger y source select setting
 */
sim_ftm_trg_src_t sim_get_ftm_trg_src(uint8_t instance, uint8_t trigger);

/*!
 * @brief Sets the FlexTimer x external clock pin select setting.
 *
 * This function  selects the source of FTMx external clock pin select.
 *
 * @param select FTMx external clock pin select
 *               - 0: FTMx external clock driven by FTM CLKIN0 pin.
 *               - 1: FTMx external clock driven by FTM CLKIN1 pin.
 */
void sim_set_ftm_clk_sel(uint8_t instance, sim_ftm_clk_sel_t select);

/*!
 * @brief Gets the FlexTimer x external clock pin select setting.
 *
 * This function  gets the FlexTimer x external clock pin select setting. 
 *
 * @return select FlexTimer x external clock pin select setting
 */
sim_ftm_clk_sel_t sim_get_ftm_clk_sel(uint8_t instance);

/*!
 * @brief Sets the FlexTimer x channel y input capture source select setting.
 *
 * This function  selects the FlexTimer x channel y input capture source.
 *
 * @param select FlexTimer x channel y input capture source 
 *               See the reference manual for detailed definition for each channel and selection.
 */
void sim_set_ftm_ch_src(uint8_t instance, uint8_t channel, sim_ftm_ch_src_t select);

/*!
 * @brief Gets the FlexTimer x channel y input capture source select setting.
 *
 * This function  gets the FlexTimer x channel y input capture source select setting. 
 *
 * @return select FlexTimer x channel y input capture source select setting
 */
sim_ftm_ch_src_t sim_get_ftm_ch_src(uint8_t instance, uint8_t channel);

/*!
 * @brief Sets the FlexTimer x fault y select setting.
 *
 * This function  sets the FlexTimer x fault y select setting.
 *
 * @param select FlexTimer x fault y select setting
 *               - 0: FlexTimer x fault y select 0.
 *               - 1: FlexTimer x fault y select 1.
 */
void sim_set_ftm_flt_sel(uint8_t instance, uint8_t fault, sim_ftm_flt_sel_t select);

/*!
 * @brief Gets the FlexTimer x fault y select setting.
 *
 * This function  gets the FlexTimer x fault y select setting. 
 *
 * @return select FlexTimer x fault y select setting
 */
sim_ftm_flt_sel_t sim_get_ftm_flt_sel(uint8_t instance, uint8_t fault);
#endif

#if FSL_FEATURE_SIM_OPT_HAS_TPM
/*!
 * @brief Sets the Timer/PWM x external clock pin select setting.
 *
 * This function  selects the source of the Timer/PWM x external clock pin select.
 *
 * @param select Timer/PWM x external clock pin select
 *               - 0: Timer/PWM x external clock driven by the TPM_CLKIN0 pin.
 *               - 1: Timer/PWM x external clock driven by the TPM_CLKIN1 pin.
 */
void sim_set_tpm_clk_sel(uint8_t instance, sim_tpm_clk_sel_t select);

/*!
 * @brief Gets the Timer/PWM x external clock pin select setting.
 *
 * This function  gets the Timer/PWM x external clock pin select setting. 
 *
 * @return select Timer/PWM x external clock pin select setting
 */
sim_tpm_clk_sel_t sim_get_tpm_clk_sel(uint8_t instance);

/*!
 * @brief Sets the Timer/PWM x channel y input capture source select setting.
 *
 * This function  selects the Timer/PWM x channel y input capture source.
 *
 * @param select Timer/PWM x channel y input capture source 
 *               - 0: TPMx_CH0 signal
 *               - 1: CMP0 output
 */
void sim_set_tpm_ch_src(uint8_t instance, uint8_t channel, sim_tpm_ch_src_t select);

/*!
 * @brief Gets the Timer/PWM x channel y input capture source select setting.
 *
 * This function  gets the Timer/PWM x channel y input capture source select setting. 
 *
 * @return select Timer/PWM x channel y input capture source select setting
 */
sim_tpm_ch_src_t sim_get_tpm_ch_src(uint8_t instance, uint8_t channel);
#endif

#if FSL_FEATURE_SIM_SDID_HAS_FAMILYID
/*!
 * @brief Gets the Kinetis Family ID in the System Device ID register (SIM_SDID).
 *
 * This function  gets the Kinetis Family ID in the System Device ID register. 
 *
 * @return id Kinetis Family ID
 */
static inline uint32_t sim_get_family_id(void)
{
    return BR_SIM_SDID_FAMILYID;
}
#endif

#if FSL_FEATURE_SIM_SDID_HAS_SUBFAMID
/*!
 * @brief Gets the Kinetis Sub-Family ID in the System Device ID register (SIM_SDID). 
 *
 * This function  gets the Kinetis Sub-Family ID in System Device ID register. 
 *
 * @return id Kinetis Sub-Family ID
 */
static inline uint32_t sim_get_subfam_id(void)
{
    return BR_SIM_SDID_SUBFAMID;
}
#endif

#if FSL_FEATURE_SIM_SDID_HAS_SERIESID
/*!
 * @brief Gets the Kinetis SeriesID in the System Device ID register (SIM_SDID).
 *
 * This function  gets the Kinetis Series ID in System Device ID register. 
 *
 * @return id Kinetis Series ID
 */
static inline uint32_t sim_get_series_id(void)
{
    return BR_SIM_SDID_SERIESID;
}
#endif

/*!
 * @brief Gets the Kinetis Fam ID in System Device ID register (SIM_SDID).
 *
 * This function  gets the Kinetis Fam ID in System Device ID register. 
 *
 * @return id Kinetis Fam ID
 */
static inline uint32_t sim_get_fam_id(void)
{
    return BR_SIM_SDID_FAMID;
}

/*!
 * @brief Gets the Kinetis Pincount ID in System Device ID register (SIM_SDID).
 *
 * This function  gets the Kinetis Pincount ID in System Device ID register. 
 *
 * @return id Kinetis Pincount ID
 */
static inline uint32_t sim_get_pin_id(void)
{
    return BR_SIM_SDID_PINID;
}

/*!
 * @brief Gets the Kinetis Revision ID in the System Device ID register (SIM_SDID). 
 *
 * This function  gets the Kinetis Revision ID in System Device ID register. 
 *
 * @return id Kinetis Revision ID
 */
static inline uint32_t sim_get_rev_id(void)
{
    return BR_SIM_SDID_REVID;
}

#if FSL_FEATURE_SIM_SDID_HAS_DIEID
/*!
 * @brief Gets the Kinetis Die ID in the System Device ID register (SIM_SDID).
 *
 * This function  gets the Kinetis Die ID in System Device ID register. 
 *
 * @return id Kinetis Die ID
 */
static inline uint32_t sim_get_die_id(void)
{
    return BR_SIM_SDID_DIEID;
}
#endif

#if FSL_FEATURE_SIM_SDID_HAS_SRAMSIZE
/*!
 * @brief Gets the Kinetis SRAM size in the System Device ID register (SIM_SDID).
 *
 * This function  gets the Kinetis SRAM Size in System Device ID register. 
 *
 * @return id Kinetis SRAM Size
 */
static inline uint32_t sim_get_sram_size(void)
{
    return BR_SIM_SDID_SRAMSIZE;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_NVMSIZE
/*!
 * @brief Gets the FlexNVM size in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the FlexNVM size in the Flash Configuration Register 1.
 *
 * @return size FlexNVM Size
 */
static inline uint32_t sim_get_nvm_size(void)
{
    return BR_SIM_FCFG1_NVMSIZE;
}
#endif

/*!
 * @brief Gets the program flash size in  the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the program flash size in the Flash Configuration Register 1.
 *
 * @return size Program flash Size
 */
static inline uint32_t sim_get_pf_size(void)
{
    return BR_SIM_FCFG1_PFSIZE;
}

#if FSL_FEATURE_SIM_FCFG_HAS_EESIZE
/*!
 * @brief Gets the EEProm size in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the EEProm size in the Flash Configuration Register 1.
 *
 * @return size EEProm Size
 */
static inline uint32_t sim_get_eeprom_size(void)
{
    return BR_SIM_FCFG1_EESIZE;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_DEPART
/*!
 * @brief Gets the FlexNVM partition in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the FlexNVM partition in the Flash Configuration Register 1  
 *
 * @return setting FlexNVM partition setting
 */
static inline uint32_t sim_get_depart(void)
{
    return BR_SIM_FCFG1_DEPART;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_FLASHDOZE
/*!
 * @brief Sets the Flash Doze in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  sets the Flash Doze in the Flash Configuration Register 1.
 * 
 * @param setting Flash Doze setting
 */
static inline void sim_set_flashdoze(uint32_t setting)
{
    BW_SIM_FCFG1_FLASHDOZE(setting);
}

/*!
 * @brief Gets the Flash Doze in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the Flash Doze in the Flash Configuration Register 1.
 *
 * @return setting Flash Doze setting
 */
static inline uint32_t sim_get_flashdoze(void)
{
    return BR_SIM_FCFG1_FLASHDOZE;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_FLASHDIS
/*!
 * @brief Sets the Flash disable setting in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  sets the Flash disable setting in the Flash Configuration Register 1. 
 *
 * @param setting Flash disable setting
 */
static inline void sim_set_flashdis(uint32_t setting)
{
    BW_SIM_FCFG1_FLASHDIS(setting);
}

/*!
 * @brief Gets the Flash disable setting in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the Flash disable setting in the Flash Configuration Register 1.
 *
 * @return setting Flash disable setting
 */
static inline uint32_t sim_get_flashdis(void)
{
    return BR_SIM_FCFG1_FLASHDIS;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_MAXADDR0
/*!
 * @brief Gets the Flash maximum address block 0 in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the Flash maximum block 0 in Flash Configuration Register 2.
 *
 * @return address Flash maximum block 0 address
 */
static inline uint32_t sim_get_maxaddr0(void)
{
    return BR_SIM_FCFG2_MAXADDR0;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_MAXADDR1
/*!
 * @brief Gets the Flash maximum address block 1 in Flash Configuration Register 2.
 *
 * This function  gets the Flash maximum block 1 in Flash Configuration Register 1.
 *
 * @return address Flash maximum block 0 address
 */
static inline uint32_t sim_get_maxaddr1(void)
{
    return BR_SIM_FCFG2_MAXADDR1;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_MAXADDR01
/*!
 * @brief Gets the Flash maximum address block 0 in the Flash Configuration Register 1  (SIM_FCFG).
 *
 * This function  gets the Flash maximum block 0 in Flash Configuration Register 2. 
 *
 * @return address Flash maximum block 0 address
 */
static inline uint32_t sim_get_maxaddr01(void)
{
    return BR_SIM_FCFG2_MAXADDR01;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_MAXADDR23
/*!
 * @brief Gets the Flash maximum address block 1 in the Flash Configuration Register 2.
 *
 * This function  gets the Flash maximum block 1 in Flash Configuration Register 1.
 *
 * @return address Flash maximum block 0 address
 */
static inline uint32_t sim_get_maxaddr23(void)
{
    return BR_SIM_FCFG2_MAXADDR23;
}
#endif

#if FSL_FEATURE_SIM_FCFG_HAS_PFLSH
/*!
 * @brief Gets the program flash in the Flash Configuration Register 2.
 *
 * This function  gets the program flash maximum block 0 in Flash Configuration Register 1.
 *
 * @return status program flash status
 */
static inline uint32_t sim_get_pflsh(void)
{
    return BR_SIM_FCFG2_PFLSH;
}
#endif

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_SIM_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

