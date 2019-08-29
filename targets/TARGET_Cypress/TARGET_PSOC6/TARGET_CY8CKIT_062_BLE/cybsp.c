/***************************************************************************//**
* \file CY8CKIT-062-BLE/cybsp.c
*
* Description:
* Provides basic hardware initialization for the CY8CKIT-062-BLE pioneer kit.
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
#include "cycfg_system.h"

#if defined(__cplusplus)
extern "C" {
#endif

cy_rslt_t cybsp_init(void)
{
    cy_rslt_t result;

    result = cyhal_hwmgr_init();
    init_cycfg_system();

    if (CY_RSLT_SUCCESS == result)
    {
        result = cybsp_register_sysclk_pm_callback();
    }

#ifndef __MBED__
    if (CY_RSLT_SUCCESS == result)
    {
        /* Initialize User LEDs */
        /* Reserves: CYBSP_USER_LED1 */
        result |= cybsp_led_init(CYBSP_USER_LED1);
        /* Reserves: CYBSP_USER_LED2 */
        result |= cybsp_led_init(CYBSP_USER_LED2);
        /* Reserves: CYBSP_USER_LED3 */
        result |= cybsp_led_init(CYBSP_USER_LED3);
        /* Reserves: CYBSP_USER_LED4 */
        result |= cybsp_led_init(CYBSP_USER_LED4);
        /* Reserves: CYBSP_USER_LED5 */
        result |= cybsp_led_init(CYBSP_USER_LED5);
        /* Initialize User Buttons */
        /* Reserves: CYBSP_USER_BTN1 */
        result |= cybsp_btn_init(CYBSP_USER_BTN1);
        CY_ASSERT(CY_RSLT_SUCCESS == result);

        /* Initialize retargetting stdio to 'DEBUG_UART' peripheral */
        if (CY_RSLT_SUCCESS == result)
        {
           /* Reserves: CYBSP_DEBUG_UART_RX, CYBSP_DEBUG_UART_TX, corresponding SCB instance and one of available
            *   clock dividers */
            result = cybsp_retarget_init();
        }
    }
#endif /* __MBED__ */

    /* CYHAL_HWMGR_RSLT_ERR_INUSE error code could be returned if any needed for BSP resource was reserved by
    *   user previously. Please review the Device Configurator (design.modus) and the BSP reservation list
    *   (cyreservedresources.list) to make sure no resources are reserved by both. */
    return result;
}

#if defined(__cplusplus)
}
#endif
