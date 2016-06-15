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
