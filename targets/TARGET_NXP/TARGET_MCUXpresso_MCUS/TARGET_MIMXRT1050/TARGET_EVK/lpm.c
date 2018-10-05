/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "lpm.h"
#include "fsl_gpc.h"
#include "fsl_dcdc.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "fsl_iomuxc.h"
#include "fsl_clock_config.h"
#include "serial_api.h"
#include "mbed_critical.h"
#include "cmsis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPM_GPC_IMR_NUM (sizeof(GPC->IMR) / sizeof(GPC->IMR[0]))

typedef struct _lpm_clock_context
{
    uint32_t armDiv;
    uint32_t ahbDiv;
    uint32_t ipgDiv;
    uint32_t perDiv;
    uint32_t perSel;
    uint32_t periphSel;
    uint32_t preperiphSel;
    uint32_t pfd480;
    uint32_t pfd528;
    uint32_t pllArm_loopdiv;
    uint32_t pllArm;
    uint32_t pllSys;
    uint32_t pllUsb1;
    uint32_t pllUsb2;
    uint32_t pllAudio;
    uint32_t pllVideo;
    uint32_t pllEnet;
    uint32_t is_valid;
} lpm_clock_context_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

static lpm_clock_context_t s_clockContext;
static uint32_t s_DllBackupValue = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_SetLPClockGate(void)
{
    CCM->CCGR0 = 0x0F4000C5U;
    CCM->CCGR1 = 0x541C0000U;
    CCM->CCGR2 = 0x00150010U;
    CCM->CCGR3 = 0x50040130U;
    CCM->CCGR4 = 0x00005514U;
    CCM->CCGR5 = 0x51001105U;
    CCM->CCGR6 = 0x005405C0U;
}

void BOARD_ResetLPClockGate(void)
{
    CCM->CCGR0 = 0xFFFFFFFFU;
    CCM->CCGR1 = 0xFFFFFFFFU;
    CCM->CCGR2 = 0xFFFFFFFFU;
    CCM->CCGR3 = 0xFFFFFFFFU;
    CCM->CCGR4 = 0xFFFFFFFFU;
    CCM->CCGR5 = 0xFFFFFFFFU;
    CCM->CCGR6 = 0xFFFFFFFFU;
}

void LPM_SwitchToRcOSC(void)
{
    /* Switch to RC-OSC */
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_SET_OSC_SEL_MASK;
    /* Turn off XTAL-OSC */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK; /* Power down */

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_SwitchToXtalOSC(void)
{
    /* Restore XTAL-OSC and enable detector */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK; /* Power up */
    while ((XTALOSC24M->LOWPWR_CTRL & XTALOSC24M_LOWPWR_CTRL_XTALOSC_PWRUP_STAT_MASK) == 0)
    {
    }
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK; /* detect freq */
    while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_OSC_XTALOK_MASK) == 0)
    {
    }
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;

    /* Switch to XTAL-OSC */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_CLR_OSC_SEL_MASK;
    /* Turn off XTAL-OSC detector */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_SwitchBandgap(void)
{
    /* Switch bandgap */
    PMU->MISC0_SET = 0x00000004;
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_SET = CCM_ANALOG_MISC0_REFTOP_PWD_MASK;

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_RestoreBandgap(void)
{
    /* Restore bandgap */
    /* Turn on regular bandgap and wait for stable */
    PMU->MISC0_CLR = CCM_ANALOG_MISC0_REFTOP_PWD_MASK;
    while ((PMU->MISC0 & PMU_MISC0_REFTOP_VBGUP_MASK) == 0)
    {
    }
    /* Low power band gap disable */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_CLR = 0x00000004;

    /* Wait CCM operation finishes */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_EnableWakeupSource(uint32_t irq)
{
    GPC_EnableIRQ(GPC, irq);
}

void LPM_DisableWakeupSource(uint32_t irq)
{
    GPC_DisableIRQ(GPC, irq);
}

/*
 * ERR007265: CCM: When improper low-power sequence is used,
 * the SoC enters low power mode before the ARM core executes WFI.
 *
 * Software workaround:
 * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
 *    by setting IOMUX_GPR1_GINT.
 * 2) Software should then unmask IRQ #41 in GPC before setting CCM
 *    Low-Power mode.
 * 3) Software should mask IRQ #41 right after CCM Low-Power mode
 *    is set (set bits 0-1 of CCM_CLPCR).
 */
static void LPM_SetClockMode(clock_mode_t mode, uint32_t clpcr)
{
    switch (mode)
    {
        case kCLOCK_ModeRun:
            CCM->CLPCR = clpcr;
            break;
        default:
            LPM_EnableWakeupSource(GPR_IRQ_IRQn);
            CCM->CLPCR = clpcr;
            LPM_DisableWakeupSource(GPR_IRQ_IRQn);
            break;
    }
}

void LPM_SwitchFlexspiClock(clock_mode_t powermode)
{
    while (!((FLEXSPI->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }
    FLEXSPI->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
    /* Disable clock gate of flexspi. */
    CCM->CCGR6 &= (~CCM_CCGR6_CG5_MASK);

    /* Periph_clk output will be used as SEMC clock root */
    CLOCK_SET_MUX(kCLOCK_SemcMux, 0x0);
    /* Set post divider for SEMC clock as 0. */
    CLOCK_SET_DIV(kCLOCK_SemcDiv, 0x0);

    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    /* Semc_clk_root_pre will be used as flexspi clock. */
    CLOCK_SET_MUX(kCLOCK_FlexspiMux, 0x0);
    /* Set divider for flexspi clock root 0. */
    CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 0x0);

    /* Enable clock gate of flexspi. */
    CCM->CCGR6 |= (CCM_CCGR6_CG5_MASK);

    if (kCLOCK_ModeStop == powermode) {
        FLEXSPI->DLLCR[0] = FLEXSPI_DLLCR_OVRDEN(1) | FLEXSPI_DLLCR_OVRDVAL(19);
    } else {
        FLEXSPI->DLLCR[0] = 0x79;
    }
    FLEXSPI->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    FLEXSPI->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;

    while (FLEXSPI->MCR0 & FLEXSPI_MCR0_SWRESET_MASK) {
    }

    while (!((FLEXSPI->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI->STS0 & FLEXSPI_STS0_SEQIDLE_MASK))) {
    }

    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_RestoreFlexspiClock(void)
{
    while (!((FLEXSPI->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }
    FLEXSPI->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
    /* Disable clock gate of flexspi. */
    CCM->CCGR6 &= (~CCM_CCGR6_CG5_MASK);

    /* PLL3 PFD0 will be used as flexspi clock. */
    CLOCK_SET_MUX(kCLOCK_FlexspiMux, 0x3);
    /* Set divider for flexspi clock root 0. */
    CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 0x0);

    /* Enable clock gate of flexspi. */
    CCM->CCGR6 |= (CCM_CCGR6_CG5_MASK);

    /* PLL2 PFD2 output will be used as SEMC clock root */
    CLOCK_SET_MUX(kCLOCK_SemcMux, 0x1);
    /* Set post divider for SEMC clock as 1. */
    CLOCK_SET_DIV(kCLOCK_SemcDiv, 0x1);

    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    FLEXSPI->DLLCR[0] = s_DllBackupValue;
    FLEXSPI->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    FLEXSPI->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (FLEXSPI->MCR0 & FLEXSPI_MCR0_SWRESET_MASK)
    {
    }
    while (!((FLEXSPI->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) && (FLEXSPI->STS0 & FLEXSPI_STS0_SEQIDLE_MASK)))
    {
        ;
    }
    /* Take some delay */
    LPM_DELAY(40);
}

void LPM_DisablePLLs(clock_mode_t powermode)
{
    s_clockContext.pfd480 = CCM_ANALOG->PFD_480;
    s_clockContext.pfd528 = CCM_ANALOG->PFD_528;
    s_clockContext.pllSys = CCM_ANALOG->PLL_SYS;
    s_clockContext.pllUsb1 = CCM_ANALOG->PLL_USB1;
    s_clockContext.pllUsb2 = CCM_ANALOG->PLL_USB2;
    s_clockContext.pllAudio = CCM_ANALOG->PLL_AUDIO;
    s_clockContext.pllVideo = CCM_ANALOG->PLL_VIDEO;
    s_clockContext.pllEnet = CCM_ANALOG->PLL_ENET;
    s_clockContext.pllArm_loopdiv =
        (CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT;
    s_clockContext.pllArm = CCM_ANALOG->PLL_ARM;
    s_clockContext.periphSel = CLOCK_GetMux(kCLOCK_PeriphMux);
    s_clockContext.ipgDiv = CLOCK_GetDiv(kCLOCK_IpgDiv);
    s_clockContext.ahbDiv = CLOCK_GetDiv(kCLOCK_AhbDiv);
    s_clockContext.perSel = CLOCK_GetMux(kCLOCK_PerclkMux);
    s_clockContext.perDiv = CLOCK_GetDiv(kCLOCK_PerclkDiv);
    s_clockContext.preperiphSel = CLOCK_GetMux(kCLOCK_PrePeriphMux);
    s_clockContext.armDiv = CLOCK_GetDiv(kCLOCK_ArmDiv);
    s_clockContext.is_valid = 1;

    /* Power off USB2 PLL */
    CCM_ANALOG->PLL_USB2_SET = CCM_ANALOG_PLL_USB2_BYPASS_MASK;
    CCM_ANALOG->PLL_USB2_CLR = CCM_ANALOG_PLL_USB2_POWER_MASK;
    /* Power off AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    /* Power off VIDEO PLL */
    CCM_ANALOG->PLL_VIDEO_SET = CCM_ANALOG_PLL_VIDEO_BYPASS_MASK;
    CCM_ANALOG->PLL_VIDEO_SET = CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK;
    /* Power off ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;

    if (kCLOCK_ModeWait == powermode) {
        CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
        CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);
        CLOCK_SetDiv(kCLOCK_IpgDiv, 0);
        CLOCK_SetDiv(kCLOCK_AhbDiv, 0);

        /*ARM PLL as clksource*/
        /* 24 * 88 / 2 / 8 = 132MHz */
        CCM_ANALOG->PLL_ARM_CLR = CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;
        CCM_ANALOG->PLL_ARM_SET = CCM_ANALOG_PLL_ARM_ENABLE_MASK | CCM_ANALOG_PLL_ARM_BYPASS_MASK;
        CCM_ANALOG->PLL_ARM_CLR = CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK;
        CCM_ANALOG->PLL_ARM_SET = CCM_ANALOG_PLL_ARM_DIV_SELECT(88);
        CLOCK_SetDiv(kCLOCK_ArmDiv, 7);
        CCM_ANALOG->PLL_ARM_CLR = CCM_ANALOG_PLL_ARM_BYPASS_MASK;

        /*Select ARM_PLL for pre_periph_clock */
        CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0);

        /* SET AHB, IPG to 33MHz */
        CLOCK_SetDiv(kCLOCK_IpgDiv, 0);
        CLOCK_SetDiv(kCLOCK_AhbDiv, 3);

        /*Set PERCLK to 33MHz*/
        //CLOCK_SetMux(kCLOCK_PerclkMux, 0);
        //CLOCK_SetDiv(kCLOCK_PerclkDiv, 0);
    } else {
        CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
        CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
        CLOCK_SetMux(kCLOCK_PeriphMux, 1);
        CLOCK_SetDiv(kCLOCK_IpgDiv, 0);
        CLOCK_SetDiv(kCLOCK_AhbDiv, 0);

        /*ARM PLL as clksource*/
        CCM_ANALOG->PLL_ARM |= CCM_ANALOG_PLL_ARM_ENABLE_MASK | CCM_ANALOG_PLL_ARM_BYPASS_MASK;
        /* Power off ARM PLL */
        CCM_ANALOG->PLL_ARM_SET = CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;

        /*Select ARM_PLL for pre_periph_clock */
        CLOCK_SetDiv(kCLOCK_ArmDiv, 0x0);
        CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x3);
        CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);

        /* SET AHB, IPG to 12MHz */
        CLOCK_SetDiv(kCLOCK_IpgDiv, 0);
        CLOCK_SetDiv(kCLOCK_AhbDiv, 1);

        /*Set PERCLK to 12Mhz*/
        //CLOCK_SetMux(kCLOCK_PerclkMux, 0x0);
        //CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x0);
    }

    core_util_critical_section_enter();
    LPM_SwitchFlexspiClock(powermode);
    core_util_critical_section_exit();

    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd0);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd1);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd2);
    CLOCK_DeinitUsb1Pfd(kCLOCK_Pfd3);

    /* Power off USB1 PLL */
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1_CLR = CCM_ANALOG_PLL_USB1_POWER_MASK;

    CLOCK_DeinitSysPfd(kCLOCK_Pfd0);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd1);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd2);
    CLOCK_DeinitSysPfd(kCLOCK_Pfd3);

    /* When need to close PLL, we need to use bypass clock first and then power it down. */
    /* Power off SYS PLL */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
}

void LPM_RestorePLLs(void)
{
    if (s_clockContext.is_valid)
    {
        /* Bypass PLL first */
        CCM_ANALOG->PLL_USB1 = (CCM_ANALOG->PLL_USB1 & (~CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_MASK)) |
                               CCM_ANALOG_PLL_USB1_BYPASS_MASK;

        CCM_ANALOG->PLL_USB1 = (CCM_ANALOG->PLL_USB1 & (~CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK)) |
                               CCM_ANALOG_PLL_USB1_ENABLE_MASK | CCM_ANALOG_PLL_USB1_POWER_MASK |
                               CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;

        while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
        {
        }

        /* Disable Bypass */
        CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;

        /* Restore USB1 PLL PFD */
        CCM_ANALOG->PFD_480 = s_clockContext.pfd480;
        CCM_ANALOG->PFD_480_CLR = CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK;

        /* Disable Usb1 PLL output for USBPHY1. */
        CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;

        /* When need to enable PLL, we need to use bypass clock first and then switch pll back. */
        /* Power on SYS PLL and wait for locked */
        CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_BYPASS_MASK;
        CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
        CCM_ANALOG->PLL_SYS = s_clockContext.pllSys;

        while ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK) == 0)
        {
        }

        /* Restore SYS PLL PFD */
        CCM_ANALOG->PFD_528 = s_clockContext.pfd528;
        CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD2_CLKGATE_MASK;

        core_util_critical_section_enter();
        LPM_RestoreFlexspiClock();
        core_util_critical_section_exit();
    } else {
        return;
    }

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0);
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);

    /* ARM PLL as clksource*/
    CCM_ANALOG->PLL_ARM_CLR = CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;
    CCM_ANALOG->PLL_ARM_SET = CCM_ANALOG_PLL_ARM_ENABLE_MASK | CCM_ANALOG_PLL_ARM_BYPASS_MASK;

    CLOCK_SetDiv(kCLOCK_ArmDiv, s_clockContext.armDiv);
    CCM_ANALOG->PLL_ARM_CLR = CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK;
    CCM_ANALOG->PLL_ARM_SET = CCM_ANALOG_PLL_ARM_DIV_SELECT(s_clockContext.pllArm_loopdiv);
    if ((s_clockContext.pllArm & CCM_ANALOG_PLL_ARM_BYPASS_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK) == 0)
        {
        }
    }
    if ((s_clockContext.pllArm & CCM_ANALOG_PLL_ARM_BYPASS_MASK) == 0)
    {
        CCM_ANALOG->PLL_ARM_CLR = CCM_ANALOG_PLL_ARM_BYPASS_MASK;
    }

    /* Restore AHB and IPG div */
    CCM->CBCDR = (CCM->CBCDR & ~(CCM_CBCDR_AHB_PODF_MASK | CCM_CBCDR_IPG_PODF_MASK | CCM_CBCDR_PERIPH_CLK_SEL_MASK)) |
                 CCM_CBCDR_AHB_PODF(s_clockContext.ahbDiv) | CCM_CBCDR_IPG_PODF(s_clockContext.ipgDiv) |
                 CCM_CBCDR_PERIPH_CLK_SEL(s_clockContext.periphSel);

    /* Restore Periphral clock */
    CCM->CSCMR1 = (CCM->CSCMR1 & ~CCM_CSCMR1_PERCLK_PODF_MASK) | CCM_CSCMR1_PERCLK_PODF(s_clockContext.perDiv) |
                  CCM_CSCMR1_PERCLK_CLK_SEL(s_clockContext.perSel);

    /* Switch clocks back */
    CCM->CBCMR =
        (CCM->CBCMR & ~CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK) | CCM_CBCMR_PRE_PERIPH_CLK_SEL(s_clockContext.preperiphSel);

    /* Wait CCM operation finishes */
    while (CCM->CDHIPR != 0)
    {
    }

    /* Restore USB2 PLL */
    CCM_ANALOG->PLL_USB2_SET = CCM_ANALOG_PLL_USB2_BYPASS_MASK;
    CCM_ANALOG->PLL_USB2_SET = CCM_ANALOG_PLL_USB2_POWER_MASK;
    CCM_ANALOG->PLL_USB2 = s_clockContext.pllUsb2;
    if ((CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_POWER_MASK) != 0)
    {
        while ((CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore AUDIO PLL */
    CCM_ANALOG->PLL_AUDIO_SET = CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
    CCM_ANALOG->PLL_AUDIO_CLR = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
    CCM_ANALOG->PLL_AUDIO = s_clockContext.pllAudio;
    if ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore VIDEO PLL */
    CCM_ANALOG->PLL_VIDEO_SET = CCM_ANALOG_PLL_VIDEO_BYPASS_MASK;
    CCM_ANALOG->PLL_VIDEO_CLR = CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK;
    CCM_ANALOG->PLL_VIDEO = s_clockContext.pllVideo;
    if ((CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_LOCK_MASK) == 0)
        {
        }
    }

    /* Restore ENET PLL */
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;
    CCM_ANALOG->PLL_ENET = s_clockContext.pllEnet;
    if ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_POWERDOWN_MASK) == 0)
    {
        while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0)
        {
        }
    }

    s_clockContext.is_valid = 0;
}

static void LPM_SystemIdle(clock_mode_t powermode)
{
    /* Switch DCDC to use DCDC internal OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);

    /* Power down USBPHY */
    USBPHY1->CTRL = 0xFFFFFFFF;
    USBPHY2->CTRL = 0xFFFFFFFF;

    LPM_DisablePLLs(powermode);

    /* Enable weak 2P5 and turn off regular 2P5 */
    PMU->REG_2P5 &= ~PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_2P5 |= PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;

    /* Enable weak 1P1 and turn off regular 1P1 */
    PMU->REG_1P1 &= ~PMU_REG_1P1_ENABLE_LINREG_MASK;
    PMU->REG_1P1 |= PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    core_util_critical_section_enter();
    LPM_SwitchToRcOSC();
    core_util_critical_section_exit();

    /* Lower OSC current by 37.5% */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_OSC_I_MASK;
    /* Enable FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* Disconnect vdd_high_in and connect vdd_snvs_in */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;

    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);

    core_util_critical_section_enter();
    LPM_SwitchBandgap();
    core_util_critical_section_exit();

    /* RBC = 0; Enable COSC, OSC COUNT = 0xAF */
    CCM->CCR = (CCM_CCR_COSC_EN_MASK | CCM_CCR_OSCNT(0xAF));
}

void LPM_SystemRestoreIdle(void)
{
    DCDC_AdjustTargetVoltage(DCDC, 0x12, 0x1);

    /* Switch DCDC to use DCDC internal OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockExternalOsc);

    /* Disconnect vdd_snvs_in and connect vdd_high_in */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_DISCON_HIGH_SNVS_MASK;
    /* Increase OSC current to normal */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_I_MASK;

    core_util_critical_section_enter();
    LPM_SwitchToXtalOSC();
    LPM_RestoreBandgap();
    core_util_critical_section_exit();

    /* Disable FET ODRIVE */
    PMU->REG_CORE_CLR = PMU_REG_CORE_FET_ODRIVE_MASK;

    /* Enable regular 2P5 and wait for stable */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_LINREG_MASK;
    while ((PMU->REG_2P5 & PMU_REG_2P5_OK_VDD2P5_MASK) == 0)
    {
    }
    /* Turn off weak 2P5 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;

    /* Enable regular 1P1 and wait for stable */
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_LINREG_MASK;
    while ((PMU->REG_1P1 & PMU_REG_1P1_OK_VDD1P1_MASK) == 0)
    {
    }
    /* Turn off weak 1P1 */
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    LPM_RestorePLLs();
}

bool LPM_Init(void)
{
    uint32_t i;
    uint32_t tmp_reg = 0;

    CLOCK_SetMode(kCLOCK_ModeRun);

    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* Enable RC OSC. It needs at least 4ms to be stable, so self tuning need to be enabled. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* Configure RC OSC */
    XTALOSC24M->OSC_CONFIG0 = XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG_CUR(0x4) | XTALOSC24M_OSC_CONFIG0_SET_HYST_MINUS(0x2) |
                              XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG(0xA7) | XTALOSC24M_OSC_CONFIG0_START_MASK |
                              XTALOSC24M_OSC_CONFIG0_ENABLE_MASK;
    XTALOSC24M->OSC_CONFIG1 = XTALOSC24M_OSC_CONFIG1_COUNT_RC_CUR(0x40) | XTALOSC24M_OSC_CONFIG1_COUNT_RC_TRG(0x2DC);
    /* Take some delay */
    LPM_DELAY(40);
    /* Add some hysteresis */
    tmp_reg = XTALOSC24M->OSC_CONFIG0;
    tmp_reg &= ~(XTALOSC24M_OSC_CONFIG0_HYST_PLUS_MASK | XTALOSC24M_OSC_CONFIG0_HYST_MINUS_MASK);
    tmp_reg |= XTALOSC24M_OSC_CONFIG0_HYST_PLUS(3) | XTALOSC24M_OSC_CONFIG0_HYST_MINUS(3);
    XTALOSC24M->OSC_CONFIG0 = tmp_reg;
    /* Set COUNT_1M_TRG */
    tmp_reg = XTALOSC24M->OSC_CONFIG2;
    tmp_reg &= ~XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG_MASK;
    tmp_reg |= XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG(0x2d7);
    XTALOSC24M->OSC_CONFIG2 = tmp_reg;
    /* Hardware requires to read OSC_CONFIG0 or OSC_CONFIG1 to make OSC_CONFIG2 write work */
    tmp_reg = XTALOSC24M->OSC_CONFIG1;
    XTALOSC24M->OSC_CONFIG1 = tmp_reg;

    s_DllBackupValue = FLEXSPI->DLLCR[0];

    /* ERR007265 */
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_GINT_MASK;

    /* Initialize GPC to mask all IRQs */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++) {
        GPC->IMR[i] = 0xFFFFFFFFU;
    }

    return true;
}

void LPM_Deinit(void)
{
    /* ERR007265 */
    IOMUXC_GPR->GPR1 &= ~IOMUXC_GPR_GPR1_GINT_MASK;
}

void vPortPRE_SLEEP_PROCESSING(clock_mode_t powermode)
{
    uint32_t clpcr;

    clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));

    switch (powermode)
    {
        case kCLOCK_ModeWait:
#if 0
            LPM_EnableWakeupSource(PIT_IRQn);

            LPM_SetClockMode(kCLOCK_ModeWait, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) |
                                              CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | 0x1C |
                                              0x08280000);
            IOMUXC_GPR->GPR8 = 0xaaaaaaaa;
            IOMUXC_GPR->GPR12 = 0x0000000a;
#endif
            break;
        case kCLOCK_ModeStop:
            LPM_EnableWakeupSource(GPT2_IRQn);

            LPM_SetClockMode(kCLOCK_ModeWait, clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) |
                                              CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | 0x1C |
                                              0x08280000);
            BOARD_SetLPClockGate();
            LPM_SystemIdle(powermode);
            IOMUXC_GPR->GPR8 = 0xaaaaaaaa;
            IOMUXC_GPR->GPR12 = 0x0000000a;
            break;
        default:
            assert(false);
            break;
    }
}

void vPortPOST_SLEEP_PROCESSING(clock_mode_t powermode)
{
    uint32_t clpcr;

    clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));

    switch (powermode)
    {
        case kCLOCK_ModeWait:
#if 0

            IOMUXC_GPR->GPR8 = 0x00000000;
            IOMUXC_GPR->GPR12 = 0x00000000;
            LPM_SetClockMode(kCLOCK_ModeRun, clpcr);

            LPM_DisableWakeupSource(PIT_IRQn);
#endif

            break;
        case kCLOCK_ModeStop:
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            IOMUXC_GPR->GPR8 = 0x00000000;
            IOMUXC_GPR->GPR12 = 0x00000000;
            /* Interrupt occurs before system idle */
            LPM_SystemRestoreIdle();
            BOARD_ResetLPClockGate();

            LPM_SetClockMode(kCLOCK_ModeRun, clpcr);

            LPM_DisableWakeupSource(GPT2_IRQn);

            break;
        default:
            assert(false);
            break;
    }
}

