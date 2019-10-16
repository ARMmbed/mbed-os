/***************************************************************************//**
* \file cybsp_wifi.c
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

#include "cybsp.h"
#include "cybsp_wifi.h"
#include "cy_network_buffer.h"
#include "cyabs_rtos.h"
#include "whd_types.h"
#include "cyhal.h"
#include "cycfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define THREAD_STACK_SIZE   	    5120
#define THREAD_PRIORITY   	        CY_RTOS_PRIORITY_HIGH
#define COUNTRY                     WHD_COUNTRY_AUSTRALIA
#define DEFAULT_OOB_PIN		        0
#define WLAN_POWER_UP_DELAY_MS      250

#define SDIO_ENUMERATION_TRIES      500
#define SDIO_RETRY_DELAY_MS         1
#define SDIO_BUS_LEVEL_MAX_RETRIES  5

#if !defined(CY_WIFI_OOB_INTR_PRIORITY)
#define CY_WIFI_OOB_INTR_PRIORITY   2
#endif/* !defined(CY_WIFI_OOB_INTR_PRIORITY) */

/* Determine whether to use the SDIO oob interrupt.
 * Use CY_WIFI_HOST_WAKE_SW_FORCE to force the enable status.
 */
#if !defined(CY_WIFI_HOST_WAKE_SW_FORCE)
    #define CY_SDIO_BUS_USE_OOB_INTR (1u)
#else
    #define CY_SDIO_BUS_USE_OOB_INTR CY_WIFI_HOST_WAKE_SW_FORCE
#endif /* defined(CY_WIFI_HOST_WAKE_SW_FORCE) */

#if (CY_SDIO_BUS_USE_OOB_INTR != 0)
    /* Setup configuration based on configurator or BSP, where configurator has precedence. */
    #if defined(CYCFG_WIFI_HOST_WAKE_ENABLED)
        #define CY_WIFI_HOST_WAKE_GPIO CYCFG_WIFI_HOST_WAKE_GPIO
        #define CY_WIFI_HOST_WAKE_IRQ_EVENT CYCFG_WIFI_HOST_WAKE_IRQ_EVENT
    #else
        /* Setup host-wake pin */
        #if defined(CYBSP_WIFI_HOST_WAKE)
            #define CY_WIFI_HOST_WAKE_GPIO CYBSP_WIFI_HOST_WAKE
        #else
            #error "CYBSP_WIFI_HOST_WAKE must be defined"
        #endif
        /* Setup host-wake irq */
        #if defined(CYBSP_WIFI_HOST_WAKE_IRQ_EVENT)
            #define CY_WIFI_HOST_WAKE_IRQ_EVENT CYBSP_WIFI_HOST_WAKE_IRQ_EVENT
        #else
            #error "CYBSP_WIFI_HOST_WAKE_IRQ_EVENT must be defined"
        #endif
    #endif
#else
    #define CY_WIFI_HOST_WAKE_GPIO CYHAL_NC_PIN_VALUE
    #define CY_WIFI_HOST_WAKE_IRQ_EVENT 0
#endif /* (CY_SDIO_BUS_USE_OOB_INTR != 0) */

static whd_driver_t whd_drv;

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

static cy_rslt_t init_sdio_bus(void)
{
    cy_rslt_t result = reset_wifi_chip();
    if(result == CY_RSLT_SUCCESS)
    {
        cyhal_sdio_t* sdio_p = cybsp_get_wifi_sdio_obj();
        cy_rslt_t result = cybsp_sdio_enumerate(sdio_p);
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
            whd_bus_sdio_attach(whd_drv, &whd_sdio_config, sdio_p);

            /* Increase frequency to 25 MHz for better performance */
            config.frequencyhal_hz = 25000000;
            config.block_size = 0;
            cyhal_sdio_configure(sdio_p, &config);
        }
    }
    return result;
}

cy_rslt_t cybsp_wifi_init_primary(whd_interface_t* interface)
{
    whd_init_config_t whd_init_config;
    whd_init_config.thread_stack_size = (uint32_t)THREAD_STACK_SIZE;
    whd_init_config.thread_stack_start = NULL;
    whd_init_config.thread_priority =  (uint32_t)THREAD_PRIORITY;
    whd_init_config.country = COUNTRY;

    cy_rslt_t result = whd_init(&whd_drv, &whd_init_config, &resource_ops, &buffer_ops, &netif_ops);
    if(result == CY_RSLT_SUCCESS)
    {
        result = init_sdio_bus();
        if(result == CY_RSLT_SUCCESS)
        {
            result = whd_wifi_on(whd_drv, interface);
        }
    }
    return result;
}

cy_rslt_t cybsp_wifi_init_secondary(whd_interface_t* interface, whd_mac_t* mac_address)
{
    return whd_add_secondary_interface(whd_drv, mac_address, interface);
}

cy_rslt_t cybsp_wifi_deinit(whd_interface_t interface)
{
    cy_rslt_t result = whd_wifi_off(interface);
    if(result == CY_RSLT_SUCCESS)
    {
        result = whd_deinit(interface);
        if(result == CY_RSLT_SUCCESS)
        {
            cyhal_gpio_free(CYBSP_WIFI_WL_REG_ON);
        }
    }
    return result;
}

whd_driver_t cybsp_get_wifi_driver(void)
{
    return whd_drv;
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CYBSP_WIFI_CAPABLE) */
