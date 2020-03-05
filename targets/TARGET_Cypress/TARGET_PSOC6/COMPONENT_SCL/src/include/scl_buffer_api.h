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
 *  Provides prototypes and declarations for common APSTA functionalities
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
#define  SDIO_BLOCK_SIZE (64U)

/******************************************************
*                      Macros
******************************************************/

/******************************************************
*             Structures and Enumerations
******************************************************/
/**
 * Indicates the transmit/receive direction that the packet buffer has
 * been used for. This is needed if TX/RX pools are separate.
 */
typedef enum
{
    SCL_NETWORK_TX = 0, /**< Transmit direction */
    SCL_NETWORK_RX = 1 /**< Recieve direction */
} scl_buffer_dir_t;

/******************************************************
*             Function Prototypes
******************************************************/
/** Allocates a packet buffer
 *  Attempts to allocate a packet buffer of the requested size. This is done
 *  by allocating a pre-existing packet from a pool using a static buffer
 *  or by dynamically allocating memory. 
 *  
 *  @param   buffer    A pointer which receives the allocated packet buffer handle
 *  @param   direction Indicates transmit/receive direction that the packet buffer is
 *                      used for. This may be needed if tx/rx pools are separate.
 *  @param   size      The number of bytes to allocate.
 *  @param   wait      Whether to wait for a packet buffer to be available
 *
 *  @return            SCL_SUCCESS or error code
 *
 */
scl_result_t scl_host_buffer_get(scl_buffer_t *buffer, scl_buffer_dir_t direction,
                                 uint16_t size, uint32_t wait);

/** Releases a packet buffer
 *
 *  Implemented in the port layer interface, which will be specific to the
 *  buffering scheme in use.
 *  This function is used by SCL to indicate that it no longer requires
 *  a packet buffer. The buffer can then be released back into a pool for
 *  reuse or the dynamically allocated memory can be freed, according to
 *  how the packet was allocated.
 *  Returns void since SCL cannot do anything about failures.
 *
 *  @param   buffer    The handle of the packet buffer to be released.
 *  @param   direction Indicates the transmit/receive direction that the packet buffer has
 *                     been used for. This might be needed if TX/RX pools are separate.
 *
 */
void scl_buffer_release(scl_buffer_t buffer, scl_buffer_dir_t direction);

/** Retrieves the current pointer of a packet buffer
 *
 *  Implemented in the port layer interface which is specific to the
 *  buffering scheme in use.
 *  Since packet buffers usually need to be created with space at the
 *  beginning for additional headers, this function allows SCL to get
 *  the current 'front' location pointer.
 *
 *  @param   buffer   The handle of the packet buffer whose pointer is to be retrieved.
 *
 *  @return           The packet buffer's current pointer.
 */
uint8_t *scl_buffer_get_current_piece_data_pointer(scl_buffer_t buffer);

/** Retrieves the size of a packet buffer
 *
 *  Implemented in the port layer interface, which is specific to the
 *  buffering scheme in use.
 *  Since packet buffers usually need to be created with space at the
 *  beginning for additional headers, the memory block used to contain a packet buffer
 *  will often be larger than the current size of the packet buffer data.
 *  This function allows SCL to retrieve the current size of a packet buffer's data.
 *
 *  @param   buffer   The handle of the packet buffer whose size is to be retrieved.
 *
 *  @return           The size of the packet buffer.
 */
uint16_t scl_buffer_get_current_piece_size(scl_buffer_t buffer);
#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef _SCL_INTERNAL_BUFFER_API_H_ */

