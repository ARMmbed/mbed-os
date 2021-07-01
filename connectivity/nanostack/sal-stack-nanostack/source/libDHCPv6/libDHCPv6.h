/*
 * Copyright (c) 2014-2015, 2017-2021, Pelion and affiliates.
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

/*
 * \file libDHCPv6.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef LIBDHCPV6_H_
#define LIBDHCPV6_H_

#include "ns_list.h"

typedef struct dhcp_duid_options_params {
    uint16_t type;
    uint16_t duid_length;
    uint8_t *duid;
} dhcp_duid_options_params_t;

typedef struct {
    uint16_t link_type;
    uint32_t link_time;
    uint8_t *link_id;
} dhcp_link_options_params_t;

typedef struct {
    uint16_t type;
    uint16_t len;
    uint8_t *msg_ptr;
} dhcp_options_msg_t;

typedef struct {
    uint8_t *ipv6Address;
    uint32_t validLifeTime;
    uint32_t preferredLifeTime;
} dhcp_ia_addressoptions_msg_t;

typedef struct {
    uint8_t *requestedAddress;
    uint32_t validLifeTime;
    uint32_t preferredLifeTime;
} dhcpv6_ia_non_temporal_address_s;

typedef struct {
    dhcp_duid_options_params_t clientDUID;
    dhcp_duid_options_params_t serverDUID;
    uint32_t transaction_ID;
    uint32_t iaId;
    uint32_t T0;
    uint32_t T1;
    bool    rapidCommit;
} dhcpv6_reply_packet_s;

typedef struct {
    uint8_t *vendorData;
    uint16_t vendorDataLength;
} dhcpv6_vendor_data_packet_s;

typedef struct {
    uint8_t messageType;
    uint32_t transActionId;
    dhcp_duid_options_params_t clientDUID;
    uint32_t iaID;
    uint32_t timerT0;
    uint32_t timerT1;
    uint16_t *requestedOptionList;
    uint8_t  requestedOptionCnt;
} dhcpv6_solication_base_packet_s;

typedef struct {
    uint32_t iaId;
    uint32_t T0;
    uint32_t T1;
    uint8_t *nonTemporalAddress;
    uint32_t validLifeTime;
    uint32_t preferredValidLifeTime;
} dhcp_ia_non_temporal_params_t;

typedef struct dhcpv6_ia_nontemp_addres_entry_t {
    uint32_t    validLifetime;
    uint32_t    preferredTime;
    uint8_t     addressPrefix[16];
} dhcpv6_ia_nontemp_addres_entry_t;


typedef struct dhcpv6_client_server_entry_s {
    int8_t      interfaceId;
    uint8_t     instanceId; // instance identifying specific client
    uint8_t     dyn_server_duid_length;
    bool        useServerAddress; //This indicate
    bool        iaNonTemporalStructValid;
    bool        GlobalAddress;
    uint8_t     server_address[16];
    uint8_t     *serverDynamic_DUID;
    uint32_t    T0;
    uint32_t    T1;
    uint32_t    IAID; //Take random for that
    uint32_t    transActionId; //Client save this and use for get
    uint32_t    reNewTimer;
    dhcp_duid_options_params_t clientDUID;
    dhcp_duid_options_params_t serverDUID;
    dhcpv6_ia_nontemp_addres_entry_t iaNontemporalAddress; // Dynamical Part
    ns_list_link_t      link;                   /*!< List link entry */
} dhcpv6_client_server_data_t;

typedef NS_LIST_HEAD(dhcpv6_client_server_data_t, link) dhcpv6_client_server_entry_s;

typedef struct dhcpv6_relay_msg {
    uint8_t    type;
    uint8_t    hop_limit;
    uint8_t    *link_address;
    uint8_t    *peer_address;
    dhcp_options_msg_t relay_interface_id;
    dhcp_options_msg_t relay_options;
} dhcpv6_relay_msg_t;

/**  UDP Port Number definition */
#define DHCPV6_SERVER_PORT 547
#define DHCPV6_CLIENT_PORT 546

/** Message type Definition  */
#define DHCPV6_SOLICATION_TYPE          1
#define DHCPV6_ADVERTISMENT_TYPE        2
#define DHCPV6_REQUEST_TYPE             3
#define DHCPV6_RENEW_TYPE               5
#define DHCPV6_REPLY_TYPE               7
#define DHCPV6_RELEASE_TYPE             8
#define DHCPV6_RELAY_FORWARD            12
#define DHCPV6_RELAY_REPLY              13
#define DHCPV6_LEASEQUERY_TYPE          14
#define DHCPV6_LEASEQUERY_REPLY_TYPE    15

/* DHCP timeout values for different messages RFC3315
 * LQ from RFC5007
 * UPDATE from RFC7083
 *
 * Timers are 16-bit, and in 1/10s ticks, so times must be <=6553
 */

#define SOL_MAX_DELAY     1 //sec   Max delay of first Solicit
#define SOL_TIMEOUT       1 //sec   Initial Solicit timeout
#define SOL_MAX_RT     3600 //secs  Max Solicit timeout value
#define REQ_TIMEOUT       1 //sec   Initial Request timeout
#define REQ_MAX_RT       30 //secs  Max Request timeout value
#define REQ_MAX_RC       10 //      Max Request retry attempts
#define CNF_MAX_DELAY     1 //sec   Max delay of first Confirm
#define CNF_TIMEOUT       1 //sec   Initial Confirm timeout
#define CNF_MAX_RT        4 //secs  Max Confirm timeout
#define CNF_MAX_RD       10 //secs  Max Confirm duration
#define REN_TIMEOUT      10 //secs  Initial Renew timeout
#define REN_MAX_RT      600 //secs  Max Renew timeout value
#define REB_TIMEOUT      10 //secs  Initial Rebind timeout
#define REB_MAX_RT      600 //secs  Max Rebind timeout value
#define INF_MAX_DELAY     1 //sec   Max delay of first Information-request
#define INF_TIMEOUT       1 //sec   Initial Information-request timeout
#define INF_MAX_RT      120 //secs  Max Information-request timeout value
#define REL_TIMEOUT       1 //sec   Initial Release timeout
#define REL_MAX_RC        5 //      MAX Release attempts
#define DEC_TIMEOUT       1 //sec   Initial Decline timeout
#define DEC_MAX_RC        5 //      Max Decline attempts
#define REC_TIMEOUT       2 //secs  Initial Reconfigure timeout
#define REC_MAX_RC        8 //      Max Reconfigure attempts
#define HOP_COUNT_LIMIT  32 //      Max hop count in a Relay-forward message
#define LQ_TIMEOUT        1 //sec   Initial LEASEQUERY timeout
#define LQ_MAX_RT        10 //secs  Max LEASEQUERY timeout value
#define LQ_MAX_RC         5 //      Max LEASEQUERY retry attempts



#define DHCP_MESSGAGE_MAX_SUPPORTED_OPTIONS 8

#define DHCPV6_ELAPSED_TIME_OPTION 0x0008
#define DHCPV6_ELAPSED_TIME_OPTION_LEN 0x0002
/** Client Identifier part */
#define DHCPV6_CLIENT_ID_OPTION 0x001
#define DHCPV6_CLIENT_ID_OPTION_LEN 0x000c
/** Client Identifier part End */
/** Server Identifier Part */
#define DHCPV6_SERVER_ID_OPTION 0x002
#define DHCPV6_SERVER_ID_OPTION_LEN 0x000c
#define DHCPV6_SERVER_ID_MAC48_OPTION_LEN 0x000a
#define DHCPV6_SERVER_ID_MAC64_OPTION_LEN 0x000c
/** Server Identifier END */
/** Common for server and Client Identifier option */
#define DHCPV6_DUID_LINK_LAYER_PLUS_TIME_TYPE 0x0001
#define DHCPV6_DUID_EN_TYPE 0x0002
#define DHCPV6_DUID_LINK_LAYER_TYPE 0x0003
#define DHCPV6_DUID_UUID_TYPE 0x0004
#define DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE 0x0006
#define DHCPV6_DUID_HARDWARE_EUI64_TYPE 0x001b
#define DHCPV6_DUID_HARDWARE_EUI48_TYPE 0x0001

/** Identity Association part */
#define DHCPV6_IDENTITY_ASSOCIATION_OPTION 0x0003
#define DHCPV6_IDENTITY_ASSOCIATION_OPTION_MIN_LEN 0x000c

#define DHCPV6_IA_ADDRESS_OPTION 0x0005
#define DHCPV6_IA_ADDRESS_OPTION_LEN 0x0018

#define DHCPV6_OPTION_REQUEST_OPTION 0x0006

#define DHCPV6_OPTION_IA_PREFIX_DELEGATION 0x001a
#define DHCPV6_OPTION_IA_PREFIX_DELEGATION_MIN_LENGTH 0x000c
/** Identity Association END */

#define DHCPV6_OPTION_VENDOR_CLASS 0x0010
#define DHCPV6_OPTION_VENDOR_SPECIFIC_INFO 0x0011
/** SEQUENCYID, RouterIDMask 32-bit*/

#define DHCPV6_OPTION_DNS_SERVERS         0x0017
#define DHCPV6_OPTION_DOMAIN_LIST         0x0018

#define DHCPV6_STATUS_CODE_OPTION 0x000d
#define DHCPV6_STATUS_CODE_OPTION_LEN 0x0002
#define DHCPV6_STATUS_NO_ADDR_AVAILABLE_CODE 0x0002

#define DHCPV6_OPTION_RAPID_COMMIT 0x000E //Define Reply for Solication
#define DHCPV6_OPTION_RAPID_COMMIT_LEN 0x0000

#define DHCPV6_OPTION_CLIENT_DATA 0x002d

#define DHCPV6_OPTION_CLT_TIME 0x002e

#define DHCPV6_RELAY_LENGTH 34
#define DHCPV6_OPTION_RELAY 0x0009
#define DHCPV6_OPTION_INTERFACE_ID 0x0012



/** DHCPv6 client Nontemporal address and server data allocate, free and search */
dhcpv6_client_server_data_t *libdhcvp6_nontemporalAddress_server_data_allocate(int8_t interfaceId, uint8_t instanceId, uint8_t *nonTemporalPrefix, uint8_t *serverIPv6Address);
void libdhcvp6_nontemporalAddress_server_data_free(dhcpv6_client_server_data_t *removedEntry);

uint32_t libdhcpv6_renew_time_define(dhcpv6_client_server_data_t *addresInfo);
uint32_t libdhcpv6_txid_get(void);
dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_iaid(uint32_t iaId);
dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_transactionId(uint32_t txId);
dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_prefix(int8_t interfaceId, uint8_t *prefix);
dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_instance(uint8_t instanceId);
dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_validate_class_pointer(void *class_ptr);
uint8_t libdhcpv6_nonTemporal_entry_get_unique_instance_id(void);


/**
 * This Function for verify dhcpv6 message payload
 *
 * \param ptr payload pointer
 * \param data_len payload length
 *
 * return 0 Valid Payload structure
 * return <0 Maloformed payload
 */
int libdhcpv6_message_malformed_check(uint8_t *ptr, uint16_t data_len);

int libdhcpv6_message_option_discover(uint8_t *ptr, uint16_t data_len, uint16_t discovered_type, dhcp_options_msg_t *option_info);

/** Static Option sizes */
#define libdhcpv6_header_size() 4
#define libdhcpv6_ia_address_option_size() 28
#define libdhcpv6_elapsed_time_option_size() 6
#define libdhcpv6_rapid_commit_option_size() 4
#define libdhcvp6_thread_vendor_spesific_option_size() 21
#define libdhcpv6_status_option_size() 6
#define libdhcpv6_client_last_transaction_time_option_size() 8

/** Dynamic Option lengths */
uint16_t libdhcpv6_duid_option_size(uint16_t duidLength);
uint8_t libdhcpv6_duid_linktype_size(uint16_t linkType);
uint16_t libdhcvp6_request_option_size(uint8_t optionCnt);
uint16_t libdhcpv6_non_temporal_address_size(bool addressDefined);
uint16_t libdhcpv6_vendor_option_size(uint16_t vendor_data_length);

uint16_t libdhcpv6_solication_message_length(uint16_t clientDUIDLength, bool addressDefined, uint8_t requestOptionCount);
uint16_t libdhcpv6_address_request_message_len(uint16_t clientDUIDLength, uint16_t serverDUIDLength, uint8_t requstOptionCnt, bool add_address);
#ifdef HAVE_DHCPV6_SERVER
uint16_t libdhcpv6_address_reply_message_len(uint16_t clientDUIDLength, uint16_t serverDUIDLength, uint16_t vendorDataLen, bool rapidCommon, bool status);
#else
#define libdhcpv6_address_reply_message_len(clientDUIDLength, serverDUIDLength, vendorDataLen, rapidCommon, status) 0
#endif

uint8_t *libdhcpv6_generic_nontemporal_address_message_write(uint8_t *ptr, dhcpv6_solication_base_packet_s *packet, dhcpv6_ia_non_temporal_address_s *nonTemporalAddress, dhcp_duid_options_params_t *serverLink);
uint8_t *libdhcpv6_dhcp_relay_msg_write(uint8_t *ptr, uint8_t type, uint8_t hop_limit,  uint8_t *peer_addres, uint8_t *link_address);
uint8_t *libdhcpv6_dhcp_option_header_write(uint8_t *ptr, uint16_t option_type, uint16_t length);

int libdhcpv6_get_IA_address(uint8_t *ptr, uint16_t data_length, dhcp_ia_non_temporal_params_t *params);
int libdhcpv6_get_duid_by_selected_type_id_opt(uint8_t *ptr, uint16_t data_length, uint16_t type, dhcp_duid_options_params_t *params);
bool libdhcpv6_duid_length_validate(uint16_t duid_type, uint16_t duid_length);
int libdhcpv6_get_link_address_from_duid(uint8_t *ptr, uint16_t data_length, uint16_t type, dhcp_link_options_params_t *params);
int libdhcpv6_compare_DUID(dhcp_duid_options_params_t *targetId, dhcp_duid_options_params_t *parsedId);

/**
 * This Function write dhcpv6 basic header
 *
 * \param ptr pointer where header will be writed
 * \param msgType dhcpv6 message type
 * \param transActionId 24-bit unique Transaction ID
 *
 * return incremented pointer after write
 */
uint8_t *libdhcpv6_header_write(uint8_t *ptr, uint8_t msgType, uint32_t transActionId);

/**
 * This Function write dhcpv6 elapsed time option
 *
 * \param ptr pointer where option will be writed
 * \param elapsedTime option elapsed time
 *
 * return incremented pointer after write
 */
uint8_t *libdhcpv6_elapsed_time_option_write(uint8_t *ptr, uint16_t elapsedTime);

/**
 * This Function write dhcpv6 rapid common option
 *
 * \param ptr pointer where option will be writed
 *
 * return incremented pointer after write
 */
uint8_t *libdhcpv6_rapid_commit_option_write(uint8_t *ptr);

/**
 * This Function write dhcpv6 request option write
 *
 * \param ptr pointer where option will be writed
 * \param optionCnt Option counter for read behind pointer
 * \param optionPtr Option list pointer
 * return incremented pointer after write
 */
uint8_t *libdhcvp6_request_option_write(uint8_t *ptr, uint8_t optionCnt, uint16_t *optionPtr);

uint8_t *libdhcpv6_option_interface_id_write(uint8_t *ptr, int8_t interface_id);



/**
 * This Function for Client / Server DUID write for specific link type
 *
 * \param ptr payload pointer
 * \param duidPtr pointer id
 * \param duidRole supported values DHCPV6_SERVER_ID_OPTION & DHCPV6_CLIENT_ID_OPTION
 * \param linkType supported values DHCPV6_DUID_HARDWARE_EUI64_TYPE & DHCPV6_DUID_HARDWARE_EUI48_TYPE & DHCPV6_DUID_HARDWARE_IEEE_802_NETWORKS_TYPE
 *
 * return incremented pointer after write
 */
uint8_t *libdhcpv6_duid_option_write(uint8_t *ptr, uint16_t duidRole, const dhcp_duid_options_params_t *duid);
uint8_t *libdhcpv6_ia_address_option_write(uint8_t *ptr, const uint8_t *addressPtr, uint32_t preferredValidLifeTime, uint32_t validLifeTime);
uint8_t *libdhcpv6_identity_association_option_write(uint8_t *ptr, uint32_t iaID, uint32_t TimerT1, uint32_t TimerT2, bool withAddress);
uint8_t *libdhcpv6_identity_association_option_write_with_status(uint8_t *ptr, uint32_t iaID, uint32_t TimerT1, uint32_t TimerT2, uint16_t status);
uint8_t *libdhcpv6_status_code_write(uint8_t *ptr, uint16_t statusCode);
uint8_t *libdhcpv6_prefix_delegation_info_option_write(uint8_t *ptr, uint32_t iaId);

int libdhcpv6_reply_message_option_validate(dhcp_duid_options_params_t *clientId, dhcp_duid_options_params_t *serverId, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params, uint8_t *ptr, uint16_t data_length);

#ifdef HAVE_DHCPV6_SERVER
int libdhcpv6_renew_message_options_validate(uint8_t *ptr, uint16_t data_length, dhcp_duid_options_params_t *clientLinkData, dhcp_duid_options_params_t *serverLinkData, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params);
int libdhcpv6_solication_message_options_validate(uint8_t *ptr, uint16_t data_length, dhcp_duid_options_params_t *clientLink, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params);
#else
#define libdhcpv6_renew_message_options_validate(ptr, data_length, clientLinkData, serverLinkData, dhcp_ia_non_temporal_params) -1
#define libdhcpv6_solication_message_options_validate(ptr, data_length, clientLink, dhcp_ia_non_temporal_params) -1
#endif
int libdhcpv6_advertisment_message_option_validate(dhcp_duid_options_params_t *clientId, dhcp_duid_options_params_t *serverId, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params, uint8_t *ptr, uint16_t data_length);
bool libdhcpv6_rapid_commit_option_at_packet(uint8_t *ptr, uint16_t length);
bool libdhcpv6_time_elapsed_option_at_packet(uint8_t *ptr, uint16_t length);

bool libdhcpv6_relay_msg_read(uint8_t *ptr, uint16_t length, dhcpv6_relay_msg_t *relay_msg);
#endif /* LIBDHCPV6_H_ */
