/*
 * Copyright (c) 2018-2019 ARM Limited. All rights reserved.
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
#include "ns_list.h"
#include "net_interface.h"
#include "nwk_stats_api.h"
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
    bool configured_and_connected;
} wisun_tasklet_data_str_t;

typedef struct {
    arm_certificate_entry_s arm_cert_entry;
    ns_list_link_t      link; /*!< List link entry */
} wisun_certificate_entry_t;

typedef NS_LIST_HEAD(wisun_certificate_entry_t, link) cert_list_t;
typedef struct {
    cert_list_t own_certificates_list;
    cert_list_t trusted_certificates_list;
} wisun_certificates_t;

/* Tasklet data */
static wisun_tasklet_data_str_t *wisun_tasklet_data_ptr = NULL;
static wisun_certificates_t *wisun_certificates_ptr = NULL;
static mac_api_t *mac_api = NULL;

typedef struct {
    nwk_stats_t nwk_stats;
    mac_statistics_t mac_statistics;
    ws_statistics_t ws_statistics;
} wisun_statistics_t;

static bool statistics_started = false;
static wisun_statistics_t *statistics = NULL;

extern fhss_timer_t fhss_functions;

/* private function prototypes */
static void wisun_tasklet_main(arm_event_s *event);
static void wisun_tasklet_network_state_changed(mesh_connection_status_t status);
static void wisun_tasklet_parse_network_event(arm_event_s *event);
static void wisun_tasklet_configure_and_connect_to_network(void);
static void wisun_tasklet_clear_stored_certificates(int8_t clear_trusted, int8_t clear_own);
static int wisun_tasklet_store_certificate_data(const uint8_t *cert, uint16_t cert_len, const uint8_t *cert_key, uint16_t cert_key_len, bool trusted_cert);
static int wisun_tasklet_add_stored_certificates(int8_t *trusted_set, int8_t *own_set);
static void wisun_tasklet_statistics_do_start(void);

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
            tr_info("Wi-SUN bootstrap ready");
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_READY;
            wisun_tasklet_network_state_changed(MESH_CONNECTED);
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
            wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_BOOTSTRAP_STARTED;
            wisun_tasklet_network_state_changed(MESH_BOOTSTRAP_STARTED);
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

    if ((wisun_tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_READY &&
            wisun_tasklet_data_ptr->tasklet_state != TASKLET_STATE_BOOTSTRAP_STARTED) &&
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
    int status;

    if (wisun_tasklet_data_ptr->configured_and_connected) {
        return;
    }

    fhss_timer_t *fhss_timer_ptr = &fhss_functions;

    if (MBED_CONF_MBED_MESH_API_WISUN_DEVICE_TYPE == MESH_DEVICE_TYPE_WISUN_BORDER_ROUTER) {
        wisun_tasklet_data_ptr->operating_mode = NET_6LOWPAN_BORDER_ROUTER;
    } else {
        wisun_tasklet_data_ptr->operating_mode = NET_6LOWPAN_ROUTER;
    }

    wisun_tasklet_data_ptr->operating_mode_extension = NET_6LOWPAN_WS;

    arm_nwk_interface_configure_6lowpan_bootstrap_set(
        wisun_tasklet_data_ptr->network_interface_id,
        wisun_tasklet_data_ptr->operating_mode,
        wisun_tasklet_data_ptr->operating_mode_extension);

    int8_t trusted_set = false;
    int8_t own_set = false;
    if (wisun_tasklet_add_stored_certificates(&trusted_set, &own_set) != 0) {
        tr_error("Can't set Wi-SUN certificates");
        return;
    }

    char network_name[33];
    status = ws_management_network_name_get(wisun_tasklet_data_ptr->network_interface_id, (char *) &network_name);
    if (status < 0) {
        tr_error("Failed to read network name");
        return;
    }
    uint8_t regulatory_domain;
    uint8_t operating_class;
    uint8_t operating_mode;
    status = ws_management_regulatory_domain_get(wisun_tasklet_data_ptr->network_interface_id,
                                                 &regulatory_domain,
                                                 &operating_class,
                                                 &operating_mode);
    if (status < 0) {
        tr_error("Failed to read regulatory domain");
        return;
    }
    status = ws_management_node_init(wisun_tasklet_data_ptr->network_interface_id,
                                     regulatory_domain,
                                     (char *) network_name,
                                     fhss_timer_ptr);
    if (status < 0) {
        tr_error("Failed to initialize WS");
        return;
    }

#if defined(MBED_CONF_MBED_MESH_API_CERTIFICATE_HEADER)
    if (!trusted_set) {
        arm_certificate_entry_s trusted_cert = {
            .cert = MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE,
            .key = NULL,
            .cert_len = 0,
            .key_len = 0
        };
#ifdef MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE_LEN
        trusted_cert.cert_len = MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE_LEN;
#else
        trusted_cert.cert_len = strlen((const char *) MBED_CONF_MBED_MESH_API_ROOT_CERTIFICATE) + 1;
#endif
        arm_network_trusted_certificates_remove();
        arm_network_trusted_certificate_add((const arm_certificate_entry_s *)&trusted_cert);
    }
    if (!own_set) {
        arm_certificate_entry_s own_cert = {
            .cert = MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE,
            .key =  MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_KEY,
            .cert_len = 0,
            .key_len = 0
        };
#ifdef MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_LEN
        own_cert.cert_len = MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_LEN;
#else
        own_cert.cert_len = strlen((const char *) MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE) + 1;
#endif
#ifdef MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_KEY_LEN
        own_cert.key_len = MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_KEY_LEN;
#else
        own_cert.key_len = strlen((const char *) MBED_CONF_MBED_MESH_API_OWN_CERTIFICATE_KEY) + 1;
#endif
        arm_network_own_certificates_remove();
        arm_network_own_certificate_add((const arm_certificate_entry_s *)&own_cert);
    }
#endif

    if (statistics_started) {
        wisun_tasklet_statistics_do_start();
    }

    status = arm_nwk_interface_up(wisun_tasklet_data_ptr->network_interface_id);
    if (status >= 0) {
        wisun_tasklet_data_ptr->configured_and_connected = true;
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

static int wisun_tasklet_store_certificate_data(const uint8_t *cert, uint16_t cert_len, const uint8_t *cert_key, uint16_t cert_key_len, bool trusted_cert)
{
    if (wisun_certificates_ptr == NULL) {
        wisun_certificates_ptr = (wisun_certificates_t *)ns_dyn_mem_alloc(sizeof(wisun_certificates_t));
        if (!wisun_certificates_ptr) {
            return -1;
        }
        ns_list_init(&wisun_certificates_ptr->own_certificates_list);
        ns_list_init(&wisun_certificates_ptr->trusted_certificates_list);
    }

    wisun_certificate_entry_t *ws_cert_entry_store = (wisun_certificate_entry_t *)ns_dyn_mem_alloc(sizeof(wisun_certificate_entry_t));
    if (!ws_cert_entry_store) {
        wisun_tasklet_clear_stored_certificates(true, true);
        return -1;
    }

    ws_cert_entry_store->arm_cert_entry.cert = cert;
    ws_cert_entry_store->arm_cert_entry.cert_len = cert_len;

    if (cert_key) {
        ws_cert_entry_store->arm_cert_entry.key = cert_key;
        ws_cert_entry_store->arm_cert_entry.key_len = cert_key_len;
    } else {
        ws_cert_entry_store->arm_cert_entry.key = NULL;
        ws_cert_entry_store->arm_cert_entry.key_len = 0;
    }

    if (trusted_cert) {
        ns_list_add_to_end(&wisun_certificates_ptr->trusted_certificates_list, ws_cert_entry_store);
    } else {
        ns_list_add_to_end(&wisun_certificates_ptr->own_certificates_list, ws_cert_entry_store);
    }

    return 0;
}

static void wisun_tasklet_clear_stored_certificates(int8_t clear_trusted, int8_t clear_own)
{
    if (!wisun_certificates_ptr) {
        return;
    }

    if (clear_trusted) {
        ns_list_foreach_safe(wisun_certificate_entry_t, trusted_cert_entry, &wisun_certificates_ptr->trusted_certificates_list) {
            ns_list_remove(&wisun_certificates_ptr->trusted_certificates_list, trusted_cert_entry);
            ns_dyn_mem_free(trusted_cert_entry);
        }
    }

    if (clear_own) {
        ns_list_foreach_safe(wisun_certificate_entry_t, own_cert_entry, &wisun_certificates_ptr->own_certificates_list) {
            ns_list_remove(&wisun_certificates_ptr->own_certificates_list, own_cert_entry);
            ns_dyn_mem_free(own_cert_entry);
        }
    }

    if (ns_list_is_empty(&wisun_certificates_ptr->trusted_certificates_list) && ns_list_is_empty(&wisun_certificates_ptr->own_certificates_list)) {
        ns_dyn_mem_free(wisun_certificates_ptr);
        wisun_certificates_ptr = NULL;
    }
}

static int wisun_tasklet_add_stored_certificates(int8_t *trusted_set, int8_t *own_set)
{
    int8_t status = 0;

    if (wisun_certificates_ptr == NULL) {
        // certificates not updated
        return 0;
    }

    if (!ns_list_is_empty(&wisun_certificates_ptr->trusted_certificates_list)) {
        status = arm_network_trusted_certificates_remove();
        if (status != 0) {
            goto CERTIFICATE_SET_END;
        }
    }

    if (!ns_list_is_empty(&wisun_certificates_ptr->own_certificates_list)) {
        status = arm_network_own_certificates_remove();
        if (status != 0) {
            goto CERTIFICATE_SET_END;
        }
    }

    ns_list_foreach(wisun_certificate_entry_t, cert_entry, &wisun_certificates_ptr->trusted_certificates_list) {
        status = arm_network_trusted_certificate_add(&cert_entry->arm_cert_entry);
        if (status != 0) {
            goto CERTIFICATE_SET_END;
        }
        *trusted_set = true;
    }

    ns_list_foreach(wisun_certificate_entry_t, cert_entry, &wisun_certificates_ptr->own_certificates_list) {
        status = arm_network_own_certificate_add(&cert_entry->arm_cert_entry);
        if (status != 0) {
            goto CERTIFICATE_SET_END;
        }
        *own_set = true;
    }

CERTIFICATE_SET_END:
    wisun_tasklet_clear_stored_certificates(true, true);

    return status;
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
    bool re_connecting = true;
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
    wisun_tasklet_data_ptr->configured_and_connected = false;

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
        wisun_tasklet_data_ptr->configured_and_connected = false;
        wisun_tasklet_data_ptr->mesh_api_cb = NULL;
    }
    return status;
}

void wisun_tasklet_init(void)
{
    if (wisun_tasklet_data_ptr == NULL) {
        wisun_tasklet_data_ptr = (wisun_tasklet_data_str_t *)ns_dyn_mem_alloc(sizeof(wisun_tasklet_data_str_t));
        // allocation not validated, in case of failure execution stops here
        memset(wisun_tasklet_data_ptr, 0, sizeof(wisun_tasklet_data_str_t));
        wisun_tasklet_data_ptr->tasklet_state = TASKLET_STATE_CREATED;
        wisun_tasklet_data_ptr->network_interface_id = INVALID_INTERFACE_ID;
        wisun_tasklet_data_ptr->configured_and_connected = false;
    }
}

int8_t wisun_tasklet_network_init(int8_t device_id)
{
    // TODO, read interface name from configuration
    mac_description_storage_size_t storage_sizes;
    storage_sizes.device_decription_table_size = MBED_CONF_MBED_MESH_API_MAC_NEIGH_TABLE_SIZE;
    storage_sizes.key_description_table_size = 4;
    storage_sizes.key_lookup_size = 1;
    storage_sizes.key_usage_size = 3;
    if (!mac_api) {
        mac_api = ns_sw_mac_create(device_id, &storage_sizes);
    }

    return arm_nwk_interface_lowpan_init(mac_api, INTERFACE_NAME);
}

int wisun_tasklet_set_own_certificate(uint8_t *cert, uint16_t cert_len, uint8_t *cert_key, uint16_t cert_key_len)
{
    if (wisun_tasklet_data_ptr && wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID && wisun_tasklet_data_ptr->configured_and_connected) {
        // interface already active write certificates to stack.
        arm_certificate_entry_s arm_cert_entry;
        arm_cert_entry.cert = cert;
        arm_cert_entry.cert_len = cert_len;
        arm_cert_entry.key = cert_key;
        arm_cert_entry.key_len = cert_key_len;
        return arm_network_own_certificate_add(&arm_cert_entry);
    }
    // Stack is inactive store the certificates and activate when connect() called
    return wisun_tasklet_store_certificate_data(cert, cert_len, cert_key, cert_key_len, false);
}

int wisun_tasklet_remove_own_certificates(void)
{
    if (wisun_tasklet_data_ptr && wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID && wisun_tasklet_data_ptr->configured_and_connected) {
        int8_t status = arm_network_own_certificates_remove();
        if (status != 0) {
            return -1;
        }
        return 0;
    }

    wisun_tasklet_clear_stored_certificates(false, true);
    return 0;
}

int wisun_tasklet_remove_trusted_certificates(void)
{
    if (wisun_tasklet_data_ptr && wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID && wisun_tasklet_data_ptr->configured_and_connected) {
        int8_t status = arm_network_trusted_certificates_remove();
        if (status != 0) {
            return -1;
        }
        return 0;
    }

    wisun_tasklet_clear_stored_certificates(true, false);
    return 0;
}

int wisun_tasklet_set_trusted_certificate(uint8_t *cert, uint16_t cert_len)
{
    if (wisun_tasklet_data_ptr && wisun_tasklet_data_ptr->network_interface_id != INVALID_INTERFACE_ID && wisun_tasklet_data_ptr->configured_and_connected) {
        // interface already active write certificates to stack.
        arm_certificate_entry_s arm_cert_entry;
        arm_cert_entry.cert = cert;
        arm_cert_entry.cert_len = cert_len;
        arm_cert_entry.key = NULL;
        arm_cert_entry.key_len = 0;
        return arm_network_trusted_certificate_add(&arm_cert_entry);
    }
    // Stack is inactive store the certificates and activate when connect() called
    return wisun_tasklet_store_certificate_data(cert, cert_len, NULL, 0, true);
}

int wisun_tasklet_statistics_start(void)
{
    statistics_started = true;

    if (statistics == NULL) {
        statistics = ns_dyn_mem_alloc(sizeof(wisun_statistics_t));
    }
    if (statistics == NULL) {
        return -1;
    }
    memset(statistics, 0, sizeof(wisun_statistics_t));

    wisun_tasklet_statistics_do_start();

    return 0;
}

static void wisun_tasklet_statistics_do_start(void)
{
    if (!wisun_tasklet_data_ptr || wisun_tasklet_data_ptr->network_interface_id < 0 || !mac_api) {
        return;
    }

    protocol_stats_start(&statistics->nwk_stats);
    ns_sw_mac_statistics_start(mac_api, &statistics->mac_statistics);
    ws_statistics_start(wisun_tasklet_data_ptr->network_interface_id, &statistics->ws_statistics);
}

int wisun_tasklet_statistics_nw_read(mesh_nw_statistics_t *stats)
{
    if (!statistics || !stats) {
        return -1;
    }

    stats->rpl_total_memory = statistics->nwk_stats.rpl_total_memory;
    stats->etx_1st_parent = statistics->nwk_stats.etx_1st_parent;
    stats->etx_2nd_parent = statistics->nwk_stats.etx_2nd_parent;
    stats->asynch_tx_count = statistics->ws_statistics.asynch_tx_count;
    stats->asynch_rx_count = statistics->ws_statistics.asynch_rx_count;

    return 0;
}

int wisun_tasklet_statistics_mac_read(mesh_mac_statistics_t *stats)
{
    if (!statistics || !stats) {
        return -1;
    }

    stats->mac_rx_count = statistics->mac_statistics.mac_rx_count;
    stats->mac_tx_count = statistics->mac_statistics.mac_tx_count;
    stats->mac_bc_rx_count = statistics->mac_statistics.mac_bc_rx_count;
    stats->mac_bc_tx_count = statistics->mac_statistics.mac_bc_tx_count;
    stats->mac_tx_bytes = statistics->mac_statistics.mac_tx_bytes;
    stats->mac_rx_bytes = statistics->mac_statistics.mac_rx_bytes;
    stats->mac_tx_failed_count = statistics->mac_statistics.mac_tx_failed_count;
    stats->mac_retry_count = statistics->mac_statistics.mac_retry_count;
    stats->mac_cca_attempts_count = statistics->mac_statistics.mac_cca_attempts_count;
    stats->mac_failed_cca_count = statistics->mac_statistics.mac_failed_cca_count;

    return 0;
}
