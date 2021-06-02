/***********************************************************************************************//**
 * \file cybsp.c
 *
 * Description:
 * Provides initialization code for starting up the hardware contained on the
 * Cypress board.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2018-2021 Cypress Semiconductor Corporation
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
 **************************************************************************************************/

#include <stdlib.h>
#include "cy_syspm.h"
#include "cy_sysclk.h"
#include "cybsp.h"
#if defined(CY_USING_HAL)
#include "cyhal_hwmgr.h"
#include "cyhal_syspm.h"
#endif

#if !defined (CY_CFG_PWR_SYS_IDLE_MODE) && defined(__MBED__)
#include "mbed_power_mgmt.h"
#endif

// Define the VDDA voltage - this is needed as 'Power' configuration is disabled
// from design.modus for now.
#ifndef CY_CFG_PWR_VDDA_MV
    #define CY_CFG_PWR_VDDA_MV              (3300)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// The sysclk deep sleep callback is recommended to be the last callback that is executed before
// entry into deep sleep mode and the first one upon exit the deep sleep mode.
// Doing so minimizes the time spent on low power mode entry and exit.
#ifndef CYBSP_SYSCLK_PM_CALLBACK_ORDER
    #define CYBSP_SYSCLK_PM_CALLBACK_ORDER  (255u)
#endif

#if defined(CYBSP_WIFI_CAPABLE) && defined(CY_USING_HAL)
static cyhal_sdio_t sdio_obj;

//--------------------------------------------------------------------------------------------------
// cybsp_get_wifi_sdio_obj
//--------------------------------------------------------------------------------------------------
cyhal_sdio_t* cybsp_get_wifi_sdio_obj(void)
{
    return &sdio_obj;
}


#endif // if defined(CYBSP_WIFI_CAPABLE) && defined(CY_USING_HAL)

//--------------------------------------------------------------------------------------------------
// cybsp_register_sysclk_pm_callback
//
// Registers a power management callback that prepares the clock system for entering deep sleep mode
// and restore the clocks upon wakeup from deep sleep.
// NOTE: This is called automatically as part of \ref cybsp_init
//--------------------------------------------------------------------------------------------------
static cy_rslt_t cybsp_register_sysclk_pm_callback(void)
{
    cy_rslt_t                             result                         = CY_RSLT_SUCCESS;
    static cy_stc_syspm_callback_params_t cybsp_sysclk_pm_callback_param = { NULL, NULL };
    static cy_stc_syspm_callback_t        cybsp_sysclk_pm_callback       =
    {
        .callback       = &Cy_SysClk_DeepSleepCallback,
        .type           = CY_SYSPM_DEEPSLEEP,
        .callbackParams = &cybsp_sysclk_pm_callback_param,
        .order          = CYBSP_SYSCLK_PM_CALLBACK_ORDER
    };

    if (!Cy_SysPm_RegisterCallback(&cybsp_sysclk_pm_callback))
    {
        result = CYBSP_RSLT_ERR_SYSCLK_PM_CALLBACK;
    }
    return result;
}


//--------------------------------------------------------------------------------------------------
// cybsp_init
//--------------------------------------------------------------------------------------------------
cy_rslt_t cybsp_init(void)
{
    // Setup hardware manager to track resource usage then initialize all system (clock/power) board
    // configuration
    #if defined(CY_USING_HAL)
    cy_rslt_t result = cyhal_hwmgr_init();

    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_syspm_init();
    }

    #ifdef CY_CFG_PWR_VDDA_MV
    if (CY_RSLT_SUCCESS == result)
    {
        cyhal_syspm_set_supply_voltage(CYHAL_VOLTAGE_SUPPLY_VDDA, CY_CFG_PWR_VDDA_MV);
    }
    #endif

    #else // if defined(CY_USING_HAL)
    cy_rslt_t result = CY_RSLT_SUCCESS;
    #endif // if defined(CY_USING_HAL)

    #if defined(COMPONENT_BSP_DESIGN_MODUS) || defined(COMPONENT_CUSTOM_DESIGN_MODUS)
    init_cycfg_all();
    #endif

    if (CY_RSLT_SUCCESS == result)
    {
        result = cybsp_register_sysclk_pm_callback();
    }

    #if !defined(CY_CFG_PWR_SYS_IDLE_MODE)
    #ifdef __MBED__
    // Disable deep-sleep
    sleep_manager_lock_deep_sleep();
    #else
    cyhal_syspm_lock_deepsleep();
    #endif
    #endif

    // Reserve clock dividers used by NP
    cyhal_clock_divider_t clock1;
    cyhal_hwmgr_allocate_clock(&clock1, CY_SYSCLK_DIV_16_BIT, true);
    cyhal_clock_divider_t clock2;
    cyhal_hwmgr_allocate_clock(&clock2, CY_SYSCLK_DIV_16_BIT, true);

    // CYHAL_HWMGR_RSLT_ERR_INUSE error code could be returned if any needed for BSP resource was
    // reserved by user previously. Please review the Device Configurator (design.modus) and the BSP
    // reservation list (cyreservedresources.list) to make sure no resources are reserved by both.
    return result;
}


#if defined(__cplusplus)
}
#endif
