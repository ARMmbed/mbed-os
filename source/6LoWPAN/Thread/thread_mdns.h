/*
 * Copyright (c) 2017, Pelion and affiliates.
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
 * \brief Thread mDNS responser. This module implements Thread specific MDNS-SD functionality
 * by using mDNS responder available in Nanostack.
 *
 */

#ifndef __THREAD_MDNS_H__
#define __THREAD_MDNS_H__

/**
 * \brief Start Thread mDNS responder
 *
 * \param interface_id Thread Network interface ID.
 * \param interface_id_mdns Interface ID where mDNS messaging occurs.
 * \param service_name Name of the instance
 *
 * \return 0 on success, negative value on error
  */
int thread_mdns_start(int8_t interface_id, int8_t interface_id_mdns, const char *service_name);

/**
 * \brief Stop Thread mDNS responder
 *
 * \return 0 on success, negative value on error
 */
int thread_mdns_stop(void);

/**
 * \brief Notify Thread mDNS responser that network data has been changed
 *
 */
void thread_mdns_network_data_update_notify(void);

#endif // __THREAD_MDNS_H__

