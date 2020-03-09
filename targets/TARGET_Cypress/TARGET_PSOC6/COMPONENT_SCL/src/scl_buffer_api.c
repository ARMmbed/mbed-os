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

#include "scl_buffer_api.h"

/******************************************************
** @cond               Constants
*******************************************************/

/******************************************************
**                   Enumerations
*******************************************************/

/******************************************************
**               Function Declarations
*******************************************************/

/******************************************************
 *        Variables Definitions
 *****************************************************/

/******************************************************
*               Function Definitions
******************************************************/

scl_result_t scl_host_buffer_get(scl_buffer_t *buffer, scl_buffer_dir_t direction,
                                 uint16_t size, uint32_t wait)
{
    UNUSED_PARAMETER( direction );
    struct pbuf *p = NULL;
    if ( ( direction == SCL_NETWORK_TX) && ( size <= PBUF_POOL_BUFSIZE ) )
    {
        p = pbuf_alloc(PBUF_RAW, size, PBUF_POOL);
    }
    else
    {
        p = pbuf_alloc(PBUF_RAW, size+SDIO_BLOCK_SIZE, PBUF_RAM);
        if ( p != NULL )
        {
            p->len = size;
            p->tot_len -=  SDIO_BLOCK_SIZE;
        }
    }
    if (p != NULL )
    {
        *buffer = p;
        return SCL_SUCCESS;
    }
    else
    {
        return SCL_BUFFER_ALLOC_FAIL;
    }

}

void scl_buffer_release(scl_buffer_t buffer, scl_buffer_dir_t direction)
{
    UNUSED_PARAMETER( direction );
    (void) pbuf_free( (struct pbuf *)buffer );
}

uint8_t *scl_buffer_get_current_piece_data_pointer(scl_buffer_t buffer)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf *pbuffer= (struct pbuf*) buffer;
    return (uint8_t*) pbuffer->payload;
}

uint16_t scl_buffer_get_current_piece_size(scl_buffer_t buffer)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf*) buffer;
    return (uint16_t) pbuffer->len;
}

