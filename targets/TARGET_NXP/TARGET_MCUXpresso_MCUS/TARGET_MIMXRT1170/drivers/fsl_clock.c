/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
#include "fsl_anatop.h"
#include "fsl_anatop_ai.h"
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.clock"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* To make full use of CM7 hardware FPU, use double instead of uint64_t in clock driver to
achieve better performance, it is depend on the IDE Floating point settings, if double precision is selected
in IDE, clock_64b_t will switch to double type automatically. only support IAR and MDK here */
#if __FPU_USED

#if ((defined(__ICCARM__)) || (defined(__GNUC__)))

#if (__ARMVFP__ >= __ARMFPV5__) && \
    (__ARM_FP == 0xE) /*0xe implies support for half, single and double precision operations*/
typedef double clock_64b_t;
#else
typedef uint64_t clock_64b_t;
#endif

#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)

#if defined __TARGET_FPU_FPV5_D16
typedef double clock_64b_t;
#else
typedef uint64_t clock_64b_t;
#endif

#else
typedef uint64_t clock_64b_t;
#endif

#else
typedef uint64_t clock_64b_t;
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#define ARM_PLL_DIV_SEL_MIN 104
#define ARM_PLL_DIV_SEL_MAX 208

void CLOCK_InitArmPll(const clock_arm_pll_config_t *config)
{
    assert((config->loopDivider <= ARM_PLL_DIV_SEL_MAX) && (config->loopDivider >= ARM_PLL_DIV_SEL_MIN));

    uint32_t reg;

    if ((ANADIG_PLL->PLL_ARM_CTRL & (ANADIG_PLL_PLL_ARM_CTRL_POWERUP_MASK)) &&
        ((ANADIG_PLL->PLL_ARM_CTRL & (ANADIG_PLL_PLL_ARM_CTRL_DIV_SELECT(config->loopDivider))) ==
         (ANADIG_PLL_PLL_ARM_CTRL_DIV_SELECT(config->loopDivider))) &&
        ((ANADIG_PLL->PLL_ARM_CTRL & (ANADIG_PLL_PLL_ARM_CTRL_POST_DIV_SEL(config->postDivider))) ==
         (ANADIG_PLL_PLL_ARM_CTRL_POST_DIV_SEL(config->postDivider))))
    {
        /* no need to reconfigure the PLL if all the configuration is the same */
        if (!(ANADIG_PLL->PLL_ARM_CTRL & ANADIG_PLL_PLL_ARM_CTRL_ENABLE_CLK_MASK))
        {
            ANADIG_PLL->PLL_ARM_CTRL |= ANADIG_PLL_PLL_ARM_CTRL_ENABLE_CLK_MASK;
        }
        if ((ANADIG_PLL->PLL_ARM_CTRL & ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_GATE_MASK))
        {
            ANADIG_PLL->PLL_ARM_CTRL &= ~ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_GATE_MASK;
        }
        return;
    }

    ANATOP_EnablePllLdo();

    reg = ANADIG_PLL->PLL_ARM_CTRL & (~ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_STABLE_MASK);
    if (reg & (ANADIG_PLL_PLL_ARM_CTRL_POWERUP_MASK | ANADIG_PLL_PLL_ARM_CTRL_ENABLE_CLK_MASK))
    {
        /* Power down the PLL. */
        reg &= ~(ANADIG_PLL_PLL_ARM_CTRL_POWERUP_MASK | ANADIG_PLL_PLL_ARM_CTRL_ENABLE_CLK_MASK);
        reg |= ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_GATE_MASK;
        ANADIG_PLL->PLL_ARM_CTRL = reg;
    }

    /* Set the configuration. */
    reg &= ~(ANADIG_PLL_PLL_ARM_CTRL_DIV_SELECT_MASK | ANADIG_PLL_PLL_ARM_CTRL_POST_DIV_SEL_MASK);
    reg |= (ANADIG_PLL_PLL_ARM_CTRL_DIV_SELECT(config->loopDivider) |
            ANADIG_PLL_PLL_ARM_CTRL_POST_DIV_SEL(config->postDivider)) |
           ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_GATE_MASK | ANADIG_PLL_PLL_ARM_CTRL_POWERUP_MASK;
    ANADIG_PLL->PLL_ARM_CTRL = reg;
    __DSB();
    __ISB();
    SDK_DelayAtLeastUs(30, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Wait for the PLL stable, */
    while (0U == (ANADIG_PLL->PLL_ARM_CTRL & ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_STABLE_MASK))
    {
    }

    /* Enable and ungate the clock. */
    reg |= ANADIG_PLL_PLL_ARM_CTRL_ENABLE_CLK_MASK;
    reg &= ~ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_GATE_MASK;
    ANADIG_PLL->PLL_ARM_CTRL = reg;
}

void CLOCK_DeinitArmPll(void)
{
    uint32_t reg = ANADIG_PLL->PLL_ARM_CTRL & (~ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_STABLE_MASK);

    reg &= ~(ANADIG_PLL_PLL_ARM_CTRL_POWERUP_MASK | ANADIG_PLL_PLL_ARM_CTRL_ENABLE_CLK_MASK);
    reg |= ANADIG_PLL_PLL_ARM_CTRL_PLL_ARM_GATE_MASK;
    ANADIG_PLL->PLL_ARM_CTRL = reg;
}

#define SYS_PLL2_DIV_SEL_MIN 54
#define SYS_PLL2_DIV_SEL_MAX 108
/* 528PLL */
void CLOCK_InitSysPll2(const clock_sys_pll_config_t *config)
{
    uint32_t reg;

    if ((ANADIG_PLL->PLL_528_CTRL & ANADIG_PLL_PLL_528_CTRL_POWERUP_MASK) &&
        ((ANADIG_PLL->PLL_528_MFN == config->mfn) && (ANADIG_PLL->PLL_528_MFI == config->mfi)))
    {
        /* no need to reconfigure the PLL if all the configuration is the same */
        if (!(ANADIG_PLL->PLL_528_CTRL & ANADIG_PLL_PLL_528_CTRL_ENABLE_CLK_MASK))
        {
            ANADIG_PLL->PLL_528_CTRL |= ANADIG_PLL_PLL_528_CTRL_ENABLE_CLK_MASK;
        }

        if ((ANADIG_PLL->PLL_528_CTRL & ANADIG_PLL_PLL_528_CTRL_PLL_528_GATE_MASK))
        {
            ANADIG_PLL->PLL_528_CTRL &= ~ANADIG_PLL_PLL_528_CTRL_PLL_528_GATE_MASK;
        }
        return;
    }

    ANATOP_EnablePllLdo();
    /* Gate all PFDs */
    ANADIG_PLL->PLL_528_PFD |=
        ANADIG_PLL_PLL_528_PFD_PFD0_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_528_PFD_PFD1_DIV1_CLKGATE(1) |
        ANADIG_PLL_PLL_528_PFD_PFD2_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_528_PFD_PFD3_DIV1_CLKGATE(1);

    reg = ANADIG_PLL->PLL_528_CTRL;
    if (reg & (ANADIG_PLL_PLL_528_CTRL_POWERUP_MASK | ANADIG_PLL_PLL_528_CTRL_ENABLE_CLK_MASK))
    {
        /* Power down the PLL. */
        reg &= ~(ANADIG_PLL_PLL_528_CTRL_POWERUP_MASK | ANADIG_PLL_PLL_528_CTRL_ENABLE_CLK_MASK);
        reg |= ANADIG_PLL_PLL_528_CTRL_PLL_528_GATE_MASK;
        ANADIG_PLL->PLL_528_CTRL = reg;
    }

    /* Config PLL */
    ANADIG_PLL->PLL_528_MFD = 0x0FFFFFFFUL;
    ANADIG_PLL->PLL_528_MFN = config->mfn;
    ANADIG_PLL->PLL_528_MFI = config->mfi;

    /* REG_EN = 1, GATE = 1, DIV_SEL = 0, POWERUP = 0 */
    reg = ANADIG_PLL_PLL_528_CTRL_PLL_REG_EN(1) | ANADIG_PLL_PLL_528_CTRL_PLL_528_GATE(1);
    ANADIG_PLL->PLL_528_CTRL = reg;
    /* Wait until LDO is stable */
    SDK_DelayAtLeastUs(30, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* REG_EN = 1, GATE = 1, DIV_SEL = 0, POWERUP = 1, HOLDRING_OFF = 1 */
    reg |= ANADIG_PLL_PLL_528_CTRL_POWERUP(1) | ANADIG_PLL_PLL_528_CTRL_HOLD_RING_OFF_MASK;
    ANADIG_PLL->PLL_528_CTRL = reg;
    SDK_DelayAtLeastUs(250, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* REG_EN = 1, GATE = 1, DIV_SEL = 0, POWERUP = 1, HOLDRING_OFF = 0 */
    reg &= ~ANADIG_PLL_PLL_528_CTRL_HOLD_RING_OFF_MASK;
    ANADIG_PLL->PLL_528_CTRL = reg;
    /* Wait for PLL stable */
    while (ANADIG_PLL_PLL_528_CTRL_PLL_528_STABLE_MASK !=
           (ANADIG_PLL->PLL_528_CTRL & ANADIG_PLL_PLL_528_CTRL_PLL_528_STABLE_MASK))
    {
    }

    /* REG_EN = 1, GATE = 1, DIV_SEL = 0, POWERUP = 1, HOLDRING_OFF = 0, CLK = 1*/
    reg |= ANADIG_PLL_PLL_528_CTRL_ENABLE_CLK_MASK;
    ANADIG_PLL->PLL_528_CTRL = reg;

    /* REG_EN = 1, GATE = 0, DIV_SEL = 0, POWERUP = 1, HOLDRING_OFF = 0, CLK = 1*/
    reg &= ~ANADIG_PLL_PLL_528_CTRL_PLL_528_GATE_MASK;
    ANADIG_PLL->PLL_528_CTRL = reg;
    ANADIG_PLL->PLL_528_PFD &=
        ~(ANADIG_PLL_PLL_528_PFD_PFD0_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_528_PFD_PFD1_DIV1_CLKGATE(1) |
          ANADIG_PLL_PLL_528_PFD_PFD2_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_528_PFD_PFD3_DIV1_CLKGATE(1));
}

void CLOCK_DeinitSysPll2(void)
{
    ANADIG_PLL->PLL_528_PFD |=
        ANADIG_PLL_PLL_528_PFD_PFD0_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_528_PFD_PFD1_DIV1_CLKGATE(1) |
        ANADIG_PLL_PLL_528_PFD_PFD2_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_528_PFD_PFD3_DIV1_CLKGATE(1);

    ANADIG_PLL->PLL_528_CTRL |= ANADIG_PLL_PLL_528_CTRL_PLL_528_GATE_MASK;
    ANADIG_PLL->PLL_528_CTRL &= ~(ANADIG_PLL_PLL_528_CTRL_ENABLE_CLK_MASK | ANADIG_PLL_PLL_528_CTRL_POWERUP_MASK |
                                  ANADIG_PLL_PLL_528_CTRL_PLL_REG_EN_MASK);
}

#define PFD_FRAC_MIN 12
#define PFD_FRAC_MAX 35
void CLOCK_InitPfd(clock_pll_t pll, clock_pfd_t pfd, uint8_t frac)
{
    volatile uint32_t *pfdCtrl = 0, *pfdUpdate = 0, stable;

    assert(frac <= PFD_FRAC_MAX && frac >= PFD_FRAC_MIN);

    switch (pll)
    {
        case kCLOCK_PllSys2:
            pfdCtrl = &ANADIG_PLL->PLL_528_PFD;
            pfdUpdate = &ANADIG_PLL->PLL_528_UPDATE;
            break;
        case kCLOCK_PllSys3:
            pfdCtrl = &ANADIG_PLL->PLL_480_PFD;
            pfdUpdate = &ANADIG_PLL->PLL_480_UPDATE;
            break;
        default:
            assert(false);
            break;
    }
    stable = *pfdCtrl & (ANADIG_PLL_PLL_528_PFD_PFD0_STABLE_MASK << (8 * (uint32_t)pfd));
    *pfdCtrl |= (ANADIG_PLL_PLL_528_PFD_PFD0_DIV1_CLKGATE_MASK << (8 * (uint32_t)pfd));

    /* all pfds support to be updated on-the-fly after corresponding PLL is stable */
    *pfdCtrl &= ~(ANADIG_PLL_PLL_528_PFD_PFD0_FRAC_MASK << (8 * (uint32_t)pfd));
    *pfdCtrl |= (ANADIG_PLL_PLL_528_PFD_PFD0_FRAC(frac) << (8 * (uint32_t)pfd));

    *pfdUpdate ^= (ANADIG_PLL_PLL_528_UPDATE_PFD0_UPDATE_MASK << (uint32_t)pfd);
    *pfdCtrl &= ~(ANADIG_PLL_PLL_528_PFD_PFD0_DIV1_CLKGATE_MASK << (8 * (uint32_t)pfd));
    /* Wait for stablizing */
    while (stable == (*pfdCtrl & (ANADIG_PLL_PLL_528_PFD_PFD0_STABLE_MASK << (8 * (uint32_t)pfd))))
    {
    }
}

#ifndef GET_FREQ_FROM_OBS
uint32_t CLOCK_GetPfdFreq(clock_pll_t pll, clock_pfd_t pfd)
{
    uint32_t pllFreq = 0, frac = 0;
    assert((pll == kCLOCK_PllSys2) ||  (pll == kCLOCK_PllSys3));

    switch (pll)
    {
        case kCLOCK_PllSys2:
            frac = (ANADIG_PLL->PLL_528_PFD & (ANADIG_PLL_PLL_528_PFD_PFD0_FRAC_MASK << (8 * (uint32_t)pfd)));
            pllFreq = SYS_PLL2_FREQ;
            break;
        case kCLOCK_PllSys3:
            frac = (ANADIG_PLL->PLL_480_PFD & (ANADIG_PLL_PLL_480_PFD_PFD0_FRAC_MASK << (8 * (uint32_t)pfd)));
            pllFreq = SYS_PLL3_FREQ;
            break;
        default:
            assert(false);
            break;
    }

    frac = frac >> (8 * (uint32_t)pfd);
    assert((frac >= PFD_FRAC_MIN) && (frac <= PFD_FRAC_MAX));
    return (frac ? pllFreq / frac * 18 : 0);
}
#else
uint32_t CLOCK_GetPfdFreq(clock_pll_t pll, clock_pfd_t pfd)
{
    uint32_t freq = 0;
    assert((pll == kCLOCK_PllSys2) ||  (pll == kCLOCK_PllSys3));

    switch (pll)
    {
        case kCLOCK_PllSys2:
            /* PLL_528_PFD0 OBS index starts from 234 */
            freq = CLOCK_GetFreqFromObs(pfd + 234, 2);
            break;
        case kCLOCK_PllSys3:
            /* PLL_480_PFD0 OBS index starts from 241 */
            freq = CLOCK_GetFreqFromObs(pfd + 241, 2);
            break;
        default:
            assert(false);
    }
    return freq;
}
#endif

#define SYS_PLL3_DIV_SEL_MIN 54
#define SYS_PLL3_DIV_SEL_MAX 108
/* 480PLL */
void CLOCK_InitSysPll3(const clock_sys_pll3_config_t *config)
{
    uint32_t reg;

    if ((ANADIG_PLL->PLL_480_CTRL & ANADIG_PLL_PLL_480_CTRL_POWERUP_MASK) &&
        ((ANADIG_PLL->PLL_480_CTRL & ANADIG_PLL_PLL_480_CTRL_DIV_SELECT(config->divSelect)) ==
         ANADIG_PLL_PLL_480_CTRL_DIV_SELECT(config->divSelect)))
    {
        /* no need to reconfigure the PLL if all the configuration is the same */
        if (!(ANADIG_PLL->PLL_480_CTRL & ANADIG_PLL_PLL_480_CTRL_ENABLE_CLK_MASK))
        {
            ANADIG_PLL->PLL_480_CTRL |= ANADIG_PLL_PLL_480_CTRL_ENABLE_CLK_MASK;
        }

        if ((ANADIG_PLL->PLL_480_CTRL & ANADIG_PLL_PLL_480_CTRL_PLL_480_GATE_MASK))
        {
            ANADIG_PLL->PLL_480_CTRL &= ~ANADIG_PLL_PLL_480_CTRL_PLL_480_GATE_MASK;
        }
        return;
    }

    ANATOP_EnablePllLdo();

    /* Gate all PFDs */
    ANADIG_PLL->PLL_480_PFD |=
        ANADIG_PLL_PLL_480_PFD_PFD0_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_480_PFD_PFD1_DIV1_CLKGATE(1) |
        ANADIG_PLL_PLL_480_PFD_PFD2_DIV1_CLKGATE(1) | ANADIG_PLL_PLL_480_PFD_PFD3_DIV1_CLKGATE(1);
    /*
     * 1. configure PLL registres
     * 2. Enable internal LDO
     * 3. Wati LDO stable
     * 4. Power up PLL, assert hold_ring_off (only needed for avpll)
     * 5. At half lock time, de-asserted hold_ring_off (only needed for avpll)
     * 6. Wait PLL lock
     * 7. Enable clock output, release pfd_gate
     */
    reg = ANADIG_PLL_PLL_480_CTRL_DIV_SELECT(config->divSelect) | ANADIG_PLL_PLL_480_CTRL_PLL_REG_EN(1) |
          ANADIG_PLL_PLL_480_CTRL_PLL_480_GATE(1);
    ANADIG_PLL->PLL_480_CTRL = reg;
    SDK_DelayAtLeastUs(30, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    reg |= ANADIG_PLL_PLL_480_CTRL_POWERUP(1) | ANADIG_PLL_PLL_480_CTRL_HOLD_RING_OFF_MASK;
    ANADIG_PLL->PLL_480_CTRL = reg;
    SDK_DelayAtLeastUs(30, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    reg &= ~ANADIG_PLL_PLL_480_CTRL_HOLD_RING_OFF_MASK;
    ANADIG_PLL->PLL_480_CTRL = reg;

    /* Wait for PLL stable */
    while (ANADIG_PLL_PLL_480_CTRL_PLL_480_STABLE_MASK !=
           (ANADIG_PLL->PLL_480_CTRL & ANADIG_PLL_PLL_480_CTRL_PLL_480_STABLE_MASK))
    {
    }

    reg |= ANADIG_PLL_PLL_480_CTRL_ENABLE_CLK(1) | ANADIG_PLL_PLL_480_CTRL_PLL_480_DIV2(1);
    ANADIG_PLL->PLL_480_CTRL = reg;

    reg &= ~ANADIG_PLL_PLL_480_CTRL_PLL_480_GATE_MASK;
    ANADIG_PLL->PLL_480_CTRL = reg;
}

void CLOCK_DeinitSysPll3(void)
{
}

void ANATOP_PllSetPower(anatop_ai_itf_t itf, bool enable)
{
    ANATOP_AI_Write(itf, enable ? 0x04 : 0x08, 0x4000 | (enable ? 0x2000 : 0));
}

void ANATOP_PllBypass(anatop_ai_itf_t itf, bool bypass)
{
    ANATOP_AI_Write(itf, bypass ? 0x04 : 0x08, 0x10000);
}

void ANATOP_PllEnablePllReg(anatop_ai_itf_t itf, bool enable)
{
    ANATOP_AI_Write(itf, enable ? 0x04 : 0x08, 0x400000);
}

void ANATOP_PllHoldRingOff(anatop_ai_itf_t itf, bool off)
{
    ANATOP_AI_Write(itf, off ? 0x04 : 0x08, 0x2000);
}

void ANATOP_PllToggleHoldRingOff(anatop_ai_itf_t itf, uint32_t delay_us)
{
    ANATOP_PllHoldRingOff(itf, true);
    SDK_DelayAtLeastUs(delay_us, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    ANATOP_PllHoldRingOff(itf, false);
}

void ANATOP_PllEnableClk(anatop_ai_itf_t itf, bool enable)
{
    ANATOP_AI_Write(itf, enable ? 0x04 : 0x08, 0x8000);
}

void ANATOP_PllConfigure(anatop_ai_itf_t itf, uint8_t div, uint32_t numer, uint8_t post_div, uint32_t denom)
{
    if (itf != kAI_Itf_1g)
    {
        ANATOP_PllSetPower(itf, false);
    }
    ANATOP_AI_Write(itf, 0x30, denom);
    ANATOP_AI_Write(itf, 0x20, numer);
    ANATOP_AI_WriteWithMaskShift(itf, 0x00, div, 0x7f, 0);
    if (itf != kAI_Itf_1g)
    {
        ANATOP_AI_WriteWithMaskShift(itf, 0x00, post_div, 0xE000000, 25);
    }
    ANATOP_PllEnablePllReg(itf, true);
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    ANATOP_PllSetPower(itf, true);
}

void ANATOP_AudioPllGate(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_AUDIO_CTRL &= ~ANADIG_PLL_PLL_AUDIO_CTRL_PLL_AUDIO_GATE_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_AUDIO_CTRL |= ANADIG_PLL_PLL_AUDIO_CTRL_PLL_AUDIO_GATE_MASK;
    }
}

void ANATOP_AudioPllSwEnClk(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_AUDIO_CTRL &= ~ANADIG_PLL_PLL_AUDIO_CTRL_ENABLE_CLK_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_AUDIO_CTRL |= ANADIG_PLL_PLL_AUDIO_CTRL_ENABLE_CLK_MASK;
    }
}

status_t CLOCK_InitAudioPllWithFreq(uint32_t freqInMhz)
{
    clock_audio_pll_config_t config = {0};

    if (kStatus_Success == CLOCK_CalcAvPllFreq(&config, freqInMhz))
    {
        CLOCK_InitAudioPll(&config);
        return kStatus_Success;
    }
    return kStatus_Fail;
}

void CLOCK_InitAudioPll(const clock_audio_pll_config_t *config)
{
    uint32_t reg;
    bool pllStable = false;

    ANATOP_EnablePllLdo();

    reg = ANADIG_PLL->PLL_AUDIO_CTRL;
    if ((reg & (ANADIG_PLL_PLL_AUDIO_CTRL_PLL_AUDIO_STABLE_MASK | ANADIG_PLL_PLL_AUDIO_CTRL_ENABLE_CLK_MASK)) ==
        (ANADIG_PLL_PLL_AUDIO_CTRL_PLL_AUDIO_STABLE_MASK | ANADIG_PLL_PLL_AUDIO_CTRL_ENABLE_CLK_MASK))
    {
        pllStable = true;
    }
    /* bypass pll */
    ANATOP_PllBypass(kAI_Itf_Audio, true);

    if (pllStable)
    {
        /* sw enable clock */
        ANATOP_AudioPllSwEnClk(false);
        /* gate clock */
        ANATOP_AudioPllGate(true);
        ANATOP_PllEnableClk(kAI_Itf_Audio, false);
        ANATOP_PllSetPower(kAI_Itf_Audio, false);
        ANATOP_PllEnablePllReg(kAI_Itf_Audio, false);
    }

    ANATOP_AudioPllSwEnClk(true);
    /* configure pll */
    ANATOP_PllConfigure(kAI_Itf_Audio, config->loopDivider, config->numerator, config->postDivider,
                        config->denominator);

    /* toggle hold ring off */
    ANATOP_PllToggleHoldRingOff(kAI_Itf_Audio, 225);

    /* wait pll stable */
    do
    {
        reg = ANADIG_PLL->PLL_AUDIO_CTRL;
    } while ((reg & ANADIG_PLL_PLL_AUDIO_CTRL_PLL_AUDIO_STABLE_MASK) !=
             ANADIG_PLL_PLL_AUDIO_CTRL_PLL_AUDIO_STABLE_MASK); /* wait for PLL locked */

    /* enabled clock */
    ANATOP_PllEnableClk(kAI_Itf_Audio, true);

    /* ungate clock */
    ANATOP_AudioPllGate(false);

    ANATOP_PllBypass(kAI_Itf_Audio, false);
}

void CLOCK_DeinitAudioPll(void)
{
    ANATOP_AudioPllSwEnClk(false);
    ANATOP_AudioPllGate(true);
    ANATOP_PllEnableClk(kAI_Itf_Audio, false);
    ANATOP_PllSetPower(kAI_Itf_Audio, false);
    ANATOP_PllEnablePllReg(kAI_Itf_Audio, false);
}

void ANATOP_VideoPllGate(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_VIDEO_CTRL &= ~ANADIG_PLL_PLL_VIDEO_CTRL_PLL_VIDEO_GATE_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_VIDEO_CTRL |= ANADIG_PLL_PLL_VIDEO_CTRL_PLL_VIDEO_GATE_MASK;
    }
}

void ANATOP_VideoPllSwEnClk(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_VIDEO_CTRL &= ~ANADIG_PLL_PLL_VIDEO_CTRL_ENABLE_CLK_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_VIDEO_CTRL |= ANADIG_PLL_PLL_VIDEO_CTRL_ENABLE_CLK_MASK;
    }
}

status_t CLOCK_CalcArmPllFreq(clock_arm_pll_config_t *cfg, uint32_t freqInMhz)
{
    assert(cfg);
    uint32_t refFreq = (XTAL_FREQ / 1000000) * 104; /* MHz */
    assert((freqInMhz <= refFreq) && (freqInMhz >= 156));

    /*
     * ARM_PLL  (156Mhz - 2496Mhz configureable )
     * freqInMhz = osc_freq * loopDivider / (2 * postDivider)
     *   - loopDivider:  104 - 208
     *   - postDivider:  0 - 3
     *   postDivider:
     *     0 - divide by 2; 1 - divide by 4; 2 - divide by 8; 3 - divide by 1
     */

    if (freqInMhz >= refFreq)
    {
        cfg->postDivider = kCLOCK_PllPostDiv1;
        cfg->loopDivider = 208;
    }
    else if (freqInMhz >= (refFreq >> 1))
    {
        cfg->postDivider = kCLOCK_PllPostDiv1;
        cfg->loopDivider = freqInMhz / 12 ;
    }
    else if (freqInMhz >= (refFreq >> 2))
    {
        cfg->postDivider = kCLOCK_PllPostDiv2;
        cfg->loopDivider = freqInMhz / 6;
    }
    else if (freqInMhz >= (refFreq >> 3))
    {
        cfg->postDivider = kCLOCK_PllPostDiv4;
        cfg->loopDivider = freqInMhz / 3;
    }
    else if (freqInMhz > (refFreq >> 4))
    {
        cfg->postDivider = kCLOCK_PllPostDiv8;
        cfg->loopDivider = 2 * freqInMhz / 3;
    }
    else
    {
        cfg->postDivider = kCLOCK_PllPostDiv8;
        cfg->loopDivider = 104;
    }
    return kStatus_Success;
}

status_t CLOCK_InitArmPllWithFreq(uint32_t freqInMhz)
{
    clock_arm_pll_config_t config;
    if (kStatus_Success == CLOCK_CalcArmPllFreq(&config, freqInMhz))
    {
        CLOCK_InitArmPll(&config);
        return kStatus_Success;
    }
    return kStatus_Fail;
}

status_t CLOCK_CalcAvPllFreq(clock_av_pll_config_t *cfg, uint32_t freqInMhz)
{
    assert(cfg);

    uint32_t refFreq = (XTAL_FREQ / 1000000) * 54; /* MHz */

    assert((freqInMhz <= refFreq) && (freqInMhz > 20));

    /*
     * AUDIO_PLL/VIDEO_PLL  (20.3125MHZ--- 1300MHZ configureable )
     * freqInMhz = osc_freq * (loopDivider + numerator / (2^28 - 1) ) / 2^postDivider
     *   - loopDivider:  27---54
     *   - postDivider: 0---5
     *   - numerator is a signed number, 30 bit,  numer[29] is the sign bit, such as +1--->0x00000001; -1---> 0x20000001
     * such as: div_sel = 27, numer = 0x026EEEEF, post_div =0, fref = 24.0MHZ, output_fre =24.0*(27 + 40824559/(2^28 -
     * 1))/2^0 = 651.65M such as: div_sel = 33, numer = 0x0F0AAAAA, post_div =0, fref = 19.2MHZ, output_fre =19.2*(33 +
     * 252357290/(2^28 - 1))/2^0= 651.65M
     */

    cfg->denominator = 0x0FFFFFFF;

    if (freqInMhz >= refFreq)
    {
        cfg->postDivider = 0;
        cfg->loopDivider = 54;
        cfg->numerator = 0;
    }
    else if (freqInMhz >= (refFreq >> 1))
    {
        cfg->postDivider = 0;
        cfg->loopDivider = freqInMhz / 24;
        cfg->numerator = (cfg->denominator / 24) * (freqInMhz % 24);
    }
    else if (freqInMhz >= (refFreq >> 2))
    {
        cfg->postDivider = 1;
        cfg->loopDivider = freqInMhz / 12;
        cfg->numerator = (cfg->denominator / 12) * (freqInMhz % 12);
    }
    else if (freqInMhz >= (refFreq >> 3))
    {
        cfg->postDivider = 2;
        cfg->loopDivider = freqInMhz / 6;
        cfg->numerator = (cfg->denominator / 6) * (freqInMhz % 6);
    }
    else if (freqInMhz >= (refFreq >> 4))
    {
        cfg->postDivider = 3;
        cfg->loopDivider = freqInMhz / 3;
        cfg->numerator = (cfg->denominator / 3) * (freqInMhz % 3);
    }
    else if (freqInMhz >= (refFreq >> 5))
    {
        cfg->postDivider = 4;
        cfg->loopDivider = freqInMhz * 2 / 3;
        cfg->numerator = (cfg->denominator * 2 / 3) * (freqInMhz * 2 % 3);
    }
    else if (freqInMhz > (refFreq >> 6))
    {
        cfg->postDivider = 5;
        cfg->loopDivider = freqInMhz * 4 / 3;
        cfg->numerator = (cfg->denominator * 4 / 3) * (freqInMhz * 4 % 3);
    }
    else
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}

status_t CLOCK_InitVideoPllWithFreq(uint32_t freqInMhz)
{
    clock_video_pll_config_t config = {0};
    if (kStatus_Success == CLOCK_CalcAvPllFreq(&config, freqInMhz))
    {
        CLOCK_InitVideoPll(&config);
        return kStatus_Success;
    }
    return kStatus_Fail;
}

void CLOCK_InitVideoPll(const clock_video_pll_config_t *config)
{
    uint32_t reg;
    bool pllStable = false;

    ANATOP_EnablePllLdo();

    reg = ANADIG_PLL->PLL_VIDEO_CTRL;
    if ((reg & (ANADIG_PLL_PLL_VIDEO_CTRL_PLL_VIDEO_STABLE_MASK | ANADIG_PLL_PLL_VIDEO_CTRL_ENABLE_CLK_MASK)) ==
        (ANADIG_PLL_PLL_VIDEO_CTRL_PLL_VIDEO_STABLE_MASK | ANADIG_PLL_PLL_VIDEO_CTRL_ENABLE_CLK_MASK))
    {
        pllStable = true;
    }
    /* bypass pll */
    ANATOP_PllBypass(kAI_Itf_Video, true);

    if (pllStable)
    {
        /* sw enable clock */
        ANATOP_VideoPllSwEnClk(false);
        /* gate clock */
        ANATOP_VideoPllGate(true);
        ANATOP_PllEnableClk(kAI_Itf_Video, false);
        ANATOP_PllSetPower(kAI_Itf_Video, false);
        ANATOP_PllEnablePllReg(kAI_Itf_Video, false);
    }

    ANATOP_VideoPllSwEnClk(true);
    /* configure pll */
    ANATOP_PllConfigure(kAI_Itf_Video, config->loopDivider, config->numerator, config->postDivider,
                        config->denominator);

    /* toggle hold ring off */
    ANATOP_PllToggleHoldRingOff(kAI_Itf_Video, 225);

    /* wait pll stable */
    do
    {
        reg = ANADIG_PLL->PLL_VIDEO_CTRL;
    } while ((reg & ANADIG_PLL_PLL_VIDEO_CTRL_PLL_VIDEO_STABLE_MASK) !=
             ANADIG_PLL_PLL_VIDEO_CTRL_PLL_VIDEO_STABLE_MASK); /* wait for PLL locked */

    /* enabled clock */
    ANATOP_PllEnableClk(kAI_Itf_Video, true);

    /* ungate clock */
    ANATOP_VideoPllGate(false);

    ANATOP_PllBypass(kAI_Itf_Video, false);
}

void CLOCK_DeinitVideoPll(void)
{
    ANATOP_VideoPllSwEnClk(false);
    ANATOP_VideoPllGate(true);
    ANATOP_PllEnableClk(kAI_Itf_Video, false);
    ANATOP_PllSetPower(kAI_Itf_Video, false);
    ANATOP_PllEnablePllReg(kAI_Itf_Video, false);
}

void ANATOP_SysPll1Gate(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_1G_CTRL &= ~ANADIG_PLL_PLL_1G_CTRL_PLL_1G_GATE_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_1G_CTRL |= ANADIG_PLL_PLL_1G_CTRL_PLL_1G_GATE_MASK;
    }
}

void ANATOP_SysPll1Div2En(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_1G_CTRL &= ~ANADIG_PLL_PLL_1G_CTRL_PLL_1G_DIV2_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_1G_CTRL |= ANADIG_PLL_PLL_1G_CTRL_PLL_1G_DIV2_MASK;
    }
}

void ANATOP_SysPll1Div5En(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_1G_CTRL &= ~ANADIG_PLL_PLL_1G_CTRL_PLL_1G_DIV5_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_1G_CTRL |= ANADIG_PLL_PLL_1G_CTRL_PLL_1G_DIV5_MASK;
    }
}

void ANATOP_SysPll1SwEnClk(bool enable)
{
    if (!enable)
    {
        ANADIG_PLL->PLL_1G_CTRL &= ~ANADIG_PLL_PLL_1G_CTRL_ENABLE_CLK_MASK;
    }
    else
    {
        ANADIG_PLL->PLL_1G_CTRL |= ANADIG_PLL_PLL_1G_CTRL_ENABLE_CLK_MASK;
    }
}

void ANATOP_SysPll1WaitStable(void)
{
    uint32_t reg;
    do
    {
        reg = ANADIG_PLL->PLL_1G_CTRL;
    } while ((reg & ANADIG_PLL_PLL_1G_CTRL_PLL_1G_STABLE_MASK) !=
             ANADIG_PLL_PLL_1G_CTRL_PLL_1G_STABLE_MASK); /* wait for PLL locked */
}

/* 1GPLL */
void CLOCK_InitSysPll1(const clock_sys_pll1_config_t *config)
{
    uint32_t div, numerator, denominator;

    ANATOP_EnablePllLdo();
    /* bypass pll */
    ANATOP_PllBypass(kAI_Itf_1g, true);

    /* sw enable clock */
    ANATOP_SysPll1SwEnClk(true);

    denominator = 0x0FFFFFFF;
    switch (XTAL_FREQ / 100000)
    {
        case 240:
            div = 41;
            numerator = 178956970;
            break;
        case 192:
            div = 52;
            numerator = 22369621;
            break;
        default:
            assert(false);
            return;
    }

    /* configure pll */
    ANATOP_PllConfigure(kAI_Itf_1g, div, numerator, 0, denominator);

    /* toggle hold ring off */
    ANATOP_PllToggleHoldRingOff(kAI_Itf_1g, 225);

    /* wait pll stable */
    ANATOP_SysPll1WaitStable();

    /* enabled clock */
    ANATOP_PllEnableClk(kAI_Itf_1g, true);

    /* ungate clock */
    ANATOP_SysPll1Gate(false);

    ANATOP_SysPll1Div2En(config->pllDiv2En);
    ANATOP_SysPll1Div5En(config->pllDiv5En);

    /* bypass pll */
    ANATOP_PllBypass(kAI_Itf_1g, false);
}

void CLOCK_DeinitSysPll1(void)
{
    ANATOP_SysPll1SwEnClk(false);
    ANATOP_SysPll1Div2En(false);
    ANATOP_SysPll1Div5En(false);
    ANATOP_SysPll1Gate(true);
    ANATOP_PllEnableClk(kAI_Itf_1g, false);
    ANATOP_PllSetPower(kAI_Itf_1g, false);
    ANATOP_PllEnablePllReg(kAI_Itf_1g, false);
}

void CLOCK_EnableOsc24M(void)
{
    if (!(ANADIG_OSC->OSC_24M_CTRL & ANADIG_OSC_OSC_24M_CTRL_OSC_EN_MASK))
    {
        ANADIG_OSC->OSC_24M_CTRL = ANADIG_OSC_OSC_24M_CTRL_OSC_EN_MASK
                            | ANADIG_OSC_OSC_24M_CTRL_LP_EN_MASK;
        while (ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK !=
                (ANADIG_OSC->OSC_24M_CTRL & ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK))
        {
        }
    }
}

uint32_t CLOCK_GetAvPllFreq(clock_pll_t pll)
{
    uint32_t freq = 0;
    uint32_t div;
    uint32_t post_div;
    double denom;
    double numer;

    assert((pll == kCLOCK_PllAudio) || (pll == kCLOCK_PllVideo));

    div = ANATOP_AI_Read(pll == kCLOCK_PllAudio ? kAI_Itf_Audio : kAI_Itf_Video, 0);
    post_div = (div & (0xE000000)) >> 25;
    div &= 0x7f;

    denom = ANATOP_AI_Read(pll == kCLOCK_PllAudio ? kAI_Itf_Audio : kAI_Itf_Video, 0x30);
    numer = ANATOP_AI_Read(pll == kCLOCK_PllAudio ? kAI_Itf_Audio : kAI_Itf_Video, 0x20);

    freq = (uint32_t)(XTAL_FREQ * (div + numer/denom) / (1 << post_div));
    return freq;
}

uint32_t CLOCK_GetPllFreq(clock_pll_t pll)
{
    uint32_t freq = 0;
#ifndef GET_FREQ_FROM_OBS
    uint32_t divSelect, postDiv, mfi, mfn, mfd;
#endif

    switch (pll)
    {
        case kCLOCK_PllArm:
#ifndef GET_FREQ_FROM_OBS
            divSelect = (ANADIG_PLL->PLL_ARM_CTRL & ANADIG_PLL_PLL_ARM_CTRL_DIV_SELECT_MASK) >>
                        ANADIG_PLL_PLL_ARM_CTRL_DIV_SELECT_SHIFT;
            postDiv = (ANADIG_PLL->PLL_ARM_CTRL & ANADIG_PLL_PLL_ARM_CTRL_POST_DIV_SEL_MASK) >>
                      ANADIG_PLL_PLL_ARM_CTRL_POST_DIV_SEL_SHIFT;
            postDiv = (1 << (postDiv + 1));
            freq = XTAL_FREQ / (2 * postDiv);
            freq *= divSelect;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_PLL_ARM_OUT);
#endif
            break;
        case kCLOCK_PllSys1:
#ifndef GET_FREQ_FROM_OBS
            freq = SYS_PLL1_FREQ;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_PLL_1G_OUT);
#endif
            break;
        case kCLOCK_PllSys2:
#ifndef GET_FREQ_FROM_OBS
            mfi = (ANADIG_PLL->PLL_528_MFI & ANADIG_PLL_PLL_528_MFI_MFI_MASK) >> ANADIG_PLL_PLL_528_MFI_MFI_SHIFT;
            mfn = (ANADIG_PLL->PLL_528_MFN & ANADIG_PLL_PLL_528_MFN_MFN_MASK) >> ANADIG_PLL_PLL_528_MFN_MFN_SHIFT;
            mfd = (ANADIG_PLL->PLL_528_MFD & ANADIG_PLL_PLL_528_MFD_MFD_MASK) >> ANADIG_PLL_PLL_528_MFD_MFD_SHIFT;
            freq = XTAL_FREQ * (mfi + mfn / mfd);
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_PLL_528_OUT);
#endif
            break;
        case kCLOCK_PllSys3:
#ifndef GET_FREQ_FROM_OBS
            freq = SYS_PLL3_FREQ;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_PLL_480_OUT);
#endif
            break;
        case kCLOCK_PllAudio:
#ifndef GET_FREQ_FROM_OBS
            freq = CLOCK_GetAvPllFreq(kCLOCK_PllAudio);
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_PLL_AUDIO_OUT);
#endif
            break;
        case kCLOCK_PllVideo:
#ifndef GET_FREQ_FROM_OBS
            freq = CLOCK_GetAvPllFreq(kCLOCK_PllVideo);
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_PLL_VIDEO_OUT);
#endif
            break;
        default:
            assert(false);
            break;
    }
    assert(freq);
    return freq;
}

uint32_t CLOCK_GetFreq(clock_name_t name)
{
    uint32_t freq = 0;
    switch (name)
    {
        case kCLOCK_OscRc16M:
#ifndef GET_FREQ_FROM_OBS
            /* If select 16M. */
            if (0UL != (ANADIG_OSC->OSC_4M16M_CTRL & ANADIG_OSC_OSC_4M16M_CTRL_SEL_16M_MASK))
            {
                freq = 16000000U;
            }
            else
            {
                freq = 4000000U;
            }
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_OSC_RC_16M);
#endif
            break;
        case kCLOCK_OscRc48M:
#ifndef GET_FREQ_FROM_OBS
            freq = 48000000U;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_OSC_RC_48M);
#endif
            break;
        case kCLOCK_OscRc48MDiv2:
#ifndef GET_FREQ_FROM_OBS
            freq = 24000000U;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_OSC_RC_48M_DIV2);
#endif
            break;
        case kCLOCK_OscRc400M:
#ifndef GET_FREQ_FROM_OBS
            freq = 400000000U;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_OSC_RC_400M);
#endif
            break;
        case kCLOCK_Osc24MOut:
        case kCLOCK_Osc24M:
#ifndef GET_FREQ_FROM_OBS
            freq = 24000000U;
#else
            freq = CLOCK_GetFreqFromObs(CCM_OBS_OSC_24M_OUT);
#endif
            break;
        case kCLOCK_ArmPllOut:
        case kCLOCK_ArmPll:
            freq = CLOCK_GetPllFreq(kCLOCK_PllArm);
            break;
        case kCLOCK_SysPll2:
        case kCLOCK_SysPll2Out:
            freq = CLOCK_GetPllFreq(kCLOCK_PllSys2);
            break;
        case kCLOCK_SysPll2Pfd0:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys2, kCLOCK_Pfd0);
            break;
        case kCLOCK_SysPll2Pfd1:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys2, kCLOCK_Pfd1);
            break;
        case kCLOCK_SysPll2Pfd2:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys2, kCLOCK_Pfd2);
            break;
        case kCLOCK_SysPll2Pfd3:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys2, kCLOCK_Pfd3);
            break;
        case kCLOCK_SysPll3Out:
        case kCLOCK_SysPll3:
            freq = CLOCK_GetPllFreq(kCLOCK_PllSys3);
            break;
        case kCLOCK_SysPll3Pfd0:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys3, kCLOCK_Pfd0);
            break;
        case kCLOCK_SysPll3Pfd1:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys3, kCLOCK_Pfd1);
            break;
        case kCLOCK_SysPll3Pfd2:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys3, kCLOCK_Pfd2);
            break;
        case kCLOCK_SysPll3Pfd3:
            freq = CLOCK_GetPfdFreq(kCLOCK_PllSys3, kCLOCK_Pfd3);
            break;
        case kCLOCK_SysPll1:
        case kCLOCK_SysPll1Out:
            freq = CLOCK_GetPllFreq(kCLOCK_PllSys1);
            break;
        case kCLOCK_SysPll1Div2:
            freq = CLOCK_GetPllFreq(kCLOCK_PllSys1) / 2;
            break;
        case kCLOCK_SysPll1Div5:
            freq = CLOCK_GetPllFreq(kCLOCK_PllSys1) / 5;
            break;
        case kCLOCK_AudioPll:
        case kCLOCK_AudioPllOut:
            freq = CLOCK_GetPllFreq(kCLOCK_PllAudio);
            break;
        case kCLOCK_VideoPll:
        case kCLOCK_VideoPllOut:
            freq = CLOCK_GetPllFreq(kCLOCK_PllVideo);
            break;
        case kCLOCK_CpuClk:
        case kCLOCK_CoreSysClk:
            freq = CLOCK_GetCpuClkFreq();
            break;
        default:
            assert(false);
            break;
    }
    assert(freq);
    return freq;
}

void CLOCK_SetGroupConfig(clock_group_t group, const clock_group_config_t *config)
{
    assert(group < kCLOCK_Group_Last);

    CCM->CLOCK_GROUP[group].CONTROL =
        ((config->clockOff ? CCM_CLOCK_GROUP_CONTROL_OFF_MASK : 0U) |
         (config->resetDiv << CCM_CLOCK_GROUP_CONTROL_RSTDIV_SHIFT) |
         (config->div0 << CCM_CLOCK_GROUP_CONTROL_DIV0_SHIFT) | (config->div1 << CCM_CLOCK_GROUP_CONTROL_DIV1_SHIFT) |
         (config->div2 << CCM_CLOCK_GROUP_CONTROL_DIV2_SHIFT) | (config->div3 << CCM_CLOCK_GROUP_CONTROL_DIV3_SHIFT));

    CCM->CLOCK_GROUP[group].CONTROL_EXTEND = ((config->div4 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV4_SHIFT) |
                                              (config->div5 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV5_SHIFT) |
                                              (config->div6 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV6_SHIFT) |
                                              (config->div7 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV7_SHIFT) |
                                              (config->div8 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV8_SHIFT) |
                                              (config->div9 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV9_SHIFT) |
                                              (config->div10 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV10_SHIFT) |
                                              (config->div11 << CCM_CLOCK_GROUP_CONTROL_EXTEND_DIV11_SHIFT));
}

void CLOCK_TrimOscRc400M(void)
{
    uint32_t trimRegValue = ANADIG_OSC->OSC_OTP_TRIM_VALUE_200M;
    uint32_t trimEnable = (trimRegValue & (1u << 9)) ? 1 : 0;
    uint32_t trimBypass = (trimRegValue & (1u << 8)) ? 1 : 0;
    uint32_t trimValue = trimRegValue & 0xFF;

    if (trimEnable)
    {
        ANADIG_MISC->VDDLPSR_AI400M_CTRL = 0x20;
        ANADIG_MISC->VDDLPSR_AI400M_WDATA = (trimBypass << 10) | (trimValue << 24);
        ANADIG_MISC->VDDLPSR_AI400M_CTRL |= 0x100;
        SDK_DelayAtLeastUs(1, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        ANADIG_MISC->VDDLPSR_AI400M_CTRL &= ~0x100;
    }
}

void CLOCK_EnableOscRc400M(void)
{
    ANADIG_OSC->OSC_200M_CTRL1 &= ~0x1;
    ANADIG_OSC->OSC_200M_CTRL2 |= 0x1;
}

uint32_t CLOCK_GetFreqFromObs(uint32_t obsSigIndex, uint32_t obsIndex)
{
    CCM_OBS->OBSERVE[obsIndex].CONTROL = CCM_OBS_OBSERVE_CONTROL_OFF_MASK;   /* turn off detect */
    CCM_OBS->OBSERVE[obsIndex].CONTROL_SET = CCM_OBS_OBSERVE_CONTROL_RESET_MASK; /* reset slice */
    CCM_OBS->OBSERVE[obsIndex].CONTROL_CLR = CCM_OBS_OBSERVE_CONTROL_RAW_MASK;   /* select raw obsSigIndex */
    CCM_OBS->OBSERVE[obsIndex].CONTROL &= ~CCM_OBS_OBSERVE_CONTROL_SELECT_MASK;  /* Select observed obsSigIndex */
    CCM_OBS->OBSERVE[obsIndex].CONTROL |= CCM_OBS_OBSERVE_CONTROL_SELECT(obsSigIndex) |
                                          CCM_OBS_OBSERVE_CONTROL_DIVIDE(CCM_OBS_DIV); /* turn on detection */
    CCM_OBS->OBSERVE[obsIndex].CONTROL_CLR =
        CCM_OBS_OBSERVE_CONTROL_RESET_MASK | CCM_OBS_OBSERVE_CONTROL_OFF_MASK; /* unreset and turn on detect */

    while (CCM_OBS->OBSERVE[obsIndex].FREQUENCY_CURRENT == 0)
    {
    }

    return (CCM_OBS->OBSERVE[obsIndex].FREQUENCY_CURRENT * (CCM_OBS_DIV + 1));
}

/*! brief Enable USB HS clock.
 *
 * This function only enables the access to USB HS prepheral, upper layer
 * should first call the ref CLOCK_EnableUsbhs0PhyPllClock to enable the PHY
 * clock to use USB HS.
 *
 * param src  USB HS does not care about the clock source, here must be ref kCLOCK_UsbSrcUnused.
 * param freq USB HS does not care about the clock source, so this parameter is ignored.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs0Clock(clock_usb_src_t src, uint32_t freq)
{
    return true;
}
/*! brief Enable USB HS PHY PLL clock.
 *
 * This function enables the internal 480MHz USB PHY PLL clock.
 *
 * param src  USB HS PHY PLL clock source.
 * param freq The frequency specified by src.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs0PhyPllClock(clock_usb_phy_src_t src, uint32_t freq)
{
    uint32_t phyPllDiv = 0U;
    uint16_t multiplier = 0U;
    CLOCK_EnableClock(kCLOCK_Usb);

    USBPHY1->CTRL_CLR = USBPHY_CTRL_SFTRST_MASK;

    USBPHY1->PLL_SIC_SET = (USBPHY_PLL_SIC_PLL_POWER(1) | USBPHY_PLL_SIC_PLL_REG_ENABLE_MASK);
    if (480000000U % freq)
    {
        return false;
    }
    multiplier = 480000000 / freq;

    switch (multiplier)
    {
        case 13:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(0U);
            break;
        }
        case 15:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(1U);
            break;
        }
        case 16:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(2U);
            break;
        }
        case 20:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(3U);
            break;
        }
        case 22:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(4U);
            break;
        }
        case 25:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(5U);
            break;
        }
        case 30:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(6U);
            break;
        }
        case 240:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(7U);
            break;
        }
        default:
        {
            return false;
        }
    }
    USBPHY1->PLL_SIC = (USBPHY1->PLL_SIC & ~(USBPHY_PLL_SIC_PLL_DIV_SEL_MASK)) | phyPllDiv;

    USBPHY1->PLL_SIC_CLR = USBPHY_PLL_SIC_PLL_BYPASS_MASK;
    USBPHY1->PLL_SIC_SET = (USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK);

    USBPHY1->CTRL_CLR = USBPHY_CTRL_CLR_CLKGATE_MASK;
    USBPHY1->PWD_SET = 0x0;

    while (!(USBPHY1->PLL_SIC & USBPHY_PLL_SIC_PLL_LOCK_MASK))
    {
    }
    return true;
}
/*! brief Enable USB HS clock.
 *
 * This function only enables the access to USB HS prepheral, upper layer
 * should first call the ref CLOCK_EnableUsbhs0PhyPllClock to enable the PHY
 * clock to use USB HS.
 *
 * param src  USB HS does not care about the clock source, here must be ref kCLOCK_UsbSrcUnused.
 * param freq USB HS does not care about the clock source, so this parameter is ignored.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
/*! brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhs0PhyPllClock(void)
{
    USBPHY1->PLL_SIC_CLR = (USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK);
    USBPHY1->CTRL |= USBPHY_CTRL_CLKGATE_MASK; /* Set to 1U to gate clocks */
}
bool CLOCK_EnableUsbhs1Clock(clock_usb_src_t src, uint32_t freq)
{
    return true;
}
bool CLOCK_EnableUsbhs1PhyPllClock(clock_usb_phy_src_t src, uint32_t freq)
{
    uint32_t phyPllDiv = 0U;
    uint16_t multiplier = 0U;
    CLOCK_EnableClock(kCLOCK_Usb);

    USBPHY2->CTRL_CLR = USBPHY_CTRL_SFTRST_MASK;

    USBPHY2->PLL_SIC_SET = (USBPHY_PLL_SIC_PLL_POWER(1) | USBPHY_PLL_SIC_PLL_REG_ENABLE_MASK);
    if (480000000U % freq)
    {
        return false;
    }
    multiplier = 480000000 / freq;

    switch (multiplier)
    {
        case 13:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(0U);
            break;
        }
        case 15:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(1U);
            break;
        }
        case 16:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(2U);
            break;
        }
        case 20:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(3U);
            break;
        }
        case 22:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(4U);
            break;
        }
        case 25:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(5U);
            break;
        }
        case 30:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(6U);
            break;
        }
        case 240:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(7U);
            break;
        }
        default:
        {
            return false;
        }
    }
    USBPHY2->PLL_SIC = (USBPHY2->PLL_SIC & ~(USBPHY_PLL_SIC_PLL_DIV_SEL_MASK)) | phyPllDiv;

    USBPHY2->PLL_SIC_CLR = USBPHY_PLL_SIC_PLL_BYPASS_MASK;
    USBPHY2->PLL_SIC_SET = (USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK);

    USBPHY2->CTRL_CLR = USBPHY_CTRL_CLR_CLKGATE_MASK;
    USBPHY2->PWD_SET = 0x0;

    while (!(USBPHY2->PLL_SIC & USBPHY_PLL_SIC_PLL_LOCK_MASK))
    {
    }
    return true;
}
/*! brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhs1PhyPllClock(void)
{
    USBPHY2->PLL_SIC_CLR = (USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK);
    USBPHY2->CTRL |= USBPHY_CTRL_CLKGATE_MASK; /* Set to 1U to gate clocks */
}
