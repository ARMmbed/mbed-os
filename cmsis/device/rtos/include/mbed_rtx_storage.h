/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MBED_RTX_STORAGE_H
#define MBED_RTX_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtx_os.h"
#include "mbed_rtx_conf.h"

/** \ingroup mbed-os-internal */
/** \addtogroup rtos-internal-api */
/** @{*/

/** \defgroup rtos_storage RTOS primitives storage types for RTX
 Types defined in this file should be utilized, when the direct RTOS C API usage is required, to provide backing memory
 for internal RTX data. Allocated object should be wrapped in attribute struct and passed to os*New call, for details
 see CMSIS-RTOS2 documentation.

 This file breaks abstraction layers and uses RTX internal types, but it limits the contamination to single, RTOS
 implementation specific, header file, therefore limiting scope of possible changes.
  @{
*/

/** RTX Mutex storage */
typedef osRtxMutex_t mbed_rtos_storage_mutex_t;

/** RTX Semaphore storage */
typedef osRtxSemaphore_t mbed_rtos_storage_semaphore_t;

/** RTX Thread storage */
typedef osRtxThread_t mbed_rtos_storage_thread_t;

/** RTX Memory Pool storage */
typedef osRtxMemoryPool_t mbed_rtos_storage_mem_pool_t;

/** RTX Message Queue storage */
typedef osRtxMessageQueue_t mbed_rtos_storage_msg_queue_t;

/** RTX Event Flags storage */
typedef osRtxEventFlags_t mbed_rtos_storage_event_flags_t;

/** RTX Message storage */
typedef osRtxMessage_t mbed_rtos_storage_message_t;

/** RTX Timer storage */
typedef osRtxTimer_t mbed_rtos_storage_timer_t;
/** @}*/
/** @}*/

#define MBED_RTOS_STORAGE_MEM_POOL_MEM_SIZE(block_count, block_size) \
  osRtxMemoryPoolMemSize(block_count, block_size)

#ifdef __cplusplus
}
#endif

#endif
