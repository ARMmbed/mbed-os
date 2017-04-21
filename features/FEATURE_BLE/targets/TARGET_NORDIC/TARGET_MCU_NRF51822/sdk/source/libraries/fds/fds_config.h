/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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
 * @addtogroup flash_data_storage
 * @{
 */

/**@brief Configures the size of the internal queue. */
#define FDS_CMD_QUEUE_SIZE          (8)
/**@brief Determines how many @ref fds_record_chunk_t structures can be buffered at any time. */
#define FDS_CHUNK_QUEUE_SIZE        (8)

/**@brief Configures the number of physical flash pages to use. Out of the total, one is reserved
 *        for garbage collection, hence, two pages is the minimum: one for the application data
 *        and one for the system. */
#define FDS_MAX_PAGES               (2)
/**@brief Configures the maximum number of callbacks which can be registred. */
#define FDS_MAX_USERS               (10)

/** Page tag definitions. */
#define FDS_PAGE_TAG_WORD_0_SWAP    (0xA5A5A5A5)
#define FDS_PAGE_TAG_WORD_0_VALID   (0xA4A4A4A4)
#define FDS_PAGE_TAG_WORD_1         (0xAABBCCDD)
#define FDS_PAGE_TAG_WORD_2         (0xAABB01DD) /**< Includes version. */
#define FDS_PAGE_TAG_WORD_3         (0x1CEB00DA)
#define FDS_PAGE_TAG_WORD_3_GC      (0x1CEB00D8)

/** @} */

#endif // FDS_CONFIG_H__
