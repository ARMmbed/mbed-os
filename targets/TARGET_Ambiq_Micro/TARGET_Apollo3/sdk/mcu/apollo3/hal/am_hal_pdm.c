//*****************************************************************************
//
//! @file am_hal_pdm.c
//!
//! @brief HAL implementation for the PDM module.
//!
//! @addtogroup
//! @ingroup
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

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

//*****************************************************************************
//
// PDM magic number for handle verification.
//
//*****************************************************************************
#define AM_HAL_MAGIC_PDM                0xF956E2

#define AM_HAL_PDM_HANDLE_VALID(h)                                            \
    ((h) &&                                                                   \
     ((am_hal_handle_prefix_t *)(h))->s.bInit &&                              \
     (((am_hal_handle_prefix_t *)(h))->s.magic == AM_HAL_MAGIC_PDM))

//*****************************************************************************
//
// Convenience macro for passing errors.
//
//*****************************************************************************
#define RETURN_ON_ERROR(x)                                                    \
    if ((x) != AM_HAL_STATUS_SUCCESS)                                         \
    {                                                                         \
        return (x);                                                           \
    };

//*****************************************************************************
//
// Abbreviation for validating handles and returning errors.
//
//*****************************************************************************
#ifndef AM_HAL_DISABLE_API_VALIDATION

#define AM_HAL_PDM_HANDLE_CHECK(h)                                            \
    if (!AM_HAL_PDM_HANDLE_VALID(h))                                          \
    {                                                                         \
        return AM_HAL_STATUS_INVALID_HANDLE;                                  \
    }

#else

#define AM_HAL_PDM_HANDLE_CHECK(h)

#endif // AM_HAL_DISABLE_API_VALIDATION

//*****************************************************************************
//
// Helper macros for delays.
//
//*****************************************************************************
#define delay_ms(ms)                                                          \
    if (1)                                                                    \
    {                                                                         \
        am_hal_clkgen_status_t sClkGenStatus;                                 \
        am_hal_clkgen_status_get(&sClkGenStatus);                             \
        am_hal_flash_delay((ms) * (sClkGenStatus.ui32SysclkFreq / 3000));     \
    }

#define delay_us(us)                                                          \
    if (1)                                                                    \
    {                                                                         \
        am_hal_clkgen_status_t sClkGenStatus;                                 \
        am_hal_clkgen_status_get(&sClkGenStatus);                             \
        am_hal_flash_delay((us) * (sClkGenStatus.ui32SysclkFreq / 3000000));  \
    }

//*****************************************************************************
//
// Structure for handling PDM register state information for power up/down
//
//*****************************************************************************
typedef struct
{
    bool bValid;
}
am_hal_pdm_register_state_t;

//*****************************************************************************
//
// Structure for handling PDM HAL state information.
//
//*****************************************************************************
typedef struct
{
    am_hal_handle_prefix_t prefix;
    am_hal_pdm_register_state_t sRegState;
    uint32_t ui32Module;
}
am_hal_pdm_state_t;

//*****************************************************************************
//
// State structure for each module.
//
//*****************************************************************************
am_hal_pdm_state_t g_am_hal_pdm_states[AM_REG_PDM_NUM_MODULES];

//*****************************************************************************
//
// Static function definitions.
//
//*****************************************************************************
static uint32_t find_dma_threshold(uint32_t ui32TotalCount);

//*****************************************************************************
//
// Initialization function.
//
//*****************************************************************************
uint32_t
am_hal_pdm_initialize(uint32_t ui32Module, void **ppHandle)
{
    //
    // Check that the request module is in range.
    //
    if ( ui32Module >= AM_REG_PDM_NUM_MODULES )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    //
    // Check for valid arguements.
    //
    if (!ppHandle)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Check if the handle is unallocated.
    //
    if (g_am_hal_pdm_states[ui32Module].prefix.s.bInit)
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }

    //
    // Initialize the handle.
    //
    g_am_hal_pdm_states[ui32Module].prefix.s.bInit = true;
    g_am_hal_pdm_states[ui32Module].prefix.s.magic = AM_HAL_MAGIC_PDM;
    g_am_hal_pdm_states[ui32Module].ui32Module = ui32Module;
    g_am_hal_pdm_states[ui32Module].sRegState.bValid = false;

    //
    // Return the handle.
    //
    *ppHandle = (void *)&g_am_hal_pdm_states[ui32Module];

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// De-Initialization function.
//
//*****************************************************************************
uint32_t
am_hal_pdm_deinitialize(void *pHandle)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *)pHandle;

    //
    // Check the handle.
    //
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    //
    // Reset the handle.
    //
    pState->prefix.s.bInit = false;
    pState->prefix.s.magic = 0;
    pState->ui32Module = 0;

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Power control function.
//
//*****************************************************************************
uint32_t
am_hal_pdm_power_control(void *pHandle,
                         am_hal_sysctrl_power_state_e ePowerState,
                         bool bRetainState)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    am_hal_pwrctrl_periph_e ePDMPowerModule = ((am_hal_pwrctrl_periph_e)
                                               (AM_HAL_PWRCTRL_PERIPH_PDM +
                                                ui32Module));
    //
    // Check the handle.
    //
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    //
    // Decode the requested power state and update PDM operation accordingly.
    //
    switch (ePowerState)
    {
        //
        // Turn on the PDM.
        //
        case AM_HAL_SYSCTRL_WAKE:
            //
            // Make sure we don't try to restore an invalid state.
            //
            if (bRetainState && !pState->sRegState.bValid)
            {
                return AM_HAL_STATUS_INVALID_OPERATION;
            }

            //
            // Enable power control.
            //
            am_hal_pwrctrl_periph_enable(ePDMPowerModule);

            if (bRetainState)
            {
                //
                // Restore PDM registers
                //
                AM_CRITICAL_BEGIN;

                pState->sRegState.bValid = false;

                AM_CRITICAL_END;
            }
            break;

        //
        // Turn off the PDM.
        //
        case AM_HAL_SYSCTRL_NORMALSLEEP:
        case AM_HAL_SYSCTRL_DEEPSLEEP:
            if (bRetainState)
            {
                AM_CRITICAL_BEGIN;

                pState->sRegState.bValid = true;

                AM_CRITICAL_END;
            }

            //
            // Disable power control.
            //
            am_hal_pwrctrl_periph_disable(ePDMPowerModule);
            break;

        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Configure the PDM.
//
//*****************************************************************************
uint32_t
am_hal_pdm_configure(void *pHandle, am_hal_pdm_config_t *psConfig)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    //
    // Apply the config structure settings to the PCFG register.
    //
    PDMn(ui32Module)->PCFG_b.SOFTMUTE = psConfig->bSoftMute;
    PDMn(ui32Module)->PCFG_b.CYCLES = psConfig->ui32GainChangeDelay;
    PDMn(ui32Module)->PCFG_b.HPCUTOFF = psConfig->ui32HighPassCutoff;
    PDMn(ui32Module)->PCFG_b.ADCHPD = psConfig->bHighPassEnable;
    PDMn(ui32Module)->PCFG_b.SINCRATE = psConfig->ui32DecimationRate;
    PDMn(ui32Module)->PCFG_b.MCLKDIV = psConfig->eClkDivider;
    PDMn(ui32Module)->PCFG_b.PGALEFT = psConfig->eLeftGain;
    PDMn(ui32Module)->PCFG_b.PGARIGHT = psConfig->eRightGain;
    PDMn(ui32Module)->PCFG_b.LRSWAP = psConfig->bLRSwap;

    //
    // Set the PDM Core enable bit to enable PDM to PCM conversions.
    //
    PDMn(ui32Module)->PCFG_b.PDMCOREEN = PDM_PCFG_PDMCOREEN_EN;

    //
    // Program the "voice" registers.
    //
    PDMn(ui32Module)->VCFG_b.PDMCLKEN = PDM_VCFG_PDMCLKEN_DIS;
    PDMn(ui32Module)->VCFG_b.IOCLKEN = PDM_VCFG_IOCLKEN_DIS;
    PDMn(ui32Module)->VCFG_b.RSTB = PDM_VCFG_RSTB_RESET;
    PDMn(ui32Module)->VCFG_b.CHSET = psConfig->ePCMChannels;
    PDMn(ui32Module)->VCFG_b.PCMPACK = psConfig->bDataPacking;
    PDMn(ui32Module)->VCFG_b.SELAP = psConfig->ePDMClkSource;
    PDMn(ui32Module)->VCFG_b.DMICKDEL = psConfig->bPDMSampleDelay;
    PDMn(ui32Module)->VCFG_b.BCLKINV = psConfig->bInvertI2SBCLK;
    PDMn(ui32Module)->VCFG_b.I2SEN = psConfig->bI2SEnable;
    PDMn(ui32Module)->VCFG_b.PDMCLKSEL = psConfig->ePDMClkSpeed;

    delay_us(100);

    PDMn(ui32Module)->VCFG_b.RSTB = PDM_VCFG_RSTB_NORM;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Enable the PDM.
//
//*****************************************************************************
uint32_t
am_hal_pdm_enable(void *pHandle)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    PDMn(ui32Module)->VCFG_b.IOCLKEN = PDM_VCFG_IOCLKEN_EN;
    PDMn(ui32Module)->VCFG_b.PDMCLKEN = PDM_VCFG_PDMCLKEN_EN;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Disable the PDM.
//
//*****************************************************************************
uint32_t
am_hal_pdm_disable(void *pHandle)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    PDMn(ui32Module)->VCFG_b.IOCLKEN = PDM_VCFG_IOCLKEN_DIS;
    PDMn(ui32Module)->VCFG_b.PDMCLKEN = PDM_VCFG_PDMCLKEN_DIS;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Given the total number of bytes in a DMA transaction, find a reasonable
// threshold setting.
//
//*****************************************************************************
static uint32_t
find_dma_threshold(uint32_t ui32TotalCount)
{
    //
    // Start with a threshold value of 24, and search downward for values that
    // fit our criteria.
    //
    uint32_t ui32Threshold;
    uint32_t ui32Minimum = AM_HAL_PDM_DMA_THRESHOLD_MIN;

    for ( ui32Threshold = 24; ui32Threshold >= ui32Minimum; ui32Threshold -= 4 )
    {
        //
        // With our loop parameters, we've already guaranteed that the
        // threshold will be no higher than 24, and that it will be divisible
        // by 4. The only remaining requirement is that ui32TotalCount must
        // also be divisible by the threshold.
        //
        if ((ui32TotalCount % ui32Threshold) == 0)
        {
            break;
        }
    }

    //
    // If we found an appropriate value, we'll return it here. Otherwise, we
    // will return zero.
    //
    if (ui32Threshold < ui32Minimum)
    {
        ui32Threshold = 0;
    }

    return ui32Threshold;
}

//*****************************************************************************
//
// Starts a DMA transaction from the PDM directly to SRAM
//
//*****************************************************************************
uint32_t
am_hal_pdm_dma_start(void *pHandle, am_hal_pdm_transfer_t *pDmaCfg)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    //
    // Find an appropriate threshold size for this transfer.
    //
    uint32_t ui32Threshold = find_dma_threshold(pDmaCfg->ui32TotalCount);

    //
    // If we didn't find a threshold that will work, throw an error.
    //
    if (ui32Threshold == 0)
    {
        return AM_HAL_PDM_STATUS_BAD_TOTALCOUNT;
    }

    PDMn(ui32Module)->FIFOTHR = ui32Threshold;

    //
    // Configure DMA.
    //
    PDMn(ui32Module)->DMACFG = 0;
    PDMn(ui32Module)->DMACFG_b.DMAPRI = PDM_DMACFG_DMAPRI_LOW;
    PDMn(ui32Module)->DMACFG_b.DMADIR = PDM_DMACFG_DMADIR_P2M;
    PDMn(ui32Module)->DMATOTCOUNT = pDmaCfg->ui32TotalCount;
    PDMn(ui32Module)->DMATARGADDR = pDmaCfg->ui32TargetAddr;

    //
    // Make sure the trigger is set for threshold.
    //
    PDMn(ui32Module)->DMATRIGEN_b.DTHR = 1;

    //
    // Enable DMA
    //
    PDMn(ui32Module)->DMACFG_b.DMAEN = PDM_DMACFG_DMAEN_EN;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Flush the PDM FIFO
//
//*****************************************************************************
uint32_t
am_hal_pdm_fifo_flush(void *pHandle)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    PDMn(ui32Module)->FIFOFLUSH = 1;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Enable PDM passthrough to the I2S slave.
//
//*****************************************************************************
uint32_t
am_hal_pdm_i2s_enable(void *pHandle)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);
    uint32_t ui32Module = pState->ui32Module;

    PDMn(ui32Module)->VCFG_b.I2SEN = PDM_VCFG_I2SEN_EN;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Disable PDM passthrough to the I2S slave.
//
//*****************************************************************************
uint32_t
am_hal_pdm_i2s_disable(void *pHandle)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    AM_HAL_PDM_HANDLE_CHECK(pHandle);
    uint32_t ui32Module = pState->ui32Module;

    PDMn(ui32Module)->VCFG_b.I2SEN = PDM_VCFG_I2SEN_DIS;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Interrupt enable.
//
//*****************************************************************************
uint32_t
am_hal_pdm_interrupt_enable(void *pHandle, uint32_t ui32IntMask)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Check the handle.
    //
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    PDMn(ui32Module)->INTEN |= ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Interrupt disable.
//
//*****************************************************************************
uint32_t
am_hal_pdm_interrupt_disable(void *pHandle, uint32_t ui32IntMask)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Check the handle.
    //
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    PDMn(ui32Module)->INTEN &= ~ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Interrupt clear.
//
//*****************************************************************************
uint32_t
am_hal_pdm_interrupt_clear(void *pHandle, uint32_t ui32IntMask)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Check the handle.
    //
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    PDMn(ui32Module)->INTCLR = ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// Returns the interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_pdm_interrupt_status_get(void *pHandle, uint32_t *pui32Status, bool bEnabledOnly)
{
    am_hal_pdm_state_t *pState = (am_hal_pdm_state_t *) pHandle;
    uint32_t ui32Module = pState->ui32Module;

    //
    // Check the handle.
    //
    AM_HAL_PDM_HANDLE_CHECK(pHandle);

    //
    // If requested, only return the interrupts that are enabled.
    //
    if ( bEnabledOnly )
    {
        *pui32Status = PDMn(ui32Module)->INTSTAT;
        *pui32Status &= PDMn(ui32Module)->INTEN;
    }
    else
    {
        *pui32Status = PDMn(ui32Module)->INTSTAT;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
