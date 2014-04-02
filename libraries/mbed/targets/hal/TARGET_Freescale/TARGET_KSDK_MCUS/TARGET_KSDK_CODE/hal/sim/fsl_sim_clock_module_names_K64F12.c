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

#include <stdint.h>
#include <stdlib.h>
#include "fsl_device_registers.h"
#include "fsl_sim_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief CLOCK name config table for K64*/
const sim_clock_name_config_t kSimClockNameConfigTable [] =  {
    {kSimCoreClock,       false, kSimSystemClock, kSimClockDividerOutdiv1},
    {kSimSystemClock,     false, kSimSystemClock, kSimClockDividerOutdiv1},
    {kSimBusClock,        false, kSimSystemClock, kSimClockDividerOutdiv2},
    {kSimFlexBusClock,    false, kSimSystemClock, kSimClockDividerOutdiv3},
    {kSimFlashClock,      false, kSimSystemClock, kSimClockDividerOutdiv4},
    {kSimClockNameCount,  false, kSimSystemClock, kSimClockDividerMax}
};

/*! @brief Clock gate module config table for K64.*/
const sim_clock_gate_module_config_t kSimClockGateModuleConfigTable [] =  {

    /* System modules*/
    {kSimClockModuleDMA,           0,  HW_SIM_SCGC7_ADDR, SIM_SCGC7_DMA_MASK},
    {kSimClockModuleDMAMUX,        0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_DMAMUX_MASK},
    {kSimClockModulePORT,          0,  HW_SIM_SCGC5_ADDR, SIM_SCGC5_PORTA_MASK},
    {kSimClockModulePORT,          1,  HW_SIM_SCGC5_ADDR, SIM_SCGC5_PORTB_MASK},
    {kSimClockModulePORT,          2,  HW_SIM_SCGC5_ADDR, SIM_SCGC5_PORTC_MASK},
    {kSimClockModulePORT,          3,  HW_SIM_SCGC5_ADDR, SIM_SCGC5_PORTD_MASK},
    {kSimClockModulePORT,          4,  HW_SIM_SCGC5_ADDR, SIM_SCGC5_PORTE_MASK},
    {kSimClockModuleMPU,           0,  HW_SIM_SCGC7_ADDR, SIM_SCGC7_MPU_MASK},
    {kSimClockModuleEWM,           0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_EWM_MASK},

    /* Memory and memory interfaces*/
    {kSimClockModuleFLEXBUS,       0,  HW_SIM_SCGC7_ADDR, SIM_SCGC7_FLEXBUS_MASK},
    {kSimClockModuleFTF,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_FTF_MASK},

    /* Security*/
    {kSimClockModuleCRC,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_CRC_MASK},
    {kSimClockModuleRNGA,          0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_RNGA_MASK},

    /* Analog*/
    {kSimClockModuleADC,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_ADC0_MASK},
    {kSimClockModuleADC,           1,  HW_SIM_SCGC3_ADDR, SIM_SCGC3_ADC1_MASK},
    {kSimClockModuleCMP,           0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_CMP_MASK},
    {kSimClockModuleDAC,           0,  HW_SIM_SCGC2_ADDR, SIM_SCGC2_DAC0_MASK},
    {kSimClockModuleDAC,           1,  HW_SIM_SCGC2_ADDR, SIM_SCGC2_DAC1_MASK},
    {kSimClockModuleVREF,          0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_VREF_MASK},
    {kSimClockModuleSAI,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_I2S_MASK},

    /* Timers*/
    {kSimClockModulePDB,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_PDB_MASK},
    {kSimClockModuleFTM,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_FTM0_MASK},
    {kSimClockModuleFTM,           1,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_FTM1_MASK},
    {kSimClockModuleFTM,           2,  HW_SIM_SCGC3_ADDR, SIM_SCGC3_FTM2_MASK},
    {kSimClockModuleFTM,           3,  HW_SIM_SCGC3_ADDR, SIM_SCGC3_FTM3_MASK},
    {kSimClockModulePIT,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_PIT_MASK},
    {kSimClockModuleLPTIMER,       0,  HW_SIM_SCGC5_ADDR, SIM_SCGC5_LPTMR_MASK},
    {kSimClockModuleCMT,           0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_CMT_MASK},
    {kSimClockModuleRTC,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_RTC_MASK},

    /* Communication Interfaces*/
    {kSimClockModuleENET,          0,  HW_SIM_SCGC2_ADDR, SIM_SCGC2_ENET_MASK},
    {kSimClockModuleUSBFS,         0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_USBOTG_MASK},
    {kSimClockModuleUSBDCD,        0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_USBDCD_MASK},
    {kSimClockModuleFLEXCAN,       0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_FLEXCAN0_MASK},
    {kSimClockModuleSPI,           0,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_SPI0_MASK},
    {kSimClockModuleSPI,           1,  HW_SIM_SCGC6_ADDR, SIM_SCGC6_SPI1_MASK},
    {kSimClockModuleSPI,           2,  HW_SIM_SCGC3_ADDR, SIM_SCGC3_SPI2_MASK},
    {kSimClockModuleI2C,           0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_I2C0_MASK},
    {kSimClockModuleI2C,           1,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_I2C1_MASK},
    {kSimClockModuleI2C,           2,  HW_SIM_SCGC1_ADDR, SIM_SCGC1_I2C2_MASK},
    {kSimClockModuleUART,          0,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_UART0_MASK},
    {kSimClockModuleUART,          1,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_UART1_MASK},
    {kSimClockModuleUART,          2,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_UART2_MASK},
    {kSimClockModuleUART,          3,  HW_SIM_SCGC4_ADDR, SIM_SCGC4_UART3_MASK},
    {kSimClockModuleUART,          4,  HW_SIM_SCGC1_ADDR, SIM_SCGC1_UART4_MASK},
    {kSimClockModuleUART,          5,  HW_SIM_SCGC1_ADDR, SIM_SCGC1_UART5_MASK},
    {kSimClockModuleESDHC,         0,  HW_SIM_SCGC3_ADDR, SIM_SCGC3_SDHC_MASK},

    /* Human-machine Interfaces*/
    {kSimClockModuleMax,           0,  0, 0}
};

/*! @brief CLOCK source OSC32KSEL setting config table for K64*/
const sim_clock_source_value_t kSimClockOsc32kSelTable [] = {
    {kSimOsc32kClock,      false,  false, kSimClockDividerMax},
    {kSimReserved,         false,  false, kSimClockDividerMax},
    {kSimRtc32kClock,      false,  false, kSimClockDividerMax},
    {kSimLpoClock,         false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source SDHC setting config table for K64*/
const sim_clock_source_value_t kSimClockSdhcSrcTable [] = {
    {kSimCoreClock,        false,  false, kSimClockDividerMax},
    {kSimClockPllfllSel,   true,   false, kSimClockDividerMax},
    {kSimOsc0ErClock,      false,  false, kSimClockDividerMax},
    {kSimSDHC0_CLKIN,      false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source TIME setting config table for K64*/
const sim_clock_source_value_t kSimClockTimeSrcTable [] = {
    {kSimCoreClock,        false,  false, kSimClockDividerMax},
    {kSimClockPllfllSel,   true,   false, kSimClockDividerMax},
    {kSimOsc0ErClock,      false,  false, kSimClockDividerMax},
    {kSimENET_1588_CLKIN,  false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source RMII setting config table for K64*/
const sim_clock_source_value_t kSimClockRmiiSrcTable [] = {
    {kSimEXTAL_Clock,      false,  false, kSimClockDividerMax},
    {kSimENET_1588_CLKIN,  false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source USB setting config table for K64*/
const sim_clock_source_value_t kSimClockUsbSrcTable [] = {
    {kSimUSB_CLKIN,        false,  false, kSimClockDividerMax},
    {kSimClockPllfllSel,   true,   true,  kSimClockDividerUsbDiv}
};

/*! @brief CLOCK source PLLFLLSEL setting config table for K64*/
const sim_clock_source_value_t kSimClockPllfllSelTable [] = {
    {kSimMcgFllClock,      false,  false, kSimClockDividerMax},
    {kSimMcgPll0Clock,     false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source TRACESEL setting config table for K64*/
const sim_clock_source_value_t kSimClockTraceSelTable [] = {
    {kSimMcgOutClock,      false,  false, kSimClockDividerMax},
    {kSimCoreClock,        false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source CLKOUT_SEL setting config table for K64*/
const sim_clock_source_value_t kSimClockClkoutSelTable [] = {
    {kSimFlexBusClock,     false,  false, kSimClockDividerMax},
    {kSimReserved,         false,  false, kSimClockDividerMax},
    {kSimFlashClock,       false,  false, kSimClockDividerMax},
    {kSimLpoClock,         false,  false, kSimClockDividerMax},
    {kSimMcgIrClock,       false,  false, kSimClockDividerMax},
    {kSimRtc32kClock,      false,  false, kSimClockDividerMax},
    {kSimReserved,         false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source RTCCLKOUTSEL setting config table for K64*/
const sim_clock_source_value_t kSimClockRtcClkoutSelTable [] = {
    {kSimRtc1hzClock,      false,  false, kSimClockDividerMax},
    {kSimRtc32kClock,      false,  false, kSimClockDividerMax}
};

/*! @brief CLOCK source value table for K64*/
const sim_clock_source_value_t *kSimClockSourceValueTable [] =  {
    NULL,
    NULL,
    kSimClockSdhcSrcTable,
    NULL,
    kSimClockTimeSrcTable,
    kSimClockRmiiSrcTable,
    NULL,
    kSimClockUsbSrcTable,
    NULL,
    NULL,
    NULL,
    kSimClockOsc32kSelTable,
    NULL,
    kSimClockPllfllSelTable,
    NULL,
    NULL,
    kSimClockTraceSelTable,
    kSimClockClkoutSelTable,
    kSimClockRtcClkoutSelTable,
};

/*! @}*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

