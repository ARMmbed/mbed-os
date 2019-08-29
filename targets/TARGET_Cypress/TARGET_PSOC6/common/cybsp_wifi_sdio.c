/***************************************************************************//**
* \file cybsp_wifi_sdio.c
*
* \brief
* Utility functions to intialize the SDIO communication bus used to 
* communicate with the WiFi radio.
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
#include "cybsp_wifi_sdio.h"
#include "cyhal_gpio_impl.h"
#include "cybsp_types.h"

#if defined(CYBSP_WIFI_CAPABLE)

#define WLAN_POWER_UP_DELAY_MS      250

static cyhal_sdio_t sdio_obj;

static cy_rslt_t reset_wifi_chip(void)
{
    /* WiFi into reset */
    cy_rslt_t result = cyhal_gpio_init(CYBSP_WIFI_WL_REG_ON, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_PULLUP, 0);
    if(result == CY_RSLT_SUCCESS)
    {
        /* WiFi out of reset */
        cyhal_gpio_write(CYBSP_WIFI_WL_REG_ON, true);
        Cy_SysLib_Delay(WLAN_POWER_UP_DELAY_MS);
    }
    return result;
}

void cybsp_wifi_sdio_deinit(void)
{
    cyhal_sdio_free(&sdio_obj);
    cyhal_gpio_free(CYBSP_WIFI_WL_REG_ON);
}

cy_rslt_t cybsp_wifi_sdio_init(void)
{
    cy_rslt_t result = cyhal_sdio_init(&sdio_obj, CYBSP_WIFI_SDIO_CMD, CYBSP_WIFI_SDIO_CLK, CYBSP_WIFI_SDIO_D0, CYBSP_WIFI_SDIO_D1, CYBSP_WIFI_SDIO_D2, CYBSP_WIFI_SDIO_D3);
    if(result == CY_RSLT_SUCCESS)
    {
        cy_rslt_t result = reset_wifi_chip();
        if (result != CY_RSLT_SUCCESS)
        {
            cybsp_wifi_sdio_deinit();
        }
    }
    return result;
}

cyhal_sdio_t* cybsp_get_wifi_sdio_obj(void)
{
    return &sdio_obj;
}

#endif /* defined(CYBSP_WIFI_CAPABLE) */
