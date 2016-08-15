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

#ifndef FS_CONFIG_H__
#define FS_CONFIG_H__

#include <stdint.h>
#include "nrf.h"

/**
 * @defgroup fstorage_config FStorage configuration
 * @ingroup fstorage
 * @{
 * @brief FStorage configuration.
 */


/**@brief Macro for max number of operations in the fs cmd queue.
 */
#define FS_CMD_QUEUE_SIZE   (8)


/**@brief Macro for max number of retries for a flash command before it notifies as failed.
 */
#define FS_CMD_MAX_RETRIES  (3)


/**@brief Macro for the content of a flash address that has not been written to.
 */
#define FS_EMPTY_MASK       (0xFFFFFFFF)


/**@brief Macro for flash page size according to chip family
 */
#if defined (NRF51)
    #define FS_PAGE_SIZE    (1024)
#elif defined (NRF52)
    #define FS_PAGE_SIZE    (4096)
#else
    #error "Device family must be defined. See nrf.h."
#endif


/*@brief Macro for flash page size according to chip family
*/
#define FS_PAGE_SIZE_WORDS  (FS_PAGE_SIZE/4)


/**@brief Static inline function that provides last page address
 *
 * @note    If there is a bootloader present the bootloader address read from UICR
 *          will act as the page beyond the end of the available flash storage
 */
static __INLINE uint32_t fs_flash_page_end_addr()
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
    return  ((bootloader_addr != FS_EMPTY_MASK) ?
             bootloader_addr : NRF_FICR->CODESIZE * FS_PAGE_SIZE);
}


/**@brief Macro for last page address
 *
 * @note    If there is a bootloader present the bootloader address read from UICR
 *          will act as the page beyond the end of the available flash storage
 */
#define FS_PAGE_END_ADDR  fs_flash_page_end_addr()


/**@brief Macro to describe the write
 *
 */
#if defined (NRF51)
    #define FS_MAX_WRITE_SIZE_WORDS	    (256)
#elif defined (NRF52)
    #define FS_MAX_WRITE_SIZE_WORDS     (1024)
#else
    #error "Device family must be defined. see nrf.h"
#endif

/** @} */

#endif // FS_CONFIG_H__

