/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 *
 * Implements functions called by WHD user APIs, but not directly exposed to user
 *
 * This file provides functions which are not directly exposed to user but, called by end-user functions which allow actions such as
 * seting the MAC address, getting channel info, etc
 */

#include "whd_ap.h"
#include "whd_chip_constants.h"
#include "whd_debug.h"
#include "whd_events_int.h"
#include "whd_cdc_bdc.h"
#include "whd_thread_internal.h"
#include "whd_utils.h"
#include "whd_wifi_api.h"
#include "whd_wlioctl.h"

/******************************************************
* @cond       Constants
******************************************************/
#define MAC_ADDRESS_LOCALLY_ADMINISTERED_BIT 0x02

/******************************************************
*             Local Structures
******************************************************/



/******************************************************
*                   Variables
******************************************************/
void (*whd_wifi_link_update_callback)(void) = NULL;

/******************************************************
*             Function definitions
******************************************************/

uint32_t whd_wifi_set_mac_address(whd_interface_t ifp, whd_mac_t mac)
{
    whd_buffer_t buffer;
    uint32_t *data;
    whd_driver_t whd_driver = ifp->whd_driver;

    /* AP interface needs to come up with MAC different from STA  */
#ifdef APOLLO_AUDIO

    /* Work around the issue of asking API to set one address and it sets a different address.
     * This will cause any comparison of set and get mac address to fail.  TODO: move twiddling this
     * bit to a higher level.
     */
    if (0)
#else
    if (ifp->role == WHD_AP_ROLE)
#endif
    {
        whd_mac_t ap_mac_address;

        memcpy(&ap_mac_address, &mac, sizeof(whd_mac_t) );
        if (ap_mac_address.octet[0] & MAC_ADDRESS_LOCALLY_ADMINISTERED_BIT)
        {
            ap_mac_address.octet[0] &= (uint8_t) ~(MAC_ADDRESS_LOCALLY_ADMINISTERED_BIT);
        }
        else
        {
            ap_mac_address.octet[0] |= MAC_ADDRESS_LOCALLY_ADMINISTERED_BIT;
        }

        data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, sizeof(whd_mac_t), IOVAR_STR_CUR_ETHERADDR);
        CHECK_IOCTL_BUFFER(data);
        memcpy(data, &ap_mac_address, sizeof(whd_mac_t) );
        CHECK_RETURN(whd_cdc_send_iovar(ifp, CDC_SET, buffer, NULL) );

        if (memcmp(&mac, &ap_mac_address, sizeof(whd_mac_t) ) != 0)
        {
            WPRINT_WHD_INFO( (" STA MAC address : %02x:%02x:%02x:%02x:%02x:%02x \n"
                              " AP  MAC address : %02x:%02x:%02x:%02x:%02x:%02x \n",
                              mac.octet[0], mac.octet[1], mac.octet[2],
                              mac.octet[3], mac.octet[4], mac.octet[3],
                              ap_mac_address.octet[0], ap_mac_address.octet[1], ap_mac_address.octet[2],
                              ap_mac_address.octet[3], ap_mac_address.octet[4], ap_mac_address.octet[3]) );
        }
    }
    else
    {
        data = (uint32_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, sizeof(whd_mac_t), IOVAR_STR_CUR_ETHERADDR);
        CHECK_IOCTL_BUFFER(data);
        memcpy(data, &mac, sizeof(whd_mac_t) );
        CHECK_RETURN(whd_cdc_send_iovar(ifp, CDC_SET, buffer, NULL) );
    }

    return WHD_SUCCESS;
}

