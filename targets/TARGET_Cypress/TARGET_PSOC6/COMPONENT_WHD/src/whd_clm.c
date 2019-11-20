/*
 * Copyright 2019 Cypress Semiconductor Corporation
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

/** @file
 *  Provides generic clm blob file download functionality
 */

#include <stdlib.h>
#include "whd_clm.h"
#include "whd_wlioctl.h"
#include "whd_cdc_bdc.h"
#include "whd_debug.h"
#include "whd_int.h"
#include "whd_buffer_api.h"
#include "whd_resource_if.h"
#include "whd_resource_api.h"
#include "whd_types_int.h"

/******************************************************
* @cond       Constants
******************************************************/

/*
   Generic interface for downloading required data onto the dongle
 */
static int whd_download_wifi_clm_image(whd_interface_t ifp, const char *iovar, uint16_t flag, uint16_t dload_type,
                                       unsigned char *dload_buf, uint32_t len)
{
    wl_dload_data_t *dload_ptr = (wl_dload_data_t *)dload_buf;
    unsigned int dload_data_offset;
    whd_buffer_t buffer;
    uint8_t *iov_data;
    whd_driver_t whd_driver = ifp->whd_driver;

    dload_data_offset = offsetof(wl_dload_data_t, data);
    dload_ptr->flag = htod16( (DLOAD_HANDLER_VER << DLOAD_FLAG_VER_SHIFT) | flag );
    dload_ptr->dload_type = htod16(dload_type);
    dload_ptr->len = htod32(len - dload_data_offset);

    dload_ptr->crc = 0;

    whd_assert("dload buffer too large", len < 0xffffffff - 8);
    len = len + 8 - (len % 8);

    iov_data = (uint8_t *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, (uint16_t)len, iovar);
    CHECK_IOCTL_BUFFER(iov_data);
    memcpy(iov_data, (uint8_t *)dload_ptr, len);
    CHECK_RETURN(whd_cdc_send_iovar(ifp, CDC_SET, buffer, NULL) );
    return WHD_SUCCESS;
}

whd_result_t whd_process_clm_data(whd_interface_t ifp)
{
    whd_result_t ret = WHD_SUCCESS;
    uint32_t clm_blob_size;
    unsigned int size2alloc, data_offset;
    unsigned char *chunk_buf;
    uint16_t dl_flag = DL_BEGIN;
    unsigned int chunk_len;
    uint32_t size_read;
    uint8_t *image;
    uint32_t blocks_count = 0;
    uint16_t datalen = 0;
    uint32_t i, j, num_buff;
    unsigned int transfer_progress;
    whd_driver_t whd_driver = ifp->whd_driver;

    /* clm file size is the initial datalen value which is decremented */
    ret = whd_resource_size(whd_driver, WHD_RESOURCE_WLAN_CLM, &clm_blob_size);

    if (ret != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Fatal error: download_resource doesn't exist\n") );
        return ret;
    }

    ret = whd_get_resource_no_of_blocks(whd_driver, WHD_RESOURCE_WLAN_CLM, &blocks_count);
    if (ret != WHD_SUCCESS)
    {
        WPRINT_WHD_ERROR( ("Fatal error: download_resource blocks count not know\n") );
        return ret;
    }

    data_offset = offsetof(wl_dload_data_t, data);
    size2alloc = data_offset + BLOCK_SIZE;


    if ( (chunk_buf = (unsigned char *)malloc(size2alloc) ) != NULL )
    {
        memset(chunk_buf, 0, size2alloc);
        transfer_progress = 0;
        for (i = 0; i < blocks_count; i++)
        {
            whd_get_resource_block(whd_driver, WHD_RESOURCE_WLAN_CLM, i, (const uint8_t **)&image, &size_read);

            num_buff = (size_read + BLOCK_SIZE - 1) / BLOCK_SIZE;
            if (blocks_count != 1)
                transfer_progress = 0;

            for (j = 0; j < num_buff; j++)
            {
                if (size_read >= BLOCK_SIZE)
                    chunk_len = BLOCK_SIZE;
                else
                    chunk_len = (int)size_read;
                memcpy(chunk_buf + data_offset, &image[transfer_progress], chunk_len);

                if (datalen + chunk_len == clm_blob_size)
                {
                    dl_flag |= DL_END;
                }

                ret = whd_download_wifi_clm_image(ifp, IOVAR_STR_CLMLOAD, dl_flag, DL_TYPE_CLM, chunk_buf,
                                                  data_offset + chunk_len);
                dl_flag &= (uint16_t) ~DL_BEGIN;
                transfer_progress += chunk_len;
                size_read = size_read - chunk_len;
                datalen += chunk_len;
            }
        }

        free(chunk_buf);
        if (ret != WHD_SUCCESS)
        {
            whd_result_t ret_clmload_status;
            whd_buffer_t buffer;
            whd_buffer_t response;
            void *data;

            WPRINT_WHD_DEBUG( ("clmload (%" PRIu32 " byte file) failed with return %" PRIu32 "; ", clm_blob_size,
                               ret) );
            data = (int *)whd_cdc_get_iovar_buffer(whd_driver, &buffer, 4, IOVAR_STR_CLMLOAD_STATUS);
            CHECK_IOCTL_BUFFER(data);
            ret_clmload_status = whd_cdc_send_iovar(ifp, CDC_GET, buffer, &response);
            if (ret_clmload_status != WHD_SUCCESS)
            {
                WPRINT_WHD_DEBUG( ("clmload_status failed with return %lu\n", ret_clmload_status) );
            }
            else
            {
                uint8_t *clmload_status = (uint8_t *)whd_buffer_get_current_piece_data_pointer(whd_driver, response);
                if (clmload_status != NULL)
                {
                    WPRINT_WHD_DEBUG( ("clmload_status is %d\n", *clmload_status) );
                    CHECK_RETURN(whd_buffer_release(whd_driver, response, WHD_NETWORK_RX) );
                }
            }
        }
    }
    else
    {
        WPRINT_WHD_ERROR( ("Memory allocation failure, %s failed at %d \n", __func__, __LINE__) );
        ret = WHD_MALLOC_FAILURE;
    }

    return ret;
}

