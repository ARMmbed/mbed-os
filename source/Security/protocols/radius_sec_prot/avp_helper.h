/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef AVP_HELPER_H_
#define AVP_HELPER_H_

/*
 * RADIUS AVP helper functions
 *
 */

#define AVP_FIXED_LEN                            2    // type, length
#define AVP_VALUE_MAX_LEN                        253  // 255 - type field - length field

#define AVP_TYPE_USER_NAME_LEN(len)              (AVP_FIXED_LEN + len)
#define AVP_TYPE_NAS_IP_ADDRESS_LEN              (AVP_FIXED_LEN + 4)
#define AVP_TYPE_NAS_PORT_LEN                    (AVP_FIXED_LEN + 4)
#define AVP_TYPE_FRAMED_MTU_LEN                  (AVP_FIXED_LEN + 4)
#define AVP_TYPE_STATE_LEN                       (AVP_FIXED_LEN + 8)
//#define AVP_TYPE_VENDOR_SPECIFIC               26
#define AVP_TYPE_CALLED_STATION_ID_LEN(len)      (AVP_FIXED_LEN + len)
#define AVP_TYPE_CALLING_STATION_ID_LEN(len)     (AVP_FIXED_LEN + len)
#define AVP_TYPE_NAS_IDENTIFIER_LEN(len)         (AVP_FIXED_LEN + len)
#define AVP_TYPE_NAS_PORT_TYPE_LEN               (AVP_FIXED_LEN + 4)
#define AVP_TYPE_EAP_MESSAGE_LEN(len)            (AVP_FIXED_LEN + len)
#define AVP_TYPE_MESSAGE_AUTHENTICATOR_LEN       (AVP_FIXED_LEN + 16)
#define AVP_TYPE_NAS_IPV6_ADDRESS_LEN            (AVP_FIXED_LEN + 16)

// Wireless - IEEE 802.11
#define NAS_PORT_TYPE_WIRELESS_IEEE802_11        19

// EUI-64 in ascii string: 00-11-..-77
#define STATION_ID_LEN                           16 + 7

// MTU value is set by EAP-TLS fragment length
#define FRAMED_MTU                               EAP_TLS_FRAGMENT_LEN_VALUE

#define NAS_PORT                                 1

/**
 * avp_user_name_write write use name
 *
 * \param ptr pointer where to write
 * \param name_len name length
 * \param name name
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_user_name_write(uint8_t *ptr, const uint8_t name_len, const uint8_t *name);

/**
 * avp_nas_ip_address_write nas ip address
 *
 * \param ptr pointer where to write
 * \param addr address
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_nas_ip_address_write(uint8_t *ptr, uint32_t addr);

/**
 * avp_nas_port_write write nas port
 *
 * \param ptr pointer where to write
 * \param port nas port
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_nas_port_write(uint8_t *ptr, const uint32_t port);

/**
 * avp_framed_mtu_write write frame mtu
 *
 * \param ptr pointer where to write
 * \param mtu frame mtu
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_framed_mtu_write(uint8_t *ptr, const uint32_t mtu);

/**
 * avp_state_write write write state
 *
 * \param ptr pointer where to write
 * \param state_len state length
 * \param state state
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_state_write(uint8_t *ptr, const uint8_t state_len, const uint8_t *state);

/**
 * avp_called_station_id_write write called station id
 *
 * \param ptr pointer where to write
 * \param id_len identifier length
 * \param id identifier
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_called_station_id_write(uint8_t *ptr, const uint8_t id_len, const uint8_t *id);

/**
 * avp_calling_station_id_write write calling station id
 *
 * \param ptr pointer where to write
 * \param id_len identifier length
 * \param id identifier
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_calling_station_id_write(uint8_t *ptr, const uint8_t id_len, const uint8_t *id);

/**
 * avp_nas_identifier_write write nas identifier
 *
 * \param ptr pointer where to write
 * \param id_len nas identifier length
 * \param id nas identifier
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_nas_identifier_write(uint8_t *ptr, const uint8_t id_len, const uint8_t *id);

/**
 * avp_nas_port_type_write write nas port type
 *
 * \param ptr pointer where to write
 * \param port_type port type
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_nas_port_type_write(uint8_t *ptr, const uint32_t port_type);

/**
 * avp_nas_message_write write eap message
 *
 * \param ptr pointer where to write
 * \param eap_len eap length
 * \param eap eap frame
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_eap_message_write(uint8_t *ptr, const uint8_t eap_len, const uint8_t *eap);

/**
 * avp_message_authenticator_write write message authenticator
 *
 * \param ptr pointer where to write
 * \param auth authenticator
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_message_authenticator_write(uint8_t *ptr, const uint8_t *auth);

/**
 * avp_nas_ipv6_address_write write ipv6 address
 *
 * \param ptr pointer where to write
 * \param address ipv6 address
 *
 * return incremented write pointer
 *
 */
uint8_t *avp_nas_ipv6_address_write(uint8_t *ptr, const uint8_t *address);

/**
 * avp_eap_message_read read eap message
 *
 * \param ptr pointer to received message
 * \param len received message length
 * \param eap_len length of the eap frame
 *
 * return pointer to eap message or null
 *
 */
uint8_t *avp_eap_message_read(uint8_t *ptr, uint16_t len, uint8_t *eap_len);

/**
 * avp_message_authenticator_read read message authenticator
 *
 * \param ptr pointer to received message
 * \param len received message length
 *
 * return pointer to message authenticator or null
 *
 */
uint8_t *avp_message_authenticator_read(uint8_t *ptr, uint16_t len);

/**
 * avp_state_read read state
 *
 * \param ptr pointer to received message
 * \param len received message length
 * \param state_len length of the state
 *
 * return pointer to state or null
 *
 */
uint8_t *avp_state_read(uint8_t *ptr, uint16_t len, uint8_t *state_len);

/**
 * avp_vsa_ms_mppe_recv_key_read read vendor specific MS-MPPE-Recv-Key
 *
 * \param ptr pointer to received message
 * \param len received message length
 * \param recv_key_len length of the state
 *
 * return pointer to MS-MPPE-Recv-Key or null
 *
 */
uint8_t *avp_vsa_ms_mppe_recv_key_read(uint8_t *ptr, uint16_t len, uint8_t *recv_key_len);

#endif /* AVP_HELPER_H_ */
