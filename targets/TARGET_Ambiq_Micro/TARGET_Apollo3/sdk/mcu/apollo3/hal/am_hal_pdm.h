//*****************************************************************************
//
//! @file am_hal_pdm.h
//!
//! @brief API for the PDM module
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

#ifndef AM_HAL_PDM_H
#define AM_HAL_PDM_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// CMSIS-style macro for handling a variable IOS module number.
//
//*****************************************************************************
#define AM_REG_PDM_NUM_MODULES                       1
#define PDMn(n) ((PDM_Type*)(PDM_BASE + (n * (PDM_BASE - PDM_BASE))))

//*****************************************************************************
//
// DMA threshold minimum.
//
// The PDM DMA works best if its threshold value is set to a multiple of 4
// between 16 and 24, but it will technically allow threshold settings between
// 4 and 24. This macro sets the minimum threshold value that the HAL layer
// will allow.
//
//*****************************************************************************
#define AM_HAL_PDM_DMA_THRESHOLD_MIN        16

//*****************************************************************************
//
// PDM-specific error conditions.
//
//*****************************************************************************
typedef enum
{
    //
    // The PDM HAL will throw this error if it can't find a threshold value to
    // match the total-count value passed in by a caller requesting a DMA
    // transfer. The PDM hardware requires all DMA transactions to be evenly
    // divisible in chunks of one FIFO size or smaller. Try changing your
    // ui32TotalCount value to a more evenly divisible number.
    //
    AM_HAL_PDM_STATUS_BAD_TOTALCOUNT = AM_HAL_STATUS_MODULE_SPECIFIC_START,
}
am_hal_pdm_status_e;

//*****************************************************************************
//
// Gain settings.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_PDM_GAIN_P405DB = PDM_PCFG_PGALEFT_P405DB,
    AM_HAL_PDM_GAIN_P390DB = PDM_PCFG_PGALEFT_P390DB,
    AM_HAL_PDM_GAIN_P375DB = PDM_PCFG_PGALEFT_P375DB,
    AM_HAL_PDM_GAIN_P360DB = PDM_PCFG_PGALEFT_P360DB,
    AM_HAL_PDM_GAIN_P345DB = PDM_PCFG_PGALEFT_P345DB,
    AM_HAL_PDM_GAIN_P330DB = PDM_PCFG_PGALEFT_P330DB,
    AM_HAL_PDM_GAIN_P315DB = PDM_PCFG_PGALEFT_P315DB,
    AM_HAL_PDM_GAIN_P300DB = PDM_PCFG_PGALEFT_P300DB,
    AM_HAL_PDM_GAIN_P285DB = PDM_PCFG_PGALEFT_P285DB,
    AM_HAL_PDM_GAIN_P270DB = PDM_PCFG_PGALEFT_P270DB,
    AM_HAL_PDM_GAIN_P255DB = PDM_PCFG_PGALEFT_P255DB,
    AM_HAL_PDM_GAIN_P240DB = PDM_PCFG_PGALEFT_P240DB,
    AM_HAL_PDM_GAIN_P225DB = PDM_PCFG_PGALEFT_P225DB,
    AM_HAL_PDM_GAIN_P210DB = PDM_PCFG_PGALEFT_P210DB,
    AM_HAL_PDM_GAIN_P195DB = PDM_PCFG_PGALEFT_P195DB,
    AM_HAL_PDM_GAIN_P180DB = PDM_PCFG_PGALEFT_P180DB,
    AM_HAL_PDM_GAIN_P165DB = PDM_PCFG_PGALEFT_P165DB,
    AM_HAL_PDM_GAIN_P150DB = PDM_PCFG_PGALEFT_P150DB,
    AM_HAL_PDM_GAIN_P135DB = PDM_PCFG_PGALEFT_P135DB,
    AM_HAL_PDM_GAIN_P120DB = PDM_PCFG_PGALEFT_P120DB,
    AM_HAL_PDM_GAIN_P105DB = PDM_PCFG_PGALEFT_P105DB,
    AM_HAL_PDM_GAIN_P90DB  = PDM_PCFG_PGALEFT_P90DB,
    AM_HAL_PDM_GAIN_P75DB  = PDM_PCFG_PGALEFT_P75DB,
    AM_HAL_PDM_GAIN_P60DB  = PDM_PCFG_PGALEFT_P60DB,
    AM_HAL_PDM_GAIN_P45DB  = PDM_PCFG_PGALEFT_P45DB,
    AM_HAL_PDM_GAIN_P30DB  = PDM_PCFG_PGALEFT_P30DB,
    AM_HAL_PDM_GAIN_P15DB  = PDM_PCFG_PGALEFT_P15DB,
    AM_HAL_PDM_GAIN_0DB    = PDM_PCFG_PGALEFT_0DB,
    AM_HAL_PDM_GAIN_M15DB  = PDM_PCFG_PGALEFT_M15DB,
    AM_HAL_PDM_GAIN_M300DB = PDM_PCFG_PGALEFT_M300DB,
    AM_HAL_PDM_GAIN_M45DB  = PDM_PCFG_PGALEFT_M45DB,
    AM_HAL_PDM_GAIN_M60DB  = PDM_PCFG_PGALEFT_M60DB,
}
am_hal_pdm_gain_e;

//*****************************************************************************
//
// Clock Source selection.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_PDM_INTERNAL_CLK = PDM_VCFG_SELAP_INTERNAL,
    AM_HAL_PDM_I2S_CLK      = PDM_VCFG_SELAP_I2S,
}
am_hal_pdm_clksrc_e;

//*****************************************************************************
//
// PDM internal clock speed selection.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_PDM_CLK_DISABLE = PDM_VCFG_PDMCLKSEL_DISABLE,
    AM_HAL_PDM_CLK_12MHZ   = PDM_VCFG_PDMCLKSEL_12MHz,
    AM_HAL_PDM_CLK_6MHZ    = PDM_VCFG_PDMCLKSEL_6MHz,
    AM_HAL_PDM_CLK_3MHZ    = PDM_VCFG_PDMCLKSEL_3MHz,
    AM_HAL_PDM_CLK_1_5MHZ  = PDM_VCFG_PDMCLKSEL_1_5MHz,
    AM_HAL_PDM_CLK_750KHZ  = PDM_VCFG_PDMCLKSEL_750KHz,
    AM_HAL_PDM_CLK_375KHZ  = PDM_VCFG_PDMCLKSEL_375KHz,
    AM_HAL_PDM_CLK_187KHZ  = PDM_VCFG_PDMCLKSEL_187KHz,
}
am_hal_pdm_clkspd_e;

//*****************************************************************************
//
// PDM clock divider setting.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_PDM_MCLKDIV_4 = PDM_PCFG_MCLKDIV_MCKDIV4,
    AM_HAL_PDM_MCLKDIV_3 = PDM_PCFG_MCLKDIV_MCKDIV3,
    AM_HAL_PDM_MCLKDIV_2 = PDM_PCFG_MCLKDIV_MCKDIV2,
    AM_HAL_PDM_MCLKDIV_1 = PDM_PCFG_MCLKDIV_MCKDIV1,
}
am_hal_pdm_mclkdiv_e;

//*****************************************************************************
//
// PCM Channel Select.
//
//*****************************************************************************
typedef enum
{
    AM_HAL_PDM_CHANNEL_LEFT   = PDM_VCFG_CHSET_LEFT,
    AM_HAL_PDM_CHANNEL_RIGHT  = PDM_VCFG_CHSET_RIGHT,
    AM_HAL_PDM_CHANNEL_STEREO = PDM_VCFG_CHSET_STEREO,
}
am_hal_pdm_chset_e;

//*****************************************************************************
//
// PDM power state settings.
//
//*****************************************************************************
#define AM_HAL_PDM_POWER_ON           AM_HAL_SYSCTRL_WAKE
#define AM_HAL_PDM_POWER_OFF          AM_HAL_SYSCTRL_NORMALSLEEP

//*****************************************************************************
//
// PDM interrupts.
//
//*****************************************************************************
#define AM_HAL_PDM_INT_DERR           PDM_INTSTAT_DERR_Msk
#define AM_HAL_PDM_INT_DCMP           PDM_INTSTAT_DCMP_Msk
#define AM_HAL_PDM_INT_UNDFL          PDM_INTSTAT_UNDFL_Msk
#define AM_HAL_PDM_INT_OVF            PDM_INTSTAT_OVF_Msk
#define AM_HAL_PDM_INT_THR            PDM_INTSTAT_THR_Msk

//*****************************************************************************
//
// Configuration structure for the PDM
//
//*****************************************************************************
typedef struct
{
    // Clock
    am_hal_pdm_mclkdiv_e eClkDivider;

    // Gain
    am_hal_pdm_gain_e eLeftGain;
    am_hal_pdm_gain_e eRightGain;

    // Decimation Rate
    uint32_t ui32DecimationRate;

    // Filters
    bool bHighPassEnable;
    uint32_t ui32HighPassCutoff;

    // PDMCLKSEL
    am_hal_pdm_clkspd_e ePDMClkSpeed;

    // BCLKINV
    bool bInvertI2SBCLK;

    // SELAP
    am_hal_pdm_clksrc_e ePDMClkSource;

    // DMICKDEL
    bool bPDMSampleDelay;

    // PCMPACK
    bool bDataPacking;

    // CHSET
    am_hal_pdm_chset_e ePCMChannels;

    uint32_t ui32GainChangeDelay;

    bool bI2SEnable;

    bool bSoftMute;

    bool bLRSwap;
}
am_hal_pdm_config_t;

//*****************************************************************************
//
// DMA transfer structure
//
//*****************************************************************************
typedef struct
{
    uint32_t ui32TargetAddr;
    uint32_t ui32TotalCount;
}
am_hal_pdm_transfer_t;

// Init/De-init.
extern uint32_t am_hal_pdm_initialize(uint32_t ui32Module, void **ppHandle);
extern uint32_t am_hal_pdm_deinitialize(void *pHandle);

// Power
extern uint32_t am_hal_pdm_power_control(void *pHandle, am_hal_sysctrl_power_state_e ePowerState, bool bRetainState);

// Config
extern uint32_t am_hal_pdm_configure(void *pHandle, am_hal_pdm_config_t *psConfig);

// Enable/Disable
extern uint32_t am_hal_pdm_enable(void *pHandle);
extern uint32_t am_hal_pdm_disable(void *pHandle);

// Gather PDM data.
extern uint32_t am_hal_pdm_dma_start(void *pHandle, am_hal_pdm_transfer_t *pDmaCfg);

// Flush the PDM FIFO.
extern uint32_t am_hal_pdm_fifo_flush(void *pHandle);

// I2S Passthrough
extern uint32_t am_hal_pdm_i2s_enable(void *pHandle);
extern uint32_t am_hal_pdm_i2s_disable(void *pHandle);

// Interrupts.
extern uint32_t am_hal_pdm_interrupt_enable(void *pHandle, uint32_t ui32IntMask);
extern uint32_t am_hal_pdm_interrupt_disable(void *pHandle, uint32_t ui32IntMask);
extern uint32_t am_hal_pdm_interrupt_clear(void *pHandle, uint32_t ui32IntMask);
extern uint32_t am_hal_pdm_interrupt_status_get(void *pHandle, uint32_t *pui32Status, bool bEnabledOnly);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_PDM_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
