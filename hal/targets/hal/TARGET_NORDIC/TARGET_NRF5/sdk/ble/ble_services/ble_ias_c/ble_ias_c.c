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

#include "ble_ias_c.h"

#include <string.h>
#include "sdk_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gattc.h"
#include "ble_db_discovery.h"


void ble_ias_c_on_db_disc_evt(ble_ias_c_t * p_ias_c, const ble_db_discovery_evt_t * p_evt)
{
    ble_ias_c_evt_t evt;

    memset(&evt, 0, sizeof(ble_ias_c_evt_t));
    evt.evt_type = BLE_IAS_C_EVT_DISCOVERY_FAILED;
    evt.conn_handle = p_evt->conn_handle;
    
    const ble_gatt_db_char_t * p_chars = p_evt->params.discovered_db.charateristics;

    // Check if the Immediate Alert Service was discovered.
    if (p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE
        &&
        p_evt->params.discovered_db.srv_uuid.uuid == BLE_UUID_IMMEDIATE_ALERT_SERVICE
        &&
        p_evt->params.discovered_db.srv_uuid.type == BLE_UUID_TYPE_BLE)
    {

        uint32_t i;

        for (i = 0; i < p_evt->params.discovered_db.char_count; i++)
        {
            // The Alert Level characteristic in the Immediate Alert Service instance is found
            // on peer. Check if it has the correct property 'Write without response'.
            switch(p_chars[i].characteristic.uuid.uuid)
            {
                case BLE_UUID_ALERT_LEVEL_CHAR:
                    if(p_chars[i].characteristic.char_props.write_wo_resp)
                    {
                        // Found Alert Level characteristic inside the Immediate Alert Service. 
                        memcpy(&evt.alert_level,
                               &p_chars[i].characteristic,
                               sizeof(ble_gattc_char_t));
                    }
                    break;

                default:
                    break;
            }
        }
    }
    if (evt.alert_level.handle_value != BLE_GATT_HANDLE_INVALID)
    {
        evt.evt_type = BLE_IAS_C_EVT_DISCOVERY_COMPLETE;
    }

    p_ias_c->evt_handler(p_ias_c, &evt);
}


uint32_t ble_ias_c_init(ble_ias_c_t * p_ias_c, ble_ias_c_init_t const * p_ias_c_init)
{
    VERIFY_PARAM_NOT_NULL(p_ias_c);
    VERIFY_PARAM_NOT_NULL(p_ias_c_init->evt_handler);
    VERIFY_PARAM_NOT_NULL(p_ias_c_init);

    p_ias_c->evt_handler                   = p_ias_c_init->evt_handler;
    p_ias_c->error_handler                 = p_ias_c_init->error_handler;
    p_ias_c->conn_handle                   = BLE_CONN_HANDLE_INVALID;
    p_ias_c->alert_level_char.handle_value = BLE_GATT_HANDLE_INVALID;

    BLE_UUID_BLE_ASSIGN(p_ias_c->alert_level_char.uuid, BLE_UUID_ALERT_LEVEL_CHAR);
    BLE_UUID_BLE_ASSIGN(p_ias_c->service_uuid, BLE_UUID_IMMEDIATE_ALERT_SERVICE);

    return ble_db_discovery_evt_register(&p_ias_c->service_uuid);
}



/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_ias_c     Immediate Alert Service client structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_ias_c_t * p_ias_c, ble_evt_t const * p_ble_evt)
{
    // The following values will be re-initialized when a new connection is made.
    p_ias_c->conn_handle = BLE_CONN_HANDLE_INVALID;

    if (ble_ias_c_is_discovered(p_ias_c))
    {
        // There was a valid instance of IAS on the peer. Send an event to the
        // application, so that it can do any clean up related to this module.
        ble_ias_c_evt_t evt;

        evt.evt_type = BLE_IAS_C_EVT_DISCONN_COMPLETE;

        p_ias_c->evt_handler(p_ias_c, &evt);
        p_ias_c->alert_level_char.handle_value = BLE_GATT_HANDLE_INVALID;
    }
}


void ble_ias_c_on_ble_evt(ble_ias_c_t * p_ias_c, ble_evt_t const * p_ble_evt)
{
    uint32_t err_code = NRF_SUCCESS;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_ias_c, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }

    if (err_code != NRF_SUCCESS && p_ias_c->error_handler != 0)
    {
        p_ias_c->error_handler(err_code);
    }
}


/**@brief Function for performing a Write procedure.
 *
 * @param[in]   conn_handle    Handle of the connection on which to perform the write operation.
 * @param[in]   write_handle   Handle of the attribute to be written.
 * @param[in]   length         Length of data to be written.
 * @param[in]   p_value        Data to be written.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t write_characteristic_value(uint16_t  conn_handle,
                                           uint16_t  write_handle,
                                           uint16_t  length,
                                           uint8_t * p_value)
{
    ble_gattc_write_params_t write_params;

    memset(&write_params, 0, sizeof(write_params));

    write_params.handle   = write_handle;
    write_params.write_op = BLE_GATT_OP_WRITE_CMD;
    write_params.offset   = 0;
    write_params.len      = length;
    write_params.p_value  = p_value;

    return sd_ble_gattc_write(conn_handle, &write_params);
}


uint32_t ble_ias_c_send_alert_level(ble_ias_c_t const * p_ias_c, uint8_t alert_level)
{
    if (!ble_ias_c_is_discovered(p_ias_c))
    {
        return NRF_ERROR_NOT_FOUND;
    }

    return write_characteristic_value(p_ias_c->conn_handle,
                                      p_ias_c->alert_level_char.handle_value,
                                      sizeof(uint8_t),
                                      &alert_level);
}


uint32_t ble_ias_c_handles_assign(ble_ias_c_t  * p_ias_c,
                                  const uint16_t conn_handle,
                                  const uint16_t alert_level_handle)
{
    VERIFY_PARAM_NOT_NULL(p_ias_c);

    p_ias_c->conn_handle = conn_handle;
    p_ias_c->alert_level_char.handle_value = alert_level_handle;
    return NRF_SUCCESS;
}
