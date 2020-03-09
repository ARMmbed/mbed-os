/*
 * (c) (2019-2020), Cypress Semiconductor Corporation. All rights reserved.
 *
 * This software, including source code, documentation and related materials
 * ("Software"), is owned by Cypress Semiconductor Corporation or one of its
 * subsidiaries ("Cypress") and is protected by and subject to worldwide patent
 * protection (United States and foreign), United States copyright laws and
 * international treaty provisions. Therefore, you may use this Software only
 * as provided in the license agreement accompanying the software package from
 * which you obtained this Software ("EULA").
 *
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software source
 * code solely for use in connection with Cypress' integrated circuit products.
 * Any reproduction, modification, translation, compilation, or representation
 * of this Software except as specified above is prohibited without the express
 * written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress' product in a High Risk Product, the manufacturer of such
 * system or application assumes all risk of such use and in doing so agrees to
 * indemnify Cypress against all liability.
 */

/** @file
 *  Provides declarations for buffer management functionality
 */
#ifndef _SCL_INTERNAL_BUFFER_API_H_
#define _SCL_INTERNAL_BUFFER_API_H_

#include "scl_types.h"
#include "scl_common.h"
#include <stdlib.h>
#include "cy_utils.h"
#include "memp.h"
#include "pbuf.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*                  Constants
******************************************************/
/**
 * Size of the SDIO block
 */
#define  SDIO_BLOCK_SIZE (64U)

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*             Structures and Enumerations
******************************************************/
/**
 * Indicates the transmit/receive direction that the buffer has
 * been used for. This is needed if transmit/receive pools are separate.
 */
typedef enum
{
    SCL_NETWORK_TX = 0, /**< Transmit direction */
    SCL_NETWORK_RX = 1 /**< Receive direction */
} scl_buffer_dir_t;

/******************************************************
*             Function Prototypes
******************************************************/
/** Allocates the SCL buffer.
 *
 *  Attempts to allocate a buffer of the requested size. A buffer
 *  is either allocated from a static pool of memory or allocated dynamically.
 *  
 *  @param   buffer    A pointer which receives the allocated buffer.
 *  @param   direction Indicates transmit/receive direction that the buffer is
 *                      used for. This may be needed if transmit/receive pools are separate.
 *  @param   size      The number of bytes to allocate.
 *  @param   wait      Time to wait for a buffer to be available in milli-seconds.
 *
 *  @return  SCL_SUCCESS or Error code
 *
 */
scl_result_t scl_host_buffer_get(scl_buffer_t *buffer, scl_buffer_dir_t direction,
                                 uint16_t size, uint32_t wait);

/** Releases the SCL buffer.
 *
 *  This function is used by SCL to indicate that it no longer requires
 *  the buffer. The buffer can then be released back into a pool for
 *  reuse or the dynamically allocated memory can be freed.
 *
 *  @param   buffer    The buffer to be released.
 *  @param   direction Indicates the transmit/receive direction that the buffer has
 *                     been used for. This might be needed if transmit/receive pools are separate.
 *
 */
void scl_buffer_release(scl_buffer_t buffer, scl_buffer_dir_t direction);

/** Retrieves the pointer to the payload of the buffer.
 *
 *  @param   buffer   The buffer whose payload pointer is to be retrieved.
 *
 *  @return  Pointer to the payload.
 */
uint8_t *scl_buffer_get_current_piece_data_pointer(scl_buffer_t buffer);

/** Retrieves the size of the buffer.
 *
 *  @param   buffer   The buffer whose size is to be retrieved.
 *
 *  @return  The size of the buffer.
 */
uint16_t scl_buffer_get_current_piece_size(scl_buffer_t buffer);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _SCL_INTERNAL_BUFFER_API_H_ */