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

#include "ble_advdata.h"
#include "nordic_common.h"
#include "nrf_error.h"
#include "ble_gap.h"
#include "ble_srv_common.h"
#include "app_util.h"

// NOTE: For now, Security Manager Out of Band Flags (OOB) are omitted from the advertising data.

// Types of LE Bluetooth Device Address AD type
#define AD_TYPE_BLE_DEVICE_ADDR_TYPE_PUBLIC 0UL
#define AD_TYPE_BLE_DEVICE_ADDR_TYPE_RANDOM 1UL

static uint32_t tk_value_encode(ble_advdata_tk_value_t * p_tk_value,
                                uint8_t                * p_encoded_data,
                                uint16_t               * p_offset,
                                uint16_t                 max_size)
{
    int8_t i;

    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_TK_VALUE_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode LE Role.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_TK_VALUE_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;

    for (i = AD_TYPE_TK_VALUE_DATA_SIZE - 1; i >= 0; i--, (*p_offset)++)
    {
        p_encoded_data[*p_offset] = p_tk_value->tk[i];
    }

    return NRF_SUCCESS;
}

static uint32_t le_role_encode(ble_advdata_le_role_t   le_role,
                               uint8_t               * p_encoded_data,
                               uint16_t              * p_offset,
                               uint16_t                max_size)
{
    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_LE_ROLE_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode LE Role.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_LE_ROLE_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_LE_ROLE;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    switch(le_role)
    {
        case BLE_ADVDATA_ROLE_ONLY_PERIPH:
            p_encoded_data[*p_offset] = 0;
            break;
        case BLE_ADVDATA_ROLE_ONLY_CENTRAL:
            p_encoded_data[*p_offset] = 1;
            break;
        case BLE_ADVDATA_ROLE_BOTH_PERIPH_PREFERRED:
            p_encoded_data[*p_offset] = 2;
            break;
        case BLE_ADVDATA_ROLE_BOTH_CENTRAL_PREFERRED:
            p_encoded_data[*p_offset] = 3;
            break;
        default:
            return NRF_ERROR_INVALID_PARAM;
    }
    *p_offset += AD_TYPE_LE_ROLE_DATA_SIZE;

    return NRF_SUCCESS;
}

static uint32_t ble_device_addr_encode(uint8_t  * p_encoded_data,
                                       uint16_t * p_offset,
                                       uint16_t   max_size)
{
    uint32_t err_code;
    ble_gap_addr_t device_addr;

    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_BLE_DEVICE_ADDR_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Get BLE address
    err_code = sd_ble_gap_address_get(&device_addr);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Encode LE Bluetooth Device Address
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE +
                                               AD_TYPE_BLE_DEVICE_ADDR_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    memcpy(&p_encoded_data[*p_offset], &device_addr.addr[0], BLE_GAP_ADDR_LEN);
    *p_offset                 += BLE_GAP_ADDR_LEN;
    if(BLE_GAP_ADDR_TYPE_PUBLIC == device_addr.addr_type)
    {
        p_encoded_data[*p_offset] = AD_TYPE_BLE_DEVICE_ADDR_TYPE_PUBLIC;
    }
    else
    {
        p_encoded_data[*p_offset] = AD_TYPE_BLE_DEVICE_ADDR_TYPE_RANDOM;
    }
    *p_offset += AD_TYPE_BLE_DEVICE_ADDR_TYPE_SIZE;

    return NRF_SUCCESS;
}

static uint32_t name_encode(const ble_advdata_t * p_advdata,
                            uint8_t             * p_encoded_data,
                            uint16_t            * p_offset,
                            uint16_t              max_size)
{
    uint32_t err_code;
    uint16_t rem_adv_data_len;
    uint16_t actual_length;
    uint8_t  adv_data_format;


    // Validate parameters
    if((BLE_ADVDATA_SHORT_NAME == p_advdata->name_type) && (0 == p_advdata->short_name_len))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Check for buffer overflow.
    if ( (((*p_offset) + ADV_AD_DATA_OFFSET) > max_size) ||
         ( (BLE_ADVDATA_SHORT_NAME == p_advdata->name_type) &&
           (((*p_offset) + ADV_AD_DATA_OFFSET + p_advdata->short_name_len) > max_size)))
    {
        return NRF_ERROR_DATA_SIZE;
    }

    rem_adv_data_len = max_size - (*p_offset) - ADV_AD_DATA_OFFSET;
    actual_length    = rem_adv_data_len;

    // Get GAP device name and length
    err_code = sd_ble_gap_device_name_get(&p_encoded_data[(*p_offset) + ADV_AD_DATA_OFFSET],
                                          &actual_length);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    // Check if device intend to use short name and it can fit available data size.
    if ((p_advdata->name_type == BLE_ADVDATA_FULL_NAME) && (actual_length <= rem_adv_data_len))
    {
        // Complete device name can fit, setting Complete Name in Adv Data.
        adv_data_format = BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME;
    }
    else
    {
        // Else short name needs to be used. Or application has requested use of short name.
        adv_data_format = BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME;

        // If application has set a preference on the short name size, it needs to be considered,
        // else fit what can be fit.
        if ((BLE_ADVDATA_SHORT_NAME == p_advdata->name_type) &&
                (p_advdata->short_name_len <= rem_adv_data_len))
        {
            // Short name fits available size.
            actual_length = p_advdata->short_name_len;
        }
        // Else whatever can fit the data buffer will be packed.
        else
        {
            actual_length = rem_adv_data_len;
        }
    }

    // There is only 1 byte intended to encode length which is (actual_length + ADV_AD_TYPE_FIELD_SIZE)
    if(actual_length > (0x00FF - ADV_AD_TYPE_FIELD_SIZE))
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Complete name field in encoded data.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + actual_length);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = adv_data_format;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    *p_offset                 += actual_length;

    return NRF_SUCCESS;
}


static uint32_t appearance_encode(uint8_t  * p_encoded_data,
                                  uint16_t * p_offset,
                                  uint16_t   max_size)
{
    uint32_t err_code;
    uint16_t appearance;

    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_APPEARANCE_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Get GAP appearance field.
    err_code = sd_ble_gap_appearance_get(&appearance);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Encode Length, AD Type and Appearance.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_APPEARANCE_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_APPEARANCE;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    *p_offset                 += uint16_encode(appearance, &p_encoded_data[*p_offset]);

    return NRF_SUCCESS;
}

static uint32_t flags_encode(int8_t     flags,
                             uint8_t  * p_encoded_data,
                             uint16_t * p_offset,
                             uint16_t   max_size)
{
    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_FLAGS_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode flags.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_FLAGS_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_FLAGS;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    p_encoded_data[*p_offset]  = flags;
    *p_offset                 += AD_TYPE_FLAGS_DATA_SIZE;

    return NRF_SUCCESS;
}

static uint32_t sec_mgr_oob_flags_encode(uint8_t    oob_flags,
                                         uint8_t  * p_encoded_data,
                                         uint16_t * p_offset,
                                         uint16_t   max_size)
{
    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_OOB_FLAGS_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode flags.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_OOB_FLAGS_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    p_encoded_data[*p_offset]  = oob_flags;
    *p_offset                 += AD_TYPE_OOB_FLAGS_DATA_SIZE;

    return NRF_SUCCESS;
}

static uint32_t tx_power_level_encode(int8_t     tx_power_level,
                                      uint8_t  * p_encoded_data,
                                      uint16_t * p_offset,
                                      uint16_t   max_size)
{
    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_TX_POWER_LEVEL_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode TX Power Level.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE +
                                                  AD_TYPE_TX_POWER_LEVEL_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_TX_POWER_LEVEL;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    p_encoded_data[*p_offset]  = tx_power_level;
    *p_offset                 += AD_TYPE_TX_POWER_LEVEL_DATA_SIZE;

    return NRF_SUCCESS;
}


static uint32_t uuid_list_sized_encode(const ble_advdata_uuid_list_t * p_uuid_list,
                                       uint8_t                         adv_type,
                                       uint8_t                         uuid_size,
                                       uint8_t                       * p_encoded_data,
                                       uint16_t                      * p_offset,
                                       uint16_t                        max_size)
{
    int      i;
    bool     is_heading_written = false;
    uint16_t start_pos          = *p_offset;
    uint16_t length;

    for (i = 0; i < p_uuid_list->uuid_cnt; i++)
    {
        uint32_t   err_code;
        uint8_t    encoded_size;
        ble_uuid_t uuid = p_uuid_list->p_uuids[i];
        
        // Find encoded uuid size.
        err_code = sd_ble_uuid_encode(&uuid, &encoded_size, NULL);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        // Check size.
        if (encoded_size == uuid_size)
        {
            uint8_t heading_bytes = (is_heading_written) ? 0 : ADV_AD_DATA_OFFSET;
            
            // Check for buffer overflow
            if (((*p_offset) + encoded_size + heading_bytes) > max_size)
            {
                return NRF_ERROR_DATA_SIZE;
            }

            if (!is_heading_written)
            {
                // Write AD structure heading.
                *p_offset                 += ADV_LENGTH_FIELD_SIZE;
                p_encoded_data[*p_offset]  = adv_type;
                *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
                is_heading_written         = true;
            }

            // Write UUID.
            err_code = sd_ble_uuid_encode(&uuid, &encoded_size, &p_encoded_data[*p_offset]);
            if (err_code != NRF_SUCCESS)
            {
                return err_code;
            }
            *p_offset += encoded_size;
        }
    }

    if (is_heading_written)
    {
        // Write length.
        length = (*p_offset) - (start_pos + ADV_LENGTH_FIELD_SIZE);
        // There is only 1 byte intended to encode length
        if(length > 0x00FF)
        {
            return NRF_ERROR_DATA_SIZE;
        }
        p_encoded_data[start_pos] = (uint8_t)length;
    }

    return NRF_SUCCESS;
}


static uint32_t uuid_list_encode(const ble_advdata_uuid_list_t * p_uuid_list,
                                 uint8_t                         adv_type_16,
                                 uint8_t                         adv_type_128,
                                 uint8_t                       * p_encoded_data,
                                 uint16_t                      * p_offset,
                                 uint16_t                        max_size)
{
    uint32_t err_code;

    // Encode 16 bit UUIDs.
    err_code = uuid_list_sized_encode(p_uuid_list,
                                      adv_type_16,
                                      sizeof(uint16_le_t),
                                      p_encoded_data,
                                      p_offset,
                                      max_size);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Encode 128 bit UUIDs.
    err_code = uuid_list_sized_encode(p_uuid_list,
                                      adv_type_128,
                                      sizeof(ble_uuid128_t),
                                      p_encoded_data,
                                      p_offset,
                                      max_size);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


static uint32_t conn_int_check(const ble_advdata_conn_int_t *p_conn_int)
{
    // Check Minimum Connection Interval.
    if ((p_conn_int->min_conn_interval < 0x0006) ||
        (
            (p_conn_int->min_conn_interval > 0x0c80) &&
            (p_conn_int->min_conn_interval != 0xffff)
        )
       )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Check Maximum Connection Interval.
    if ((p_conn_int->max_conn_interval < 0x0006) || 
        (
            (p_conn_int->max_conn_interval > 0x0c80) && 
            (p_conn_int->max_conn_interval != 0xffff)
        )
       )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Make sure Minimum Connection Interval is not bigger than Maximum Connection Interval.
    if ((p_conn_int->min_conn_interval != 0xffff) &&
        (p_conn_int->max_conn_interval != 0xffff) &&
        (p_conn_int->min_conn_interval > p_conn_int->max_conn_interval)
        )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    return NRF_SUCCESS;
}


static uint32_t conn_int_encode(const ble_advdata_conn_int_t * p_conn_int,
                                uint8_t                      * p_encoded_data,
                                uint16_t                     * p_offset,
                                uint16_t                       max_size)
{
    uint32_t err_code;

    // Check for buffer overflow.
    if (((*p_offset) + AD_TYPE_CONN_INT_SIZE) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Check parameters.
    err_code = conn_int_check(p_conn_int);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Encode Length and AD Type.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + AD_TYPE_CONN_INT_DATA_SIZE);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;

    // Encode Minimum and Maximum Connection Intervals.
    *p_offset += uint16_encode(p_conn_int->min_conn_interval, &p_encoded_data[*p_offset]);
    *p_offset += uint16_encode(p_conn_int->max_conn_interval, &p_encoded_data[*p_offset]);

    return NRF_SUCCESS;
}


static uint32_t manuf_specific_data_encode(const ble_advdata_manuf_data_t * p_manuf_sp_data,
                                           uint8_t                        * p_encoded_data,
                                           uint16_t                       * p_offset,
                                           uint16_t                         max_size)
{
    uint32_t data_size = AD_TYPE_MANUF_SPEC_DATA_ID_SIZE + p_manuf_sp_data->data.size;

    // Check for buffer overflow.
    if (((*p_offset) + ADV_AD_DATA_OFFSET + data_size) > max_size)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // There is only 1 byte intended to encode length which is (data_size + ADV_AD_TYPE_FIELD_SIZE)
    if(data_size > (0x00FF - ADV_AD_TYPE_FIELD_SIZE))
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode Length and AD Type.
    p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + data_size);
    *p_offset                 += ADV_LENGTH_FIELD_SIZE;
    p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
    *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;
    
    // Encode Company Identifier.
    *p_offset += uint16_encode(p_manuf_sp_data->company_identifier, &p_encoded_data[*p_offset]);
    
    // Encode additional manufacturer specific data.
    if (p_manuf_sp_data->data.size > 0)
    {
        if (p_manuf_sp_data->data.p_data == NULL)
        {
            return NRF_ERROR_INVALID_PARAM;
        }
        memcpy(&p_encoded_data[*p_offset], p_manuf_sp_data->data.p_data, p_manuf_sp_data->data.size);
        *p_offset += p_manuf_sp_data->data.size;
    }

    return NRF_SUCCESS;
}

// Implemented only for 16-bit UUIDs
static uint32_t service_data_encode(const ble_advdata_t * p_advdata,
                                    uint8_t             * p_encoded_data,
                                    uint16_t            * p_offset,
                                    uint16_t              max_size)
{
    uint8_t i;

    // Check parameter consistency.
    if (p_advdata->p_service_data_array == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    for (i = 0; i < p_advdata->service_data_count; i++)
    {
        ble_advdata_service_data_t * p_service_data;
        uint32_t                     data_size;

        p_service_data = &p_advdata->p_service_data_array[i];
        // For now implemented only for 16-bit UUIDs
        data_size      = AD_TYPE_SERV_DATA_16BIT_UUID_SIZE + p_service_data->data.size;

        // There is only 1 byte intended to encode length which is (data_size + ADV_AD_TYPE_FIELD_SIZE)
        if(data_size > (0x00FF - ADV_AD_TYPE_FIELD_SIZE))
        {
            return NRF_ERROR_DATA_SIZE;
        }

        // Encode Length and AD Type.
        p_encoded_data[*p_offset]  = (uint8_t)(ADV_AD_TYPE_FIELD_SIZE + data_size);
        *p_offset                 += ADV_LENGTH_FIELD_SIZE;
        p_encoded_data[*p_offset]  = BLE_GAP_AD_TYPE_SERVICE_DATA;
        *p_offset                 += ADV_AD_TYPE_FIELD_SIZE;

        // Encode service 16-bit UUID.
        *p_offset += uint16_encode(p_service_data->service_uuid, &p_encoded_data[*p_offset]);

        // Encode additional service data.
        if (p_service_data->data.size > 0)
        {
            if (p_service_data->data.p_data == NULL)
            {
                return NRF_ERROR_INVALID_PARAM;
            }
            memcpy(&p_encoded_data[*p_offset], p_service_data->data.p_data, p_service_data->data.size);
            *p_offset += p_service_data->data.size;
        }
    }

    return NRF_SUCCESS;
}

uint32_t adv_data_encode(ble_advdata_t const * const p_advdata,
                         uint8_t             * const p_encoded_data,
                         uint16_t            * const p_len)
{
    uint32_t err_code = NRF_SUCCESS;
    uint16_t max_size = *p_len;
    *p_len = 0;

    //Encode Security Manager OOB Flags
    if (p_advdata->p_sec_mgr_oob_flags != NULL)
    {
        err_code = sec_mgr_oob_flags_encode(*p_advdata->p_sec_mgr_oob_flags,
                                             p_encoded_data,
                                             p_len,
                                             max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
		
    // Encode Security Manager TK value
    if (NULL != p_advdata->p_tk_value)
    {
        err_code = tk_value_encode(p_advdata->p_tk_value, p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode LE Role
    if (BLE_ADVDATA_ROLE_NOT_PRESENT != p_advdata->le_role)
    {
        err_code = le_role_encode(p_advdata->le_role, p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode LE Bluetooth Device Address
    if (p_advdata->include_ble_device_addr)
    {
        err_code = ble_device_addr_encode(p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode appearance.
    if (p_advdata->include_appearance)
    {
        err_code = appearance_encode(p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    //Encode Flags
    if(p_advdata->flags != 0 )
    {
        err_code = flags_encode(p_advdata->flags, p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    } 

    // Encode TX power level.
    if (p_advdata->p_tx_power_level != NULL)
    {
        err_code = tx_power_level_encode(*p_advdata->p_tx_power_level,
                                         p_encoded_data,
                                         p_len,
                                         max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    
    // Encode 'more available' uuid list.
    if (p_advdata->uuids_more_available.uuid_cnt > 0)
    {
        err_code = uuid_list_encode(&p_advdata->uuids_more_available,
                                    BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE,
                                    BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE,
                                    p_encoded_data,
                                    p_len,
                                    max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode 'complete' uuid list.
    if (p_advdata->uuids_complete.uuid_cnt > 0)
    {
        err_code = uuid_list_encode(&p_advdata->uuids_complete,
                                    BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE,
                                    BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE,
                                    p_encoded_data,
                                    p_len,
                                    max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode 'solicited service' uuid list.
    if (p_advdata->uuids_solicited.uuid_cnt > 0)
    {
        err_code = uuid_list_encode(&p_advdata->uuids_solicited,
                                    BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT,
                                    BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT,
                                    p_encoded_data,
                                    p_len,
                                    max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode Slave Connection Interval Range.
    if (p_advdata->p_slave_conn_int != NULL)
    {
        err_code = conn_int_encode(p_advdata->p_slave_conn_int, p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode Manufacturer Specific Data.
    if (p_advdata->p_manuf_specific_data != NULL)
    {
        err_code = manuf_specific_data_encode(p_advdata->p_manuf_specific_data,
                                              p_encoded_data,
                                              p_len,
                                              max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode Service Data.
    if (p_advdata->service_data_count > 0)
    {
        err_code = service_data_encode(p_advdata, p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Encode name. WARNING: it is encoded last on purpose since too long device name is truncated.
    if (p_advdata->name_type != BLE_ADVDATA_NO_NAME)
    {
        err_code = name_encode(p_advdata, p_encoded_data, p_len, max_size);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    return err_code;
}


static uint32_t advdata_check(const ble_advdata_t * p_advdata)
{
    // Flags must be included in advertising data, and the BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED flag must be set.
    if (
        ((p_advdata->flags & BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED) == 0)
       )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    return NRF_SUCCESS;
}


static uint32_t srdata_check(const ble_advdata_t * p_srdata)
{
    // Flags shall not be included in the scan response data.
    if (p_srdata->flags)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    return NRF_SUCCESS;
}


uint32_t ble_advdata_set(const ble_advdata_t * p_advdata, const ble_advdata_t * p_srdata)
{
    uint32_t  err_code;
    uint16_t  len_advdata = BLE_GAP_ADV_MAX_SIZE;
    uint16_t  len_srdata  = BLE_GAP_ADV_MAX_SIZE;
    uint8_t   encoded_advdata[BLE_GAP_ADV_MAX_SIZE];
    uint8_t   encoded_srdata[BLE_GAP_ADV_MAX_SIZE];
    uint8_t * p_encoded_advdata;
    uint8_t * p_encoded_srdata;

    // Encode advertising data (if supplied).
    if (p_advdata != NULL)
    {
        err_code = advdata_check(p_advdata);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        err_code = adv_data_encode(p_advdata, encoded_advdata, &len_advdata);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
        p_encoded_advdata = encoded_advdata;
    }
    else
    {
        p_encoded_advdata = NULL;
        len_advdata = 0;
    }

    // Encode scan response data (if supplied).
    if (p_srdata != NULL)
    {
        err_code = srdata_check(p_srdata);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        err_code = adv_data_encode(p_srdata, encoded_srdata, &len_srdata);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
        p_encoded_srdata = encoded_srdata;
    }
    else
    {
        p_encoded_srdata = NULL;
        len_srdata = 0;
    }

    // Pass encoded advertising data and/or scan response data to the stack.
    return sd_ble_gap_adv_data_set(p_encoded_advdata, len_advdata, p_encoded_srdata, len_srdata);
}
