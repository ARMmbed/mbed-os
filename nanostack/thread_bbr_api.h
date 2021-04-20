/*
 * Copyright (c) 2017-2018, Arm Limited and affiliates.
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
 * \file thread_bbr_api.h
 * \brief Thread backbone border router (BBR) application interface.
 *
 * This is Thread backbone Border router service.
 * When started the module takes care of starting the
 * components that enables default border router functionality in Thread mesh network.
 *
 */

#ifndef THREAD_BBR_API_H_
#define THREAD_BBR_API_H_

#include "ns_types.h"

/**
 * Start backbone border router service.
 *
 * Enables the mDNS responder with default service name. \<rand\>-ARM-\<Network name\>
 * if backbone interface is enabled and allows routing.
 *    Publishes Default route with DHCP addressing
 *    Enables ND proxy
 *
 * \param interface_id Thread network interface id.
 * \param backbone_interface_id backbone interface id.
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_start(int8_t interface_id, int8_t backbone_interface_id);

/**
 * Set the bbr timeout values. Default timeout values are used if not called.
 *
 * If this function is called it will trigger the modification to thread network causing'
 * refreshing of registrations.
 *
 * \param interface_id interface ID of the Thread network.
 * \param timeout_a timeout a set. 0 = do not change.
 * \param timeout_b timeout_b set. 0 = do not change
 * \param delay Jitter delay value. 0 do not change
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_timeout_set(int8_t interface_id, uint32_t timeout_a, uint32_t timeout_b, uint32_t delay);

/**
 * Set prefix to be used as combining multiple thread networks on backbone.
 *
 * update prefix value
 *
 * \param interface_id interface ID of the Thread network.
 * \param prefix value must be buffer at the size of 8 bytes
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_prefix_set(int8_t interface_id, uint8_t *prefix);

/**
 * Set sequence number to be used by bbr
 *
 * update sequence number value
 *
 * \param interface_id interface ID of the Thread network.
 * \param sequence_number value that needs to be set on bbr
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_sequence_number_set(int8_t interface_id, uint8_t sequence_number);

/**
 * Set the Thread validation interface destination address.
 *
 * \param interface_id interface ID of the Thread network.
 * \param addr_ptr IPv6 address.
 * \param port UDP port.
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
int thread_bbr_validation_interface_address_set(int8_t interface_id, const uint8_t *addr_ptr, uint16_t port);

/**
 * Stop backbone Border router.
 *
 * \param interface_id interface ID of the Thread network
 *
 * \return 0 on success
 * \return <0 in case of errors
 *
 */
void thread_bbr_stop(int8_t interface_id);


#endif /* THREAD_BBR_API_H_ */
