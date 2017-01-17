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

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call BOARD_ExtClk_Setup_HookUp to set the clock output for wireless modem.
 *
 * 3. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 4. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 5. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

uint8_t BOARD_ExtClk_Setup_HookUp(uint32_t clk_out_value)
{
    uint8_t result = 0;

    switch (clk_out_value)
    {
        case 4000000U:
            /* Start XCVR clock in order to derive MCGOUTCLK */
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK; /* Ungate PORTB and PORTC clock*/
            GPIOB->PDDR |= 0x00080000u;                                /* Set PORTB.19 as output - XCVR RESET pin */
            GPIOC->PDDR |= 0x00000001u;                                /* Set PORTC.0 as output - XCVR GPIO5 pin */
            PORTB->PCR[19] = (PORTB->PCR[19] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(0x01u); /* PORTB.19 as GPIO */
            PORTC->PCR[0] = (PORTC->PCR[0] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(0x01u);   /* PORTC.0 as GPIO*/
            GPIOC->PCOR = 0x00000001u;                                                    /* Clear XCVR GPIO5 pin*/
            GPIOB->PCOR = 0x00080000u;                                                    /* Clear XCVR RESET pin*/
            GPIOB->PSOR = 0x00080000u;                                                    /* Set XCVR RESET pin*/
            result = 1U; /*  The output was set successfully */
            break;
        case 0U:
            /* No initialization, modem remains in the reset state */
            result = 1U; /*  The output was set successfully */
            break;
        default:
            result = 0U; /* Requested value cannot be set */
            break;
    }
    return result;
}

void BOARD_BootClockVLPR(void)
{
    /*
    * Core clock: 4MHz
    */
    const sim_clock_config_t simConfig = {
        .pllFllSel = 0U,        /* PLLFLLSEL select FLL. */
        .er32kSrc = 2U,         /* ERCLK32K selection, use RTC. */
        .clkdiv1 = 0x00030000U, /* SIM_CLKDIV1. */
    };

    CLOCK_SetSimSafeDivs();

    CLOCK_BootToBlpiMode(0U, kMCG_IrcFast, kMCG_IrclkEnable);

    CLOCK_SetSimConfig(&simConfig);

    SystemCoreClock = 4000000U;

    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    SMC_SetPowerModeVlpr(SMC, false);
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
}

void BOARD_BootClockRUN(void)
{
    /*
    * Core clock: 48MHz
    * Bus clock: 24MHz
    */
    const mcg_pll_config_t pll0Config = {
        .enableMode = 0U,
        .prdiv = 0x1U,
        .vdiv = 0x0U,
    };
    const sim_clock_config_t simConfig = {
        .pllFllSel = 1U,        /* PLLFLLSEL select PLL. */
        .er32kSrc = 2U,         /* ERCLK32K selection, use RTC. */
        .clkdiv1 = 0x00010000U, /* SIM_CLKDIV1. */
    };

    /* Perform initialization of the wireless modem clock output */
    if (BOARD_ExtClk_Setup_HookUp(BOARD_XTAL0_CLK_HZ) != 1U)
    {
        /* If the initialization was not successfully, do not continue with clock setup */
        return;
    }
    CLOCK_SetSimSafeDivs();
    BOARD_InitOsc0();

    CLOCK_BootToPeeMode(kMCG_OscselOsc, kMCG_PllClkSelPll0, &pll0Config);

    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0);
    CLOCK_SetSimConfig(&simConfig);

    SystemCoreClock = 48000000U;
}

void BOARD_InitOsc0(void)
{
    const osc_config_t oscConfig = {.freq = BOARD_XTAL0_CLK_HZ,
                                    .capLoad = 0,
                                    .workMode = kOSC_ModeOscLowPower,
                                    .oscerConfig = {
                                        .enableMode = kOSC_ErClkEnable | kOSC_ErClkEnableInStop,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                                        .erclkDiv = 0U,
#endif
                                    }};

    CLOCK_InitOsc0(&oscConfig);

    /* Passing the XTAL0 frequency to clock driver. */
    CLOCK_SetXtal0Freq(BOARD_XTAL0_CLK_HZ);
}
