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

#include "ble_lls.h"
#include <string.h>
#include "ble_hci.h"
#include "ble_srv_common.h"


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_lls   Link Loss Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_lls_t * p_lls, ble_evt_t * p_ble_evt)
{
    // Link reconnected, notify application with a no_alert event
    ble_lls_evt_t evt;

    p_lls->conn_handle     = p_ble_evt->evt.gap_evt.conn_handle;

    evt.evt_type           = BLE_LLS_EVT_LINK_LOSS_ALERT;
    evt.params.alert_level = BLE_CHAR_ALERT_LEVEL_NO_ALERT;
    p_lls->evt_handler(p_lls, &evt);
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_lls       Link Loss Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_lls_t * p_lls, ble_evt_t * p_ble_evt)
{
    uint8_t reason = p_ble_evt->evt.gap_evt.params.disconnected.reason;

    if (reason == BLE_HCI_CONNECTION_TIMEOUT)
    {
        // Link loss detected, notify application
        uint32_t      err_code;
        ble_lls_evt_t evt;

        evt.evt_type = BLE_LLS_EVT_LINK_LOSS_ALERT;

        err_code = ble_lls_alert_level_get(p_lls, &evt.params.alert_level);
        if (err_code == NRF_SUCCESS)
        {
            p_lls->evt_handler(p_lls, &evt);
        }
        else
        {
            if (p_lls->error_handler != NULL)
            {
                p_lls->error_handler(err_code);
            }
        }
    }
}


/**@brief Function for handling the Authentication Status event.
 *
 * @param[in]   p_lls       Link Loss Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_auth_status(ble_lls_t * p_lls, ble_evt_t * p_ble_evt)
{
    if (p_ble_evt->evt.gap_evt.params.auth_status.auth_status == BLE_GAP_SEC_STATUS_SUCCESS)
    {
        ble_lls_evt_t evt;

        evt.evt_type           = BLE_LLS_EVT_LINK_LOSS_ALERT;
        evt.params.alert_level = BLE_CHAR_ALERT_LEVEL_NO_ALERT;

        p_lls->evt_handler(p_lls, &evt);
    }
}


void ble_lls_on_ble_evt(ble_lls_t * p_lls, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_lls, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_lls, p_ble_evt);
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            on_auth_status(p_lls, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding Alert Level characteristics.
 *
 * @param[in]   p_lls        Link Loss Service structure.
 * @param[in]   p_lls_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t alert_level_char_add(ble_lls_t * p_lls, const ble_lls_init_t * p_lls_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t             initial_alert_level;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_ALERT_LEVEL_CHAR);
    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_lls_init->lls_attr_md.read_perm;
    attr_md.write_perm = p_lls_init->lls_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));
    initial_alert_level = p_lls_init->initial_alert_level;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof (uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof (uint8_t);
    attr_char_value.p_value   = &initial_alert_level;

    return sd_ble_gatts_characteristic_add(p_lls->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_lls->alert_level_handles);
}


uint32_t ble_lls_init(ble_lls_t * p_lls, const ble_lls_init_t * p_lls_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    if (p_lls_init->evt_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    p_lls->evt_handler   = p_lls_init->evt_handler;
    p_lls->error_handler = p_lls_init->error_handler;

    // Add service
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_LINK_LOSS_SERVICE);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_lls->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add alert level characteristic
    return alert_level_char_add(p_lls, p_lls_init);
}


uint32_t ble_lls_alert_level_get(ble_lls_t * p_lls, uint8_t * p_alert_level)
{
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = p_alert_level;

    return sd_ble_gatts_value_get(p_lls->conn_handle,
                                  p_lls->alert_level_handles.value_handle,
                                  &gatts_value);
}
