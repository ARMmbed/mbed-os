/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#ifndef THREAD_NVM_STORE_H_
#define THREAD_NVM_STORE_H_

#include <stdio.h>
#include <errno.h>
#include <stdint.h>

/*
 * \file thread_nvm_store.h
 * \brief This API is for storing Thread related values to NVM file system.
 * The Posix API is used for file storing/reading (file fopen, fread, fwrite).
 * The get functions should only be used when the device is booted up, because read functions
 * are slow and can cause packet dropping.
 * The storing functions do some filtering (see below) in order to avoid excess writing to NVM.
 */


#ifdef __cplusplus
extern "C" {
#endif

#define THREAD_NVM_FILE_SUCCESS            0
#define THREAD_NVM_FILE_READ_ERROR        -1
#define THREAD_NVM_FILE_WRITE_ERROR       -2
#define THREAD_NVM_FILE_VERSION_WRONG     -3
#define THREAD_NVM_FILE_CANNOT_OPEN       -4
#define THREAD_NVM_FILE_ROOT_PATH_INVALID -5
#define THREAD_NVM_FILE_PARAMETER_INVALID -6
#define THREAD_NVM_FILE_REMOVE_ERROR      -7

typedef struct {
    uint32_t mle_frame_counter;
    uint32_t mac_frame_counter;
    uint32_t seq_counter;
} thread_nvm_fast_data_t;

typedef struct {
    uint8_t mac[8];
    uint8_t mle_id[8];
} thread_nvm_device_conf_t;

/* reads all fast data from nvm, if the return values is THREAD_NVM_FILE_ROOT_PATH_INVALID, the cached values are returned.  */
int thread_nvm_store_fast_data_read(thread_nvm_fast_data_t* fast_data);
/* stores all fast data to nvm */
int thread_nvm_store_fast_data_write(thread_nvm_fast_data_t* fast_data);
/* stores new frame counters to nvm only if the any frame counter threshold is passed*/
int thread_nvm_store_frame_counters_check_and_write(uint32_t mac_frame_counter, uint32_t mle_frame_counter);
/* stores the frame counter and seq counter to nvm only if any threshold is passed*/
int thread_nvm_store_fast_data_check_and_write(uint32_t mac_frame_counter, uint32_t mle_frame_counter, uint32_t network_seq_counter);
/* stores the value to nvm only if it has changed */

int thread_nvm_store_seq_counter_write(uint32_t network_seq_counter);
/* stores the active configuration */
int thread_nvm_store_active_configuration_write(void *data, uint16_t data_size);
/* Reads the active configuration */
int thread_nvm_store_active_configuration_read(void *data, uint16_t data_size);
/* Removes the active configuration */
int thread_nvm_store_active_configuration_remove(void);

int thread_nvm_store_device_configuration_write(uint8_t *mac_ptr, uint8_t *mleid_ptr);
int thread_nvm_store_device_configuration_read(uint8_t *mac_ptr, uint8_t *mleid_ptr);
int thread_nvm_store_pending_configuration_write(void *data, uint16_t size);
int thread_nvm_store_pending_configuration_read(void *data, uint16_t size);
int thread_nvm_store_pending_configuration_remove(void);

int thread_nvm_store_seq_counter_store(uint32_t network_seq_counter);
/* read link info to cache */
int thread_nvm_store_link_info_read(void);
/* get link information */
int thread_nvm_store_link_info_get(uint8_t *parent_mac64, uint16_t *my_short_address);
/* clear link information */
int thread_nvm_store_link_info_clear(void);
/* write link information, will use caching and delayed writing */
int thread_nvm_store_link_info_write(uint8_t *parent_mac, uint16_t short_addr);
/* second timer for NVM store to delay operations */
void thread_nvm_store_seconds_timer(uint32_t seconds);

#ifdef __cplusplus
}
#endif

#endif //THREAD_NVM_STORE_H_
