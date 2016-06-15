/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
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
