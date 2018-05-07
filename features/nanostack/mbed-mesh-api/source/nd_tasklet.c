/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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
#include "net_interface.h"
#include "ip6string.h"  //ip6tos
#include "nsdynmemLIB.h"
#include "include/nd_tasklet.h"
#include "include/mesh_system.h"
#include "ns_event_loop.h"
#include "multicast_api.h"

// For tracing we need to define flag, have include and define group
#define HAVE_DEBUG 1
#include "ns_trace.h"
#define TRACE_GROUP  "m6LND"

#include "mac_api.h"
#include "sw_mac.h"

#define INTERFACE_NAME   "6L-ND"

// Tasklet timer events
#define TIMER_EVENT_START_BOOTSTRAP   1

#define INVALID_INTERFACE_ID        (-1)

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

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
    channel_list_s channel_list;
    tasklet_state_t tasklet_state;
    net_6lowpan_mode_e mode;
    net_6lowpan_link_layer_sec_mode_e sec_mode;
    net_link_layer_psk_security_info_s psk_sec_info;
    int8_t node_main_tasklet_id;
    int8_t network_interface_id;
    int8_t tasklet;
} tasklet_data_str_t;

/* Tasklet data */
static tasklet_data_str_t *tasklet_data_ptr = NULL;
static mac_api_t *mac_api = NULL;

/* private function prototypes */
void nd_tasklet_main(arm_event_s *event);
void nd_tasklet_network_state_changed(mesh_connection_status_t status);
void nd_tasklet_parse_network_event(arm_event_s *event);
void nd_tasklet_configure_and_connect_to_network(void);
#define TRACE_ND_TASKLET
#ifndef TRACE_ND_TASKLET
#define nd_tasklet_trace_bootstrap_info() ((void) 0)
#else
void nd_tasklet_trace_bootstrap_info(void);
#endif

static void initialize_channel_list(void)
{
    uint32_t channel = MBED_CONF_MBED_MESH_API_6LOWPAN_ND_CHANNEL;

    const int_fast8_t word_index = channel / 32;
    const int_fast8_t bit_index = channel % 32;

    memset(&tasklet_data_ptr->channel_list, 0, sizeof(tasklet_data_ptr->channel_list));

    tasklet_data_ptr->channel_list.channel_page = (channel_page_e)MBED_CONF_MBED_MESH_API_6LOWPAN_ND_CHANNEL_PAGE;
    tasklet_data_ptr->channel_list.channel_mask[0] = MBED_CONF_MBED_MESH_API_6LOWPAN_ND_CHANNEL_MASK;

    if (channel > 0) {
        memset(&tasklet_data_ptr->channel_list.channel_mask, 0, sizeof(tasklet_data_ptr->channel_list.channel_mask));
        tasklet_data_ptr->channel_list.channel_mask[word_index] |= ((uint32_t) 1 << bit_index);
    }

    arm_nwk_set_channel_list(tasklet_data_ptr->network_interface_id, &tasklet_data_ptr->channel_list);

    tr_debug("Channel: %ld", channel);
    tr_debug("Channel page: %d", tasklet_data_ptr->channel_list.channel_page);
    tr_debug("Channel mask: 0x%.8lx", tasklet_data_ptr->channel_list.channel_mask[word_index]);
}

/*
 * \brief A function which will be eventually called by NanoStack OS when ever the OS has an event to deliver.
 * @param event, describes the sender, receiver and event type.
 *
 * NOTE: Interrupts requested by HW are possible during this function!
 */
void nd_tasklet_main(arm_event_s *event)
{
    arm_library_event_type_e event_type;
    event_type = (arm_library_event_type_e) event->event_type;

    switch (event_type) {
        case ARM_LIB_NWK_INTERFACE_EVENT:
            /* This event is delivered every and each time when there is new
             * information of network connectivity.
             */
            nd_tasklet_parse_network_event(event);
            break;

        case ARM_LIB_TASKLET_INIT_EVENT:
            /* Event with type EV_INIT is an initializer event of NanoStack OS.
             * The event is delivered when the NanoStack OS is running fine.
             * This event should be delivered ONLY ONCE.
             */
            tasklet_data_ptr->node_main_tasklet_id = event->receiver;
            mesh_system_send_connect_event(tasklet_data_ptr->tasklet);
            break;

        case ARM_LIB_SYSTEM_TIMER_EVENT:
            eventOS_event_timer_cancel(event->event_id,
                                       tasklet_data_ptr->node_main_tasklet_id);

            if (event->event_id == TIMER_EVENT_START_BOOTSTRAP) {
                tr_debug("Restart bootstrap");
                nd_tasklet_configure_and_connect_to_network();
            }
            break;

        case APPLICATION_EVENT:
            if (event->event_id == APPL_EVENT_CONNECT) {
                nd_tasklet_configure_and_connect_to_network();
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
 * - ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL: No ND Router at current Channel Stack is Already at Idle state
 * - ARM_NWK_NWK_CONNECTION_DOWN: Connection to Access point is lost wait for Scan Result
 * - ARM_NWK_NWK_PARENT_POLL_FAIL: Host should run net start without any PAN-id filter and all channels
 * - ARM_NWK_AUHTENTICATION_FAIL: Pana Authentication fail, Stack is Already at Idle state
 */
void nd_tasklet_parse_network_event(arm_event_s *event)
{
    arm_nwk_interface_status_type_e status = (arm_nwk_interface_status_type_e) event->event_data;
    tr_debug("app_parse_network_event() %d", status);
    switch (status) {
        case ARM_NWK_BOOTSTRAP_READY:
            /* Network is ready and node is connected to Access Point */
            if (tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_READY) {
                tr_info("6LoWPAN ND bootstrap ready");
                tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_READY;
                nd_tasklet_trace_bootstrap_info();
                nd_tasklet_network_state_changed(MESH_CONNECTED);
            }
            break;
        case ARM_NWK_NWK_SCAN_FAIL:
            /* Link Layer Active Scan Fail, Stack is Already at Idle state */
            tr_debug("Link Layer Scan Fail: No Beacons");
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            nd_tasklet_network_state_changed(MESH_DISCONNECTED);
            break;
        case ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL:
            /* No ND Router at current Channel Stack is Already at Idle state */
            tr_debug("ND Scan/ GP REG fail");
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            nd_tasklet_network_state_changed(MESH_DISCONNECTED);
            break;
        case ARM_NWK_NWK_CONNECTION_DOWN:
            /* Connection to Access point is lost wait for Scan Result */
            tr_debug("ND/RPL scan new network");
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            nd_tasklet_network_state_changed(MESH_DISCONNECTED);
            break;
        case ARM_NWK_NWK_PARENT_POLL_FAIL:
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            nd_tasklet_network_state_changed(MESH_DISCONNECTED);
            break;
        case ARM_NWK_AUHTENTICATION_FAIL:
            tr_debug("Network authentication fail");
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            nd_tasklet_network_state_changed(MESH_DISCONNECTED);
            break;
        default:
            tr_warn("Unknown event %d", status);
            break;
    }

    if (tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_READY) {
        // Set 5s timer for new network scan
        eventOS_event_timer_request(TIMER_EVENT_START_BOOTSTRAP,
                                    ARM_LIB_SYSTEM_TIMER_EVENT,
                                    tasklet_data_ptr->node_main_tasklet_id,
                                    5000);
    }
}

/*
 * \brief Configure and establish network connection
 *
 */
void nd_tasklet_configure_and_connect_to_network(void)
{
    int8_t status;
    char *sec_mode;

    // configure bootstrap
    arm_nwk_interface_configure_6lowpan_bootstrap_set(
        tasklet_data_ptr->network_interface_id, tasklet_data_ptr->mode,
        NET_6LOWPAN_ND_WITH_MLE);

    sec_mode = STR(MBED_CONF_MBED_MESH_API_6LOWPAN_ND_SECURITY_MODE);

    if (strcmp(sec_mode, "PSK") == 0) {
        tr_debug("Using PSK security mode.");
        tasklet_data_ptr->sec_mode = NET_SEC_MODE_PSK_LINK_SECURITY;
        tasklet_data_ptr->psk_sec_info.key_id = MBED_CONF_MBED_MESH_API_6LOWPAN_ND_PSK_KEY_ID;
        memcpy(tasklet_data_ptr->psk_sec_info.security_key, (const uint8_t[16])MBED_CONF_MBED_MESH_API_6LOWPAN_ND_PSK_KEY, 16);
    } else {
        tr_debug("Link-layer security NOT enabled.");
        tasklet_data_ptr->sec_mode = NET_SEC_MODE_NO_LINK_SECURITY;
    }

    // configure link layer security
    arm_nwk_link_layer_security_mode(
        tasklet_data_ptr->network_interface_id,
        tasklet_data_ptr->sec_mode,
        MBED_CONF_MBED_MESH_API_6LOWPAN_ND_SEC_LEVEL,
        &tasklet_data_ptr->psk_sec_info);

    // configure scan parameters
    arm_nwk_6lowpan_link_scan_parameter_set(tasklet_data_ptr->network_interface_id, 5);

    // configure scan channels
    initialize_channel_list();

    // Configure scan options (NULL disables filter)
    arm_nwk_6lowpan_link_nwk_id_filter_for_nwk_scan(
        tasklet_data_ptr->network_interface_id, NULL);

    arm_nwk_6lowpan_link_panid_filter_for_nwk_scan(
         tasklet_data_ptr->network_interface_id,
         MBED_CONF_MBED_MESH_API_6LOWPAN_ND_PANID_FILTER);

    // Enable MPL by default
    const uint8_t all_mpl_forwarders[16] = {0xff, 0x03, [15]=0xfc};
    multicast_mpl_domain_subscribe(tasklet_data_ptr->network_interface_id,
                                      all_mpl_forwarders,
                                      MULTICAST_MPL_SEED_ID_DEFAULT,
                                      NULL);

    status = arm_nwk_interface_up(tasklet_data_ptr->network_interface_id);
    if (status >= 0) {
        tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_STARTED;
        tr_info("Start 6LoWPAN ND Bootstrap");
        nd_tasklet_network_state_changed(MESH_BOOTSTRAP_STARTED);
    } else {
        tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
        tr_err("Bootstrap start failed, %d", status);
        nd_tasklet_network_state_changed(MESH_BOOTSTRAP_START_FAILED);
    }
}

/*
 * Inform application about network state change
 */
void nd_tasklet_network_state_changed(mesh_connection_status_t status)
{
    if (tasklet_data_ptr->mesh_api_cb) {
        (tasklet_data_ptr->mesh_api_cb)(status);
    }
}

/*
 * Trace bootstrap information.
 */
#ifdef TRACE_ND_TASKLET
void nd_tasklet_trace_bootstrap_info()
{
    network_layer_address_s app_nd_address_info;
    link_layer_address_s app_link_address_info;
    uint8_t temp_ipv6[16];
    if (arm_nwk_nd_address_read(tasklet_data_ptr->network_interface_id,
                                &app_nd_address_info) != 0) {
        tr_error("ND Address read fail");
    } else {
        tr_debug("ND Access Point: %s", trace_ipv6(app_nd_address_info.border_router));
        tr_debug("ND Prefix 64: %s", trace_array(app_nd_address_info.prefix, 8));

        if (arm_net_address_get(tasklet_data_ptr->network_interface_id,
                                ADDR_IPV6_GP, temp_ipv6) == 0) {
            tr_debug("GP IPv6: %s", trace_ipv6(temp_ipv6));
        }
    }

    if (arm_nwk_mac_address_read(tasklet_data_ptr->network_interface_id,
                                 &app_link_address_info) != 0) {
        tr_error("MAC Address read fail\n");
    } else {
        uint8_t temp[2];
        common_write_16_bit(app_link_address_info.mac_short,temp);
        tr_debug("MAC 16-bit: %s", trace_array(temp, 2));
        common_write_16_bit(app_link_address_info.PANId, temp);
        tr_debug("PAN ID: %s", trace_array(temp, 2));
        tr_debug("MAC 64-bit: %s", trace_array(app_link_address_info.mac_long, 8));
        tr_debug("IID (Based on MAC 64-bit address): %s", trace_array(app_link_address_info.iid_eui64, 8));
    }

    tr_debug("Channel: %d", arm_net_get_current_channel(tasklet_data_ptr->network_interface_id));
}
#endif /* #define TRACE_ND_TASKLET */

/* Public functions */
int8_t nd_tasklet_get_ip_address(char *address, int8_t len)
{
    uint8_t binary_ipv6[16];

    if ((len >= 40) && (0 == arm_net_address_get(
                            tasklet_data_ptr->network_interface_id, ADDR_IPV6_GP, binary_ipv6))) {
        ip6tos(binary_ipv6, address);
        //tr_debug("IP address: %s", address);
        return 0;
    } else {
        return -1;
    }
}

int8_t nd_tasklet_get_router_ip_address(char *address, int8_t len)
{
    network_layer_address_s nd_address;

    if ((len >= 40) && (0 == arm_nwk_nd_address_read(
                            tasklet_data_ptr->network_interface_id, &nd_address))) {
        ip6tos(nd_address.border_router, address);
        //tr_debug("Router IP address: %s", address);
        return 0;
    } else {
        return -1;
    }
}

int8_t nd_tasklet_connect(mesh_interface_cb callback, int8_t nwk_interface_id)
{
    int8_t re_connecting = true;
    int8_t tasklet_id = tasklet_data_ptr->tasklet;

    if (tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID) {
        return -3;  // already connected to network
    }

    if (tasklet_data_ptr->tasklet_state == TASKLET_STATE_CREATED) {
        re_connecting = false;
    }

    memset(tasklet_data_ptr, 0, sizeof(tasklet_data_str_t));
    tasklet_data_ptr->mesh_api_cb = callback;
    tasklet_data_ptr->network_interface_id = nwk_interface_id;
    tasklet_data_ptr->tasklet_state = TASKLET_STATE_INITIALIZED;

    tasklet_data_ptr->mode = MBED_CONF_MBED_MESH_API_6LOWPAN_ND_DEVICE_TYPE;
    tasklet_data_ptr->sec_mode = NET_SEC_MODE_NO_LINK_SECURITY;
    //tasklet_data_ptr->psk_sec_info.key_id = 0;

    if (re_connecting == false) {
        tasklet_data_ptr->tasklet = eventOS_event_handler_create(&nd_tasklet_main,
                ARM_LIB_TASKLET_INIT_EVENT);
        if (tasklet_data_ptr->tasklet < 0) {
            // -1 handler already used by other tasklet
            // -2 memory allocation failure
            return tasklet_data_ptr->tasklet;
        }
        ns_event_loop_thread_start();
    } else {
        tasklet_data_ptr->tasklet = tasklet_id;
        mesh_system_send_connect_event(tasklet_data_ptr->tasklet);
    }

    return tasklet_data_ptr->tasklet;
}

int8_t nd_tasklet_disconnect(bool send_cb)
{
    int8_t status = -1;
    if (tasklet_data_ptr != NULL) {
        if (tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID) {
            status = arm_nwk_interface_down(tasklet_data_ptr->network_interface_id);
            tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
            if (send_cb == true) {
                nd_tasklet_network_state_changed(MESH_DISCONNECTED);
            }
        }
        tasklet_data_ptr->mesh_api_cb = NULL;
    }
    return status;
}

void nd_tasklet_init(void)
{
    if (tasklet_data_ptr == NULL) {
        // memory allocation will not fail as memory was just initialized
        tasklet_data_ptr = ns_dyn_mem_alloc(sizeof(tasklet_data_str_t));
        tasklet_data_ptr->tasklet_state = TASKLET_STATE_CREATED;
        tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
    }
}

int8_t nd_tasklet_network_init(int8_t device_id)
{
    // TODO, read interface name from configuration
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = 32;
    storage_sizes.key_description_table_size = 3;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    if (!mac_api) {
        mac_api = ns_sw_mac_create(device_id, &storage_sizes);
    }
    return arm_nwk_interface_lowpan_init(mac_api, INTERFACE_NAME);
}

