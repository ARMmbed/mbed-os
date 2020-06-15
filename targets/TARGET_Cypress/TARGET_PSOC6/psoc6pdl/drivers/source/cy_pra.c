/***************************************************************************//**
* \file cy_pra.c
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

#include "cy_pra.h"
#include "cy_pra_cfg.h"
#include "cy_sysint.h"
#include "cy_ipc_drv.h"
#include "cy_gpio.h"
#include "cy_device.h"
#include "cy_syspm.h"

#if defined (CY_DEVICE_SECURE) || defined (CY_DOXYGEN)

#define CY_PRA_REG_POLICY_WRITE_ALL   (0x00000000UL)
#define CY_PRA_REG_POLICY_WRITE_NONE  (0xFFFFFFFFUL)

/* Table to get register/function address based on its index */
cy_stc_pra_reg_policy_t regIndexToAddr[CY_PRA_REG_INDEX_COUNT];

#if (CY_CPU_CORTEX_M4)
    static IPC_STRUCT_Type *ipcPraBase = NULL;
#endif /* (CY_CPU_CORTEX_M0P) */


#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)
    static void Cy_PRA_Handler(void);
    static void Cy_PRA_ProcessCmd(cy_stc_pra_msg_t *message);
    static void Cy_PRA_PmHibernate(uint32_t funcProc);
    static void Cy_PRA_PmCm4DpFlagSet(void);
    static cy_en_pra_status_t Cy_PRA_ClkDSBeforeTransition(void);
    static cy_en_pra_status_t Cy_PRA_ClkDSAfterTransition(void);
    static bool Cy_PRA_RegAccessRangeValid(uint16_t index);
#endif /* (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN) */


/*******************************************************************************
* Function Name: Cy_PRA_Init
****************************************************************************//**
*
* Initializes the Protected Register Access driver.
*
*******************************************************************************/
void Cy_PRA_Init(void)
{

#if (CY_CPU_CORTEX_M0P)
    for (uint32_t i = 0UL; i < CY_PRA_REG_INDEX_COUNT; i++)
    {
        regIndexToAddr[i].writeMask = CY_PRA_REG_POLICY_WRITE_ALL;
    }
    regIndexToAddr[CY_PRA_INDX_SRSS_PWR_LVD_CTL].addr          = &SRSS_PWR_LVD_CTL;
    regIndexToAddr[CY_PRA_INDX_SRSS_SRSS_INTR].addr            = &SRSS_SRSS_INTR;
    regIndexToAddr[CY_PRA_INDX_SRSS_SRSS_INTR_SET].addr        = &SRSS_SRSS_INTR_SET;
    regIndexToAddr[CY_PRA_INDX_SRSS_SRSS_INTR_MASK].addr       = &SRSS_SRSS_INTR_MASK;
    regIndexToAddr[CY_PRA_INDX_SRSS_SRSS_INTR_CFG].addr        = &SRSS_SRSS_INTR_CFG;
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_1].addr    = &SRSS_CLK_ROOT_SELECT[1U];
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_2].addr    = &SRSS_CLK_ROOT_SELECT[2U];
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_3].addr    = &SRSS_CLK_ROOT_SELECT[3U];
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_4].addr    = &SRSS_CLK_ROOT_SELECT[4U];
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_5].addr    = (CY_SRSS_NUM_HFROOT > 4U) ? &SRSS_CLK_ROOT_SELECT[5U] : 0U;
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_6].addr    = (CY_SRSS_NUM_HFROOT > 5U) ? &SRSS_CLK_ROOT_SELECT[6U] : 0U;
    regIndexToAddr[CY_PRA_INDX_FLASHC_FLASH_CMD].addr          = &FLASHC_FLASH_CMD;
    regIndexToAddr[CY_PRA_INDX_SRSS_PWR_HIBERNATE].addr        = &SRSS_PWR_HIBERNATE;
    regIndexToAddr[CY_PRA_INDX_SRSS_PWR_HIBERNATE].writeMask   = (uint32_t) ~ (SRSS_PWR_HIBERNATE_TOKEN_Msk |
                                                                               SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |
                                                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk |
                                                                               SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |
                                                                               SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk);
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_MFO_CONFIG].addr       = &SRSS_CLK_MFO_CONFIG;
    regIndexToAddr[CY_PRA_INDX_SRSS_CLK_MF_SELECT].addr        = &SRSS_CLK_MF_SELECT;
    regIndexToAddr[CY_PRA_INDX_FLASHC_FM_CTL_BOOKMARK].addr     = &FLASHC_FM_CTL_BOOKMARK;
    regIndexToAddr[CY_PRA_INDX_FLASHC_FM_CTL_BOOKMARK].writeMask= CY_PRA_REG_POLICY_WRITE_NONE;

    /* Configure the IPC interrupt handler. */
    Cy_IPC_Drv_SetInterruptMask(Cy_IPC_Drv_GetIntrBaseAddr(CY_IPC_INTR_PRA), CY_PRA_IPC_NONE_INTR, CY_PRA_IPC_CHAN_INTR);
    cy_stc_sysint_t intr = {
        .intrSrc = (IRQn_Type)CY_SYSINT_CM0P_MUX4,
        .cm0pSrc = (cy_en_intr_t)(int32_t) CY_IPC_INTR_NUM_TO_VECT((int32_t) CY_IPC_INTR_PRA),
        .intrPriority = 0UL
    };
    (void) Cy_SysInt_Init(&intr, &Cy_PRA_Handler);
    NVIC_EnableIRQ(intr.intrSrc);
#else

    /* Need to get this address in RAM, because there are use cases
    *  where this address is used but flash is not accesible
    */
    ipcPraBase = Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_PRA);
#endif /* (CY_CPU_CORTEX_M0P) */
}


#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)
/*******************************************************************************
* Function Name: Cy_PRA_Handler
****************************************************************************//**
*
* The IPC interrupt handler called once there is request from non-secure core.
*
*******************************************************************************/
static void Cy_PRA_Handler(void)
{
    cy_stc_pra_msg_t msgLocal;
    cy_stc_pra_msg_t* msgRemote;

    /* Process internal command's copy and update original value */
    msgRemote = (cy_stc_pra_msg_t *)Cy_IPC_Drv_ReadDataValue(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_PRA));

    msgLocal = *msgRemote;
    Cy_PRA_ProcessCmd(&msgLocal);
    *msgRemote = msgLocal;

    /* Clear interrupt logic. Required to detect next interrupt */
    Cy_IPC_Drv_ClearInterrupt(Cy_IPC_Drv_GetIntrBaseAddr(CY_IPC_INTR_PRA), CY_PRA_IPC_NONE_INTR, CY_PRA_IPC_CHAN_INTR);

    (void) Cy_IPC_Drv_LockRelease(Cy_IPC_Drv_GetIpcBaseAddress(CY_IPC_CHAN_PRA), CY_PRA_IPC_NONE_INTR);
}


/*******************************************************************************
* Function Name: Cy_PRA_ProcessCmd
****************************************************************************//**
*
* Process and executes the command received from non-secure core.
*
* \param message \ref cy_stc_pra_msg_t
*
*******************************************************************************/
static void Cy_PRA_ProcessCmd(cy_stc_pra_msg_t *message)
{
    static uint32_t structInit = CY_PRA_STRUCT_NOT_INITIALIZED;
    static cy_stc_pra_system_config_t structCpy = {0UL};

    CY_ASSERT_L1(NULL != message);


    switch (message->praCommand)
    {
        case CY_PRA_MSG_TYPE_REG32_CLR_SET:
            /* Report error if any of the following conditions is false:
            *  - New value (message->praData2) has zeros in the write-protected fields
            *  - Register index is within the valid range
            */
            if ((0U == (message->praData2 & regIndexToAddr[message->praIndex].writeMask)) &&
                (CY_PRA_REG_POLICY_WRITE_NONE != regIndexToAddr[message->praIndex].writeMask) &&
                (Cy_PRA_RegAccessRangeValid(message->praIndex)))
            {
                uint32_t tmp;

                tmp =  CY_GET_REG32(regIndexToAddr[message->praIndex].addr);

                tmp &= (message->praData1 | regIndexToAddr[message->praIndex].writeMask);
                tmp |= message->praData2;
                CY_SET_REG32(regIndexToAddr[message->praIndex].addr, tmp);
                message->praStatus = CY_PRA_STATUS_SUCCESS;
            }
            else
            {
                message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
            }
            break;

        case CY_PRA_MSG_TYPE_REG32_SET:
            /* Report error if any of the following conditions is false:
            *  - New value (message->praData1) has zeros in the write-protected fields
            *  - Register index is within the valid range
            */
            if ((0U == (message->praData1 & regIndexToAddr[message->praIndex].writeMask)) &&
                (CY_PRA_REG_POLICY_WRITE_NONE != regIndexToAddr[message->praIndex].writeMask) &&
                (Cy_PRA_RegAccessRangeValid(message->praIndex)))
            {
                uint32_t tmp;

                tmp =  CY_GET_REG32(regIndexToAddr[message->praIndex].addr);

                /* Clear bits allowed to write */
                tmp &= regIndexToAddr[message->praIndex].writeMask;

                /* Set allowed bits based on new value.
                   Write-protected fields have zeros in the new value, so no additional checks needed
                */
                tmp |= message->praData1;
                CY_SET_REG32(regIndexToAddr[message->praIndex].addr, tmp);
                message->praStatus = CY_PRA_STATUS_SUCCESS;
            }
            else
            {
                message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
            }
            break;

        case CY_PRA_MSG_TYPE_REG32_GET:
            if (Cy_PRA_RegAccessRangeValid(message->praIndex))
            {
                message->praData1 = CY_GET_REG32(regIndexToAddr[message->praIndex].addr);
                message->praStatus = CY_PRA_STATUS_SUCCESS;
            }
            else
            {
                message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
            }
            break;

        case CY_PRA_MSG_TYPE_CM0_WAKEUP:
            message->praStatus = CY_PRA_STATUS_SUCCESS;
            break;

        case CY_PRA_MSG_TYPE_SYS_CFG_FUNC:
            CY_ASSERT_L1((cy_stc_pra_system_config_t *)(message->praData1) != NULL);
            if( NULL != (cy_stc_pra_system_config_t *)(message->praData1))
            {
                structCpy = *((cy_stc_pra_system_config_t *)(message->praData1));
                message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                if((CY_PRA_STRUCT_NOT_INITIALIZED == structInit) && (CY_PRA_STATUS_SUCCESS == message->praStatus))
                {
                    structInit = CY_PRA_STRUCT_INITIALIZED;
                }
            }
            else
            {
                message->praStatus = CY_PRA_STATUS_INVALID_PARAM;
            }
            break;

        case CY_PRA_MSG_TYPE_SECURE_ONLY:
            switch (message->praIndex)
            {
                case CY_PRA_PM_FUNC_HIBERNATE:
                    Cy_PRA_PmHibernate(message->praData1);
                    message->praStatus = CY_PRA_STATUS_SUCCESS;
                    break;

                case CY_PRA_PM_FUNC_CM4_DP_FLAG_SET:
                    Cy_PRA_PmCm4DpFlagSet();
                    message->praStatus = CY_PRA_STATUS_SUCCESS;
                    break;

                case CY_PRA_CLK_FUNC_DS_BEFORE_TRANSITION:
                    message->praStatus = Cy_PRA_ClkDSBeforeTransition();
                    break;

                case CY_PRA_CLK_FUNC_DS_AFTER_TRANSITION:
                    message->praStatus = Cy_PRA_ClkDSAfterTransition();
                    break;

                case CY_PRA_PM_FUNC_BUCK_ENABLE_VOLTAGE2:
                    Cy_SysPm_BuckEnableVoltage2();
                    message->praStatus = CY_PRA_STATUS_SUCCESS;
                    break;

                case CY_PRA_PM_FUNC_BUCK_DISABLE_VOLTAGE2:
                    Cy_SysPm_BuckDisableVoltage2();
                    message->praStatus = CY_PRA_STATUS_SUCCESS;
                    break;

                case CY_PRA_PM_FUNC_BUCK_VOLTAGE2_HW_CTRL:
                    Cy_SysPm_BuckSetVoltage2HwControl((bool) message->praData1);
                    message->praStatus = CY_PRA_STATUS_SUCCESS;
                    break;

                case CY_PRA_PM_FUNC_BUCK_SET_VOLTAGE2:
                    if (CY_SYSPM_IS_BUCK_VOLTAGE2_VALID(((cy_stc_pra_voltage2_t *) message->praData1)->praVoltage))
                    {
                        Cy_SysPm_BuckSetVoltage2(((cy_stc_pra_voltage2_t *) message->praData1)->praVoltage,
                                                 ((cy_stc_pra_voltage2_t *) message->praData1)->praWaitToSettle);
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                    }
                    else
                    {
                        message->praStatus = CY_PRA_STATUS_INVALID_PARAM;
                    }
                    break;

                default:
                    message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                    break;
            }
            break;

        case CY_PRA_MSG_TYPE_FUNC_POLICY:
            if(CY_PRA_STRUCT_NOT_INITIALIZED != structInit)
            {
                switch (message->praIndex)
                {
                    case CY_PRA_PM_FUNC_LDO_SET_VOLTAGE:
                    {
                        bool powerEnableTmp, ldoEnableTmp, ulpEnableTmp;
                        powerEnableTmp = structCpy.powerEnable; /* old value backup */
                        ldoEnableTmp = structCpy.ldoEnable; /* old value backup */
                        ulpEnableTmp = structCpy.ulpEnable; /* old value backup */
                        structCpy.powerEnable = true;
                        structCpy.ldoEnable = true;
                        structCpy.ldoVoltage = (cy_en_syspm_ldo_voltage_t)message->praData1;
                        if (structCpy.ldoVoltage == CY_SYSPM_LDO_VOLTAGE_0_9V)
                        {
                            structCpy.ulpEnable = true;
                        }
                        else
                        {
                            structCpy.ulpEnable = false;
                        }
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.powerEnable = powerEnableTmp;
                            structCpy.ldoEnable = ldoEnableTmp;
                            structCpy.ulpEnable = ulpEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_PM_FUNC_BUCK_ENABLE:
                    {
                        bool powerEnableTmp, ldoEnableTmp, ulpEnableTmp;
                        powerEnableTmp = structCpy.powerEnable; /* old value backup */
                        ldoEnableTmp = structCpy.ldoEnable; /* old value backup */
                        ulpEnableTmp = structCpy.ulpEnable; /* old value backup */
                        structCpy.powerEnable = true;
                        structCpy.ldoEnable = false;
                        structCpy.buckVoltage = (cy_en_syspm_buck_voltage1_t)message->praData1;
                        if (structCpy.buckVoltage == CY_SYSPM_BUCK_OUT1_VOLTAGE_0_9V)
                        {
                            structCpy.ulpEnable = true;
                        }
                        else
                        {
                            structCpy.ulpEnable = false;
                        }
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.powerEnable = powerEnableTmp;
                            structCpy.ldoEnable = ldoEnableTmp;
                            structCpy.ulpEnable = ulpEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_PM_FUNC_SET_MIN_CURRENT:
                    {
                        bool powerEnableTmp, pwrCurrentModeMinTmp;
                        /* Backup old values */
                        powerEnableTmp = structCpy.powerEnable;
                        pwrCurrentModeMinTmp = structCpy.pwrCurrentModeMin;
                        structCpy.powerEnable = true;
                        structCpy.pwrCurrentModeMin = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.powerEnable = powerEnableTmp;
                            structCpy.pwrCurrentModeMin = pwrCurrentModeMinTmp;
                        }
                    }
                    break;

                    case CY_PRA_PM_FUNC_SET_NORMAL_CURRENT:
                    {
                        bool powerEnableTmp, pwrCurrentModeMinTmp;
                        /* Backup old values */
                        powerEnableTmp = structCpy.powerEnable;
                        pwrCurrentModeMinTmp = structCpy.pwrCurrentModeMin;
                        structCpy.powerEnable = true;
                        structCpy.pwrCurrentModeMin = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.powerEnable = powerEnableTmp;
                            structCpy.pwrCurrentModeMin = pwrCurrentModeMinTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_ECO_DISABLE:
                    {
                        bool ecoEnableTmp;
                        /* Backup old values */
                        ecoEnableTmp = structCpy.ecoEnable;
                        structCpy.ecoEnable = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.ecoEnable = ecoEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_FLL_DISABLE:
                    {
                        bool fllEnableTmp;
                        uint32_t fllOutFreqHzTmp;
                        /* Backup old values */
                        fllOutFreqHzTmp = structCpy.fllOutFreqHz;
                        fllEnableTmp = structCpy.fllEnable;
                        structCpy.fllEnable = false;
                        structCpy.fllOutFreqHz = CY_PRA_DEFAULT_ZERO;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.fllEnable = fllEnableTmp;
                            structCpy.fllOutFreqHz = fllOutFreqHzTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PLL_DISABLE:
                    {
                        bool pllEnable;
                        if (((message->praData1) > CY_PRA_CLKPATH_0) && ((message->praData1) <= CY_SRSS_NUM_PLL)) /* 0 is invalid pll number */
                        {
                            /* Backup old values */
                            ((message->praData1) == CY_PRA_CLKPLL_1) ? (pllEnable = structCpy.pll0Enable) : (pllEnable = structCpy.pll1Enable);

                            ((message->praData1) == CY_PRA_CLKPLL_1) ? (structCpy.pll0Enable = false) : (structCpy.pll1Enable = false);

                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);

                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                ((message->praData1) == CY_PRA_CLKPLL_1) ? (structCpy.pll0Enable = pllEnable) : (structCpy.pll1Enable = pllEnable);
                            }
                        }
                        else
                        {
                            message->praStatus = CY_PRA_STATUS_INVALID_PARAM_PLL_NUM;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_ILO_ENABLE:
                    {
                        bool iloEnableTmp;
                        /* Backup old values */
                        iloEnableTmp = structCpy.iloEnable;
                        structCpy.iloEnable = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.iloEnable = iloEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_ILO_DISABLE:
                    {
                        bool iloEnableTmp;
                        /* Backup old values */
                        iloEnableTmp = structCpy.iloEnable;
                        structCpy.iloEnable = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.iloEnable = iloEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_ILO_HIBERNATE_ON:
                    {
                        bool iloHibernateOnTmp;
                        /* Backup old values */
                        iloHibernateOnTmp = structCpy.iloHibernateON;
                        structCpy.iloHibernateON = (CY_PRA_DATA_DISABLE != message->praData1);
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.iloHibernateON = iloHibernateOnTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PILO_ENABLE:
                    {
                        bool piloEnableTmp;
                        /* Backup old values */
                        piloEnableTmp = structCpy.piloEnable;
                        structCpy.piloEnable = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.piloEnable = piloEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PILO_DISABLE:
                    {
                        bool piloEnableTmp;
                        /* Backup old values */
                        piloEnableTmp = structCpy.piloEnable;
                        structCpy.piloEnable = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.piloEnable = piloEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_WCO_ENABLE:
                    {
                        bool wcoEnableTmp;
                        /* Backup old values */
                        wcoEnableTmp = structCpy.wcoEnable;
                        structCpy.wcoEnable = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.wcoEnable = wcoEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_WCO_DISABLE:
                    {
                        bool wcoEnableTmp;
                        /* Backup old values */
                        wcoEnableTmp = structCpy.wcoEnable;
                        structCpy.wcoEnable = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.wcoEnable = wcoEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_WCO_BYPASS:
                    {
                        bool bypassEnableTmp;
                        /* Backup old values */
                        bypassEnableTmp = structCpy.bypassEnable;
                        structCpy.bypassEnable = ((cy_en_wco_bypass_modes_t) message->praData1 == CY_SYSCLK_WCO_BYPASSED) ? true : false;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Bypass value will be written to REGISTER only when WCO is enabled */
                        if (structCpy.wcoEnable)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                structCpy.bypassEnable = bypassEnableTmp;
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_HF_ENABLE:
                    {
                        bool clkHFEnable;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Backup old values */
                        switch (message->praData1)
                        {
                            case CY_PRA_CLKHF_0:
                            clkHFEnable = structCpy.clkHF0Enable;
                            structCpy.clkHF0Enable = true;
                            break;

                            case CY_PRA_CLKHF_1:
                            clkHFEnable = structCpy.clkHF1Enable;
                            structCpy.clkHF1Enable = true;
                            break;

                            case CY_PRA_CLKHF_2:
                            clkHFEnable = structCpy.clkHF2Enable;
                            structCpy.clkHF2Enable = true;
                            break;

                            case CY_PRA_CLKHF_3:
                            clkHFEnable = structCpy.clkHF3Enable;
                            structCpy.clkHF3Enable = true;
                            break;

                            case CY_PRA_CLKHF_4:
                            clkHFEnable = structCpy.clkHF4Enable;
                            structCpy.clkHF4Enable = true;
                            break;

                            case CY_PRA_CLKHF_5:
                            clkHFEnable = structCpy.clkHF5Enable;
                            structCpy.clkHF5Enable = true;
                            break;

                            default:
                            clkHFEnable = false;
                            message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                            break;

                        }
                        if (message->praStatus == CY_PRA_STATUS_SUCCESS)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                switch (message->praData1)
                                {
                                    case CY_PRA_CLKHF_0:
                                    structCpy.clkHF0Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_1:
                                    structCpy.clkHF1Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_2:
                                    structCpy.clkHF2Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_3:
                                    structCpy.clkHF3Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_4:
                                    structCpy.clkHF4Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_5:
                                    structCpy.clkHF5Enable = clkHFEnable;
                                    break;

                                    default:
                                    break;
                                }
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_HF_DISABLE:
                    {
                        bool clkHFEnable;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Backup old values */
                        switch (message->praData1)
                        {
                            case CY_PRA_CLKHF_0:
                            clkHFEnable = structCpy.clkHF0Enable;
                            structCpy.clkHF0Enable = false;
                            break;

                            case CY_PRA_CLKHF_1:
                            clkHFEnable = structCpy.clkHF1Enable;
                            structCpy.clkHF1Enable = false;
                            break;

                            case CY_PRA_CLKHF_2:
                            clkHFEnable = structCpy.clkHF2Enable;
                            structCpy.clkHF2Enable = false;
                            break;

                            case CY_PRA_CLKHF_3:
                            clkHFEnable = structCpy.clkHF3Enable;
                            structCpy.clkHF3Enable = false;
                            break;

                            case CY_PRA_CLKHF_4:
                            clkHFEnable = structCpy.clkHF4Enable;
                            structCpy.clkHF4Enable = false;
                            break;

                            case CY_PRA_CLKHF_5:
                            clkHFEnable = structCpy.clkHF5Enable;
                            structCpy.clkHF5Enable = false;
                            break;

                            default:
                            clkHFEnable = false;
                            message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                            break;

                        }
                        if (message->praStatus == CY_PRA_STATUS_SUCCESS)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                switch (message->praData1)
                                {
                                    case CY_PRA_CLKHF_0:
                                    structCpy.clkHF0Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_1:
                                    structCpy.clkHF1Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_2:
                                    structCpy.clkHF2Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_3:
                                    structCpy.clkHF3Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_4:
                                    structCpy.clkHF4Enable = clkHFEnable;
                                    break;

                                    case CY_PRA_CLKHF_5:
                                    structCpy.clkHF5Enable = clkHFEnable;
                                    break;

                                    default:
                                    break;
                                }
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_HF_SET_SOURCE:
                    {
                        cy_en_clkhf_in_sources_t hfSource;
                        uint32_t hfOutFreqMHz;
                        bool hfEnabled;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Backup old values */
                        switch (((cy_stc_pra_clkhfsetsource_t *) message->praData1)->clkHf)
                        {
                            case CY_PRA_CLKHF_0:
                            hfSource = structCpy.hf0Source;
                            structCpy.hf0Source = ((cy_stc_pra_clkhfsetsource_t *) message->praData1)->source;
                            hfOutFreqMHz = structCpy.hf0OutFreqMHz;
                            hfEnabled = structCpy.clkHF0Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf0OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf0Source)/(1UL << structCpy.hf0Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_1:
                            hfSource = structCpy.hf1Source;
                            structCpy.hf1Source = ((cy_stc_pra_clkhfsetsource_t *) message->praData1)->source;
                            hfOutFreqMHz = structCpy.hf1OutFreqMHz;
                            hfEnabled = structCpy.clkHF1Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf1OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf1Source)/(1UL << structCpy.hf1Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_2:
                            hfSource = structCpy.hf2Source;
                            structCpy.hf2Source = ((cy_stc_pra_clkhfsetsource_t *) message->praData1)->source;
                            hfOutFreqMHz = structCpy.hf2OutFreqMHz;
                            hfEnabled = structCpy.clkHF2Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf2OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf2Source)/(1UL << structCpy.hf2Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_3:
                            hfSource = structCpy.hf3Source;
                            structCpy.hf3Source = ((cy_stc_pra_clkhfsetsource_t *) message->praData1)->source;
                            hfOutFreqMHz = structCpy.hf3OutFreqMHz;
                            hfEnabled = structCpy.clkHF3Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf3OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf3Source)/(1UL << structCpy.hf3Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_4:
                            hfSource = structCpy.hf4Source;
                            structCpy.hf4Source = ((cy_stc_pra_clkhfsetsource_t *) message->praData1)->source;
                            hfOutFreqMHz = structCpy.hf4OutFreqMHz;
                            hfEnabled = structCpy.clkHF4Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf4OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf4Source)/(1UL << structCpy.hf4Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_5:
                            hfSource = structCpy.hf5Source;
                            structCpy.hf5Source = ((cy_stc_pra_clkhfsetsource_t *) message->praData1)->source;
                            hfOutFreqMHz = structCpy.hf5OutFreqMHz;
                            hfEnabled = structCpy.clkHF5Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf5OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf5Source)/(1UL << structCpy.hf5Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            default:
                            hfEnabled = false;
                            message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                            break;
                        }
                        /* Source value of HF will be updated in REGISTER only when
                         * that particular HF is enabled otherwise it will be stored in
                         * the system config structure
                         * */
                        if ((message->praStatus == CY_PRA_STATUS_SUCCESS) && (hfEnabled))
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                switch (((cy_stc_pra_clkhfsetsource_t *) message->praData1)->clkHf)
                                {
                                    case CY_PRA_CLKHF_0:
                                    structCpy.hf0Source = hfSource;
                                    structCpy.hf0OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_1:
                                    structCpy.hf1Source = hfSource;
                                    structCpy.hf1OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_2:
                                    structCpy.hf2Source = hfSource;
                                    structCpy.hf2OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_3:
                                    structCpy.hf3Source = hfSource;
                                    structCpy.hf3OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_4:
                                    structCpy.hf4Source = hfSource;
                                    structCpy.hf4OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_5:
                                    structCpy.hf5Source = hfSource;
                                    structCpy.hf5OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    default:
                                    break;
                                }
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_HF_SET_DIVIDER:
                    {
                        cy_en_clkhf_dividers_t hfDivider;
                        uint32_t hfOutFreqMHz;
                        bool hfEnabled;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Backup old values */
                        switch (((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->clkHf)
                        {
                            case CY_PRA_CLKHF_0:
                            hfDivider = structCpy.hf0Divider;
                            structCpy.hf0Divider = ((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->divider;
                            hfOutFreqMHz = structCpy.hf0OutFreqMHz;
                            hfEnabled = structCpy.clkHF0Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf0OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf0Source)/(1UL << structCpy.hf0Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_1:
                            hfDivider = structCpy.hf1Divider;
                            structCpy.hf1Divider = ((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->divider;
                            hfOutFreqMHz = structCpy.hf1OutFreqMHz;
                            hfEnabled = structCpy.clkHF1Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf1OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf1Source)/(1UL << structCpy.hf1Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_2:
                            hfDivider = structCpy.hf2Divider;
                            structCpy.hf2Divider = ((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->divider;
                            hfOutFreqMHz = structCpy.hf2OutFreqMHz;
                            hfEnabled = structCpy.clkHF2Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf2OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf2Source)/(1UL << structCpy.hf2Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_3:
                            hfDivider = structCpy.hf3Divider;
                            structCpy.hf3Divider = ((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->divider;
                            hfOutFreqMHz = structCpy.hf3OutFreqMHz;
                            hfEnabled = structCpy.clkHF3Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf3OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf3Source)/(1UL << structCpy.hf3Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_4:
                            hfDivider = structCpy.hf4Divider;
                            structCpy.hf4Divider = ((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->divider;
                            hfOutFreqMHz = structCpy.hf4OutFreqMHz;
                            hfEnabled = structCpy.clkHF4Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf4OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf4Source)/(1UL << structCpy.hf4Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            case CY_PRA_CLKHF_5:
                            hfDivider = structCpy.hf5Divider;
                            structCpy.hf5Divider = ((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->divider;
                            hfOutFreqMHz = structCpy.hf5OutFreqMHz;
                            hfEnabled = structCpy.clkHF5Enable;
                            /* HF output frequency is not present in PDL API argument. So updated the system config structure with current HF output frequency value */
                            structCpy.hf5OutFreqMHz = (Cy_SysClk_ClkPathGetFrequency((uint32_t) structCpy.hf5Source)/(1UL << structCpy.hf5Divider))/CY_PRA_FREQUENCY_HZ_CONVERSION;
                            break;

                            default:
                            hfEnabled = false;
                            message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                            break;
                        }
                        /* Divider value of HF will be updated in REGISTER only when
                         * that particular HF is enabled otherwise it will be stored in
                         * the system config structure
                         * */
                        if ((message->praStatus == CY_PRA_STATUS_SUCCESS) && (hfEnabled))
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                switch (((cy_stc_pra_clkhfsetdivider_t *) message->praData1)->clkHf)
                                {
                                    case CY_PRA_CLKHF_0:
                                    structCpy.hf0Divider = hfDivider;
                                    structCpy.hf0OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_1:
                                    structCpy.hf1Divider = hfDivider;
                                    structCpy.hf1OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_2:
                                    structCpy.hf2Divider = hfDivider;
                                    structCpy.hf2OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_3:
                                    structCpy.hf3Divider = hfDivider;
                                    structCpy.hf3OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_4:
                                    structCpy.hf4Divider = hfDivider;
                                    structCpy.hf4OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    case CY_PRA_CLKHF_5:
                                    structCpy.hf5Divider = hfDivider;
                                    structCpy.hf5OutFreqMHz = hfOutFreqMHz;
                                    break;

                                    default:
                                    break;
                                }
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_FAST_SET_DIVIDER:
                    {
                        uint8_t clkFastDivTmp;
                        bool clkFastEnableTmp;
                        /* Backup old values */
                        clkFastEnableTmp = structCpy.clkFastEnable;
                        clkFastDivTmp = structCpy.clkFastDiv;
                        structCpy.clkFastEnable = true;
                        structCpy.clkFastDiv =  (uint8_t)(message->praData1);
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkFastDiv = clkFastDivTmp;
                            structCpy.clkFastEnable = clkFastEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PERI_SET_DIVIDER:
                    {
                        uint8_t clkPeriDivTmp;
                        bool clkPeriEnableTmp;
                        /* Backup old values */
                        clkPeriDivTmp = structCpy.clkPeriDiv;
                        clkPeriEnableTmp = structCpy.clkPeriEnable;
                        structCpy.clkPeriEnable = true;
                        structCpy.clkPeriDiv =  (uint8_t)(message->praData1);
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkPeriDiv = clkPeriDivTmp;
                            structCpy.clkPeriEnable = clkPeriEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_LF_SET_SOURCE:
                    {
                        cy_en_clklf_in_sources_t clkLfSourceTmp;
                        bool clkLFEnableTmp;
                        /* Backup old values */
                        clkLFEnableTmp = structCpy.clkLFEnable;
                        clkLfSourceTmp = structCpy.clkLfSource;
                        structCpy.clkLFEnable = true;
                        structCpy.clkLfSource = (cy_en_clklf_in_sources_t)message->praData1;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkLfSource = clkLfSourceTmp;
                            structCpy.clkLFEnable = clkLFEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_TIMER_SET_SOURCE:
                    {
                        cy_en_clktimer_in_sources_t clkTimerSourceTmp;
                        /* Backup old values */
                        clkTimerSourceTmp = structCpy.clkTimerSource;
                        structCpy.clkTimerSource = (cy_en_clktimer_in_sources_t)message->praData1;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        if (structCpy.clkTimerEnable)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                structCpy.clkTimerSource = clkTimerSourceTmp;
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_TIMER_SET_DIVIDER:
                    {
                        uint8_t clkTimerDividerTmp;
                        /* Backup old values */
                        clkTimerDividerTmp = structCpy.clkTimerDivider;
                        structCpy.clkTimerDivider = (uint8_t)(message->praData1);
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Timer Divider value will be updated in REGISTER only-
                         * when CLK_TIMER is enabled. Otherwise it will be only
                         * stored in the system config structure
                         * */
                        if (structCpy.clkTimerEnable)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                structCpy.clkTimerDivider = clkTimerDividerTmp;
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_TIMER_ENABLE:
                    {
                        bool clkTimerEnableTmp;
                        /* Backup old values */
                        clkTimerEnableTmp = structCpy.clkTimerEnable;
                        structCpy.clkTimerEnable = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkTimerEnable = clkTimerEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_TIMER_DISABLE:
                    {
                        bool clkTimerEnableTmp;
                        /* Backup old values */
                        clkTimerEnableTmp = structCpy.clkTimerEnable;
                        structCpy.clkTimerEnable = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkTimerEnable = clkTimerEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PUMP_SET_SOURCE:
                    {
                        cy_en_clkpump_in_sources_t pumpSourceTmp;
                        /* Backup old values */
                        pumpSourceTmp = structCpy.pumpSource;
                        structCpy.pumpSource = (cy_en_clkpump_in_sources_t)message->praData1;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* PUMP source value will be updated in REGISTER only-
                         * when PUMP is enabled. Otherwise it will be only
                         * stored in the system config structure
                         * */
                        if (structCpy.clkPumpEnable)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                structCpy.pumpSource = pumpSourceTmp;
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PUMP_SET_DIVIDER:
                    {
                        cy_en_clkpump_divide_t pumpDividerTmp;
                        /* Backup old values */
                        pumpDividerTmp = structCpy.pumpDivider;
                        structCpy.pumpDivider = (cy_en_clkpump_divide_t)message->praData1;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* PUMP Divider value will be updated in REGISTER only-
                         * when PUMP is enabled. Otherwise it will be only
                         * stored in the system config structure
                         * */
                        if (structCpy.clkPumpEnable)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous values */
                                structCpy.pumpDivider = pumpDividerTmp;
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PUMP_ENABLE:
                    {
                        bool clkPumpEnableTmp;
                        /* Backup old values */
                        clkPumpEnableTmp = structCpy.clkPumpEnable;
                        structCpy.clkPumpEnable = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkPumpEnable = clkPumpEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PUMP_DISABLE:
                    {
                        bool clkPumpEnableTmp;
                        /* Backup old values */
                        clkPumpEnableTmp = structCpy.clkPumpEnable;
                        structCpy.clkPumpEnable = false;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkPumpEnable = clkPumpEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_BAK_SET_SOURCE:
                    {
                        cy_en_clkbak_in_sources_t clkBakSourceTmp;
                        bool clkBakEnableTmp;
                        /* Backup old values */
                        clkBakEnableTmp = structCpy.clkBakEnable;
                        clkBakSourceTmp = structCpy.clkBakSource;
                        structCpy.clkBakEnable = true;
                        structCpy.clkBakSource = (cy_en_clkbak_in_sources_t)message->praData1;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.clkBakSource = clkBakSourceTmp;
                            structCpy.clkBakEnable = clkBakEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_ECO_CONFIGURE:
                    {
                        /* ECO configuration is not allowed if ECO is already enaled.
                         * Right sequence is ECO_DISABLE -> ECO_CONFIGURE -> ECO_ENABLE
                         * */
                        if (structCpy.ecoEnable)
                        {
                            message->praStatus = CY_PRA_STATUS_ERROR_PROCESSING_ECO_ENABLED;
                        }
                        else
                        {
                            /* Stored the ECO config values into system config structure.
                             * These values will be applied to REGISTER when ECO_ENABLE call comes.
                             * */
                            structCpy.ecoFreqHz = ((cy_stc_pra_clk_eco_configure_t *) message->praData1)->praClkEcofreq;
                            structCpy.ecoLoad = ((cy_stc_pra_clk_eco_configure_t *) message->praData1)->praCsum;
                            structCpy.ecoEsr = ((cy_stc_pra_clk_eco_configure_t *) message->praData1)->praEsr;
                            structCpy.ecoDriveLevel = ((cy_stc_pra_clk_eco_configure_t *) message->praData1)->praDriveLevel;
                            message->praStatus = CY_PRA_STATUS_SUCCESS;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_ECO_ENABLE:
                    {
                        bool ecoEnableTmp;
                        /* Backup old values */
                        ecoEnableTmp = structCpy.ecoEnable;
                        structCpy.ecoEnable = true;
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous values */
                            structCpy.ecoEnable = ecoEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PATH_SET_SOURCE:
                    {
                        bool pathEnable = false;
                        cy_en_clkpath_in_sources_t pathSrc = CY_SYSCLK_CLKPATH_IN_IMO;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                        /* Backup old values */
                        switch (((cy_stc_pra_clkpathsetsource_t *) message->praData1)->clk_path)
                        {
                            case CY_PRA_CLKPATH_0:
                            pathEnable = structCpy.path0Enable;
                            pathSrc = structCpy.path0Src;
                            structCpy.path0Enable = true;
                            structCpy.path0Src = ((cy_stc_pra_clkpathsetsource_t *) message->praData1)->source;
                            break;

                            case CY_PRA_CLKPATH_1:
                            pathEnable = structCpy.path1Enable;
                            pathSrc = structCpy.path1Src;
                            structCpy.path1Enable = true;
                            structCpy.path1Src = ((cy_stc_pra_clkpathsetsource_t *) message->praData1)->source;
                            break;

                            case CY_PRA_CLKPATH_2:
                            pathEnable = structCpy.path2Enable;
                            pathSrc = structCpy.path2Src;
                            structCpy.path2Enable = true;
                            structCpy.path2Src = ((cy_stc_pra_clkpathsetsource_t *) message->praData1)->source;
                            break;

                            case CY_PRA_CLKPATH_3:
                            pathEnable = structCpy.path3Enable;
                            pathSrc = structCpy.path3Src;
                            structCpy.path3Enable = true;
                            structCpy.path3Src = ((cy_stc_pra_clkpathsetsource_t *) message->praData1)->source;
                            break;

                            case CY_PRA_CLKPATH_4:
                            pathEnable = structCpy.path4Enable;
                            pathSrc = structCpy.path4Src;
                            structCpy.path4Enable = true;
                            structCpy.path4Src = ((cy_stc_pra_clkpathsetsource_t *) message->praData1)->source;
                            break;

                            case CY_PRA_CLKPATH_5:
                            pathEnable = structCpy.path5Enable;
                            pathSrc = structCpy.path5Src;
                            structCpy.path5Enable = true;
                            structCpy.path5Src = ((cy_stc_pra_clkpathsetsource_t *) message->praData1)->source;
                            break;

                            default:
                            message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                            break;
                        }
                        if (message->praStatus == CY_PRA_STATUS_SUCCESS)
                        {
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                /* On failure restored previous path values */
                                switch (((cy_stc_pra_clkpathsetsource_t *) message->praData1)->clk_path)
                                {
                                    case CY_PRA_CLKPATH_0:
                                    structCpy.path0Enable = pathEnable;
                                    structCpy.path0Src = pathSrc;
                                    break;

                                    case CY_PRA_CLKPATH_1:
                                    structCpy.path1Enable = pathEnable;
                                    structCpy.path1Src = pathSrc;
                                    break;

                                    case CY_PRA_CLKPATH_2:
                                    structCpy.path2Enable = pathEnable;
                                    structCpy.path2Src = pathSrc;
                                    break;

                                    case CY_PRA_CLKPATH_3:
                                    structCpy.path3Enable = pathEnable;
                                    structCpy.path3Src = pathSrc;
                                    break;

                                    case CY_PRA_CLKPATH_4:
                                    structCpy.path4Enable = pathEnable;
                                    structCpy.path4Src = pathSrc;
                                    break;

                                    case CY_PRA_CLKPATH_5:
                                    structCpy.path5Enable = pathEnable;
                                    structCpy.path5Src = pathSrc;
                                    break;

                                    default:
                                    message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                                    break;
                                }
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_FLL_MANCONFIG:
                    {
                        /* FLL manual configuration values are not wrriten to
                         * REGISTER but stored in the system config structure.
                         * When CY_PRA_CLK_FUNC_FLL_ENABLE call comes then these
                         * values will be applied to REGISTER. */
                        structCpy.fllMult = ((cy_stc_fll_manual_config_t *) message->praData1)->fllMult;
                        structCpy.fllRefDiv = ((cy_stc_fll_manual_config_t *) message->praData1)->refDiv;
                        structCpy.fllCcoRange = ((cy_stc_fll_manual_config_t *) message->praData1)->ccoRange;
                        structCpy.enableOutputDiv = ((cy_stc_fll_manual_config_t *) message->praData1)->enableOutputDiv;
                        structCpy.lockTolerance = ((cy_stc_fll_manual_config_t *) message->praData1)->lockTolerance;
                        structCpy.igain = ((cy_stc_fll_manual_config_t *) message->praData1)->igain;
                        structCpy.pgain = ((cy_stc_fll_manual_config_t *) message->praData1)->pgain;
                        structCpy.settlingCount = ((cy_stc_fll_manual_config_t *) message->praData1)->settlingCount;
                        structCpy.outputMode = ((cy_stc_fll_manual_config_t *) message->praData1)->outputMode;
                        structCpy.ccoFreq = ((cy_stc_fll_manual_config_t *) message->praData1)->cco_Freq;
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                    }
                    break;

                    case CY_PRA_CLK_FUNC_FLL_ENABLE:
                    {
                        /* FLL Enable is not allowed if it is already enabled */
                        if (structCpy.fllEnable == true) /* FLL is already enabled */
                        {
                            message->praStatus = CY_PRA_STATUS_ERROR_PROCESSING_FLL0_ENABLED;
                        }
                        else
                        {
                            structCpy.fllEnable = true;
                            /* FLL Enable API does not contains FLL output value
                             * as argument. So calculate and update FLL output value */
                            structCpy.fllOutFreqHz = Cy_PRA_CalculateFLLOutFreq(&structCpy);
                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                structCpy.fllOutFreqHz = CY_PRA_DEFAULT_ZERO;
                                structCpy.fllEnable = false;
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PLL_MANCONFIG:
                    {
                        /* Checking for valid PLL number */
                        if((((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->clkPath == CY_PRA_CLKPATH_0) ||
                           (((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->clkPath > CY_SRSS_NUM_PLL))
                        {
                            message->praStatus = CY_PRA_STATUS_INVALID_PARAM_PLL_NUM;
                        }
                        else
                        {
                            /* PLL manual configuration values are not wrriten to
                             * REGISTER but stored in the system config structure.
                             * When CY_PRA_CLK_FUNC_PLL_ENABLE call comes then these
                             * values will be applied to REGISTER. */
                            if(((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->clkPath == CY_PRA_CLKPATH_1)
                            {
                                structCpy.pll0FeedbackDiv = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->feedbackDiv;
                                structCpy.pll0ReferenceDiv = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->referenceDiv;
                                structCpy.pll0OutputDiv = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->outputDiv;
                                structCpy.pll0LfMode = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->lfMode;
                                structCpy.pll0OutputMode = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->outputMode;
                            }
                            else
                            {
                                structCpy.pll1FeedbackDiv = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->feedbackDiv;
                                structCpy.pll1ReferenceDiv = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->referenceDiv;
                                structCpy.pll1OutputDiv = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->outputDiv;
                                structCpy.pll1LfMode = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->lfMode;
                                structCpy.pll1OutputMode = ((cy_stc_pra_clk_pll_manconfigure_t *) message->praData1)->praConfig->outputMode;
                            }
                            message->praStatus = CY_PRA_STATUS_SUCCESS;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_PLL_ENABLE:
                    {
                        if ((((message->praData1) == CY_PRA_CLKPLL_1) && (structCpy.pll0Enable == true))
                            || (((message->praData1) == CY_PRA_CLKPLL_2) && (structCpy.pll1Enable == true))
                            || (message->praData1 > CY_SRSS_NUM_PLL) || (message->praData1 == 0UL))
                        {
                            message->praStatus = CY_PRA_STATUS_ERROR_PROCESSING_PLL_ENABLED;
                        }
                        else
                        {
                            /* PLL Enable API does not contains PLL output value
                             * as argument. So calculate and update PLL output value */
                            if ((message->praData1) == CY_PRA_CLKPLL_1)
                            {
                                structCpy.pll0OutFreqHz = Cy_PRA_CalculatePLLOutFreq(CY_PRA_CLKPLL_1, &structCpy);
                                structCpy.pll0Enable = true;
                            }
                            else
                            {
                                structCpy.pll1OutFreqHz = Cy_PRA_CalculatePLLOutFreq(CY_PRA_CLKPLL_2, &structCpy);
                                structCpy.pll1Enable = true;
                            }

                            message->praStatus = Cy_PRA_SystemConfig(&structCpy);

                            if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                            {
                                if ((message->praData1) == CY_PRA_CLKPLL_1)
                                {
                                    structCpy.pll0OutFreqHz = CY_PRA_DEFAULT_ZERO;
                                    structCpy.pll0Enable = false;
                                }
                                else
                                {
                                    structCpy.pll1OutFreqHz = CY_PRA_DEFAULT_ZERO;
                                    structCpy.pll1Enable = false;
                                }
                            }
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_SLOW_SET_DIVIDER:
                    {
                        uint8_t clkSlowDivTmp;
                        bool clkSlowEnableTmp;
                        /* Backup old values */
                        clkSlowEnableTmp = structCpy.clkSlowEnable;
                        clkSlowDivTmp = structCpy.clkSlowDiv;
                        structCpy.clkSlowEnable = true;
                        structCpy.clkSlowDiv = (uint8_t)(message->praData1);
                        message->praStatus = Cy_PRA_SystemConfig(&structCpy);
                        if (message->praStatus != CY_PRA_STATUS_SUCCESS)
                        {
                            /* On failure restored previous path values */
                            structCpy.clkSlowDiv = clkSlowDivTmp;
                            structCpy.clkSlowEnable = clkSlowEnableTmp;
                        }
                    }
                    break;

                    case CY_PRA_CLK_FUNC_EXT_CLK_SET_FREQUENCY:
                    {
                        structCpy.extClkFreqHz = (uint32_t)(message->praData1);
                        message->praStatus = CY_PRA_STATUS_SUCCESS;
                    }
                    break;

                    default:
                        message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
                        break;
                }
            }
            else
            {
                message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
            }
            break;

        default:
            message->praStatus = CY_PRA_STATUS_ACCESS_DENIED;
           break;
    }

}
#endif /* (CY_CPU_CORTEX_M0P) */


#if (CY_CPU_CORTEX_M4) || defined (CY_DOXYGEN)

/*******************************************************************************
* Function Name: Cy_PRA_SendCmd
****************************************************************************//**
*
* Process and executes the command received from non-secure core.
*
* \param cmd Command to be executed on secure side.
* \param regIndex Index of the function or register depending on command parameter.
* \param clearMask Data send to secure core.
* \param setMask Additional data send to secure core.
*
* \return Status of the command execution. For register read command the read
* value is returned.
*
*******************************************************************************/
#if defined(CY_DEVICE_PSOC6ABLE2)

    CY_RAMFUNC_BEGIN
    #if !defined (__ICCARM__)
        CY_NOINLINE
    #endif
#endif /* defined(CY_DEVICE_PSOC6ABLE2) */
    cy_en_pra_status_t Cy_PRA_SendCmd(uint16_t cmd, uint16_t regIndex, uint32_t clearMask, uint32_t setMask)
    {
        CY_ASSERT_L1(NULL != ipcPraBase);

        cy_en_pra_status_t status;
        CY_ALIGN(4UL) cy_stc_pra_msg_t ipcMsg;
        uint32_t interruptState;

        ipcMsg.praCommand = cmd;
        ipcMsg.praStatus  = CY_PRA_STATUS_REQUEST_SENT;
        ipcMsg.praIndex   = regIndex;
        ipcMsg.praData1   = clearMask;
        ipcMsg.praData2   = setMask;

        interruptState = Cy_SysLib_EnterCriticalSection();

        while (0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, REG_IPC_STRUCT_ACQUIRE(ipcPraBase)))
        {
            /* Wait until the PRA IPC structure is acquired */
        }

        /* Send the message */
        REG_IPC_STRUCT_DATA(ipcPraBase) = (uint32_t) &ipcMsg;

        /* Generate an acquire notification event by PRA IPC interrupt structure */
        REG_IPC_STRUCT_NOTIFY(ipcPraBase) = _VAL2FLD(IPC_STRUCT_NOTIFY_INTR_NOTIFY, CY_PRA_IPC_NOTIFY_INTR);

        while (0U != _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, REG_IPC_STRUCT_LOCK_STATUS(ipcPraBase)))
        {
            /* Wait until the PRA IPC structure is released */
        }

        Cy_SysLib_ExitCriticalSection(interruptState);

        /* Cortex-M0+ has updated ipcMsg variable */

        status = (cy_en_pra_status_t) ipcMsg.praStatus;

        if (CY_PRA_STATUS_ACCESS_DENIED == status)
        {
            CY_HALT();
        }

        if (CY_PRA_MSG_TYPE_SYS_CFG_FUNC == ipcMsg.praCommand)
        {
            SystemCoreClockUpdate();
        }

        if (CY_PRA_MSG_TYPE_REG32_GET == ipcMsg.praCommand)
        {
            status = (cy_en_pra_status_t)ipcMsg.praData1;
        }

        return status;
    }
#if defined(CY_DEVICE_PSOC6ABLE2)
    CY_RAMFUNC_END
#endif /* defined(CY_DEVICE_PSOC6ABLE2) */

#endif /* (CY_CPU_CORTEX_M4) */


#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)

/* The mask to unlock the Hibernate power mode */
#define HIBERNATE_UNLOCK_VAL                 ((uint32_t) 0x3Au << SRSS_PWR_HIBERNATE_UNLOCK_Pos)

/* The mask to set the Hibernate power mode */
#define SET_HIBERNATE_MODE                   ((HIBERNATE_UNLOCK_VAL |\
                                               SRSS_PWR_HIBERNATE_FREEZE_Msk |\
                                               SRSS_PWR_HIBERNATE_HIBERNATE_Msk))

/* The mask to retain the Hibernate power mode status */
#define HIBERNATE_RETAIN_STATUS_MASK         ((SRSS_PWR_HIBERNATE_TOKEN_Msk |\
                                               SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                               SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                               SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                               SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk))

/** The mask for the Hibernate wakeup sources */
#define HIBERNATE_WAKEUP_MASK               ((SRSS_PWR_HIBERNATE_MASK_HIBALARM_Msk |\
                                              SRSS_PWR_HIBERNATE_MASK_HIBWDT_Msk |\
                                              SRSS_PWR_HIBERNATE_POLARITY_HIBPIN_Msk |\
                                              SRSS_PWR_HIBERNATE_MASK_HIBPIN_Msk))

/** The define to update the token to indicate the transition into Hibernate */
#define HIBERNATE_TOKEN                    ((uint32_t) 0x1BU << SRSS_PWR_HIBERNATE_TOKEN_Pos)


/*******************************************************************************
* Function Name: Cy_PRA_PmHibernate
****************************************************************************//**
*
* Update SRSS_PWR_HIBERNATE register for Cy_SysPm_SystemEnterHibernate and
* Cy_SysPm_IoUnfreeze functions.
*
*******************************************************************************/
static void Cy_PRA_PmHibernate(uint32_t funcProc)
{

    if(0UL == funcProc)
    {
        /* Preserve the token that will be retained through a wakeup sequence.
         * This could be used by Cy_SysLib_GetResetReason() to differentiate
         * Wakeup from a general reset event.
         * Preserve the wakeup source(s) configuration.
         */
        SRSS_PWR_HIBERNATE = (SRSS_PWR_HIBERNATE & HIBERNATE_WAKEUP_MASK) | HIBERNATE_TOKEN;

        /* Disable overriding by the peripherals the next pin-freeze command */
        SRSS_PWR_HIBERNATE |= SET_HIBERNATE_MODE;

        /* The second write causes freezing of I/O cells to save the I/O-cell state */
        SRSS_PWR_HIBERNATE |= SET_HIBERNATE_MODE;

        /* Third write cause system to enter Hibernate */
        SRSS_PWR_HIBERNATE |= SET_HIBERNATE_MODE;
    }
    else
    {
        /* Preserve the last reset reason and wakeup polarity. Then, unfreeze I/O:
         * write PWR_HIBERNATE.FREEZE=0, .UNLOCK=0x3A, .HIBERANTE=0
         */
        SRSS_PWR_HIBERNATE = (SRSS_PWR_HIBERNATE & HIBERNATE_RETAIN_STATUS_MASK) | HIBERNATE_UNLOCK_VAL;

        /* Lock the Hibernate mode:
        * write PWR_HIBERNATE.HIBERNATE=0, UNLOCK=0x00, HIBERANTE=0
        */
        SRSS_PWR_HIBERNATE &= HIBERNATE_RETAIN_STATUS_MASK;
    }
}


/*******************************************************************************
* Function Name: Cy_PRA_PmCm4DpFlagSet
****************************************************************************//**
*
* Set Deep Sleep Flag for the CM4 core
*
*******************************************************************************/
static void Cy_PRA_PmCm4DpFlagSet(void)
{
    uint32_t ddftStructData;

    /* Acquire the IPC to prevent changing of the shared resources at the same time */
    while (0U == _FLD2VAL(IPC_STRUCT_ACQUIRE_SUCCESS, REG_IPC_STRUCT_ACQUIRE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT))))
    {
        /* Wait until the IPC structure is released by another CPU */
    }

    ddftStructData = REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT));

    /* Update CM4 core deep sleep mask */
    ddftStructData |= (0x01UL << 28u);

    /* Update pointer to the latest saved structure */
    REG_IPC_STRUCT_DATA(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = ddftStructData;

    /* Release the IPC */
    REG_IPC_STRUCT_RELEASE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT)) = 0U;

    /* Read the release value to make sure it is set */
    (void) REG_IPC_STRUCT_RELEASE(CY_IPC_STRUCT_PTR(CY_IPC_CHAN_DDFT));
}

/* Timeout count for use in function Cy_PRA_ClkDeepSleepCallback() is sufficiently large for ~1 second */
#define CY_PRA_TIMEOUT (1000000UL)
/* These variables act as locks to prevent collisions between clock measurement and entry into
   Deep Sleep mode. See Cy_SysClk_DeepSleep(). */
static uint16_t changedSourcePaths = CY_PRA_DEFAULT_ZERO;
static uint16_t pllAutoModes = CY_PRA_DEFAULT_ZERO;


/*******************************************************************************
* Function Name: Cy_PRA_ClkDSBeforeTransition
****************************************************************************//**
*
* SysClock before deep sleep transition.
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ClkDSBeforeTransition(void)
{
    uint32_t fllPll; /* 0 = FLL, all other values = a PLL */

    /* Initialize the storage of changed paths */
    changedSourcePaths = CY_PRA_DEFAULT_ZERO;
    pllAutoModes = CY_PRA_DEFAULT_ZERO;

    /* For FLL and each PLL */
    for (fllPll = 0UL; fllPll <= CY_SRSS_NUM_PLL; fllPll++)
    {
        /* If FLL or PLL is enabled */
        if ((0UL == fllPll) ? Cy_SysClk_FllIsEnabled() : Cy_SysClk_PllIsEnabled(fllPll))
        {
            /* And the FLL/PLL has ECO as a source */
            if (Cy_SysClk_ClkPathGetSource(fllPll) == CY_SYSCLK_CLKPATH_IN_ECO)
            {
                /* Bypass the FLL/PLL */
                if (0UL == fllPll)
                {
                    CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
                }
                else
                {
                    if (((uint32_t)CY_SYSCLK_FLLPLL_OUTPUT_AUTO  == _FLD2VAL(SRSS_CLK_PLL_CONFIG_BYPASS_SEL, SRSS_CLK_PLL_CONFIG[fllPll - 1UL])) ||
                        ((uint32_t)CY_SYSCLK_FLLPLL_OUTPUT_AUTO1 == _FLD2VAL(SRSS_CLK_PLL_CONFIG_BYPASS_SEL, SRSS_CLK_PLL_CONFIG[fllPll - 1UL])))
                    {
                        pllAutoModes |= (uint16_t)(1UL << fllPll);
                    }

                    CY_REG32_CLR_SET(SRSS_CLK_PLL_CONFIG[fllPll - 1UL], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
                }

                /* Change this path source to IMO */
                (void)Cy_SysClk_ClkPathSetSource(fllPll, CY_SYSCLK_CLKPATH_IN_IMO);

                /* Store a record that this path source was changed from ECO */
                changedSourcePaths |= (uint16_t)(1UL << fllPll);
            }
            else if (0UL == fllPll)
            {
                CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
            }
            else
            {
                /* Do nothing */
            }
        }
    }

    return CY_PRA_STATUS_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_PRA_ClkDSAfterTransition
****************************************************************************//**
*
* SysClock after deep sleep transition.
*
*******************************************************************************/
static cy_en_pra_status_t Cy_PRA_ClkDSAfterTransition(void)
{
    /* Bitmapped paths with enabled FLL/PLL sourced by ECO */
    uint32_t timeout = CY_PRA_TIMEOUT;
    cy_en_pra_status_t retVal = CY_PRA_STATUS_ERROR_SYSPM_TIMEOUT;

    /* After return from Deep Sleep, for each FLL/PLL, if needed, restore the source to ECO.
     * And block until the FLL/PLL has regained its frequency lock.
     */
    if (0U != changedSourcePaths)
    {
        /* If any FLL/PLL was sourced by the ECO, timeout wait for the ECO to become fully stabilized again */
        while ((CY_SYSCLK_ECOSTAT_STABLE != Cy_SysClk_EcoGetStatus()) && (0UL != timeout))
        {
            timeout--;
        }

        if (0UL != timeout)
        {
            uint32_t fllPll; /* 0 = FLL, all other values = PLL */

            for (fllPll = 0UL; fllPll <= CY_SRSS_NUM_PLL; fllPll++)
            {
                /* If there is a correspondent record about a changed clock source */
                if (0U != (changedSourcePaths & (uint16_t)(1UL << fllPll)))
                {
                    /* Change this path source back to ECO */
                    (void)Cy_SysClk_ClkPathSetSource(fllPll, CY_SYSCLK_CLKPATH_IN_ECO);

                    /* Timeout wait for FLL/PLL to regain lock.
                     * Split FLL and PLL lock polling loops into two separate threads to minimize one polling loop duration.
                     */
                    if (0UL == fllPll)
                    {
                        while ((!Cy_SysClk_FllLocked()) && (0UL != timeout))
                        {
                            timeout--;
                        }
                    }
                    else
                    {
                        while ((!Cy_SysClk_PllLocked(fllPll)) && (0UL != timeout))
                        {
                            timeout--;
                        }
                    }

                    if (0UL != timeout)
                    {
                        /* Undo bypass the FLL/PLL */
                        if (0UL == fllPll)
                        {
                            CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
                        }
                        else
                        {
                            if (0U != (pllAutoModes & (uint16_t)(1UL << fllPll)))
                            {
                                CY_REG32_CLR_SET(SRSS_CLK_PLL_CONFIG[fllPll - 1UL], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_AUTO);
                            }
                            else
                            {
                                CY_REG32_CLR_SET(SRSS_CLK_PLL_CONFIG[fllPll - 1UL], SRSS_CLK_PLL_CONFIG_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
                            }
                        }

                        retVal = CY_PRA_STATUS_SUCCESS;
                    }
                }
            }
        }
    }
    else if (Cy_SysClk_FllIsEnabled())
    {
        /* Timeout wait for FLL to regain lock */
        while ((!Cy_SysClk_FllLocked()) && (0UL != timeout))
        {
            timeout--;
        }

        if (0UL != timeout)
        {
            /* Undo bypass the FLL */
            CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
            retVal = CY_PRA_STATUS_SUCCESS;
        }
    }
    else
    {
        retVal = CY_PRA_STATUS_SUCCESS;
    }

    return (retVal);
}


/*******************************************************************************
* Function Name: Cy_PRA_RegAccessRangeValid
****************************************************************************//**
*
* Check if access is within valid range and access address is non-zero.
*
* \param index Index of the accessed register.
*
* \return Return true for valid access.
*
*******************************************************************************/
static bool Cy_PRA_RegAccessRangeValid(uint16_t index)
{
    bool accessValid = true;

    /* Check if access is within array range */
    if (index >= CY_PRA_REG_INDEX_COUNT)
    {
        accessValid = false;
    }

    /* Some registers do not exist for some families */
    if (regIndexToAddr[index].addr == (const volatile uint32_t *) 0U)
    {
        accessValid = false;
    }

    return accessValid;
}


#endif /* (CY_CPU_CORTEX_M0P) */

#endif /* (CY_DEVICE_SECURE) */


/* [] END OF FILE */
