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

#include <string.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_gattc.h"
#include "ble_cts_c.h"
#include "ble_date_time.h"
#include "ble_db_discovery.h"
#include "nrf_log.h"
#include "nrf_log.h"
#include "sdk_common.h"

#define CTS_LOG      NRF_LOG_PRINTF_DEBUG     /**< Debug logger macro that will be used in this file to do logging of important information over UART. */
#define CTS_YEAR_MIN 1582                     /**< The lowest valid Current Time year is the year when the western calendar was introduced. */
#define CTS_YEAR_MAX 9999                     /**< The highest possible Current Time. */

#define CTS_C_CURRENT_TIME_EXPECTED_LENGTH 10 /**< |     Year        |Month   |Day     |Hours   |Minutes |Seconds |Weekday |Fraction|Reason  |
                                                   |     2 bytes     |1 byte  |1 byte  |1 byte  |1 byte  |1 byte  |1 byte  |1 byte  |1 byte  | = 10 bytes. */


/**@brief Function for handling events from the database discovery module.
 *
 * @details This function will handle an event from the database discovery module, and determine
 *          if it relates to the discovery of Current Time Service at the peer. If so, it will
 *          call the application's event handler indicating that the Current Time Service has been
 *          discovered at the peer. It also populates the event with the service related
 *          information before providing it to the application.
 *
 * @param[in] p_evt Pointer to the event received from the database discovery module.
 *
 */
void ble_cts_c_on_db_disc_evt(ble_cts_c_t * p_cts, ble_db_discovery_evt_t * p_evt)
{
    CTS_LOG("[CTS]: Database Discovery handler called with event 0x%x\r\n", p_evt->evt_type);

    ble_cts_c_evt_t evt;
    const ble_gatt_db_char_t * p_chars = p_evt->params.discovered_db.charateristics;
    
    evt.evt_type    = BLE_CTS_C_EVT_DISCOVERY_FAILED;
    evt.conn_handle = p_evt->conn_handle;
    
    // Check if the Current Time Service was discovered.
    if (p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE &&
        p_evt->params.discovered_db.srv_uuid.uuid == BLE_UUID_CURRENT_TIME_SERVICE &&
        p_evt->params.discovered_db.srv_uuid.type == BLE_UUID_TYPE_BLE)
    {

        // Find the handles of the Current Time characteristic.
        uint32_t i;

        for (i = 0; i < p_evt->params.discovered_db.char_count; i++)
        {
            if (p_evt->params.discovered_db.charateristics[i].characteristic.uuid.uuid ==
                BLE_UUID_CURRENT_TIME_CHAR)
            {
                // Found Current Time characteristic. Store CCCD and value handle and break.
                evt.params.char_handles.cts_handle      = p_chars->characteristic.handle_value;
                evt.params.char_handles.cts_cccd_handle = p_chars->cccd_handle;
                break;
            }
        }

        CTS_LOG("[CTS]: Current Time Service discovered at peer.\r\n");

        evt.evt_type    = BLE_CTS_C_EVT_DISCOVERY_COMPLETE;
    }
    p_cts->evt_handler(p_cts, &evt);
}


uint32_t ble_cts_c_init(ble_cts_c_t * p_cts, ble_cts_c_init_t const * p_cts_init)
{
    //Verify that the parameters needed for to initialize this instance of CTS are not NULL.
    VERIFY_PARAM_NOT_NULL(p_cts);
    VERIFY_PARAM_NOT_NULL(p_cts_init);
    VERIFY_PARAM_NOT_NULL(p_cts_init->error_handler);
    VERIFY_PARAM_NOT_NULL(p_cts_init->evt_handler);

    static ble_uuid_t cts_uuid;

    BLE_UUID_BLE_ASSIGN(cts_uuid, BLE_UUID_CURRENT_TIME_SERVICE);

    p_cts->evt_handler                  = p_cts_init->evt_handler;
    p_cts->error_handler                = p_cts_init->error_handler;
    p_cts->conn_handle                  = BLE_CONN_HANDLE_INVALID;
    p_cts->char_handles.cts_handle      = BLE_GATT_HANDLE_INVALID;
    p_cts->char_handles.cts_cccd_handle = BLE_GATT_HANDLE_INVALID;

    return ble_db_discovery_evt_register(&cts_uuid);
}


/**@brief Function for decoding a read from the current time characteristic.
 *
 * @param[in] p_time  Current Time structure.
 * @param[in] p_data  Pointer to the buffer containing the current time.
 * @param[in] length  length of the buffer containing the current time.
 *
 * @return NRF_SUCCESS if the time struct is valid.
 * @return NRF_ERROR_DATA_SIZE if length does not match the expected size of the data.
 */
static uint32_t current_time_decode(current_time_char_t * p_time,
                                    const uint8_t       * p_data,
                                    const uint32_t        length)
{
    //lint -save -e415 -e416 "Access of out of bounds pointer" "Creation of out of bounds pointer"

    if (length != CTS_C_CURRENT_TIME_EXPECTED_LENGTH)
    {
        // Return to prevent accessing out of bounds data.
        return NRF_ERROR_DATA_SIZE;
    }

    CTS_LOG("Current Time read response data: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X \r\n",
        p_data[0],
        p_data[1],
        p_data[2],
        p_data[3],
        p_data[4],
        p_data[5],
        p_data[6],
        p_data[7],
        p_data[8],
        p_data[9]);

    uint32_t index = 0;

    // Date.
    index += ble_date_time_decode(&p_time->exact_time_256.day_date_time.date_time, p_data);

    // Day of week.
    p_time->exact_time_256.day_date_time.day_of_week = p_data[index++];

    // Fractions of a second.
    p_time->exact_time_256.fractions256 = p_data[index++];

    // Reason for updating the time.
    p_time->adjust_reason.manual_time_update              = (p_data[index] >> 0) & 0x01;
    p_time->adjust_reason.external_reference_time_update  = (p_data[index] >> 1) & 0x01;
    p_time->adjust_reason.change_of_time_zone             = (p_data[index] >> 2) & 0x01;
    p_time->adjust_reason.change_of_daylight_savings_time = (p_data[index] >> 3) & 0x01;


    //lint -restore
    return NRF_SUCCESS;
}


/**@brief Function for decoding a read from the current time characteristic.
 *
 * @param[in] p_time  Current Time struct.
 *
 * @return NRF_SUCCESS if the time struct is valid.
 * @return NRF_ERROR_INVALID_DATA if the time is out of bounds.
 */
static uint32_t current_time_validate(current_time_char_t * p_time)
{
    // Year.
    if (   (p_time->exact_time_256.day_date_time.date_time.year > CTS_YEAR_MAX)
        || ((p_time->exact_time_256.day_date_time.date_time.year < CTS_YEAR_MIN)
         && (p_time->exact_time_256.day_date_time.date_time.year != 0)))
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // Month.
    if (p_time->exact_time_256.day_date_time.date_time.month > 12)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // Day.
    if (p_time->exact_time_256.day_date_time.date_time.day > 31)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // Hours.
    if (p_time->exact_time_256.day_date_time.date_time.hours > 23)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // Minutes.
    if (p_time->exact_time_256.day_date_time.date_time.minutes > 59)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // Seconds.
    if (p_time->exact_time_256.day_date_time.date_time.seconds > 59)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    // Day of week.
    if (p_time->exact_time_256.day_date_time.day_of_week > 7)
    {
        return NRF_ERROR_INVALID_DATA;
    }

    return NRF_SUCCESS;
}


/**@brief Function for reading the current time. The time is decoded, then it is validated.
 *        Depending on the outcome the cts event handler will be called with
 *        the current time event or an invalid time event.
 *
 * @param[in] p_cts      Current Time Service client structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void current_time_read(ble_cts_c_t * p_cts, const ble_evt_t * p_ble_evt)
{
    ble_cts_c_evt_t evt;
    uint32_t        err_code = NRF_SUCCESS;

    // Check if the event is on the same connection as this cts instance
    if (p_cts->conn_handle != p_ble_evt->evt.gattc_evt.conn_handle)
    {
        return;
    }

    if (p_ble_evt->evt.gattc_evt.gatt_status == BLE_GATT_STATUS_SUCCESS)
    {
        err_code = current_time_decode(&evt.params.current_time,
                                       p_ble_evt->evt.gattc_evt.params.read_rsp.data,
                                       p_ble_evt->evt.gattc_evt.params.read_rsp.len);

        if (err_code != NRF_SUCCESS)
        {
            // The data length was invalid, decoding was not completed.
            evt.evt_type = BLE_CTS_C_EVT_INVALID_TIME;
        }
        else
        {
            // Verify That the time is valid.
            err_code = current_time_validate(&evt.params.current_time);

            if (err_code != NRF_SUCCESS)
            {
                // Invalid time received.
                evt.evt_type = BLE_CTS_C_EVT_INVALID_TIME;
            }
            else
            {
                // Valid time reveiced.
                evt.evt_type = BLE_CTS_C_EVT_CURRENT_TIME;
            }
        }
        p_cts->evt_handler(p_cts, &evt);
    }
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in] p_cts      Current Time Service client structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_disconnect(ble_cts_c_t * p_cts, ble_evt_t const * p_ble_evt)
{
    if (p_cts->conn_handle == p_ble_evt->evt.gap_evt.conn_handle)
    {
        p_cts->conn_handle = BLE_CONN_HANDLE_INVALID;
    }

    if (ble_cts_c_is_cts_discovered(p_cts))
    {
        // There was a valid instance of cts on the peer. Send an event to the
        // application, so that it can do any clean up related to this module.
        ble_cts_c_evt_t evt;

        evt.evt_type = BLE_CTS_C_EVT_DISCONN_COMPLETE;

        p_cts->evt_handler(p_cts, &evt);
        p_cts->char_handles.cts_handle      = BLE_GATT_HANDLE_INVALID;
        p_cts->char_handles.cts_cccd_handle = BLE_GATT_HANDLE_INVALID;
    }
}


void ble_cts_c_on_ble_evt(ble_cts_c_t * p_cts, ble_evt_t const * p_ble_evt)
{
    CTS_LOG("[CTS]: BLE event handler called with event 0x%x\r\n", p_ble_evt->header.evt_id);

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTC_EVT_READ_RSP:
            current_time_read(p_cts, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_cts, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_cts_c_current_time_read(ble_cts_c_t const * p_cts)
{
    if (!ble_cts_c_is_cts_discovered(p_cts))
    {
        return NRF_ERROR_NOT_FOUND;
    }

    return sd_ble_gattc_read(p_cts->conn_handle, p_cts->char_handles.cts_handle, 0);
}


uint32_t ble_cts_c_handles_assign(ble_cts_c_t               * p_cts,
                                  const uint16_t              conn_handle,
                                  const ble_cts_c_handles_t * p_peer_handles)
{
    VERIFY_PARAM_NOT_NULL(p_cts);

    p_cts->conn_handle = conn_handle;
    if (p_peer_handles != NULL)
    {
        p_cts->char_handles.cts_cccd_handle = p_peer_handles->cts_cccd_handle;
        p_cts->char_handles.cts_handle = p_peer_handles->cts_handle;
    }

    return NRF_SUCCESS;
}
