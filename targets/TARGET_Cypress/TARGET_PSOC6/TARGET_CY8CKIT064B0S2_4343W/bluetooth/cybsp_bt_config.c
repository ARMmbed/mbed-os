/***********************************************************************************************//**
 * \copyright
 * Copyright 2020-2021 Cypress Semiconductor Corporation
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

#if defined(COMPONENT_WICED_BLE) || defined(COMPONENT_WICED_DUALMODE)

#include "cybsp_bt_config.h"
#include "wiced_bt_dev.h"

const cybt_platform_config_t cybsp_bt_platform_cfg =
{
    .hci_config                             =
    {
        .hci_transport                      = CYBT_HCI_UART,

        .hci                                =
        {
            .hci_uart                       =
            {
                .uart_tx_pin                = CYBSP_BT_UART_TX,
                .uart_rx_pin                = CYBSP_BT_UART_RX,
                .uart_rts_pin               = CYBSP_BT_UART_RTS,
                .uart_cts_pin               = CYBSP_BT_UART_CTS,

                .baud_rate_for_fw_download  = CYBSP_BT_PLATFORM_CFG_BAUD_DOWNLOAD,
                .baud_rate_for_feature      = CYBSP_BT_PLATFORM_CFG_BAUD_FEATURE,

                .data_bits                  = CYBSP_BT_PLATFORM_CFG_BITS_DATA,
                .stop_bits                  = CYBSP_BT_PLATFORM_CFG_BITS_STOP,
                .parity                     = CYHAL_UART_PARITY_NONE,
                .flow_control               = true
            }
        }
    },

    .controller_config                      =
    {
        .bt_power_pin                       = CYBSP_BT_POWER,
        #if (bt_0_power_0_ENABLED == 1)
        .sleep_mode                         =
        {
            .sleep_mode_enabled             = CYCFG_BT_LP_ENABLED,
            .device_wakeup_pin              = CYCFG_BT_DEV_WAKE_GPIO,
            .host_wakeup_pin                = CYCFG_BT_HOST_WAKE_GPIO,
            .device_wake_polarity           = CYCFG_BT_DEV_WAKE_POLARITY,
            .host_wake_polarity             = CYCFG_BT_HOST_WAKE_IRQ_EVENT
        }
        #else
        .sleep_mode                         =
        {
            .sleep_mode_enabled             = true,
            .device_wakeup_pin              = CYBSP_BT_DEVICE_WAKE,
            .host_wakeup_pin                = CYBSP_BT_HOST_WAKE,
            .device_wake_polarity           = CYBT_WAKE_ACTIVE_LOW,
            .host_wake_polarity             = CYBT_WAKE_ACTIVE_LOW
        }
        #endif /* (bt_0_power_0_ENABLED == 1) */
    },

    .task_mem_pool_size                     = CYBSP_BT_PLATFORM_CFG_MEM_POOL_BYTES
};

#endif /* defined(COMPONENT_WICED_BLE) || defined(COMPONENT_WICED_DUALMODE) */
