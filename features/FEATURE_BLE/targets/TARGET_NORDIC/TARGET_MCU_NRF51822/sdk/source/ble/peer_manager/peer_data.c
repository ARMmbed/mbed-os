/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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

#include <stdint.h>
#include <string.h>
#include "peer_manager_types.h"
#include "fds.h"



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
        switch (p_peer_data->data_type)
        {
            case PM_PEER_DATA_ID_BONDING:
                p_chunks[0].p_data       = p_peer_data->data.p_bonding_data;
                p_chunks[0].length_words = p_peer_data->length_words;
                *p_n_chunks = 1;
                break;
            case PM_PEER_DATA_ID_SERVICE_CHANGED_PENDING:
                p_chunks[0].p_data       = p_peer_data->data.p_service_changed_pending;
                p_chunks[0].length_words = p_peer_data->length_words;
                *p_n_chunks = 1;
                break;
            case PM_PEER_DATA_ID_GATT_LOCAL:
                p_chunks[0].p_data       = p_peer_data->data.p_local_gatt_db;
                p_chunks[0].length_words = PM_N_WORDS(PM_LOCAL_DB_LEN_OVERHEAD_BYTES);
                p_chunks[1].p_data       = p_peer_data->data.p_local_gatt_db->p_data;
                p_chunks[1].length_words = p_peer_data->length_words - p_chunks[0].length_words;
                *p_n_chunks = 2;
                break;
            case PM_PEER_DATA_ID_GATT_REMOTE:
                p_chunks[0].p_data       = p_peer_data->data.p_remote_gatt_db;
                p_chunks[0].length_words = PM_N_WORDS(PM_REMOTE_DB_LEN_OVERHEAD_BYTES);
                p_chunks[1].p_data       = p_peer_data->data.p_remote_gatt_db->p_data;
                p_chunks[1].length_words = p_peer_data->length_words - p_chunks[0].length_words;
                *p_n_chunks = 2;
                break;
            case PM_PEER_DATA_ID_APPLICATION:
                p_chunks[0].p_data       = p_peer_data->data.p_application_data;
                p_chunks[0].length_words = p_peer_data->length_words;
                *p_n_chunks = 1;
                break;
            default:
                *p_n_chunks = 0;
                break;
        }
    }
}


ret_code_t peer_data_deserialize(pm_peer_data_flash_t const * p_in_data, pm_peer_data_t * p_out_data)
{
    if ((p_in_data == NULL) || (p_out_data == NULL))
    {
        return NRF_ERROR_NULL;
    }
    else
    {
        if (p_out_data->length_words < p_in_data->length_words)
        {
            p_out_data->length_words = p_in_data->length_words;
            return NRF_ERROR_NO_MEM;
        }
        p_out_data->length_words = p_in_data->length_words;
        p_out_data->data_type    = p_in_data->data_type;

        switch (p_in_data->data_type)
        {
            case PM_PEER_DATA_ID_BONDING:
                *p_out_data->data.p_bonding_data = *p_in_data->data.p_bonding_data;
                break;
            case PM_PEER_DATA_ID_SERVICE_CHANGED_PENDING:
                *p_out_data->data.p_service_changed_pending = *p_in_data->data.p_service_changed_pending;
                break;
            case PM_PEER_DATA_ID_GATT_LOCAL:
                if (p_out_data->data.p_local_gatt_db->p_data == NULL)
                {
                    return NRF_ERROR_NULL;
                }
                if (p_out_data->data.p_local_gatt_db->len < p_in_data->data.p_local_gatt_db->len)
                {
                    p_out_data->data.p_local_gatt_db->len = p_in_data->data.p_local_gatt_db->len;
                    return NRF_ERROR_NO_MEM;
                }
                else
                {
                    p_out_data->data.p_local_gatt_db->flags = p_in_data->data.p_local_gatt_db->flags;
                    p_out_data->data.p_local_gatt_db->len   = p_in_data->data.p_local_gatt_db->len;
                    memcpy(p_out_data->data.p_local_gatt_db->p_data,
                           p_in_data->data.p_local_gatt_db->p_data,
                           p_in_data->data.p_local_gatt_db->len);
                }
                break;
            case PM_PEER_DATA_ID_GATT_REMOTE:
                if (p_out_data->data.p_remote_gatt_db->p_data == NULL)
                {
                    return NRF_ERROR_NULL;
                }
                if (p_out_data->data.p_remote_gatt_db->service_count < p_in_data->data.p_remote_gatt_db->service_count)
                {
                    p_out_data->data.p_remote_gatt_db->service_count = p_in_data->data.p_remote_gatt_db->service_count;
                    return NRF_ERROR_NO_MEM;
                }
                else
                {
                    p_out_data->data.p_remote_gatt_db->service_count = p_in_data->data.p_remote_gatt_db->service_count;
                    memcpy(p_out_data->data.p_remote_gatt_db->p_data,
                           p_in_data->data.p_remote_gatt_db->p_data,
                           p_in_data->data.p_remote_gatt_db->service_count * sizeof(ble_gatt_db_srv_t));
                }
                break;
            case PM_PEER_DATA_ID_APPLICATION:
                memcpy(p_out_data->data.p_application_data,
                       p_in_data->data.p_application_data,
                       p_in_data->length_words * 4);
                break;
            default:
                break;
        }
    }
    return NRF_SUCCESS;
}


