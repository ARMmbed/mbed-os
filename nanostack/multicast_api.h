/*
 * Copyright (c) 2012-2018, Arm Limited and affiliates.
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
#ifndef MULTICAST_API_H_
#define MULTICAST_API_H_
/**
 * \file multicast_api.h
 * \brief Multicast Trickle Forwarding API.
 * \section multi-init Init API:
 * - multicast_set_parameters(), Set trickle parameters.
 * \section multi-cnf Configure API:
 * - multicast_add_address(), Add new address to a multicast group and control trickle forwarding.
 * - multicast_free_address(), Remove supported multicast address from list.
 *
 * \section ZigBeeIP Trickle Setups for Multicast Init
 *
 * | Parameter         | VALUE |
 * | :---------------: | :---: |
 * | imin              | 10    |
 * | imax              | 0     |
 * | k                 | 20    |
 * | timer_expirations | 3     |
 * | window_expiration | 75    |
 *
 */

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Set new parameters for trickle multicast.
 * \deprecated
 *
 * \param i_min Minimum trickle timer interval in 50ms resolution: Imin = i_min * 50ms.
 * \param i_doublings Maximum trickle timer interval expressed as number of doublings of the minimum interval.
 * \param k Redundancy constant.
 * \param timer_expirations Number of trickle timer expirations before terminating the trickle process.
 * \param window_expiration The time window for keeping the state after the end of trickle process in 50ms resolution.
 *        NOTE: If window_expiration value is set too small an infinite retransmission loop may occur when using the trickle multicast.
 */
void multicast_set_parameters(uint8_t i_min, uint8_t i_doublings, uint8_t k, uint8_t timer_expirations, uint16_t window_expiration);


/**
 * \brief Add new address to multicast group.
 *
 * \deprecated Use setsockopt(SOCKET_IPV6_JOIN_GROUP) to join a group. Use
 * multicast_mpl_subscribe_domain() to control MPL behaviour.
 *
 * \param address_ptr Pointer to a 16-byte array that includes the address to be added.
 * \param use_trickle 0 = no trickle multicast forwarding, all other values = trickle multicast forwarding will be used with this address.
 *
 * \return 0 General error.
 * \return 1 Address updated.
 * \return 2 Address added.
 * \return 255 Link local not allowed when using multicast.
*
 */
uint8_t multicast_add_address(const uint8_t *address_ptr, uint8_t use_trickle);



/**
 * \brief Free address from multicast group.
 * \deprecated Use setsockopt(SOCKET_IPV6_LEAVE_GROUP)
 *
 * \param address_ptr Pointer to a 16-byte array that includes the address to be removed.
 *
 * \return 0 will be returned on successful execution, other values indicate an error on removing the address.
 */
uint8_t multicast_free_address(const uint8_t *address_ptr);

/**
 * \brief Treat all Realm-local Domains as one
 *
 * If enabled on an interface, then all Realm-local scope multicasts will
 * be treated as if belonging to the All-MPL-Forwarders domain, rather than
 * the domain indicated by their destination address. This is non-standard
 * behaviour, not covered by the real MPL specification, but required by the
 * Thread specification.
 *
 * All devices in a realm should have this setting set the same. With the
 * setting on, reactive forwarding and control messages will not work.
 *
 * With this setting on, Realm-scope domains other than All-MPL-Forwarders
 * cannot be joined.
 *
 * This must be set before bringing up an interface, and then not be modified.
 *
 * \param interface_id interface id
 * \param enable true to enable domain unification
 *
 * \return 0 for success, negative on failure
 */
int_fast8_t multicast_mpl_treat_realm_domains_as_one(int8_t interface_id, bool enable);

/**
 * \brief Set default MPL Domain parameters
 *
 * Modifies the default parameters for MPL Domains using the specified
 * interface.
 *
 * This must be set before subscribing to any MPL Domains with default
 * parameters on that interface.
 *
 * Initial defaults are as specified by RFC 7731.
 *
 * \param interface_id interface id
 * \param proactive_forwarding              true to forward Data Messages when first received (default true)
 * \param seed_set_entry_lifetime           minimum seed set lifetime (seconds, default 1800 = 30 minutes)
 * \param data_message_imin                 minimum Trickle timer for Data Messages (ms, default ~= 10 * expected link latency)
 * \param data_message_imax                 maximum Trickle timer for Data Messages (ms, default = Imin)
 * \param data_message_k                    Trickle redundancy constant for Data Messages (default = 1)
 * \param data_message_timer_expirations    controls termination of retransmissions (default = 3)
 * \param control_message_imin              minimum Trickle timer for Control Messages (ms, default ~= 10 * worst-case link latency)
 * \param control_message_imax              maximum Trickle timer for Control Messages (ms, default = 5 minutes)
 * \param control_message_k                 Trickle redundancy constant for Control Messages (default = 1)
 * \param control_message_timer_expirations controls termination of retransmissions (default = 10); 0 disables control messages
 *
 * \return 0 for success, negative on failure
 */
int_fast8_t multicast_mpl_set_default_parameters(int8_t interface_id,
                                                 bool proactive_forwarding,
                                                 uint16_t seed_set_entry_lifetime,
                                                 uint32_t data_message_imin,
                                                 uint32_t data_message_imax,
                                                 uint8_t data_message_k,
                                                 uint8_t data_message_timer_expirations,
                                                 uint32_t control_message_imin,
                                                 uint32_t control_message_imax,
                                                 uint8_t control_message_k,
                                                 uint8_t control_message_timer_expirations);

/** Control selection of MPL Seed Identifier for packets we originate */
typedef enum {
    MULTICAST_MPL_SEED_ID_DEFAULT = -256,               /** Default selection (used to make a domain use the interface's default) */
    MULTICAST_MPL_SEED_ID_MAC_SHORT = -1,               /** Use short MAC address if available (eg IEEE 802.15.4 interface's macShortAddress (16-bit)), else full MAC */
    MULTICAST_MPL_SEED_ID_MAC = -2,                     /** Use MAC padded to 64-bit (eg IEEE 802.15.4 interface's macExtendedAddress, or 48-bit Ethernet MAC followed by 2 zero pad bytes) */
    MULTICAST_MPL_SEED_ID_IID_EUI64 = -3,               /** Use 64-bit IPv6 IID based on EUI-64 (eg 02:11:22:ff:fe:00:00:00 for an Ethernet interface with MAC 00:11:22:00:00:00) */
    MULTICAST_MPL_SEED_ID_IID_SLAAC = -4,               /** Use 64-bit IPv6 IID that would be used for SLAAC */
    MULTICAST_MPL_SEED_ID_IPV6_SRC_FOR_DOMAIN = 0,      /** Use IPv6 source address selection to choose 128-bit Seed ID based on MPL Domain Address as destination */
    MULTICAST_MPL_SEED_ID_16_BIT = 2,                   /** Use a manually-specified 16-bit ID */
    MULTICAST_MPL_SEED_ID_64_BIT = 8,                   /** Use a manually-specified 64-bit ID */
    MULTICAST_MPL_SEED_ID_128_BIT = 16,                 /** Use a manually-specified 128-bit ID */
} multicast_mpl_seed_id_mode_e;

/**
 * \brief Set default MPL Seed Identifier
 *
 * Sets the default MPL Seed Identifier used when acting as an MPL Seed.
 *
 * \param interface_id interface id
 * \param seed_id_mode Seed ID selection mode
 * \param seed_id For positive (constant) types, pointer to Seed ID data
 *
 * \return 0 for success, negative on failure
 */
int_fast8_t multicast_mpl_set_default_seed_id(int8_t interface_id,
                                              multicast_mpl_seed_id_mode_e seed_id_mode,
                                              const void *seed_id);

/**
 * \brief Subscribe to an MPL Domain (RFC 7731)
 *
 * This subscribes to an MPL Domain with default parameters on the
 * specified interface. (At present there is no support for subscribing to
 * the same Domain on multiple interfaces)
 *
 * If the ALL_MPL_FORWARDERS Domain ff03::fc has not already been subscribed
 * to, this will automatically also subscribe to it with default parameters.
 *
 * Once 1 or more MPL Domains have been subscribed to on an interface,
 * multicast transmissions sent to a group with scope greater than link-local
 * on that interface will be sent using MPL.
 *
 * If the destination corresponds to a subscribed MPL Domain, it will be sent
 * to that MPL Domain (and hence forwarded only by other subscribers to that
 * domain).
 *
 * If the destination does not correspond to a subscribed MPL Domain, it will
 * be tunnelled, with the outer IP packet sent to the ALL_MPL_FORWARDERS Domain.
 *
 * Typical behaviour for ZigBee IP and Thread networks would be achieved by
 * subscribing to ff03::1, and enabling realm auto-subscription - [this is
 * done automatically when calling multicast_add_address?]
 *
 * \param interface_id interface id
 * \param address MPL Domain Address (IPv6 multicast address)
 * \param seed_id_mode Seed ID selection mode
 * \param seed_id For positive (constant) types, pointer to Seed ID data
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_mpl_domain_subscribe(int8_t interface_id,
                                      const uint8_t address[16],
                                      multicast_mpl_seed_id_mode_e seed_id_mode,
                                      const void *seed_id);

/**
 * \brief Subscribe to an MPL Domain (RFC 7731)
 *
 * This subscribes to an MPL Domain with default parameters on the
 * specified interface.
 *
 * If the ALL_MPL_FORWARDERS Domain ff03::fc has not already been subscribed
 * to, this will automatically also subscribe to it with default parameters.
 *
 * See multicast_mpl_subscribe_domain and multicast_set_default_parameters
 * for more information on parameters.
 */
int8_t multicast_mpl_domain_subscribe_with_parameters
(int8_t interface_id,
 const uint8_t address[16],
 multicast_mpl_seed_id_mode_e seed_id_mode,
 const void *seed_id,
 bool proactive_forwarding,
 uint16_t seed_set_entry_lifetime,
 uint32_t data_message_imin,
 uint32_t data_message_imax,
 uint8_t data_message_k,
 uint8_t data_message_timer_expirations,
 uint32_t control_message_imin,
 uint32_t control_message_imax,
 uint8_t control_message_k,
 uint8_t control_message_timer_expirations);

/**
 * \brief Unsubscribe from an MPL Domain
 *
 * This subscribes from a previously-subscribed MPL Domain
 *
 * \param interface_id interface id
 * \param address MPL Domain Address (IPv6 multicast address)
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_mpl_domain_unsubscribe(int8_t interface_id,
                                        const uint8_t address[16]);

/**
 * \brief Set interface's master multicast forwarding control
 *
 * Multicast forwarding can occur between interfaces with multicast forwarding
 * enabled, according to forwarding records and scope rules controlled by the
 * functions below.
 *
 * If this call disables forwarding on the current proxying upstream interface,
 * proxying is disabled, and would need to be manually re-enabled later.
 *
 * Multicast forwarding state for an interface - forwarding records and scope level - are
 * retained even if its master forwarding switch is disabled, enabling the upkeep of backup
 * information.
 *
 * By default this flag is enabled.
 *
 * \param interface_id interface id
 * \param enable Enable / Disable multicast forwarding
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_fwd_set_forwarding(int8_t interface_id, bool enable);

/**
 * \brief Add multicast forwarding record to an interface
 *
 * This adds a group to the forwarding list on the specified interface.
 * Received multicast packets for the specified group will be forwarded onto
 * the specified interface from other interfaces, if multicast forwarding is enabled on
 * both incoming and outgoing interfaces, subject to a Reverse Path Forwarding
 * check on the source address, and usual scope rules.
 *
 * If a finite lifetime is set, the record will be removed after that many seconds.
 *
 * If an entry for the specified group already exists, its lifetime will
 * increased if it is lower than the value passed.
 *
 * \param interface_id interface id
 * \param group IPv6 multicast group address
 * \param lifetime The time in seconds to maintain the forwarding entry - 0xFFFFFFFF means infinite
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_fwd_add(int8_t interface_id, const uint8_t group[16], uint32_t lifetime);

/**
 * \brief Remove multicast forwarding record from an interface
 *
 * Delete a a group from the forwarding list on the specified interface.
 *
 * \param interface_id interface id
 * \param group IPv6 multicast group address
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_fwd_remove(int8_t interface_id, const uint8_t group[16]);

/**
 * \brief Set full multicast forwarding
 *
 * If enabled, all multicast packets of specified scope or greater will be
 * forwarded onto the specified interface from other interfaces, if multicast
 * forwarding is enabled on both incoming and outgoing interfaces, subject to a
 * Reverse Path Forwarding check on the source address.
 *
 * Setting this is equivalent to "multicast_fwd_add" being called for all
 * addresses with scope >= min_scope.
 *
 * This functionality is disabled by setting min_scope to 0x10 or greater (so
 * no packets can match).
 *
 * By default this is set to 0x5 (site-local) for all interfaces.
 *
 * \param interface_id interface id
 * \param min_scope minimum IPv6 scope value for forwarding (see RFC 4291)
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_fwd_full_for_scope(int8_t interface_id, uint_fast8_t min_scope);

/**
 * \brief Set upstream interface for MLD proxying
 *
 * This sets the upstream interface for MLD proxying. If set, the stack will
 * report group membership on that interface according to the forwarding lists
 * of the other interfaces with multicast forwarding enabled (ie it will send
 * MLD reports or equivalent on that upstream interface).
 *
 * Multicast forwarding must be enabled on the upstream interface.
 *
 * \param interface_id interface id, or -1 to disable
 *
 * \return 0 for success, negative on failure
 */
int8_t multicast_fwd_set_proxy_upstream(int8_t interface_id);

#ifdef __cplusplus
}
#endif
#endif /* MULTICAST_API_H_ */
