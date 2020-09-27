//*****************************************************************************
//
//  am_hal_stimer.h
//! @file
//!
//! @brief Functions for interfacing with the system timer (STIMER).
//!
//! @addtogroup stimer3 System Timer (STIMER)
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_HAL_STIMER_H
#define AM_HAL_STIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

//
//! Compute address of a given COMPARE register.
//! @note - The parameter n should be 0 (as only 1 stimer module exists).
//!         For Apollo3, the parameter r should be 0-7 (compare) or 0-3 (capture).
//
#define AM_REG_STIMER_COMPARE(n, r)     (CTIMERADDRn(CTIMER, n, SCMPR0) +   \
                                         (r * (offsetof(CTIMER_Type, SCMPR1) - offsetof(CTIMER_Type, SCMPR0))))

//! Compute address of a given CAPTURE register. r should be 0-3.
#define AM_REG_STIMER_CAPTURE(n, r)     (CTIMERADDRn(CTIMER, n, SCAPT0) +   \
                                         (r * (offsetof(CTIMER_Type, SCAPT1) - offsetof(CTIMER_Type, SCAPT0))))

//! Compute address of a given NVRAM register. r should be 0-3.
#define AM_REG_STIMER_NVRAM(n, r)       (CTIMERADDRn(CTIMER, n, SNVR0) +    \
                                         (r * (offsetof(CTIMER_Type, SNVR1) - offsetof(CTIMER_Type, SNVR0))))


//*****************************************************************************
//
//! @name Interrupt Status Bits
//! @brief Interrupt Status Bits for enable/disble use
//!
//! These macros may be used to set and clear interrupt bits
//! @{
//
//*****************************************************************************
#define AM_HAL_STIMER_INT_COMPAREA         CTIMER_STMINTSTAT_COMPAREA_Msk
#define AM_HAL_STIMER_INT_COMPAREB         CTIMER_STMINTSTAT_COMPAREB_Msk
#define AM_HAL_STIMER_INT_COMPAREC         CTIMER_STMINTSTAT_COMPAREC_Msk
#define AM_HAL_STIMER_INT_COMPARED         CTIMER_STMINTSTAT_COMPARED_Msk
#define AM_HAL_STIMER_INT_COMPAREE         CTIMER_STMINTSTAT_COMPAREE_Msk
#define AM_HAL_STIMER_INT_COMPAREF         CTIMER_STMINTSTAT_COMPAREF_Msk
#define AM_HAL_STIMER_INT_COMPAREG         CTIMER_STMINTSTAT_COMPAREG_Msk
#define AM_HAL_STIMER_INT_COMPAREH         CTIMER_STMINTSTAT_COMPAREH_Msk

#define AM_HAL_STIMER_INT_OVERFLOW         CTIMER_STMINTSTAT_OVERFLOW_Msk

#define AM_HAL_STIMER_INT_CAPTUREA         CTIMER_STMINTSTAT_CAPTUREA_Msk
#define AM_HAL_STIMER_INT_CAPTUREB         CTIMER_STMINTSTAT_CAPTUREB_Msk
#define AM_HAL_STIMER_INT_CAPTUREC         CTIMER_STMINTSTAT_CAPTUREC_Msk
#define AM_HAL_STIMER_INT_CAPTURED         CTIMER_STMINTSTAT_CAPTURED_Msk

//! @}

//*****************************************************************************
//
//! @name STimer Configuration Bits
//! @brief Interrupt Status Bits for enable/disble use
//!
//! These macros may be used to set and clear interrupt bits
//! @{
//
//*****************************************************************************
#define AM_HAL_STIMER_CFG_THAW              _VAL2FLD(CTIMER_STCFG_FREEZE,       CTIMER_STCFG_FREEZE_THAW)
#define AM_HAL_STIMER_CFG_FREEZE            _VAL2FLD(CTIMER_STCFG_FREEZE,       CTIMER_STCFG_FREEZE_FREEZE)
#define AM_HAL_STIMER_CFG_RUN               _VAL2FLD(CTIMER_STCFG_CLEAR,        CTIMER_STCFG_CLEAR_RUN)
#define AM_HAL_STIMER_CFG_CLEAR             _VAL2FLD(CTIMER_STCFG_CLEAR,        CTIMER_STCFG_CLEAR_CLEAR)
#define AM_HAL_STIMER_CFG_COMPARE_A_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_A_EN, CTIMER_STCFG_COMPARE_A_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_B_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_B_EN, CTIMER_STCFG_COMPARE_B_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_C_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_C_EN, CTIMER_STCFG_COMPARE_C_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_D_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_D_EN, CTIMER_STCFG_COMPARE_D_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_E_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_E_EN, CTIMER_STCFG_COMPARE_E_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_F_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_F_EN, CTIMER_STCFG_COMPARE_F_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_G_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_G_EN, CTIMER_STCFG_COMPARE_G_EN_ENABLE)
#define AM_HAL_STIMER_CFG_COMPARE_H_ENABLE  _VAL2FLD(CTIMER_STCFG_COMPARE_H_EN, CTIMER_STCFG_COMPARE_H_EN_ENABLE)

//! @}

//*****************************************************************************
//
//! @name Clock Configuration options
//! @brief STimer Configuration register options.
//!
//! These options are to be used with the am_hal_stimer_config() function.
//!  @{
//
//*****************************************************************************
#define AM_HAL_STIMER_NO_CLK            _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_NOCLK)
#define AM_HAL_STIMER_HFRC_3MHZ         _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_HFRC_DIV16)
#define AM_HAL_STIMER_HFRC_187_5KHZ     _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_HFRC_DIV256)
#define AM_HAL_STIMER_XTAL_32KHZ        _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_XTAL_DIV1)
#define AM_HAL_STIMER_XTAL_16KHZ        _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_XTAL_DIV2)
#define AM_HAL_STIMER_XTAL_1KHZ         _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_XTAL_DIV32)
#define AM_HAL_STIMER_LFRC_1KHZ         _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_LFRC_DIV1)
#define AM_HAL_STIMER_HFRC_CTIMER0A     _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_CTIMER0A)
#define AM_HAL_STIMER_HFRC_CTIMER0B     _VAL2FLD(CTIMER_STCFG_CLKSEL, CTIMER_STCFG_CLKSEL_CTIMER0B)
//! @}



//*****************************************************************************
//
//! @name Capture Control Register options.
//! @brief Configuration options for capture control register.
//!
//! These options are to be used with the am_hal_stimer_capture_control_set
//! function.
//!  @{
//
//*****************************************************************************
#define AM_HAL_STIMER_CAPTURE0_ENABLE   _VAL2FLD(CTIMER_CAPTURECONTROL_CAPTURE0, CTIMER_CAPTURECONTROL_CAPTURE0_ENABLE)
#define AM_HAL_STIMER_CAPTURE1_ENABLE   _VAL2FLD(CTIMER_CAPTURECONTROL_CAPTURE1, CTIMER_CAPTURECONTROL_CAPTURE1_ENABLE)
#define AM_HAL_STIMER_CAPTURE2_ENABLE   _VAL2FLD(CTIMER_CAPTURECONTROL_CAPTURE2, CTIMER_CAPTURECONTROL_CAPTURE2_ENABLE)
#define AM_HAL_STIMER_CAPTURE3_ENABLE   _VAL2FLD(CTIMER_CAPTURECONTROL_CAPTURE3, CTIMER_CAPTURECONTROL_CAPTURE3_ENABLE)

//! @}

//*****************************************************************************
//
//! Stimer configuration structure
//
//*****************************************************************************
typedef struct
{
    //
    //! Configuration options for the STIMER
    //
    uint32_t ui32STimerConfig;
}
am_hal_stimer_config_t;

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern uint32_t am_hal_stimer_config(uint32_t ui32STimerConfig);
extern uint32_t am_hal_stimer_counter_get(void);
extern void     am_hal_stimer_counter_clear(void);
extern void     am_hal_stimer_compare_delta_set(uint32_t ui32CmprInstance,
                                                uint32_t ui32Delta);
extern uint32_t am_hal_stimer_compare_get(uint32_t ui32CmprInstance);
extern void     am_hal_stimer_capture_start(uint32_t ui32CaptureNum,
                                            uint32_t ui32GPIONumber,
                                            bool bPolarity);
extern void     am_hal_stimer_capture_stop(uint32_t ui32CaptureNum);
extern uint32_t am_hal_stimer_capture_get(uint32_t ui32CaptureNum);
extern void am_hal_stimer_nvram_set(uint32_t ui32NvramNum, uint32_t ui32NvramVal);
extern uint32_t am_hal_stimer_nvram_get(uint32_t ui32NvramNum);
extern void     am_hal_stimer_int_enable(uint32_t ui32Interrupt);
extern uint32_t am_hal_stimer_int_enable_get(void);
extern void     am_hal_stimer_int_disable(uint32_t ui32Interrupt);
extern void     am_hal_stimer_int_set(uint32_t ui32Interrupt);
extern void     am_hal_stimer_int_clear(uint32_t ui32Interrupt);
extern uint32_t am_hal_stimer_int_status_get(bool bEnabledOnly);


#ifdef __cplusplus
}
#endif

#endif // AM_HAL_STIMER_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
