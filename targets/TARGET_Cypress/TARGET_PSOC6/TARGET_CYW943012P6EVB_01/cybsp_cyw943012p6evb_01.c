/***************************************************************************//**
* \file cybsp_cyw943012p6evb_01.c
*
* Description:
* Provides APIs for interacting with the hardware contained on the Cypress
* CYW943012P6EVB-01 kit.
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
#include "cybsp_cyw943012p6evb_01.h"
#include "cyhal_implementation.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

cy_rslt_t cybsp_init(void)
{
	init_cycfg_system();

    cy_rslt_t result = CY_RSLT_SUCCESS;

#ifndef __MBED__
    /* Initialize User LEDs */
    result |= cybsp_led_init(CYBSP_USER_LED1);
    result |= cybsp_led_init(CYBSP_USER_LED2);
    result |= cybsp_led_init(CYBSP_USER_LED3);
    /* Initialize User Buttons */
    result |= cybsp_btn_init(CYBSP_USER_BTN1);

    CY_ASSERT(CY_RSLT_SUCCESS == result);
#endif

#if defined(CYBSP_WIFI_CAPABLE)
    /* Initialize UDB SDIO interface. This must be done before any other HAL API attempts to allocate clocks or DMA
       instances. The UDB SDIO interface uses specific instances which are reserved as part of this call.
       NOTE: The full WiFi interface still needs to be initialized via cybsp_wifi_init(). This is typically done
       when starting up WiFi. */
    if (CY_RSLT_SUCCESS == result)
    {
        result = cybsp_sdio_init();
    }
#endif

#if defined(CYBSP_RETARGET_ENABLED)
    /* Initialize retargetting stdio to 'DEBUG_UART' peripheral */
    if (CY_RSLT_SUCCESS == result)
    {
        result = cybsp_retarget_init();
    }
#endif

    return result;
}

#if defined(__cplusplus)
}
#endif
