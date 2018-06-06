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


#ifndef FSTORAGE_INTERNAL_DEFS_H__
#define FSTORAGE_INTERNAL_DEFS_H__

#include "nrf.h"


#define FS_FLAG_INITIALIZED         (1 << 0)  // The module has been initialized.
#define FS_FLAG_PROCESSING          (1 << 1)  // The module is processing flash operations.
// The module is waiting for a flash operation initiated by another module to complete.
#define FS_FLAG_FLASH_REQ_PENDING   (1 << 2)

#define FS_ERASED_WORD              (0xFFFFFFFF)

// Helper macros for section variables.
#define FS_SECTION_VARS_GET(i)          NRF_SECTION_VARS_GET((i), fs_config_t, fs_data)
#define FS_SECTION_VARS_COUNT           NRF_SECTION_VARS_COUNT(fs_config_t, fs_data)
#define FS_SECTION_VARS_START_ADDR      NRF_SECTION_VARS_START_ADDR(fs_data)
#define FS_SECTION_VARS_END_ADDR        NRF_SECTION_VARS_END_ADDR(fs_data)


// Register the section 'fs_data'.
//lint -save -e19
NRF_SECTION_VARS_REGISTER_SECTION(fs_data);
//lint -restore

// Declare symbols into the 'fs_data' section.
NRF_SECTION_VARS_REGISTER_SYMBOLS(fs_config_t, fs_data);
//lint -esym(526,fs_dataBase)
//lint -esym(526,fs_dataLimit)


// fstorage op-codes.
typedef enum
{
    FS_OP_NONE,   // No operation.
    FS_OP_STORE,  // Store data.
    FS_OP_ERASE   // Erase one or more flash pages.
} fs_op_code_t;


#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  // anonymous unions are enabled by default.
#endif

// fstorage operation.
// Encapsulates details of a flash operation to be executed by this module.
typedef struct
{
    fs_config_t  const * p_config;          // Application-specific fstorage configuration.
    fs_op_code_t         op_code;           // ID of the operation.
    union
    {
        struct
        {
            uint32_t const * p_src;         // Pointer to the data to be written to flash.
            uint32_t const * p_dest;        // Destination of the data in flash.
            uint16_t         length_words;  // Length of the data to be written, in words.
            uint16_t         offset;        // Write offset.
        } store;
        struct
        {
            uint16_t page;
            uint16_t pages_erased;
            uint16_t pages_to_erase;
        } erase;
    };
} fs_op_t;

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  // leave anonymous unions enabled.
#elif defined(__GNUC__)
  // anonymous unions are enabled by default.
#endif


// Queue of requested operations.
// This queue holds flash operations requested to the module.
// The data to be written to flash must be kept in memory until the write operation
// is completed, i.e., an event indicating completion is received.
typedef struct
{
    fs_op_t  op[FS_QUEUE_SIZE];  // Queue elements.
    uint32_t rp;                 // Index of the operation being processed.
    uint32_t count;              // Number of elements in the queue.
} fs_op_queue_t;


// Size of a flash page in bytes.
#if   defined (NRF51)
    #define FS_PAGE_SIZE    (1024)
#elif defined (NRF52)
    #define FS_PAGE_SIZE    (4096)
#endif


// Size of a flash page in words.
#define FS_PAGE_SIZE_WORDS  (FS_PAGE_SIZE / sizeof(uint32_t))


// Function to obtain the end of the flash space available to fstorage.
static uint32_t const * fs_flash_page_end_addr()
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];

    return  (uint32_t*)((bootloader_addr != FS_ERASED_WORD) ? bootloader_addr :
                                                              NRF_FICR->CODESIZE * FS_PAGE_SIZE);
}


// Macro to obtain the address of the last page.
// If there is a bootloader present the bootloader address read from UICR
// will act as the page beyond the end of the available flash storage.
#define FS_PAGE_END_ADDR    (fs_flash_page_end_addr())


#endif //__FSTORAGE_INTERNAL_DEFS_H
