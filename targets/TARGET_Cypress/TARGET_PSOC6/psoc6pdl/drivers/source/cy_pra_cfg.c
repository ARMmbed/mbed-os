/***************************************************************************//**
* \file cy_pra_cfg.c
* \version 1.0
*
* \brief The source code file for the PRA driver.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
#include "cy_pra_cfg.h"
#include "cy_gpio.h"
#include "cy_device.h"
#include "cy_gpio.h"
#include "cy_wdt.h"

#if defined (CY_DEVICE_SECURE) || defined (CY_DOXYGEN)

#if (CY_CPU_CORTEX_M0P)
    #include "cy_prot.h"
#endif /* (CY_CPU_CORTEX_M0P) */

#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_IloEnable(void);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_IloDisable(void);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_IloHibernateOn(bool hibernateEnable);
__STATIC_INLINE void Cy_PRA_ClkPumpInit(cy_en_clkpump_in_sources_t source, cy_en_clkpump_divide_t divider);
__STATIC_INLINE void Cy_PRA_ClkTimerInit(cy_en_clktimer_in_sources_t source, uint8_t divider);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_PllInit(uint32_t clkPath, const cy_stc_pll_manual_config_t *pllConfig);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_ClkHfInit( uint32_t clkHf, cy_en_clkhf_in_sources_t hfClkPath, cy_en_clkhf_dividers_t divider);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_FllInit(const cy_stc_pra_system_config_t *devConfig);
__STATIC_INLINE void Cy_PRA_ExtClkInit( const cy_stc_pra_system_config_t *devConfig );
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_EcoInit(const cy_stc_pra_system_config_t *devConfig);
#if defined(CY_IP_MXBLESS)
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_AltHfInit(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateAltHf(const cy_stc_pra_system_config_t *devConfig);
#endif
__STATIC_INLINE void Cy_PRA_PiloInit(void);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_ClkLfInit(cy_en_clklf_in_sources_t clkLfSource);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_WcoInit(const cy_stc_pra_system_config_t *devConfig);
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_PowerInit(const cy_stc_pra_system_config_t *devConfig);
static uint32_t Cy_PRA_GetInputPathMuxFrq(cy_en_clkpath_in_sources_t pathMuxSrc, const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_GetInputSourceFreq(uint32_t clkPath, const cy_stc_pra_system_config_t *devConfig, uint32_t *srcFreq );
static uint32_t Cy_PRA_GetClkLfFreq(const cy_stc_pra_system_config_t *devConfig);
static uint32_t Cy_PRA_GetClkBakFreq(const cy_stc_pra_system_config_t *devConfig);
static uint32_t Cy_PRA_GetClkTimerFreq(const cy_stc_pra_system_config_t *devConfig);
static cy_en_clkpath_in_sources_t Cy_PRA_GetInputSourceClock(uint32_t clkPath, const cy_stc_pra_system_config_t *devConfig, cy_en_pra_status_t *status);
static cy_en_pra_status_t Cy_PRA_ValidateECO(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateEXTClk(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateFLL(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidatePLL(const cy_stc_pra_system_config_t *devConfig, uint8_t pll);
static cy_en_pra_status_t Cy_PRA_ValidateAllPLL(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkLf(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkPathMux(cy_en_clkpath_in_sources_t pathSrc, const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkPath(uint32_t clkPath, const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateAllClkPathMux(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkHfFreqDiv(uint32_t outFreqMHz, cy_en_clkhf_dividers_t divider);
static cy_en_pra_status_t Cy_PRA_ValidateClkHFs(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkPump(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkBak(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkFast(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkPeri(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkTimer(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkSlow(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateClkAltSysTick(const cy_stc_pra_system_config_t *devConfig);
static cy_en_pra_status_t Cy_PRA_ValidateSystemConfig(const cy_stc_pra_system_config_t *devConfig);


/*******************************************************************************
* Function Name: Cy_PRA_IloEnable
****************************************************************************//**
*
* Enables ILO Clock
*
* \return
* CY_PRA_STATUS_SUCCESS when success
* CY_PRA_STATUS_ERROR_PROCESSING_ILO when failure
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_IloEnable(void)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_ERROR_PROCESSING_ILO;

    if (Cy_SysClk_IloIsEnabled()) /* No change in ILO configuration */
    {
        retStatus = CY_PRA_STATUS_SUCCESS;
    }
    else
    {
        if (!Cy_WDT_Locked())
        {
            Cy_SysClk_IloEnable();
            retStatus = CY_PRA_STATUS_SUCCESS;
        }
    }
    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_IloDisable
****************************************************************************//**
*
* Disables ILO Clock
*
* \return
* CY_PRA_STATUS_SUCCESS when success
* CY_PRA_STATUS_ERROR_PROCESSING_ILO when failure
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_IloDisable(void)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_ERROR_PROCESSING_ILO;

    if (!Cy_SysClk_IloIsEnabled()) /* No change in ILO configuration */
    {
        retStatus = CY_PRA_STATUS_SUCCESS;
    }
    else
    {
        if (!Cy_WDT_Locked())
        {
            if (CY_SYSCLK_SUCCESS == Cy_SysClk_IloDisable())
            {
                retStatus = CY_PRA_STATUS_SUCCESS;
            }
        }
    }
    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_IloHibernateOn
****************************************************************************//**
*
* Controls whether the ILO stays on during a hibernate, or through an XRES or
* brown-out detect (BOD) event.
*
* \param hibernateEnable
* true = ILO stays on during hibernate or across XRES/BOD. \n
* false = ILO turns off for hibernate or XRES/BOD.
*
* \return
* CY_PRA_STATUS_SUCCESS when success
* CY_PRA_STATUS_ERROR_PROCESSING_ILO when failure
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_IloHibernateOn(bool hibernateEnable)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_ERROR_PROCESSING_ILO;
    static bool hibernateOn = false; /* default hibernate state is false */

    if (hibernateOn == hibernateEnable) /* No change in ILO configuration */
    {
        retStatus = CY_PRA_STATUS_SUCCESS;
    }
    else
    {
        if (!Cy_WDT_Locked())
        {
            Cy_SysClk_IloHibernateOn(hibernateEnable);
            hibernateOn = hibernateEnable;
            retStatus = CY_PRA_STATUS_SUCCESS;
        }
    }
    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ClkPumpInit
****************************************************************************//**
*
* Initializes PUMP Clock
*
* \param source \ref cy_en_clkpump_in_sources_t
* \param divider \ref cy_en_clkpump_divide_t
*
*******************************************************************************/
__STATIC_INLINE void Cy_PRA_ClkPumpInit(cy_en_clkpump_in_sources_t source, cy_en_clkpump_divide_t divider)
{
    Cy_SysClk_ClkPumpDisable();
    Cy_SysClk_ClkPumpSetSource(source);
    Cy_SysClk_ClkPumpSetDivider(divider);
    Cy_SysClk_ClkPumpEnable();
}


/*******************************************************************************
* Function Name: Cy_PRA_ClkTimerInit
****************************************************************************//**
*
* Initializes Timer Clock
*
* \param source \ref cy_en_clktimer_in_sources_t
* \param divider Divider value; valid range is 0 to 255. Divides the selected
* source (\ref Cy_SysClk_ClkTimerSetSource) by the (value + 1).
*
*******************************************************************************/
__STATIC_INLINE void Cy_PRA_ClkTimerInit(cy_en_clktimer_in_sources_t source, uint8_t divider)
{
    Cy_SysClk_ClkTimerDisable();
    Cy_SysClk_ClkTimerSetSource(source);
    Cy_SysClk_ClkTimerSetDivider(divider);
    Cy_SysClk_ClkTimerEnable();
}


/*******************************************************************************
* Function Name: Cy_PRA_PllInit
****************************************************************************//**
*
* Initializes Phase Locked Loop
*
* \param clkPath Selects which PLL to configure. 1 is the first PLL; 0 is invalid.
* \param pllConfig \ref cy_stc_pll_manual_config_t
*
* \return
* CY_PRA_STATUS_SUCCESS PLL init is success
* CY_PRA_STATUS_ERROR_PROCESSING PLL init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_PllInit(uint32_t clkPath, const cy_stc_pll_manual_config_t *pllConfig)
{
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_PllManualConfigure(clkPath, pllConfig))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING;
    }
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_PllEnable(clkPath, 10000u))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING;
    }

    return CY_PRA_STATUS_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_PRA_ClkHfInit
****************************************************************************//**
*
* Initializes High Frequency Clock
*
* \param clkHf selects which clkHf mux to configure.
* \param hfClkPath \ref cy_en_clkhf_in_sources_t
* \param divider \ref cy_en_clkhf_dividers_t
*
* \return
* CY_PRA_STATUS_SUCCESS CLK_HF init is success
* CY_PRA_STATUS_ERROR_PROCESSING CLK_HF init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_ClkHfInit( uint32_t clkHf, cy_en_clkhf_in_sources_t hfClkPath, cy_en_clkhf_dividers_t divider)
{
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_ClkHfSetSource(clkHf, hfClkPath))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING;
    }
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_ClkHfSetDivider(clkHf, divider))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING;
    }
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_ClkHfEnable(clkHf))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING;
    }

    return CY_PRA_STATUS_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_PRA_FllInit
****************************************************************************//**
*
* Initializes Frequency Locked Loop
*
* \param devConfig
*
* \return
* CY_PRA_STATUS_SUCCESS FLL init is success
* CY_PRA_STATUS_ERROR_PROCESSING_FLL0 FLL init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_FllInit(const cy_stc_pra_system_config_t *devConfig)
{
    const cy_stc_fll_manual_config_t fllconfig =
    {
        .ccoRange = devConfig->fllCcoRange,
        .cco_Freq = devConfig->ccoFreq,
        .enableOutputDiv = devConfig->enableOutputDiv,
        .fllMult = devConfig->fllMult,
        .igain = devConfig->igain,
        .lockTolerance = devConfig->lockTolerance,
        .outputMode = devConfig->outputMode,
        .pgain = devConfig->pgain,
        .refDiv = devConfig->fllRefDiv,
        .settlingCount = devConfig->settlingCount,
    };

    if (CY_SYSCLK_SUCCESS != Cy_SysClk_FllManualConfigure(&fllconfig))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_FLL0;
    }
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_FllEnable(CY_PRA_FLL_ENABLE_TIMEOUT))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_FLL0;
    }

    return CY_PRA_STATUS_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_PRA_ExtClkInit
****************************************************************************//**
*
* Initializes External Clock Source
*
* \param devConfig
*
*******************************************************************************/
__STATIC_INLINE void Cy_PRA_ExtClkInit( const cy_stc_pra_system_config_t *devConfig )
{
    if (devConfig->extClkPort != NULL)
    {
        Cy_GPIO_Pin_FastInit(devConfig->extClkPort, devConfig->extClkPinNum, CY_GPIO_DM_HIGHZ, 0UL, devConfig->extClkHsiom);
    }
    Cy_SysClk_ExtClkSetFrequency(devConfig->extClkFreqHz);
}


/*******************************************************************************
* Function Name: Cy_PRA_EcoInit
****************************************************************************//**
*
* Initializes External Crystal Oscillator
*
* \param devConfig
*
* \return
* CY_PRA_STATUS_SUCCESS CLK_ECO init is success
* CY_PRA_STATUS_ERROR_PROCESSING_ECO CLK_ECO init is failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_EcoInit(const cy_stc_pra_system_config_t *devConfig)
{
    if ((devConfig->ecoInPort != NULL) && (devConfig->ecoOutPort != NULL))
    {
        Cy_GPIO_Pin_FastInit(devConfig->ecoInPort, devConfig->ecoInPinNum, CY_GPIO_DM_ANALOG, 0UL, HSIOM_SEL_GPIO);
        Cy_GPIO_Pin_FastInit(devConfig->ecoOutPort, devConfig->ecoOutPinNum, CY_GPIO_DM_ANALOG, 0UL, HSIOM_SEL_GPIO);
    }
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_EcoConfigure(devConfig->ecoFreqHz, devConfig->ecoLoad, devConfig->ecoEsr, devConfig->ecoDriveLevel))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_ECO;
    }
    if (CY_SYSCLK_SUCCESS != Cy_SysClk_EcoEnable(CY_PRA_ECO_ENABLE_TIMEOUT))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_ECO;
    }

    return CY_PRA_STATUS_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_PRA_PiloInit
****************************************************************************//**
*
* Initializes PILO
*
*******************************************************************************/
__STATIC_INLINE void Cy_PRA_PiloInit(void)
{
    Cy_SysClk_PiloEnable();
}

#if defined(CY_IP_MXBLESS)
/*******************************************************************************
* Function Name: Cy_PRA_AltHfInit
****************************************************************************//**
*
* Initializes Alternative High-Frequency Clock
*
* \param devConfig
*
* \return
* CY_PRA_STATUS_SUCCESS AltHF init is success
* CY_PRA_STATUS_ERROR_PROCESSING_ALTHF AltHF init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_AltHfInit(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_ble_eco_status_t status;
    status = Cy_BLE_EcoConfigure((cy_en_ble_eco_freq_t)devConfig->altHFfreq, (cy_en_ble_eco_sys_clk_div_t)devConfig->altHFsysClkDiv, devConfig->altHFcLoad, devConfig->altHFxtalStartUpTime, (cy_en_ble_eco_voltage_reg_t)devConfig->altHFvoltageReg);
    if ((CY_BLE_ECO_SUCCESS != status) && (CY_BLE_ECO_ALREADY_STARTED !=status))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_ALTHF;
    }

    return CY_PRA_STATUS_SUCCESS;
}
#endif /* CY_IP_MXBLESS */


/*******************************************************************************
* Function Name: Cy_PRA_ClkLfInit
****************************************************************************//**
*
* Initializes Low-Frequency Clock
*
* \param clkLfSource \ref cy_en_clklf_in_sources_t
*
* \return
* CY_PRA_STATUS_SUCCESS LF init is success
* CY_PRA_STATUS_ERROR_PROCESSING_CLKLF LF init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_ClkLfInit(cy_en_clklf_in_sources_t clkLfSource)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_ERROR_PROCESSING_CLKLF;

    if (clkLfSource == Cy_SysClk_ClkLfGetSource()) /* No change in CLK_LF */
    {
        retStatus = CY_PRA_STATUS_SUCCESS;
    }
    else
    {
        if (!Cy_WDT_Locked())
        {
            Cy_SysClk_ClkLfSetSource(clkLfSource);
            retStatus = CY_PRA_STATUS_SUCCESS;
        }
    }
    return retStatus;
}


/*******************************************************************************
* Function Name: Cy_PRA_WcoInit
****************************************************************************//**
*
* Initializes Watch Crystal Oscillator
*
* \param devConfig
*
* \return
* CY_PRA_STATUS_SUCCESS WCO init is success
* CY_PRA_STATUS_ERROR_PROCESSING_WCO WCO init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_WcoInit(const cy_stc_pra_system_config_t *devConfig)
{
    if ((devConfig->wcoInPort != NULL) && (devConfig->wcoOutPort != NULL))
    {
        Cy_GPIO_Pin_FastInit(devConfig->wcoInPort , devConfig->wcoInPinNum, 0x00U, 0x00U, HSIOM_SEL_GPIO);
        Cy_GPIO_Pin_FastInit(devConfig->wcoOutPort , devConfig->wcoOutPinNum, 0x00U, 0x00U, HSIOM_SEL_GPIO);
    }
    if (devConfig->bypassEnable)
    {
        Cy_SysClk_WcoBypass(CY_SYSCLK_WCO_BYPASSED);
    }
    else
    {
        Cy_SysClk_WcoBypass(CY_SYSCLK_WCO_NOT_BYPASSED);
    }

    if (CY_SYSCLK_SUCCESS != Cy_SysClk_WcoEnable(CY_PRA_WCO_ENABLE_TIMEOUT))
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_WCO;
    }

    return CY_PRA_STATUS_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_PRA_PowerInit
****************************************************************************//**
*
* Initializes Power
*
* \param devConfig
*
* \return
* CY_PRA_STATUS_SUCCESS Power init is success
* CY_PRA_STATUS_ERROR_PROCESSING_PWR Power init failed
*
*******************************************************************************/
__STATIC_INLINE cy_en_pra_status_t Cy_PRA_PowerInit(const cy_stc_pra_system_config_t *devConfig)
{
    static bool firstCallAfterReset = true;
    cy_en_pra_status_t status;

    /* Reset the Backup domain on POR, XRES, BOD only if Backup domain is supplied by VDDD */
    if ((devConfig->vBackupVDDDEnable) && firstCallAfterReset)
    {
        if (devConfig->iloEnable)
        {
            if (0u == Cy_SysLib_GetResetReason() /* POR, XRES, or BOD */)
            {
                if (CY_SYSLIB_SUCCESS != Cy_SysLib_ResetBackupDomain())
                {
                    return CY_PRA_STATUS_ERROR_PROCESSING_PWR;
                }
                status = Cy_PRA_IloDisable();
                if (CY_PRA_STATUS_SUCCESS != status)
                {
                    return status;
                }
                status = Cy_PRA_IloEnable();
                if (CY_PRA_STATUS_SUCCESS != status)
                {
                    return status;
                }
                status = Cy_PRA_IloHibernateOn(devConfig->iloHibernateON);
                if (CY_PRA_STATUS_SUCCESS != status)
                {
                    return status;
                }
            }
        }
    }
    firstCallAfterReset = false;

    if (devConfig->ldoEnable)
    {
        /* LDO valid voltage */
        if ((devConfig->ldoVoltage == CY_SYSPM_LDO_VOLTAGE_0_9V) ||
            (devConfig->ldoVoltage == CY_SYSPM_LDO_VOLTAGE_1_1V))
            {
                if (CY_SYSPM_SUCCESS != Cy_SysPm_LdoSetVoltage(devConfig->ldoVoltage))
                {
                    return CY_PRA_STATUS_ERROR_PROCESSING_PWR;
                }
            }
    }
    else
    {
        if ((devConfig->buckVoltage == CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V) ||
            (devConfig->buckVoltage == CY_SYSPM_BUCK_OUT1_VOLTAGE_1_1V))
            {
                if (CY_SYSPM_SUCCESS != Cy_SysPm_BuckEnable(devConfig->buckVoltage))
                {
                    return CY_PRA_STATUS_ERROR_PROCESSING_PWR;
                }
            }
    }

    if (devConfig->pwrCurrentModeMin)
    {
        /* Sets the system into minimum core regulator current mode */
        if (CY_SYSPM_SUCCESS != Cy_SysPm_SystemSetMinRegulatorCurrent())
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PWR;
        }
    }
    else
    {
        /* Sets the system into normal core regulator current mode */
        if (CY_SYSPM_SUCCESS != Cy_SysPm_SystemSetNormalRegulatorCurrent())
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PWR;
        }
    }

    /* Configure PMIC */
    Cy_SysPm_UnlockPmic();
    if (devConfig->pmicEnable)
    {
        Cy_SysPm_PmicEnableOutput();
    }
    else
    {
        Cy_SysPm_PmicDisableOutput();
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_GetInputPathMuxFrq
****************************************************************************//**
*
* Get Source clock frequency for the PATH MUX
*
* \param pathMuxSrc Source clock for the PATH_MUX
* \param devConfig System Configuration Parameter
*
* \return
* Returns source frequency of path mux
*
*******************************************************************************/
static uint32_t Cy_PRA_GetInputPathMuxFrq(cy_en_clkpath_in_sources_t pathMuxSrc, const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t srcFreq; /* Hz */

    switch (pathMuxSrc)
    {
        case CY_SYSCLK_CLKPATH_IN_IMO:
        {
            srcFreq = CY_PRA_IMO_SRC_FREQUENCY; /* IMO Freq = 8 MHz */
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_EXT:
        {
            srcFreq = devConfig->extClkFreqHz;
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_ECO:
        {
            srcFreq = devConfig->ecoFreqHz;
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_ALTHF:
        {
            srcFreq = devConfig->altHFfreq;
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_ILO:
        {
            srcFreq = CY_PRA_ILO_SRC_FREQUENCY; /* ILO Freq = 32 KHz */
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_WCO:
        {
            srcFreq = CY_PRA_WCO_SRC_FREQUENCY; /* WCO Freq = 32.768 KHz */
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_PILO:
        {
            srcFreq = CY_PRA_PILO_SRC_FREQUENCY; /* PILO Freq = 32.768 KHz */
        }
        break;
        default:
        {
            srcFreq = CY_PRA_DEFAULT_SRC_FREQUENCY;
        }
        break;
    } /* End Switch */

    return srcFreq;
}

/*******************************************************************************
* Function Name: Cy_PRA_GetInputSourceFreq
****************************************************************************//**
*
* Get Source clock frequency for the Clock Path. This function is called from HF
* level
*
* \param clkPath Clock Path
* \param devConfig System Configuration Parameter
* \param srcFreq is source frequency of clock path that is updated in this function.
*
* \return
* CY_PRA_STATUS_SUCCESS if frequency is updated.
* CY_PRA_STATUS_INVALID_PARAM if the clkpaht is not valid path.
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_GetInputSourceFreq(uint32_t clkPath, const cy_stc_pra_system_config_t *devConfig, uint32_t *srcFreq )
{
    cy_en_pra_status_t status = CY_PRA_STATUS_INVALID_PARAM;
    *srcFreq = CY_PRA_DEFAULT_SRC_FREQUENCY;

    if(clkPath >= CY_SRSS_NUM_CLKPATH)
    {
        return status;
    }

    switch (clkPath)
    {
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH0: /* CLK_PATH0 */
        {
            if (devConfig->path0Enable)
            {
                /* check for FLL */
                if ((devConfig->fllEnable) && (devConfig->outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT))
                {
                    *srcFreq = devConfig->fllOutFreqHz;
                    status = CY_PRA_STATUS_SUCCESS;
                }
                else
                {
                    *srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path0Src, devConfig);
                    status = CY_PRA_STATUS_SUCCESS;
                }
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH1: /* CLK_PATH1 */
        {
            if (devConfig->path1Enable)
            {
                /* check for PLL0 */
                if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_1) && (devConfig->pll0Enable) && (devConfig->pll0OutputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT))
                {
                    *srcFreq = devConfig->pll0OutFreqHz;
                    status = CY_PRA_STATUS_SUCCESS;
                }
                else
                {
                    *srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path1Src, devConfig);
                    status = CY_PRA_STATUS_SUCCESS;
                }
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH2: /* CLK_PATH2 */
        {
            if (devConfig->path2Enable)
            {
                /* check for PLL1 */
                if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_2) && (devConfig->pll1Enable) && (devConfig->pll1OutputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT))
                {
                    *srcFreq = devConfig->pll1OutFreqHz;
                    status = CY_PRA_STATUS_SUCCESS;
                }
                else
                {
                    *srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path2Src, devConfig);
                    status = CY_PRA_STATUS_SUCCESS;
                }
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH3: /* CLK_PATH3 */
        {
            if (devConfig->path3Enable)
            {
                *srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path3Src, devConfig);
                status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH4: /* CLK_PATH4 */
        {
            if (devConfig->path4Enable)
            {
                *srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path4Src, devConfig);
                status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH5: /* CLK_PATH5 */
        {
            if (devConfig->path5Enable)
            {
                *srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path5Src, devConfig);
                status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        default:
        {
            *srcFreq = CY_PRA_DEFAULT_SRC_FREQUENCY;
        }
        break;
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_PRA_GetClkLfFreq
****************************************************************************//**
*
* Get Low-Frequency Clock (CLK_LF)
*
* \param devConfig System Configuration Parameter
*
* \return CLK_LF frequency
*
*******************************************************************************/
static uint32_t Cy_PRA_GetClkLfFreq(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq = CY_PRA_DEFAULT_SRC_FREQUENCY;

    if (devConfig->clkLFEnable)
    {
        switch (devConfig->clkLfSource)
        {
            case CY_SYSCLK_CLKLF_IN_ILO:
            {
                if (devConfig->iloEnable)
                {
                    freq = CY_PRA_ILO_SRC_FREQUENCY;
                }
            }
            break;
            case CY_SYSCLK_CLKLF_IN_WCO:
            {
                if (devConfig->wcoEnable)
                {
                    freq = CY_PRA_WCO_SRC_FREQUENCY;
                }
            }
            break;
            case CY_SYSCLK_CLKLF_IN_PILO:
            {
                if (devConfig->piloEnable)
                {
                    freq = CY_PRA_PILO_SRC_FREQUENCY;
                }
            }
            break;
            default:
            {
                freq = CY_PRA_DEFAULT_SRC_FREQUENCY;
            }
            break;
        } /* End Switch */
    }

    return freq;
}

/*******************************************************************************
* Function Name: Cy_PRA_GetClkBakFreq
****************************************************************************//**
*
* Get BAK Clock (CLK_BAK) frequency
*
* \param devConfig System Configuration Parameter
*
* \return CLK_LF frequency
*
*******************************************************************************/
static uint32_t Cy_PRA_GetClkBakFreq(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq = CY_PRA_DEFAULT_SRC_FREQUENCY;

    if (devConfig->clkBakEnable)
    {
        switch (devConfig->clkBakSource)
        {
            case CY_SYSCLK_BAK_IN_WCO:
            {
                if (devConfig->wcoEnable)
                {
                    freq = CY_PRA_WCO_SRC_FREQUENCY;
                }
            }
            break;
            case CY_SYSCLK_BAK_IN_CLKLF:
            {
                if (devConfig->clkLFEnable)
                {
                    freq = Cy_PRA_GetClkLfFreq(devConfig);
                }
            }
            break;
            default:
            {
                freq = CY_PRA_DEFAULT_SRC_FREQUENCY;
            }
            break;
        } /* End Switch */
    }

    return freq;
}


/*******************************************************************************
* Function Name: Cy_PRA_GetClkTimerFreq
****************************************************************************//**
*
* Get CLK_TIMER output frequency
*
* \param devConfig System Configuration Parameter
*
* \return CLK_TIME output frequency
*
*******************************************************************************/
static uint32_t Cy_PRA_GetClkTimerFreq(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq = CY_PRA_DEFAULT_SRC_FREQUENCY;
    uint8_t srcDiv;
    cy_en_pra_status_t status = CY_PRA_STATUS_SUCCESS;

    /* source clock must be enabled */
    if (devConfig->clkTimerEnable)
    {
        switch (devConfig->clkTimerSource)
        {
            case CY_SYSCLK_CLKTIMER_IN_IMO:
            {
                /* IMO is always on */
                freq = CY_PRA_IMO_SRC_FREQUENCY; /* 8 MHz */
                srcDiv = CY_PRA_DIVIDER_1;
            }
            break;
            case CY_SYSCLK_CLKTIMER_IN_HF0_NODIV:
            case CY_SYSCLK_CLKTIMER_IN_HF0_DIV2:
            case CY_SYSCLK_CLKTIMER_IN_HF0_DIV4:
            case CY_SYSCLK_CLKTIMER_IN_HF0_DIV8:
            {
                if ( devConfig->clkHF0Enable )
                {
                    if (devConfig->clkTimerSource == CY_SYSCLK_CLKTIMER_IN_HF0_DIV2)
                    {
                        srcDiv = CY_PRA_DIVIDER_2;
                    }
                    else if (devConfig->clkTimerSource == CY_SYSCLK_CLKTIMER_IN_HF0_DIV4)
                    {
                        srcDiv = CY_PRA_DIVIDER_4;
                    }
                    else if (devConfig->clkTimerSource == CY_SYSCLK_CLKTIMER_IN_HF0_DIV8)
                    {
                        srcDiv = CY_PRA_DIVIDER_8;
                    }
                    else
                    {
                        srcDiv = CY_PRA_DIVIDER_1;
                    }
                    freq = devConfig->hf0OutFreqMHz*CY_PRA_FREQUENCY_HZ_CONVERSION;
                }
                else
                {
                    status = CY_PRA_STATUS_INVALID_PARAM_CLKTIMER;
                }
            }
            break;
            default:
            {
                freq = CY_PRA_DEFAULT_SRC_FREQUENCY;
                status = CY_PRA_STATUS_INVALID_PARAM_CLKTIMER;
            }
            break;
        }

        if (status != CY_PRA_STATUS_INVALID_PARAM_CLKTIMER)
        {
            freq = (freq / (devConfig->clkTimerDivider + 1UL)) / srcDiv;
        }
    }

    return freq;
}

/*******************************************************************************
* Function Name: Cy_PRA_GetInputSourceClock
****************************************************************************//**
*
* Get Source clock for the Clock Path
*
* \param clkPath Clock Path
* \param devConfig System Configuration Parameter
*
* \return
* Returns source clock
*
*******************************************************************************/
static cy_en_clkpath_in_sources_t Cy_PRA_GetInputSourceClock(uint32_t clkPath, const cy_stc_pra_system_config_t *devConfig, cy_en_pra_status_t *status)
{
    cy_en_clkpath_in_sources_t srcClock = CY_SYSCLK_CLKPATH_IN_IMO;

    if(clkPath >= CY_SRSS_NUM_CLKPATH)
    {
        *status = CY_PRA_STATUS_INVALID_PARAM;
        return srcClock;
    }

    *status = CY_PRA_STATUS_INVALID_PARAM;

    switch (clkPath)
    {
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH0: /* CLK_PATH0 */
        {
            if (devConfig->path0Enable)
            {
                srcClock = devConfig->path0Src;
                *status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH1: /* CLK_PATH1 */
        {
            if (devConfig->path1Enable)
            {
                srcClock = devConfig->path1Src;
                *status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH2: /* CLK_PATH2 */
        {
            if (devConfig->path2Enable)
            {
                srcClock = devConfig->path2Src;
                *status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH3: /* CLK_PATH3 */
        {
            if (devConfig->path3Enable)
            {
                srcClock = devConfig->path3Src;
                *status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH4: /* CLK_PATH4 */
        {
            if (devConfig->path4Enable)
            {
                srcClock = devConfig->path4Src;
                *status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH5: /* CLK_PATH5 */
        {
            if (devConfig->path5Enable)
            {
                srcClock = devConfig->path5Src;
                *status = CY_PRA_STATUS_SUCCESS;
            }
        }
        break;
        default:
        {
            /* Return CY_PRA_STATUS_INVALID_PARAM */
        }
        break;
    }

    return srcClock;
}

#define CY_PRA_SYSCLK_ECO_FREQ_MIN (16000000UL) /* 16 MHz   */
#define CY_PRA_SYSCLK_ECO_FREQ_MAX (35000000UL) /* 35 MHz   */
#define CY_PRA_SYSCLK_ECO_CSM_MIN  (1UL)        /* 1  pF    */
#define CY_PRA_SYSCLK_ECO_CSM_MAX  (100UL)      /* 100 pF   */
#define CY_PRA_SYSCLK_ECO_ESR_MIN  (1UL)        /* 1  Ohm   */
#define CY_PRA_SYSCLK_ECO_ESR_MAX  (1000UL)     /* 1000 Ohm */
#define CY_PRA_SYSCLK_ECO_DRV_MIN  (1UL)        /* 1 kW     */
#define CY_PRA_SYSCLK_ECO_DRV_MAX  (1000UL)     /* 1 mW     */


/*******************************************************************************
* Function Name: Cy_PRA_ValidateECO
****************************************************************************//**
*
* Validate ECO
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_ECO for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateECO(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_SUCCESS;

    /* ECO can't be source for HF0. This check will be performed in HF0 validation  */
    /* Parameter validation will be implemented in DRIVERS-2751 */
    if ((CY_SYSCLK_ECOSTAT_STABLE != Cy_SysClk_EcoGetStatus()) && (devConfig->ecoEnable))
    {
        if (((devConfig->ecoFreqHz >= CY_PRA_SYSCLK_ECO_FREQ_MIN) && (devConfig->ecoFreqHz <= CY_PRA_SYSCLK_ECO_FREQ_MAX)) /* legal range of ECO frequecy is [16.0000-35.0000] */
            && ((devConfig->ecoLoad >= CY_PRA_SYSCLK_ECO_CSM_MIN) && (devConfig->ecoLoad <= CY_PRA_SYSCLK_ECO_CSM_MAX)) /* ECO cLoad range [1 - 100] */
            && ((devConfig->ecoEsr >= CY_PRA_SYSCLK_ECO_ESR_MIN) && (devConfig->ecoEsr <= CY_PRA_SYSCLK_ECO_ESR_MAX)) /* ECO ESR range [1 - 1000] */
            && ((devConfig->ecoDriveLevel >= CY_PRA_SYSCLK_ECO_DRV_MIN) && (devConfig->ecoDriveLevel <= CY_PRA_SYSCLK_ECO_DRV_MAX))) /* ECO Drive Level range [1 - 1000] */
        {
            retStatus = CY_PRA_STATUS_SUCCESS;
        }
        else
        {
            retStatus = CY_PRA_STATUS_INVALID_PARAM_ECO;
        }
    }

    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateEXTClk
****************************************************************************//**
*
* Validate External Clock Source
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_EXTCLK for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateEXTClk(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t maxFreq;

    /* For ULP mode, Fextclk_max = 50 MHz. For LP mode, Fextclk_max = 100 MHz or Fcpu_max (if Fcpu_max < 100 MHz) */
    if (devConfig->extClkEnable)
    {
        if (devConfig->ulpEnable)
        {
            if (devConfig->extClkFreqHz > CY_PRA_ULP_MODE_MAX_FREQUENCY)
            {
                return CY_PRA_STATUS_INVALID_PARAM_EXTCLK;
            }
        }
        else
        {
            (CY_HF_CLK_MAX_FREQ > CY_PRA_LP_MODE_MAX_FREQUENCY) ? (maxFreq = CY_PRA_LP_MODE_MAX_FREQUENCY) : (maxFreq = CY_HF_CLK_MAX_FREQ);
            if (devConfig->extClkFreqHz > maxFreq)
            {
                return CY_PRA_STATUS_INVALID_PARAM_EXTCLK;
            }
        } /*usingULP*/

        /* GPIO port can't be NULL */
        if (devConfig->extClkPort == NULL)
        {
            return CY_PRA_STATUS_INVALID_PARAM_EXTCLK;
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

#if defined(CY_IP_MXBLESS)

/*******************************************************************************
* Function Name: Cy_PRA_ValidateAltHf
****************************************************************************//**
*
* Validate Alternative High-Frequency Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_ALTHF for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateAltHf(const cy_stc_pra_system_config_t *devConfig)
{

    if (devConfig->clkAltHfEnable)
    {
        uint32_t startupTime;
        /* Validate Frequency */
        if ((devConfig->altHFfreq < CY_PRA_ALTHF_MIN_FREQUENCY) || (devConfig->altHFfreq > CY_PRA_ALTHF_MAX_FREQUENCY))
        {
            return CY_PRA_STATUS_INVALID_PARAM_ALTHF;
        }
        /* Startup Time */
        startupTime = devConfig->altHFxtalStartUpTime << 5U;
        if ((startupTime < CY_PRA_ALTHF_MIN_STARTUP_TIME) || (startupTime > CY_PRA_ALTHF_MAX_STARTUP_TIME))
        {
            return CY_PRA_STATUS_INVALID_PARAM_ALTHF;
        }
        /* Load Cap Range min="7.5" max="26.325" */
        if ((devConfig->altHFcLoad < CY_PRA_ALTHF_MIN_LOAD) || (devConfig->altHFcLoad > CY_PRA_ALTHF_MAX_LOAD))
        {
            return CY_PRA_STATUS_INVALID_PARAM_ALTHF;
        }
        /* Validate clock divider */
        if (devConfig->altHFsysClkDiv > CY_BLE_SYS_ECO_CLK_DIV_8)
        {
            return CY_PRA_STATUS_INVALID_PARAM_ALTHF;
        }
    }
    return CY_PRA_STATUS_SUCCESS;
}
#endif /* CY_IP_MXBLESS */

/*******************************************************************************
* Function Name: Cy_PRA_ValidateFLL
****************************************************************************//**
*
* Validate Frequency Locked Loop (FLL)
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_FLL for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateFLL(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t srcFreq;

    if ((!Cy_SysClk_FllIsEnabled()) && (devConfig->fllEnable))
    {
        /* FLL is always sourced from PATH_MUX0 */
        /* If FLL is sourced from ECO, WCO, ALTHF, EXTCLK, ILO, PILO clocks, then FLL output can't source
         * to HF0. This check is performed at HF0 validation */
        if (devConfig->path0Enable)
        {
            /* Source clock for FLL valid range is 1 kHz - 100 MHz */
            srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path0Src, devConfig);
            if ((srcFreq < CY_PRA_FLL_SRC_MIN_FREQUENCY) || (srcFreq > CY_PRA_FLL_SRC_MAX_FREQUENCY))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }

            /* For ULP mode, out frequency range is 24 - 50 MHz */
            /* For LP mode, out frequency range is 24 - 100 MHz */
            if (devConfig->ulpEnable)
            {
                if ((devConfig->fllOutFreqHz < CY_PRA_FLL_OUT_MIN_FREQUENCY) || (devConfig->fllOutFreqHz > CY_PRA_FLL_ULP_OUT_MAX_FREQUENCY))
                {
                    return CY_PRA_STATUS_INVALID_PARAM_FLL0;
                }
            }
            else
            {
                if ((devConfig->fllOutFreqHz < CY_PRA_FLL_OUT_MIN_FREQUENCY) || (devConfig->fllOutFreqHz > CY_PRA_FLL_OUT_MAX_FREQUENCY))
                {
                    return CY_PRA_STATUS_INVALID_PARAM_FLL0;
                }
            } /*usingULP*/

            /* Validate multiplier min="1" max="262143" */
            if ((devConfig->fllMult < CY_PRA_FLL_MIN_MULTIPLIER) || (devConfig->fllMult > CY_PRA_FLL_MAX_MULTIPLIER))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            /* Validate reference min="1" max="8191" */
            if ((devConfig->fllRefDiv < CY_PRA_FLL_MIN_REFDIV) || (devConfig->fllRefDiv > CY_PRA_FLL_MAX_REFDIV))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            /* ccoRange */
            if (devConfig->fllCcoRange > CY_SYSCLK_FLL_CCO_RANGE4)
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            /* lockTolerance min="0" max="511" */
            if (devConfig->lockTolerance > CY_PRA_FLL_MAX_LOCK_TOLERENCE)
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            if (devConfig->igain > (SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Msk >> SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Pos))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            if (devConfig->pgain > (SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Msk >> SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Pos))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            if (devConfig->settlingCount > (SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Msk >> SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Pos))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }
            if (devConfig->ccoFreq > (SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Msk >> SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Pos))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }

            /* Validate FLL output frequency */
            if ((devConfig->fllOutFreqHz) != Cy_PRA_CalculateFLLOutFreq(devConfig))
            {
                return CY_PRA_STATUS_INVALID_PARAM_FLL0;
            }

            return CY_PRA_STATUS_SUCCESS;
        }
        else
        {
            return CY_PRA_STATUS_INVALID_PARAM_FLL0;
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidatePLL
****************************************************************************//**
*
* Validate Phase Locked Loop (PLL)
*
* \param devConfig System Configuration Parameter
* \param pll PLL number
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidatePLL(const cy_stc_pra_system_config_t *devConfig, uint8_t pll)
{
    uint32_t srcFreq, outFreq, minDesiredOutFreq;
    bool pllEnable, pathEnable, lowFreqMode;
    cy_en_clkpath_in_sources_t pathSrc;

    if ((pll == CY_PRA_DEFAULT_ZERO) || (pll > CY_SRSS_NUM_PLL))
    {
        return CY_PRA_STATUS_INVALID_PARAM;
    }

    if (pll == CY_PRA_CLKPLL_1)
    {
        pllEnable = devConfig->pll0Enable;
        pathEnable = devConfig->path1Enable;
        pathSrc = devConfig->path1Src;
        outFreq = devConfig->pll0OutFreqHz;
        lowFreqMode = devConfig->pll0LfMode;
    }
    else if (pll == CY_PRA_CLKPLL_2)
    {
        pllEnable = devConfig->pll1Enable;
        pathEnable = devConfig->path2Enable;
        pathSrc = devConfig->path2Src;
        outFreq = devConfig->pll1OutFreqHz;
        lowFreqMode = devConfig->pll1LfMode;
    }
    else
    {
        return CY_PRA_STATUS_INVALID_PARAM;
    }

    /* If PLL is sourced from ECO, WCO, ALTHF, EXTCLK, ILO, PILO clocks, then PLL output can't source
     * to HF0. This check is performed at HF0 validation */
    if (pllEnable)
    {
        if (pathEnable)
        {
            /* Source clock for PLL has valid range of 4 MHz - 64 MHz */
            srcFreq = Cy_PRA_GetInputPathMuxFrq(pathSrc, devConfig);
            if ((srcFreq < CY_PRA_PLL_SRC_MIN_FREQUENCY) || (srcFreq > CY_PRA_PLL_SRC_MAX_FREQUENCY))
            {
                return CY_PRA_STATUS_INVALID_PARAM;
            }

            /* If Low frequency mode is enabled then pll minimum output frequency is 10.625 MHz otherwise 12.5 MHz */
            (lowFreqMode) ? (minDesiredOutFreq = CY_PRA_PLL_LOW_OUT_MIN_FREQUENCY) : (minDesiredOutFreq = CY_PRA_PLL_OUT_MIN_FREQUENCY);
            /* For ULP mode, out frequency should not > 50 MHz */
            /* For LP mode, out frequency should not > CY_HF_CLK_MAX_FREQ */
            if (devConfig->ulpEnable)
            {
                if ((outFreq < minDesiredOutFreq) || (outFreq > CY_PRA_PLL_ULP_OUT_MAX_FREQUENCY))
                {
                    return CY_PRA_STATUS_INVALID_PARAM;
                }
            }
            else
            {
                if ((outFreq < minDesiredOutFreq) || (outFreq > CY_PRA_PLL_OUT_MAX_FREQUENCY))
                {
                    return CY_PRA_STATUS_INVALID_PARAM;
                }
            } /*usingULP*/

            /* Check PLL output frequency */
            if (outFreq != Cy_PRA_CalculatePLLOutFreq(pll, devConfig))
            {
                return CY_PRA_STATUS_INVALID_PARAM;
            }
        }
        else
        {
            return CY_PRA_STATUS_INVALID_PARAM;
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateAllPLL
****************************************************************************//**
*
* Validate All Phase Locked Loop (PLL)
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_PLL0 for bad parameter for PLL0
* CY_PRA_STATUS_INVALID_PARAM_PLL1 for bad parameter for PLL1
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateAllPLL(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_SUCCESS;

    /* PLL0 is always sourced from PATH_MUX1 */
    if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_1) && (devConfig->pll0Enable))
    {
        if (!Cy_SysClk_PllIsEnabled(CY_PRA_CLKPLL_1))
        {
            retStatus = Cy_PRA_ValidatePLL(devConfig, CY_PRA_CLKPLL_1);
        }

        if (CY_PRA_STATUS_SUCCESS != retStatus)
        {
            retStatus = CY_PRA_STATUS_INVALID_PARAM_PLL0;
        }
    }
    /* PLL1 is always sourced from PATH_MUX2 */
    if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_2) && (devConfig->pll1Enable) && (retStatus == CY_PRA_STATUS_SUCCESS))
    {
        if (!Cy_SysClk_PllIsEnabled(CY_PRA_CLKPLL_2))
        {
            retStatus = Cy_PRA_ValidatePLL(devConfig, CY_PRA_CLKPLL_2);
        }

        if (CY_PRA_STATUS_SUCCESS != retStatus)
        {
            retStatus = CY_PRA_STATUS_INVALID_PARAM_PLL1;
        }
    }
    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkLf
****************************************************************************//**
*
* Validate Low-Frequency Clock (CLK_LF)
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKLF for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkLf(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_SUCCESS;

    /* output frequency = input frequency [range min="0" max="100000"] */
    if (devConfig->clkLFEnable)
    {
        switch (devConfig->clkLfSource)
        {
            case CY_SYSCLK_CLKLF_IN_ILO:
            {
                (devConfig->iloEnable) ? (retStatus = CY_PRA_STATUS_SUCCESS) : (retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKLF);
            }
            break;
            case CY_SYSCLK_CLKLF_IN_WCO:
            {
                (devConfig->wcoEnable) ? (retStatus = CY_PRA_STATUS_SUCCESS) : (retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKLF);
            }
            break;
            case CY_SYSCLK_CLKLF_IN_PILO:
            {
                (devConfig->piloEnable) ? (retStatus = CY_PRA_STATUS_SUCCESS) : (retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKLF);
            }
            break;
            default:
            {
                retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKLF;
            }
            break;
        } /* End of Switch */
    }

    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkPathMux
****************************************************************************//**
*
* Return error if specified path source is disabled.
*
* \param pathMuxSrc Source clock for the PATH_MUX
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkPathMux(cy_en_clkpath_in_sources_t pathSrc, const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t status;

    /* Check Source clock is enabled */
    switch (pathSrc)
    {
        case CY_SYSCLK_CLKPATH_IN_IMO:
        {
            status = CY_PRA_STATUS_SUCCESS;
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_EXT:
        {
            (devConfig->extClkEnable) ? (status = CY_PRA_STATUS_SUCCESS) : (status = CY_PRA_STATUS_INVALID_PARAM);
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_ECO:
        {
            (devConfig->ecoEnable) ? (status = CY_PRA_STATUS_SUCCESS) : (status = CY_PRA_STATUS_INVALID_PARAM);
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_ALTHF:
        {
            (devConfig->clkAltHfEnable) ? (status = CY_PRA_STATUS_SUCCESS) : (status = CY_PRA_STATUS_INVALID_PARAM);
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_ILO:
        {
            (devConfig->iloEnable) ? (status = CY_PRA_STATUS_SUCCESS) : (status = CY_PRA_STATUS_INVALID_PARAM);
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_WCO:
        {
            (devConfig->wcoEnable) ? (status = CY_PRA_STATUS_SUCCESS) : (status = CY_PRA_STATUS_INVALID_PARAM);
        }
        break;
        case CY_SYSCLK_CLKPATH_IN_PILO:
        {
            (devConfig->piloEnable) ? (status = CY_PRA_STATUS_SUCCESS) : (status = CY_PRA_STATUS_INVALID_PARAM);
        }
        break;
        default:
        {
            status = CY_PRA_STATUS_INVALID_PARAM;
        }
        break;
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkPath
****************************************************************************//**
*
* Validate Clock Path
*
* \param clkPath Clock path
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkPath(uint32_t clkPath, const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_INVALID_PARAM;
    if(clkPath >= CY_SRSS_NUM_CLKPATH)
    {
        return     retStatus;
    }

    switch (clkPath)
    {
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH0: /* CLK_PATH0 */
        {
            if (devConfig->path0Enable)
            {
                retStatus = Cy_PRA_ValidateClkPathMux(devConfig->path0Src, devConfig);
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH1: /* CLK_PATH1 */
        {
            if (devConfig->path1Enable)
            {
                retStatus = Cy_PRA_ValidateClkPathMux(devConfig->path1Src, devConfig);
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH2: /* CLK_PATH2 */
        {
            if (devConfig->path2Enable)
            {
                retStatus = Cy_PRA_ValidateClkPathMux(devConfig->path2Src, devConfig);
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH3: /* CLK_PATH3 */
        {
            if (devConfig->path3Enable)
            {
                retStatus = Cy_PRA_ValidateClkPathMux(devConfig->path3Src, devConfig);
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH4: /* CLK_PATH4 */
        {
            if (devConfig->path4Enable)
            {
                retStatus = Cy_PRA_ValidateClkPathMux(devConfig->path4Src, devConfig);
            }
        }
        break;
        case (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH5: /* CLK_PATH5 */
        {
            if (devConfig->path5Enable)
            {
                retStatus = Cy_PRA_ValidateClkPathMux(devConfig->path5Src, devConfig);
            }
        }
        break;
        default:
        {
            retStatus = CY_PRA_STATUS_INVALID_PARAM;
        }
        break;
    }

    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateAllClkPathMux
****************************************************************************//**
*
* Validate All PATH MUXes
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX0 for bad parameter for path-mux0
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX1 for bad parameter for path-mux1
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX2 for bad parameter for path-mux2
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX3 for bad parameter for path-mux3
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX4 for bad parameter for path-mux4
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX5 for bad parameter for path-mux5
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateAllClkPathMux(const cy_stc_pra_system_config_t *devConfig)
{

    if ((devConfig->path0Enable) && (CY_PRA_CLKPATH_0 < CY_SRSS_NUM_CLKPATH)) /* path_mux0 is enabled */
    {
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPathMux(devConfig->path0Src, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_PATHMUX0;
        }
    }
    if ((devConfig->path1Enable) && (CY_PRA_CLKPATH_1 < CY_SRSS_NUM_CLKPATH)) /* path_mux1 is enabled */
    {
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPathMux(devConfig->path1Src, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_PATHMUX1;
        }
    }
    if ((devConfig->path2Enable) && (CY_PRA_CLKPATH_2 < CY_SRSS_NUM_CLKPATH)) /* path_mux2 is enabled */
    {
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPathMux(devConfig->path2Src, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_PATHMUX2;
        }
    }
    if ((devConfig->path3Enable) && (CY_PRA_CLKPATH_3 < CY_SRSS_NUM_CLKPATH)) /* path_mux3 is enabled */
    {
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPathMux(devConfig->path3Src, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_PATHMUX3;
        }
    }
    if ((devConfig->path4Enable) && (CY_PRA_CLKPATH_4 < CY_SRSS_NUM_CLKPATH)) /* path_mux4 is enabled */
    {
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPathMux(devConfig->path4Src, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_PATHMUX4;
        }
    }
    if ((devConfig->path5Enable) && (CY_PRA_CLKPATH_5 < CY_SRSS_NUM_CLKPATH))/* path_mux5 is enabled */
    {
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPathMux(devConfig->path5Src, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_PATHMUX5;
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkHfFreqDiv
****************************************************************************//**
*
* Validate High Frequency Clock's output frequency and divider
*
* \param outFreq Output frequency
* \param divider Frequency divider
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkHfFreqDiv(uint32_t outFreqMHz, cy_en_clkhf_dividers_t divider)
{
    /* min="1" max="CY_HF_CLK_MAX_FREQ" */
    if ((outFreqMHz*CY_PRA_FREQUENCY_HZ_CONVERSION) > CY_HF_CLK_MAX_FREQ)
    {
        return CY_PRA_STATUS_INVALID_PARAM;
    }
    if (divider > CY_SYSCLK_CLKHF_DIVIDE_BY_8)
    {
        return CY_PRA_STATUS_INVALID_PARAM;
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkHFs
****************************************************************************//**
*
* Validate All High Frequency Clocks
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKHF0 for bad parameter for HF0
* CY_PRA_STATUS_INVALID_PARAM_CLKHF1 for bad parameter for HF1
* CY_PRA_STATUS_INVALID_PARAM_CLKHF2 for bad parameter for HF2
* CY_PRA_STATUS_INVALID_PARAM_CLKHF3 for bad parameter for HF3
* CY_PRA_STATUS_INVALID_PARAM_CLKHF4 for bad parameter for HF4
* CY_PRA_STATUS_INVALID_PARAM_CLKHF5 for bad parameter for HF5
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkHFs(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_clkpath_in_sources_t clkSource;
    cy_en_pra_status_t status;
    uint32_t freq;

    /* Validate HF0 */
    if ((devConfig->clkHF0Enable) && (CY_PRA_CLKHF_0 < CY_SRSS_NUM_HFROOT))
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t)(devConfig->hf0Source), devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF0;
        }

        /* ECO, WCO, ALTHF, EXTCLK, ILO, PILO cannot act as source to HF0  */
        clkSource = Cy_PRA_GetInputSourceClock((uint32_t) devConfig->hf0Source, devConfig, &status);
        if ((clkSource != CY_SYSCLK_CLKPATH_IN_IMO) || (status != CY_PRA_STATUS_SUCCESS))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF0;
        }

        /* HF0: input source can not be slower than legal min 200 kHz */
        status = Cy_PRA_GetInputSourceFreq((uint32_t) devConfig->hf0Source, devConfig, &freq);
        if ((freq < CY_PRA_HF0_MIN_FREQUENCY) || (status != CY_PRA_STATUS_SUCCESS))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF0;
        }

        /* Validate Output frequency and Dvivider */
        if ((devConfig->hf0OutFreqMHz == 0UL) || (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkHfFreqDiv(devConfig->hf0OutFreqMHz, devConfig->hf0Divider)))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF0;
        }
    }
    else
    {
        /* This can't be disabled */
        return CY_PRA_STATUS_INVALID_PARAM_CLKHF0;
    }

    /* Validate HF1 */
    if ((devConfig->clkHF1Enable) && (CY_PRA_CLKHF_1 < CY_SRSS_NUM_HFROOT))
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t)(devConfig->hf1Source), devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF1;
        }
        /* Validate Output frequency and Dvivider */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkHfFreqDiv(devConfig->hf1OutFreqMHz, devConfig->hf1Divider))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF1;
        }
    }

    /* Validate HF2 */
    if ((devConfig->clkHF2Enable) && (CY_PRA_CLKHF_2 < CY_SRSS_NUM_HFROOT))
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t)(devConfig->hf2Source), devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF2;
        }

        /* Validate Output frequency and Dvivider */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkHfFreqDiv(devConfig->hf2OutFreqMHz, devConfig->hf2Divider))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF2;
        }
    }

    /* Validate HF3 */
    if ((devConfig->clkHF3Enable) && (CY_PRA_CLKHF_3 < CY_SRSS_NUM_HFROOT))
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t) devConfig->hf3Source, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF3;
        }

        /* Validate Output frequency and Dvivider */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkHfFreqDiv(devConfig->hf3OutFreqMHz, devConfig->hf3Divider))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF3;
        }
    }

    /* Validate HF4 */
    if ((devConfig->clkHF4Enable) && (CY_PRA_CLKHF_4 < CY_SRSS_NUM_HFROOT))
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t) devConfig->hf4Source, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF4;
        }

        /* Validate Output frequency and Dvivider */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkHfFreqDiv(devConfig->hf4OutFreqMHz, devConfig->hf4Divider))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF4;
        }
    }

    /* Validate HF5 */
    if ((devConfig->clkHF5Enable) && (CY_PRA_CLKHF_5 < CY_SRSS_NUM_HFROOT))
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t) devConfig->hf5Source, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF5;
        }

        /* Validate Output frequency and Dvivider */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkHfFreqDiv(devConfig->hf5OutFreqMHz, devConfig->hf5Divider))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF5;
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkPump
****************************************************************************//**
*
* Validate PUMP Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKPUMP for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkPump(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq;
    cy_en_pra_status_t status = CY_PRA_STATUS_SUCCESS;
    /* Validate PUMP */
    if (devConfig->clkPumpEnable)
    {
        /* input source clock should be enabled */
        if (CY_PRA_STATUS_SUCCESS != Cy_PRA_ValidateClkPath((uint32_t) devConfig->pumpSource, devConfig))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKPUMP;
        }

        /* Validate Divider */
        if (devConfig->pumpDivider > CY_SYSCLK_PUMP_DIV_16)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKPUMP;
        }

        /* Output frequency range min="0" max="400000000" */
        status = Cy_PRA_GetInputSourceFreq((uint32_t) devConfig->pumpSource,devConfig, &freq);    
        if( status == CY_PRA_STATUS_SUCCESS )
        {
            freq = freq / (1UL << devConfig->pumpDivider); /* Calculate Output frequency */
            if (freq > CY_PRA_PUMP_OUT_MAX_FREQUENCY)
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKPUMP;
            }
        }
    }

    return status;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkBak
****************************************************************************//**
*
* Validate Backup Domain Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKBAK for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkBak(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_SUCCESS;
    uint32_t freq;

    if (devConfig->clkBakEnable)
    {
        /* Validate CLK_BAK source */
        switch (devConfig->clkBakSource)
        {
            case CY_SYSCLK_BAK_IN_WCO:
            {
                (devConfig->wcoEnable) ? (retStatus = CY_PRA_STATUS_SUCCESS) : (retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKBAK);
            }
            break;
            case CY_SYSCLK_BAK_IN_CLKLF:
            {
                (devConfig->clkLFEnable) ? (retStatus = CY_PRA_STATUS_SUCCESS) : (retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKBAK);
            }
            break;
            default:
            {
                retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKBAK;
            }
            break;
        }

        if (CY_PRA_STATUS_SUCCESS != retStatus)
        {
            return retStatus;
        }

        /* Validate Output Frequency min="0" max="100000" */
        /* There is no divider for CLK_BAK. So output frequency = input frequency */
        freq = Cy_PRA_GetClkBakFreq(devConfig);
        if (freq > CY_PRA_BAK_OUT_MAX_FREQUENCY)
        {
            retStatus = CY_PRA_STATUS_INVALID_PARAM_CLKBAK;
        }
    }

    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkFast
****************************************************************************//**
*
* Validate Fast Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKFAST for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkFast(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq;

    /* source clock (HF0) must be enabled */
    if (devConfig->clkFastEnable)
    {
        if (!(devConfig->clkHF0Enable))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKFAST;
        }

        /* Validate frequency range. min="0" max="400000000" */
        freq = devConfig->hf0OutFreqMHz*CY_PRA_FREQUENCY_HZ_CONVERSION; /* input frequency */
        freq = freq / (devConfig->clkFastDiv+1UL); /* Calculate Output frequency */
        if (freq > CY_PRA_FAST_OUT_MAX_FREQUENCY)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKFAST;
        }

        /* Validate divider min="1" max="256". User has to pass actual divider-1 */
        /* No need to validate divider because max value input can not be more that 255 */
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkPeri
****************************************************************************//**
*
* Validate Peripheral Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKPERI for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkPeri(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq;

    /* source clock (HF0) must be enabled */
    if (devConfig->clkPeriEnable)
    {
        if (!(devConfig->clkHF0Enable))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKPERI;
        }

        freq = devConfig->hf0OutFreqMHz*CY_PRA_FREQUENCY_HZ_CONVERSION; /* input frequency */
        if (freq > CY_HF_CLK_MAX_FREQ)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKPERI;
        }

        freq = freq / (devConfig->clkPeriDiv+1UL); /* Calculate Output frequency */

        /* Maximum of 25 MHz when the ULP mode is used and 100 MHz for LP mode */
        if (devConfig->ulpEnable)
        {
            if (freq > CY_PRA_ULP_MODE_HF0_MAX_FREQUENCY)
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKPERI;
            }
        }
        else
        {
            if (freq > CY_PRA_LP_MODE_MAX_FREQUENCY)
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKPERI;
            }
        }

        /* Validate divider min="1" max="256". User has to pass actual divider-1 */
        /* No need to validate divider because max value input can not be more that 255 */
    }
    else
    {
        /* check if this clock can't be disabled */
        return CY_PRA_STATUS_INVALID_PARAM_CLKPERI;
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkTimer
****************************************************************************//**
*
* Validate Timer Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkTimer(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq;
    uint8_t srcDiv;

    /* source clock must be enabled */
    if (devConfig->clkTimerEnable)
    {
        switch (devConfig->clkTimerSource)
        {
            case CY_SYSCLK_CLKTIMER_IN_IMO:
            {
                /* IMO is always on */
                freq = CY_PRA_IMO_SRC_FREQUENCY; /* 8 MHz */
                srcDiv = CY_PRA_DIVIDER_1;
            }
            break;
            case CY_SYSCLK_CLKTIMER_IN_HF0_NODIV:
            case CY_SYSCLK_CLKTIMER_IN_HF0_DIV2:
            case CY_SYSCLK_CLKTIMER_IN_HF0_DIV4:
            case CY_SYSCLK_CLKTIMER_IN_HF0_DIV8:
            {
                if ( devConfig->clkHF0Enable )
                {
                    srcDiv = CY_PRA_DIVIDER_1;
                    if (devConfig->clkTimerSource == CY_SYSCLK_CLKTIMER_IN_HF0_DIV2)
                    {
                        srcDiv = CY_PRA_DIVIDER_2;
                    }
                    if (devConfig->clkTimerSource == CY_SYSCLK_CLKTIMER_IN_HF0_DIV4)
                    {
                        srcDiv = CY_PRA_DIVIDER_4;
                    }
                    if (devConfig->clkTimerSource == CY_SYSCLK_CLKTIMER_IN_HF0_DIV8)
                    {
                        srcDiv = CY_PRA_DIVIDER_8;
                    }
                    freq = devConfig->hf0OutFreqMHz*CY_PRA_FREQUENCY_HZ_CONVERSION;
                }
                else
                {
                    return CY_PRA_STATUS_INVALID_PARAM_CLKTIMER;
                }
            }
            break;
            default:
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKTIMER;
            }
        }

        freq = (freq / (devConfig->clkTimerDivider + 1UL)) / srcDiv;
        /* Output frequency range min="0" max="400000000" */
        if (freq > CY_PRA_TIMER_OUT_MAX_FREQUENCY)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKTIMER;
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkSlow
****************************************************************************//**
*
* Validate Slow Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM_CLKPERI for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkSlow(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t freq;

    /* source clock must be enabled */
    if (devConfig->clkSlowEnable)
    {
        if (!(devConfig->clkPeriEnable))
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKSLOW;
        }

        /* outFreq = (sourceFreq / divider) range is min="0" max="400000000" */
        freq = devConfig->hf0OutFreqMHz*CY_PRA_FREQUENCY_HZ_CONVERSION; /* input frequency */
        if (freq > CY_HF_CLK_MAX_FREQ)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKPERI;
        }

        freq = freq / (devConfig->clkPeriDiv+1UL); /* Calculate Output frequency for PERI and Input Freq for SLOW_CLK */

        freq = freq / (devConfig->clkSlowDiv+1UL); /* Output CLK_SLOW frequency */

        if (freq > CY_PRA_SLOW_OUT_MAX_FREQUENCY)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKSLOW;
        }
    }
    else
    {
        /* check if this clock is always on */
        return CY_PRA_STATUS_INVALID_PARAM_CLKSLOW;
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateClkAltSysTick
****************************************************************************//**
*
* Validate Alt Sys Tick Clock
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM for bad parameter
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateClkAltSysTick(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t retStatus = CY_PRA_STATUS_SUCCESS;
    uint32_t freq;

    /* source clock must be enabled */
    if (devConfig->clkAltSysTickEnable)
    {
        switch (devConfig->clkSrcAltSysTick)
        {
            case CY_SYSTICK_CLOCK_SOURCE_CLK_LF:
            {
                if (devConfig->clkLFEnable)
                {
                    freq = Cy_PRA_GetClkLfFreq(devConfig);
                    retStatus = CY_PRA_STATUS_SUCCESS;
                }
                else
                {
                    retStatus = CY_PRA_STATUS_INVALID_PARAM_SYSTICK;
                }
            }
            break;
            case CY_SYSTICK_CLOCK_SOURCE_CLK_IMO:
            {
                freq = CY_PRA_IMO_SRC_FREQUENCY;
                retStatus = CY_PRA_STATUS_SUCCESS;
            }
            break;
            case CY_SYSTICK_CLOCK_SOURCE_CLK_ECO:
            {
                /* ECO is disabled for secure device */
                retStatus = CY_PRA_STATUS_INVALID_PARAM_SYSTICK;
            }
            break;
            case CY_SYSTICK_CLOCK_SOURCE_CLK_TIMER:
            {
                if (devConfig->clkTimerEnable)
                {
                    freq = Cy_PRA_GetClkTimerFreq(devConfig);
                    retStatus = CY_PRA_STATUS_SUCCESS;
                }
                else
                {
                    retStatus = CY_PRA_STATUS_INVALID_PARAM_SYSTICK;
                }
            }
            break;
            default:
            {
                retStatus = CY_PRA_STATUS_INVALID_PARAM_SYSTICK;
            }
            break;
        } /* switch */

        if (retStatus != CY_PRA_STATUS_SUCCESS)
        {
            return retStatus;
        }

        /* output frequency range min="0" max="400000000" */
        /* There is no divider for Timer so, output frequency = input frequency */
        if (freq > CY_PRA_SYSTICK_OUT_MAX_FREQUENCY)
        {
            retStatus = CY_PRA_STATUS_INVALID_PARAM_SYSTICK;
        }
    }

    return retStatus;
}

/*******************************************************************************
* Function Name: Cy_PRA_ValidateSystemConfig
****************************************************************************//**
*
* Validate System Configuration
*
* \param devConfig System Configuration Parameter
*
* \return
* CY_PRA_STATUS_SUCCESS for valid input configuration
* CY_PRA_STATUS_INVALID_PARAM generic failure in validation
* CY_PRA_STATUS_INVALID_PARAM_ECO Error in ECO validation
* CY_PRA_STATUS_INVALID_PARAM_EXTCLK Error in EXTCLK validation
* CY_PRA_STATUS_INVALID_PARAM_ALTHF Error in ALTHF validation
* CY_PRA_STATUS_INVALID_PARAM_ILO Error in ILO validation
* CY_PRA_STATUS_INVALID_PARAM_PILO Error in PILO validation
* CY_PRA_STATUS_INVALID_PARAM_WCO Error in WCO validation
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX0 Error in PATHMUX0 validation
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX1 Error in PATHMUX1 validation
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX2 Error in PATHMUX2 validation
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX3 Error in PATHMUX3 validation
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX4 Error in PATHMUX4 validation
* CY_PRA_STATUS_INVALID_PARAM_PATHMUX5 Error in PATHMUX5 validation
* CY_PRA_STATUS_INVALID_PARAM_FLL0 Error in FLL validation
* CY_PRA_STATUS_INVALID_PARAM_PLL0 Error in PLL0 validation
* CY_PRA_STATUS_INVALID_PARAM_PLL1 Error in PLL1 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKLF Error in CLKLF validation
* CY_PRA_STATUS_INVALID_PARAM_CLKHF0 Error in CLKHF0 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKHF1 Error in CLKHF1 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKHF2 Error in CLKHF2 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKHF3 Error in CLKHF3 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKHF4 Error in CLKHF4 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKHF5 Error in CLKHF5 validation
* CY_PRA_STATUS_INVALID_PARAM_CLKPUMP Error in CLKPUMP validation
* CY_PRA_STATUS_INVALID_PARAM_CLKBAK Error in CLKBAK validation
* CY_PRA_STATUS_INVALID_PARAM_CLKFAST Error in CLKFAST validation
* CY_PRA_STATUS_INVALID_PARAM_CLKPERI Error in CLKPERI validation
* CY_PRA_STATUS_INVALID_PARAM_CLKSLOW Error in CLKSLOW validation
* CY_PRA_STATUS_INVALID_PARAM_SYSTICK Error in CLKTICK validation
* CY_PRA_STATUS_INVALID_PARAM_CLKTIMER Error in CLKTIMER validation
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ValidateSystemConfig(const cy_stc_pra_system_config_t *devConfig)
{

     cy_en_pra_status_t retStatus;

    if (devConfig == NULL )
    {
        return CY_PRA_STATUS_INVALID_PARAM;
    }

    /* Validate IMO */
    /* IMO must be enabled for proper chip operation. So user option is not given for IMO.
     * The output frequency of IMO is fixed to 8MHz.
     */

     /* Validate ECO */
    retStatus = Cy_PRA_ValidateECO(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate EXTCLK */
    retStatus = Cy_PRA_ValidateEXTClk(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate ALTHF (BLE ECO) */
#if defined(CY_IP_MXBLESS)
    retStatus = Cy_PRA_ValidateAltHf(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }
#endif

    /* Validate ILO */
    /* ILO frequency fixed to 32KHz */

    /* Validate PILO */
    /* PILO frequency is fixed to 32.768KHz */

    /* Validate WCO */
    /* WCO frequency is fixed to 32.768KHz */

    /* Validate Path Mux */
    retStatus = Cy_PRA_ValidateAllClkPathMux(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate FLL */
    /* For ULP mode, Ffll_max = 50 MHz. For LP mode, Ffll_max = 100 MHz or Fcpu_max (if Fcpu_max < 100 MHz) */
    retStatus = Cy_PRA_ValidateFLL(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate PLLs */
    retStatus = Cy_PRA_ValidateAllPLL(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_LF */
    retStatus = Cy_PRA_ValidateClkLf(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_HF */
    retStatus = Cy_PRA_ValidateClkHFs(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_PUMP */
    retStatus = Cy_PRA_ValidateClkPump(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_BAK */
    retStatus = Cy_PRA_ValidateClkBak(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_FAST */
    retStatus = Cy_PRA_ValidateClkFast(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_PERI */
    retStatus = Cy_PRA_ValidateClkPeri(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_TIMER */
    retStatus = Cy_PRA_ValidateClkTimer(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_SLOW */
    retStatus = Cy_PRA_ValidateClkSlow(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

    /* Validate CLK_ALT_SYS_TICK */
    retStatus = Cy_PRA_ValidateClkAltSysTick(devConfig);
    if (CY_PRA_STATUS_SUCCESS != retStatus)
    {
        return retStatus;
    }

     return CY_PRA_STATUS_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_PRA_CalculateFLLOutFreq
****************************************************************************//**
*
* Calculate and return FLL output frequency
*
* \param devConfig
*
* \return
* FLL output frequency
*
*******************************************************************************/
uint32_t Cy_PRA_CalculateFLLOutFreq(const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t srcFreq;
    uint32_t outFreq=0UL;

    srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path0Src, devConfig);
    if ((srcFreq != CY_PRA_DEFAULT_SRC_FREQUENCY) && (devConfig->fllRefDiv != 0UL))
    {
        outFreq = ((srcFreq * devConfig->fllMult) / devConfig->fllRefDiv) / (devConfig->enableOutputDiv ? 2UL : 1UL);
    }

    return outFreq;
}

/*******************************************************************************
* Function Name: Cy_PRA_CalculatePLLOutFreq
****************************************************************************//**
*
* Calculate and return PLL output frequency
*
* \param devConfig
*
* \return
* PLL output frequency
*
*******************************************************************************/
uint32_t Cy_PRA_CalculatePLLOutFreq(uint8_t pll, const cy_stc_pra_system_config_t *devConfig)
{
    uint32_t srcFreq;
    uint32_t outFreq;
    uint32_t feedback;
    uint32_t reference;
    uint32_t pllOutputDiv;

    if ((pll > CY_PRA_DEFAULT_ZERO) && (pll <= CY_SRSS_NUM_PLL)) /* 0 is invalid pll number */
    {
        if (pll == CY_PRA_CLKPLL_1)
        {
            srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path1Src, devConfig);
            feedback = devConfig->pll0FeedbackDiv;
            reference = devConfig->pll0ReferenceDiv;
            pllOutputDiv = devConfig->pll0OutputDiv;
        }
        else if (pll == CY_PRA_CLKPLL_2)
        {
            srcFreq = Cy_PRA_GetInputPathMuxFrq(devConfig->path2Src, devConfig);
            feedback = devConfig->pll1FeedbackDiv;
            reference = devConfig->pll1ReferenceDiv;
            pllOutputDiv = devConfig->pll1OutputDiv;
        }
        else
        {
            srcFreq = CY_PRA_DEFAULT_SRC_FREQUENCY;
        }

        if ((srcFreq != CY_PRA_DEFAULT_SRC_FREQUENCY) && (reference != CY_PRA_DIVIDER_0) && (pllOutputDiv != CY_PRA_DIVIDER_0))
        {
            outFreq = ((srcFreq * feedback) / reference) / pllOutputDiv;
        }
        else
        {
            outFreq = CY_PRA_DEFAULT_ZERO;
        }
    }
    else
    {
        outFreq = CY_PRA_DEFAULT_ZERO;
    }

    return outFreq;
}

#endif /* (CY_CPU_CORTEX_M0P) */


/*******************************************************************************
* Function Name: Cy_PRA_SystemConfig
****************************************************************************//**
*
* Initializes and Configure Device
*
* \param devConfig
*
* \return
* Status of the function processing.
*
*******************************************************************************/
cy_en_pra_status_t Cy_PRA_SystemConfig(const cy_stc_pra_system_config_t *devConfig)
{
    cy_en_pra_status_t status;

#if (CY_CPU_CORTEX_M4)
    (void) devConfig;
    status = CY_PRA_STATUS_SUCCESS;
#else

    cy_en_sysclk_status_t sysClkStatus;
    bool tmp, tmp1, tmp2;


    /* Validate input parameters */
    status = Cy_PRA_ValidateSystemConfig(devConfig);
    if (CY_PRA_STATUS_SUCCESS != status)
    {
        return status;
    }

    /* Set worst case memory wait states (! ultra low power, 150 MHz), will update at the end */
    Cy_SysLib_SetWaitStates(false, CY_PRA_150MHZ_FREQUENCY);
    if (devConfig->powerEnable)
    {
        status = Cy_PRA_PowerInit(devConfig);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }

    /* Reset the core clock path to default and disable all the FLLs/PLLs */
    sysClkStatus = Cy_SysClk_ClkHfSetDivider(CY_PRA_CLKHF_0, CY_SYSCLK_CLKHF_NO_DIVIDE);
    if (CY_SYSCLK_SUCCESS != sysClkStatus)
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0;
    }


    tmp = Cy_SysClk_FllIsEnabled();
    if (CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_1)
    {
        tmp1 = Cy_SysClk_PllIsEnabled(CY_PRA_CLKPLL_1);
    }
    if (CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_2)
    {
        tmp2 = Cy_SysClk_PllIsEnabled(CY_PRA_CLKPLL_2);
    }
    if ((tmp  != (devConfig->fllEnable))  ||
        ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_1) && (tmp1 != (devConfig->pll0Enable))) ||
        ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_2) && (tmp2 != (devConfig->pll1Enable))))
    {
        Cy_SysClk_ClkFastSetDivider(CY_PRA_DIVIDER_0);
        Cy_SysClk_ClkPeriSetDivider(CY_PRA_DIVIDER_1);
        Cy_SysClk_ClkSlowSetDivider(CY_PRA_DIVIDER_0);

        if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_1) && (!devConfig->pll0Enable))
        {
            sysClkStatus = Cy_SysClk_PllDisable(CY_PRA_CLKPLL_1);
        }

        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PLL0;
        }

        if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_2) && (!devConfig->pll1Enable))
        {
            sysClkStatus = Cy_SysClk_PllDisable(CY_PRA_CLKPLL_2);
        }

        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PLL1;
        }

        sysClkStatus = Cy_SysClk_ClkPathSetSource((uint32_t) CY_SYSCLK_CLKHF_IN_CLKPATH1, CY_SYSCLK_CLKPATH_IN_IMO);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX1;
        }

        if (CY_SYSCLK_CLKHF_IN_CLKPATH0 == Cy_SysClk_ClkHfGetSource(CY_PRA_CLKHF_0))
        {
                if (CY_SYSCLK_CLKPATH_IN_WCO == Cy_SysClk_ClkPathGetSource((uint32_t) CY_SYSCLK_CLKHF_IN_CLKPATH0))
                {
                            sysClkStatus = Cy_SysClk_ClkHfSetSource(CY_PRA_CLKHF_0, CY_SYSCLK_CLKHF_IN_CLKPATH1);
                            if (CY_SYSCLK_SUCCESS != sysClkStatus)
                            {
                                        return CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0;
                            }
                }
        }

        if (!devConfig->fllEnable)
        {
            sysClkStatus = Cy_SysClk_FllDisable();
        }
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_FLL0;
        }

        sysClkStatus = Cy_SysClk_ClkPathSetSource((uint32_t) CY_SYSCLK_CLKHF_IN_CLKPATH0, CY_SYSCLK_CLKPATH_IN_IMO);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX0;
        }

        sysClkStatus = Cy_SysClk_ClkHfSetSource(CY_PRA_CLKHF_0, CY_SYSCLK_CLKHF_IN_CLKPATH0);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0;
        }
    }
#ifdef CY_IP_MXBLESS
    (void)Cy_BLE_EcoReset();
#endif

    /* Enable all source clocks */
    if (devConfig->piloEnable)
    {
        Cy_PRA_PiloInit();
    }
    else
    {
        Cy_SysClk_PiloDisable();
    }

    if (devConfig->wcoEnable)
    {
        status = Cy_PRA_WcoInit(devConfig);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }
    else
    {
        Cy_SysClk_WcoDisable();
    }

    if (devConfig->clkLFEnable)
    {
        status = Cy_PRA_ClkLfInit(devConfig->clkLfSource);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }
#if defined(CY_IP_MXBLESS)
    if (devConfig->clkAltHfEnable)
    {
        status = Cy_PRA_AltHfInit(devConfig);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }
#endif /* CY_IP_MXBLESS */
    tmp = (CY_SYSCLK_ECOSTAT_STABLE == Cy_SysClk_EcoGetStatus());
    if (tmp != (devConfig->ecoEnable))
    {
        if (devConfig->ecoEnable)
        {
            status = Cy_PRA_EcoInit(devConfig);
            if (CY_PRA_STATUS_SUCCESS != status)
            {
                return status;
            }
        }
        else
        {
            Cy_SysClk_EcoDisable();
        }
    }

    if (devConfig->extClkEnable)
    {
        Cy_PRA_ExtClkInit(devConfig);
    }

    if (devConfig->clkFastEnable)
    {
        Cy_SysClk_ClkFastSetDivider(devConfig->clkFastDiv);
    }

    if (devConfig->clkPeriEnable)
    {
        Cy_SysClk_ClkPeriSetDivider(devConfig->clkPeriDiv);
    }

    if (devConfig->clkSlowEnable)
    {
        Cy_SysClk_ClkSlowSetDivider(devConfig->clkSlowDiv);
    }

    if ((devConfig->path0Src == CY_SYSCLK_CLKPATH_IN_WCO) &&
        (devConfig->hf0Source == CY_SYSCLK_CLKHF_IN_CLKPATH0))
    {
        /* Configure HFCLK0 to temporarily run from IMO to initialize other clocks */
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_1, CY_SYSCLK_CLKPATH_IN_IMO);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX1;
        }
        sysClkStatus = Cy_SysClk_ClkHfSetSource(CY_PRA_CLKHF_0, CY_SYSCLK_CLKHF_IN_CLKPATH1);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0;
        }
    }
    else
    {
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_1, devConfig->path1Src);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX1;
        }
    }

    /* Configure Path Clocks */

    if (devConfig->path0Enable)
    {
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_0, devConfig->path0Src);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX0;
        }
    }

    if (devConfig->path2Enable)
    {
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_2, devConfig->path2Src);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX2;
        }
    }

    if ((devConfig->path3Enable) && (CY_PRA_CLKPATH_3 < CY_SRSS_NUM_CLKPATH))
    {
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_3, devConfig->path3Src);
        if (CY_SYSCLK_SUCCESS != sysClkStatus) 
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX3;
        }
    }

    if ((devConfig->path4Enable) && (CY_PRA_CLKPATH_4 < CY_SRSS_NUM_CLKPATH))
    {
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_4, devConfig->path4Src);
        if (CY_SYSCLK_SUCCESS != sysClkStatus) 
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX4;
        }
    }

    if ((devConfig->path5Enable) && (CY_PRA_CLKPATH_5 < CY_SRSS_NUM_CLKPATH))
    {
        sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_5, devConfig->path5Src);
        if (CY_SYSCLK_SUCCESS != sysClkStatus)
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX5;
        }
    }

    if ((!Cy_SysClk_FllIsEnabled()) && (devConfig->fllEnable))
    {
        status = Cy_PRA_FllInit(devConfig);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }

    /* ClkHf0Init */
    sysClkStatus = Cy_SysClk_ClkHfSetSource(CY_PRA_CLKHF_0, devConfig->hf0Source);
    if (CY_SYSCLK_SUCCESS != sysClkStatus)
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0;
    }
    sysClkStatus = Cy_SysClk_ClkHfSetDivider(CY_PRA_CLKHF_0, devConfig->hf0Divider);
    if (CY_SYSCLK_SUCCESS != sysClkStatus)
    {
        return CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0;
    }

    if ((devConfig->path0Src == CY_SYSCLK_CLKPATH_IN_WCO) && (devConfig->hf0Source == CY_SYSCLK_CLKHF_IN_CLKPATH0))
    {
        if (devConfig->path1Enable)
        {
            sysClkStatus = Cy_SysClk_ClkPathSetSource(CY_PRA_CLKPATH_1, devConfig->path1Src);
            if (CY_SYSCLK_SUCCESS != sysClkStatus)
            {
                return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX1;
            }
        }
        else
        {
            return CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX1;
        }

    }

    if ((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_1) && (devConfig->pll0Enable))
    {
        if (!Cy_SysClk_PllIsEnabled(CY_PRA_CLKPLL_1))
        {
            const cy_stc_pll_manual_config_t pll0Config =
            {
                .feedbackDiv = devConfig->pll0FeedbackDiv,
                .referenceDiv = devConfig->pll0ReferenceDiv,
                .outputDiv = devConfig->pll0OutputDiv,
                .lfMode = devConfig->pll0LfMode,
                .outputMode = devConfig->pll0OutputMode,
            };

            status = Cy_PRA_PllInit(CY_PRA_CLKPLL_1, &pll0Config);
            if (CY_PRA_STATUS_SUCCESS != status)
            {
                return CY_PRA_STATUS_ERROR_PROCESSING_PLL0;
            }
        }
    }

    if((CY_SRSS_NUM_PLL >= CY_PRA_CLKPLL_2) && (devConfig->pll1Enable))
    {
        if (!Cy_SysClk_PllIsEnabled(CY_PRA_CLKPLL_2))
        {
            const cy_stc_pll_manual_config_t pll1Config =
            {
                .feedbackDiv = devConfig->pll1FeedbackDiv,
                .referenceDiv = devConfig->pll1ReferenceDiv,
                .outputDiv = devConfig->pll1OutputDiv,
                .lfMode = devConfig->pll1LfMode,
                .outputMode = devConfig->pll1OutputMode,
            };

            status = Cy_PRA_PllInit(CY_PRA_CLKPLL_2, &pll1Config);
            if (CY_PRA_STATUS_SUCCESS != status)
            {
                return CY_PRA_STATUS_ERROR_PROCESSING_PLL1;
            }
        }
    }

    /* Configure HF clocks */
    if (devConfig->clkHF1Enable)
    {
        status = Cy_PRA_ClkHfInit(CY_PRA_CLKHF_1, devConfig->hf1Source, devConfig->hf1Divider);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF1;
        }
    }
    else
    {
        status = (cy_en_pra_status_t)Cy_SysClk_ClkHfDisable(CY_PRA_CLKHF_1);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF1;
        }
    }
    if (devConfig->clkHF2Enable)
    {
        status = Cy_PRA_ClkHfInit(CY_PRA_CLKHF_2, devConfig->hf2Source, devConfig->hf2Divider);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF2;
        }
    }
    else
    {
        status = (cy_en_pra_status_t)Cy_SysClk_ClkHfDisable(CY_PRA_CLKHF_2);
        if (CY_PRA_STATUS_SUCCESS != status) 
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF2;
        }

    }
    if ((devConfig->clkHF3Enable) && (CY_PRA_CLKHF_3 < CY_SRSS_NUM_HFROOT))
    {
        status = Cy_PRA_ClkHfInit(CY_PRA_CLKHF_3, devConfig->hf3Source, devConfig->hf3Divider);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF3;
        }
    }
    else
    {
        if (CY_PRA_CLKHF_3 < CY_SRSS_NUM_HFROOT)
        {
            status = (cy_en_pra_status_t)Cy_SysClk_ClkHfDisable(CY_PRA_CLKHF_3);
            if (CY_PRA_STATUS_SUCCESS != status)
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKHF3;
            }
        }

    }
    if ((devConfig->clkHF4Enable) && (CY_PRA_CLKHF_4 < CY_SRSS_NUM_HFROOT))
    {
        status = Cy_PRA_ClkHfInit(CY_PRA_CLKHF_4, devConfig->hf4Source, devConfig->hf4Divider);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF4;
        }
    }
    else
    {
        if(CY_PRA_CLKHF_4 < CY_SRSS_NUM_HFROOT)
        {
            status = (cy_en_pra_status_t)Cy_SysClk_ClkHfDisable(CY_PRA_CLKHF_4);
            if (CY_PRA_STATUS_SUCCESS != status)
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKHF4;
            }
        }
    }
    if ((devConfig->clkHF5Enable) && (CY_PRA_CLKHF_5 < CY_SRSS_NUM_HFROOT))
    {
        status = Cy_PRA_ClkHfInit(CY_PRA_CLKHF_5, devConfig->hf5Source, devConfig->hf5Divider);
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return CY_PRA_STATUS_INVALID_PARAM_CLKHF5;
        }
    }
    else
    {
        if(CY_PRA_CLKHF_5 < CY_SRSS_NUM_HFROOT)
        {
            status = (cy_en_pra_status_t)Cy_SysClk_ClkHfDisable(CY_PRA_CLKHF_5);
            if (CY_PRA_STATUS_SUCCESS != status)
            {
                return CY_PRA_STATUS_INVALID_PARAM_CLKHF5;
            }
        }
    }
    /* Configure miscellaneous clocks */
    if (devConfig->clkTimerEnable)
    {
        Cy_PRA_ClkTimerInit(devConfig->clkTimerSource, devConfig->clkTimerDivider);
    }
    else
    {
         Cy_SysClk_ClkTimerDisable();
    }

    if (devConfig->clkAltSysTickEnable)
    {
        Cy_SysTick_SetClockSource(devConfig->clkSrcAltSysTick);
    }

    if (devConfig->clkPumpEnable)
    {
        Cy_PRA_ClkPumpInit(devConfig->pumpSource, devConfig->pumpDivider);
    }
    else
    {
        Cy_SysClk_ClkPumpDisable();
    }

    if (devConfig->clkBakEnable)
    {
        Cy_SysClk_ClkBakSetSource(devConfig->clkBakSource);
    }

    status = Cy_PRA_IloHibernateOn(devConfig->iloHibernateON);
    if (CY_PRA_STATUS_SUCCESS != status)
    {
        return status;
    }

    /* Configure default enabled clocks */
    if (devConfig->iloEnable)
    {
        status = Cy_PRA_IloEnable();
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }
    else
    {
        status = Cy_PRA_IloDisable();
        if (CY_PRA_STATUS_SUCCESS != status)
        {
            return status;
        }
    }

    /* SYSCLK MFO INIT */
    /* SYSCLK MF INIT */

    /* Set accurate flash wait states */
    if ((devConfig->powerEnable) && (devConfig->clkHF0Enable))
    {
        Cy_SysLib_SetWaitStates(devConfig->ulpEnable, devConfig->hf0OutFreqMHz);
    }

    /* Update System Core Clock values for correct Cy_SysLib_Delay functioning */
    SystemCoreClockUpdate();

#endif /* (CY_CPU_CORTEX_M4) */

    return (status);
}


#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)

    #if !defined(CY_DEVICE_PSOC6ABLE2)
        #define CY_PRA_ALL_PC_MASK      (CY_PROT_PCMASK1 + CY_PROT_PCMASK2 + \
                                        CY_PROT_PCMASK3 + CY_PROT_PCMASK4 + \
                                        CY_PROT_PCMASK5 + CY_PROT_PCMASK6 + \
                                        CY_PROT_PCMASK7)

        #define CY_PRA_SECURE_PC_MASK   (CY_PROT_PCMASK1 + CY_PROT_PCMASK2 + \
                                        CY_PROT_PCMASK3 + CY_PROT_PCMASK4)
    #endif /* !defined(CY_DEVICE_PSOC6ABLE2) */


/*******************************************************************************
* Function Name: Cy_PRA_CloseSrssMain2
****************************************************************************//**
*
* Restricts access to the SRSS_MAIN2 region that includes following register:
* SRSS_TST_DDFT_FAST_CTL_REG, SRSS_TST_DDFT_FAST_CTL_REG.
*
*******************************************************************************/
void Cy_PRA_CloseSrssMain2(void)
{
    #if defined(CY_DEVICE_PSOC6ABLE2)
        /* Will be Implemented for BLE device as per DRIVERS-3084 */
    #else
        (void) Cy_Prot_ConfigPpuFixedSlaveAtt(PERI_MS_PPU_FX_SRSS_MAIN2,
                                    (uint16_t) CY_PRA_SECURE_PC_MASK,
                                    CY_PROT_PERM_RW,
                                    CY_PROT_PERM_RW,
                                    false);

        (void) Cy_Prot_ConfigPpuFixedSlaveAtt(PERI_MS_PPU_FX_SRSS_MAIN2,
                                    (uint16_t) (CY_PRA_ALL_PC_MASK ^ CY_PRA_SECURE_PC_MASK),
                                    CY_PROT_PERM_DISABLED,
                                    CY_PROT_PERM_DISABLED,
                                    false);
    #endif /* defined(CY_DEVICE_PSOC6ABLE2) */
}


/*******************************************************************************
* Function Name: Cy_PRA_OpenSrssMain2
****************************************************************************//**
*
* Restores access to the SRSS_MAIN2 region that was restrcited by
* \ref Cy_PRA_CloseSrssMain2.
*
*******************************************************************************/
void Cy_PRA_OpenSrssMain2(void)
{
    #if defined(CY_DEVICE_PSOC6ABLE2)
        /* Will be Implemented for BLE device as per DRIVERS-3084 */
    #else
        (void) Cy_Prot_ConfigPpuFixedSlaveAtt(PERI_MS_PPU_FX_SRSS_MAIN2,
                                    (uint16_t) CY_PRA_ALL_PC_MASK,
                                    CY_PROT_PERM_RW,
                                    CY_PROT_PERM_RW,
                                    false);
    #endif /* defined(CY_DEVICE_PSOC6ABLE2) */
}

#endif /* (CY_CPU_CORTEX_M0P) */


#endif /* (CY_DEVICE_SECURE) */


/* [] END OF FILE */
