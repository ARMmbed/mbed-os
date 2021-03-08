/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_pmu.h"
#include "fsl_anatop_ai.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.pmu_1"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PMU_LDO_LPSR_DIG_TRG_SPX_REG_SETPOINT_COUNTS         4U
#define PMU_LDO_LPSR_DIG_TRG_SPX_VOLTAGE_SETPOINTX_BIT_WIDTH 8UL

#define PMU_POWER_DETECT_CTRL_REGISTER (ANADIG_PMU->PMU_POWER_DETECT_CTRL)

#define PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS (0x00U)

#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_MASK (0x1U)

#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION_MASK  (0x7U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION_SHIFT (0U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION_SHIFT)) & \
     PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION_MASK)

#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER_MASK  (0x8U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER_SHIFT (3U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER_SHIFT)) & \
     PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER_MASK)

#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF_MASK  (0x10U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF_SHIFT (4U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF_SHIFT)) & \
     PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF_MASK)

#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ_MASK  (0xE0U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ_SHIFT (5U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ_SHIFT)) & \
     PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ_MASK)

#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ_MASK  (0x1C00U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ_SHIFT (10U)
#define PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ_SHIFT)) & \
     PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ_MASK)

#define PMU_BANDGAP_ANALOG_STATUS_REGISTER_ADDRESS (0x50U)

#define PMU_BANDGAP_ANALOG_STATUS_REFTOP_VBGUP_MASK (0x01U)

#define PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK  (0x2U)
#define PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_SHIFT 1U
#define PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_SHIFT)) & \
     PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK)

#define PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_MASK  (0x1CU)
#define PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_SHIFT 2U
#define PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_SHIFT)) & \
     PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH_MASK)

#define PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_MASK  (0x1E0U)
#define PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_SHIFT 5U
#define PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ(x)                                    \
    (((uint32_t)(((uint32_t)(x)) << PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_SHIFT)) & \
     PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ_MASK)

#define PMU_LDO_ENABLE_SETPOINT_REGISTERS                                                            \
    {                                                                                                \
        (uint32_t)(&ANADIG_PMU->LDO_PLL_ENABLE_SP), (uint32_t)(&ANADIG_PMU->LDO_LPSR_ANA_ENABLE_SP), \
            (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_ENABLE_SP), 0UL                                     \
    }

#define PMU_LDO_LP_MODE_EN_SETPOINT_REGISTERS                                                                        \
    {                                                                                                                \
        0UL, (uint32_t)(&ANADIG_PMU->LDO_LPSR_ANA_LP_MODE_SP), (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_LP_MODE_SP), 0UL \
    }

#define PMU_LDO_TRACKING_EN_SETPOINT_REGISTERS                        \
    {                                                                 \
        0UL, (uint32_t)(&ANADIG_PMU->LDO_LPSR_ANA_TRACKING_EN_SP),    \
            (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_TRACKING_EN_SP), 0UL \
    }

#define PMU_LDO_BYPASS_EN_SETPOINT_REGISTERS                                                                         \
    {                                                                                                                \
        0UL, (uint32_t)(&ANADIG_PMU->LDO_LPSR_ANA_BYPASS_EN_SP), (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_BYPASS_EN_SP), \
            0UL                                                                                                      \
    }

#define PMU_LDO_STBY_EN_REGISTERS                                                                      \
    {                                                                                                  \
        (uint32_t)(&ANADIG_PMU->PLL_LDO_STBY_EN_SP), (uint32_t)(&ANADIG_PMU->LDO_LPSR_ANA_STBY_EN_SP), \
            (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_STBY_EN_SP), 0UL                                      \
    }

#define PMU_LPSR_DIG_TRG_REGISTERS                                                                       \
    {                                                                                                    \
        (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_TRG_SP0), (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_TRG_SP1),    \
            (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_TRG_SP2), (uint32_t)(&ANADIG_PMU->LDO_LPSR_DIG_TRG_SP3) \
    }

#define PMU_BODY_BIAS_ENABLE_REGISTERS                                                         \
    {                                                                                          \
        (uint32_t)(&ANADIG_PMU->FBB_M7_ENABLE_SP), (uint32_t)(&ANADIG_PMU->RBB_SOC_ENABLE_SP), \
            (uint32_t)(&ANADIG_PMU->RBB_LPSR_ENABLE_SP)                                        \
    }

#define PMU_BODY_BIAS_STBY_EN_REGISTERS                                                          \
    {                                                                                            \
        (uint32_t)(&ANADIG_PMU->FBB_M7_STBY_EN_SP), (uint32_t)(&ANADIG_PMU->RBB_SOC_STBY_EN_SP), \
            (uint32_t)(&ANADIG_PMU->RBB_LPSR_STBY_EN_SP)                                         \
    }

#define PMU_BODY_BIAS_CONFIGURE_REGISTERS                                                      \
    {                                                                                          \
        (uint32_t)(&ANADIG_PMU->FBB_M7_CONFIGURE), (uint32_t)(&ANADIG_PMU->RBB_SOC_CONFIGURE), \
            (uint32_t)(&ANADIG_PMU->RBB_LPSR_CONFIGURE)                                        \
    }

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * brief Selects the control mode of the PLL LDO.
 *
 * param base PMU peripheral base address.
 * param mode The control mode of the PLL LDO. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetPllLdoControlMode(ANADIG_PMU_Type *base, pmu_control_mode_t mode)
{
    if (mode == kPMU_StaticMode)
    {
        base->PMU_LDO_PLL &= ~ANADIG_PMU_PMU_LDO_PLL_LDO_PLL_CONTROL_MODE_MASK;
    }
    else
    {
        base->PMU_LDO_PLL |= ANADIG_PMU_PMU_LDO_PLL_LDO_PLL_CONTROL_MODE_MASK;
    }
}

/*!
 * brief Switches the PLL LDO from Static/Software Mode to GPC/Hardware Mode.
 *
 * param base PMU peripheral base address.
 */
void PMU_SwitchPllLdoToGPCMode(ANADIG_PMU_Type *base)
{
    if ((base->LDO_PLL_ENABLE_SP & ANADIG_PMU_LDO_PLL_ENABLE_SP_ON_OFF_SETPOINT0_MASK) != 0UL)
    {
        base->PMU_LDO_PLL |= ANADIG_PMU_PMU_LDO_PLL_LDO_PLL_ENABLE_MASK;
    }
    else
    {
        base->PMU_LDO_PLL &= ~ANADIG_PMU_PMU_LDO_PLL_LDO_PLL_ENABLE_MASK;
    }
}

/*!
 * brief Enables PLL LDO via AI interface in Static/Software mode.
 *
 * param base PMU peripheral base address.
 */
void PMU_StaticEnablePllLdo(ANADIG_PMU_Type *base)
{
    uint32_t temp32;

    temp32 = ANATOP_AI_Read(kAI_Itf_Ldo, 0U);

    if (temp32 != 0x105UL)
    {
        ANATOP_AI_Write(kAI_Itf_Ldo, 0U, 0x105UL);
        SDK_DelayAtLeastUs(1, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Enable Voltage Reference for PLLs before those PLLs were enabled. */
        base->PMU_REF_CTRL |= ANADIG_PMU_PMU_REF_CTRL_EN_PLL_VOL_REF_BUFFER_MASK;
    }
}

/*!
 * @brief Disables PLL LDO via AI interface in Static/Software mode.
 *
 * @param base PMU peripheral base address.
 */
void PMU_StaticDisablePllLdo(void)
{
    ANATOP_AI_Write(kAI_Itf_Ldo, 0U, 0UL);
}

/*!
 * brief Selects the control mode of the LPSR ANA LDO.
 *
 * param base PMU peripheral base address.
 * param mode The control mode of the LPSR ANA LDO. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetLpsrAnaLdoControlMode(ANADIG_LDO_SNVS_Type *base, pmu_control_mode_t mode)
{
    if (mode == kPMU_StaticMode)
    {
        base->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_LPSR_ANA_CONTROL_MODE_MASK;
    }
    else
    {
        base->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_LPSR_ANA_CONTROL_MODE_MASK;
    }
}

/*!
 * brief Sets the Bypass mode of the LPSR ANA LDO.
 *
 * param base ANADIG_LDO_SNVS peripheral base address.
 * param bypassMode The Bypass mode of LPSR ANA LDO. Please refer to @ref pmu_lpsr_ana_ldo_bypass_mode_t.
 */
void PMU_StaticSetLpsrAnaLdoBypassMode(ANADIG_LDO_SNVS_Type *base, pmu_lpsr_ana_ldo_bypass_mode_t bypassMode)
{
    uint32_t temp32;

    if (bypassMode == kPMU_LpsrAnaLdoBypassModeDisable)
    {
        /* Enable LPSR ANA LDO and HP mode. */
        base->PMU_LDO_LPSR_ANA &=
            ~(ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Clear Bypass. */
        base->PMU_LDO_LPSR_ANA &= ~(ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK |
                                    ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_CFG_MASK);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable Tracking mode. */
        base->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRACK_MODE_EN_MASK;
    }
    else
    {
        /* Enable HP mode. */
        base->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Enable Tracking mode. */
        base->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRACK_MODE_EN_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Enabled Bypass and set bypass mode. */
        temp32 = base->PMU_LDO_LPSR_ANA;
        temp32 &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_CFG_MASK;
        temp32 |= (ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_CFG(bypassMode) |
                   ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_MODE_EN_MASK);
        base->PMU_LDO_LPSR_ANA = temp32;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable LPSR ANA LDO. */
        base->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK;
    }
}

/*!
 * brief Fill the LPSR ANA LDO configuration structure with default settings.
 *
 * The default values are:
 * code
 *      config->mode                   = kPMU_HighPowerMode;
        config->enable2mALoad          = true;
        config->enable20uALoad         = false;
        config->enable4mALoad          = true;
        config->enableStandbyMode      = false;
        config->driverStrength         = kPMU_LpsrAnaLdoDriverStrength0;
        config->brownOutDetectorConfig = kPMU_LpsrAnaLdoBrownOutDetectorDisable;
        config->chargePumpCurrent      = kPMU_LpsrAnaChargePump300nA;
        config->outputRange            = kPMU_LpsrAnaLdoOutputFrom1P77To1P83;
 * endcode
 *
 * param config Pointer to the structure pmu_static_lpsr_ana_ldo_config_t. Please refer to @ref
 * pmu_static_lpsr_ana_ldo_config_t.
 */
void PMU_StaticGetLpsrAnaLdoDefaultConfig(pmu_static_lpsr_ana_ldo_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->mode                   = kPMU_HighPowerMode;
    config->enable2mALoad          = true;
    config->enable20uALoad         = false;
    config->enable4mALoad          = true;
    config->enableStandbyMode      = false;
    config->driverStrength         = kPMU_LpsrAnaLdoDriverStrength0;
    config->brownOutDetectorConfig = kPMU_LpsrAnaLdoBrownOutDetectorDisable;
    config->chargePumpCurrent      = kPMU_LpsrAnaChargePump300nA;
    config->outputRange            = kPMU_LpsrAnaLdoOutputFrom1P77To1P83;
}

/*!
 * brief Initialize the LPSR ANA LDO in Static/Sofware Mode.
 *
 * param base ANADIG_LDO_SNVS peripheral base address.
 * param config Pointer to the structure pmu_static_lpsr_ana_ldo_config_t. Please refer to @ref
 * pmu_static_lpsr_ana_ldo_config_t.
 */
void PMU_StaticLpsrAnaLdoInit(ANADIG_LDO_SNVS_Type *base, const pmu_static_lpsr_ana_ldo_config_t *config)
{
    assert(config != NULL);

    uint32_t regValue = base->PMU_LDO_LPSR_ANA;

    regValue &=
        ~(ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_PULL_DOWN_2MA_EN_MASK |
          ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_ALWAYS_4MA_PULLDOWN_EN_MASK |
          ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_PULL_DOWN_20UA_EN_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_STANDBY_EN_MASK |
          ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_PRECHRG_CURRENT_CFG_MASK |
          ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_ICP_TRIM_SNVS_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRIM_MASK |
          ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BO_EN_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BO_OFFSET_MASK);

    if ((config->mode) == kPMU_LowPowerMode)
    {
        regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_LP_EN_MASK;
    }
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_PULL_DOWN_2MA_EN(config->enable2mALoad);
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_ALWAYS_4MA_PULLDOWN_EN(config->enable4mALoad);
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_PULL_DOWN_20UA_EN(config->enable20uALoad);
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_STANDBY_EN(config->enableStandbyMode);
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BYPASS_PRECHRG_CURRENT_CFG(config->driverStrength);
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_ICP_TRIM_SNVS(config->chargePumpCurrent);
    regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_TRIM(config->outputRange);
    if (config->brownOutDetectorConfig != kPMU_LpsrAnaLdoBrownOutDetectorDisable)
    {
        regValue |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BO_EN_MASK |
                    ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_BO_OFFSET(config->brownOutDetectorConfig);
    }
    base->PMU_LDO_LPSR_ANA = regValue;

    /* Enable LPSR ANA DIG. */
    base->PMU_LDO_LPSR_ANA &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK;
}

/*!
 * brief Disable the output of LPSR ANA LDO.
 *
 * param base ANADIG_LDO_SNVS peripheral base address.
 */
void PMU_StaticLpsrAnaLdoDeinit(ANADIG_LDO_SNVS_Type *base)
{
    /* Disable LPSR ANA LDO. */
    base->PMU_LDO_LPSR_ANA |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_ANA_REG_DISABLE_MASK;
}

/*!
 * brief Selects the control mode of the LPSR DIG LDO.
 *
 * param base ANADIG_LDO_SNVS peripheral base address.
 * param mode The control mode of the LPSR DIG LDO. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetLpsrDigLdoControlMode(ANADIG_LDO_SNVS_Type *base, pmu_control_mode_t mode)
{
    if (mode == kPMU_StaticMode)
    {
        base->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_LPSR_DIG_CONTROL_MODE_MASK;
    }
    else
    {
        base->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_LPSR_DIG_CONTROL_MODE_MASK;
    }
}

/*!
 * brief Turn on/off Bypass mode of the LPSR DIG LDO in Static/Software mode.
 *
 * param base ANADIG_LDO_SNVS peripheral base address.
 * param enable
 *              true    -   Turn on Bypass mode of the LPSR DIG LDO.
 *              false   -   Turn off Bypass mode of the LPSR DIG LDO.
 */
void PMU_StaticEnableLpsrDigLdoBypassMode(ANADIG_LDO_SNVS_Type *base, bool enable)
{
    if (enable)
    {
        /* HP mode */
        base->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* tracking */
        base->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRACKING_MODE_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* set BYPASS */
        base->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable LPSR DIG LDO */
        base->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK;
    }
    else
    {
        /* Enable LPSR DIG LDO and HP mode */
        base->PMU_LDO_LPSR_DIG |=
            (ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK);
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Clear BYPASS */
        base->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BYPASS_MODE_MASK;
        SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Disable tracking */
        base->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRACKING_MODE_MASK;
    }
}

/*!
 * @brief Gets the default configuration of LPSR DIG LDO.
 *
 * @param config Pointer to the structure pmu_static_lpsr_dig_config_t. Please refer to @ref
 * pmu_static_lpsr_dig_config_t.
 */
void PMU_StaticGetLpsrDigLdoDefaultConfig(pmu_static_lpsr_dig_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->enableStableDetect = false;
    config->voltageStepTime    = kPMU_LpsrDigVoltageStepInc50us;
    config->brownOutConfig     = kPMU_LpsrDigBrownOutDisable;
    config->targetVoltage      = kPMU_LpsrDigTargetStableVoltage1P0V;
    config->mode               = kPMU_HighPowerMode;
}

/*!
 * @brief Initialize the LPSR DIG LDO in static mode.
 *
 * @param base ANADIG_LDO_SNVS peripheral base address.
 * @param config Pointer to the structure pmu_static_lpsr_dig_config_t. Please refer to @ref
 * pmu_static_lpsr_dig_config_t.
 */
void PMU_StaticLpsrDigLdoInit(ANADIG_LDO_SNVS_Type *base, const pmu_static_lpsr_dig_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32 = base->PMU_LDO_LPSR_DIG;

    temp32 &= ~(ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TEST_OVERRIDE_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BO_OFFSET_MASK |
                ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_RBB_STABLE_DETECT_MASK | ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRIM_MASK |
                ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN_MASK);

    if (config->brownOutConfig != kPMU_LpsrDigBrownOutDisable)
    {
        temp32 |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TEST_OVERRIDE_MASK;
        temp32 |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_BO_OFFSET(config->brownOutConfig);
    }
    temp32 |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_RBB_STABLE_DETECT(config->enableStableDetect);
    temp32 |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_TRIM(config->targetVoltage);
    temp32 |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_HP_EN(config->mode);
    base->PMU_LDO_LPSR_DIG = temp32;

    temp32 = base->PMU_LDO_LPSR_DIG_2;
    temp32 &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_2_VOLTAGE_STEP_INC_MASK;
    temp32 |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_2_VOLTAGE_STEP_INC(config->voltageStepTime);
    base->PMU_LDO_LPSR_DIG_2 = temp32;

    /* Enable LPSR DIG LDO. */
    base->PMU_LDO_LPSR_DIG |= ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK;
    SDK_DelayAtLeastUs(125U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PMU_POWER_DETECT_CTRL_REGISTER |= ANADIG_PMU_PMU_POWER_DETECT_CTRL_CKGB_LPSR1P0_MASK;
}

/*!
 * @brief Disable the LPSR DIG LDO.
 *
 * @param base ANADIG_LDO_SNVS peripheral base address.
 */
void PMU_StaticLpsrDigLdoDeinit(ANADIG_LDO_SNVS_Type *base)
{
    PMU_POWER_DETECT_CTRL_REGISTER &= ~ANADIG_PMU_PMU_POWER_DETECT_CTRL_CKGB_LPSR1P0_MASK;
    base->PMU_LDO_LPSR_DIG &= ~ANADIG_LDO_SNVS_PMU_LDO_LPSR_DIG_REG_EN_MASK;
}

/*!
 * brief Sets the voltage step of LPSR DIG LDO in the certain setpoint during GPC mode.
 *
 * note The function provides the feature to set the voltage step to the different setpoints.
 *
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param voltageStep The voltage step to be set.
 */
void PMU_GPCSetLpsrDigLdoTargetVoltage(uint32_t setpointMap, pmu_lpsr_dig_target_output_voltage_t voltageValue)
{
    uint32_t regValue             = 0UL;
    uint32_t lpsrDigTrgRegArray[] = PMU_LPSR_DIG_TRG_REGISTERS;
    uint8_t regIndex;
    uint8_t temp8;
    uint32_t i;

    for (regIndex = 0U; regIndex < ARRAY_SIZE(lpsrDigTrgRegArray); regIndex++)
    {
        temp8 = (((uint8_t)(setpointMap >> (PMU_LDO_LPSR_DIG_TRG_SPX_REG_SETPOINT_COUNTS * regIndex))) & 0xFU);
        if (temp8 != 0UL)
        {
            regValue = (*(volatile uint32_t *)lpsrDigTrgRegArray[regIndex]);
            for (i = 0U; i < PMU_LDO_LPSR_DIG_TRG_SPX_REG_SETPOINT_COUNTS; i++)
            {
                if (((temp8 >> (1U * i)) & 0x1U) != 0U)
                {
                    regValue &= ~(0xFFUL << (PMU_LDO_LPSR_DIG_TRG_SPX_VOLTAGE_SETPOINTX_BIT_WIDTH * i));
                    regValue |= voltageValue << (PMU_LDO_LPSR_DIG_TRG_SPX_VOLTAGE_SETPOINTX_BIT_WIDTH * i);
                }
            }
            (*(volatile uint32_t *)lpsrDigTrgRegArray[regIndex]) = regValue;
        }
    }
}

/*!
 * brief Gets the default config of the SNVS DIG LDO.
 *
 * The default values are:
 *  code
 *      config->mode                   = kPMU_LowPowerMode;
 *      config->chargePumpCurrent      = kPMU_SnvsDigChargePump12P5nA;
 *      config->dischargeResistorValue = kPMU_SnvsDigDischargeResistor15K;
 *      config->trimValue              = 0U;
 *      config->enablePullDown         = true;
 *      config->enableLdoStable        = false;
 *  endcode
 *
 * param config Pointer to the structure pmu_snvs_dig_config_t. Please refer to @ref pmu_snvs_dig_config_t.
 */
void PMU_GetSnvsDigLdoDefaultConfig(pmu_snvs_dig_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->mode                   = kPMU_LowPowerMode;
    config->chargePumpCurrent      = kPMU_SnvsDigChargePump12P5nA;
    config->dischargeResistorValue = kPMU_SnvsDigDischargeResistor15K;
    config->trimValue              = 0U;
    config->enablePullDown         = true;
    config->enableLdoStable        = false;
}

/*!
 * brief Initialize the SNVS DIG LDO.
 *
 * param base LDO SNVS DIG peripheral base address.
 * param config Pointer to the structure pmu_snvs_dig_config_t. Please refer to @ref pmu_snvs_dig_config_t.
 */
void PMU_SnvsDigLdoInit(ANADIG_LDO_SNVS_DIG_Type *base, const pmu_snvs_dig_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32 = base->PMU_LDO_SNVS_DIG;

    temp32 &=
        ~(ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_ENB_PULLDOWN_MASK | ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_REG_LP_EN_MASK |
          ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_CP_CONFIG_MASK | ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_RES_CONFIG_MASK |
          ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_TRIM_MASK | ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_REG_STABLE_MASK);

    if (!(config->enablePullDown))
    {
        temp32 |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_ENB_PULLDOWN_MASK;
    }
    temp32 |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_REG_LP_EN(config->mode);
    temp32 |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_CP_CONFIG(config->chargePumpCurrent);
    temp32 |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_RES_CONFIG(config->dischargeResistorValue);
    temp32 |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_TRIM(config->trimValue);
    temp32 |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_REG_STABLE(config->enableLdoStable);
    base->PMU_LDO_SNVS_DIG = temp32;

    /* Enable SNVS DIG LDO. */
    base->PMU_LDO_SNVS_DIG |= ANADIG_LDO_SNVS_DIG_PMU_LDO_SNVS_DIG_REG_EN_MASK;
}

/*!
 * brief  Controls the ON/OFF of the selected LDO in the certain setpoints with GPC mode.
 *
 * param name The name of the selected ldo. Please see the enumeration pmu_ldo_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param enable Turn on/off the LDO.
 *                true  -  Turn on the selected LDO in the certain setpoints.
 *                false -  Turn off the selected LDO in the certain setpoints.
 */
void PMU_GPCEnableLdo(pmu_ldo_name_t name, uint32_t setpointMap, bool enable)
{
    assert((name == kPMU_PllLdo) || (name > kPMU_PllLdo));
    assert(name < kPMU_SnvsDigLdo);

    uint32_t ldoEnableRegArray[] = PMU_LDO_ENABLE_SETPOINT_REGISTERS;

    if (enable)
    {
        (*(volatile uint32_t *)ldoEnableRegArray[(uint8_t)name]) &= ~setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)ldoEnableRegArray[(uint8_t)name]) |= setpointMap;
    }
}

/*!
 * brief Sets the operating mode of the selected LDO in the certain setpoints with GPC mode.
 *
 * param name The name of the selected ldo. Please see the enumeration @ref pmu_ldo_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param mode The operating mode of the selected ldo. Please refer to the enumeration @ref pmu_ldo_operate_mode_t for
 * details.
 */
void PMU_GPCSetLdoOperateMode(pmu_ldo_name_t name, uint32_t setpointMap, pmu_ldo_operate_mode_t mode)
{
    assert(name > kPMU_PllLdo);
    assert(name < kPMU_SnvsDigLdo);

    uint32_t ldoLpModeRegArray[] = PMU_LDO_LP_MODE_EN_SETPOINT_REGISTERS;

    if (mode == kPMU_LowPowerMode)
    {
        (*(volatile uint32_t *)ldoLpModeRegArray[(uint8_t)name]) &= ~setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)ldoLpModeRegArray[(uint8_t)name]) |= setpointMap;
    }
}

/*!
 * brief Controls the ON/OFF of the selected LDOs' Tracking mode in the certain setpoints with GPC mode.
 *
 * param name The name of the selected ldo. Please see the enumeration @ref pmu_ldo_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param enable Turn on/off the LDOs' Tracking mode.
 *                  true    -   Turn on the selected LDO's tracking mode in the certain setpoints.
 *                  false   -   Turn off the selected LDO's tracking mode in the certain setpoints.
 */
void PMU_GPCEnableLdoTrackingMode(pmu_ldo_name_t name, uint32_t setpointMap, bool enable)
{
    assert(name > kPMU_PllLdo);
    assert(name < kPMU_SnvsDigLdo);

    uint32_t ldoTrackingEnableRegArray[] = PMU_LDO_TRACKING_EN_SETPOINT_REGISTERS;

    if (enable)
    {
        (*(volatile uint32_t *)ldoTrackingEnableRegArray[(uint8_t)name]) |= setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)ldoTrackingEnableRegArray[(uint8_t)name]) &= ~setpointMap;
    }
}

/*!
 * brief Controls the ON/OFF of the selected LDOs' Bypass mode in the certain setpoints with GPC mode.
 *
 * param name The name of the selected ldo. Please see the enumeration @ref pmu_ldo_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param enable Turn on/off the LDOs' Bypass mode.
 *                  true    -   Turn on the selected LDO's Bypass mode in the certain setpoints.
 *                  false   -   Turn off the selected LDO's Bypass mode in the certain setpoints.
 */
void PMU_GPCEnableLdoBypassMode(pmu_ldo_name_t name, uint32_t setpointMap, bool enable)
{
    assert(name > kPMU_PllLdo);
    assert(name < kPMU_SnvsDigLdo);

    uint32_t ldoBypassEnableRegArray[] = PMU_LDO_BYPASS_EN_SETPOINT_REGISTERS;

    if (enable)
    {
        (*(volatile uint32_t *)ldoBypassEnableRegArray[(uint8_t)name]) |= setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)ldoBypassEnableRegArray[(uint8_t)name]) &= ~setpointMap;
    }
}

/*!
 * brief Controls the ON/OFF of the selected LDOs' Standby mode in the certain setpoints with GPC mode.
 *
 * param name The name of the selected ldo. Please see the enumeration @ref pmu_ldo_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param enable Turn on/off the LDOs' Standby mode.
 *                  true    -   Turn on the selected LDO's Standby mode in the certain setpoints.
 *                  false   -   Turn off the selected LDO's Standby mode in the certain setpoints.
 */
void PMU_GPCEnableLdoStandbyMode(pmu_ldo_name_t name, uint32_t setpointMap, bool enable)
{
    assert((name == kPMU_PllLdo) || (name > kPMU_PllLdo));
    assert(name < kPMU_SnvsDigLdo);

    uint32_t ldoStandbyEnableRegArray[] = PMU_LDO_STBY_EN_REGISTERS;

    if (enable)
    {
        (*(volatile uint32_t *)ldoStandbyEnableRegArray[(uint8_t)name]) |= setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)ldoStandbyEnableRegArray[(uint8_t)name]) &= ~setpointMap;
    }
}

/*!
 * brief Selects the control mode of the Bandgap Reference.
 *
 * param base PMU peripheral base address.
 * param mode The control mode of the Bandgap Reference. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetBandgapControlMode(ANADIG_PMU_Type *base, pmu_control_mode_t mode)
{
    if (mode == kPMU_StaticMode)
    {
        base->PMU_REF_CTRL &= ~ANADIG_PMU_PMU_REF_CTRL_REF_CONTROL_MODE_MASK;
    }
    else
    {
        base->PMU_REF_CTRL |= ANADIG_PMU_PMU_REF_CTRL_REF_CONTROL_MODE_MASK;
    }
}

/*!
 * brief Switches the Bandgap from Static/Software Mode to GPC/Hardware Mode.
 *
 * param base PMU peripheral base address.
 */
void PMU_SwitchBandgapToGPCMode(ANADIG_PMU_Type *base)
{
    if ((base->BANDGAP_ENABLE_SP & ANADIG_PMU_BANDGAP_ENABLE_SP_ON_OFF_SETPOINT0_MASK) == 0UL)
    {
        base->PMU_REF_CTRL &= ~ANADIG_PMU_PMU_REF_CTRL_REF_ENABLE_MASK;
    }
    else
    {
        base->PMU_REF_CTRL |= ANADIG_PMU_PMU_REF_CTRL_REF_ENABLE_MASK;
    }
}

/*!
 * brief Disables Bandgap self bias for best noise performance.
 *
 * This function waits for the bandgap to be stable and disables the bandgap self bias.
 * After being powered up, it needs to wait for the bandgap stable to be stable and then disable Bandgap
 * Self bias for best noise performance.
 */
void PMU_DisableBandgapSelfBiasAfterPowerUp(void)
{
    uint32_t temp32;
    uint32_t regValue;

    /* Wait Bandgap stable. */
    do
    {
        regValue = ANATOP_AI_Read(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_STATUS_REGISTER_ADDRESS);
    } while ((regValue & PMU_BANDGAP_ANALOG_STATUS_REFTOP_VBGUP_MASK) == 0UL);

    /* Disable Bandgap self bias for best noise performance. */
    temp32 = ANATOP_AI_Read(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS);
    temp32 |= PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF_MASK;
    ANATOP_AI_Write(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS, temp32);
}

/*!
 * brief Enables Bandgap self bias before power down.
 *
 * This function will enable Bandgap self bias feature before powering down or there
 * will be risk of Bandgap not starting properly.
 */
void PMU_EnableBandgapSelfBiasBeforePowerDown(void)
{
    uint32_t temp32;

    temp32 = ANATOP_AI_Read(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS);
    temp32 &= ~PMU_BANDGAP_ANALOG_CTRL0_REFTOP_SELFBIASOFF_MASK;
    ANATOP_AI_Write(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS, temp32);
}

/*!
 * brief Init Bandgap.
 *
 * param config. Pointer to the structure pmu_static_bandgap_config_t. Please refer to @ref pmu_static_bandgap_config_t.
 */
void PMU_StaticBandgapInit(const pmu_static_bandgap_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32;

    temp32 = ANATOP_AI_Read(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS);
    temp32 &= ~(PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION_MASK | PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER_MASK |
                PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ_MASK | PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ_MASK);
    temp32 |= PMU_BANDGAP_ANALOG_CTRL0_REFTOP_PWD_OPTION(config->powerDownOption);
    temp32 |= PMU_BANDGAP_ANALOG_CTRL0_REFTOP_LOWPOWER(config->enableLowPowerMode);
    temp32 |= PMU_BANDGAP_ANALOG_CTRL0_REFTOP_VBGADJ(config->outputVoltage);
    temp32 |= PMU_BANDGAP_ANALOG_CTRL0_REFTOP_IBZTCADJ(config->outputCurrent);

    ANATOP_AI_Write(kAI_Itf_Bandgap, PMU_BANDGAP_ANALOG_CTRL0_REGISTER_ADDRESS, temp32);
}

/*!
 * brief Selects the control mode of the Body Bias.
 *
 * param base PMU peripheral base address.
 * param name The name of the body bias. Please refer to @ref pmu_body_bias_name_t.
 * param mode The control mode of the Body Bias. Please refer to @ref pmu_control_mode_t.
 */
void PMU_SetBodyBiasControlMode(ANADIG_PMU_Type *base, pmu_body_bias_name_t name, pmu_control_mode_t mode)
{
    uint32_t temp32;

    switch (name)
    {
        case kPMU_FBB_CM7:
        {
            temp32 = base->PMU_BIAS_CTRL2;
            temp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_FBB_M7_CONTROL_MODE_MASK;
            temp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_FBB_M7_CONTROL_MODE(mode);
            base->PMU_BIAS_CTRL2 = temp32;
            break;
        }
        case kPMU_RBB_SOC:
        {
            temp32 = base->PMU_BIAS_CTRL2;
            temp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_RBB_SOC_CONTROL_MODE_MASK;
            temp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_RBB_SOC_CONTROL_MODE(mode);
            base->PMU_BIAS_CTRL2 = temp32;
            break;
        }
        case kPMU_RBB_LPSR:
        {
            temp32 = base->PMU_BIAS_CTRL2;
            temp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_RBB_LPSR_CONTROL_MODE_MASK;
            temp32 |= ANADIG_PMU_PMU_BIAS_CTRL2_RBB_LPSR_CONTROL_MODE(mode);
            base->PMU_BIAS_CTRL2 = temp32;
            break;
        }
        default:
            assert(false);
            break;
    }
}

/*!
 * brief Gets the default config of CM7 Forward Body Bias in static mode.
 *
 * param config Pointer to the structure pmu_static_body_bias_config_t. Please refer to @ref
 * pmu_static_body_bias_config_t.
 */
void PMU_StaticGetCm7FBBDefaultConfig(pmu_static_body_bias_config_t *config)
{
    assert(config != NULL);

    (void)memset(config, 0, sizeof(*config));

    config->voltageLevel   = kPMU_BodyBiasWellRegulateTo0P6V;
    config->driveStrength  = 0x3U;
    config->oscillatorFreq = 0xFU;
}

/*!
 * brief Initialize CM7 Forward Body Bias in Static/Software Mode.
 *
 * param base PMU peripheral base address.
 * param config Pointer to the structure pmu_static_body_bias_config_t. Please refer to @ref
 * pmu_static_body_bias_config_t.
 */
void PMU_StaticCm7FBBInit(ANADIG_PMU_Type *base, const pmu_static_body_bias_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32;
    uint32_t wellBiasConfig;

    base->PMU_BIAS_CTRL &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8_MASK;
    base->PMU_BIAS_CTRL |= ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8(config->voltageLevel);
    base->PMU_BIAS_CTRL &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8_MASK;
    base->PMU_BIAS_CTRL |= ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8(config->voltageLevel);
    temp32 = base->PMU_BIAS_CTRL;
    temp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8_MASK;
    wellBiasConfig = PMU_BIAS_CTRL_WB_CFG_1P8_VOLTAGE_THRESHOLD_MASK |
                     PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH(config->driveStrength) |
                     PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ(config->oscillatorFreq);
    temp32 |= ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8(wellBiasConfig);
    base->PMU_BIAS_CTRL = temp32;
    base->PMU_BIAS_CTRL2 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK;
    base->PMU_BIAS_CTRL2 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(0x1U);
    base->PMU_BIAS_CTRL2 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

/*!
 * brief Gets the default config of LPSR Reverse Body Bias in Static/Software mode.
 *
 * param config Pointer to the structure pmu_static_body_bias_config_t. Please refer to @ref
 * pmu_static_body_bias_config_t.
 */
void PMU_StaticLpsrRBBDefaultConfig(pmu_static_body_bias_config_t *config)
{
    assert(config != NULL);

    config->voltageLevel   = kPMU_BodyBiasWellRegulateTo1P0V;
    config->driveStrength  = 0x5U;
    config->oscillatorFreq = 0xFU;
}

/*!
 * brief Initialize LPSR Reverse Body Bias in Static/Software Mode.
 *
 * param base PMU peripheral base address.
 * param config Pointer to the structure pmu_static_body_bias_config_t. Please refer to @ref
 * pmu_static_body_bias_config_t.
 */
void PMU_StaticLpsrRBBInit(ANADIG_PMU_Type *base, const pmu_static_body_bias_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32;
    uint32_t wellBiasConfig;

    base->PMU_BIAS_CTRL &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8_MASK;
    base->PMU_BIAS_CTRL |= ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8(config->voltageLevel);
    base->PMU_BIAS_CTRL &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8_MASK;
    base->PMU_BIAS_CTRL |= ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8(config->voltageLevel);
    temp32 = base->PMU_BIAS_CTRL;
    temp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8_MASK;
    wellBiasConfig = PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH(config->driveStrength) |
                     PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ(config->oscillatorFreq);
    temp32 |= ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8(wellBiasConfig);
    base->PMU_BIAS_CTRL = temp32;
    base->PMU_BIAS_CTRL2 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK;
    base->PMU_BIAS_CTRL2 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(0x2U);
    base->PMU_BIAS_CTRL2 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

/*!
 * brief Gets the default config of SOC Reverse Body Bias in Static/Software mode.
 *
 * param config Pointer to the structure pmu_static_body_bias_config_t. Please refer to @ref
 * pmu_static_body_bias_config_t.
 */
void PMU_StaticSocRBBDefaultConfig(pmu_static_body_bias_config_t *config)
{
    assert(config != NULL);

    config->voltageLevel   = kPMU_BodyBiasWellRegulateTo1P0V;
    config->driveStrength  = 0x1U;
    config->oscillatorFreq = 0xFU;
}

/*!
 * brief Initialize SOC Reverse Body Bias in Static/Software Mode.
 *
 * param base PMU peripheral base address.
 * param config Pointer to the structure pmu_static_body_bias_config_t. Please refer to @ref
 * pmu_static_body_bias_config_t.
 */
void PMU_StaticSocRBBInit(ANADIG_PMU_Type *base, const pmu_static_body_bias_config_t *config)
{
    assert(config != NULL);

    uint32_t temp32;
    uint32_t wellBiasConfig;

    base->PMU_BIAS_CTRL &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8_MASK;
    base->PMU_BIAS_CTRL |= ANADIG_PMU_PMU_BIAS_CTRL_WB_NW_LVL_1P8(config->voltageLevel);
    base->PMU_BIAS_CTRL &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8_MASK;
    base->PMU_BIAS_CTRL |= ANADIG_PMU_PMU_BIAS_CTRL_WB_PW_LVL_1P8(config->voltageLevel);
    temp32 = base->PMU_BIAS_CTRL;
    temp32 &= ~ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8_MASK;
    wellBiasConfig = PMU_BIAS_CTRL_WB_CFG_1P8_DRIVE_STRENGTH(config->driveStrength) |
                     PMU_BIAS_CTRL_WB_CFG_1P8_OSCILLATOR_FREQ(config->oscillatorFreq);
    temp32 |= ANADIG_PMU_PMU_BIAS_CTRL_WB_CFG_1P8(wellBiasConfig);
    base->PMU_BIAS_CTRL = temp32;
    base->PMU_BIAS_CTRL2 &= ~ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8_MASK;
    base->PMU_BIAS_CTRL2 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_PWR_SW_EN_1P8(0x4U);
    base->PMU_BIAS_CTRL2 |= ANADIG_PMU_PMU_BIAS_CTRL2_WB_EN_MASK;
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
}

/*!
 * brief  Controls the ON/OFF of the selected body bias in the certain setpoints with GPC mode.
 *
 * param name The name of the selected body bias. Please see the enumeration @ref pmu_body_bias_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param enable Turn on/off the LDO.
 *                true  -  Turn on the selected body bias in the certain setpoints.
 *                false -  Turn off the selected body bias in the certain setpoints.
 */
void PMU_GPCEnableBodyBias(pmu_body_bias_name_t name, uint32_t setpointMap, bool enable)
{
    uint32_t bodyBiasEnableRegArray[] = PMU_BODY_BIAS_ENABLE_REGISTERS;

    if (enable)
    {
        (*(volatile uint32_t *)bodyBiasEnableRegArray[(uint8_t)name]) &= ~setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)bodyBiasEnableRegArray[(uint8_t)name]) |= setpointMap;
    }
}

/*!
 * brief Controls the ON/OFF of the selected Body Bias' Standby mode in the certain setpoints with GPC mode.
 *
 * param name The name of the selected body bias. Please see the enumeration @ref pmu_body_bias_name_t for details.
 * param setpointMap The map of setpoints should be the OR'ed Value of _pmu_setpoint_map.
 * param enable Turn on/off the body bias' Standby mode.
 *                  true    -   Turn on the selected body bias' Standby mode in the certain setpoints.
 *                  false   -   Turn off the selected body bias' Standby mode in the certain setpoints.
 */
void PMU_GPCEnableBodyBiasStandbyMode(pmu_body_bias_name_t name, uint32_t setpointMap, bool enable)
{
    uint32_t BBStandbyEnableRegArray[] = PMU_BODY_BIAS_STBY_EN_REGISTERS;

    if (enable)
    {
        (*(volatile uint32_t *)BBStandbyEnableRegArray[(uint8_t)name]) |= setpointMap;
    }
    else
    {
        (*(volatile uint32_t *)BBStandbyEnableRegArray[(uint8_t)name]) &= ~setpointMap;
    }
}

/*!
 * brief Gets the default config of body bias in GPC mode.
 *
 * param config Pointer to the structure @ref pmu_gpc_body_bias_config_t.
 */
void PMU_GPCGetBodyBiasDefaultConfig(pmu_gpc_body_bias_config_t *config)
{
    assert(config != NULL);

    config->PWELLRegulatorSize = 1U;
    config->NWELLRegulatorSize = 1U;
    config->oscillatorSize     = 7U;
    config->regulatorStrength  = 5U;
}

/*!
 * brief Sets the config of the selected Body Bias in GPC mode.
 *
 * param name The name of the selected body bias. Please see the enumeration @ref pmu_body_bias_name_t for details.
 * param config Pointer to the structure @ref pmu_gpc_body_bias_config_t.
 */
void PMU_GPCSetBodyBiasConfig(pmu_body_bias_name_t name, const pmu_gpc_body_bias_config_t *config)
{
    assert(config != NULL);

    uint32_t bodyBiasConfigRegArray[] = PMU_BODY_BIAS_CONFIGURE_REGISTERS;
    uint32_t temp32;

    temp32 = (*(volatile uint32_t *)bodyBiasConfigRegArray[(uint8_t)name]);
    temp32 &= ~(ANADIG_PMU_FBB_M7_CONFIGURE_WB_CFG_PW_MASK | ANADIG_PMU_FBB_M7_CONFIGURE_WB_CFG_NW_MASK |
                ANADIG_PMU_FBB_M7_CONFIGURE_OSCILLATOR_BITS_MASK | ANADIG_PMU_FBB_M7_CONFIGURE_REGULATOR_STRENGTH_MASK);
    temp32 |= ANADIG_PMU_FBB_M7_CONFIGURE_WB_CFG_PW(config->PWELLRegulatorSize) |
              ANADIG_PMU_FBB_M7_CONFIGURE_WB_CFG_NW(config->NWELLRegulatorSize) |
              ANADIG_PMU_FBB_M7_CONFIGURE_OSCILLATOR_BITS(config->oscillatorSize) |
              ANADIG_PMU_FBB_M7_CONFIGURE_REGULATOR_STRENGTH(config->regulatorStrength);
    (*(volatile uint32_t *)bodyBiasConfigRegArray[(uint8_t)name]) = temp32;
}
