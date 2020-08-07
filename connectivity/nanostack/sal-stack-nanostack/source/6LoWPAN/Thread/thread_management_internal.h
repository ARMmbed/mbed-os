/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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

#ifndef THREAD_MANAGEMENT_INTERNAL_H_
#define THREAD_MANAGEMENT_INTERNAL_H_

#include "net_interface.h"
#ifdef HAVE_THREAD

struct link_configuration;
/**
 *  Function for pushing Network Data to Managment IF from MLE module
 *
 * \param cur interface pointer to active interface
 * \param data_pointer pointer to Network Data
 * \param length Network Data length
 */
int thread_management_network_data_tlv_push(struct protocol_interface_info_entry *cur, uint8_t *data_pointer, uint16_t length);
/**
 * Get Mesh Local prefix.
 *
 * \param interface_id Network Interface.
 * \param prefix_ptr pointer where address prefix will be copied.
 *
 * return 0, Read OK
 * return <0 Read fail
 */

int thread_management_get_ml_prefix(int8_t interface_id, uint8_t *prefix_ptr);
/**
 * Get Mesh Local prefix added with short address prefix.
 *
 * You can add short address at end of this prefix to get ML16 address of any device.
 *
 * \param interface_id Network Interface.
 * \param prefix_ptr pointer where address prefix will be copied.
 *
 * return 0, Read OK
 * return <0 Read fail
 */
int thread_management_get_ml_prefix_112(int8_t interface_id, uint8_t *prefix_ptr);

/**
 * Step into next key in network.
 *
 * \param interface_id Network Interface
 * return 0, OK
 * return < 0 NOK
 */
int thread_management_increment_key_sequence_counter(int8_t interface_id);

int thread_management_key_sets_calc(protocol_interface_info_entry_t *cur, link_configuration_s *linkConfiguration, uint32_t thrKeySequenceCounter);

void thread_management_key_synch_req(int8_t interface_id, uint32_t keySequnce);
uint8_t *thread_management_key_request_with_sequence(int8_t interface_id, uint8_t keyId, uint32_t keySequnce);

void thread_security_update_from_mac(struct protocol_interface_info_entry *cur);

void thread_security_key_generate(struct protocol_interface_info_entry *cur, uint8_t *masterKey, uint32_t keySequence);
void thread_security_prev_key_generate(struct protocol_interface_info_entry *cur, uint8_t *masterKey, uint32_t keySequence);
void thread_security_next_key_generate(struct protocol_interface_info_entry *cur, uint8_t *masterKey, uint32_t keySequence);

void thread_key_get(uint8_t *key, uint8_t *key_material_buf,  uint32_t key_sequence_counter);

int thread_management_get_current_keysequence(int8_t interface_id, uint32_t *sequencePtr);

void arm_thread_private_ula_prefix_set(protocol_interface_info_entry_t *cur, const uint8_t *ula_prefix);

int8_t thread_node_bootstrap_init(int8_t interface_id, net_6lowpan_mode_e bootstrap_mode);
#else // HAVE_THREAD

#define thread_management_key_request(interface_id, keyId) ((uint8_t *) NULL)
#define thread_management_key_request_with_sequence(interface_id, keyId, keySequnce) ((uint8_t *) NULL)
#define thread_history_key_material_push(thread_info, mleKeyPtr, keyId) ((void) mleKeyPtr, (void) keyId) // evaluate parameters to suppress "unused" warnings
#define thread_security_update_from_mac(cur) ((void) 0)
#define thread_node_bootstrap_init(interface_id, bootstrap_mode) -1
#define thread_management_get_current_keysequence(interface_id, sequencePtr) -1
//#define thread_security_next_key_generate(cur, masterKey, keySequence) ((void) 0)

#endif // HAVE_THREAD


#endif /* THREAD_MANAGEMENT_INTERNAL_H_ */
