/*
 * Copyright 2018-2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
    UNUSED_PARAMETER(direction);
    struct pbuf *p = NULL;
    if ((direction == SCL_NETWORK_TX) && (size <= PBUF_POOL_BUFSIZE)) {
        p = pbuf_alloc(PBUF_RAW, size, PBUF_POOL);
    } else {
        p = pbuf_alloc(PBUF_RAW, size + SDIO_BLOCK_SIZE, PBUF_RAM);
        if (p != NULL) {
            p->len = size;
            p->tot_len -=  SDIO_BLOCK_SIZE;
        }
    }
    if (p != NULL) {
        *buffer = p;
        return SCL_SUCCESS;
    } else {
        return SCL_BUFFER_ALLOC_FAIL;
    }

}

void scl_buffer_release(scl_buffer_t buffer, scl_buffer_dir_t direction)
{
    UNUSED_PARAMETER(direction);
    (void) pbuf_free((struct pbuf *)buffer);
}

uint8_t *scl_buffer_get_current_piece_data_pointer(scl_buffer_t buffer)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf *) buffer;
    return (uint8_t *) pbuffer->payload;
}

uint16_t scl_buffer_get_current_piece_size(scl_buffer_t buffer)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf *) buffer;
    return (uint16_t) pbuffer->len;
}

