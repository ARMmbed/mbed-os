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
#include "include/mesh_system.h"
#include "ns_event_loop.h"
#include "mesh_interface_types.h"
#include "eventOS_event.h"
#include "enet_tasklet.h"

// For tracing we need to define flag, have include and define group
#include "ns_trace.h"
#define TRACE_GROUP  "IPV6"

#include "ethernet_mac_api.h"

#define INTERFACE_NAME   "eth0"

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
    tasklet_state_t tasklet_state;
    mesh_connection_status_t connection_status;
    timeout_t *poll_network_status_timeout;
    int8_t node_main_tasklet_id;
    int8_t network_interface_id;
    int8_t tasklet;
    uint8_t ip[16];
} tasklet_data_str_t;

/* Tasklet data */
static tasklet_data_str_t *tasklet_data_ptr = NULL;
static eth_mac_api_t *eth_mac_api = NULL;
typedef void (*mesh_interface_cb)(mesh_connection_status_t mesh_status);


/* private function prototypes */
static void enet_tasklet_main(arm_event_s *event);
static void enet_tasklet_network_state_changed(mesh_connection_status_t status);
static void enet_tasklet_parse_network_event(arm_event_s *event);
static void enet_tasklet_configure_and_connect_to_network(void);
static void enet_tasklet_poll_network_status(void *param);
static void enet_tasklet_generate_event(uint8_t link_status, mesh_connection_status_t mesh_status);
/*
 * \brief A function which will be eventually called by NanoStack OS when ever the OS has an event to deliver.
 * @param event, describes the sender, receiver and event type.
 *
 * NOTE: Interrupts requested by HW are possible during this function!
 */
void enet_tasklet_main(arm_event_s *event)
{
    arm_library_event_type_e event_type;
    event_type = (arm_library_event_type_e) event->event_type;

    switch (event_type) {
        case ARM_LIB_NWK_INTERFACE_EVENT:
            /* This event is delivered every and each time when there is new
             * information of network connectivity.
             */
            enet_tasklet_parse_network_event(event);
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
                enet_tasklet_configure_and_connect_to_network();
            }
            break;

        case APPLICATION_EVENT:
            if (event->event_id == APPL_EVENT_CONNECT) {
                enet_tasklet_configure_and_connect_to_network();
            } else if (event->event_id == APPL_BACKHAUL_LINK_UP
                       && tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_STARTED) {
                // Ethernet cable has been plugged in
                arm_nwk_interface_configure_ipv6_bootstrap_set(
                    tasklet_data_ptr->network_interface_id, NET_IPV6_BOOTSTRAP_AUTONOMOUS, NULL);
                enet_tasklet_configure_and_connect_to_network();

                if (tasklet_data_ptr->poll_network_status_timeout != NULL) {
                    // Restart poll timer
                    eventOS_timeout_cancel(tasklet_data_ptr->poll_network_status_timeout);
                }
                tasklet_data_ptr->poll_network_status_timeout =
                    eventOS_timeout_every_ms(enet_tasklet_poll_network_status, 2000, NULL);
            } else if (event->event_id == APPL_BACKHAUL_LINK_DOWN) {
                // Ethernet cable has been removed
                arm_nwk_interface_down(tasklet_data_ptr->network_interface_id);
                eventOS_timeout_cancel(tasklet_data_ptr->poll_network_status_timeout);
                tasklet_data_ptr->poll_network_status_timeout = NULL;
                memset(tasklet_data_ptr->ip, 0x0, 16);
                enet_tasklet_network_state_changed(MESH_BOOTSTRAP_STARTED);
            } else if (event->event_id == APPL_BACKHAUL_INTERFACE_PHY_DOWN) {
                // disconnect called
                if (tasklet_data_ptr != NULL) {
                    if (tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID) {
                        if (tasklet_data_ptr->connection_status != MESH_BOOTSTRAP_STARTED) {
                            arm_nwk_interface_down(tasklet_data_ptr->network_interface_id);
                        }
                        tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
                        enet_tasklet_network_state_changed(MESH_DISCONNECTED);
                    }
                    tasklet_data_ptr->mesh_api_cb = NULL;
                    eventOS_timeout_cancel(tasklet_data_ptr->poll_network_status_timeout);
                }
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
void enet_tasklet_parse_network_event(arm_event_s *event)
{
    arm_nwk_interface_status_type_e status = (arm_nwk_interface_status_type_e) event->event_data;
    tr_debug("app_parse_network_event() %d", status);
    switch (status) {
        case ARM_NWK_BOOTSTRAP_READY:
            /* Network is ready and node is connected to Access Point */
            if (tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_READY) {
                tr_info("IPv6 bootstrap ready");
                tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_READY;
                enet_tasklet_poll_network_status(NULL);
            }
            break;
        case ARM_NWK_IP_ADDRESS_ALLOCATION_FAIL:
            /* No ND Router at current Channel Stack is Already at Idle state */
            tr_info("Bootstrap fail");
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            enet_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
            break;
        case ARM_NWK_NWK_CONNECTION_DOWN:
            /* Connection to Access point is lost wait for Scan Result */
            tr_info("Connection lost");
            tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
            enet_tasklet_network_state_changed(MESH_BOOTSTRAP_FAILED);
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

static void enet_tasklet_poll_network_status(void *param)
{
    /* Check if we do have an IP */
    uint8_t temp_ipv6[16];
    if (arm_net_address_get(tasklet_data_ptr->network_interface_id, ADDR_IPV6_GP, temp_ipv6) == 0) {
        /* Check if this is link local address or not */
        if (memcmp(temp_ipv6, tasklet_data_ptr->ip, 16) == 0) {
            return;
        } else {
            memcpy(tasklet_data_ptr->ip, temp_ipv6, 16);
            uint8_t temp_ipv6_local[16];
            if (arm_net_address_get(tasklet_data_ptr->network_interface_id, ADDR_IPV6_LL, temp_ipv6_local) == 0
                    && (memcmp(temp_ipv6, temp_ipv6_local, 16) != 0)) {
                enet_tasklet_network_state_changed(MESH_CONNECTED_GLOBAL);
            } else {
                enet_tasklet_network_state_changed(MESH_CONNECTED_LOCAL);;
            }
        }
    } else {
        if (tasklet_data_ptr->connection_status != MESH_DISCONNECTED &&
                tasklet_data_ptr->connection_status != MESH_BOOTSTRAP_STARTED) {
            enet_tasklet_network_state_changed(MESH_DISCONNECTED);
        }
    }
}

/*
 * \brief Configure and establish network connection
 *
 */
void enet_tasklet_configure_and_connect_to_network(void)
{
    int8_t status;

    status = arm_nwk_interface_up(tasklet_data_ptr->network_interface_id);
    if (status >= 0) {
        tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_STARTED;
        tr_info("Start Bootstrap");
        enet_tasklet_network_state_changed(MESH_BOOTSTRAP_STARTED);
    } else {
        tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_FAILED;
        tr_err("Bootstrap start failed, %d", status);
        enet_tasklet_network_state_changed(MESH_BOOTSTRAP_START_FAILED);
    }
}

/*
 * Inform application about network state change
 */
void enet_tasklet_network_state_changed(mesh_connection_status_t status)
{
    tasklet_data_ptr->connection_status = status;
    if (tasklet_data_ptr->mesh_api_cb) {
        (tasklet_data_ptr->mesh_api_cb)(status);
    }
}

/* Public functions */
int8_t enet_tasklet_connect(mesh_interface_cb callback, int8_t nwk_interface_id)
{
    int8_t re_connecting = true;
    int8_t tasklet_id = tasklet_data_ptr->tasklet;

    if (tasklet_data_ptr->tasklet_state == TASKLET_STATE_CREATED) {
        re_connecting = false;
    }

    memset(tasklet_data_ptr, 0, sizeof(tasklet_data_str_t));
    tasklet_data_ptr->mesh_api_cb = callback;
    tasklet_data_ptr->network_interface_id = nwk_interface_id;
    tasklet_data_ptr->tasklet_state = TASKLET_STATE_INITIALIZED;
    tasklet_data_ptr->poll_network_status_timeout =
        eventOS_timeout_every_ms(enet_tasklet_poll_network_status, 2000, NULL);

    if (re_connecting == false) {
        tasklet_data_ptr->tasklet = eventOS_event_handler_create(&enet_tasklet_main,
                                                                 ARM_LIB_TASKLET_INIT_EVENT);
        if (tasklet_data_ptr->tasklet < 0) {
            // -1 handler already used by other tasklet
            // -2 memory allocation failure
            return tasklet_data_ptr->tasklet;
        }
    } else {
        tasklet_data_ptr->tasklet = tasklet_id;
        mesh_system_send_connect_event(tasklet_data_ptr->tasklet);
    }

    return 0;
}

int8_t enet_tasklet_disconnect(bool send_cb)
{
    enet_tasklet_generate_event(APPL_BACKHAUL_INTERFACE_PHY_DOWN, MESH_DISCONNECTED);
    return 0;
}

void enet_tasklet_init(void)
{
    if (tasklet_data_ptr == NULL) {
        tasklet_data_ptr = ns_dyn_mem_alloc(sizeof(tasklet_data_str_t));
        tasklet_data_ptr->tasklet_state = TASKLET_STATE_CREATED;
        tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
    }
}

int8_t enet_tasklet_network_init(int8_t device_id)
{
    if (tasklet_data_ptr->network_interface_id != -1) {
        tr_debug("Interface already at active state\n");
        return tasklet_data_ptr->network_interface_id;
    }
    if (!eth_mac_api) {
        eth_mac_api = ethernet_mac_create(device_id);
    }

    tasklet_data_ptr->network_interface_id = arm_nwk_interface_ethernet_init(eth_mac_api, "eth0");

    tr_debug("interface ID: %d", tasklet_data_ptr->network_interface_id);
    arm_nwk_interface_configure_ipv6_bootstrap_set(
        tasklet_data_ptr->network_interface_id, NET_IPV6_BOOTSTRAP_AUTONOMOUS, NULL);
    return tasklet_data_ptr->network_interface_id;
}

/* For now, to enable to compile without Nanostack interface changes, enables Nanostack PPP
   interface only if default stack is Nanostack and interface cellular. After Nanostack
   with updated interface call arm_nwk_interface_ppp_init() is integrated to mbed-os, this
   flagging can be removed.
*/
#define NANOSTACK   0x11991199
#define CELLULAR    0x22992299
#if MBED_CONF_NSAPI_DEFAULT_STACK == NANOSTACK && MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == CELLULAR
#define ENABLE_NANOSTACK_PPP
#endif
#undef NANOSTACK
#undef CELLULAR

int8_t enet_tasklet_ppp_network_init(int8_t device_id)
{
#ifdef ENABLE_NANOSTACK_PPP
    if (tasklet_data_ptr->network_interface_id != -1) {
        tr_debug("Interface already at active state\n");
        return tasklet_data_ptr->network_interface_id;
    }
    if (!eth_mac_api) {
        eth_mac_api = ethernet_mac_create(device_id);
    }

    tasklet_data_ptr->network_interface_id = arm_nwk_interface_ppp_init(eth_mac_api, "ppp0");

    tr_debug("interface ID: %d", tasklet_data_ptr->network_interface_id);
    arm_nwk_interface_configure_ipv6_bootstrap_set(
        tasklet_data_ptr->network_interface_id, NET_IPV6_BOOTSTRAP_AUTONOMOUS, NULL);
    return tasklet_data_ptr->network_interface_id;
#else
    return -1;
#endif
}

void enet_tasklet_link_state_changed(bool up)
{
    if (up) {
        enet_tasklet_generate_event(APPL_BACKHAUL_LINK_UP, MESH_BOOTSTRAP_STARTED);
    } else {
        enet_tasklet_generate_event(APPL_BACKHAUL_LINK_DOWN, MESH_BOOTSTRAP_STARTED);
    }
}

static void enet_tasklet_generate_event(uint8_t link_status, mesh_connection_status_t mesh_status)
{
    arm_event_s event = {
        .receiver = tasklet_data_ptr->tasklet,
        .sender =  tasklet_data_ptr->tasklet,
        .event_type = APPLICATION_EVENT,
        .priority = ARM_LIB_LOW_PRIORITY_EVENT,
        .event_id = link_status,
        .event_data = mesh_status
    };
    eventOS_event_send(&event);
}
