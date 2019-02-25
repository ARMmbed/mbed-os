/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "serial_mac_api.h"
#include "eventOS_event.h"
#include "nsdynmemLIB.h"
#include "common_functions.h"
#include "ns_trace.h"
#include "MAC/rf_driver_storage.h"

#define TRACE_GROUP "seMa"

#define SERIAL_MAC_DATA_FLOW_TYPE 0
#define SERIAL_MAC_VIRTUAL_FLOW_TYPE 1

typedef struct serial_mac_buf {
    struct serial_mac_buf *next;
    data_protocol_e type;
    uint16_t length;
    uint8_t buf[];
} serial_mac_buf_t;


typedef struct serial_mac_internal_s {
    serial_mac_api_t *mac_api;
    arm_device_driver_list_s *dev_driver;
    arm_device_driver_list_s *virtual_driver;
    serial_mac_buf_t *active_tx_buf;
    serial_mac_buf_t *tx_queue;
    int8_t tasklet_id;
    //linked list link
} serial_mac_internal_t;

typedef struct serial_data_ind_s {
    uint16_t msduLength;
    uint8_t *msdu;
} serial_data_ind_t;

static serial_mac_internal_t serial_mac_store; //Hack only at this point, later put into linked list

#define SERIAL_INIT_EVENT 100
#define SERIAL_DATA_IND_EVENT 101
#define SERIAL_DATA_CNF_EVENT 102

static int8_t serial_mac_api_init(serial_mac_api_t *api, data_indication *ind_cb);
static int8_t serial_mac_virtual_data_request(const virtual_data_req_t *data, int8_t driver_id);
static int8_t serial_mac_data_request(const serial_mac_api_t *api, const uint8_t *data_ptr, uint16_t data_legth);

static int8_t generic_data_request(serial_mac_internal_t *mac, const virtual_data_req_t *data, uint8_t data_flow_type);

static int8_t serial_mac_net_phy_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id);
static int8_t serial_mac_net_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);
static void serial_mac_tasklet(arm_event_s *event);

static serial_mac_buf_t *serial_mac_buffer_get(const virtual_data_req_t *b, uint8_t data_type);

static int8_t serial_mac_data_request(const serial_mac_api_t *api, const uint8_t *data_ptr, uint16_t data_length)
{

    if (!serial_mac_store.mac_api || serial_mac_store.mac_api != api) {
        return -1;
    }

    if (!data_ptr || !data_length) {
        return -2;
    }

    virtual_data_req_t data;
    data.parameter_length = 0;
    data.parameters = NULL;
    data.msdu = data_ptr;
    data.msduLength = data_length;

    return generic_data_request(&serial_mac_store, &data, SERIAL_MAC_DATA_FLOW_TYPE);
}


static int8_t serial_mac_virtual_init(const serial_mac_api_t *api, int8_t driver_id)
{
    arm_device_driver_list_s *virtual_driver = arm_net_phy_driver_pointer(driver_id);

    if (!serial_mac_store.mac_api || serial_mac_store.mac_api != api || !virtual_driver) {
        return -1;
    }

    if (!virtual_driver->phy_driver->arm_net_virtual_rx_cb) {
        return -1;
    }

    serial_mac_store.virtual_driver = virtual_driver;
    virtual_driver->phy_driver->arm_net_virtual_tx_cb = &serial_mac_virtual_data_request;
    return 0;

}

serial_mac_api_t *serial_mac_create(int8_t serial_driver_id)
{
    //TODO: Refactor this away, Drivers should be stored in MAC layer in future
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(serial_driver_id);
    if (!driver || !driver->phy_driver) {
        return NULL;
    }

    serial_mac_api_t *this = ns_dyn_mem_alloc(sizeof(serial_mac_api_t));
    if (!this) {
        return NULL;
    }
    memset(this, 0, sizeof(serial_mac_api_t));
    this->mac_initialize = &serial_mac_api_init;
    this->virtual_initilize = &serial_mac_virtual_init;
    this->data_request_cb = &serial_mac_data_request;
    serial_mac_store.active_tx_buf = NULL;
    serial_mac_store.mac_api = this;
    serial_mac_store.dev_driver = driver;
    serial_mac_store.tasklet_id = eventOS_event_handler_create(&serial_mac_tasklet, SERIAL_INIT_EVENT);
    arm_net_phy_init(driver->phy_driver, &serial_mac_net_phy_rx, &serial_mac_net_phy_tx_done);
    return this;
}


static int8_t serial_mac_virtual_data_request(const virtual_data_req_t *data, int8_t driver_id)
{

    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);
    if (!driver || serial_mac_store.virtual_driver != driver) {
        return -1;
    }

    return generic_data_request(&serial_mac_store, data, SERIAL_MAC_VIRTUAL_FLOW_TYPE);
}

static serial_mac_buf_t *serial_mac_buffer_get(const virtual_data_req_t *b, uint8_t data_type)
{
    if (!b || !b->msdu || !b->msduLength) {
        return NULL;
    }

    if (b->parameter_length && !b->parameters) {
        return NULL;
    }

    uint16_t length = b->msduLength + b->parameter_length + 1; //Allocate 1 byte from Serial mac

    serial_mac_buf_t *buffer = ns_dyn_mem_temporary_alloc(sizeof(serial_mac_buf_t) + length);

    if (buffer) {
        uint8_t *ptr = &(buffer)->buf[0];
        memset(buffer, 0, sizeof(serial_mac_buf_t));
        *ptr++ = data_type;
        if (b->parameter_length) {
            memcpy(ptr, b->parameters, b->parameter_length);
            ptr += b->parameter_length;
        }
        memcpy(ptr, b->msdu, b->msduLength);
        buffer->length = length;
    }
    return buffer;
}

static void serial_mac_buffer_queue_free(serial_mac_buf_t *buf)
{
    serial_mac_buf_t *cur;
    while (buf) {
        cur = buf;
        buf = cur->next;
        ns_dyn_mem_free(cur);
    }
}

static serial_mac_buf_t *arm_driver_queue_read(serial_mac_internal_t *mac)
{
    serial_mac_buf_t *b = NULL;
    if (mac->tx_queue) {
        b = mac->tx_queue;
        mac->tx_queue = b->next;
        b->next = NULL;
    }
    return b;
}

static int serial_mac_tx_buf_from_queue(serial_mac_internal_t *mac)
{
    if (mac->active_tx_buf) {
        return 0;
    }
    serial_mac_buf_t *b = arm_driver_queue_read(mac);

    if (!b) {
        return 0;
    }

    mac->active_tx_buf = b;
    if (mac->dev_driver->phy_driver->tx(&(b)->buf[0], b->length, 1, b->type) == 0) {
        return 0;
    }
    //This should be tx busy/failed event and when needed error status should be included in an event
    arm_event_s event = {
        .receiver = serial_mac_store.tasklet_id,
        .sender = 0,
        .event_id = 0,
        .data_ptr = NULL,
        .event_type = SERIAL_DATA_CNF_EVENT,
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    return eventOS_event_send(&event);
}

static void serial_mac_queue_push(serial_mac_internal_t *mac, serial_mac_buf_t *b)
{
    if (mac->tx_queue) {
        serial_mac_buf_t *temp_buf = mac->tx_queue;
        while (temp_buf->next) {
            temp_buf = temp_buf->next;
        }
        temp_buf->next = b;
    } else {
        mac->tx_queue = b;
    }

}

static void serial_mac_tasklet(arm_event_s *event)
{
    uint8_t event_type = event->event_type;

    switch (event_type) {
        case SERIAL_DATA_IND_EVENT: {

            if (!event->data_ptr) {
                return;
            }

            serial_data_ind_t *data_ind = (serial_data_ind_t *)event->data_ptr;
            uint8_t *ptr = data_ind->msdu;
            uint8_t data_type = *ptr++;
            if (data_type == SERIAL_MAC_DATA_FLOW_TYPE && serial_mac_store.mac_api->data_ind_cb) {
                serial_mac_store.mac_api->data_ind_cb(serial_mac_store.mac_api, ptr, data_ind->msduLength - 1);

            } else if (data_type == SERIAL_MAC_VIRTUAL_FLOW_TYPE && serial_mac_store.virtual_driver) {
                arm_device_driver_list_s *driver = serial_mac_store.virtual_driver;
                if (driver->phy_driver && driver->phy_driver->arm_net_virtual_rx_cb) {
                    driver->phy_driver->arm_net_virtual_rx_cb(ptr, data_ind->msduLength - 1, driver->id);
                }

            }
            ns_dyn_mem_free(((serial_data_ind_t *)event->data_ptr)->msdu);
            ns_dyn_mem_free(event->data_ptr);
            break;
        }
        case SERIAL_DATA_CNF_EVENT: {

            if (serial_mac_store.active_tx_buf) {
                serial_mac_buffer_queue_free(serial_mac_store.active_tx_buf);
                serial_mac_store.active_tx_buf = NULL;
                serial_mac_tx_buf_from_queue(&serial_mac_store);
            }
            break;
        }
        case SERIAL_INIT_EVENT:
        default: {
            break;
        }
    }
}

static int8_t serial_mac_api_init(serial_mac_api_t *api, data_indication *ind_cb)
{
    if (serial_mac_store.mac_api != api) {
        return -1;
    }
    serial_mac_store.mac_api->data_ind_cb = ind_cb;

    return 0;
}


static int8_t generic_data_request(serial_mac_internal_t *mac, const virtual_data_req_t *data, uint8_t data_flow_type)
{
    serial_mac_buf_t *buf = serial_mac_buffer_get(data, data_flow_type);
    if (!buf) {
        return -1;
    }

    serial_mac_queue_push(mac, buf);

    serial_mac_tx_buf_from_queue(mac);

    return 0;

}

static int8_t serial_mac_net_phy_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);

    if (!driver || driver != serial_mac_store.dev_driver || !data_ptr) {
        return -1;
    }

    (void)link_quality;
    (void) dbm;

    serial_data_ind_t *data_ind = ns_dyn_mem_temporary_alloc(sizeof(serial_data_ind_t));
    if (!data_ind) {
        return -1;
    }
    data_ind->msdu = ns_dyn_mem_temporary_alloc(data_len);
    if (!data_ind->msdu) {
        ns_dyn_mem_free(data_ind);
        return -1;
    }
    memcpy(data_ind->msdu, data_ptr, data_len);
    data_ind->msduLength = data_len;

    arm_event_s event = {
        .receiver = serial_mac_store.tasklet_id,
        .sender = 0,
        .event_id = 0,
        .data_ptr = data_ind,
        .event_type = SERIAL_DATA_IND_EVENT,
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    return eventOS_event_send(&event);
}

static int8_t serial_mac_net_phy_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    (void)tx_handle;
    (void)cca_retry;
    (void)tx_retry;
    (void)status;
    arm_device_driver_list_s *driver = arm_net_phy_driver_pointer(driver_id);
    if (!driver || driver != serial_mac_store.dev_driver || !serial_mac_store.active_tx_buf) {
        return -1;
    }
    arm_event_s event = {
        .receiver = serial_mac_store.tasklet_id,
        .sender = 0,
        .event_id = 0,
        .data_ptr = NULL,
        .event_type = SERIAL_DATA_CNF_EVENT,
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    return eventOS_event_send(&event);
}

