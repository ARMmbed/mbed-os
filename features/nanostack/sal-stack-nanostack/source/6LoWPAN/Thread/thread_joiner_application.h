/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

/**
 * Joiner application starts the commisioning and saves the Static configuration of thread network
 * to nvm to recover from restart.
 */


#ifndef THREAD_JOINER_APPLICATION_H_
#define THREAD_JOINER_APPLICATION_H_
#include <ns_types.h>

struct link_configuration;

#define TRACE_GROUP_THREAD_JOINER_APPLICATION "TjoA"

/** Initialise thread Joiner application.
 *
 * This instance needs to bee initialised once for each thread network interface.
 * if only one thread instance is supported this is needed to call only once.
 *
 * /param interface interface id of this thread instance.
 * /param device_configuration_ptr Device configuration settings for application provisioning.
 * /param default_configuration_ptr Default configuration used if not other configuration available.
 *
 */
int thread_joiner_application_init(int8_t interface_id, device_configuration_s *device_configuration_ptr, struct link_configuration *default_configuration_ptr);

/** Delete Joiner application.
 *
 * /param interface interface id of this thread instance.
 */
void thread_joiner_application_deinit(int8_t interface_id);

/** Get configuration
 *
 * /param interface interface id of this thread instance.
 *
 */
struct link_configuration *thread_joiner_application_get_config(int8_t interface_id);

uint64_t thread_joiner_application_active_timestamp_get(int8_t interface_id);
uint8_t thread_joiner_application_security_policy_get(int8_t interface_id);
void thread_joiner_application_active_timestamp_set(int8_t interface_id, uint64_t timestamp);
uint8_t *thread_joiner_application_network_name_get(int8_t interface_id);
uint8_t *thread_joiner_application_active_config_tlv_list_get(uint8_t interface_id, uint16_t *length);

uint8_t *thread_joiner_application_active_config_params_get(uint8_t interface_id, uint16_t *length);

/** Get device configuration
 *
 * /param interface interface id of this thread instance.
 *
 */
device_configuration_s *thread_joiner_application_get_device_config(int8_t interface_id);

/** Store link configuration settings to platform specific NVM.
 *
 * Note1. All platforms may not have support for NVM.
 * Note2. Store is asynchronous operation, this method makes a storing request that
 *        will be completed in background.
 *
 * /param interface interface id of this thread instance. -1 if instance not initialized.
 * /param link_config Pointer to structure containing link configuration parameters
 *
 *\ return 0 if store request has been delivered to lower layers.
 *\ return -1 if store failed (store request not made)
 */

uint8_t *thread_joiner_application_random_mac_get(int8_t interface_id);

uint8_t *thread_joiner_application_ml_eid_get(int8_t interface_id);

int thread_joiner_application_link_configuration_store(int8_t interface_id, link_configuration_s *link_config);

/** Delete link configuration from platform NVM
 *
 * Note1. All platforms may not have support for NVM.
 * Note2. Delete is asynchronous operation, this method makes a delete request that
 *        will be completed on the background
 *
 * /param interface interface id of this thread instance. -1 if instance not initialized.
 *
 *\ return 0 if delete request has been delivered to lower layers.
 *\ return -1 if delete failed (delete request not made)
 */
int thread_joiner_application_link_configuration_delete(int8_t interface_id);

/** Check if link configuration settings should be loaded from NVM and make a read
 * request if settings needs to be loaded.
 *
 * /param interface interface id of this thread instance.
 *
 * /return true if read request is made, false otherwise
 */
bool thread_joiner_application_nvm_link_configuration_load(int8_t interface_id);


/** Provisioning status of the device.
 *
 */
typedef enum {
    PROVISIONING_STATUS_NOT_DONE,
    PROVISIONING_STATUS_ONGOING,
    PROVISIONING_STATUS_REJECTED,
    PROVISIONING_STATUS_DONE,
} thread_provisioning_status_e;

/** Set the provisioning status of the device.
 *
 * not done -> When commissioner attaches to network JOIN_APP.req is sent to commissioner
 * ongoing -> Waiting response to JOIN_APP.req message.
 * rejected -> Provisioning was attempted, but did not succeed. State will go back to not done after random period
 * done -> JOIN_APP.resp was received with accepted status.
 *
 * /param interface interface id of this thread instance.
 * /param status Current status of provisioning
 *
 */
int thread_joiner_application_provisioning_set(int8_t interface_id, thread_provisioning_status_e status);
/** get the provisioning status of the device.
 *
 * /param interface interface id of this thread instance.
 *
 * /return true if application provisioning is done.
 */
thread_provisioning_status_e thread_joiner_application_provisioning_get(int8_t interface_id);

/** form thread network with randomized values.
 *
 * /param interface interface id of this thread instance.
 * /param commissioning_credentials_ptr commissioning credentials used to form the network
 * /param name_ptr ascii string for name of the network can be NULL
 *
 */
int thread_joiner_application_form_network(int8_t interface_id, uint8_t *commissioning_credentials_ptr, char *name_ptr);

/** Commissioning done callback.
 *
 * This callback is called when commissioning is made and new attach can be made with new static configuration.
 *
 * /param interface interface id of this thread instance.
 *
 */
typedef void thread_joiner_application_commission_done_cb(int8_t interface_id);

/** Commissioning start.
 *
 * Start commissioning process to this network
 *
 * /param interface interface id of this thread instance.
 *
 */
int thread_joiner_application_pskd_commission_start(int8_t interface_id, uint8_t parent_address[16], uint16_t joiner_port, uint16_t panid, uint8_t xpanid[8], uint8_t channel, thread_joiner_application_commission_done_cb *done_cb);

/** One second ticker for delayed configurations.
 *
 * /param interface interface id of this thread instance.
 *
 */
void thread_joiner_application_seconds_timer(int8_t interface_id, uint32_t seconds);

/** Create new pending configuration.
 *
 * Cleans the previous pending configuration if it is present.
 *
 * /param interface interface id of this thread instance.
 * /param data_ptr Pointer to data that is included in the new pending set
 * /param data_len length of operational dataset that is used to generate the pending set
 *
 */
int thread_joiner_application_pending_config_create(int8_t interface_id, uint8_t *data_ptr, uint16_t data_len);

/** Checks if pending configuration is present.
 *
 * /param interface interface id of this thread instance.
 *
 */
bool thread_joiner_application_pending_config_exists(int8_t interface_id);
/** get timestamp value of pending configuration.
 *
 * /param interface interface id of this thread instance.
 *
 */
uint64_t thread_joiner_application_pending_config_timestamp_get(int8_t interface_id);

/** Set pending timestamp value
 *
 * /param interface interface id of this thread instance.
 * /param timestamp timestamp value.
 *
 */
int thread_joiner_application_pending_config_timestamp_set(int8_t interface_id, uint64_t timestamp);

/** get pending timeout value.
 *
 * /param interface interface id of this thread instance.
 *
 * /return timeout in ms.
 *
 */
uint32_t thread_joiner_application_pending_config_timeout_get(int8_t interface_id);

/** Enable pending configuration.
 *
 * Pending configuration is taken in use after timeout.
 *
 * /param interface interface id of this thread instance.
 * /param timeout_in_ms Timeout value after which pending set is activated.
 *
 */
int thread_joiner_application_pending_config_enable(int8_t interface_id, uint32_t timeout_in_ms);


/** delete pending configuration.
 *
 * Cleans the previous pending configuration if it is present.
 *
 * /param interface interface id of this thread instance.
 *
 */
void thread_joiner_application_pending_config_delete(int8_t interface_id);
uint8_t *thread_joiner_application_pending_config_tlv_list_get(uint8_t interface_id, uint16_t *length);
uint16_t thread_joiner_application_pending_config_length(uint8_t interface_id, uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len);
uint8_t *thread_joiner_application_pending_config_build(uint8_t interface_id, uint8_t *ptr, uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len);
void thread_joiner_application_pending_config_add_missing_fields(uint8_t interface_id);

/**
 *  TODO still expirimental for Thread 1.1
 */
void thread_joiner_application_next_pending_config_save(int8_t interface_id);
bool thread_joiner_application_next_pending_config_exists(int8_t interface_id);
uint16_t thread_joiner_application_next_pending_config_length(int8_t interface_id);
uint8_t *thread_joiner_application_next_pending_config_build(int8_t interface_id, uint8_t *ptr);
void thread_joiner_application_next_pending_config_delete(int8_t interface_id);
bool thread_joiner_application_pending_delay_timer_in_sync(int8_t interface_id);


void thread_joiner_application_next_active_config_save(int8_t interface_id);
bool thread_joiner_application_next_active_config_exists(int8_t interface_id);
uint16_t thread_joiner_application_next_active_config_length(int8_t interface_id);
uint8_t *thread_joiner_application_next_active_config_write(int8_t interface_id, uint8_t *ptr);
void thread_joiner_application_next_active_config_delete(int8_t interface_id);

/**Activate old configuration.
 *
 * If we have old configuration that we can try to restore connectivity to old configuration is is started
 *
 * /param interface interface id of this thread instance.
 *
 */
int thread_joiner_application_old_config_activate(int8_t interface_id);

/**Delete old configuration.
 *
 * /param interface interface id of this thread instance.
 *
 */
void thread_joiner_application_old_config_delete(int8_t interface_id);

/**Create old configuration.
 *
 * If we have have an active configuration that we have to come back to then the active configuration can be saved
 * temporarily to old configuration
 *
 * /param interface interface id of this thread instance.
 *
 */
link_configuration_s *thread_joiner_application_old_config_create(int8_t interface_id);

/**Get old configuration.
 *
 * If we have have a saved old configuration then it can be retrieved
 *
 * /param interface interface id of this thread instance.
 *
 */
bool thread_joiner_application_old_config_exists(int8_t interface_id);

/** Saves the current configurations to NVM.
 *
 * /param interface interface id of this thread instance.
 *
 * /return 0 OK.
 * /return <0 Failure.
 *
 */
int thread_joiner_application_configuration_nvm_save(int8_t interface_id);

/** Update configuration from meshcop tlv message.
 *
 * /param interface interface id of this thread instance.
 * /param msg_ptr meshcop TLV message structure.
 * /param msg_len Length of message.
 *
 * /return 0 OK.
 * /return <0 Failure.
 *
 */
int thread_joiner_application_update_configuration(uint8_t interface_id, uint8_t *msg_ptr, uint16_t msg_len, bool include_missing_tlvs);

/** Calculate length of active/pending configuration dataset TLVs.
 *
 * /param link_configuration Link configuration used to build the message.
 * /param msg_ptr meshcop TLV message structure.
 * /param msg_len Length of message.
  *
 * /return length of the message.
 * /return <0 Failure.
 */
uint16_t thread_joiner_application_active_config_length(uint8_t interface_id, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len);

/** build active/pending configuration dataset TLVs.
 *
 * /param link_configuration Link configuration used to build the message.
 * /param response_ptr meshcop TLV message structure.
 * /param tlv_list List of TLVs requested NULL if all wanted.
 * /param list_len length of tlv list.
  *
 * /return length of the message.
 * /return <0 Failure.
 */

uint8_t *thread_joiner_application_active_config_write(uint8_t interface_id, uint8_t *ptr, const uint8_t *req_tlv_ptr, uint8_t req_tlv_len, const uint8_t *ignored_tlv_ptr, uint8_t ignored_tlv_len);

/** Calculate length of device configuration TLVs.
 *
 * /param device_configuration device configuration used to build the message.
 *
 * /return length of the message.
 * /return <0 Failure.
 */
int thread_joiner_application_device_configuration_length(device_configuration_s *device_configuration);

/** write device configuration TLVs.
 *
 * /param ptr pointer where to write message.
 * /param device_configuration device configuration used to build the message.
 *
 * /return length of the message.
 * /return <0 Failure.
 */
uint8_t *thread_joiner_application_device_configuration_build(uint8_t *ptr, device_configuration_s *device_configuration);

/** function to activate pending configuration if present
 */
void thread_joiner_pending_config_activate(int8_t interface_id);

/* MLE active configuration data set fields
 *
 */
extern const uint8_t mle_active_configuration_dataset_tlvs[];

extern const uint8_t mle_pending_configuration_dataset_ignore_tlvs[];

extern const uint8_t mle_active_configuration_dataset_ignore_tlvs[];

uint8_t mle_active_configuration_dataset_tlvs_size(void);

uint8_t mle_active_configuration_dataset_ignore_tlvs_size(void);

uint8_t mle_pending_configuration_dataset_ignore_tlvs_size(void);

extern const uint8_t mle_pending_configuration_dataset_tlvs[];

uint8_t mle_pending_configuration_dataset_tlvs_size(void);

extern const uint8_t pending_configuration_dataset_tlvs[];

uint8_t pending_configuration_dataset_tlvs_size(void);

extern const uint8_t active_configuration_dataset_tlvs[];

uint8_t active_configuration_dataset_tlvs_size(void);

extern const uint8_t entrust_dataset_tlvs[];

uint8_t entrust_dataset_tlvs_size(void);

int joiner_application_security_done_cb(int8_t service_id, uint8_t address[16], uint8_t keyblock[40]);

int8_t thread_joiner_application_active_configuration_update(int8_t service_id, uint8_t *data_ptr, uint16_t data_len, const uint8_t *ignore_ptr, uint8_t ingore_len);

#endif /* THREAD_JOINER_APPLICATION_H_ */
