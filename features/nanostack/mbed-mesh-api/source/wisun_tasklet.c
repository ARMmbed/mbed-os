/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h> //memset
#include "eventOS_event_timer.h"
#include "common_functions.h"
#include "ip6string.h"  //ip6tos
#include "nsdynmemLIB.h"
#include "include/wisun_tasklet.h"
#include "include/mesh_system.h"
#include "ns_event_loop.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "multicast_api.h"
#include "mac_api.h"
#include "sw_mac.h"
#include "ws_management_api.h" //ws_management_node_init
#ifdef MBED_CONF_MBED_MESH_API_CERTIFICATE_HEADER
#if !defined(MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE) || !defined(MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE) || \
    !defined(MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_KEY)
#error Invalid Wi-SUN certificate configuration
#endif
#include MBED_CONF_MBED_MESH_API_CERTIFICATE_HEADER
#endif

// For tracing we need to define flag, have include and define group
//#define HAVE_DEBUG
#define TRACE_GROUP  "WSND"
#include "ns_trace.h"

// Tasklet timer events
#define TIMER_EVENT_START_BOOTSTRAP   1
#define INVALID_INTERFACE_ID        (-1)
#define INTERFACE_NAME              "WiSunInterface"
/*
 * Mesh tasklet states.
 */
typedef enum {
    TASKLET_STATE_CREATED = 0,
    TASKLET_STATE_INITIALIZED,
    TASKLET_STATE_BOOTSTRAP_STARTED,
    TASKLET_STATE_BOOTSTRAP_FAILED,
    TASKLET_STATE_BOOTSTRAP_READY
} tasklet_state_t;

/*
 * Mesh tasklet data structure.
 */
typedef struct {
    void (*mesh_api_cb)(mesh_connection_status_t nwk_status);
    tasklet_state_t tasklet_state;
    int8_t tasklet;
    net_6lowpan_mode_e operating_mode;
    net_6lowpan_mode_extension_e operating_mode_extension;
    int8_t network_interface_id;
} wisun_tasklet_data_str_t;


/* Tasklet data */
static wisun_tasklet_data_str_t *wisun_tasklet_data_ptr = NULL;
static mac_api_t *mac_api = NULL;
static char *network_name = MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME;
extern fhss_timer_t fhss_functions;

/* private function prototypes */
static void wisun_tasklet_main(arm_event_s *event);
static void wisun_tasklet_network_state_changed(mesh_connection_status_t status);
static void wisun_tasklet_parse_network_event(arm_event_s *event);
static void wisun_tasklet_configure_and_connect_to_network(void);

/*
 * \brief A function which will be eventually called by NanoStack OS when ever the OS has an event to deliver.
 * @param event, describes the sender, receiver and event type.
 *
 * NOTE: Interrupts requested by HW are possible during this function!
 */
static void wisun_tasklet_main(arm_event_s *event)
{
    arm_library_event_type_e event_type;
    event_type = (arm_library_event_type_e) event->event_type;

    switch (event_type) {
        case ARM_LIB_NWK_INTERFACE_EVENT:
            /* This event is delivered every and each time when there is new
             * information of network connectivity.
             */
            wisun_tasklet_parse_network_event(event);
            break;

        case ARM_LIB_TASKLET_INIT_EVENT:
            /* Event with type EV_INIT is an initializer event of NanoStack OS.
             * The event is delivered when the NanoStack OS is running fine.
             * This event should be delivered ONLY ONCE.
             */
            mesh_system_send_connect_event(wisun_tasklet_data_ptr->tasklet);
            break;

        case ARM_LIB_SYSTEM_TIMER_EVENT:
            eventOS_event_timer_cancel(event->event_id, wisun_tasklet_data_ptr->tasklet);

            if (event->event_id == TIMER_EVENT_START_BOOTSTRAP) {
                tr_debug("Restart bootstrap");
                wisun_tasklet_configure_and_connect_to_network();
            }
            break;

        case APPLICATION_EVENT:
            if (event->event_id == APPL_EVENT_CONNECT) {
                wisun_tasklet_configure_and_connect_to_network();
            }
            break;

        default:
            break;
    } // switch(event_type)
}

/**
 * \brief Network state event handler.
 * \param event show network start response or current network state.
 *
 * - ARM_NWK_BOOTSTRAP_READY: Save NVK persistent data to NVM and Net role
 * - ARM_NWK_NWK_SCAN_FAIL: Link Layer Active Scan Fail, Stack is Already at Idle state
 * - ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL: No WS Router at current Channel Stack is Already at Idle state
 * - ARM_NWK_NWK_CONNECTION_DOWN: Connection to Access point is lost wait for Scan Result
 * - ARM_NWK_NWK_PARENT_POLL_FAIL: Host should run net start without any PAN-id filter and all channels
 * - ARM_NWK_AUHTENTICATION_FAIL: Pana Authentication fail, Stack is Already at Idle state
 */
static void wisun_tasklet_parse_network_event(arm_event_s *event)
{
    arm_nwk_interface_status_type_e status = (arm_nwk_interface_status_type_e) event->event_data;
    tr_debug("app_parse_network_event() %d", status);
    switch (status) {
        case ARM_NWK_BOOTSTRAP_READY:
            /* Network is ready and node is connected to Access Point */
            if (wisun_tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_READY) {
                tr_info("Wi-SUN bootstrap ready");
                wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_READY;
                wisun_tasklet_network_state_changed(MESH_CONNECTED);
            }
            break;
        case ARM_NWK_NWK_SCAN_FAIL:
            /* Link Layer Active Scan Fail, Stack is Already at Idle state */
            tr_debug("Link Layer Scan Fail: No Beacons");
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
            break;
        case ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL:
            /* No WS Router at current Channel Stack is Already at Idle state */
            tr_debug("WS Scan/ GP REG fail");
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
            break;
        case ARM_NWK_NWK_CONNECTION_DOWN:
            /* Connection to Access point is lost wait for Scan Result */
            tr_debug("WS/RPL scan new network");
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
            break;
        case ARM_NWK_NWK_PARENT_POLL_FAIL:
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
            break;
        case ARM_NWK_AUHTENTICATION_FAIL:
            tr_debug("Network authentication fail");
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
            break;
        default:
            tr_warn("Unknown event %d", status);
            break;
    }

    if (wisun_tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_READY &&
            wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID) {
        // Set 5s timer for new network scan
        eventOS_event_timer_request(TIMER_EVENT_START_BOOTSTRAP,
                                    ARM_LIB_SYSTEM_TIMER_EVENT,
                                    wisun_tasklet_data_ptr->tasklet,
                                    5000);

    }
}

/*
 * \brief Configure and establish network connection
 *
 */
static void wisun_tasklet_configure_and_connect_to_network(void)
{
    int8_t status;
    fhss_timer_t *fhss_timer_ptr = &fhss_functions;

    wisun_tasklet_data_ptr->operating_mode = NET_6LOWPAN_ROUTER;
    wisun_tasklet_data_ptr->operating_mode_extension = NET_6LOWPAN_WS;

    arm_nwk_interface_configure_6lowpan_bootstrap_set(
        wisun_tasklet_data_ptr->network_interface_id,
        wisun_tasklet_data_ptr->operating_mode,
        wisun_tasklet_data_ptr->operating_mode_extension);

    ws_management_node_init(wisun_tasklet_data_ptr->network_interface_id,
                            MBED_CONF_MBED_MESH_API_WISUN_REGULATORY_DOMAIN,
                            network_name,
                            fhss_timer_ptr);

#if defined(MBED_CONF_MBED_MESH_API_CERTIFICATE_HEADER)
    arm_certificate_chain_entry_s chain_info;
    memset(&chain_info, 0, sizeof(arm_certificate_chain_entry_s));
    chain_info.cert_chain[0] = (const uint8_t *) MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE;
    chain_info.cert_len[0] = strlen((const char *) MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE) + 1;
    chain_info.cert_chain[1] = (const uint8_t *) MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE;
    chain_info.cert_len[1] = strlen((const char *) MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE) + 1;
    chain_info.key_chain[1] = (const uint8_t *) MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_KEY;
    chain_info.chain_length = 2;
    arm_network_certificate_chain_set((const arm_certificate_chain_entry_s *) &chain_info);
#endif

    status = arm_nwk_interface_up(wisun_tasklet_data_ptr->network_interface_id);
    if (status >= 0) {
        wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_STARTED;
        tr_info("Start Wi-SUN Bootstrap");
        wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_STARTED);
    } else {
        wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
        tr_err("Bootstrap start failed, %d", status);
        wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_START_FAILED);
    }
}

/*
 * Inform application about network state change
 */
static void wisun_tasklet_network_state_changed(mesh_connection_status_t status)
{
    if (wisun_tasklet_data_ptr->mesh_api_cb) {
        (wisun_tasklet_data_ptr->mesh_api_cb)(status);
    }
}

/* Public functions */
int8_t wisun_tasklet_get_router_ip_address(char *address, int8_t len)
{
    network_layer_address_s nd_address;

    if ((len >= 40) && (0 == arm_nwk_nd_address_read(wisun_tasklet_data_ptr->network_interface_id, &nd_address))) {
        ip6tos(nd_address.border_router, address);
        tr_debug("Router IP address: %s", address);
        return 0;
    } else {
        return -1;
    }
}

int8_t wisun_tasklet_connect(mesh_interface_cb callback, int8_t nwk_interface_id)
{
    int8_t re_connecting = true;
    int8_t tasklet_id = wisun_tasklet_data_ptr->tasklet;

    if (wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID) {
        return -3;  // already connected to network
    }

    if (wisun_tasklet_data_ptr->tasklet_state == TASKLET_STATE_CREATED) {
        re_connecting = false;
    }

    memset(wisun_tasklet_data_ptr, 0, sizeof(wisun_tasklet_data_str_t));
    wisun_tasklet_data_ptr->mesh_api_cb = callback;
    wisun_tasklet_data_ptr->network_interface_id = nwk_interface_id;
    wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_INITIALIZED;

    if (re_connecting == false) {
        wisun_tasklet_data_ptr->tasklet = eventOS_event_handler_create(&wisun_tasklet_main,
                                                                       ARM_LIB_TASKLET_INIT_EVENT);
        if (wisun_tasklet_data_ptr->tasklet < 0) {
            // -1 handler already used by other tasklet
            // -2 memory allocation failure
            return wisun_tasklet_data_ptr->tasklet;
        }
    } else {
        wisun_tasklet_data_ptr->tasklet = tasklet_id;
        mesh_system_send_connect_event(wisun_tasklet_data_ptr->tasklet);
    }

    return wisun_tasklet_data_ptr->tasklet;
}

int8_t wisun_tasklet_disconnect(bool send_cb)
{
    int8_t status = -1;
    if (wisun_tasklet_data_ptr != NULL) {
        if (wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID) {
            status = arm_nwk_interface_down(wisun_tasklet_data_ptr->network_interface_id);
            wisun_tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
            if (send_cb == true) {
                wisun_tasklet_network_state_changed(MESH_DISCONNECTED);
            }
        }
        wisun_tasklet_data_ptr->mesh_api_cb = NULL;
    }
    return status;
}

void wisun_tasklet_init(void)
{
    if (wisun_tasklet_data_ptr == NULL) {
        wisun_tasklet_data_ptr = ns_dyn_mem_alloc(sizeof(wisun_tasklet_data_str_t));
        memset(wisun_tasklet_data_ptr, 0, sizeof(wisun_tasklet_data_str_t));
        wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_CREATED;
        wisun_tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
    }
}

int8_t wisun_tasklet_network_init(int8_t device_id)
{
    // TODO, read interface name from configuration
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 4;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    if (!mac_api) {
        mac_api = ns_sw_mac_create(device_id, &storage_sizes);
    }
    return arm_nwk_interface_lowpan_init(mac_api, INTERFACE_NAME);
}
