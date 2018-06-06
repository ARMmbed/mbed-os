/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef LOWPAN_THREAD_H_
#define LOWPAN_THREAD_H_

#include "ns_list.h"
#include "libDHCPv6/libDHCPv6.h"
#include "libDHCPv6/libDHCPv6_server.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "6LoWPAN/Thread/thread_network_data_storage.h"
#include "thread_management_if.h"
#include "thread_commissioning_api.h"
#include "thread_border_router_api.h"
#include "eventOS_event_timer.h"
#include "MLE/mle_tlv.h"

#define MAX_MLE_CHALLENGE_LENGTH 32

/*
 * How long a Leader must wait after a router ID has become unassigned before it can be reused.
 *
 */
#define ROUTER_ID_REUSE_DELAY 100 //Seconds

#define LEADER_NVM_SYNC_DELAY 30 // Leader router ids write delay to NVM

#define ROUTER_ID_INFINITY_DELAY 90 //Seconds

#define NETWORK_ID_TIMEOUT 120 //seconds

// Values when adverticements are made faster when leader connection is restored
#define NETWORK_ID_SPEEDUP 60 //seconds
#define NETWORK_ID_SPEEDUP_MAX 100 //seconds

#define DHCPV6_ENTERPRISE_THREAD 0x0000AFAA
#define DHCPV6_OPTION_VENDOR_SPESIFIC_INFO_LEN 0x0011
#define DHCPV6_ROUTER_ID_OPTION 0x0000
#define DHCPV6_ROUTER_ID_OPTION_LEN 0x0009

#define THREAD_KEY_INDEX(seq) ((uint8_t) (((seq) & 0x0000007f) + 1))

extern uint8_t thread_version;
extern uint32_t thread_delay_timer_default;
extern uint32_t thread_router_selection_jitter;
extern uint16_t thread_joiner_port;

typedef enum {
    THREAD_STATE_NETWORK_DISCOVER,      // Not commissioned to Thread network
    THREAD_STATE_REATTACH,              // Connection to leader lost, searching for new parent
    THREAD_STATE_REATTACH_RETRY,        // Connection to leader lost, searching for new parent with REED bit is set
    THREAD_STATE_ATTACH_ANY,            // Searching for all partitions with leader connectivity
    THREAD_STATE_CONNECTED,             // Attached to Thread network - can't route
    THREAD_STATE_CONNECTED_ROUTER,      // Attached to Thread network - Routing enabled
} thread_attach_state_e;

typedef enum {
    THREAD_DEVICE_MODE_ROUTER,
    THREAD_DEVICE_MODE_FULL_END_DEVICE,
    THREAD_DEVICE_MODE_END_DEVICE,
    THREAD_DEVICE_MODE_SLEEPY_END_DEVICE,
} thread_attach_device_mode_e;

typedef enum {
    THREAD_COMMISSIONER_NOT_REGISTERED,
    THREAD_COMMISSIONER_REGISTRATION_OBSOLETE,
    THREAD_COMMISSIONER_REGISTERED
} thread_commissioner_register_status_e;

typedef struct thread_mcast_child {
    uint8_t mac64[8];
    ns_list_link_t link;
} thread_mcast_child_t;

typedef NS_LIST_HEAD(thread_mcast_child_t, link) thread_mcast_children_list_t;

typedef struct thread_registered_mcast_addr {
    uint8_t address[16];
    thread_mcast_children_list_t children;
    ns_list_link_t link;
} thread_registered_mcast_addr_t;

typedef NS_LIST_HEAD(thread_registered_mcast_addr_t, link) thread_registered_mcast_addr_list_t;

typedef struct thread_pending_child_id_req {
    uint8_t euid64[8];
    uint8_t eiid[8];
    uint8_t linkMargin;
    uint8_t mode;
    uint32_t keySeq;
    uint8_t keyId;
    bool request_active_config;
    uint64_t active_timestamp;
    uint64_t pending_timestamp;
    uint32_t frameCounter;
    uint32_t mleFrameCounter;
    uint32_t timeout;
    uint16_t version;
    bool    shortAddressReq: 1;
    bool    networkDataReq: 1;
    bool    routeReq: 1;
    ns_list_link_t      link;                   /*!< List link entry */
} thread_pending_child_id_req_t;

typedef NS_LIST_HEAD(thread_pending_child_id_req_t, link) thread_pending_id_req_list_t;

typedef struct thread_router_id_entry_s {
    uint8_t eui64[8];
    uint32_t validLifeTime;
    bool    reUsePossible;
} thread_router_id_entry_t;

typedef struct thread_network_data_req_s {
    uint8_t request_destination[16];
    uint8_t timeout;
    bool active;
} thread_network_data_req_t;

typedef struct thread_leader_info_s {
    //DHCPv6 Allocated Router Id's
    thread_router_id_entry_t thread_router_id_list[64];
    uint8_t leader_id_seq_timer;
    uint8_t master_router_id_mask[8];
    uint8_t maskSeq;
    uint8_t leader_nvm_sync_timer;
} thread_leader_info_t;

typedef struct thread_leader_data_s {
    uint32_t partitionId; //Unique partition ID
    uint8_t dataVersion;    //Leader data version number
    uint8_t stableDataVersion;  //Leader data stable version number
    uint8_t leaderRouterId;   //Leader Router ID
    uint8_t weighting;  //Data weighting
} thread_leader_data_t;

typedef struct thread_connectivity_s {
    uint16_t SEDBufferSize;
    int8_t  parentPriority;
    uint8_t linkQuality3;
    uint8_t linkQuality2;
    uint8_t linkQuality1;
    uint8_t leaderCost;
    uint8_t idSequence;
    uint8_t activeRouters;
    uint8_t SEDDatagramCount;
} thread_connectivity_t;

typedef struct thread_parent_info_s {
    uint8_t mac64[8];
    uint16_t shortAddress;
    uint8_t router_id;
    uint8_t pathCostToLeader;
    bool    childUpdatePending: 1;
    bool    childUpdateProcessActive: 1;
} thread_parent_info_t;


typedef struct thread_scanned_parent_s {
    thread_leader_data_t leader_data;
    uint8_t mac64[8];
    uint8_t challengeData[MAX_MLE_CHALLENGE_LENGTH];
    uint32_t linLayerFrameCounter;
    uint32_t mleFrameCounter;
    uint32_t keySequence;
    uint16_t shortAddress;
    uint8_t chal_len;
    uint8_t linkMarginFromParent;
    uint8_t linkMarginToParent;
    uint8_t security_key_index;
    uint8_t routeCostToLeader;
    uint8_t linkQuality3;
    uint8_t linkQuality2;
    uint8_t linkQuality1;
    int8_t parentPriority;
    uint8_t activeRouters;
    uint16_t version;
    uint16_t child_id_request_id;
} thread_scanned_parent_t;

typedef struct thread_ula_prefix_s {
    uint8_t     ulaPrefix[8];
    bool        ulaValid;
} thread_ula_prefix_t;

typedef struct thread_master_secret_material_s {
    uint8_t     historyKey[32];
    uint32_t    keySwitchGuardTimer;
    uint32_t    keyRotation;
    bool        historyKeyValid:1;
    bool        valid_Info:1;
    uint8_t     historyKeyId;
} thread_master_secret_material_t;

/*This structure is used for storing active commissioner information*/
typedef struct thread_commissioner {
    uint8_t steering_data[16];
    uint8_t border_router_address[16];
    timeout_t *commissioner_timeout;
    char *commissioner_id_ptr;
    uint16_t session_id;    /* session_id is valid even if commissioner is not valid */
    thread_commissioner_register_status_e commissioner_registration; /* Only for Leader */
    uint8_t steering_data_len;
    bool commissioner_valid:1;
} thread_commissioner_t;

typedef struct thread_announcement_s {
    timeout_t *timer;
    uint64_t timestamp;
    uint16_t channel;
    uint16_t panid;
    uint8_t channel_page;
    uint8_t count;
    uint16_t period;
    bool announce_success;
} thread_announcement_t;

typedef struct thread_router_select {
    uint8_t routerUpgradeThresHold;
    uint8_t routerDowngradeThresHold;
    uint8_t possibleDefaultParent;
    bool    jitterTimerActive;
    uint16_t reedAdvertisementInterval;
    uint16_t reedAdvertisementJitterInterval;
    timeout_t *reedAdvertisementTimeout;
} thread_router_select_t;

struct thread_extension_info;
struct thread_extension_credentials;

typedef struct thread_previous_partition_info_s {
    uint32_t partitionId; //partition ID of the previous partition
    uint8_t idSequence;   //idSequence last heard from the previous partition
    uint8_t weighting;    //weighting last heard from the previous partition
} thread_previous_partition_t;


typedef struct thread_info_s {
    thread_routing_info_t routing;
    thread_master_secret_material_t masterSecretMaterial;
    thread_network_data_cache_entry_t networkDataStorage;
    thread_network_local_data_cache_entry_t localServerDataBase;
    thread_pending_id_req_list_t childIdReqPending;
    thread_commissioner_t registered_commissioner;
    thread_ula_prefix_t threadPrivatePrefixInfo;
    thread_router_select_t routerSelectParameters;
    thread_previous_partition_t previous_partition_info;
    thread_leader_info_t *leader_private_data;  //Leader Proxy
    thread_parent_info_t *thread_endnode_parent;
    thread_leader_data_t *thread_leader_data;  //Dynamic Learned Network Data
    thread_scanned_parent_t *thread_attach_scanned_parent;
    thread_commissioning_link_configuration_s *native_commissioner_link;
    thread_commissioning_native_select_cb *native_commissioner_cb;
    thread_network_data_tlv_cb *network_data_tlv_cb;
    thread_announcement_t *announcement_info;
    struct thread_extension_info *extension_info;
    struct thread_extension_credentials *extension_credentials_ptr;
    thread_attach_device_mode_e thread_device_mode;
    thread_attach_state_e thread_attached_state; //Indicate Thread stack state
    thread_registered_mcast_addr_list_t child_mcast_list;
    uint32_t host_link_timeout;
    uint32_t sleepy_host_poll_time;
    uint32_t testRandomPartitionId;
    uint16_t lastAllocatedChildAddress;
    uint16_t native_commissioner_port;
    uint16_t routerShortAddress;
    uint16_t reedJitterTimer;
    uint16_t reedMergeAdvTimer;
    uint16_t routerIdReqCoapID;  // COAP msg id of RouterID request
    int16_t childUpdateReqTimer;
    uint16_t childUpdateReqMsgId;
    uint16_t proactive_an_timer;
    //uint8_t lastValidRouteMask[8];
    int8_t interface_id; //Thread Interface ID
    uint8_t version;
    uint8_t testMaxActiveRouterIdLimit; //Default for this is 32
    uint8_t maxChildCount; //Default for this is 24
    uint8_t partition_weighting;
    bool rfc6775: 1;
    bool requestFullNetworkData: 1;
    bool leaderCab: 1;
    bool releaseRouterId: 1;
    bool networkSynch: 1;
    bool networkDataRequested: 1;
    bool end_device_link_synch: 1;
    bool router_mc_addrs_registered: 1;
    bool leader_synced:1; // flag used by leader after restart
} thread_info_t;

#ifdef HAVE_THREAD

struct link_configuration;
struct if_group_entry;

/* Ceil log2 function  Needed to calculate specific values for thread TLVs.
 * This is very approximate
 */
uint16_t thread_log2_aprx(uint32_t n);

void thread_anycast_address_policy_update(const thread_info_t *thread_info, bool addPolicy);

#define thread_info(cur) ((cur)->thread_info)
#define thread_am_reed(cur) ((cur)->thread_info && (cur)->thread_info->thread_attached_state == THREAD_STATE_CONNECTED && (cur)->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER)
#define thread_am_router(cur) ((cur)->thread_info && (cur)->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER && (cur)->thread_info->thread_device_mode == THREAD_DEVICE_MODE_ROUTER)
#define thread_is_connected(cur) ((cur)->thread_info && ((cur)->thread_info->thread_attached_state == THREAD_STATE_CONNECTED || (cur)->thread_info->thread_attached_state == THREAD_STATE_CONNECTED_ROUTER))
#define thread_am_host(cur) ((cur)->thread_info && \
        ((cur)->thread_info->thread_device_mode == THREAD_DEVICE_MODE_SLEEPY_END_DEVICE ||\
         (cur)->thread_info->thread_device_mode == THREAD_DEVICE_MODE_END_DEVICE ||\
         (cur)->thread_info->thread_device_mode == THREAD_DEVICE_MODE_FULL_END_DEVICE))

//int thread_configuration_save(thread_info_t *thread_info, link_configuration_s *leader_link_setup);

bool thread_leader_commissioner_create(thread_info_t *thread_info);
void thread_leader_commissioner_free(thread_info_t *thread_info);

bool thread_leader_data_parse(uint8_t *ptr, uint16_t dataLength, thread_leader_data_t *leaderDataBuf);
bool thread_connectivity_tlv_parse(uint8_t *ptr, uint16_t dataLength, thread_connectivity_t *connectivityTlv);

thread_leader_data_t *thread_leader_data_get(thread_info_t *info);
thread_parent_info_t *thread_parent_data_allocate(thread_info_t *info);
void thread_data_base_init(thread_info_t *thread_info, int8_t interfaceId);
int thread_info_allocate_and_init(protocol_interface_info_entry_t *cur);
void thread_info_deallocate(protocol_interface_info_entry_t *cur);
int thread_init(protocol_interface_info_entry_t *cur);
int thread_attach_ready(protocol_interface_info_entry_t *cur);
bool thread_attach_active_router(protocol_interface_info_entry_t *cur);
bool thread_scan_mask_validation(protocol_interface_info_entry_t *cur, uint8_t mask);
int thread_route_ready_to_leader(protocol_interface_info_entry_t *cur);
void thread_timer(protocol_interface_info_entry_t *cur, uint8_t ticks);
void thread_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t ticks);
bool thread_insist_that_mesh_isnt_a_link(const protocol_interface_info_entry_t *cur);
bool thread_addr_is_mesh_local(const uint8_t *addr, const protocol_interface_info_entry_t *cur);
bool thread_on_mesh_route_possible_add(thread_attach_device_mode_e threadMode);
bool thread_addr_is_mesh_local_16(const uint8_t *addr, const protocol_interface_info_entry_t *cur);
uint8_t *thread_addr_write_mesh_local_16(uint8_t ip_addr_out[16], uint16_t addr16, const thread_info_t *thread_info);
int thread_leader_ul16_address_get(thread_info_t *thread_info, uint8_t *address_ptr);
void thread_network_data_request_send(protocol_interface_info_entry_t *cur, uint8_t *requestDstAddress, bool delaydTrig);
uint16_t thread_network_data_generate_stable_set(protocol_interface_info_entry_t *cur, uint8_t *result_ptr);

void thread_set_active_router(protocol_interface_info_entry_t *cur, if_address_entry_t *address_entry, uint8_t *routerId);
uint8_t thread_get_router_count_from_route_tlv(mle_tlv_info_t *routeTlv);
void thread_reset_neighbour_info(protocol_interface_info_entry_t *cur, mle_neigh_table_entry_t *neighbour);

void thread_child_id_request_entry_clean(protocol_interface_info_entry_t *cur);
thread_pending_child_id_req_t *thread_child_id_request_entry_get(protocol_interface_info_entry_t *cur, uint8_t *euid64);
void thread_child_id_request_entry_remove(protocol_interface_info_entry_t *cur, thread_pending_child_id_req_t *entry);
thread_pending_child_id_req_t *thread_child_id_request_entry_get_from_the_list(protocol_interface_info_entry_t *cur);

thread_mcast_child_t *thread_child_mcast_entry_get(protocol_interface_info_entry_t *cur, const uint8_t *mcast_addr, const uint8_t *mac64);
void thread_registered_mcast_addr_entry_clean(protocol_interface_info_entry_t *cur);
thread_registered_mcast_addr_t *thread_registered_mcast_addr_entry_find(protocol_interface_info_entry_t *cur, const uint8_t *mcast_addr);
void thread_child_mcast_entries_remove(protocol_interface_info_entry_t *cur, const uint8_t *mac64);

uint8_t thread_leader_data_tlv_size(protocol_interface_info_entry_t *cur);
uint8_t *thread_leader_data_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur);
uint8_t *thread_address_registration_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur);
int thread_link_reject_send(protocol_interface_info_entry_t *interface, const uint8_t *ll64);
thread_leader_info_t *thread_allocate_and_init_leader_private_data(void);
thread_route_cost_t thread_link_quality_to_cost(thread_link_quality_e quality);
thread_route_cost_t thread_link_cost_sum(thread_route_cost_t a, thread_route_cost_t b);
thread_link_quality_e thread_link_margin_to_quality(thread_link_margin_t margin);
uint_fast8_t thread_sum_rx_path_cost_and_link_cost(uint8_t inMargim, uint8_t outMargin, uint8_t pathCost);
/*
 * Thread beacon payload handling functions
 */
int8_t thread_beacon_create_payload(struct protocol_interface_info_entry *cur);
uint8_t thread_beacon_indication(uint8_t *ptr, uint8_t len, protocol_interface_info_entry_t *cur);

int thread_network_data_propagation(struct protocol_interface_info_entry *cur, uint8_t *childUnicastAddress, bool fullList);

uint8_t thread_route_option_size(protocol_interface_info_entry_t *cur);
uint8_t *thread_route_option_write(protocol_interface_info_entry_t *cur, uint8_t *ptr);

/*Generic Thread TLV write*/
uint8_t *thread_connectivity_tlv_write(uint8_t *ptr, protocol_interface_info_entry_t *cur, uint8_t mode);

/*Network data TLV write*/
uint16_t thread_network_data_tlv_size(struct protocol_interface_info_entry *cur, bool fulllist);
uint8_t *thread_network_data_tlv_write(struct protocol_interface_info_entry *cur, uint8_t *ptr, bool fulllist);

/* Active and pending operational dataset write*/
uint8_t *thread_active_timestamp_write(protocol_interface_info_entry_t *cur, uint8_t *ptr);
uint16_t thread_active_operational_dataset_size(protocol_interface_info_entry_t *cur);
uint8_t *thread_active_operational_dataset_write(protocol_interface_info_entry_t *cur, uint8_t *ptr);
bool thread_active_operational_dataset_process(protocol_interface_info_entry_t *cur, uint8_t *ptr, uint16_t len, uint64_t dataset_timestamp);
uint8_t *thread_pending_timestamp_write(protocol_interface_info_entry_t *cur, uint8_t *ptr);
uint16_t thread_pending_operational_dataset_size(protocol_interface_info_entry_t *cur);
uint8_t *thread_pending_operational_dataset_write(protocol_interface_info_entry_t *cur, uint8_t *ptr);
bool thread_pending_operational_dataset_process(protocol_interface_info_entry_t *cur, uint64_t mle_pending_timestamp, uint8_t *ptr, uint16_t len);
/*Write optional thread leader data TLV if leader data is known*/
uint8_t thread_pending_timestamp_tlv_size(protocol_interface_info_entry_t *cur);
void thread_calculate_key_guard_timer(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration, bool is_init);
void thread_set_link_local_address(protocol_interface_info_entry_t *cur);
void thread_mcast_group_change(struct protocol_interface_info_entry *interface, struct if_group_entry *group, bool group_added);
void thread_old_partition_data_purge(protocol_interface_info_entry_t *cur);

#else // HAVE_THREAD

NS_DUMMY_DEFINITIONS_OK

#define thread_info(cur) ((thread_info_t *) NULL)
#define thread_am_router(cur) (false)
#define thread_am_host(cur) (false)

#define thread_addr_is_mesh_local(addr, cur) false
#define thread_addr_is_mesh_local_16(addr, cur) false
#define thread_timer(cur, ticks) ((void) 0)
#define thread_seconds_timer(cur, ticks) ((void) 0)
#define thread_insist_that_mesh_isnt_a_link(cur) false
#define thread_attach_ready(cur) 0
#define thread_info_allocate_and_init(cur) (-1)
#define thread_info_deallocate(cur) ((void)0)
#define thread_network_data_propagation(cur, childUnicastAddress, fullList) (void(-1))
#define thread_link_reject_send(interface, ll64) 0
#define thread_addr_is_mesh_local_16(addr, cur) false
#define thread_mcast_group_change(interface, group, group_added) ((void)0)
#endif // HAVE_THREAD

#endif /* LOWPAN_THREAD_H_ */
