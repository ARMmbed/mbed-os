/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#ifndef THREAD_NEIGHBOR_CLASS_H_
#define THREAD_NEIGHBOR_CLASS_H_

struct thread_neighbor_class_s;

/** Thread Spesific ModeFlags */
#define MLE_THREAD_SECURED_DATA_REQUEST 0x04
#define MLE_THREAD_REQ_FULL_DATA_SET 0x01

bool thread_neighbor_class_create(struct thread_neighbor_class_s *class_ptr, uint8_t neigh_table_size);

void thread_neighbor_class_delete(struct thread_neighbor_class_s *class_ptr);

void thread_neighbor_class_add_link(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, uint8_t linkmargin);

void thread_neighbor_class_add_mleid(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, const uint8_t *mleid);

uint8_t *thread_neighbor_class_get_mleid(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

void thread_neighbor_class_update_link(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, uint8_t linkmargin, bool new_link);

void thread_neighbor_last_communication_time_update(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

uint16_t thread_neighbor_entry_linkmargin_get(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

uint32_t thread_neighbor_last_communication_time_get(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

bool thread_neighbor_class_mleid_compare(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, const uint8_t *mleid);

bool thread_neighbor_class_request_full_data_setup(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

bool thread_neighbor_class_secured_data_request(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

void thread_neighbor_class_mode_parse_to_entry(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, uint8_t mode);

uint8_t thread_neighbor_class_mode_write_from_entry(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

void thread_neighbor_class_request_full_data_setup_set(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, bool value);

void thread_neighbor_class_secured_data_request_set(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index, bool value);

void thread_neighbor_class_entry_remove(struct thread_neighbor_class_s *class_ptr, uint8_t attribute_index);

#endif /* THREAD_NEIGHBOR_CLASS_H_ */
