/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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

#include "ble_dfu.h"
#include "ble_types.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"
#include <stddef.h>
#include "sdk_common.h"

#define MAX_DFU_PKT_LEN         20                                              /**< Maximum length (in bytes) of the DFU Packet characteristic. */
#define PKT_START_DFU_PARAM_LEN 2                                               /**< Length (in bytes) of the parameters for Packet Start DFU Request. */
#define PKT_INIT_DFU_PARAM_LEN  2                                               /**< Length (in bytes) of the parameters for Packet Init DFU Request. */
#define PKT_RCPT_NOTIF_REQ_LEN  3                                               /**< Length (in bytes) of the Packet Receipt Notification Request. */
#define MAX_PKTS_RCPT_NOTIF_LEN 6                                               /**< Maximum length (in bytes) of the Packets Receipt Notification. */
#define MAX_RESPONSE_LEN        7                                               /**< Maximum length (in bytes) of the response to a Control Point command. */
#define MAX_NOTIF_BUFFER_LEN    MAX(MAX_PKTS_RCPT_NOTIF_LEN, MAX_RESPONSE_LEN)  /**< Maximum length (in bytes) of the buffer needed by DFU Service while sending notifications to peer. */

enum
{
    OP_CODE_START_DFU          = 1,                                             /**< Value of the Op code field for 'Start DFU' command.*/
    OP_CODE_RECEIVE_INIT       = 2,                                             /**< Value of the Op code field for 'Initialize DFU parameters' command.*/
    OP_CODE_RECEIVE_FW         = 3,                                             /**< Value of the Op code field for 'Receive firmware image' command.*/
    OP_CODE_VALIDATE           = 4,                                             /**< Value of the Op code field for 'Validate firmware' command.*/
    OP_CODE_ACTIVATE_N_RESET   = 5,                                             /**< Value of the Op code field for 'Activate & Reset' command.*/
    OP_CODE_SYS_RESET          = 6,                                             /**< Value of the Op code field for 'Reset System' command.*/
    OP_CODE_IMAGE_SIZE_REQ     = 7,                                             /**< Value of the Op code field for 'Report received image size' command.*/
    OP_CODE_PKT_RCPT_NOTIF_REQ = 8,                                             /**< Value of the Op code field for 'Request packet receipt notification.*/
    OP_CODE_RESPONSE           = 16,                                            /**< Value of the Op code field for 'Response.*/
    OP_CODE_PKT_RCPT_NOTIF     = 17                                             /**< Value of the Op code field for 'Packets Receipt Notification'.*/
};

static bool     m_is_dfu_service_initialized = false;                           /**< Variable to check if the DFU service was initialized by the application.*/
static uint8_t  m_notif_buffer[MAX_NOTIF_BUFFER_LEN];                           /**< Buffer used for sending notifications to peer. */

/**@brief       Function for adding DFU Packet characteristic to the BLE Stack.
 *
 * @param[in]   p_dfu DFU Service structure.
 *
 * @return      NRF_SUCCESS on success. Otherwise an error code.
 */
static uint32_t dfu_pkt_char_add(ble_dfu_t * const p_dfu)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write_wo_resp = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    char_uuid.type = p_dfu->uuid_type;
    char_uuid.uuid = BLE_DFU_PKT_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = MAX_DFU_PKT_LEN;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_dfu->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_dfu->dfu_pkt_handles);
}


/**@brief       Function for adding DFU Revision characteristic to the BLE Stack.
 *
 * @param[in]   p_dfu DFU Service structure.
 *
 * @return      NRF_SUCCESS on success. Otherwise an error code.
 */
static uint32_t dfu_rev_char_add(ble_dfu_t * const p_dfu, ble_dfu_init_t const * const p_dfu_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    char_uuid.type = p_dfu->uuid_type;
    char_uuid.uuid = BLE_DFU_REV_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint16_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint16_t);
    attr_char_value.p_value   = (uint8_t *)&p_dfu_init->revision;

    return sd_ble_gatts_characteristic_add(p_dfu->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_dfu->dfu_rev_handles);
}


/**@brief       Function for adding DFU Control Point characteristic to the BLE Stack.
 *
 * @param[in]   p_dfu DFU Service structure.
 *
 * @return      NRF_SUCCESS on success. Otherwise an error code.
 */
static uint32_t dfu_ctrl_pt_add(ble_dfu_t * const p_dfu)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = p_dfu->uuid_type;
    char_uuid.uuid = BLE_DFU_CTRL_PT_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_L2CAP_MTU_DEF;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_dfu->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_dfu->dfu_ctrl_pt_handles);
}


/**@brief     Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the S110 SoftDevice.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_connect(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)
{
    p_dfu->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief     Function for checking if the CCCD of DFU Control point is configured for Notification.
 *
 * @details   This function checks if the CCCD of DFU Control Point characteristic is configured
 *            for Notification by the DFU Controller.
 *
 * @param[in] p_dfu DFU Service structure.
 *
 * @return    True if the CCCD of DFU Control Point characteristic is configured for Notification.
 *            False otherwise.
 */
static bool is_cccd_configured(ble_dfu_t * p_dfu)
{
    // Check if the CCCDs are configured.
    uint8_t  cccd_val_buf[BLE_CCCD_VALUE_LEN];
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = BLE_CCCD_VALUE_LEN;
    gatts_value.offset  = 0;
    gatts_value.p_value = cccd_val_buf;

    // Check the CCCD Value of DFU Control Point.
    uint32_t err_code = sd_ble_gatts_value_get(p_dfu->conn_handle,
                                               p_dfu->dfu_ctrl_pt_handles.cccd_handle,
                                               &gatts_value);
    if (err_code != NRF_SUCCESS)
    {
        if (p_dfu->error_handler != NULL)
        {
            p_dfu->error_handler(err_code);
        }
        return false;
    }

    return ble_srv_is_notification_enabled(cccd_val_buf);
}


/**@brief     Function for handling a Write event on the Control Point characteristic.
 *
 * @param[in] p_dfu             DFU Service Structure.
 * @param[in] p_ble_write_evt   Pointer to the write event received from BLE stack.
 *
 * @return    NRF_SUCCESS on successful processing of control point write. Otherwise an error code.
 */
static uint32_t on_ctrl_pt_write(ble_dfu_t * p_dfu, ble_gatts_evt_write_t * p_ble_write_evt)
{
    ble_gatts_rw_authorize_reply_params_t auth_reply;

    auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
    auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
    auth_reply.params.write.update = 1;
    auth_reply.params.write.offset = p_ble_write_evt->offset;
    auth_reply.params.write.len = p_ble_write_evt->len;
    auth_reply.params.write.p_data = p_ble_write_evt->data;


    if (!is_cccd_configured(p_dfu))
    {
        // Send an error response to the peer indicating that the CCCD is improperly configured.
        auth_reply.params.write.gatt_status =
            BLE_GATT_STATUS_ATTERR_CPS_CCCD_CONFIG_ERROR;

        return (sd_ble_gatts_rw_authorize_reply(p_dfu->conn_handle, &auth_reply));

    }
    else
    {
        uint32_t err_code;

        auth_reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;

        err_code = (sd_ble_gatts_rw_authorize_reply(p_dfu->conn_handle, &auth_reply));
        VERIFY_SUCCESS(err_code);
    }

    ble_dfu_evt_t ble_dfu_evt;

    switch (p_ble_write_evt->data[0])
    {
        case OP_CODE_START_DFU:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_START;

            if (p_ble_write_evt->len < PKT_START_DFU_PARAM_LEN)
            {
                return ble_dfu_response_send(p_dfu,
                                             (ble_dfu_procedure_t) p_ble_write_evt->data[0],
                                             BLE_DFU_RESP_VAL_OPER_FAILED);
            }

            ble_dfu_evt.evt.ble_dfu_pkt_write.len    = 1;
            ble_dfu_evt.evt.ble_dfu_pkt_write.p_data = &(p_ble_write_evt->data[1]);

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        case OP_CODE_RECEIVE_INIT:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_RECEIVE_INIT_DATA;

            if (p_ble_write_evt->len < PKT_INIT_DFU_PARAM_LEN)
            {
                return ble_dfu_response_send(p_dfu,
                                             (ble_dfu_procedure_t) p_ble_write_evt->data[0],
                                             BLE_DFU_RESP_VAL_OPER_FAILED);
            }
            
            ble_dfu_evt.evt.ble_dfu_pkt_write.len    = 1;
            ble_dfu_evt.evt.ble_dfu_pkt_write.p_data = &(p_ble_write_evt->data[1]);

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        case OP_CODE_RECEIVE_FW:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_RECEIVE_APP_DATA;

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        case OP_CODE_VALIDATE:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_VALIDATE;

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        case OP_CODE_ACTIVATE_N_RESET:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_ACTIVATE_N_RESET;

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        case OP_CODE_SYS_RESET:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_SYS_RESET;

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        case OP_CODE_PKT_RCPT_NOTIF_REQ:
            if (p_ble_write_evt->len < PKT_RCPT_NOTIF_REQ_LEN)
            {
                return (ble_dfu_response_send(p_dfu,
                                              BLE_DFU_PKT_RCPT_REQ_PROCEDURE,
                                              BLE_DFU_RESP_VAL_NOT_SUPPORTED));
            }

            ble_dfu_evt.evt.pkt_rcpt_notif_req.num_of_pkts =
                uint16_decode(&(p_ble_write_evt->data[1]));

            if (ble_dfu_evt.evt.pkt_rcpt_notif_req.num_of_pkts == 0)
            {
                ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_PKT_RCPT_NOTIF_DISABLED;
            }
            else
            {
                ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_PKT_RCPT_NOTIF_ENABLED;
            }

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);

            break;

        case OP_CODE_IMAGE_SIZE_REQ:
            ble_dfu_evt.ble_dfu_evt_type = BLE_DFU_BYTES_RECEIVED_SEND;

            p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
            break;

        default:
            // Unsupported op code.
            return ble_dfu_response_send(p_dfu,
                                         (ble_dfu_procedure_t) p_ble_write_evt->data[0],
                                         BLE_DFU_RESP_VAL_NOT_SUPPORTED);
    }
    return NRF_SUCCESS;
}


/**@brief     Function for handling the @ref BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST event from the S110
 *            Stack.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_rw_authorize_req(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_authorize_request;

    p_authorize_request = &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    if (
        (p_authorize_request->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
        &&
        (p_authorize_request->request.write.handle == p_dfu->dfu_ctrl_pt_handles.value_handle)
        && 
        (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op != BLE_GATTS_OP_PREP_WRITE_REQ)
        &&
        (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op != BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
        &&
        (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op != BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL)
       )
    {
        uint32_t err_code;

        err_code = on_ctrl_pt_write(p_dfu, &(p_authorize_request->request.write));

        if (err_code != NRF_SUCCESS && p_dfu->error_handler != NULL)
        {
            p_dfu->error_handler(err_code);
        }
    }
}


/**@brief     Function for handling the @ref BLE_GATTS_EVT_WRITE event from the S110 SoftDevice.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)
{
    if (p_ble_evt->evt.gatts_evt.params.write.handle == p_dfu->dfu_pkt_handles.value_handle)
    {
        // DFU Packet written

        ble_dfu_evt_t ble_dfu_evt;

        ble_dfu_evt.ble_dfu_evt_type             = BLE_DFU_PACKET_WRITE;
        ble_dfu_evt.evt.ble_dfu_pkt_write.len    = p_ble_evt->evt.gatts_evt.params.write.len;
        ble_dfu_evt.evt.ble_dfu_pkt_write.p_data = p_ble_evt->evt.gatts_evt.params.write.data;

        p_dfu->evt_handler(p_dfu, &ble_dfu_evt);
    }
}


/**@brief     Function for handling the BLE_GAP_EVT_DISCONNECTED event from the S110 SoftDevice.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_disconnect(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)
{
    p_dfu->conn_handle = BLE_CONN_HANDLE_INVALID;
}


uint32_t ble_dfu_init(ble_dfu_t * p_dfu, ble_dfu_init_t * p_dfu_init)
{
    if ((p_dfu == NULL) || (p_dfu_init == NULL) || (p_dfu_init->evt_handler == NULL))
    {
        return NRF_ERROR_NULL;
    }

    p_dfu->conn_handle = BLE_CONN_HANDLE_INVALID;

    ble_uuid_t service_uuid;
    uint32_t   err_code;

    const ble_uuid128_t base_uuid128 =
    {
        {
            0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15,
            0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
        }
    };

    service_uuid.uuid = BLE_DFU_SERVICE_UUID;

    err_code = sd_ble_uuid_vs_add(&base_uuid128, &(service_uuid.type));
    VERIFY_SUCCESS(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &(p_dfu->service_handle));
    VERIFY_SUCCESS(err_code);

    p_dfu->uuid_type = service_uuid.type;

    err_code = dfu_pkt_char_add(p_dfu);
    VERIFY_SUCCESS(err_code);

    err_code = dfu_ctrl_pt_add(p_dfu);
    VERIFY_SUCCESS(err_code);

    err_code = dfu_rev_char_add(p_dfu, p_dfu_init);
    VERIFY_SUCCESS(err_code);

    p_dfu->evt_handler = p_dfu_init->evt_handler;

    if (p_dfu_init->error_handler != NULL)
    {
        p_dfu->error_handler = p_dfu_init->error_handler;
    }

    m_is_dfu_service_initialized = true;

    return NRF_SUCCESS;
}


void ble_dfu_on_ble_evt(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)
{
    if ((p_dfu == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    if (p_dfu->evt_handler != NULL)
    {
        switch (p_ble_evt->header.evt_id)
        {
            case BLE_GAP_EVT_CONNECTED:
                on_connect(p_dfu, p_ble_evt);
                break;

            case BLE_GATTS_EVT_WRITE:
                on_write(p_dfu, p_ble_evt);
                break;

            case BLE_GAP_EVT_DISCONNECTED:
                on_disconnect(p_dfu, p_ble_evt);
                break;

            case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
                on_rw_authorize_req(p_dfu, p_ble_evt);
                break;

            default:
                // No implementation needed.
                break;
        }
    }
}


uint32_t ble_dfu_bytes_rcvd_report(ble_dfu_t * p_dfu, uint32_t num_of_firmware_bytes_rcvd)
{
    if (p_dfu == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if ((p_dfu->conn_handle == BLE_CONN_HANDLE_INVALID) || !m_is_dfu_service_initialized)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    ble_gatts_hvx_params_t hvx_params;
    uint16_t               index = 0;

    // Encode the Op Code.
    m_notif_buffer[index++] = OP_CODE_RESPONSE;

    // Encode the Reqest Op Code.
    m_notif_buffer[index++] = OP_CODE_IMAGE_SIZE_REQ;

    // Encode the Response Value.
    m_notif_buffer[index++] = (uint8_t)BLE_DFU_RESP_VAL_SUCCESS;

    index += uint32_encode(num_of_firmware_bytes_rcvd, &m_notif_buffer[index]);

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_dfu->dfu_ctrl_pt_handles.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = 0;
    hvx_params.p_len  = &index;
    hvx_params.p_data = m_notif_buffer;

    return sd_ble_gatts_hvx(p_dfu->conn_handle, &hvx_params);
}


uint32_t ble_dfu_pkts_rcpt_notify(ble_dfu_t * p_dfu, uint32_t num_of_firmware_bytes_rcvd)
{
    if (p_dfu == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if ((p_dfu->conn_handle == BLE_CONN_HANDLE_INVALID) || !m_is_dfu_service_initialized)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    ble_gatts_hvx_params_t hvx_params;
    uint16_t               index = 0;

    m_notif_buffer[index++] = OP_CODE_PKT_RCPT_NOTIF;

    index += uint32_encode(num_of_firmware_bytes_rcvd, &m_notif_buffer[index]);

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_dfu->dfu_ctrl_pt_handles.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = 0;
    hvx_params.p_len  = &index;
    hvx_params.p_data = m_notif_buffer;

    return sd_ble_gatts_hvx(p_dfu->conn_handle, &hvx_params);
}


uint32_t ble_dfu_response_send(ble_dfu_t         * p_dfu,
                               ble_dfu_procedure_t dfu_proc,
                               ble_dfu_resp_val_t  resp_val)
{
    if (p_dfu == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if ((p_dfu->conn_handle == BLE_CONN_HANDLE_INVALID) || !m_is_dfu_service_initialized)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    ble_gatts_hvx_params_t hvx_params;
    uint16_t               index = 0;

    m_notif_buffer[index++] = OP_CODE_RESPONSE;

    // Encode the Request Op code
    m_notif_buffer[index++] = (uint8_t)dfu_proc;

    // Encode the Response Value.
    m_notif_buffer[index++] = (uint8_t)resp_val;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_dfu->dfu_ctrl_pt_handles.value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = 0;
    hvx_params.p_len  = &index;
    hvx_params.p_data = m_notif_buffer;

    return sd_ble_gatts_hvx(p_dfu->conn_handle, &hvx_params);
}
