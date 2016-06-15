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

/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/

#include "ble_tps.h"
#include <string.h>
#include "ble_srv_common.h"


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_tps       TX Power Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_tps_t * p_tps, ble_evt_t * p_ble_evt)
{
    p_tps->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


void ble_tps_on_ble_evt(ble_tps_t * p_tps, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_tps, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding TX Power Level characteristics.
 *
 * @param[in]   p_tps        TX Power Service structure.
 * @param[in]   p_tps_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t tx_power_level_char_add(ble_tps_t            * p_tps,
                                        const ble_tps_init_t * p_tps_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_TX_POWER_LEVEL_CHAR);

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_tps_init->tps_attr_md.read_perm;
    attr_md.write_perm = p_tps_init->tps_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    memset(&attr_char_value, 0, sizeof (attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof (int8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof (uint8_t);
    attr_char_value.p_value   = (uint8_t*)&p_tps_init->initial_tx_power_level;

    return sd_ble_gatts_characteristic_add(p_tps->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tps->tx_power_level_handles);
}


uint32_t ble_tps_init(ble_tps_t * p_tps, const ble_tps_init_t * p_tps_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Add service
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_TX_POWER_SERVICE);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_tps->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add TX Power Level characteristic
    return tx_power_level_char_add(p_tps, p_tps_init);
}


uint32_t ble_tps_tx_power_level_set(ble_tps_t * p_tps, int8_t tx_power_level)
{
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = (uint8_t*)&tx_power_level;

    // Update database
    return sd_ble_gatts_value_set(p_tps->conn_handle,
                                  p_tps->tx_power_level_handles.value_handle,
                                  &gatts_value);
}
