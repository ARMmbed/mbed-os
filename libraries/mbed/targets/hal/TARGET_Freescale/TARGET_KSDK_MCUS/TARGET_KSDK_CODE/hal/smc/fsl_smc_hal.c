/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

#include "fsl_smc_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_config_power_mode_protection
 * Description   : Config all power mode protection settings
 * This function will configure the power mode protection settings for
 * supported power mode on the specified chip family. The availabe power modes
 * are defined in smc_power_mode_protection_config_t. Application should provide
 * the protect settings for all supported power mode on the chip and aslo this
 * should be done at early system level init stage. Refer to reference manual
 * for details. This register can only write once after power reset. So either
 * use this function or use the individual set function if you only have single
 * option to set.
 * 
 *END**************************************************************************/
void smc_hal_config_power_mode_protection(smc_power_mode_protection_config_t *protectConfig)
{
    /* initialize the setting */
    uint8_t regValue = 0;

    /* check configurations for each mode and combine the seting together */
    if (protectConfig->vlpProt)
    {
        regValue |= BF_SMC_PMPROT_AVLP(1);
    }

    if (protectConfig->llsProt)
    {
        regValue |= BF_SMC_PMPROT_ALLS(1);
    }

    if (protectConfig->vllsProt)
    {
        regValue |= BF_SMC_PMPROT_AVLLS(1);
    }

    /* write once into pmprot register*/
    HW_SMC_PMPROT_SET(regValue);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_set_power_mode_protection
 * Description   : Config the individual power mode protection setting
 * This function will only configure the power mode protection settings for
 * a specified power mode on the specified chip family. The availabe power modes
 * are defined in smc_power_mode_protection_config_t. Refer to reference manual
 * for details. This register can only write once after power reset.
 * 
 *END**************************************************************************/
void smc_hal_set_power_mode_protection(power_modes_protect_t protect, bool allow)
{
    /* check the setting range */
    assert(protect < kAllowMax);

    /* branch according to mode and write the setting */
    switch (protect)
    {
    case kAllowVlp:
        if (allow) 
        {
            BW_SMC_PMPROT_AVLP(1);
        }
        else
        {
            BW_SMC_PMPROT_AVLP(0);
        }
        break;
    case kAllowLls:
        if (allow) 
        {
            BW_SMC_PMPROT_ALLS(1);
        }
        else
        {
            BW_SMC_PMPROT_ALLS(0);
        }
        break;
    case kAllowVlls:
        if (allow) 
        {
            BW_SMC_PMPROT_AVLLS(1);
        }
        else
        {
            BW_SMC_PMPROT_AVLLS(0);
        }
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_power_mode_protection
 * Description   : Get the current power mode protection setting
 * This function will get the current power mode protection settings for
 * a specified power mode.
 * 
 *END**************************************************************************/
bool smc_hal_get_power_mode_protection(power_modes_protect_t protect)
{
    /* check the mode range */
    assert(protect < kAllowMax);

    /* branch according to the mode and read the setting */
    switch (protect)
    {
    case kAllowVlp:
        return BR_SMC_PMPROT_AVLP;
    case kAllowLls:
        return BR_SMC_PMPROT_ALLS;
    case kAllowVlls:
        return BR_SMC_PMPROT_AVLLS;
    default:
        break;
    }
    return (false);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_power_mode_config_run
 * Description   : Config the RUN mode control setting
 * This function will set the run mode settings. For example, normal run mode,
 * very lower power run mode, etc. Refer to smc_run_mode_t for supported run
 * mode on the chip family. Refer to reference manual for details about the 
 * run mode.
 * 
 *END**************************************************************************/
void smc_hal_power_mode_config_run(smc_run_mode_t runMode)
{
    BW_SMC_PMCTRL_RUNM(runMode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_power_mode_get_run_config
 * Description   : Get the current RUN mode config
 * This function will get the run mode settings. Refer to smc_run_mode_t 
 * for supported run mode on the chip family. Refer to reference manual for 
 * details about the run mode.
 * 
 *END**************************************************************************/
smc_run_mode_t smc_hal_power_mode_get_run_config(void)
{
    return (smc_run_mode_t)BR_SMC_PMCTRL_RUNM;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_power_mode_config_stop
 * Description   : Config the STOP mode control setting
 * This function will set the stop mode settings. For example, normal stop mode,
 * very lower power stop mode, etc. Refer to smc_stop_mode_t for supported stop
 * mode on the chip family. Refer to reference manual for details about the 
 * stop mode.
 * 
 *END**************************************************************************/
void smc_hal_power_mode_config_stop(smc_stop_mode_t stopMode)
{
    BW_SMC_PMCTRL_STOPM(stopMode);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_power_mode_get_stop_config
 * Description   : Get the current STOP mode control setting
 * This function will get the stop mode settings. For example, normal stop mode,
 * very lower power stop mode, etc. Refer to smc_stop_mode_t for supported stop
 * mode on the chip family. Refer to reference manual for details about the 
 * stop mode.
 * 
 *END**************************************************************************/
smc_stop_mode_t smc_hal_power_mode_get_stop_config(void)
{
    return (smc_stop_mode_t)BR_SMC_PMCTRL_STOPM;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_power_mode_config_stop_submode
 * Description   : Config the stop sub mode control setting
 * This function will set the stop submode settings. Some of the stop mode will
 * further have submode supported. Refer to smc_stop_submode_t for supported
 * stop submode and Refer to reference manual for details about the submode
 * for specific stop mode.
 * 
 *END**************************************************************************/
void smc_hal_power_mode_config_stop_submode(smc_stop_submode_t stopSubMode)
{
#if FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    BW_SMC_VLLSCTRL_VLLSM(stopSubMode);
#else
#if FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM    
    BW_SMC_STOPCTRL_VLLSM(stopSubMode);
#else
    BW_SMC_STOPCTRL_LLSM(stopSubMode);
#endif    
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_power_mode_get_stop_submode_config
 * Description   : Get the current stop submode config 
 * This function will get the stop submode settings. Some of the stop mode will
 * further have submode supported. Refer to smc_stop_submode_t for supported
 * stop submode and Refer to reference manual for details about the submode
 * for specific stop mode.
 * 
 *END**************************************************************************/
smc_stop_submode_t smc_hal_power_mode_get_stop_submode_config(void)
{
#if FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    return (smc_stop_submode_t)BR_SMC_VLLSCTRL_VLLSM;
#else
#if FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM    
    return (smc_stop_submode_t)BR_SMC_STOPCTRL_VLLSM;
#else
    return (smc_stop_submode_t)BR_SMC_STOPCTRL_LLSM;
#endif
#endif
}

#if FSL_FEATURE_SMC_HAS_PORPO
/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_config_por_power_option
 * Description   : Config the POR (power-on-reset) option
 * This function will set the POR power option setting. It controls whether the
 * POR detect circuit (for brown-out detection) is enabled in certain stop mode.
 * The setting will be either enable or disable the above feature when POR 
 * happened. Refer to reference manual for details.
 * 
 *END**************************************************************************/
void smc_hal_config_por_power_option(smc_por_option_t option)
{
#if FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    BW_SMC_VLLSCTRL_PORPO(option);
#else
    BW_SMC_STOPCTRL_PORPO(option);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_por_power_config
 * Description   : Get the config of POR option
 * This function will set the POR power option setting. See config function
 * header for details.
 * 
 *END**************************************************************************/
smc_por_option_t smc_hal_get_por_power_config(void)
{
#if FSL_FEATURE_SMC_USE_VLLSCTRL_REG
    return (smc_por_option_t)BR_SMC_VLLSCTRL_PORPO;
#else
    return (smc_por_option_t)BR_SMC_STOPCTRL_PORPO;
#endif
}
#endif

#if FSL_FEATURE_SMC_HAS_PSTOPO
/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_por_power_config
 * Description   : Config the PSTOPOR (Partial Stop Option)
 * This function will set the PSTOPOR option. It controls whether a Partial 
 * Stop mode is entered when STOPM=STOP. When entering a Partial Stop mode from
 * RUN mode, the PMC, MCG and flash remain fully powered, allowing the device 
 * to wakeup almost instantaneously at the expense of higher power consumption.
 * In PSTOP2, only system clocks are gated allowing peripherals running on bus
 * clock to remain fully functional. In PSTOP1, both system and bus clocks are
 * gated. Refer to smc_pstop_option_t for supported options. Refer to reference
 * manual for details.
 *
 *END**************************************************************************/
void smc_hal_config_pstop_power_option(smc_pstop_option_t option)
{
    BW_SMC_PMCTRL_STOPM(option);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_por_power_config
 * Description   : Get the config of PSTOPO option 
 * This function will get the current PSTOPOR option setting. Refer to config
 * function for more details.
 *
 *END**************************************************************************/
smc_por_option_t smc_hal_get_pstop_power_config(void)
{
    return (smc_por_option_t)BR_SMC_PMCTRL_STOPM;
}
#endif

#if FSL_FEATURE_SMC_HAS_LPOPO
/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_por_power_config
 * Description   : Config the LPO option setting
 * This function will set the LPO option setting. It controls whether the 1kHZ
 * LPO clock is enabled in certain lower power stop modes. Refer to 
 * smc_lpo_option_t for supported options and refer to reference manual for 
 * details about this option.
 *
 *END**************************************************************************/
void smc_hal_config_lpo_power_option(smc_lpo_option_t option)
{
    BW_SMC_STOPCTRL_LPOPO(option);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_por_power_config
 * Description   : Get the config of LPO option
 * This function will get the current LPO option setting. Refer to config 
 * function for details.
 *
 *END**************************************************************************/
smc_por_option_t smc_hal_get_lpo_power_config(void)
{
    return (smc_por_option_t)BR_SMC_STOPCTRL_LPOPO;
}
#endif

#if FSL_FEATURE_SMC_HAS_LPWUI
/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_config_lpwui_option
 * Description   : Config the LPWUI (Low Power Wake Up on interrup) option
 * This function will set the LPWUI option. It will cause the system to exit
 * to normal RUN mode when any active interrupt occurs while in a certain lower
 * power mode. Refer to smc_lpwui_option_t for supported options and refer to 
 * reference manual for more details about this option.
 * 
 *END**************************************************************************/
void smc_hal_config_lpwui_option(smc_lpwui_option_t option)
{
    BW_SMC_PMCTRL_LPWUI(option);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_config_lpwui_option
 * Description   : Get the current LPWUI option
 * This function will get the LPWUI option. Refer to config function for more
 * details.
 * 
 *END**************************************************************************/
smc_lpwui_option_t smc_hal_get_lpwui_config(void)
{
    return (smc_lpwui_option_t)BR_SMC_PMCTRL_LPWUI;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : smc_hal_get_power_mode_stat
 * Description   : Get the current power mode stat
 * This function will return the current power mode stat. Once application is 
 * switching the power mode, it should always check the stat to make sure it 
 * runs into the specified mode or not. Also application will need to check 
 * this mode before switching to certain mode. The system will require that
 * only certain mode could switch to other specific mode. Refer to the 
 * reference manual for details. Refer to _power_mode_stat for the meaning
 * of the power stat
 * 
 *END**************************************************************************/
uint8_t smc_hal_get_power_mode_stat(void)
{
    return BR_SMC_PMSTAT_PMSTAT; 
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

