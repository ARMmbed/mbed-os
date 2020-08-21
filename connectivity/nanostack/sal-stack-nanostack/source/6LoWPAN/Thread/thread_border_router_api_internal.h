/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
 * \brief Nanostack Border Router instance and corresponding methods.
 */

#ifndef THREAD_BORDER_ROUTER_API_INTERNAL_H_
#define THREAD_BORDER_ROUTER_API_INTERNAL_H_

#include "net_interface.h"

#ifdef HAVE_THREAD_ROUTER

/**
 * \brief Initialize thread border router instance
 *
 * \param interface_id current interface id
 * \param external_commisssioner_port port for external commissioner
 *
 * \return 0 on success, <0 in case of error
 */
int8_t thread_border_router_init(int8_t interface_id);

/**
 * \brief Delete thread border router instance
 *
 * \param interface_id current interface id
 */
void thread_border_router_delete(int8_t interface_id);

/**
 * \brief seconds timer for border router
 *
 * \param interface_id current interface id
 * \param tics seconds pass since last call
 */
void thread_border_router_seconds_timer(int8_t interface_id, uint32_t tics);

/**
 * \brief Set thread resubmit timer timeout.
 *
 * \param interface_id Network interface ID.
 * \param seconds seconds to set to the timer,. -1 to initialize to default value.
  */
void thread_border_router_resubmit_timer_set(int8_t interface_id, int16_t seconds);

/**
 * \brief Call application callback with current Network Data TLV
 *
 * \param cur current interface entry
 */
void thread_border_router_network_data_appl_callback(protocol_interface_info_entry_t *cur);

/**
 * \brief Notify about network data update.
 *
 * \param cur current interface entry
 *
 */
void thread_border_router_network_data_update_notify(protocol_interface_info_entry_t *cur);

/**
 * \brief Clear data related to old partition.
 *
 * \param interface_id Network interface ID.
  */
void thread_border_router_old_partition_data_clean(int8_t interface_id);

#else
#define thread_border_router_init(interface_id)
#define thread_border_router_delete(interface_id)
#define thread_border_router_seconds_timer(interface_id, tics)
#define thread_border_router_resubmit_timer_set(interface_id, seconds)
#define thread_border_router_network_data_appl_callback(cur)
#define thread_border_router_network_data_update_notify(cur)
#define thread_border_router_old_partition_data_clean(interface_id)

#endif
#endif /* THREAD_BORDER_ROUTER_API_INTERNAL_H_ */
