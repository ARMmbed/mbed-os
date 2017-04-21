/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_sdramc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Define macros for SDRAM driver. */
#define SDRAMC_ONEMILLSEC_NANOSECONDS (1000000U)
#define SDRAMC_ONESECOND_MILLISECONDS (1000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Get instance number for SDRAMC module.
 *
 * @param base SDRAMC peripheral base address
 */
static uint32_t SDRAMC_GetInstance(SDRAM_Type *base);

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Pointers to SDRAMC clocks for each instance. */
static const clock_ip_name_t s_sdramClock[FSL_FEATURE_SOC_SDRAM_COUNT] = SDRAM_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*! @brief Pointers to SDRAMC bases for each instance. */
static SDRAM_Type *const s_sdramcBases[] = SDRAM_BASE_PTRS;
/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t SDRAMC_GetInstance(SDRAM_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_SDRAM_COUNT; instance++)
    {
        if (s_sdramcBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < FSL_FEATURE_SOC_SDRAM_COUNT);

    return instance;
}

void SDRAMC_Init(SDRAM_Type *base, sdramc_config_t *configure)
{
    assert(configure);
    assert(configure->refreshConfig);
    assert(configure->blockConfig);
    assert(configure->refreshConfig->busClock_Hz);

    sdramc_blockctl_config_t *bctlConfig = configure->blockConfig;
    sdramc_refresh_config_t *refreshConfig = configure->refreshConfig;
    uint32_t count;
    uint32_t index;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Un-gate sdram controller clock. */
    CLOCK_EnableClock(s_sdramClock[SDRAMC_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Initialize sdram Auto refresh timing. */
    count = refreshConfig->sdramRefreshRow * (refreshConfig->busClock_Hz / SDRAMC_ONESECOND_MILLISECONDS);
    count = (count / SDRAMC_ONEMILLSEC_NANOSECONDS) / 16 - 1;
    base->CTRL = SDRAM_CTRL_RC(count) | SDRAM_CTRL_RTIM(refreshConfig->refreshTime);

    for (index = 0; index < configure->numBlockConfig; index++)
    {
        /* Set the sdram block control. */
        base->BLOCK[index].AC = SDRAM_AC_PS(bctlConfig->portSize) | SDRAM_AC_CASL(bctlConfig->latency) |
                                SDRAM_AC_CBM(bctlConfig->location) | (bctlConfig->address & SDRAM_AC_BA_MASK);

        base->BLOCK[index].CM = (bctlConfig->addressMask & SDRAM_CM_BAM_MASK) | SDRAM_CM_V_MASK;

        /* Increases to the next sdram block. */
        bctlConfig++;
    }
}

void SDRAMC_Deinit(SDRAM_Type *base)
{
    /* Set the SDRAMC invalid, do not decode DRAM accesses. */
    SDRAMC_EnableOperateValid(base, kSDRAMC_Block0, false);
    SDRAMC_EnableOperateValid(base, kSDRAMC_Block1, false);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable SDRAM clock. */
    CLOCK_DisableClock(s_sdramClock[SDRAMC_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void SDRAMC_SendCommand(SDRAM_Type *base, sdramc_block_selection_t block, sdramc_command_t command)
{
    switch (command)
    {
        /* Initiate mrs command. */
        case kSDRAMC_ImrsCommand:
            base->BLOCK[block].AC |= SDRAM_AC_IMRS_MASK;
            break;            
        /* Initiate precharge command. */
        case kSDRAMC_PrechargeCommand:
            base->BLOCK[block].AC |= SDRAM_AC_IP_MASK;
            break;
        /* Enable Auto refresh command. */
        case kSDRAMC_AutoRefreshEnableCommand:
            base->BLOCK[block].AC |= SDRAM_AC_RE_MASK;
            break;
        /* Disable Auto refresh command. */
        case kSDRAMC_AutoRefreshDisableCommand:
            base->BLOCK[block].AC &= ~SDRAM_AC_RE_MASK;
            break;
        /* Enter self-refresh command. */
        case kSDRAMC_SelfrefreshEnterCommand:
            base->CTRL |= SDRAM_CTRL_IS_MASK;
            break;
        /* Exit self-refresh command. */
        case kSDRAMC_SelfrefreshExitCommand:
            base->CTRL &= ~SDRAM_CTRL_IS_MASK;
            break;
        default:
            break;
    }
}
