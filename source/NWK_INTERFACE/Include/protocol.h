/*
 * Copyright (c) 2014-2019, Arm Limited and affiliates.
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
/**
 *
 * \file protocol.h
 * \brief Protocol support functions.
 *
 *  Protocol core support functions.
 *
 */

#ifndef _NS_PROTOCOL_H
#define _NS_PROTOCOL_H

#ifndef _NANOSTACK_SOURCE_CONFIG_H
#error "Why haven't you included config.h before all other headers?"
#endif

#include "NWK_INTERFACE/Include/protocol_abstract.h"

// Users of protocol.h can assume it includes these headers
#include "Core/include/ns_address_internal.h"
#include "Core/include/ns_buffer.h"

// Headers below this are implementation details - users of protocol.h shouldn't rely on them
#include "6LoWPAN/IPHC_Decode/lowpan_context.h"
#include "platform/arm_hal_phy.h"
#include "net_nwk_scan.h"
#include "net_interface.h"
#include "multicast_api.h"
#include "Service_Libs/Neighbor_cache/neighbor_table_definition.h"
#include "Service_Libs/Trickle/trickle.h"
#include "Service_Libs/pan_blacklist/pan_blacklist_api.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"
#include "net_polling_api.h"
#include "ipv6_stack/ipv6_routing_table.h"

struct mlme_scan_s;
struct mlme_scan_conf_s;
struct mac_api_s;
struct eth_mac_api_s;
struct arm_device_driver_list;
struct mlme_security_s;
struct load_balance_api;
struct nwk_wpan_nvm_api;

#define SLEEP_MODE_REQ      0x80
#define SLEEP_PERIOD_ACTIVE 0x40
#define ICMP_ACTIVE         0x08

extern void set_power_state(uint8_t mode);
extern void clear_power_state(uint8_t mode);
extern uint8_t check_power_state(uint8_t mode);

#define BUFFER_DATA_FIXED_SIZE 0
extern void protocol_push(buffer_t *buf);
extern void protocol_init(void);
extern void protocol_core_init(void);

#define INTERFACE_BOOTSTRAP_DEFINED     1
#define INTERFACE_SECURITY_DEFINED      2
#define INTERFACE_NETWORK_DRIVER_SETUP_DEFINED      4
#define INTERFACE_ND_BORDER_ROUTER_DEFINED      8


#define INTERFACE_SETUP_MASK        3
#define INTERFACE_SETUP_READY       3
#define INTERFACE_SETUP_NETWORK_DRIVER_MASK         5
#define INTERFACE_SETUP_NETWORK_DRIVER_READY        5

#define INTERFACE_SETUP_BORDER_ROUTER_MASK          11
#define INTERFACE_SETUP_BORDER_ROUTER_READY         11
typedef enum icmp_state {
    ER_ACTIVE_SCAN  = 0,
    ER_WARM_ACTIVE_SCAN  = 1,
    ER_SCAN = 2,
    ER_ADDRESS_REQ = 3,
    ER_BIND_COMP = 4,
#ifdef HAVE_RPL
    ER_RPL_MC       = 5,
    ER_RPL_SCAN     = 6,
    ER_RPL_UNICAST  = 7,
    ER_DAO_TX       = 8,
#endif
    ER_PANA_AUTH    = 9,
    ER_PANA_AUTH_DONE   = 10,
    ER_PANA_AUTH_ERROR  = 11,
    ER_BIND_FAIL = 12,
    ER_MLE_LINK_REQ = 13,
    ER_MLE_LINK_SHORT_SYNCH = 14,
    ER_MLE_LINK_ADDRESS_SYNCH = 15,
#ifdef HAVE_RPL
    ER_ROUTER_SYNCH = 17,
#endif
    ER_PANA_PING = 18,
    ER_PARENT_SYNCH_LOST = 19,
    ER_MLE_SCAN = 20,
    ER_MLE_SYNCH = 21,
    ER_MLE_ATTACH_READY = 22,
    ER_DHCP_ROUTER_ID_REQ = 23,
    ER_DHCP_ROUTER_ID_RELEASE = 24,
    ER_CHILD_ID_REQ,
    ER_BOOTSRAP_DONE,
    ER_BOOTSTRAP_CONNECTION_DOWN,
    ER_BOOTSTRAP_IP_ADDRESS_ALLOC_FAIL,
    ER_BOOTSTRAP_DAD_FAIL,
    ER_BOOTSTRAP_SCAN_FAIL,
    ER_BOOTSTRAP_LEADER_UP,
    ER_BOOTSTRAP_NEW_FRAGMENT_START,
    ER_WAIT_RESTART,
    ER_RPL_LOCAL_REPAIR,
} icmp_state_t;

typedef enum {
    INTERFACE_IDLE = 0,
    INTERFACE_UP = 1
} interface_mode_t;

typedef enum arm_internal_event_type {
    ARM_IN_TASKLET_INIT_EVENT = 0, /**< Tasklet Init come always when generate tasklet*/
    ARM_IN_NWK_INTERFACE_EVENT = 1, /**< Interface Bootstrap  or state update event */
    ARM_IN_PROTOCOL_TIMER_EVENT = 2, /*!*< System Timer event */
    ARM_IN_SOCKET_EVENT = 5,    /**< Interface Bootstrap  or state update event */
    ARM_IN_INTERFACE_BOOTSTRAP_CB, /** call net_bootsrap_cb_run */
    ARM_IN_INTERFACE_CORE_TIMER_CB, /** call net_bootsrap_cb_run */
    ARM_IN_INTERFACE_PROTOCOL_HANDLE, /** protocol_buffer_poll */
    ARM_IN_SECURITY_ECC_CALLER
} arm_internal_event_type_e;

typedef enum {
    ARM_NWK_BOOTSRAP_MODE_6LoWPAN_ROUTER = 0,
    ARM_NWK_BOOTSRAP_MODE_6LoWPAN_HOST,
    ARM_NWK_BOOTSRAP_MODE_6LoWPAN_SLEEPY_HOST,
    ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER,
    ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_ACCESPOINT,
    ARM_NWK_BOOTSRAP_MODE_6LoWPAN_RF_SNIFFER,
    ARM_NWK_BOOTSRAP_MODE_ETHERNET_ROUTER,
    ARM_NWK_BOOTSRAP_MODE_ETHERNET_HOST,
} arm_nwk_bootsrap_mode_e;

typedef enum {
    ARM_NWK_IDLE_MODE = 0,
    ARM_NWK_GP_IP_MODE,
    ARM_NWK_LL_IP_MODE,
    ARM_NWK_MAC_MODE,
    ARM_NWK_RAW_PHY_MODE,
    ARM_NWK_SNIFFER_MODE,
} arm_nwk_interface_mode_e;

#define INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY   1
#define INTERFACE_NWK_BOOTSRAP_ACTIVE                   2
#define INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION      4
#define INTERFACE_NWK_ACTIVE                            8
#define INTERFACE_NWK_ROUTER_DEVICE                     16
#define INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE              64
#define INTERFACE_NWK_BOOTSRAP_MLE                      128

struct nd_router;
struct nd_router_setup;

typedef struct {
    channel_list_s      *chanlist;
    uint8_t             channel;
    int8_t              radio_tx_power;
    uint32_t            channel_list;
    uint16_t            mac_short_adr;
    uint16_t            mac_panid;
    uint8_t             border_router_gp_adr[16];
    bool                initActive;
    struct nd_router    *nd_nwk;
    struct nd_router_setup *nd_border_router_configure;
} br_info_t;

typedef struct nwk_scan_params {
    uint8_t energy_treshold;
    uint8_t nwk_scan_res_size;
    channel_list_s stack_chan_list;
    uint8_t scan_duration;
    nwk_pan_descriptor_t *nwk_response_info;
    nwk_pan_descriptor_t *nwk_cur_active;
    bool active_scan_active;
} nwk_scan_params_t;

typedef struct nwk_filter_params {
    uint8_t nwk_active_scan_level;
    uint8_t beacon_protocol_id_filter;
    uint16_t net_pan_id_filter;
    uint8_t   *beacon_nwk_id_filter;
} nwk_filter_params_s;

typedef struct mac_cordinator {
    unsigned cord_adr_mode: 2;
    uint8_t mac_mlme_coord_address[8];
} mac_cordinator_s;

typedef struct arm_15_4_mac_parameters_t {
    /* Security API USE */
    unsigned mac_configured_sec_level: 3;
    unsigned mac_security_level: 3;
    unsigned mac_key_id_mode: 2;
    uint8_t mac_prev_key_index;
    uint8_t mac_next_key_index;
    uint8_t mac_default_key_index;
    /* security mlme attribute */
    uint8_t mac_prev_key_attribute_id;
    uint8_t mac_default_key_attribute_id;
    uint8_t mac_next_key_attribute_id;
    uint32_t security_frame_counter;
    bool shortAdressValid: 1;
    /* MAC PIB boolean */
    bool SecurityEnabled: 1;
    bool RxOnWhenIdle: 1;
    bool PromiscuousMode: 1;
    bool GTSPermit: 1;
    bool AssociationPermit: 1;
    bool AssociatedPANCoord: 1;
    bool TimestampSupported: 1;
    bool BattLifeExt: 1;
    bool AutoRequest: 1;
    bool MacUnsusecured_2003_cab: 1;
    /* MAC PIB boolean */
    channel_list_s mac_channel_list;
    uint8_t mac_channel;
    uint16_t pan_id;
    uint16_t mac_short_address;
    mac_cordinator_s mac_cordinator_info;
    uint8_t number_of_fhss_channel_retries;
    /* MAC Beacon info */
    uint8_t *mac_beacon_payload;
    uint8_t mac_beacon_payload_size;
    nwk_scan_params_t nwk_scan_params;
    nwk_filter_params_s nwk_filter_params;
    uint16_t mac_in_direct_entry_timeout;
    beacon_compare_rx_cb *beacon_compare_rx_cb_ptr;
    beacon_join_priority_tx_cb *beacon_join_priority_tx_cb_ptr;
    uint8_t (*beacon_ind)(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur);
    mac_neighbor_table_t *mac_neighbor_table;
} arm_15_4_mac_parameters_t;

typedef void mac_poll_fail_cb(int8_t nwk_interface_id);

typedef struct nwk_rfd_poll_setups {
    uint32_t nwk_app_poll_time;
    net_host_mode_t host_mode;
    uint32_t slow_poll_rate_seconds;
    uint32_t timeOutInSeconds;
    uint8_t nwk_parent_poll_fail;
    uint8_t protocol_poll;
    mlme_poll_t poll_req;
    bool pollActive: 1;
    bool macDeepSleepEnabled: 1;
    mac_poll_fail_cb *pollFailCb;
} nwk_rfd_poll_setups_s;

typedef struct nwk_pana_params {
    net_tls_cipher_e nwk_chipher_mode;
    net_pana_session_mode_e client_session_mode;
    uint32_t psk_key_id;
    uint8_t pana_client;
} nwk_pana_params_s;

typedef struct gp_ipv6_address_entry {
    uint8_t address[16];
    ns_list_link_t link;
} gp_ipv6_address_entry_t;

typedef NS_LIST_HEAD(gp_ipv6_address_entry_t, link) gp_ipv6_address_list_t;

typedef struct if_6lowpan_dad_entry {
    uint8_t address[16];        // IPv6
    uint32_t state_timer;       // ticks to state change - used by DAD, then can be used by protocol
    uint8_t count;              // general count field - used by DAD, then can be used by protocol
    bool active;                // RFC 4941 temporary address
} if_6lowpan_dad_entry_t;

typedef struct if_6lowpan_security_info {
    nwk_pana_params_s *pana_params;
    net_link_layer_psk_security_info_s psk_key_info;
    uint32_t mle_security_frame_counter;
    net_6lowpan_link_layer_sec_mode_e nwk_security_mode;
    uint8_t security_level;
} if_6lowpan_security_info_t;

typedef enum {
    IPV6_LL_CONFIG,
    IPV6_ROUTER_SOLICATION,
    IPV6_GP_GEN,
    IPV6_GP_CONFIG,
    IPV6_READY,
    IPV6_DHCPV6_SOLICATION,
    IPV6_DHCPV6_ADDRESS_REQUEST,
    IPV6_DHCPV6_ADDRESS_REQ_FAIL,
    //IPV6_DHCPV6_PREFIX_READY
} IPv6_ND_STATE;

typedef struct {
    net_ipv6_mode_e ipv6_stack_mode;
    IPv6_ND_STATE IPv6_ND_state;
    net_ipv6_accept_ra_e accept_ra;
    uint8_t     wb_table_ttl;
    uint16_t    ND_TIMER;
    uint8_t     static_prefix64[8];
    uint8_t     routerSolicationRetryCounter;
    bool        temporaryUlaAddressState;
} ipv6_interface_info_t;

struct thread_info_s;
struct ws_info_s;
struct mesh_callbacks_s;
struct auth_info;
struct rpl_domain;

/* Structure to keep track of timing of multicast adverts - potentially
 * multiple required: one for our own adverts in the interface structure below,
 * and one for each ABRO that we relay (in nd_router_t).
 */
typedef struct ipv6_ra_timing {
    uint32_t rtr_adv_last_send_time;    // monotonic time
    uint8_t initial_rtr_adv_count;
} ipv6_ra_timing_t;

/**
 * @brief scan_confirm_cb Callback function for scanning results
 * @param if_id Protocol interface id
 * @param conf MLME-SCAN confirm object (ownership not passed)
 */
typedef void scan_confirm_cb(int8_t if_id, const mlme_scan_conf_t *conf);
typedef void beacon_indication_cb(int8_t if_id, const mlme_beacon_ind_t *conf);
typedef void comm_status_indication_cb(int8_t if_id, const mlme_comm_status_t *status);


struct protocol_interface_info_entry {
    beacon_indication_cb *beacon_cb;
    scan_confirm_cb *scan_cb;
    comm_status_indication_cb *comm_status_ind_cb;
    nwk_interface_id nwk_id;
    int8_t id;
    int8_t bootStrapId;
    uint8_t zone_index[16];
    int8_t net_start_tasklet;
    const char *interface_name;
    ns_list_link_t link;
    arm_nwk_bootsrap_mode_e bootsrap_mode;
    net_6lowpan_gp_address_mode_e lowpan_address_mode;
    arm_nwk_interface_mode_e nwk_mode;
    uint8_t configure_flags;
    uint8_t lowpan_info;
    uint16_t bootsrap_state_machine_cnt;
    icmp_state_t nwk_bootstrap_state;
    if_address_list_t ip_addresses;
    uint8_t ip_addresses_max_slaac_entries;
    if_group_list_t ip_groups;
#ifdef MULTICAST_FORWARDING /* Conventional (non-MPL) forwarding */
    if_group_fwd_list_t ip_groups_fwd;
    uint8_t ip_mcast_fwd_for_scope;
#endif
#ifdef HAVE_MPL
    bool mpl_proactive_forwarding;
    multicast_mpl_seed_id_mode_e mpl_seed_id_mode;
    trickle_params_t mpl_data_trickle_params;
    trickle_params_t mpl_control_trickle_params;
    uint16_t mpl_seed_set_entry_lifetime;
    uint8_t mpl_seed_id[16];
    struct mpl_domain *mpl_domain;
#endif
    if_6lowpan_dad_entry_t if_6lowpan_dad_process;
    lowpan_context_list_t lowpan_contexts;
    uint16_t lowpan_desired_short_address;
    bool global_address_available : 1;
    bool reallocate_short_address_if_duplicate : 1;
    bool iids_map_to_mac : 1;
    bool opaque_slaac_iids : 1;
    bool ip_multicast_as_mac_unicast_to_parent : 1;
    uint8_t dad_failures;
    ipv6_neighbour_cache_t ipv6_neighbour_cache;
    uint8_t nwk_rpl_scan_counter;
    uint8_t nwk_nd_re_scan_count;

    int8_t nwk_timer_id;
    uint16_t icmp_tokens; /* Token bucket for ICMP rate limiting */
    uint16_t icmp_ra_tokens; /* Token bucket for RA receive rate limiting */
    uint16_t mle_link_reject_tokens; /* Token bucket for Report wrong working device */
    uint8_t iid_eui64[8]; // IID based on EUI-64 - used for link-local address
    uint8_t iid_slaac[8]; // IID to use for SLAAC addresses - may or may not be same as iid_eui64
    uint16_t max_link_mtu;
    /* RFC 4861 Host Variables */
    uint8_t cur_hop_limit;
    uint16_t reachable_time_ttl;        // s
    uint32_t base_reachable_time;       // ms
#ifdef HAVE_IPV6_ND
    bool recv_ra_routes : 1;
    bool recv_ra_prefixes: 1;
#endif
    bool send_mld: 1;
#ifdef HAVE_MPL
    bool mpl_seed: 1;
    bool mpl_treat_realm_domains_as_one: 1;
    bool mpl_auto_domain_on_group_join: 1;
#endif
    bool send_na : 1;
    /* RFC 4861 Router Variables */
    bool ip_forwarding : 1;
    bool ip_multicast_forwarding : 1;
#ifndef NO_RADV_TX
    bool adv_send_advertisements : 1;
    bool rtr_adv_unicast_to_rs : 1;
    bool adv_copy_heard_flags : 1;
    uint8_t rtr_adv_flags;
    uint8_t max_ra_delay_time;          // 100ms ticks
    uint8_t min_delay_between_ras;      // 100ms ticks
    uint8_t max_initial_rtr_advertisements;
    uint16_t max_initial_rtr_adv_interval; // 100ms ticks
    uint8_t adv_cur_hop_limit;
    uint32_t adv_reachable_time;
    uint32_t adv_retrans_timer;
    uint16_t adv_link_mtu;
    uint16_t min_rtr_adv_interval;      // 100ms ticks
    uint16_t max_rtr_adv_interval;      // 100ms ticks
    ipv6_ra_timing_t ra_timing;
#endif
    /* RFC 4862 Node Configuration */
    uint8_t dup_addr_detect_transmits;
    uint16_t pmtu_lifetime;             // s

    /* Link Layer Part */
    uint8_t mac[8]; // MAC address (EUI-64 for LoWPAN, EUI-48 for Ethernet)

    interface_mode_t interface_mode;
    ipv6_interface_info_t *ipv6_configure;
    struct auth_info *pana_sec_info_temp;
    br_info_t *border_router_setup;
    struct load_balance_api *lb_api;
    neigh_cache_s neigh_cache;
    pan_blaclist_cache_s pan_blaclist_cache;
    pan_coordinator_blaclist_cache_s pan_cordinator_black_list;
#ifdef HAVE_THREAD
    struct thread_info_s *thread_info;
#endif
#ifdef HAVE_WS
    struct ws_info_s *ws_info;
#endif
    struct rpl_domain *rpl_domain;
    struct mesh_callbacks_s *mesh_callbacks;
    if_6lowpan_security_info_t *if_lowpan_security_params; //Security Parameters

    struct mac_api_s *mac_api;
    arm_15_4_mac_parameters_t *mac_parameters;

    struct eth_mac_api_s *eth_mac_api;

    struct nwk_wpan_nvm_api *nwk_wpan_nvm_api;
    nwk_rfd_poll_setups_s *rfd_poll_info;
    struct arm_device_driver_list *dev_driver;
    int8_t (*if_down)(struct protocol_interface_info_entry *cur);
    int8_t (*if_up)(struct protocol_interface_info_entry *cur);
    void (*if_stack_buffer_handler)(buffer_t *);
    bool (*if_ns_transmit)(struct protocol_interface_info_entry *cur, ipv6_neighbour_t *neighCacheEntry, bool unicast, uint8_t seq);
    bool (*if_map_ip_to_link_addr)(struct protocol_interface_info_entry *cur, const uint8_t *ip_addr, addrtype_t *ll_type, const uint8_t **ll_addr_out);
    bool (*if_map_link_addr_to_ip)(struct protocol_interface_info_entry *cur, addrtype_t ll_type, const uint8_t *ll_addr, uint8_t *ip_addr_out);
    buffer_t *(*if_special_forwarding)(struct protocol_interface_info_entry *cur, buffer_t *buf, const sockaddr_t *ll_src, bool *bounce);
    void (*if_special_multicast_forwarding)(struct protocol_interface_info_entry *cur, buffer_t *buf);
    buffer_t *(*if_snoop)(struct protocol_interface_info_entry *cur, buffer_t *buf, const sockaddr_t *ll_dst, const sockaddr_t *ll_src, bool *bounce);
    buffer_t *(*if_icmp_handler)(struct protocol_interface_info_entry *cur, buffer_t *buf, bool *bounce);
    uint8_t (*if_llao_parse)(struct protocol_interface_info_entry *cur, const uint8_t *opt_in, sockaddr_t *ll_addr_out);
    uint8_t (*if_llao_write)(struct protocol_interface_info_entry *cur, uint8_t *opt_out, uint8_t opt_type, bool must, const uint8_t *ip_addr);
    void (*mac_security_key_usage_update_cb)(struct protocol_interface_info_entry *cur, const struct mlme_security_s *security_params);
    uint16_t (*etx_read_override)(struct protocol_interface_info_entry *cur, addrtype_t addr_type, const uint8_t *addr_ptr);
};

typedef NS_LIST_HEAD(protocol_interface_info_entry_t, link) protocol_interface_list_t;

extern protocol_interface_list_t protocol_interface_info_list;

extern protocol_interface_info_entry_t *nwk_interface_get_ipv6_ptr(void);
extern void nwk_interface_print_neigh_cache(route_print_fn_t *print_fn);
extern void nwk_interface_flush_neigh_cache(void);

//void nwk_interface_dhcp_process_callback(int8_t interfaceID, bool status,uint8_t * routerId,  dhcpv6_client_server_data_t *server, bool reply);

void protocol_core_interface_info_reset(protocol_interface_info_entry_t *entry);

extern void arm_net_protocol_packet_handler(buffer_t *buf, protocol_interface_info_entry_t *cur_interface);

protocol_interface_info_entry_t *protocol_stack_interface_sleep_possibility(void);
extern uint8_t nwk_bootsrap_ready(protocol_interface_info_entry_t *cur);

extern protocol_interface_info_entry_t *protocol_stack_interface_info_get(nwk_interface_id nwk_id);
extern bool nwk_interface_compare_mac_address(protocol_interface_info_entry_t *cur, uint_fast8_t addrlen, const uint8_t addr[/*addrlen*/]);
extern protocol_interface_info_entry_t *protocol_stack_interface_generate_ethernet(struct eth_mac_api_s *api);
extern protocol_interface_info_entry_t *protocol_stack_interface_generate_ppp(struct eth_mac_api_s *api);
extern protocol_interface_info_entry_t *protocol_stack_interface_generate_lowpan(struct mac_api_s *api);
extern uint32_t protocol_stack_interface_set_reachable_time(protocol_interface_info_entry_t *cur, uint32_t base_reachable_time);
extern void net_bootsrap_cb_run(uint8_t event);

extern void protocol_core_security_tick_update(uint16_t tick_update);

extern int8_t protocol_read_tasklet_id(void);
extern void protocol_6lowpan_stack(buffer_t *b);
extern void protocol_6lowpan_register_handlers(protocol_interface_info_entry_t *cur);
extern void protocol_6lowpan_release_short_link_address_from_neighcache(protocol_interface_info_entry_t *cur, uint16_t shortAddress);
extern void protocol_6lowpan_release_long_link_address_from_neighcache(protocol_interface_info_entry_t *cur, uint8_t *mac64);
extern void protocol_core_dhcpv6_allocated_address_remove(protocol_interface_info_entry_t *cur, uint8_t *guaPrefix);

extern void nwk_bootsrap_state_update(arm_nwk_interface_status_type_e posted_event, protocol_interface_info_entry_t *cur);
void bootsrap_next_state_kick(icmp_state_t new_state, protocol_interface_info_entry_t *cur);
int8_t protocol_interface_address_compare(const uint8_t *addr);
bool protocol_address_prefix_cmp(protocol_interface_info_entry_t *cur, const uint8_t *prefix, uint8_t prefix_len);
bool protocol_interface_any_address_match(const uint8_t *prefix, uint8_t prefix_len);
#endif /* _NS_PROTOCOL_H */
