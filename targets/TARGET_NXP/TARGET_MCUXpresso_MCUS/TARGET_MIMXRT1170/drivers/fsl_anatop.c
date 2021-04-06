/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_anatop.h"
#include "fsl_anatop_ai.h"
#include "fsl_clock.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.anatop"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/* CM7 FBB config */
void ANATOP_Cm7FbbCfg(void)
{
    /* anatop_cm7_fbb_config! */;
    ANATOP_InitWbiasCfg(true, false);
    ANATOP_WbCfg1p8Cfg(0x01EE);
    /* This is to enable PW/NW regulator path to CM7 FBB and disable regulator
     * path to LPSR/SOC RBB. Since RVT does not support FBB.
     */
    ANATOP_WbPwrSwEn1p8(1);
    /* Enable wbias */
    ANATOP_EnableWbias(true);
}

void ANATOP_InitWbiasCfg(bool fbb_on, bool rbb_on)
{
    if(fbb_on)
    {
        /* This is to select FBB regulator target voltage as 0.6V for CM7 LVT. */
        /* Set wb_nw_lvl_1p8 and wb_pw_lvl_1p8 in PMU_BIAS_CTRL to 4b’0001! */;
        ANATOP_WbiasPwlvl_1p8(0x1);
        ANATOP_WbiasNwlvl_1p8(0x1);
    }
    else if(rbb_on)
    {
        ANATOP_WbiasPwlvl_1p8(0x5);
        ANATOP_WbiasNwlvl_1p8(0x5);
    }
    else
    {
        ANATOP_WbiasPwlvl_1p8(0x0);
        ANATOP_WbiasNwlvl_1p8(0x0);
    }
}

void ANATOP_WbiasPwlvl_1p8(uint32_t pw_lvl_1p8)
{
    uint32_t  reg;
    /* If wb_en_1p8=1, these bits set the vbb_rpw voltage level! */;
    reg = ANADIG_PMU->PMU_BIAS_CTRL;
    reg &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8_MASK;
    reg |= ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8(pw_lvl_1p8);
    ANADIG_PMU->PMU_BIAS_CTRL = reg;
    /* anatop_wbias_pw_lvl_1p8: finished. */;
}

void ANATOP_WbiasNwlvl_1p8(uint32_t nw_lvl_1p8)
{
    uint32_t  reg;
    /* If wb_en_1p8=1, these bits set the vbb_rnw voltage level! */;
    reg = ANADIG_PMU->PMU_BIAS_CTRL;
    reg &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8_MASK;
    reg |= ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8(nw_lvl_1p8);
    ANADIG_PMU->PMU_BIAS_CTRL = reg;
    /* anatop_wbias_nw_lvl_1p8: finished. */;
}

void ANATOP_WbCfg1p8Cfg(uint32_t wb_cfg_1p8)
{
    uint32_t  reg;
    /* anatop_wb_cfg_1p8_cfg. */;
    /* Wb_cfg_1p8<0> = 0, NW tracking regulator is used */
    /* Wb_cfg_1p8<1> = 1, LVT selection */
    /* Wb_cfg_1p8<4:2> = 3’b011, drive strength for M7 FBB */
    /* Wb_cfg_1p8<5:8> = 4’b1111 use osc frequency */
    reg = ANADIG_PMU->PMU_BIAS_CTRL;
    reg &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8_MASK;
    reg |= ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8(wb_cfg_1p8);
    ANADIG_PMU->PMU_BIAS_CTRL = reg;
}

void ANATOP_WbPwrSwEn1p8(uint32_t wb_pwr_sw_en_1p8)
{
    uint32_t  reg;
    /* Set wb_pwr_sw_en_1p8 in PMU_BIAS_CTRL2! */;
    reg = ANADIG_PMU->PMU_BIAS_CTRL2;
    reg &= ~ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK;
    reg |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(wb_pwr_sw_en_1p8);
    ANADIG_PMU->PMU_BIAS_CTRL2 = reg;
}

void ANATOP_EnableWbias(bool enable)
{
    uint32_t  reg;
    reg = ANADIG_PMU->PMU_BIAS_CTRL2;
    reg &= ~ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
    reg |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN(enable);
    ANADIG_PMU->PMU_BIAS_CTRL2 = reg;
}

void ANATOP_EnablePllLdo()
{
    uint32_t r = ANATOP_AI_Read(kAI_Itf_Ldo, 0);
    if (r != 0x105)
    {
        ANATOP_AI_Write(kAI_Itf_Ldo, 0, 0x105);
        ANATOP_AI_Read(kAI_Itf_Ldo, 0);
        ANADIG_PMU->PMU_POWER_DETECT_CTRL = ANADIG_PMU_PMU_POWER_DETECT_CTRL_CKGB_LPSR1P0_MASK;
        ANADIG_MISC->VDDSOC_AI_CTRL &= ~ANADIG_MISC_VDDSOC_AI_CTRL_VDDSOC_AIRWB_MASK;
        SDK_DelayAtLeastUs(1, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        ANADIG_PMU->PMU_REF_CTRL |= 0x10;
    }
}

void ANATOP_DisablePllLdo()
{
    ANATOP_AI_Write(kAI_Itf_Ldo, 0, 0);
}

void ANATOP_TrimLdoLpsrDig(uint32_t target_voltage)
{
    uint8_t trim_value;
    uint32_t reg;

    trim_value = (target_voltage - 628)/20;
    reg = ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG;
    reg &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRIM_MASK;
    reg |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRIM(trim_value);
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG = reg;
}

void ANATOP_LdoLpsrAnaBypassOn()
{
    /* HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRACK_MODE_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* set BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Disable LDO */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK;
}

void ANATOP_LdoLpsrAnaBypassOff()
{
    /* Enable LDO and HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~(ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK |
            ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK);
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Clear BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Disable tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRACK_MODE_EN_MASK;
}

void ANATOP_LdoLpsrDigBypassOn()
{
    /* HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRACKING_MODE_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* set BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Disable LDO */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK;
}

void ANATOP_LdoLpsrDigBypassOff()
{
    /* Enable LDO and HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= (ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK |
            ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK);
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Clear BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Disable tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRACKING_MODE_MASK;
}

void ANATOP_BothLdoLpsrBypassOn()
{
    /* HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK;
    /* HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRACK_MODE_EN_MASK;
    /* tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRACKING_MODE_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* set BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK;
    /* set BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Disable LDO */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK;
    /* Disable LDO */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK;
}

void ANATOP_BothLdoLpsrBypassOff()
{
    /* Enable LDO */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK;
    /* Enable LDO */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK;
    /* HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK;
    /* HP mode */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Clear BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK;
    /* Clear BYPASS */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK;
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    /* Disable tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRACKING_MODE_MASK;
    /* Disable tracking */
    ANADIG_LDO_SNVS->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRACK_MODE_EN_MASK;
}
