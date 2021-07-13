/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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
 */

/** @file
 *  Header for whd_driver structure
 */

#ifndef INCLUDED_WHD_INT_H
#define INCLUDED_WHD_INT_H

#include "whd_thread.h"
#include "whd_sdpcm.h"
#include "whd_cdc_bdc.h"
#include "whd_chip.h"
#include "whd_ap.h"
#include "whd_debug.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    uint32_t tx_total; /* Total number of TX packets sent from WHD */
    uint32_t rx_total; /* Total number of RX packets received at WHD */
    uint32_t tx_no_mem; /* Number of times WHD could not send due to no buffer */
    uint32_t rx_no_mem; /* Number of times WHD could not receive due to no buffer */
    uint32_t tx_fail; /* Number of times TX packet failed */
    uint32_t no_credit; /* Number of times WHD could not send due to no credit */
    uint32_t flow_control; /* Number of times WHD Flow control is enabled */
    uint32_t internal_host_buffer_fail_with_timeout; /* Internal host buffer get failed after timeout */
} whd_stats_t;

#define WHD_INTERFACE_MAX 3
typedef enum
{
    WHD_INVALID_ROLE           = 0,
    WHD_STA_ROLE               = 1,         /**< STA or Client Interface     */
    WHD_AP_ROLE                = 2,         /**< softAP Interface  */
    WHD_P2P_ROLE               = 3,         /**< P2P Interface  */
} whd_interface_role_t;


struct whd_interface
{
    whd_driver_t whd_driver;
    uint8_t ifidx;
    uint8_t bsscfgidx;

    char if_name[WHD_MSG_IFNAME_MAX];
    whd_interface_role_t role;
    whd_mac_t mac_addr;
    uint8_t event_reg_list[WHD_EVENT_ENTRY_MAX];
    whd_bool_t state;
};

struct whd_driver
{
    whd_interface_t iflist[WHD_INTERFACE_MAX];
    uint8_t if2ifp[WHD_INTERFACE_MAX];

    /* Bus variables */
    struct whd_bus_info *bus_if;
    struct whd_bus_priv *bus_priv;
    struct whd_bus_common_info *bus_common_info;
    whd_buffer_funcs_t *buffer_if;
    whd_netif_funcs_t *network_if;
    whd_resource_source_t *resource_if;

    whd_bool_t bus_gspi_32bit;

    whd_thread_info_t thread_info;
    whd_cdc_bdc_info_t cdc_bdc_info;
    whd_error_info_t error_info;
    whd_sdpcm_info_t sdpcm_info;
    whd_internal_info_t internal_info;
    whd_ap_int_info_t ap_info;
    whd_chip_info_t chip_info;

    whd_stats_t whd_stats;
    whd_country_code_t country;

    whd_ioctl_log_t whd_ioctl_log[WHD_IOCTL_LOG_SIZE];
    int whd_ioctl_log_index;
};

whd_result_t whd_add_interface(whd_driver_t whd_driver, uint8_t bsscfgidx, uint8_t ifidx,
                               const char *name, whd_mac_t *mac_addr,  whd_interface_t *ifpp);

whd_result_t whd_add_primary_interface(whd_driver_t whd_driver, whd_interface_t *ifpp);

whd_interface_t whd_get_primary_interface(whd_driver_t whd_driver);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* INCLUDED_WHD_INT_H */

