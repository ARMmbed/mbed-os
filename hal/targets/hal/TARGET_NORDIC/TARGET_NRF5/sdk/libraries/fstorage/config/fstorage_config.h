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

#ifndef FS_CONFIG_H__
#define FS_CONFIG_H__

/**
 * @defgroup fstorage_config fstorage configuration
 * @ingroup fstorage
 * @{
 *
 * @brief fstorage configuration options.
 */


/**@brief   Configures the size of fstorage internal queue.
 * @details Increase this if there are many users, or if it is likely that many operation will be
 *          queued at once without waiting for the previous operations to complete. In general,
 *          increase the queue size if you frequently receive @ref FS_ERR_QUEUE_FULL errors when
 *          calling @ref fs_store or @ref fs_erase.
 */
#define FS_QUEUE_SIZE       (4)

/**@brief   Configures how many times should fstorage attempt to execute an operation if
 *          the SoftDevice fails to schedule flash access due to high BLE activity.
 * @details Increase this value if fstorage events return the @ref FS_ERR_OPERATION_TIMEOUT error
 *          often.
 */
#define FS_OP_MAX_RETRIES   (3)


/**@brief   Configures the maximum number of words to be written to flash in a single operation.
 *          If data length exceeds this value, the data will be written down in several chunks,
 *          as necessary.
 *
 * @details Tweaking this value can increase the chances of the SoftDevice being able to fit
 *          flash operations in between radio activity. This value is bound by the maximum number
 *          of words which the SoftDevice can write to flash in a single call to
 *          @ref sd_flash_write, which is 256 words for nRF51 ICs and 1024 words for nRF52 ICs.
 */
#if   defined (NRF51)
    #define FS_MAX_WRITE_SIZE_WORDS     (256)
#elif defined (NRF52)
    #define FS_MAX_WRITE_SIZE_WORDS     (1024)
#endif

/** @} */

#endif // FS_CONFIG_H__

