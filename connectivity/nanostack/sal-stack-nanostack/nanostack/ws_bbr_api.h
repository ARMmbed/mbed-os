/*
 * Copyright (c) 2017-2021, Pelion and affiliates.
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
    /** Default route Link Local address of north bound router*/
    uint8_t gateway[16];
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
 * \brief Struct bbr_radius_timing_t is RADIUS timing configuration structure.
 */
typedef struct bbr_radius_timing {
    /** RADIUS retry timer Imin; in 100ms units; range 1-1200; default 20 (2 seconds) */
    uint16_t radius_retry_imin;
    /** RADIUS retry timer Imax; in 100ms units; range 1-1200; default 30 (3 seconds) */
    uint16_t radius_retry_imax;
    /** RADIUS retry count; default 3 */
    uint8_t radius_retry_count;
} bbr_radius_timing_t;

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
#define BBR_REQUIRE_DAO_REFRESH   0x0000 /**< Deprecated DAO Refresh is now the default functionality*/
#define BBR_PERIODIC_VERSION_INC  0x0010 /**< Increment PAN version number Periodically*/
#define BBR_GUA_SLAAC             0x0020 /**< in Global prefix use SLAAC address generation to reduce traffic during bootstrap */
#define BBR_DHCP_ANONYMOUS        0x0040 /**< Generate anonymous addresses from DHCP server */

/**
 * Configure border router features.
 *
 * \param interface_id interface ID of the Wi-SUN network
 * \param options Options configured to Border router
 *          BBR_ULA_C     Configure Mesh local ULA prefix with SLAAC address
 *          BBR_GUA_ROUTE Add more specific route for GUA
 *          BBR_BB_WAIT   Start Wi-SUN network only when backbone is ready
 *          BBR_DHCP_ANONYMOUS if true give anonymous address (16 bit suffix) to
 *                             optimize data in RF interface (saves 12 bytes per hop)
 *                             or false to reduce RAM usage in Border router as assigned address list is not needed (40 bytes per device).
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
 *  IPv6 = Global Prefix + IID.
 *
 * Routing table is in the format: 16 bytes per entry
 * | Node IID 8 bytes   | parent IID 8 bytes |
 * | 1122112211221122   | 1111111111111111   |
 * | 1133113311331133   | 1111111111111111   |
 * | 1144114411441144   | 1111111111111111   |
 * | 1155115511551155   | 1122112211221122   |
 * | 1166116611661166   | 1122112211221122   |
 * | 1177117711771177   | 1155115511551155   |
 * | 1188118811881188   | 1177117711771177   |
 *
 * Order is not assured only parent child link is given in random order,
 *
 * When preparing to call this function ws_bbr_info_get function should be called to get the amount of devices in the network.
 * Memory for table is allocated based on the size of network and needs to be sizeof(bbr_route_info_t) * amount of entries.
 *
 * Return value is amount of route entries written to the table.
 *
 * \param interface_id interface ID of the Wi-SUN network.
 * \param table_ptr Application allocated memory where routing table is written.
 * \param table_len Length of the table allocated by application given as amount of entries.
 *
 * \return 0 - x on success indicates amount of Route entries written to the table_ptr
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
 * Sets Wi-SUN BSI
 *
 * Sets Wi-SUN PAN BSI.
 *
 * \param interface_id Network interface ID.
 * \param new_bsi Identifier.
 *
 * \return 0, PAN BSI set.
 * \return <0 PAN BSI set failed.
 */
int ws_bbr_bsi_set(int8_t interface_id, uint16_t new_bsi);

/**
 * Sets memory used for key storages
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
 * Sets key storage settings
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

/**
 * Set RADIUS server IPv6 address
 *
 * Function sets external RADIUS server IPv6 address to Border Router. Setting the
 * address enables external RADIUS server interface on Border Router. To disable external
 * RADIUS server interface, call the function with remote address set to NULL. The RADIUS
 * shared secret must be set before address is set using ws_bbr_radius_shared_secret_set()
 * call.
 *
 * \param interface_id Network interface ID.
 * \param address Pointer to IPv6 address or NULL to disable RADIUS. Address is in binary format (16 bytes).
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_address_set(int8_t interface_id, const uint8_t *address);

/**
 * Get RADIUS server IPv6 address
 *
 * Function gets external RADIUS server IPv6 address to Border Router.
 *
 * \param interface_id Network interface ID.
 * \param address buffer where to write address, must have space at least for 39 characters and NUL terminator
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_address_get(int8_t interface_id, uint8_t *address);

/**
 * Set RADIUS shared secret
 *
 * Function sets RADIUS shared secret to Border Router. Shared secret may be an
 * ASCII string. Check the format and length constraints for the shared secret from
 * the documentation of RADIUS server you are connecting to. Nanostack will not
 * make copy of the shared secret, therefore address and data must remain permanently
 * valid.
 *
 * \param interface_id Network interface ID.
 * \param shared_secret_len The length of the shared secret in bytes.
 * \param shared_secret Pointer to shared secret. Can be 8-bit ASCII string or byte array. Is not NUL terminated.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_shared_secret_set(int8_t interface_id, const uint16_t shared_secret_len, const uint8_t *shared_secret);

/**
 * Get RADIUS shared secret
 *
 * Function gets RADIUS shared secret from Border Router.
 *
 * \param interface_id Network interface ID.
 * \param shared_secret_len On function call, is the size of the shared secret write buffer in bytes, on return is the shared secret length in bytes.
 * \param shared_secret Pointer to buffer where to write shared secret or NULL. At maximum, bytes set by the length parameter are written. If NULL only buffer length is returned.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_shared_secret_get(int8_t interface_id, uint16_t *shared_secret_len, uint8_t *shared_secret);

/**
 * Set RADIUS timing information
 *
 * Function sets RADIUS timing information to Border Router.
 *
 * \param interface_id Network interface ID.
 * \param timing Timing information
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_timing_set(int8_t interface_id, bbr_radius_timing_t *timing);

/**
 * Get RADIUS timing information
 *
 * Function sets RADIUS timing information from Border Router.
 *
 * \param interface_id Network interface ID.
 * \param timing Timing information
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_timing_get(int8_t interface_id, bbr_radius_timing_t *timing);

/**
 * Validate RADIUS timing information
 *
 * Function validates RADIUS timing information.
 *
 * \param interface_id Network interface ID.
 * \param timing Timing information
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int ws_bbr_radius_timing_validate(int8_t interface_id, bbr_radius_timing_t *timing);

/**
 * \brief A function to set DNS query results to border router
 *
 * Border router distributes these query results in DHCP Solicit responses to
 * all the devices joining to the Wi-SUN mesh network.
 *
 * Border router keeps these forever, but if application does not update these in regular interval
 * The address might stop working. So periodic keep alive is required.
 *
 * These cached query results will become available in the Wi-SUN interface.
 *
 * This function can be called multiple times.
 * if domain name matches a existing entry address is updated.
 * If domain name is set to NULL entire list is cleared
 * if address is set to NULL the Domain name is removed from the list.
 *
 * \param interface_id Network interface ID.
 * \param address The address of the DNS query result.
 * \param domain_name_ptr Domain name matching the address
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int ws_bbr_dns_query_result_set(int8_t interface_id, const uint8_t address[16], char *domain_name_ptr);

#endif /* WS_BBR_API_H_ */
