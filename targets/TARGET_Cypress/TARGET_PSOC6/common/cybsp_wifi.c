/***************************************************************************//**
* \file cybsp_utils.c
*
* \brief
* Provides utility functions that are used by board support packages.
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
#if defined(CYBSP_WIFI_CAPABLE)

#include "cybsp_types.h"
#include "cybsp_wifi.h"
#include "cy_network_buffer.h"
#include "cmsis_os2.h"
#include "whd_types.h"
#include "cyhal.h"
#include "cybsp_wifi_sdio.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define THREAD_STACK_SIZE   	    5120
#define THREAD_PRIORITY   	        osPriorityHigh
#define COUNTRY                     WHD_COUNTRY_AUSTRALIA
#define DEFAULT_OOB_PIN		        0
#define WLAN_INTR_PRIORITY	        1
#define WLAN_POWER_UP_DELAY_MS      250

#define SDIO_ENUMERATION_TRIES      500
#define SDIO_RETRY_DELAY_MS         1
#define SDIO_BUS_LEVEL_MAX_RETRIES  5

#if !defined(CY_WIFI_OOB_INTR_PRIORITY)
#define CY_WIFI_OOB_INTR_PRIORITY   2
#endif/* !defined(CY_WIFI_OOB_INTR_PRIORITY) */

/* Determine whether to use the SDIO oob interrupt.
 * When CY_SDIO_BUS_NEEDS_OOB_INTR is defined,
 * use its value to determine enable status; otherwise,
 * default to enable.  Use CY_WIFI_HOST_WAKE_SW_FORCE
 * to force the enable status.
 */
#if !defined(CY_WIFI_HOST_WAKE_SW_FORCE)

#if !defined(CY_SDIO_BUS_NEEDS_OOB_INTR)
#define CY_SDIO_BUS_USE_OOB_INTR (1u)
#else
#define CY_SDIO_BUS_USE_OOB_INTR CY_SDIO_BUS_NEEDS_OOB_INTR
#endif /* !defined(CY_SDIO_BUS_NEEDS_OOB_INTR) */

#else

#define CY_SDIO_BUS_USE_OOB_INTR CY_WIFI_HOST_WAKE_SW_FORCE

#endif /* defined(CY_WIFI_HOST_WAKE_SW_FORCE) */

/* Define the host-wake configuration.
 * Choose the configurator settings over the HAL.
 */
#if (CY_SDIO_BUS_USE_OOB_INTR != 0)

/* Prefer configurator declarations over HAL */

#if defined(CYCFG_WIFI_HOST_WAKE_GPIO)
#define CY_WIFI_HOST_WAKE_GPIO CYCFG_WIFI_HOST_WAKE_GPIO
#else
#define CY_WIFI_HOST_WAKE_GPIO CYBSP_WIFI_HOST_WAKE
#endif

#if defined(CYCFG_WIFI_HOST_WAKE_GPIO_DM)
#define CY_WIFI_HOST_WAKE_GPIO_DM CYCFG_WIFI_HOST_WAKE_GPIO_DM
#else
#define CY_WIFI_HOST_WAKE_GPIO_DM CYBSP_WIFI_HOST_WAKE_GPIO_DM
#endif

#if defined(CYCFG_WIFI_HOST_WAKE_IRQ_EVENT)
#define CY_WIFI_HOST_WAKE_IRQ_EVENT CYCFG_WIFI_HOST_WAKE_IRQ_EVENT
#else
#define CY_WIFI_HOST_WAKE_IRQ_EVENT CYBSP_WIFI_HOST_WAKE_IRQ_EVENT
#endif

#else

/* Dummy macro declarations to appease compiler */
#define CY_WIFI_HOST_WAKE_GPIO 0
#define CY_WIFI_HOST_WAKE_GPIO_DM 0
#define CY_WIFI_HOST_WAKE_IRQ_EVENT 0

#endif /* (CY_SDIO_BUS_USE_OOB_INTR != 0) */

whd_driver_t whd_drv;
bool sdio_initialized = false;
cyhal_sdio_t sdio_obj;

static whd_buffer_funcs_t buffer_ops =
{
    .whd_host_buffer_get = cy_host_buffer_get,
    .whd_buffer_release = cy_buffer_release,
    .whd_buffer_get_current_piece_data_pointer = cy_buffer_get_current_piece_data_pointer,
    .whd_buffer_get_current_piece_size = cy_buffer_get_current_piece_size,
    .whd_buffer_set_size = cy_buffer_set_size,
    .whd_buffer_add_remove_at_front = cy_buffer_add_remove_at_front,
};

static whd_netif_funcs_t netif_ops =
{
    .whd_network_process_ethernet_data = cy_network_process_ethernet_data,
};

//TODO: Need to use resource implemenatation from abstraction layer.
extern whd_resource_source_t resource_ops;

static cy_rslt_t sdio_try_send_cmd(const cyhal_sdio_t *sdio_object, cyhal_transfer_t direction, \
                          cyhal_sdio_command_t command, uint32_t argument, uint32_t* response)
{
    uint8_t loop_count = 0;
    cy_rslt_t result = CYBSP_RSLT_WIFI_SDIO_ENUM_TIMEOUT;
    do
    {
        result = cyhal_sdio_send_cmd(sdio_object, direction, command, argument, response);
        if(result != CY_RSLT_SUCCESS)
        {
            Cy_SysLib_Delay(SDIO_RETRY_DELAY_MS);
        }
        loop_count++;
    }
    while(result != CY_RSLT_SUCCESS && loop_count <= SDIO_BUS_LEVEL_MAX_RETRIES);

    return result;
}

static cy_rslt_t cybsp_sdio_enumerate(const cyhal_sdio_t *sdio_object)
{
    cy_rslt_t result = CYBSP_RSLT_WIFI_SDIO_ENUM_TIMEOUT;
    uint32_t loop_count = 0;
    uint32_t rel_addr;
    uint32_t response_ignored = 0;
    uint32_t no_argument = 0;

    do
    {
        /* Send CMD0 to set it to idle state */
        sdio_try_send_cmd(sdio_object, CYHAL_WRITE, CYHAL_SDIO_CMD_GO_IDLE_STATE, no_argument, &response_ignored /*ignored*/);

        /* CMD5. */
        sdio_try_send_cmd(sdio_object, CYHAL_READ, CYHAL_SDIO_CMD_IO_SEND_OP_COND, no_argument, &response_ignored /*ignored*/);

        /* Send CMD3 to get RCA. */
        result = sdio_try_send_cmd(sdio_object, CYHAL_READ, CYHAL_SDIO_CMD_SEND_RELATIVE_ADDR, no_argument, &rel_addr);
        if(result != CY_RSLT_SUCCESS)
        {
            Cy_SysLib_Delay(SDIO_RETRY_DELAY_MS);
        }
        loop_count++;
    } while (result != CY_RSLT_SUCCESS && loop_count <= SDIO_ENUMERATION_TRIES);

    if(result == CY_RSLT_SUCCESS)
    {
        /* Send CMD7 with the returned RCA to select the card */
        result = sdio_try_send_cmd(sdio_object, CYHAL_WRITE, CYHAL_SDIO_CMD_SELECT_CARD, rel_addr, &response_ignored);
    }
    return result;
}

static cy_rslt_t init_sdio_whd(void)
{
    /* WiFi into reset */
    cy_rslt_t result = cyhal_gpio_init(CYBSP_WIFI_WL_REG_ON, CYHAL_GPIO_DIR_OUTPUT, CY_GPIO_DM_PULLUP, 0);

    if(result == CY_RSLT_SUCCESS)
    {
        /* Init SDIO Host */
        if (!sdio_initialized)
        {
            result = cybsp_sdio_init();
        }
        if(result == CY_RSLT_SUCCESS)
        {
            /* WiFi out of reset */
            cyhal_gpio_write(CYBSP_WIFI_WL_REG_ON, true);
            Cy_SysLib_Delay(WLAN_POWER_UP_DELAY_MS);
        }
        else
        {
            cyhal_gpio_free(CYBSP_WIFI_WL_REG_ON);
        }
    }
    return result;
}

static void deinit_sdio_whd(void)
{
    cyhal_sdio_free(&sdio_obj);
    cyhal_gpio_free(CYBSP_WIFI_WL_REG_ON);
    sdio_initialized = false;
}

static cy_rslt_t init_sdio_bus(void)
{
    cy_rslt_t result = cybsp_sdio_enumerate(&sdio_obj);
    if(result == CY_RSLT_SUCCESS)
    {
        whd_sdio_config_t whd_sdio_config;
        whd_oob_config_t oob_config;
        cyhal_sdio_cfg_t config;

        oob_config.host_oob_pin = CY_WIFI_HOST_WAKE_GPIO;
        oob_config.dev_gpio_sel = DEFAULT_OOB_PIN;
        oob_config.is_falling_edge = (CY_WIFI_HOST_WAKE_IRQ_EVENT == CYHAL_GPIO_IRQ_FALL)
            ? WHD_TRUE
            : WHD_FALSE;
        oob_config.intr_priority = CY_WIFI_OOB_INTR_PRIORITY;

        whd_sdio_config.sdio_1bit_mode = WHD_FALSE;
        whd_sdio_config.high_speed_sdio_clock = WHD_FALSE;
        whd_sdio_config.oob_config = oob_config;
        whd_bus_sdio_attach(whd_drv, &whd_sdio_config, &sdio_obj);

        /* Increase frequency to 25 MHz for better performance */
        config.frequencyhal_hz = 25000000;
        config.block_size = 0;
        cyhal_sdio_configure(&sdio_obj, &config);
    }
    return result;
}

cy_rslt_t cybsp_wifi_init_primary(whd_interface_t* interface)
{
    whd_init_config_t whd_init_config;
    whd_init_config.thread_stack_size = (uint32_t) THREAD_STACK_SIZE;
    whd_init_config.thread_stack_start = (uint8_t *)malloc(THREAD_STACK_SIZE);
    whd_init_config.thread_priority = (uint32_t) THREAD_PRIORITY;
    whd_init_config.country = COUNTRY;

    uint32_t ret = whd_init(&whd_drv, &whd_init_config, &resource_ops, &buffer_ops, &netif_ops);
    if(ret == WHD_SUCCESS)
    {
        result = init_sdio_bus();
    }
    return result;
}

cy_rslt_t cybsp_wifi_init_secondary(whd_interface_t* interface, whd_mac_t* mac_address)
{
    return whd_add_secondary_interface(whd_drv, mac_address, interface);
}

whd_driver_t cybsp_get_wifi_driver(void)
{
    return whd_drv;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CYBSP_WIFI_CAPABLE) */
