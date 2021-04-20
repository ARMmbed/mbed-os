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
 * \file thread_commissioning_if.h
 * \brief Thread internal commissioning if.
 *
 */

#ifndef NANOSTACK_THREAD_COMMISSIONING_IF_H_
#define NANOSTACK_THREAD_COMMISSIONING_IF_H_

#include "ns_types.h"

/**
 * This interface is used in commissioning for all the devices attaching to New Thread network.
 */

int thread_commissioning_if_pairwise_key_add(int8_t interface_id, uint32_t valid_life_time, uint8_t eui64[static 8], uint8_t key[static 16]);

/**
 * Delete pairwise key for specific device.
 *
 * \param interface_id Network Interface
 * \param eui64 eui64 of device that key is valid for.
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_if_pairwise_key_del(int8_t interface_id, uint8_t eui64[static 8]);

/**
 * Delete all pairwise keys from the device.
 *
 * \param interface_id Network Interface
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_if_pairwise_key_delete_all(int8_t interface_id);

/**
 * Get commissioner border router locator.
 *
 * \param interface_id Network Interface
 * \param address_ptr 16 byte buffer where address is written
 *
 * return 0, OK
 * return -1 fail
 */
int thread_commissioning_if_border_router_locator_get(int8_t interface_id, uint8_t *address_ptr);

/** @todo
 * This function is interop hack only and should not exists when proper commissioner
 * joiner implementation is made. The Link configuration is learned during commissioning and should be
 * updated using new function in thread management interface thread_managenet_configuration_set.
 * functions
 *
 * */
int thread_commissioning_if_enable_security(int8_t interface_id);
#endif /* NANOSTACK_THREAD_COMMISSIONING_IF_H_ */
