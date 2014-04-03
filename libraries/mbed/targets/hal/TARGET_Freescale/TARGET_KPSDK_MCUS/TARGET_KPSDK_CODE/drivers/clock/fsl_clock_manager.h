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

#if !defined(__FSL_CLOCK_MANAGER_H__)
#define __FSL_CLOCK_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*! @addtogroup clock_manager*/
/*! @{*/

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock names */
typedef enum _clock_names {
 
   /* default system clocks*/
   kCoreClock,                         /**/
   kSystemClock,                       /**/
   kPlatformClock,                     /**/
   kBusClock,                          /**/
   kFlexBusClock,                      /**/
   kFlashClock,                        /**/
 
   /* other internal clocks used by peripherals*/

   /* osc clock*/
   kOsc32kClock,
   kOsc0ErClock,
   kOsc1ErClock,
 
   /* irc 48Mhz clock */
   kIrc48mClock,

   /* rtc clock*/
   kRtc32kClock,
   kRtc1hzClock,

   /* lpo clcok*/
   kLpoClock,
 
   /* mcg clocks*/
   kMcgFllClock,
   kMcgPll0Clock,
   kMcgPll1Clock,
   kMcgOutClock,
   kMcgIrClock,
   
   /* constant clocks (provided in other header files?)*/
   kSDHC0_CLKIN,
   kENET_1588_CLKIN,
   kEXTAL_Clock,
   kEXTAL1_Clock,
   kUSB_CLKIN,
   
   /* reserved value*/
   kReserved,
 
   /* clock name max */
   kClockNameCount

} clock_names_t;

/*! @brief Clock gate module names */
typedef enum _clock_gate_module_names {

    /* System modules*/
    kClockModuleDMA,                    /**/
    kClockModuleDMAMUX,                 /*   instance 0, 1*/
    kClockModulePORT,                   /*   instance 0 - 5 (A - F)*/
    kClockModuleMPU,                    /**/
    kClockModuleLLWU,                   /**/
    kClockModuleEWM,                    /**/

    /* Clocks*/
    kClockModuleOSC1,                   /**/

    /* Memory and memory interfaces*/
    kClockModuleFTF,                    /*  Flash memory control clock*/
    kClockModuleNFC,                    /*  NAND flash control clock*/
    kClockModuleFLEXBUS,                /**/
    kClockModuleDDR,                    /**/

    /* Security*/
    kClockModuleCRC,                    /**/
    kClockModuleRNGA,                   /**/
    kClockModuleREGFILE,                /**/
    kClockModuleDRYICESECREG,           /**/
    kClockModuleDRYICE,                 /**/

    /* Analog*/
    kClockModuleADC,                    /*   instance 0 - 3*/
    kClockModuleCMP,                    /* */
    kClockModuleDAC,                    /*   instance 0, 1*/
    kClockModuleVREF,                   /* */
    kClockModuleSAI,                    /*   instance 0, 1*/

    /* Timers*/
    kClockModuleTPM,                    /*   TPM timers 0 - 2*/
    kClockModulePDB,                    /* */
    kClockModuleFTM,                    /*   instance 0 - 3*/
    kClockModulePIT,                    /**/
    kClockModuleLPTIMER,                /**/
    kClockModuleCMT,                    /**/
    kClockModuleRTC,                    /**/

    /* Communication Interfaces*/
    kClockModuleENET,                   /**/
    kClockModuleUSBHS,                  /**/
    kClockModuleUSBFS,                  /**/
    kClockModuleUSBDCD,                 /**/
    kClockModuleFLEXCAN,                /*   instance 0, 1*/
    kClockModuleSPI,                    /*   instance 0 - 2*/
    kClockModuleI2C,                    /*   instance 0, 1*/
    kClockModuleUART,                   /*   instance 0 - 5*/
    kClockModuleESDHC,                  /**/
    kClockModuleLPUART,                 /**/

    /* Human-machine Interfaces*/
    kClockModuleTSI,                    /**/
    kClockModuleLCDC,                   /**/
    kClockModuleMax
} clock_gate_module_names_t;

/*! @brief Clock source and SEL names */
typedef enum _clock_source_names {
    kClockNfcSrc,                   /* NFCSRC*/
    kClockEsdhcSrc,                 /* ESDHCSRC K70*/
    kClockSdhcSrc,                  /* SDHCSRC  K64*/
    kClockLcdcSrc,                  /* LCDCSRC*/
    kClockTimeSrc,                  /* TIMESRC*/
    kClockRmiiSrc,                  /* RMIISRC*/
    kClockUsbfSrc,                  /* USBFSRC  K70*/
    kClockUsbSrc,                   /* USBSRC   K64, KL25, KV31, and K22*/
    kClockUsbhSrc,                  /* USBHSRC*/
    kClockUart0Src,                 /* UART0SRC*/
    kClockTpmSrc,                   /* TPMSRC*/
    kClockOsc32kSel,                /* OSC32KSEL*/
    kClockUsbfSel,                  /* USBF_CLKSEL*/
    kClockPllfllSel,                /* PLLFLLSEL*/
    kClockNfcSel,                   /* NFC_CLKSEL*/
    kClockLcdcSel,                  /* LCDC_CLKSEL*/
    kClockTraceSel,                 /* TRACE_CLKSEL*/
    kClockClkoutSel,                /* CLKOUTSEL*/
    kClockRtcClkoutSel,             /* RTCCLKOUTSEL */
    kClockSourceMax
} clock_source_names_t;

/*!
 * @brief Error code definition for the clock manager APIs
 */
typedef enum _clock_manager_error_code {
    kClockManagerSuccess,                           /*!< success */
    kClockManagerNoSuchClockName,                   /*!< cannot find the clock name */
    kClockManagerNoSuchClockModule,                 /*!< cannot find the clock module name */
    kClockManagerNoSuchClockSource,                 /*!< cannot find the clock source name */
    kClockManagerNoSuchDivider,                     /*!< cannot find the divider name */
    kClockManagerUnknown                            /*!< unknown error*/
} clock_manager_error_code_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name Clock Gating*/
/*@{*/

/*!
 * @brief Enables or disables the clock for a specific clock module.
 *
 * This function enables/disables the clock for a specified clock module and
 * instance. See the clock_gate_module_names_t for supported clock module names
 * for a specific function and see the Reference Manual for supported clock module
 * name for a specific chip family. Most module drivers call this function
 * to gate(disable)/ungate(enable) the clock for a module. However, the application 
 * can also call this function as needed. Disabling the clock causes the module
 * to stop working. See the Reference Manual to properly enable
 * and disable the clock for a device module.
 * 
 * @param moduleName Gate control module name defined in clock_gate_module_names_t
 * @param instance   Instance of the module
 * @param enable     Enable or disable the clock for the module
 *                   - true: Enable
 *                   - false: Disable
 * @return status    Error code defined in clock_manager_error_code_t
 */
clock_manager_error_code_t clock_manager_set_gate(clock_gate_module_names_t moduleName, 
                                                  uint8_t instance, bool enable);

/*!
 * @brief Gets the current clock gate status for a specific clock module.
 *
 * This function returns the current clock gate status for a specific clock
 * module. See clock_gate_module_names_t for supported clock module name.
 *
 * @param moduleName Gate control module name defined in clock_gate_module_names_t
 * @param instance   Instance of the module
 * @param isEnabled  Status of the module clock
 *                   - true: Enabled
 *                   - false: Disabled
 * @return status    Error code defined in clock_manager_error_code_t
 */
clock_manager_error_code_t clock_manager_get_gate(clock_gate_module_names_t moduleName, 
                                                  uint8_t instance, bool *isEnabled);

/*@}*/

/*! @name Clock Frequencies*/
/*@{*/

/*!
 * @brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_names_t.
 * The MCG must be properly configured before using this function. See
 * the Reference Manual for supported clock names for different chip families.
 * The returned value is in Hertz. If it cannot find the clock name
 * or the name is not supported for a specific chip family, it returns an
 * error.
 *
 * @param clockName Clock names defined in clock_names_t
 * @param frequency Returned clock frequency value in Hertz
 * @return status   Error code defined in clock_manager_error_code_t
 */
clock_manager_error_code_t clock_manager_get_frequency(clock_names_t clockName, 
                                                        uint32_t *frequency);

/*!
 * @brief Gets the clock frequency for a specified clock source.
 *
 * This function gets the specified clock source setting and converts it 
 * into a clock name. It calls the internal function to get the value
 * for that clock name. The returned value is in Hertz.
 * If it cannot find the clock source or the source is not supported for the
 * specific chip family, it returns an error.
 *
 * @param clockSource Clock source names defined in clock_source_names_t
 * @param frequency   Returned clock frequency value in Hertz
 * @return status     Error code defined in clock_manager_error_code_t
 */
clock_manager_error_code_t clock_manager_get_frequency_by_source(
                        clock_source_names_t clockSource, uint32_t *frequency);

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_CLOCK_MANAGER_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

