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

 /** @cond To make doxygen skip this file */

/** @file
 *  This header contains defines with respect persistent storage that are specific to
 *  persistent storage implementation and application use case.
 */
#ifndef PSTORAGE_PL_H__
#define PSTORAGE_PL_H__

#include <stdint.h>
#include "nrf.h"

static __INLINE uint16_t pstorage_flash_page_size()
{
  return (uint16_t)NRF_FICR->CODEPAGESIZE;
}

#define PSTORAGE_FLASH_PAGE_SIZE     pstorage_flash_page_size()          /**< Size of one flash page. */
#define PSTORAGE_FLASH_EMPTY_MASK    0xFFFFFFFF                          /**< Bit mask that defines an empty address in flash. */

#ifdef NRF51
#define BOOTLOADER_ADDRESS           (NRF_UICR->BOOTLOADERADDR)
#elif defined NRF52
#define BOOTLOADER_ADDRESS           (PSTORAGE_FLASH_EMPTY_MASK)
#endif 

static __INLINE uint32_t pstorage_flash_page_end()
{
   uint32_t bootloader_addr = BOOTLOADER_ADDRESS;
  
   return ((bootloader_addr != PSTORAGE_FLASH_EMPTY_MASK) ?
           (bootloader_addr/ PSTORAGE_FLASH_PAGE_SIZE) : NRF_FICR->CODESIZE);
}

#define PSTORAGE_FLASH_PAGE_END     pstorage_flash_page_end()

#ifndef PSTORAGE_NUM_OF_PAGES
#define PSTORAGE_NUM_OF_PAGES       1                                                           /**< Number of flash pages allocated for the pstorage module excluding the swap page, configurable based on system requirements. */
#endif

#define PSTORAGE_MIN_BLOCK_SIZE     0x0010                                                      /**< Minimum size of block that can be registered with the module. Should be configured based on system requirements, recommendation is not have this value to be at least size of word. */

#define PSTORAGE_DATA_START_ADDR    ((PSTORAGE_FLASH_PAGE_END - PSTORAGE_NUM_OF_PAGES - 1) \
                                    * PSTORAGE_FLASH_PAGE_SIZE)                                 /**< Start address for persistent data, configurable according to system requirements. */
#define PSTORAGE_DATA_END_ADDR      ((PSTORAGE_FLASH_PAGE_END - 1) * PSTORAGE_FLASH_PAGE_SIZE)  /**< End address for persistent data, configurable according to system requirements. */
#define PSTORAGE_SWAP_ADDR          PSTORAGE_DATA_END_ADDR                                      /**< Top-most page is used as swap area for clear and update. */

#define PSTORAGE_MAX_BLOCK_SIZE     PSTORAGE_FLASH_PAGE_SIZE                                    /**< Maximum size of block that can be registered with the module. Should be configured based on system requirements. And should be greater than or equal to the minimum size. */
#define PSTORAGE_CMD_QUEUE_SIZE     2                                                           /**< Maximum number of flash access commands that can be maintained by the module for all applications. Configurable. */


/** Abstracts persistently memory block identifier. */
typedef uint32_t pstorage_block_t;

typedef struct
{
    uint32_t            module_id;      /**< Module ID.*/
    pstorage_block_t    block_id;       /**< Block ID.*/
} pstorage_handle_t;

typedef uint16_t pstorage_size_t;      /** Size of length and offset fields. */

/**@brief Handles Flash Access Result Events. To be called in the system event dispatcher of the application. */
void pstorage_sys_event_handler (uint32_t sys_evt);

#endif // PSTORAGE_PL_H__

/** @} */
/** @endcond */
