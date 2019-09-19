/***************************************************************************//**
* \file CY8CPROTO-064-SB/cybsp.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CY8CPROTO-064-SB prototyping kit.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#include <stdlib.h>
#include "cybsp.h"
#include "cyhal_utils.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(CYBSP_WIFI_CAPABLE)
static cyhal_sdio_t sdio_obj;

cyhal_sdio_t* cybsp_get_wifi_sdio_obj(void)
{
    return &sdio_obj;
}
#endif

cy_rslt_t cybsp_init(void)
{
    /* Setup hardware manager to track resource usage then initialize all system (clock/power) board configuration */
    cy_rslt_t result = cyhal_hwmgr_init();
    init_cycfg_system();
    result = cybsp_register_sysclk_pm_callback();

#ifndef __MBED__
    if (CY_RSLT_SUCCESS == result)
    {
        /* Initialize User LEDs */
        result |= cybsp_led_init(CYBSP_USER_LED1);
        result |= cybsp_led_init(CYBSP_USER_LED2);
        /* Initialize User Buttons */
        result |= cybsp_btn_init(CYBSP_USER_BTN1);

        /* Initialize retargetting stdio to 'DEBUG_UART' peripheral */
        if (CY_RSLT_SUCCESS == result)
        {
            result = cybsp_retarget_init();
        }
    }

#if defined(CYBSP_WIFI_CAPABLE)
    /* Initialize SDIO interface. This must be done before other HAL API calls as some SDIO implementations require
     * specific peripheral instances.
     * NOTE: The full WiFi interface still needs to be initialized via cybsp_wifi_init_primary(). This is typically 
     * done when starting up WiFi. 
     */
    if (CY_RSLT_SUCCESS == result)
    {
        /* Reserves: CYBSP_WIFI_SDIO, CYBSP_WIFI_SDIO_D0, CYBSP_WIFI_SDIO_D1, CYBSP_WIFI_SDIO_D2, CYBSP_WIFI_SDIO_D3
         * CYBSP_WIFI_SDIO_CMD and CYBSP_WIFI_SDIO_CLK.
         */
        result = cyhal_sdio_init(
                &sdio_obj,
                CYBSP_WIFI_SDIO_CMD,
                CYBSP_WIFI_SDIO_CLK,
                CYBSP_WIFI_SDIO_D0,
                CYBSP_WIFI_SDIO_D1,
                CYBSP_WIFI_SDIO_D2,
                CYBSP_WIFI_SDIO_D3);
    }
#endif /* defined(CYBSP_WIFI_CAPABLE) */

    /* CYHAL_HWMGR_RSLT_ERR_INUSE error code could be returned if any needed for BSP resource was reserved by
     * user previously. Please review the Device Configurator (design.modus) and the BSP reservation list
     * (cyreservedresources.list) to make sure no resources are reserved by both.
     */
    return result;
}

#if defined(__cplusplus)
}
#endif
