/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ethernet_mac_api.h"
#include "eventOS_event.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "MAC/rf_driver_storage.h"

typedef struct eth_mac_internal_s {
    eth_mac_api_t *mac_api;
    arm_device_driver_list_s *dev_driver;
    uint8_t *mtu_ptr;
    uint16_t mtu_size;
    uint8_t mac48[6];
    bool active_data_request;
    int8_t tasklet_id;
    //linked list link
}eth_mac_internal_t;

static eth_mac_internal_t mac_store = //Hack only at this point, later put into linked list
{
    .tasklet_id = -1
};

#define ETH_INIT_EVENT 0
#define ETH_DATA_IND_EVENT 1
#define ETH_DATA_CNF_EVENT 2


#define ETHERNET_HDROFF_DST_ADDR    0
#define ETHERNET_HDROFF_SRC_ADDR    6
#define ETHERNET_HDROFF_TYPE        12
#define ETHERNET_HDRLEN             14

static int8_t eth_mac_api_init(eth_mac_api_t *api, eth_mac_data_confirm *conf_cb, eth_mac_data_indication *ind_cb, uint8_t parent_id);
static void data_req(const eth_mac_api_t* api, const eth_data_req_t *data);
static int8_t mac48_address_set( const eth_mac_api_t* api, const uint8_t *mac48);
static int8_t mac48_address_get( const eth_mac_api_t* api, uint8_t *mac48_buf);

static int8_t eth_mac_net_phy_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id);
static int8_t eth_mac_net_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);
static void ethernet_mac_tasklet(arm_event_s *event);

int8_t ethernet_mac_destroy(eth_mac_api_t *mac_api)
{
    if( !mac_api || mac_store.mac_api != mac_api ){
        return -1;
    }

    mac_store.active_data_request = false;
    ns_dyn_mem_free(mac_store.mac_api);
    mac_store.mac_api = NULL;
    mac_store.dev_driver = NULL;
    ns_dyn_mem_free(mac_store.mtu_ptr);
    mac_store.mtu_ptr = NULL;
    mac_store.mtu_size = 0;
    return 0;
}



eth_mac_api_t *ethernet_mac_create(int8_t driver_id)
{
    //TODO: Refactor this away, Drivers should be stored in MAC layer in future
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);
    if( !driver || !driver->phy_driver){
        return NULL;
    }

    //Accept to return same mac if mac is generated and driver is same
    if (mac_store.mac_api) {
        if (mac_store.dev_driver == driver) {
            return mac_store.mac_api;
        }
        return NULL;
    }

    uint8_t *buffer_ptr = NULL;
    uint16_t buffer_length = 0;
    bool address_resolution_needed = true;

    switch (driver->phy_driver->link_type) {

        case PHY_LINK_SLIP:
            //Do not Allocate
            address_resolution_needed = false;
            buffer_length = 0;
            break;

        case PHY_LINK_TUN:
            address_resolution_needed = false;
            buffer_length = 1500 + 4;
            break;
        case PHY_LINK_ETHERNET_TYPE:

           buffer_length = 1500 + ETHERNET_HDRLEN;



            break;

        default:
            return NULL;
    }
    if (buffer_length) {
        buffer_ptr = ns_dyn_mem_alloc(buffer_length);
        if (!buffer_ptr) {
            return NULL;
        }
    }

    eth_mac_api_t *this = ns_dyn_mem_alloc(sizeof(eth_mac_api_t));
    if( !this ){
        ns_dyn_mem_free(buffer_ptr);
        return NULL;
    }
    memset(this, 0, sizeof(eth_mac_api_t));
    this->mac_initialize = &eth_mac_api_init;
    this->data_req = &data_req;
    this->mac48_get = &mac48_address_get;
    this->mac48_set = &mac48_address_set;

    this->address_resolution_needed = address_resolution_needed;

    memset(&mac_store.mac48, 0, 6);
    mac_store.active_data_request = false;
    mac_store.mac_api = this;
    mac_store.dev_driver = driver;
    mac_store.mtu_ptr = buffer_ptr;
    mac_store.mtu_size = buffer_length;

    memcpy(&mac_store.mac48, mac_store.dev_driver->phy_driver->PHY_MAC, 6);
    if (mac_store.tasklet_id == -1) {
        mac_store.tasklet_id = eventOS_event_handler_create(&ethernet_mac_tasklet, ETH_INIT_EVENT);
    }
    arm_net_phy_init(driver->phy_driver, &eth_mac_net_phy_rx, &eth_mac_net_phy_tx_done);

    return this;
}

static void ethernet_mac_tasklet(arm_event_s *event)
{
    uint8_t event_type = event->event_type;

    switch (event_type) {
        case ETH_DATA_IND_EVENT:{
            eth_data_ind_t *data_ind = event->data_ptr;
            mac_store.mac_api->data_ind_cb(mac_store.mac_api, data_ind);
            ns_dyn_mem_free(((eth_data_ind_t*)event->data_ptr)->msdu);
            ns_dyn_mem_free(event->data_ptr);
            break;
            }
        case ETH_DATA_CNF_EVENT:{
            eth_data_conf_t *data_conf = event->data_ptr;
            mac_store.mac_api->data_conf_cb(mac_store.mac_api, data_conf);
            ns_dyn_mem_free(event->data_ptr);
            mac_store.active_data_request = false;
            break;
            }
        case ETH_INIT_EVENT:
        default:{
            break;
            }
    }
}

static int8_t eth_mac_api_init( eth_mac_api_t *api, eth_mac_data_confirm *conf_cb, eth_mac_data_indication *ind_cb, uint8_t parent_id)
{
    if( mac_store.mac_api != api ){
        return -1;
    }
    eth_mac_api_t *cur = mac_store.mac_api;
    cur->data_conf_cb = conf_cb;
    cur->data_ind_cb = ind_cb;
    cur->parent_id = parent_id;

    return 0;
}

static void data_req(const eth_mac_api_t* api, const eth_data_req_t *data)
{
    if( mac_store.mac_api != api || !mac_store.dev_driver->phy_driver || !data  || !data->msduLength){
        return;
    }


    uint8_t *data_ptr;
    uint16_t data_length;

    //Build header
    switch (mac_store.dev_driver->phy_driver->link_type) {
        case PHY_LINK_ETHERNET_TYPE:

            if (data->msduLength + ETHERNET_HDRLEN > mac_store.mtu_size || !data->dstAddress || !data->srcAddress ) {
                return;
            }

            data_ptr = mac_store.mtu_ptr;
            data_length = data->msduLength + ETHERNET_HDRLEN;
            memcpy(data_ptr + ETHERNET_HDROFF_DST_ADDR, data->dstAddress, 6);
            memcpy(data_ptr + ETHERNET_HDROFF_SRC_ADDR, data->srcAddress, 6);
            common_write_16_bit(data->etehernet_type, data_ptr + ETHERNET_HDROFF_TYPE);
            memcpy(data_ptr + ETHERNET_HDRLEN, data->msdu, data->msduLength);
            break;

        case PHY_LINK_TUN:
            if (data->msduLength + 4 > mac_store.mtu_size) {
                return;
            }
            data_ptr = mac_store.mtu_ptr;
            /* TUN header
             * [ TUN FLAGS 2B | PROTOCOL 2B | PAYLOAD ]
             * Tun flags may be 0, and protocol is same as in ether-type field, so
             * replace flags with e.g. IFF_TUN and packet is converted to TUN frame
             */
            common_write_16_bit(0, data_ptr);
            common_write_16_bit(data->etehernet_type, data_ptr + 2);
            memcpy(data_ptr + 4, data->msdu, data->msduLength);
            data_length = data->msduLength + 4;
            break;

        default: //SLIP
            data_ptr = data->msdu;
            data_length = data->msduLength;
            break;
    }

    mac_store.active_data_request = true;
    mac_store.dev_driver->phy_driver->tx(data_ptr, data_length, 0, PHY_LAYER_PAYLOAD);
}

static int8_t eth_mac_net_phy_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);
    if (!data_ptr || !driver || driver != mac_store.dev_driver) {
        return -1;
    }

    if (data_len == 0) {
        return -1;
    }

    eth_data_ind_t *data_ind = ns_dyn_mem_temporary_alloc(sizeof(eth_data_ind_t));
    if (!data_ind){
        return -1;
    }
    memset(data_ind, 0, sizeof(eth_data_ind_t));

    if (driver->phy_driver->link_type == PHY_LINK_ETHERNET_TYPE) {
        if (data_len < ETHERNET_HDRLEN + 1) {
            ns_dyn_mem_free(data_ind);
            return -1;
        }

        memcpy(data_ind->dstAddress,data_ptr +  ETHERNET_HDROFF_DST_ADDR, 6);
        memcpy(data_ind->srcAddress,data_ptr +  ETHERNET_HDROFF_SRC_ADDR, 6);

        data_ind->etehernet_type = common_read_16_bit(data_ptr + ETHERNET_HDROFF_TYPE);

        data_ptr += ETHERNET_HDRLEN;
        data_len -= ETHERNET_HDRLEN;

    } else if (driver->phy_driver->link_type == PHY_LINK_TUN) {
        if (data_len < 5) {
            ns_dyn_mem_free(data_ind);
            return -1;
        }
        /* TUN header
         * [ TUN FLAGS 2B | PROTOCOL 2B | PAYLOAD ]
         * Protocol is ether-type id.
         */
        data_ind->etehernet_type = common_read_16_bit(data_ptr + 2);

        data_len -= 4;
        data_ptr += 4;
    } else if (driver->phy_driver->link_type == PHY_LINK_SLIP) {
        data_ind->etehernet_type = ETHERTYPE_IPV6;
    }

    data_ind->msdu = ns_dyn_mem_temporary_alloc(data_len);
    if(!data_ind->msdu){
        ns_dyn_mem_free(data_ind);
        return -1;
    }
    memcpy(data_ind->msdu, data_ptr, data_len);
    data_ind->msduLength = data_len;
    data_ind->dbm = dbm;
    data_ind->link_quality = link_quality;

    arm_event_s event = {
            .receiver = mac_store.tasklet_id,
            .sender = 0,
            .event_id = 0,
            .data_ptr = data_ind,
            .event_type = ETH_DATA_IND_EVENT,
            .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event)) {
        ns_dyn_mem_free(data_ind->msdu);
        ns_dyn_mem_free(data_ind);
        return -1;
    }

    return 0;
}

static int8_t eth_mac_net_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    (void)tx_handle;
    (void)cca_retry;
    (void)tx_retry;
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);
    if (!driver) {
        return -1;
    }

    if( mac_store.active_data_request ){
        mac_store.active_data_request = false;
        eth_data_conf_t *data_conf = ns_dyn_mem_temporary_alloc(sizeof(eth_data_conf_t));
        if (!data_conf){
            return -1;
        }
        data_conf->status = status;
        arm_event_s event = {
                .receiver = mac_store.tasklet_id,
                .sender = 0,
                .event_id = 0,
                .data_ptr = data_conf,
                .event_type = ETH_DATA_CNF_EVENT,
                .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
        };

        return eventOS_event_send(&event);
    }
    return 0;
}

static int8_t mac48_address_set( const eth_mac_api_t* api, const uint8_t *mac48)
{
    if(!mac48 || !api || mac_store.mac_api != api ){
        return -1;
    }
    memcpy(mac_store.mac48, mac48, 6);
    phy_device_driver_s *driver = mac_store.dev_driver->phy_driver;
    if (driver->address_write) {
        driver->address_write(PHY_MAC_48BIT, mac_store.mac48);
    }
    return 0;
}

static int8_t mac48_address_get(const eth_mac_api_t* api, uint8_t *mac48_buf)
{
    if(!mac48_buf || !api || mac_store.mac_api != api ){
        return -1;
    }
    memcpy(&mac_store.mac48, mac_store.dev_driver->phy_driver->PHY_MAC, 6);
    memcpy(mac48_buf, mac_store.mac48, 6);
    return 0;
}
