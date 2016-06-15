/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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
 
/**@file
 *
 * @defgroup dfu_bank_internal Device Firmware Update internal header for bank handling in DFU.
 * @{     
 *
 * @brief Device Firmware Update Bank handling module interface.
 *
 * @details This header is intended for shared definition and functions between single and dual bank 
 *         implementations used for DFU support. It is not supposed to be used for external access 
 *         to the DFU module.
 *  
 */
#ifndef DFU_BANK_INTERNAL_H__
#define DFU_BANK_INTERNAL_H__

#include <dfu_types.h>

/**@brief States of the DFU state machine. */
typedef enum
{
    DFU_STATE_INIT_ERROR,                                                           /**< State for: dfu_init(...) error. */
    DFU_STATE_IDLE,                                                                 /**< State for: idle. */
    DFU_STATE_PREPARING,                                                            /**< State for: preparing, indicates that the flash is being erased and no data packets can be processed. */
    DFU_STATE_RDY,                                                                  /**< State for: ready. */
    DFU_STATE_RX_INIT_PKT,                                                          /**< State for: receiving initialization packet. */
    DFU_STATE_RX_DATA_PKT,                                                          /**< State for: receiving data packet. */
    DFU_STATE_VALIDATE,                                                             /**< State for: validate. */
    DFU_STATE_WAIT_4_ACTIVATE                                                       /**< State for: waiting for dfu_image_activate(). */
} dfu_state_t;

#define APP_TIMER_PRESCALER         0                                               /**< Value of the RTC1 PRESCALER register. */
#define DFU_TIMEOUT_INTERVAL        APP_TIMER_TICKS(120000, APP_TIMER_PRESCALER)    /**< DFU timeout interval in units of timer ticks. */     

#define IS_UPDATING_SD(START_PKT)   ((START_PKT).dfu_update_mode & DFU_UPDATE_SD)   /**< Macro for determining if a SoftDevice update is ongoing. */
#define IS_UPDATING_BL(START_PKT)   ((START_PKT).dfu_update_mode & DFU_UPDATE_BL)   /**< Macro for determining if a Bootloader update is ongoing. */
#define IS_UPDATING_APP(START_PKT)  ((START_PKT).dfu_update_mode & DFU_UPDATE_APP)  /**< Macro for determining if a Application update is ongoing. */
#define IMAGE_WRITE_IN_PROGRESS()   (m_data_received > 0)                           /**< Macro for determining if an image write is in progress. */
#define IS_WORD_SIZED(SIZE)         ((SIZE & (sizeof(uint32_t) - 1)) == 0)          /**< Macro for checking that the provided is word sized. */

/**@cond NO_DOXYGEN */
static uint32_t                     m_data_received;                                /**< Amount of received data. */
/**@endcond */

/**@brief     Type definition of function used for preparing of the bank before receiving of a
 *            software image.
 *
 * @param[in] image_size  Size of software image being received.
 */
typedef void (*dfu_bank_prepare_t)(uint32_t image_size);

/**@brief     Type definition of function used for handling clear complete of the bank before 
 *            receiving of a software image.
 */
typedef void (*dfu_bank_cleared_t)(void);

/**@brief    Type definition of function used for activating of the software image received.
 *
 * @return  NRF_SUCCESS If the image has been successfully activated any other NRF_ERROR code in
 *          case of a failure.
 */
typedef uint32_t (*dfu_bank_activate_t)(void);

/**@brief Structure for holding of function pointers for needed prepare and activate procedure for
 *        the requested update procedure. 
 */
typedef struct
{
    dfu_bank_prepare_t  prepare;                                                    /**< Function pointer to the prepare function called on start of update procedure. */
    dfu_bank_cleared_t  cleared;                                                    /**< Function pointer to the cleared function called after prepare function completes. */
    dfu_bank_activate_t activate;                                                   /**< Function pointer to the activate function called on finalizing the update procedure. */
} dfu_bank_func_t;

#endif // DFU_BANK_INTERNAL_H__

/** @} */
