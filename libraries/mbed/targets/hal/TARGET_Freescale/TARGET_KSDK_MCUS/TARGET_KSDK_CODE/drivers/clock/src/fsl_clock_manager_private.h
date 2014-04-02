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
#if !defined(__FSL_CLOCK_MANAGER_PRIVATE_H__)
#define __FSL_CLOCK_MANAGER_PRIVATE_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*! @addtogroup clock_manager*/
/*! @{*/

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock manager clock names mapping into the SIM clock name*/
sim_clock_names_t kClockNameSimMap[kClockNameCount] = {
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
   kSimReserved
};

/*! @brief Clock manager clock source names mapping into the SIM clock source name*/
sim_clock_source_names_t kClockSourceNameSimMap[kClockSourceMax] = {
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
    kSimClockRtcClkoutSel              /* RTCCLKOUTSEL */
};

/*! @brief Clock manager clock module names mapping into the SIM clock module name*/
sim_clock_gate_module_names_t kClockModuleNameSimMap[kClockModuleMax] = {
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
    kSimClockModuleLCDC                    /**/
};

extern const sim_clock_source_value_t *kSimClockSourceValueTable[];

/*******************************************************************************
 * API
 ******************************************************************************/

/*! @name System out clock access API*/
/*@{*/

/*!
 * @brief Gets the current out clock.
 *
 * @param none
 *
 * @return frequency    Out clock frequency for the clock system
 */
extern uint32_t clock_hal_get_outclk(void);

/*!
 * @brief Gets the current FLL clock.
 *
 * @param none
 *
 * @return frequency    FLL clock frequency for the clock system
 */
extern uint32_t clock_hal_get_fllclk(void);

/*!
 * @brief Gets the current PLL0 clock.
 *
 * @param none
 *
 * @return frequency    PLL0 clock frequency for the clock system
 */
extern uint32_t clock_hal_get_pll0clk(void);

/*!
 * @brief Gets the current PLL1 clock.
 *
 * @param none
 *
 * @return frequency    PLL1 clock frequency for the clock system
 */
extern uint32_t clock_hal_get_pll1clk(void);

/*!
 * @brief Get the current IR (internal reference) clock.
 *
 * @param none
 *
 * @return frequency    IR clock frequency for the clock system
 */
extern uint32_t clock_hal_get_irclk(void);

/*@}*/

/*! @}*/

#endif /* __FSL_CLOCK_MANAGER_PRIVATE_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

