/* 
 * Copyright (c) 2015 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


#ifndef FDS_CONFIG_H__
#define FDS_CONFIG_H__

 /**
 * @file fds_config.h
 *
 * @defgroup flash_data_storage_config Configuration options
 * @ingroup flash_data_storage
 * @{
 * @brief   Configuration options for FDS.
 */

/**@brief   Configures the size of the internal queue. */
#define FDS_OP_QUEUE_SIZE           (4)

/**@brief   Determines how many @ref fds_record_chunk_t structures can be buffered at any time. */
#define FDS_CHUNK_QUEUE_SIZE        (8)

/**@brief   Configures the maximum number of callbacks that can be registered. */
#define FDS_MAX_USERS               (3)

/**@brief   Configures the number of virtual flash pages to use.
 *
 * The total amount of flash memory that is used by FDS amounts to
 * @ref FDS_VIRTUAL_PAGES * @ref FDS_VIRTUAL_PAGE_SIZE * 4 bytes.
 * On nRF51 ICs, this defaults to 3 * 256 * 4 bytes = 3072 bytes.
 * On nRF52 ICs, it defaults to 3 * 1024 * 4 bytes = 12288 bytes.
 *
 * One of the virtual pages is reserved by the system for garbage collection. Therefore, the
 * minimum is two virtual pages: one page to store data and one page to be used by the system for
 * garbage collection.
 */
#define FDS_VIRTUAL_PAGES           (3)

/**@brief   Configures the size of a virtual page of flash memory, expressed in number of
 *          4-byte words.
 *
 * By default, a virtual page is the same size as a physical page. Therefore, the default size
 * is 1024 bytes for nRF51 ICs and 4096 bytes for nRF52 ICs.
 *
 * The size of a virtual page must be a multiple of the size of a physical page.
 */
#if   defined(NRF51)
    #define FDS_VIRTUAL_PAGE_SIZE   (256)
#elif defined(NRF52)
    #define FDS_VIRTUAL_PAGE_SIZE   (1024)
#endif

/** @} */

#endif // FDS_CONFIG_H__
