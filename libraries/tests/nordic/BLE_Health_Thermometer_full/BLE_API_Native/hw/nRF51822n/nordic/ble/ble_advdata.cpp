/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

#include "ble_advdata.h"
#include "nordic_common.h"
#include "nrf_error.h"
#include "ble_gap.h"
#include "ble_srv_common.h"
#include "app_util.h"


// Offset from where advertisement data other than flags information can start.
#define ADV_FLAG_OFFSET    2

// Offset for Advertising Data.
// Offset is 2 as each Advertising Data contain 1 octet of Adveritising Data Type and
// one octet Advertising Data Length.
#define ADV_DATA_OFFSET    2

// NOTE: For now, Security Manager TK Value and Security Manager Out of Band Flags (OOB) are omitted
//       from the advertising data.


static uint32_t name_encode(const ble_advdata_t * p_advdata,
                            uint8_t *             p_encoded_data,
                            uint8_t *             p_len)
{
    uint32_t err_code;
    uint16_t rem_adv_data_len;
    uint16_t actual_length;
    uint8_t  adv_data_format;
    uint8_t  adv_offset;
    
    adv_offset = *p_len;
    
    
    // Check for buffer overflow.
    if ((adv_offset + ADV_DATA_OFFSET > BLE_GAP_ADV_MAX_SIZE) ||
       ((p_advdata->short_name_len + ADV_DATA_OFFSET) > BLE_GAP_ADV_MAX_SIZE))
    {
        return NRF_ERROR_DATA_SIZE;
    }
    actual_length = rem_adv_data_len = (BLE_GAP_ADV_MAX_SIZE - adv_offset - ADV_FLAG_OFFSET);

    // Get GAP device name and length
    err_code = sd_ble_gap_device_name_get(&p_encoded_data[adv_offset + ADV_DATA_OFFSET],
                                          &actual_length);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    // Check if device internd to use short name and it can fit available data size.
    if ((p_advdata->name_type == BLE_ADVDATA_FULL_NAME) && (actual_length <= rem_adv_data_len))
    {
        // Complete device name can fit, setting Complete Name in Adv Data.
        adv_data_format = BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME;
        rem_adv_data_len = actual_length;
    }
    else
    {
        // Else short name needs to be used. Or application has requested use of short name.
        adv_data_format = BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME;
        
        // If application has set a preference on the short name size, it needs to be considered,
        // else fit what can be fit.
        if ((p_advdata->short_name_len != 0) && (p_advdata->short_name_len <= rem_adv_data_len))
        {
            // Short name fits available size.
            rem_adv_data_len = p_advdata->short_name_len;
        }
        // Else whatever can fit the data buffer will be packed.
        else
        {
            rem_adv_data_len = actual_length;
        }
    }
    
    // Complete name field in encoded data.
    p_encoded_data[adv_offset++] = rem_adv_data_len + 1;
    p_encoded_data[adv_offset++] = adv_data_format;
    (*p_len) += (rem_adv_data_len + ADV_DATA_OFFSET);
    
    return NRF_SUCCESS;
}


static uint32_t appearance_encode(uint8_t * p_encoded_data, uint8_t * p_len)
{
    uint32_t err_code;
    uint16_t appearance;

    // Check for buffer overflow.
    if ((*p_len) + 4 > BLE_GAP_ADV_MAX_SIZE)
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
    p_encoded_data[(*p_len)++] = 3;
    p_encoded_data[(*p_len)++] = BLE_GAP_AD_TYPE_APPEARANCE;

    (*p_len) += uint16_encode(appearance, &p_encoded_data[*p_len]);
    
    return NRF_SUCCESS;
}


static uint32_t uint8_array_encode(const uint8_array_t * p_uint8_array,
                                   uint8_t               adv_type,
                                   uint8_t *             p_encoded_data,
                                   uint8_t *             p_len)
{
    // Check parameter consistency.
    if (p_uint8_array->p_data == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Check for buffer overflow.
    if ((*p_len) + ADV_DATA_OFFSET + p_uint8_array->size > BLE_GAP_ADV_MAX_SIZE)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode Length and AD Type.
    p_encoded_data[(*p_len)++] = 1 + p_uint8_array->size;
    p_encoded_data[(*p_len)++] = adv_type;
    
    // Encode array.
    memcpy(&p_encoded_data[*p_len], p_uint8_array->p_data, p_uint8_array->size);
    (*p_len) += p_uint8_array->size;
    
    return NRF_SUCCESS;
}


static uint32_t tx_power_level_encode(int8_t    tx_power_level,
                                      uint8_t * p_encoded_data,
                                      uint8_t * p_len)
{
    // Check for buffer overflow.
    if ((*p_len) + 3 > BLE_GAP_ADV_MAX_SIZE)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode TX Power Level.
    p_encoded_data[(*p_len)++] = 2;
    p_encoded_data[(*p_len)++] = BLE_GAP_AD_TYPE_TX_POWER_LEVEL;
    p_encoded_data[(*p_len)++] = (uint8_t)tx_power_level;
    
    return NRF_SUCCESS;
}


static uint32_t uuid_list_sized_encode(const ble_advdata_uuid_list_t * p_uuid_list,
                                       uint8_t                         adv_type,
                                       uint8_t                         uuid_size,
                                       uint8_t *                       p_encoded_data,
                                       uint8_t *                       p_len)
{
    int     i;
    bool    is_heading_written = false;
    uint8_t start_pos = *p_len;
    
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
            uint8_t heading_bytes = (is_heading_written) ? 0 : 2;
            
            // Check for buffer overflow
            if (*p_len + encoded_size + heading_bytes > BLE_GAP_ADV_MAX_SIZE)
            {
                return NRF_ERROR_DATA_SIZE;
            }
            
            if (!is_heading_written)
            {
                // Write AD structure heading.
                (*p_len)++;
                p_encoded_data[(*p_len)++] = adv_type;
                is_heading_written = true;
            }
            
            // Write UUID.
            err_code = sd_ble_uuid_encode(&uuid, &encoded_size, &p_encoded_data[*p_len]);
            if (err_code != NRF_SUCCESS)
            {
                return err_code;
            }
            (*p_len) += encoded_size;
        }
    }
    
    if (is_heading_written)
    {
        // Write length.
        p_encoded_data[start_pos] = (*p_len) - (start_pos + 1);
    }
    
    return NRF_SUCCESS;
}


static uint32_t uuid_list_encode(const ble_advdata_uuid_list_t * p_uuid_list,
                                 uint8_t                         adv_type_16,
                                 uint8_t                         adv_type_128,
                                 uint8_t *                       p_encoded_data,
                                 uint8_t *                       p_len)
{
    uint32_t err_code;
    
    // Encode 16 bit UUIDs.
    err_code = uuid_list_sized_encode(p_uuid_list,
                                      adv_type_16,
                                      sizeof(uint16_le_t),
                                      p_encoded_data,
                                      p_len);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    // Encode 128 bit UUIDs.
    err_code = uuid_list_sized_encode(p_uuid_list,
                                      adv_type_128,
                                      sizeof(ble_uuid128_t),
                                      p_encoded_data,
                                      p_len);
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
                                uint8_t *                      p_encoded_data,
                                uint8_t *                      p_len)
{
    uint32_t err_code;

    // Check for buffer overflow.
    if ((*p_len) + ADV_DATA_OFFSET + 2 * sizeof(uint16_le_t) > BLE_GAP_ADV_MAX_SIZE)
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
    p_encoded_data[(*p_len)++] = 1 + 2 * sizeof(uint16_le_t);
    p_encoded_data[(*p_len)++] = BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE;
    
    // Encode Minimum and Maximum Connection Intervals.
    (*p_len) += uint16_encode(p_conn_int->min_conn_interval, &p_encoded_data[*p_len]);
    (*p_len) += uint16_encode(p_conn_int->max_conn_interval, &p_encoded_data[*p_len]);
    
    return NRF_SUCCESS;
}


static uint32_t manuf_specific_data_encode(const ble_advdata_manuf_data_t * p_manuf_sp_data,
                                           uint8_t *                        p_encoded_data,
                                           uint8_t *                        p_len)
{
    uint8_t data_size = sizeof(uint16_le_t) + p_manuf_sp_data->data.size;
    
    // Check for buffer overflow.
    if ((*p_len) + ADV_DATA_OFFSET + data_size > BLE_GAP_ADV_MAX_SIZE)
    {
        return NRF_ERROR_DATA_SIZE;
    }

    // Encode Length and AD Type.
    p_encoded_data[(*p_len)++] = 1 + data_size;
    p_encoded_data[(*p_len)++] = BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
    
    // Encode Company Identifier.
    (*p_len) += uint16_encode(p_manuf_sp_data->company_identifier, &p_encoded_data[*p_len]);
    
    // Encode additional manufacturer specific data.
    if (p_manuf_sp_data->data.size > 0)
    {
        if (p_manuf_sp_data->data.p_data == NULL)
        {
            return NRF_ERROR_INVALID_PARAM;
        }
        memcpy(&p_encoded_data[*p_len], p_manuf_sp_data->data.p_data, p_manuf_sp_data->data.size);
        (*p_len) += p_manuf_sp_data->data.size;
    }
    
    return NRF_SUCCESS;
}


static uint32_t service_data_encode(const ble_advdata_t * p_advdata,
                                    uint8_t *             p_encoded_data,
                                    uint8_t *             p_len)
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
        uint8_t                      data_size;
        
        p_service_data = &p_advdata->p_service_data_array[i];
        data_size      = sizeof(uint16_le_t) + p_service_data->data.size;
    
        // Encode Length and AD Type.
        p_encoded_data[(*p_len)++] = 1 + data_size;
        p_encoded_data[(*p_len)++] = BLE_GAP_AD_TYPE_SERVICE_DATA;
        
        // Encode service UUID.
        (*p_len) += uint16_encode(p_service_data->service_uuid, &p_encoded_data[*p_len]);
        
        // Encode additional service data.
        if (p_service_data->data.size > 0)
        {
            if (p_service_data->data.p_data == NULL)
            {
                return NRF_ERROR_INVALID_PARAM;
            }
            memcpy(&p_encoded_data[*p_len], p_service_data->data.p_data, p_service_data->data.size);
            (*p_len) += p_service_data->data.size;
        }
    }
    
    return NRF_SUCCESS;
}


static uint32_t adv_data_encode(const ble_advdata_t * p_advdata,
                                uint8_t *             p_encoded_data,
                                uint8_t *             p_len)
{
    uint32_t err_code = NRF_SUCCESS;
    
    *p_len = 0;
    
    // Encode name.
    if (p_advdata->name_type != BLE_ADVDATA_NO_NAME)
    {
        err_code = name_encode(p_advdata, p_encoded_data, p_len);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    
    // Encode appearance.
    if (p_advdata->include_appearance)
    {
        err_code = appearance_encode(p_encoded_data, p_len);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    
    // Encode flags.
    if (p_advdata->flags.size > 0)
    {
        err_code = uint8_array_encode(&p_advdata->flags,
                                      BLE_GAP_AD_TYPE_FLAGS,
                                      p_encoded_data,
                                      p_len);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    
    // Encode TX power level.
    if (p_advdata->p_tx_power_level != NULL)
    {
        err_code = tx_power_level_encode(*p_advdata->p_tx_power_level, p_encoded_data, p_len);
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
                                    p_len);
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
                                    p_len);
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
                                    p_len);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    
    // Encode Slave Connection Interval Range.
    if (p_advdata->p_slave_conn_int != NULL)
    {
        err_code = conn_int_encode(p_advdata->p_slave_conn_int, p_encoded_data, p_len);
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
                                              p_len);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    
    // Encode Service Data.
    if (p_advdata->service_data_count > 0)
    {
        err_code = service_data_encode(p_advdata, p_encoded_data, p_len);
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
    if ((p_advdata->flags.size == 0)      ||
        (p_advdata->flags.p_data == NULL) ||
        ((p_advdata->flags.p_data[0] & BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED) == 0)
    )
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    return NRF_SUCCESS;
}


static uint32_t srdata_check(const ble_advdata_t * p_srdata)
{
    // Flags shall not be included in the scan response data.
    if (p_srdata->flags.size > 0)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    return NRF_SUCCESS;
}


uint32_t ble_advdata_set(const ble_advdata_t * p_advdata, const ble_advdata_t * p_srdata)
{
    uint32_t  err_code;
    uint8_t   len_advdata = 0;
    uint8_t   len_srdata = 0;
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
    }

    // Pass encoded advertising data and/or scan response data to the stack.
    return sd_ble_gap_adv_data_set(p_encoded_advdata, len_advdata, p_encoded_srdata, len_srdata);
}
