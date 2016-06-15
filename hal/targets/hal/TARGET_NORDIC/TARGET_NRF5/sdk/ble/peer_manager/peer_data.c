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


