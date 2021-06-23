/*
 * Copyright (c) 2017-2019, Pelion and affiliates.
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
 * \brief Thread 1.2 primary border router support.
 */

#ifndef THREAD_BBR_COMMERCIAL_H_
#define THREAD_BBR_COMMERCIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Thread PBBR ML-EID map structure
 */
typedef struct thread_pbbr_dua_info {
    uint8_t mleid_ptr[8];
    uint32_t last_contact_time;
} thread_pbbr_dua_info_t;

#if defined(HAVE_THREAD_V2) && defined(HAVE_THREAD_BORDER_ROUTER)
int8_t thread_bbr_commercial_init(int8_t interface_id, int8_t backbone_interface_id);
void thread_bbr_commercial_delete(int8_t interface_id);
bool thread_bbr_commercial_nd_query_process(protocol_interface_info_entry_t *cur, const uint8_t *target_addr, uint16_t rloc);
void thread_bbr_commercial_seconds_timer(int8_t interface_id, uint32_t seconds);
int thread_bbr_commercial_sequence_number_set(int8_t interface_id, uint8_t seq_number);
int thread_bbr_commercial_timeout_set(int8_t interface_id, uint32_t timeout_a, uint32_t timeout_b, uint32_t delay);
int thread_bbr_commercial_address_set(int8_t interface_id, const uint8_t *addr_ptr, uint16_t port);
void thread_bbr_commercial_route_update(protocol_interface_info_entry_t *cur);
int thread_bbr_commercial_prefix_set(int8_t interface_id, uint8_t *prefix);
void thread_bbr_commercial_old_partition_data_clean(int8_t interface_id);
void thread_bbr_commercial_status_override_get(uint8_t *dua_status, uint8_t *dua_count, uint8_t *ba_failure_count);
void thread_bbr_commercial_status_override_set(uint8_t dua_status, uint8_t dua_count, uint8_t ba_failure_count);
void thread_bbr_commercial_mcast_fwd_check(int8_t interface_id, bool *multicast_fwd);


#else

#define thread_bbr_commercial_init(interface_id, backbone_interface_id)
#define thread_bbr_commercial_delete(interface_id)
#define thread_bbr_commercial_nd_query_process(cur, target_addr, rloc) false
#define thread_bbr_commercial_seconds_timer(interface_id, seconds)
#define thread_bbr_commercial_timeout_set(interface_id, timeout_a, timeout_b, delay)
#define thread_bbr_commercial_address_set(interface_id, addr_ptr, port) (-1)
#define thread_bbr_commercial_route_update(cur)
#define thread_bbr_commercial_sequence_number_set(interface_id, seq_number) (-1)
#define thread_bbr_commercial_prefix_set(interface_id, prefix) 0
#define thread_bbr_commercial_old_partition_data_clean(interface_id)
#define thread_bbr_commercial_status_override_get(dua_status, dua_count, ba_failure_count);
#define thread_bbr_commercial_status_override_set(dua_status, dua_count, ba_failure_count);
#define thread_bbr_commercial_mcast_fwd_check(interface_id, multicast_fwd)
#endif

#ifdef __cplusplus
}
#endif

#endif //THREAD_BBR_COMMERCIAL_H_
