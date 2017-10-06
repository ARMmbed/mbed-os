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



#include "peer_data.h"

#include "peer_manager_types.h"
#include "fds.h"
#include "sdk_common.h"


void peer_data_parts_get(pm_peer_data_const_t const * p_peer_data, fds_record_chunk_t * p_chunks, uint16_t * p_n_chunks)
{
    if (p_n_chunks == NULL)
    {
    }
    else if ((p_peer_data == NULL) || (p_chunks == NULL))
    {
        *p_n_chunks = 0;
    }
    else
    {
        p_chunks[0].p_data       = p_peer_data->p_all_data;
        p_chunks[0].length_words = p_peer_data->length_words;
        *p_n_chunks = 1;
    }
}


ret_code_t peer_data_deserialize(pm_peer_data_flash_t const * p_in_data, pm_peer_data_t * p_out_data)
{
    ret_code_t err_code = NRF_SUCCESS;

    if ((p_in_data == NULL) || (p_out_data == NULL))
    {
        err_code = NRF_ERROR_NULL;
    }
    else
    {
        if (p_out_data->length_words < p_in_data->length_words)
        {
            p_out_data->length_words = p_in_data->length_words;
            err_code = NRF_ERROR_NO_MEM;
        }
        else
        {
            p_out_data->length_words = p_in_data->length_words;
            p_out_data->data_id      = p_in_data->data_id;

            memcpy(p_out_data->p_all_data, p_in_data->p_all_data, p_in_data->length_words * 4);
        }
    }
    return err_code;
}


