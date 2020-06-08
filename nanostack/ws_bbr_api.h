/*
 * Copyright (c) 2017-2019, Arm Limited and affiliates.
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
 * \file ws_bbr_api.h
 * \brief Wi-SUN backbone border router (BBR) application interface.
 *
 * This is Wi-SUN backbone Border router service.
 * When started the module takes care of starting the
 * components that enables default border router functionality in Wi-SUN network.
 *
 */

#ifndef WS_BBR_API_H_
#define WS_BBR_API_H_

#include "ns_types.h"

/**
 * \brief Struct ws_statistics Border router dynamic information.
 */
typedef struct bbr_information {
    /** Timestamp of the the device. Can be used as version number*/
    uint64_t timestamp;
    /** Border router dodag id */
    uint8_t dodag_id[16];
    /** Address prefix given to devices in network  set to 0 if not available*/
    uint8_t prefix[8];
    /** Address IID of the border router set to 0 if not available*/
    uint8_t IID[8];
    /** Amount of devices in the network. */
    uint16_t devices_in_network;
    /** Border router instance identifier defined in RPL */
    uint8_t instance_id;
    /** RPL version number */
    uint8_t version;
} bbr_information_t;

/**
 * \brief Struct route_info is parent child relation structure.
 */
typedef struct bbr_route_info {
    /** IID of target device public IPv6 address can be formed by combining prefix + IID*/
    uint8_t target[8];
    /** IID of parent*/
    uint8_t parent[8];
} bbr_route_info_t;

/**
 * Start backbone border router service.
 *
 * if backbone interface is enabled and allows routing.
 *    Enables ND proxy for address found from backbone
 *
 * \param interface_id Wi-SUN network interface id.
 * \param backbone_interface_id backbone interface id.
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int ws_bbr_start(int8_t interface_id, int8_t backbone_interface_id);

/**
 * Border router configuration options
 */
#define BBR_ULA_C                 0x0001 /**< Static ULA prefix created automatically */
#define BBR_GUA_ROUTE             0x0002 /**< More specific route is added for GUA prefix */
#define BBR_BB_WAIT               0x0004 /**< Wait backbone availability before starting Wi-SUN network */
#define BBR_DEFAULT_ROUTE         0x0008 /**< Add default route parameter to DIO */
#define BBR_REQUIRE_DAO_REFRESH   0x0010 /**< Do not increment PAN version number when active forces DAO update from nodes*/

/**
 * Configure border router features.
 *
 * \param interface_id interface ID of the Wi-SUN network
 * \param options Options configured to Border router
 *          BBR_ULA_C     Configure Mesh local ULA prefix with SLAAC address
 *          BBR_GUA_ROUTE Add more specific route for GUA
 *          BBR_BB_WAIT   Start Wi-SUN network only when backbone is ready
 *
 * By default Wi-SUN network is started and is treated as separate interface even if backbone is not available.
 *
 * Default route RPL options when backbone is set up. RPL root is always Grounded
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int ws_bbr_configure(int8_t interface_id, uint16_t options);
/**
 * Stop backbone Border router.
 *
 * \param interface_id interface ID of the Wi-SUN network
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
void ws_bbr_stop(int8_t interface_id);

/**
 * Get border router information
 *
 * \param interface_id interface ID of the Wi-SUN network
 * \param info_ptr Structure given to stack where information is stored
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int ws_bbr_info_get(int8_t interface_id, bbr_information_t *info_ptr);

/**
 * Routing table get
 *
 * Table is Parent child relation using the Global address IID of the devices
 * To get the full IPv6 address of the device.
 *  IPv6 =  Global Prefix + IID.
 *
 * Routing table is in the format: 18 bytes per entry
 * | Node IID 8 bytes   | parent IID 8 bytes |
 * | 1122112211221122   | 1111111111111111   |
 * | 1133113311331133   | 1111111111111111   |
 * | 1144114411441144   | 1111111111111111   |
 * | 1155115511551155   | 1122112211221122   |
 * | 1166116611661166   | 1122112211221122   |
 * | 1177117711771177   | 1155115511551155   |
 * | 1188118811881188   | 1177117711771177   |
 *
 * Order is not assured only parent child link is given in random order
 *
 * Return value is device amount in network divided by 16 bytes per route entry
 *
 * \param interface_id interface ID of the Wi-SUN network
 * \param table_ptr Application allocated memory block where routing table is written.
 * \param table_len Length of the table allocated by application given as amount of entries.
 *
 * \return 0 - x on success indicates amount of bytes written to the table_ptr
 * \return <0 in case of errors
 *
 */
int ws_bbr_routing_table_get(int8_t interface_id, bbr_route_info_t *table_ptr, uint16_t table_len);

/**
 * Remove node's keys from border router
 *
 * Removes node's keys from border router i.e. Pairwise Master Key (PMK)
 * and Pairwise Transient Key (PTK). This function is used on revocation of
 * node's access procedure after authentication service is configured
 * to reject authentication attempts of the node (e.g. node's certificate is
 * revoked). Sub sequential calls to function can be used to remove several
 * nodes from border router.
 *
 * \param interface_id Network interface ID.
 * \param eui64 EUI-64 of revoked node
 *
 * \return 0, Node's keys has been removed
 * \return <0 Node's key remove has failed (e.g. unknown address)
 */
int ws_bbr_node_keys_remove(int8_t interface_id, uint8_t *eui64);

/**
 * Start revocation of node's access
 *
 * Starts revocation of node's access procedure on border router. Before
 * the call to this function, authentication service must be configured to
 * reject authentication attempts of the removed nodes (e.g. certificates
 * of the nodes are revoked). Also the keys for the nodes must be removed
 * from the border router.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0, Revocation started OK.
 * \return <0 Revocation start failed.
 */
int ws_bbr_node_access_revoke_start(int8_t interface_id);

/**
 * Set EAPOL node limit
 *
 * Border router stores EAPOL key information for each authenticated node.
 * Sets the maximum number of EAPOL nodes stored by border router. If count
 * of node's exceed the limit, border router deletes the node information
 * starting from oldest node (node that has authenticated longest time
 * ago), to make room for new nodes. When network keys are updated, nodes
 * which have been removed from storage, must make full authentication again.
 * Value for this parameter should be set to be more than maximum amount of
 * nodes that are expected to be connected to border router.
 *
 * \param interface_id Network interface ID.
 * \param limit Limit for nodes
 *
 * \return 0, Node limit set
 * \return <0 Node limit set failed.
 */
int ws_bbr_eapol_node_limit_set(int8_t interface_id, uint16_t limit);

/**
 * Extended certificate validation
 */
#define BBR_CRT_EXT_VALID_NONE    0x00 /**< Do not make extended validations */
#define BBR_CRT_EXT_VALID_WISUN   0x01 /**< Validate Wi-SUN specific fields */

/**
 * Sets extended certificate validation setting
 *
 * Sets extended certificate validation setting on border router. Function can be used
 * to set which fields on client certificate are validated.
 *
 * \param interface_id Network interface ID
 * \param validation Extended Certificate validation setting
 *          BBR_CRT_EXT_VALID_NONE   Do not make extended validations
 *          BBR_CRT_EXT_VALID_WISUN  Validate Wi-SUN specific fields
 *
 * \return 0 Validation setting was set
 * \return <0 Setting set failed
 */
int ws_bbr_ext_certificate_validation_set(int8_t interface_id, uint8_t validation);

/**
 * Sets RPL parameters
 *
 * Sets RPL DIO trickle parameters.
 *
 * \param interface_id Network interface ID.
 * \param dio_interval_min DIO trickle timer Imin parameter.
 * \param dio_interval_doublings DIO trickle timer Imax parameter as doublings of Imin
 * \param dio_redundancy_constant DIO trickle timer redundancy constant.
 *
 * \return 0, RPL parameters set.
 * \return <0 Node RPL parameters set failed.
 */
int ws_bbr_rpl_parameters_set(int8_t interface_id, uint8_t dio_interval_min, uint8_t dio_interval_doublings, uint8_t dio_redundancy_constant);

/**
 * Gets RPL parameters
 *
 * Gets RPL DIO trickle parameters.
 *
 * \param interface_id Network interface ID.
 * \param dio_interval_min DIO trickle timer Imin parameter.
 * \param dio_interval_doublings DIO trickle timer Imax parameter as doublings of Imin
 * \param dio_redundancy_constant DIO trickle timer redundancy constant.
 *
 * \return 0, RPL parameters get.
 * \return <0 Node RPL parameters get failed.
 */
int ws_bbr_rpl_parameters_get(int8_t interface_id, uint8_t *dio_interval_min, uint8_t *dio_interval_doublings, uint8_t *dio_redundancy_constant);

/**
 * Validate RPL parameters
 *
 * Validates RPL DIO trickle parameters.
 *
 * \param interface_id Network interface ID.
 * \param dio_interval_min DIO trickle timer Imin parameter.
 * \param dio_interval_doublings DIO trickle timer Imax parameter as doublings of Imin
 * \param dio_redundancy_constant DIO trickle timer redundancy constant.
 *
 * \return 0, RPL parameters validated.
 * \return <0 Node RPL parameters validation failed.
 */
int ws_bbr_rpl_parameters_validate(int8_t interface_id, uint8_t dio_interval_min, uint8_t dio_interval_doublings, uint8_t dio_redundancy_constant);

/**
 * Sets PAN configuration
 *
 * Sets PAN configuration parameters.
 *
 * \param interface_id Network interface ID.
 * \param pan_id PAN ID; 0xffff default, generate the PAN ID.
 *
 * \return 0, PAN configuration set.
 * \return <0 PAN configuration set failed.
 */
int ws_bbr_pan_configuration_set(int8_t interface_id, uint16_t pan_id);

/**
 * Gets PAN configuration
 *
 * Gets PAN configuration parameters.
 *
 * \param interface_id Network interface ID.
 * \param pan_id PAN ID; 0xffff default
 *
 * \return 0, PAN configuration get.
 * \return <0 PAN configuration get failed.
 */
int ws_bbr_pan_configuration_get(int8_t interface_id, uint16_t *pan_id);

/**
 * Validates PAN configuration
 *
 * Validates PAN configuration parameters.
 *
 * \param interface_id Network interface ID.
 * \param pan_id PAN ID.
 *
 * \return 0, PAN configuration validated.
 * \return <0 PAN configuration validation failed.
 */
int ws_bbr_pan_configuration_validate(int8_t interface_id, uint16_t pan_id);

/**
 * ws_bbr_key_storage_memory_set sets memory used for key storages
 *
 * This functions can be used to set memory used by EAPOL key storage. When memory
 * areas are set, module does not allocate memory internally from heap.
 *
 * \param interface_id Network interface ID.
 * \param key_storages_number number of memory areas.
 * \param key_storage_size array of memory area sizes.
 * \param key_storages array of memory area start pointers.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_key_storage_memory_set(int8_t interface_id, uint8_t key_storages_number, const uint16_t *key_storage_size, void **key_storages);

/**
 * ws_bbr_key_storage_settings_set sets key storage settings
 *
 * This functions can be used to set the settings of EAPOL key storage.
 * Allocation max number and allocation size sets the settings that are used when key storage
 * memory is allocated dynamically from heap. These settings must be set before (first) interface
 * up and shall not be set if key storage memory is set by ws_pae_key_storage_memory_set() call.
 *
 * \param interface_id Network interface ID.
 * \param alloc_max_number maximum number of allocation made to dynamic memory.
 * \param alloc_size size of each allocation.
 * \param storing_interval interval in which the check to store to NVM is made.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_key_storage_settings_set(int8_t interface_id, uint8_t alloc_max_number, uint16_t alloc_size, uint16_t storing_interval);

#endif /* WS_BBR_API_H_ */
