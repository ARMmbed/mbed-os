/***************************************************************************//**
* \file cy_network_buffer.c
*
* \brief
* Basic set of APIs for dealing with network packet buffers. This is used by WHD
* for relaying data between the network stack and the connectivity chip.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
*******************************************************************************/

#if defined(CYBSP_WIFI_CAPABLE)

#include <stdlib.h>
#include "cy_network_buffer.h"
#include "cy_utils.h"
#include "lwip/memp.h"
#define  SDIO_BLOCK_SIZE (64U)

whd_result_t cy_host_buffer_get(whd_buffer_t *buffer, whd_buffer_dir_t direction, unsigned short size, unsigned long timeout_ms)
{
    CY_UNUSED_PARAMETER( direction );
    CY_UNUSED_PARAMETER( timeout_ms );
    struct pbuf *p = NULL;
    if ( ( direction == WHD_NETWORK_TX) && ( size <= PBUF_POOL_BUFSIZE ) )
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
    	return WHD_SUCCESS;
    }
    else
    {
        return WHD_BUFFER_ALLOC_FAIL;
    }
}

void cy_buffer_release(whd_buffer_t buffer, whd_buffer_dir_t direction)
{
    CY_UNUSED_PARAMETER( direction );
    (void) pbuf_free( (struct pbuf *)buffer );
}

uint8_t *cy_buffer_get_current_piece_data_pointer(whd_buffer_t buffer)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf *pbuffer= (struct pbuf*) buffer;
    return (uint8_t*) pbuffer->payload;
}

uint16_t cy_buffer_get_current_piece_size(whd_buffer_t buffer)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf *pbuffer = (struct pbuf*) buffer;
    return (uint16_t) pbuffer->len;
}

whd_result_t cy_buffer_set_size(whd_buffer_t buffer, unsigned short size)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf * pbuffer = (struct pbuf *) buffer;

    if ( size > (unsigned short) WHD_LINK_MTU  + LWIP_MEM_ALIGN_SIZE(LWIP_MEM_ALIGN_SIZE(sizeof(struct pbuf))) + LWIP_MEM_ALIGN_SIZE(size) )
    {
        return WHD_PMK_WRONG_LENGTH;
    }

    pbuffer->tot_len = size;
    pbuffer->len = size;

    return CY_RSLT_SUCCESS;
}

whd_result_t cy_buffer_add_remove_at_front(whd_buffer_t *buffer, int32_t add_remove_amount)
{
    CY_ASSERT(buffer != NULL);
    struct pbuf **pbuffer = (struct pbuf**) buffer;

    if ( (u8_t) 0 != pbuf_header( *pbuffer, ( s16_t )( -add_remove_amount ) ) )
    {
        return WHD_PMK_WRONG_LENGTH;
    }

    return WHD_SUCCESS;
}

#endif /* defined(CYBSP_WIFI_CAPABLE) */
