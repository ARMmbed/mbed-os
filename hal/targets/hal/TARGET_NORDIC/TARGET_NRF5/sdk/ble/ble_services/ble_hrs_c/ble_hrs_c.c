/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */

/**@cond To Make Doxygen skip documentation generation for this file.
 * @{
 */

#include "ble_hrs_c.h"
#include "ble_db_discovery.h"
#include "ble_types.h"
#include "ble_srv_common.h"
#include "ble_gattc.h"
#include "app_trace.h"
#include "sdk_common.h"
#include "nrf_log.h"

#define LOG                    NRF_LOG_PRINTF_DEBUG  /**< Debug logger macro that will be used in this file to do logging of important information over UART. */

#define HRM_FLAG_MASK_HR_16BIT (0x01 << 0)           /**< Bit mask used to extract the type of heart rate value. This is used to find if the received heart rate is a 16 bit value or an 8 bit value. */

#define TX_BUFFER_MASK         0x07                  /**< TX Buffer mask, must be a mask of continuous zeroes, followed by continuous sequence of ones: 000...111. */
#define TX_BUFFER_SIZE         (TX_BUFFER_MASK + 1)  /**< Size of send buffer, which is 1 higher than the mask. */

#define WRITE_MESSAGE_LENGTH   BLE_CCCD_VALUE_LEN    /**< Length of the write message for CCCD. */
#define WRITE_MESSAGE_LENGTH   BLE_CCCD_VALUE_LEN    /**< Length of the write message for CCCD. */

typedef enum
{
    READ_REQ,  /**< Type identifying that this tx_message is a read request. */
    WRITE_REQ  /**< Type identifying that this tx_message is a write request. */
} tx_request_t;

/**@brief Structure for writing a message to the peer, i.e. CCCD.
 */
typedef struct
{
    uint8_t                  gattc_value[WRITE_MESSAGE_LENGTH];  /**< The message to write. */
    ble_gattc_write_params_t gattc_params;                       /**< GATTC parameters for this message. */
} write_params_t;

/**@brief Structure for holding data to be transmitted to the connected central.
 */
typedef struct
{
    uint16_t     conn_handle;  /**< Connection handle to be used when transmitting this message. */
    tx_request_t type;         /**< Type of this message, i.e. read or write message. */
    union
    {
        uint16_t       read_handle;  /**< Read request message. */
        write_params_t write_req;    /**< Write request message. */
    } req;
} tx_message_t;


static tx_message_t  m_tx_buffer[TX_BUFFER_SIZE];  /**< Transmit buffer for messages to be transmitted to the central. */
static uint32_t      m_tx_insert_index = 0;        /**< Current index in the transmit buffer where the next message should be inserted. */
static uint32_t      m_tx_index = 0;               /**< Current index in the transmit buffer from where the next message to be transmitted resides. */


/**@brief Function for passing any pending request from the buffer to the stack.
 */
static void tx_buffer_process(void)
{
    if (m_tx_index != m_tx_insert_index)
    {
        uint32_t err_code;

        if (m_tx_buffer[m_tx_index].type == READ_REQ)
        {
            err_code = sd_ble_gattc_read(m_tx_buffer[m_tx_index].conn_handle,
                                         m_tx_buffer[m_tx_index].req.read_handle,
                                         0);
        }
        else
        {
            err_code = sd_ble_gattc_write(m_tx_buffer[m_tx_index].conn_handle,
                                          &m_tx_buffer[m_tx_index].req.write_req.gattc_params);
        }
        if (err_code == NRF_SUCCESS)
        {
            LOG("[HRS_C]: SD Read/Write API returns Success..\r\n");
            m_tx_index++;
            m_tx_index &= TX_BUFFER_MASK;
        }
        else
        {
            LOG("[HRS_C]: SD Read/Write API returns error. This message sending will be "
                "attempted again..\r\n");
        }
    }
}


/**@brief     Function for handling write response events.
 *
 * @param[in] p_ble_hrs_c Pointer to the Heart Rate Client structure.
 * @param[in] p_ble_evt   Pointer to the BLE event received.
 */
static void on_write_rsp(ble_hrs_c_t * p_ble_hrs_c, const ble_evt_t * p_ble_evt)
{
    // Check if the event if on the link for this instance
    if (p_ble_hrs_c->conn_handle != p_ble_evt->evt.gattc_evt.conn_handle)
    {
        return;
    }
    // Check if there is any message to be sent across to the peer and send it.
    tx_buffer_process();
}


/**@brief     Function for handling Handle Value Notification received from the SoftDevice.
 *
 * @details   This function will uses the Handle Value Notification received from the SoftDevice
 *            and checks if it is a notification of the heart rate measurement from the peer. If
 *            it is, this function will decode the heart rate measurement and send it to the
 *            application.
 *
 * @param[in] p_ble_hrs_c Pointer to the Heart Rate Client structure.
 * @param[in] p_ble_evt   Pointer to the BLE event received.
 */
static void on_hvx(ble_hrs_c_t * p_ble_hrs_c, const ble_evt_t * p_ble_evt)
{
    // Check if the event is on the link for this instance
    if (p_ble_hrs_c->conn_handle != p_ble_evt->evt.gattc_evt.conn_handle)
    {
        LOG("[HRS_C]: received HVX on link 0x%x, not associated to this instance, ignore\r\n",
            p_ble_evt->evt.gattc_evt.conn_handle);
        return;
    }
    LOG("[HRS_C]: received HVX on handle 0x%x, hrm_handle 0x%x\r\n",
        p_ble_evt->evt.gattc_evt.params.hvx.handle,
        p_ble_hrs_c->peer_hrs_db.hrm_handle);
    // Check if this is a heart rate notification.
    if (p_ble_evt->evt.gattc_evt.params.hvx.handle == p_ble_hrs_c->peer_hrs_db.hrm_handle)
    {
        ble_hrs_c_evt_t ble_hrs_c_evt;
        uint32_t        index = 0;

        ble_hrs_c_evt.evt_type    = BLE_HRS_C_EVT_HRM_NOTIFICATION;
        ble_hrs_c_evt.conn_handle = p_ble_hrs_c->conn_handle;

        if (!(p_ble_evt->evt.gattc_evt.params.hvx.data[index++] & HRM_FLAG_MASK_HR_16BIT))
        {
            // 8 Bit heart rate value received.
            ble_hrs_c_evt.params.hrm.hr_value = p_ble_evt->evt.gattc_evt.params.hvx.data[index++];  //lint !e415 suppress Lint Warning 415: Likely access out of bond
        }
        else
        {
            // 16 bit heart rate value received.
            ble_hrs_c_evt.params.hrm.hr_value =
                uint16_decode(&(p_ble_evt->evt.gattc_evt.params.hvx.data[index]));
        }

        p_ble_hrs_c->evt_handler(p_ble_hrs_c, &ble_hrs_c_evt);
    }
}


/**@brief     Function for handling Disconnected event received from the SoftDevice.
 *
 * @details   This function check if the disconnect event is happening on the link
 *            associated with the current instance of the module, if so it will set its
 *            conn_handle to invalid.
 *
 * @param[in] p_ble_hrs_c Pointer to the Heart Rate Client structure.
 * @param[in] p_ble_evt   Pointer to the BLE event received.
 */
static void on_disconnected(ble_hrs_c_t * p_ble_hrs_c, const ble_evt_t * p_ble_evt)
{
    if (p_ble_hrs_c->conn_handle == p_ble_evt->evt.gap_evt.conn_handle)
    {
        p_ble_hrs_c->conn_handle                 = BLE_CONN_HANDLE_INVALID;
        p_ble_hrs_c->peer_hrs_db.hrm_cccd_handle = BLE_GATT_HANDLE_INVALID;
        p_ble_hrs_c->peer_hrs_db.hrm_handle      = BLE_GATT_HANDLE_INVALID;
    }
}


void ble_hrs_on_db_disc_evt(ble_hrs_c_t * p_ble_hrs_c, const ble_db_discovery_evt_t * p_evt)
{
    // Check if the Heart Rate Service was discovered.
    if (p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE &&
        p_evt->params.discovered_db.srv_uuid.uuid == BLE_UUID_HEART_RATE_SERVICE &&
        p_evt->params.discovered_db.srv_uuid.type == BLE_UUID_TYPE_BLE)
    {
        // Find the CCCD Handle of the Heart Rate Measurement characteristic.
        uint32_t i;

        ble_hrs_c_evt_t evt;

        evt.evt_type    = BLE_HRS_C_EVT_DISCOVERY_COMPLETE;
        evt.conn_handle = p_evt->conn_handle;

        for (i = 0; i < p_evt->params.discovered_db.char_count; i++)
        {
            if (p_evt->params.discovered_db.charateristics[i].characteristic.uuid.uuid ==
                BLE_UUID_HEART_RATE_MEASUREMENT_CHAR)
            {
                // Found Heart Rate characteristic. Store CCCD handle and break.
                evt.params.peer_db.hrm_cccd_handle =
                    p_evt->params.discovered_db.charateristics[i].cccd_handle;
                evt.params.peer_db.hrm_handle =
                    p_evt->params.discovered_db.charateristics[i].characteristic.handle_value;
                break;
            }
        }

        LOG("[HRS_C]: Heart Rate Service discovered at peer.\r\n");
        //If the instance has been assigned prior to db_discovery, assign the db_handles
        if(p_ble_hrs_c->conn_handle != BLE_CONN_HANDLE_INVALID)
        {
            if ((p_ble_hrs_c->peer_hrs_db.hrm_cccd_handle == BLE_GATT_HANDLE_INVALID)&&
                (p_ble_hrs_c->peer_hrs_db.hrm_handle == BLE_GATT_HANDLE_INVALID))
            {
                p_ble_hrs_c->peer_hrs_db = evt.params.peer_db;
            }
        }


        p_ble_hrs_c->evt_handler(p_ble_hrs_c, &evt);
    }
}


uint32_t ble_hrs_c_init(ble_hrs_c_t * p_ble_hrs_c, ble_hrs_c_init_t * p_ble_hrs_c_init)
{
    VERIFY_PARAM_NOT_NULL(p_ble_hrs_c);
    VERIFY_PARAM_NOT_NULL(p_ble_hrs_c_init);

    ble_uuid_t hrs_uuid;

    hrs_uuid.type = BLE_UUID_TYPE_BLE;
    hrs_uuid.uuid = BLE_UUID_HEART_RATE_SERVICE;


    p_ble_hrs_c->evt_handler                 = p_ble_hrs_c_init->evt_handler;
    p_ble_hrs_c->conn_handle                 = BLE_CONN_HANDLE_INVALID;
    p_ble_hrs_c->peer_hrs_db.hrm_cccd_handle = BLE_GATT_HANDLE_INVALID;
    p_ble_hrs_c->peer_hrs_db.hrm_handle      = BLE_GATT_HANDLE_INVALID;

    return ble_db_discovery_evt_register(&hrs_uuid);
}


void ble_hrs_c_on_ble_evt(ble_hrs_c_t * p_ble_hrs_c, const ble_evt_t * p_ble_evt)
{
    if ((p_ble_hrs_c == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTC_EVT_HVX:
            on_hvx(p_ble_hrs_c, p_ble_evt);
            break;

        case BLE_GATTC_EVT_WRITE_RSP:
            on_write_rsp(p_ble_hrs_c, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnected(p_ble_hrs_c, p_ble_evt);
            break;

        default:
            break;
    }
}


/**@brief Function for creating a message for writing to the CCCD.
 */
static uint32_t cccd_configure(uint16_t conn_handle, uint16_t handle_cccd, bool enable)
{
    LOG("[HRS_C]: Configuring CCCD. CCCD Handle = %d, Connection Handle = %d\r\n",
        handle_cccd,conn_handle);

    tx_message_t * p_msg;
    uint16_t       cccd_val = enable ? BLE_GATT_HVX_NOTIFICATION : 0;

    p_msg              = &m_tx_buffer[m_tx_insert_index++];
    m_tx_insert_index &= TX_BUFFER_MASK;

    p_msg->req.write_req.gattc_params.handle   = handle_cccd;
    p_msg->req.write_req.gattc_params.len      = WRITE_MESSAGE_LENGTH;
    p_msg->req.write_req.gattc_params.p_value  = p_msg->req.write_req.gattc_value;
    p_msg->req.write_req.gattc_params.offset   = 0;
    p_msg->req.write_req.gattc_params.write_op = BLE_GATT_OP_WRITE_REQ;
    p_msg->req.write_req.gattc_value[0]        = LSB_16(cccd_val);
    p_msg->req.write_req.gattc_value[1]        = MSB_16(cccd_val);
    p_msg->conn_handle                         = conn_handle;
    p_msg->type                                = WRITE_REQ;

    tx_buffer_process();
    return NRF_SUCCESS;
}


uint32_t ble_hrs_c_hrm_notif_enable(ble_hrs_c_t * p_ble_hrs_c)
{
    VERIFY_PARAM_NOT_NULL(p_ble_hrs_c);

    return cccd_configure(p_ble_hrs_c->conn_handle,
                          p_ble_hrs_c->peer_hrs_db.hrm_cccd_handle,
                          true);
}


uint32_t ble_hrs_c_handles_assign(ble_hrs_c_t * p_ble_hrs_c,
                                  uint16_t conn_handle,
                                  const hrs_db_t * p_peer_hrs_handles)
{
    VERIFY_PARAM_NOT_NULL(p_ble_hrs_c);

    p_ble_hrs_c->conn_handle = conn_handle;
    if (p_peer_hrs_handles != NULL)
    {
        p_ble_hrs_c->peer_hrs_db = *p_peer_hrs_handles;
    }
    return NRF_SUCCESS;
}
/** @}
 *  @endcond
 */
