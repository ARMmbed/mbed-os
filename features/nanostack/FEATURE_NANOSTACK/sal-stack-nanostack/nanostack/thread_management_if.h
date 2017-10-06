/*
 * Copyright (c) 2014-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/**
 * \file thread_management_if.h
 * \brief Thread management interface.
 *
 * This interface is used for configuring Thread devices.
 * After creating the Thread interface, you can use this interface to configure the Thread device
 * behaviour. When you are done with the configurations, you need to call interface up to enable a Thread node.
 *
 */

#ifndef THREAD_MANAGEMENT_IF_H_
#define THREAD_MANAGEMENT_IF_H_

#include "ns_types.h"
#include "net_interface.h" /* Declaration for channel_list_s. */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Current protocol version of the Thread implementation.
 */
#define THREAD_BEACON_PROTOCOL_ID       3   /**< Beacon Protocol ID */
#define THREAD_BEACON_PROTOCOL_VERSION  1   /**< Beacon Protocol version */

/**
 * Thread network configuration.
 *
 * You can use this structure in start-up in case of a static configuration.
 * This data can also be read after joining the Thread network.
 * If this data is not provided to the stack, the device starts the commissioning process to join the Thread network.
 *
 * If the data is provided, all fields must be initialised to 0.
 *
 * If XPANID and MASTER KEY are provided, the device starts out-of-band commissioning. The values must be initialised to other than 0.
 * If mesh_local_eid is initialised to 0 it is randomized at start-up.
 * If extended_random_mac is initialised to 0 it is randomized at start-up.
 *
 * If timestamp values are set to 0 it triggers a network configuration update when joining the network.
 *
 * */
typedef struct link_configuration {
    uint8_t name[16]; /**< Name of the Thread network*/
    uint8_t master_key[16]; /**< Master key of the thread network*/
    uint8_t PSKc[16]; /**< PSKc value that is calculated from commissioning credentials credentials,XPANID and network name*/
    uint8_t mesh_local_ula_prefix[8]; /**< Mesh local ula prefix*/
    uint8_t mesh_local_eid[8]; /**< Mesh local extented id*/
    uint8_t extented_pan_id[8]; /**< Extended pan id*/
    uint8_t extended_random_mac[8]; /**< Extended random mac which is generated during commissioning*/
    uint8_t channel_mask[8]; /**< channel page and mask only supported is page 0*/
    uint8_t channel_page;/**< channel page supported pages 0*/
    char *PSKc_ptr; /**< Commissioning credentials.  TODO! think if we need the actual credentials*/
    uint8_t PSKc_len; /**< Length of PSKc */
    uint16_t key_rotation; /**< Key rotation time in hours*/
    uint32_t key_sequence; /**< Key sequence counter */
    uint16_t panId; /**< network id*/
    uint8_t Protocol_id; /**< current protocol id*/
    uint8_t version; /**< current protocol version*/
    uint16_t rfChannel; /**< current rf channel*/
    uint8_t securityPolicy; /**< Commission Security Policy*/
    uint64_t timestamp;/**< commissioning data set timestamp. [48 bit timestamp seconds]-[15 bit timestamp ticks]-[U bit] */
} link_configuration_s;

/**
 * Security policy options. Default for all is '1';
 */
#define SECURITY_POLICY_ALL_SECURITY                            0xff
#define SECURITY_POLICY_OUT_OF_BAND_COMMISSIONING_ALLOWED       0x80       /**< Obtaining the Master Key for out-of-band commissioning is enabled when this is set. */
#define SECURITY_POLICY_NATIVE_COMMISSIONING_ALLOWED            0x40       /**< Native Commissioning using PSKc is allowed when this is set. */
#define SECURITY_POLICY_ALL_ROUTERS_JOIN_ALLOWED                0x20       /**< Thread 1.x Routers are enabled when this is set. */
#define SECURITY_POLICY_EXTERNAL_COMMISSIONER_ALLOWED           0x10       /**< This indicates that external Commissioner authentication is allowed using PSKc. */
#define SECURITY_POLICY_BEACON_PAYLOAD_ENABLED                  0x08       /**< Thread 1.x Beacons are enabled when this is set. */

/*
 * Mandatory device information
 *
 * This information is required if commissioning is enabled for this device.
 */
typedef struct {
    uint8_t eui64[8];/**< eui64 of the device. This field is used to identify device when joining to network Mandatory*/
    uint8_t *PSKd_ptr;/**< Device credentials used to authenticate device to commissioner Mandatory  length 6-32*/
    uint8_t PSKd_len;/**< Length of PSKd_ptr*/
    char *provisioning_uri_ptr;/**< Provisioning url max 64 bytes*/
    char *vendor_name_ptr;/**< Vendor name optional max 32 bytes*/
    char *vendor_model_ptr;/**< Vendor model optional max 32 bytes*/
    char *vendor_sw_version_ptr;/**<  Vendor SW version optional max 16 bytes*/
    uint8_t vendor_stack_version[6];/**< Vendor stack version optional all 0 indicates not set*/
    uint8_t *vendor_data_ptr;/**<  optional Array max 64 bytes*/
    uint8_t vendor_data_len;/**<  optional Array length max 64 bytes*/
} device_configuration_s;


/**
 * Initialize Thread stack to node mode.
 *
 * If static configuration is given and new one is updated by commissioner
 * it will override current setup. it is safe to always give this as
 * default configuration.
 *
 * \param interface_id Network interface ID.
 * \param channel_list A pointer to channel list. Can be NULL if all channels accepted.
 * \param device_configuration A pointer to device configuration.
 * \param static_configuration A pointer to static configuration. Can be NULL.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int thread_management_node_init(
    int8_t interface_id,
    channel_list_s *channel_list,
    device_configuration_s *device_configuration,
    link_configuration_s *static_configuration);

/**
 * Thread device type.
 *
 * REED - Router enabled End device. Device can become router or end device depending on network conditions.
 * FED - Full End Device. Device creates links and makes address queries but does not become router.
 * MED - Minimal End Device. Device communicates through parent. With radio on
 * SED - Sleepy End Device. Device communicates through parent. Uses data poll to sleep.
*/
typedef enum {
    THREAD_DEVICE_REED = 1,
    THREAD_DEVICE_FED,
    THREAD_DEVICE_MED,
    THREAD_DEVICE_SED,
} thread_device_type_e;

/**
 * Change thread device type.
 *
 * This function modifies the thread device mode. Default values are given in
 * function arm_nwk_interface_configure_6lowpan_bootstrap_set().
 *
 * If this function is called when interface is up re-attach is made.
 *
 * \param interface_id Network interface ID.
 * \param device_type Device type of current bootstrap.
 *
 * \return 0, Set OK.
 * \return <0 Set fail.
 */
int thread_management_device_type_set(int8_t interface_id, thread_device_type_e device_type);

/**
 * Get Thread network settings.
 *
 * Configuration is a pointer to the static configuration and only valid in current context.
 *
 * \param interface_id Network interface ID.
 *
 * \return Pointer to link configuration.
 * \return NULL Failure.
 */
link_configuration_s *thread_management_configuration_get(int8_t interface_id);

/** Store Thread network link configuration settings to NVM.
 *
 * Storing is asynchronous operation and this method makes a request to store link
 * configuration settings. Operation will be completed in the background.
 * Once settings has been stored the Thread network will be restarted with new
 * configuration settings.
 *
 * /param interface Id of network interface. -1 if interface_id is not available.
 * /param link_config Pointer to a structure containing link configuration parameters
 *
 * /return 0 if store request has been delivered successfully to lower layer.
 * /return -1 if storing failed (request not delivered to lower layer)
 * /return -2 if store request delivered to lower layer but given interface_id was not valid.
 */
int thread_management_link_configuration_store(int8_t interface_id, link_configuration_s *link_config);

/** Delete Thread network link configuration settings.
 *
 * Deletion is asynchronous operation and this method makes a request to delete link
 * configuration settings. Operation will be completed in the background.
 * Once settings has been deleted the Thread network will be restarted with default settings.
 *
 * /param interface Id of network interface. -1 can be used if interface_id is not available.
 *
 * /return 0 if delete request has been delivered successfully to lower layer.
 * /return -1 if delete failed (request not delivered to lower layer)
 * /return -2 if delete request delivered to lower layer but given interface_id was not valid.
 */
int thread_management_link_configuration_delete(int8_t interface_id);

/**
 * Get Thread device settings.
 *
 * Configuration is a pointer to the static device configuration and only valid in current context.
 *
 * \param interface_id Network interface ID.
 *
 * \return Pointer to Device configuration.
 * \return NULL Failure.
 */
device_configuration_s *thread_management_device_configuration_get(int8_t interface_id);

/**
 * Thread router max child count set.
 *
 * This function is used to limit the number of children allowed for parent.
 *
 * \param interface_id Network interface ID.
 * \param maxChildCount Min accepted value is 0 and max 32.
 *
 * \return 0, Set OK.
 * \return <0 Set Fail.
 */
int thread_management_max_child_count(
    int8_t interface_id,
    uint8_t maxChildCount);

/**
 * Set Thread device link timeout.
 *
 * \param interface_id Network interface ID.
 * \param link_timeout New timeout value in seconds.
 *
 * \return 0, Set OK.
 * \return <0 Set Fail.
 */
int8_t thread_management_set_link_timeout(int8_t interface_id, uint32_t link_timeout);

/**
 * Get link timeout from Thread device.
 *
 * \param interface_id Network interface ID.
 * \param link_timeout [out] A pointer to the location for writing the timeout.
 *
 * \return 0, Get OK
 * \return <0 Get Fail
 */
int8_t thread_management_get_link_timeout(int8_t interface_id, uint32_t *link_timeout);

/**
 * Set Thread request full network data.
 *
 * \param interface_id Network interface ID.
 * \param full_nwk_data Whether or not to request full network data.
 *
 * \return 0, Set OK.
 * \return <0 Set Fail.
 */
int8_t thread_management_set_request_full_nwk_data(int8_t interface_id, bool full_nwk_data);

/**
 * Get Thread request full network data.
 *
 * \param interface_id Network interface ID.
 * \param link_timeout [out] A pointer to the location for writing the flag value.
 * \param full_nwk_data Request full network data
 *
 * \return 0, Get OK.
 * \return <0 Get Fail.
 */
int8_t thread_management_get_request_full_nwk_data(int8_t interface_id, bool *full_nwk_data);

/**
 * Additional Thread device settings. Changing these can cause non-compliance with Thread.
 *
 */

/**
 * Diagnostics functions.
 */

/**
 * Get leader mesh local 16 address.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to the location of address after copying.
 *
 * \return 0, Read OK.
 * \return <0 Read fail.
 */
int thread_management_get_leader_address(int8_t interface_id, uint8_t *address_ptr);

/**
 * Get leader anycast address.
 *
 * Address should be used when contacting Leader without need to know the actual routing details.
 * This address will remain valid even after leader changes.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to the location of address after copying.
 *
 * \return 0, Read OK.
 * \return <0 Read fail. Not connected to Thread network.
 */
int thread_management_get_leader_aloc(int8_t interface_id, uint8_t *address_ptr);

/**
 * Get parent link local 16 address.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to the location of address after copying.
 *
 * \return 0, Read OK.
 * \return <0 Read fail.
 */
int thread_management_get_parent_address(int8_t interface_id, uint8_t *address_ptr);

/**
 * Get own mesh local 64 address.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to the location of address after copying.
 *
 * \return 0, Read OK.
 * \return <0 Read fail.
 */
int thread_management_get_ml64_address(int8_t interface_id, uint8_t *address_ptr);

/**
 * Get own mesh local 16 address.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to the location of address after copying.
 *
 * \return 0, Read OK.
 * \return <0 Read fail.
 */
int thread_management_get_ml16_address(int8_t interface_id, uint8_t *address_ptr);

/**
 * Get commissioner address.
 *
 * This function returns the commissioner address where you can continue provisioning traffic.
 * If the commissioner is not present this function returns a failure.
 *
 * \param interface_id Network interface ID.
 * \param address_ptr A pointer to the location of address after copying.
 * \param port_ptr A pointer to the location of port after copying.
 *
 * \return 0, Read OK.
 * \return <0 Read fail.
 */
int thread_management_get_commissioner_address(int8_t interface_id, uint8_t *address_ptr, uint16_t *port_ptr);


#ifdef __cplusplus
}
#endif

#endif /* THREAD_MANAGEMENT_IF_H_ */
