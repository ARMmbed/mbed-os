/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include "common_functions.h"
#include "libDHCPv6/libDHCPv6.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"



uint32_t libdhcpv6_txid_get(void)
{
    return 0;
}

uint32_t libdhcpv6_renew_time_define(dhcpv6_client_server_data_t *addresInfo)
{
    return 0;
}

dhcpv6_client_server_data_t *libdhcvp6_nontemporalAddress_server_data_allocate(int8_t interfaceId, uint8_t instanceId, uint8_t *duiId, uint16_t duiLinkType, uint8_t *nonTemporalPrefix, uint8_t *serverIPv6Address)
{
    return NULL;
}

void libdhcvp6_nontemporalAddress_server_data_free(dhcpv6_client_server_data_t *removedEntry)
{
}

dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_instance(uint8_t instanceId)
{
    return NULL;
}
uint8_t libdhcpv6_nonTemporal_entry_get_unique_instance_id(void)
{
    return 0;
}

dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_iaid(uint32_t iaId)
{
    return NULL;
}

dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_transactionId(uint32_t txId)
{
    return NULL;
}

dhcpv6_client_server_data_t *libdhcpv6_nonTemporal_entry_get_by_prefix(int8_t interfaceId, uint8_t *prefix)
{
    return NULL;
}

uint16_t libdhcpv6_duid_option_size(uint16_t linkType)
{
    return 0;
}

uint16_t libdhcpv6_client_data_option_size(uint16_t linkType)
{
    return 0;
}

uint16_t libdhcvp6_request_option_size(uint8_t optionCnt)
{
    return 0;
}

uint16_t libdhcpv6_non_temporal_address_size(bool addressDefined)
{
    return 0;
}

int libdhcpv6_message_malformed_check(uint8_t *ptr, uint16_t data_len)
{
    return 0;
}

uint8_t *libdhcpv6_header_write(uint8_t *ptr, uint8_t msgType, uint32_t transActionId)
{
    return NULL;
}

uint8_t *libdhcpv6_elapsed_time_option_write(uint8_t *ptr, uint16_t elapsedTime)
{
    return NULL;
}

uint8_t *libdhcpv6_rapid_commit_option_write(uint8_t *ptr)
{
    return NULL;
}

uint8_t *libdhcvp6_vendor_specific_option_write(uint8_t *ptr, uint8_t *data, uint16_t dataLength)
{
    return NULL;
}

uint8_t *libdhcvp6_request_option_write(uint8_t *ptr, uint8_t optionCnt, uint16_t *optionPtr)
{
    return NULL;
}

uint8_t *libdhcpv6_duid_option_write(uint8_t *ptr, uint16_t duidRole, const dhcp_link_options_params_t *duid)
{
    return NULL;
}

uint8_t *libdhcpv6_prefix_delegation_info_option_write(uint8_t *ptr, uint32_t iaId)
{
    return NULL;
}

uint8_t *libdhcpv6_identity_association_option_write(uint8_t *ptr, uint32_t iaID, uint32_t TimerT1, uint32_t TimerT2, bool withAddress)
{
    return NULL;
}

uint8_t *libdhcpv6_ia_address_option_write(uint8_t *ptr, const uint8_t *addressPtr, uint32_t preferredValidLifeTime, uint32_t validLifeTime)
{
    return NULL;
}

uint8_t *libdhcpv6_status_code_write(uint8_t *ptr, uint16_t statusCode)
{
    return NULL;
}

uint8_t *libdhcpv6_client_last_transaction_time_option_write(uint8_t *ptr, uint32_t last_transaction_Time)
{
    return NULL;
}

int libdhcpv6_message_option_discover(uint8_t *ptr, uint16_t data_len, uint16_t discovered_type, dhcp_options_msg_t *option_info)
{
    return -1;
}

int libdhcpv6_compare_DUID(dhcp_link_options_params_t *targetId, dhcp_link_options_params_t *parsedId)
{
    return -1;
}

int libdhcpv6_reply_message_option_validate(dhcp_link_options_params_t *clientId, dhcp_link_options_params_t *serverId, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params, uint8_t *ptr, uint16_t data_length)
{
    return 0;
}

int libdhcpv6_advertisment_message_option_validate(dhcp_link_options_params_t *clientId, dhcp_link_options_params_t *serverId, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params, uint8_t *ptr, uint16_t data_length)
{
    return 0;
}

int libdhcpv6_renew_message_options_validate(uint8_t *ptr, uint16_t data_length, dhcp_link_options_params_t *clientLinkData, dhcp_link_options_params_t *serverLinkData, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params)
{
    return 0;
}

int libdhcpv6_solication_message_options_validate(uint8_t *ptr, uint16_t data_length, dhcp_link_options_params_t *clientLink, dhcp_ia_non_temporal_params_t *dhcp_ia_non_temporal_params)
{
    return 0;
}

bool libdhcpv6_time_elapsed_option_at_packet(uint8_t *ptr, uint16_t length)
{
    return false;
}

bool libdhcpv6_rapid_commit_option_at_packet(uint8_t *ptr, uint16_t length)
{
    return false;
}

int libdhcpv6_get_duid_by_selected_type_id_opt(uint8_t *ptr, uint16_t data_length, uint16_t type , dhcp_link_options_params_t *params)
{
    return -1;
}

int libdhcpv6_get_IA_address(uint8_t *ptr, uint16_t data_length, dhcp_ia_non_temporal_params_t *params)
{
    return -1;
}

uint16_t libdhcpv6_address_request_message_len(uint16_t clientLinkType, uint16_t serverLinkType, uint8_t requstOptionCnt)
{
    return 0;
}

uint16_t libdhcpv6_address_reply_message_len(uint16_t clientLinkType, uint16_t serverLinkType, uint16_t vendorDataLen, bool rapidCommon, bool status)
{
    return 0;
}

uint8_t *libdhcpv6_generic_nontemporal_address_message_write(uint8_t *ptr, dhcpv6_solication_base_packet_s *packet, dhcpv6_ia_non_temporal_address_s *nonTemporalAddress, dhcp_link_options_params_t *serverLink)
{
    return NULL;
}

uint8_t *libdhcpv6_reply_message_write(uint8_t *ptr, dhcpv6_reply_packet_s *replyPacket, dhcpv6_ia_non_temporal_address_s *nonTemporalAddress, dhcpv6_vendor_data_packet_s *vendorData)
{
    return NULL;
}

uint16_t libdhcpv6_solication_message_length(uint16_t clientLinkType, bool addressDefined, uint8_t requestOptionCount)
{
    return 0;
}

bool libdhcpv6_gua_server_list_empty(void)
{
    return true;
}
