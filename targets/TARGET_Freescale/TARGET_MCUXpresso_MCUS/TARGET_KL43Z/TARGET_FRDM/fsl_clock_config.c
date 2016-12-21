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

#include "fsl_common.h"
#include "fsl_smc.h"
#include "fsl_clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcglite_config_t mcgliteConfig; /*!< MCG configuration.      */
    sim_clock_config_t simConfig;   /*!< SIM configuration.      */
    osc_config_t oscConfig;         /*!< OSC configuration.      */
    uint32_t coreClock;             /*!< core clock frequency.   */
} clock_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/* Configuration for enter VLPR mode. Core clock = 2MHz. */
const clock_config_t g_defaultClockConfigVlpr = {
    .mcgliteConfig =
        {
            .outSrc = kMCGLITE_ClkSrcLirc,
            .irclkEnableMode = kMCGLITE_IrclkEnable,
            .ircs = kMCGLITE_Lirc2M,
            .fcrdiv = kMCGLITE_LircDivBy1,
            .lircDiv2 = kMCGLITE_LircDivBy1,
            .hircEnableInNotHircMode = false,
        },
    .simConfig =
        {
            .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
        },
    .oscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
                  .capLoad = 0U,
                  .workMode = kOSC_ModeOscLowPower,
                  .oscerConfig =
                      {
                          .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                          .erclkDiv = 0U,
#endif
                      }},
    .coreClock = 2000000U, /* Core clock frequency */
};

/* Configuration for enter RUN mode. Core clock = 48000000Hz. */
const clock_config_t g_defaultClockConfigRun = {
    .mcgliteConfig =
        {
            .outSrc = kMCGLITE_ClkSrcHirc,
            .irclkEnableMode = kMCGLITE_IrclkEnable,
            .ircs = kMCGLITE_Lirc8M,
            .fcrdiv = kMCGLITE_LircDivBy1,
            .lircDiv2 = kMCGLITE_LircDivBy1,
            .hircEnableInNotHircMode = true,
        },
    .simConfig =
        {
            .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
        },
    .oscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
                  .capLoad = 0U,
                  .workMode = kOSC_ModeOscLowPower,
                  .oscerConfig =
                      {
                          .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                          .erclkDiv = 0U,
#endif
                      }},
    .coreClock = 48000000U, /* Core clock frequency */
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_SetMcgliteConfig to set MCG_Lite configuration.
 *
 * 3. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

void BOARD_BootClockVLPR(void)
{
    CLOCK_SetSimSafeDivs();

    CLOCK_SetMcgliteConfig(&g_defaultClockConfigVlpr.mcgliteConfig);

    CLOCK_SetSimConfig(&g_defaultClockConfigVlpr.simConfig);

    SystemCoreClock = g_defaultClockConfigVlpr.coreClock;

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    SMC_SetPowerModeVlpr(SMC);
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
}

void BOARD_BootClockRUN(void)
{
    CLOCK_SetSimSafeDivs();

    CLOCK_SetMcgliteConfig(&g_defaultClockConfigRun.mcgliteConfig);

    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);

    SystemCoreClock = g_defaultClockConfigRun.coreClock;
}
