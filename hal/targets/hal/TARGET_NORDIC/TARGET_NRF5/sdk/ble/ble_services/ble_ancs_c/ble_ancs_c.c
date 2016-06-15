/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */
 
/* Disclaimer: This client implementation of the Apple Notification Center Service can and will be changed at any time by Nordic Semiconductor ASA.
 * Server implementations such as the ones found in iOS can be changed at any time by Apple and may cause this client implementation to stop working.
 */

#include "ble_ancs_c.h"
#include "ble_err.h"
#include "ble_srv_common.h"
#include "nrf_assert.h"
#include "device_manager.h"
#include "ble_db_discovery.h"
#include "app_error.h"
#include "app_trace.h"
#include "sdk_common.h"

#define BLE_ANCS_NOTIF_EVT_ID_INDEX       0                       /**< Index of the Event ID field when parsing notifications. */
#define BLE_ANCS_NOTIF_FLAGS_INDEX        1                       /**< Index of the Flags field when parsing notifications. */
#define BLE_ANCS_NOTIF_CATEGORY_ID_INDEX  2                       /**< Index of the Category ID field when parsing notifications. */
#define BLE_ANCS_NOTIF_CATEGORY_CNT_INDEX 3                       /**< Index of the Category Count field when parsing notifications. */
#define BLE_ANCS_NOTIF_NOTIF_UID          4                       /**< Index of the Notification UID field when patsin notifications. */

#define ANCS_LOG                         NRF_LOG_PRINTF_DEBUG     /**< Debug logger macro that will be used in this file to do logging of important information over UART. */

#define START_HANDLE_DISCOVER            0x0001                   /**< Value of start handle during discovery. */

#define TX_BUFFER_MASK                   0x07                     /**< TX buffer mask. Must be a mask of contiguous zeroes followed by a contiguous sequence of ones: 000...111. */
#define TX_BUFFER_SIZE                   (TX_BUFFER_MASK + 1)     /**< Size of send buffer, which is 1 higher than the mask. */
#define WRITE_MESSAGE_LENGTH             20                       /**< Length of the write message for CCCD/control point. */
#define BLE_CCCD_NOTIFY_BIT_MASK         0x0001                   /**< Enable notification bit. */

#define BLE_ANCS_MAX_DISCOVERED_CENTRALS DEVICE_MANAGER_MAX_BONDS /**< Maximum number of discovered services that can be stored in the flash. This number should be identical to maximum number of bonded peer devices. */

#define TIME_STRING_LEN                  15                       /**< Unicode Technical Standard (UTS) #35 date format pattern "yyyyMMdd'T'HHmmSS" + "'\0'". */

#define DISCOVERED_SERVICE_DB_SIZE \
    CEIL_DIV(sizeof(ble_ancs_c_service_t) * BLE_ANCS_MAX_DISCOVERED_CENTRALS, sizeof(uint32_t)) /**< Size of bonded peer's database in word size (4 byte). */


/**@brief ANCS request types.
 */
typedef enum
{
    READ_REQ = 1,  /**< Type identifying that this tx_message is a read request. */
    WRITE_REQ      /**< Type identifying that this tx_message is a write request. */
} ancs_tx_request_t;


/**@brief Structure for writing a message to the central, i.e. Control Point or CCCD.
 */
typedef struct
{
    uint8_t                  gattc_value[WRITE_MESSAGE_LENGTH]; /**< The message to write. */
    ble_gattc_write_params_t gattc_params;                      /**< GATTC parameters for this message. */
} write_params_t;


/**@brief Structure for holding data to be transmitted to the connected master.
 */
typedef struct
{
    uint16_t          conn_handle;  /**< Connection handle to be used when transmitting this message. */
    ancs_tx_request_t type;         /**< Type of this message, i.e. read or write message. */
    union
    {
        uint16_t       read_handle; /**< Read request message. */
        write_params_t write_req;   /**< Write request message. */
    } req;
} tx_message_t;


static tx_message_t m_tx_buffer[TX_BUFFER_SIZE];                           /**< Transmit buffer for messages to be transmitted to the Notification Provider. */
static uint32_t     m_tx_insert_index = 0;                                 /**< Current index in the transmit buffer where the next message should be inserted. */
static uint32_t     m_tx_index        = 0;                                 /**< Current index in the transmit buffer from where the next message to be transmitted resides. */


/**@brief 128-bit service UUID for the Apple Notification Center Service.
 */
const ble_uuid128_t ble_ancs_base_uuid128 =
{
    {
        // 7905F431-B5CE-4E99-A40F-4B1E122D00D0
        0xd0, 0x00, 0x2d, 0x12, 0x1e, 0x4b, 0x0f, 0xa4,
        0x99, 0x4e, 0xce, 0xb5, 0x31, 0xf4, 0x05, 0x79
    }
};


/**@brief 128-bit control point UUID.
 */
const ble_uuid128_t ble_ancs_cp_base_uuid128 =
{
    {
        // 69d1d8f3-45e1-49a8-9821-9BBDFDAAD9D9
        0xd9, 0xd9, 0xaa, 0xfd, 0xbd, 0x9b, 0x21, 0x98,
        0xa8, 0x49, 0xe1, 0x45, 0xf3, 0xd8, 0xd1, 0x69
    }
};

/**@brief 128-bit notification source UUID.
*/
const ble_uuid128_t ble_ancs_ns_base_uuid128 =
{
    {
        // 9FBF120D-6301-42D9-8C58-25E699A21DBD
        0xbd, 0x1d, 0xa2, 0x99, 0xe6, 0x25, 0x58, 0x8c,
        0xd9, 0x42, 0x01, 0x63, 0x0d, 0x12, 0xbf, 0x9f

    }
};

/**@brief 128-bit data source UUID.
*/
const ble_uuid128_t ble_ancs_ds_base_uuid128 =
{
    {
        // 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB
        0xfb, 0x7b, 0x7c, 0xce, 0x6a, 0xb3, 0x44, 0xbe,
        0xb5, 0x4b, 0xd6, 0x24, 0xe9, 0xc6, 0xea, 0x22
    }
};


/**@brief  Function for handling Disconnected event received from the SoftDevice.
 *
 * @details This function check if the disconnect event is happening on the link
 *          associated with the current instance of the module, if so it will set its
 *          conn_handle to invalid.
 *
 * @param[in] p_ancs    Pointer to the ANCS client structure.
 * @param[in] p_ble_evt Pointer to the BLE event received.
 */
static void on_disconnected(ble_ancs_c_t * p_ancs, const ble_evt_t * p_ble_evt)
{
    if (p_ancs->conn_handle == p_ble_evt->evt.gap_evt.conn_handle)
    {
        p_ancs->conn_handle = BLE_CONN_HANDLE_INVALID;
    }
}


void ble_ancs_c_on_db_disc_evt(ble_ancs_c_t * p_ancs, ble_db_discovery_evt_t * p_evt)
{
    ANCS_LOG("[ANCS]: Database Discovery handler called with event 0x%x\r\n", p_evt->evt_type);

    ble_ancs_c_evt_t     evt;
    ble_gatt_db_char_t * p_chars;

    p_chars = p_evt->params.discovered_db.charateristics;

    // Check if the ANCS Service was discovered.
    if (p_evt->evt_type == BLE_DB_DISCOVERY_COMPLETE &&
        p_evt->params.discovered_db.srv_uuid.uuid == ANCS_UUID_SERVICE &&
        p_evt->params.discovered_db.srv_uuid.type == p_ancs->service.service.uuid.type)
    {
        // Find the handles of the ANCS characteristic.
        uint32_t i;

        for (i = 0; i < p_evt->params.discovered_db.char_count; i++)
        {
            switch (p_chars[i].characteristic.uuid.uuid)
            {
                case ANCS_UUID_CHAR_CONTROL_POINT:
                    ANCS_LOG("[ANCS]: Control Point Characteristic found.\n\r");
                    memcpy(&evt.service.control_point_char,
                           &p_chars[i].characteristic,
                           sizeof(ble_gattc_char_t));
                    break;

                case ANCS_UUID_CHAR_DATA_SOURCE:
                    ANCS_LOG("[ANCS]: Data Source Characteristic found.\n\r");
                    memcpy(&evt.service.data_source_char,
                           &p_chars[i].characteristic,
                           sizeof(ble_gattc_char_t));
                    evt.service.data_source_cccd.handle = p_chars[i].cccd_handle;
                    break;

                case ANCS_UUID_CHAR_NOTIFICATION_SOURCE:
                    ANCS_LOG("[ANCS]: Notification point Characteristic found.\n\r");
                    memcpy(&evt.service.notif_source_char,
                           &p_chars[i].characteristic,
                           sizeof(ble_gattc_char_t));
                    evt.service.notif_source_cccd.handle = p_chars[i].cccd_handle;
                    break;

                default:
                    break;
            }
        }
        evt.evt_type    = BLE_ANCS_C_EVT_DISCOVERY_COMPLETE;
        evt.conn_handle = p_evt->conn_handle;
        p_ancs->evt_handler(&evt);
    }
    else
    {
        evt.evt_type = BLE_ANCS_C_EVT_DISCOVERY_FAILED;
        p_ancs->evt_handler(&evt);
    }
}


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
            ++m_tx_index;
            m_tx_index &= TX_BUFFER_MASK;
        }
    }
}


/**@brief Function for parsing command id and notification id.
 *        Used in the @ref parse_get_notif_attrs_response state machine.
 *
 * @details UID and command ID will be received only once at the beginning of the first 
 *          GATTC notification of a new attribute request for a given iOS notification.
 *
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] index      Pointer to an index that helps us keep track of the current data to be parsed.
 *
 * @return The next parse state.
 */
static ble_ancs_c_parse_state_t command_id_and_notif_parse(ble_ancs_c_t * p_ancs,
                                                           const uint8_t * p_data_src,
                                                           uint32_t * index)
{
    ble_ancs_c_command_id_values_t command_id;

    command_id = (ble_ancs_c_command_id_values_t) p_data_src[(*index)++];
    
    if(command_id != BLE_ANCS_COMMAND_ID_GET_NOTIF_ATTRIBUTES)
    {
        ANCS_LOG("[ANCS]: Invalid Command ID");
        return DONE;
    }

     p_ancs->evt.attr.notif_uid  = uint32_decode(&p_data_src[*index]);
     *index                     += sizeof(uint32_t);
     return ATTR_ID;

}

/**@brief Function for parsing the id of an iOS attribute.
 *        Used in the @ref parse_get_notif_attrs_response state machine.
 *
 * @details We only request attributes that are registered with @ref ble_ancs_c_attr_add
 *          once they have been reveiced we stop parsing.
 *
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] index      Pointer to an index that helps us keep track of the current data to be parsed.
 *
 * @return The next parse state.
 */
static ble_ancs_c_parse_state_t attr_id_parse(ble_ancs_c_t * p_ancs,
                                              const uint8_t * p_data_src,
                                              uint32_t * index)
{
    p_ancs->evt.attr.attr_id     = (ble_ancs_c_notif_attr_id_values_t) p_data_src[(*index)++];
    p_ancs->evt.attr.p_attr_data = p_ancs->ancs_attr_list[p_ancs->evt.attr.attr_id].p_attr_data;

    if (p_ancs->expected_number_of_attrs == 0)
    {
        ANCS_LOG("[ANCS]: All requested attributes received\n\r");
       // (*index)++;
        return DONE;
    }
    else if (p_ancs->ancs_attr_list[p_ancs->evt.attr.attr_id].get == true)
    {
        ANCS_LOG("[ANCS]: Attribute ID %i \n\r", p_ancs->evt.attr.attr_id);
        return ATTR_LEN1;
    }
    else
    {
        p_ancs->expected_number_of_attrs--;
        return ATTR_ID;
    }
}


/**@brief Function for parsing the length of an iOS attribute.
 *        Used in the @ref parse_get_notif_attrs_response state machine.
 *
 * @details The Length is 2 bytes. Since there is a chance we reveice the bytes in two different
 *          GATTC notifications, we parse only the first byte here and then set the state machine
 *          ready to parse the next byte.
 *
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] index      Pointer to an index that helps us keep track of the current data to be parsed.
 *
 * @return The next parse state.
 */
static ble_ancs_c_parse_state_t attr_len1_parse(ble_ancs_c_t * p_ancs, const uint8_t * p_data_src, uint32_t * index)
{
    p_ancs->evt.attr.attr_len  = p_data_src[(*index)++];
    return ATTR_LEN2;
}

/**@brief Function for parsing the length of an iOS attribute.
 *        Used in the @ref parse_get_notif_attrs_response state machine.
 *
 * @details Second byte of the length field. If the length is zero, it means that the attribute is not
 *          present and the state machine is set to parse the next attribute.
 *
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] index      Pointer to an index that helps us keep track of the current data to be parsed.
 *
 * @return The next parse state.
 */
static ble_ancs_c_parse_state_t attr_len2_parse(ble_ancs_c_t * p_ancs, const uint8_t * p_data_src, uint32_t * index)
{
    p_ancs->evt.attr.attr_len |= (p_data_src[(*index)++] << 8);
    p_ancs->current_attr_index = 0;

    if (p_ancs->evt.attr.attr_len != 0)
    {
        return ATTR_DATA;
    }
    else
    {
        ANCS_LOG("[ANCS]: Attribute LEN %i \n\r", p_ancs->evt.attr.attr_len);
        p_ancs->evt.evt_type = BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE;
        p_ancs->evt_handler(&p_ancs->evt);
        return ATTR_ID;
    }
}

/**@brief Function for parsing the data of an iOS attribute.
 *        Used in the @ref parse_get_notif_attrs_response state machine.
 *
 * @details Read the data of the attribute into our local buffer.
 *
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] index      Pointer to an index that helps us keep track of the current data to be parsed.
 *
 * @return The next parse state.
 */
static ble_ancs_c_parse_state_t attr_data_parse(ble_ancs_c_t * p_ancs, const uint8_t * p_data_src, uint32_t * index)
{
    // We have not reached the end of the attribute, nor our max allocated internal size.
    // Proceed with copying data over to our buffer.
    if (   (p_ancs->current_attr_index < p_ancs->ancs_attr_list[p_ancs->evt.attr.attr_id].attr_len)
        && (p_ancs->current_attr_index < p_ancs->evt.attr.attr_len))
    {
        p_ancs->evt.attr.p_attr_data[p_ancs->current_attr_index++] = p_data_src[(*index)++];
    }

    // We have reached the end of the attribute, or our max allocated internal size.
    // Stop copying data over to our buffer. NUL-terminate at the current index.
    if ( (p_ancs->current_attr_index == p_ancs->evt.attr.attr_len) ||
         (p_ancs->current_attr_index == p_ancs->ancs_attr_list[p_ancs->evt.attr.attr_id].attr_len))
    {
        p_ancs->evt.attr.p_attr_data[p_ancs->current_attr_index] = '\0';
        
        // If our max buffer size is smaller than the remaining attribute data, we must
        // increase index to skip the data until the start of the next attribute.
        if (p_ancs->current_attr_index < p_ancs->evt.attr.attr_len)
        {
            (*index) += (p_ancs->evt.attr.attr_len - p_ancs->current_attr_index);
        }
        ANCS_LOG("[ANCS]: Attribute finished!\n\r");
        p_ancs->evt.evt_type = BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE;
        p_ancs->evt_handler(&p_ancs->evt);
        return ATTR_ID;
    }
    return ATTR_DATA;
}

/**@brief Function for parsing received notification attribute response data.
 *
 * @details The data that comes from the Notification Provider can be much longer than what
 *          would fit in a single GATTC notification. Therefore, this function relies on a
 *          state-oriented switch case.
 *          UID and command ID will be received only once at the beginning of the first
 *          GATTC notification of a new attribute request for a given iOS notification.
 *          After this, we can loop several ATTR_ID > LENGTH > DATA > ATTR_ID > LENGTH > DATA until
 *          we have received all attributes we wanted as a Notification Consumer.
 *          The Notification Provider can also simply stop sending attributes.
 *
 *    |1 Byte  |  4 Bytes    |1 Byte |2 Bytes | X Bytes            |1 Bytes| 2 Bytes| X Bytes   
 *    +--------+-------------+-------+--------+- - - - - - - - - - +-------+--------+- - - - - - -
 *    | CMD_ID |  NOTIF_UID  |ATTR_ID| LENGTH |        DATA        |ATTR_ID| LENGTH |    DATA
 *    +--------+-------------+-------+--------+- - - - - - - - - - +-------+--------+- - - - - - -
 *
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] hvx_len    Length of the data that was received from the Notification Provider.
 */
static void parse_get_notif_attrs_response(ble_ancs_c_t  * p_ancs,
                                           const uint8_t * p_data_src,
                                           const uint16_t  hvx_data_len)
{
    uint32_t index;

    for (index = 0; index < hvx_data_len;)
    {
        switch (p_ancs->parse_state)
        {
            case COMMAND_ID_AND_NOTIF_UID:
                p_ancs->parse_state = command_id_and_notif_parse(p_ancs, p_data_src, &index);
                break;

            case ATTR_ID:
                p_ancs->parse_state = attr_id_parse(p_ancs, p_data_src, &index);
                break;

            case ATTR_LEN1:
                p_ancs->parse_state = attr_len1_parse(p_ancs, p_data_src, &index);
                break;

            case ATTR_LEN2:
                p_ancs->parse_state = attr_len2_parse(p_ancs, p_data_src, &index);
                break;

            case ATTR_DATA:
                p_ancs->parse_state = attr_data_parse(p_ancs, p_data_src, &index);
                break;

            case DONE:
                ANCS_LOG("[ANCS]: State: Done \n\r");
                index = hvx_data_len;
                break;

            default:
                // Default case will never trigger intentionally. Go to the DONE state to minimize the consequences.
                p_ancs->parse_state = DONE;
                break;
        }
    }
}


/**@brief Function for checking if data in an iOS notification is out of bounds.
 *
 * @param[in] notif  An iOS notification.
 *
 * @retval NRF_SUCCESS             If the notification is within bounds.
 * @retval NRF_ERROR_INVALID_PARAM If the notification is out of bounds.
 */
static uint32_t ble_ancs_verify_notification_format(const ble_ancs_c_evt_notif_t * notif)
{
    if(   (notif->evt_id >= BLE_ANCS_NB_OF_EVT_ID)
       || (notif->category_id >= BLE_ANCS_NB_OF_CATEGORY_ID))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    return NRF_SUCCESS;
}

/**@brief Function for receiving and validating notifications received from the Notification Provider.
 * 
 * @param[in] p_ancs     Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_data_src Pointer to data that was received from the Notification Provider.
 * @param[in] hvx_len    Length of the data that was received by the Notification Provider.
 */
static void parse_notif(const ble_ancs_c_t * p_ancs,
                        const uint8_t      * p_data_src,
                        const uint16_t       hvx_data_len)
{
    ble_ancs_c_evt_t ancs_evt;
    uint32_t         err_code;
    if (hvx_data_len != BLE_ANCS_NOTIFICATION_DATA_LENGTH)
    {
        ancs_evt.evt_type = BLE_ANCS_C_EVT_INVALID_NOTIF;
        p_ancs->evt_handler(&ancs_evt);
    }

    /*lint --e{415} --e{416} -save suppress Warning 415: possible access out of bond */
    ancs_evt.notif.evt_id                    =
            (ble_ancs_c_evt_id_values_t) p_data_src[BLE_ANCS_NOTIF_EVT_ID_INDEX];

    ancs_evt.notif.evt_flags.silent          =
            (p_data_src[BLE_ANCS_NOTIF_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_SILENT) & 0x01;

    ancs_evt.notif.evt_flags.important       =
            (p_data_src[BLE_ANCS_NOTIF_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_IMPORTANT) & 0x01;

    ancs_evt.notif.evt_flags.pre_existing    =
            (p_data_src[BLE_ANCS_NOTIF_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_PREEXISTING) & 0x01;

    ancs_evt.notif.evt_flags.positive_action =
            (p_data_src[BLE_ANCS_NOTIF_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_POSITIVE_ACTION) & 0x01;

    ancs_evt.notif.evt_flags.negative_action =
            (p_data_src[BLE_ANCS_NOTIF_FLAGS_INDEX] >> BLE_ANCS_EVENT_FLAG_NEGATIVE_ACTION) & 0x01;

    ancs_evt.notif.category_id               =
        (ble_ancs_c_category_id_values_t) p_data_src[BLE_ANCS_NOTIF_CATEGORY_ID_INDEX];

    ancs_evt.notif.category_count            = p_data_src[BLE_ANCS_NOTIF_CATEGORY_CNT_INDEX];
    ancs_evt.notif.notif_uid = uint32_decode(&p_data_src[BLE_ANCS_NOTIF_NOTIF_UID]);
    /*lint -restore*/

    err_code = ble_ancs_verify_notification_format(&ancs_evt.notif);
    if (err_code == NRF_SUCCESS)
    {
        ancs_evt.evt_type = BLE_ANCS_C_EVT_NOTIF;
    }
    else
    {
        ancs_evt.evt_type = BLE_ANCS_C_EVT_INVALID_NOTIF;
    }

    p_ancs->evt_handler(&ancs_evt);
}


/**@brief Function for receiving and validating notifications received from the Notification Provider.
 * 
 * @param[in] p_ancs    Pointer to an ANCS instance to which the event belongs.
 * @param[in] p_ble_evt Bluetooth stack event.
 */
static void on_evt_gattc_notif(ble_ancs_c_t * p_ancs, const ble_evt_t * p_ble_evt)
{
    const ble_gattc_evt_hvx_t * p_notif = &p_ble_evt->evt.gattc_evt.params.hvx;

    if(p_ble_evt->evt.gattc_evt.conn_handle != p_ancs->conn_handle)
    {
        return;
    }

    if (p_notif->handle == p_ancs->service.notif_source_char.handle_value)
    {
        parse_notif(p_ancs, p_notif->data, p_notif->len);
    }
    else if (p_notif->handle == p_ancs->service.data_source_char.handle_value)
    {
        parse_get_notif_attrs_response(p_ancs, p_notif->data, p_notif->len);
    }
    else
    {
        // No applicable action.
    }
}

/**@brief Function for handling write response events.
 *
 * @param[in] p_ancs_c   Pointer to the Battery Service Client Structure.
 * @param[in] p_ble_evt Pointer to the SoftDevice event.
 */
static void on_write_rsp(ble_ancs_c_t * p_ancs, const ble_evt_t * p_ble_evt)
{
    // Check if the event if on the link for this instance
    if (p_ancs->conn_handle != p_ble_evt->evt.gattc_evt.conn_handle)
    {
        return;
    }
    // Check if there is any message to be sent across to the peer and send it.
    tx_buffer_process();
}


void ble_ancs_c_on_ble_evt(ble_ancs_c_t * p_ancs, const ble_evt_t * p_ble_evt)
{
    uint16_t evt = p_ble_evt->header.evt_id;

    switch (evt)
    {
        case BLE_GATTC_EVT_WRITE_RSP:
            on_write_rsp(p_ancs, p_ble_evt);
            break;

        case BLE_GATTC_EVT_HVX:
            on_evt_gattc_notif(p_ancs, p_ble_evt);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnected(p_ancs, p_ble_evt);
            break;
        default:
            break;
    }
}


uint32_t ble_ancs_c_init(ble_ancs_c_t * p_ancs, const ble_ancs_c_init_t * p_ancs_init)
{
    uint32_t err_code;
    
    //Verify that the parameters needed for to initialize this instance of ANCS are not NULL.
    VERIFY_PARAM_NOT_NULL(p_ancs);
    VERIFY_PARAM_NOT_NULL(p_ancs_init);
    VERIFY_PARAM_NOT_NULL(p_ancs_init->evt_handler);
    
    p_ancs->parse_state = COMMAND_ID_AND_NOTIF_UID;
    p_ancs->p_data_dest = NULL;
    p_ancs->current_attr_index = 0;

    p_ancs->evt_handler    = p_ancs_init->evt_handler;
    p_ancs->error_handler  = p_ancs_init->error_handler;
    p_ancs->conn_handle    = BLE_CONN_HANDLE_INVALID;

    p_ancs->service.data_source_cccd.uuid.uuid  = BLE_UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG;
    p_ancs->service.notif_source_cccd.uuid.uuid = BLE_UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG;

    // Make sure instance of service is clear. GATT handles inside the service and characteristics are set to @ref BLE_GATT_HANDLE_INVALID.
    memset(&p_ancs->service, 0, sizeof(ble_ancs_c_service_t));
    memset(m_tx_buffer, 0, TX_BUFFER_SIZE);

    // Assign UUID types.
    err_code = sd_ble_uuid_vs_add(&ble_ancs_base_uuid128, &p_ancs->service.service.uuid.type);
    VERIFY_SUCCESS(err_code);

    err_code = sd_ble_uuid_vs_add(&ble_ancs_cp_base_uuid128, &p_ancs->service.control_point_char.uuid.type);
    VERIFY_SUCCESS(err_code);

    err_code = sd_ble_uuid_vs_add(&ble_ancs_ns_base_uuid128, &p_ancs->service.notif_source_char.uuid.type);
    VERIFY_SUCCESS(err_code);

    err_code = sd_ble_uuid_vs_add(&ble_ancs_ds_base_uuid128, &p_ancs->service.data_source_char.uuid.type);
    VERIFY_SUCCESS(err_code);

    // Assign UUID to the service.
    p_ancs->service.service.uuid.uuid = ANCS_UUID_SERVICE;
    p_ancs->service.service.uuid.type = p_ancs->service.service.uuid.type;

    return ble_db_discovery_evt_register(&p_ancs->service.service.uuid);
}


/**@brief Function for creating a TX message for writing a CCCD.
 *
 * @param[in] conn_handle  Connection handle on which to perform the configuration.
 * @param[in] handle_cccd  Handle of the CCCD.
 * @param[in] enable       Enable or disable GATTC notifications.
 *
 * @retval NRF_SUCCESS              If the message was created successfully.
 * @retval NRF_ERROR_INVALID_PARAM  If one of the input parameters was invalid.
 */
static uint32_t cccd_configure(const uint16_t conn_handle, const uint16_t handle_cccd, bool enable)
{
    tx_message_t * p_msg;
    uint16_t       cccd_val = enable ? BLE_CCCD_NOTIFY_BIT_MASK : 0;

    p_msg              = &m_tx_buffer[m_tx_insert_index++];
    m_tx_insert_index &= TX_BUFFER_MASK;

    p_msg->req.write_req.gattc_params.handle   = handle_cccd;
    p_msg->req.write_req.gattc_params.len      = 2;
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


uint32_t ble_ancs_c_notif_source_notif_enable(const ble_ancs_c_t * p_ancs)
{
    ANCS_LOG("[ANCS]: Enable Notification Source notifications. writing to handle: %i \n\r",
         p_ancs->service.notif_source_cccd.handle);
    return cccd_configure(p_ancs->conn_handle, p_ancs->service.notif_source_cccd.handle, true);
}


uint32_t ble_ancs_c_notif_source_notif_disable(const ble_ancs_c_t * p_ancs)
{
    return cccd_configure(p_ancs->conn_handle, p_ancs->service.notif_source_cccd.handle, false);
}


uint32_t ble_ancs_c_data_source_notif_enable(const ble_ancs_c_t * p_ancs)
{
    ANCS_LOG("[ANCS]: Enable Data Source notifications. Writing to handle: %i \n\r",
        p_ancs->service.data_source_cccd.handle);
    return cccd_configure(p_ancs->conn_handle, p_ancs->service.data_source_cccd.handle, true);
}


uint32_t ble_ancs_c_data_source_notif_disable(const ble_ancs_c_t * p_ancs)
{
    return cccd_configure(p_ancs->conn_handle, p_ancs->service.data_source_cccd.handle, false);
}


uint32_t ble_ancs_get_notif_attrs(ble_ancs_c_t       * p_ancs,
                                  const uint32_t       p_uid)
{
    tx_message_t * p_msg;

    uint32_t index                   = 0;
    p_ancs->number_of_requested_attr = 0;
    p_msg              = &m_tx_buffer[m_tx_insert_index++];
    m_tx_insert_index &= TX_BUFFER_MASK;

    p_msg->req.write_req.gattc_params.handle   = p_ancs->service.control_point_char.handle_value;
    p_msg->req.write_req.gattc_params.p_value  = p_msg->req.write_req.gattc_value;
    p_msg->req.write_req.gattc_params.offset   = 0;
    p_msg->req.write_req.gattc_params.write_op = BLE_GATT_OP_WRITE_REQ;

    //Encode Command ID.
    p_msg->req.write_req.gattc_value[index++] = BLE_ANCS_COMMAND_ID_GET_NOTIF_ATTRIBUTES;
    
    //Encode Notification UID.
    index += uint32_encode(p_uid, &p_msg->req.write_req.gattc_value[index]);

    //Encode Attribute ID.
    for (uint32_t attr = 0; attr < BLE_ANCS_NB_OF_ATTRS; attr++)
    {
        if (p_ancs->ancs_attr_list[attr].get == true)
        {
            p_msg->req.write_req.gattc_value[index++] = attr;
            if ((attr == BLE_ANCS_NOTIF_ATTR_ID_TITLE) ||
                (attr == BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE) ||
                (attr == BLE_ANCS_NOTIF_ATTR_ID_MESSAGE))
            {
                //Encode Length field, only applicable for Title, Subtitle and Message
                index += uint16_encode(p_ancs->ancs_attr_list[attr].attr_len,
                              &p_msg->req.write_req.gattc_value[index]);
            }
            p_ancs->number_of_requested_attr++;
        }
    }
    p_msg->req.write_req.gattc_params.len = index;
    p_msg->conn_handle                    = p_ancs->conn_handle;
    p_msg->type                           = WRITE_REQ;
    p_ancs->expected_number_of_attrs      = p_ancs->number_of_requested_attr;

    tx_buffer_process();

    return NRF_SUCCESS;
}


uint32_t ble_ancs_c_attr_add(ble_ancs_c_t                          * p_ancs,
                             const ble_ancs_c_notif_attr_id_values_t id,
                             uint8_t                               * p_data,
                             const uint16_t                          len)
{
    VERIFY_PARAM_NOT_NULL(p_data);

    if((len == 0) || (len > BLE_ANCS_ATTR_DATA_MAX))
    {
        return NRF_ERROR_INVALID_LENGTH;
    }

    p_ancs->ancs_attr_list[id].get         = true;
    p_ancs->ancs_attr_list[id].attr_len    = len;
    p_ancs->ancs_attr_list[id].p_attr_data = p_data;

    return NRF_SUCCESS;
}


uint32_t ble_ancs_c_request_attrs(ble_ancs_c_t * p_ancs,
                                  const ble_ancs_c_evt_notif_t * p_notif)
{
    uint32_t err_code;
    err_code = ble_ancs_verify_notification_format(p_notif);
    VERIFY_SUCCESS(err_code);

    err_code            = ble_ancs_get_notif_attrs(p_ancs, p_notif->notif_uid);
    p_ancs->parse_state = COMMAND_ID_AND_NOTIF_UID;
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

uint32_t ble_ancs_c_handles_assign(ble_ancs_c_t * p_ancs,
                                   const uint16_t conn_handle,
                                   const ble_ancs_c_service_t * p_peer_handles)
{
    VERIFY_PARAM_NOT_NULL(p_ancs);

    p_ancs->conn_handle = conn_handle;
    
    if(p_peer_handles != NULL)
    {
        p_ancs->service.control_point_char.handle_value = p_peer_handles->control_point_char.handle_value;
        p_ancs->service.data_source_cccd.handle         = p_peer_handles->data_source_cccd.handle;
        p_ancs->service.data_source_char.handle_value   = p_peer_handles->data_source_char.handle_value;
        p_ancs->service.notif_source_cccd.handle        = p_peer_handles->notif_source_cccd.handle;
        p_ancs->service.notif_source_char.handle_value  = p_peer_handles->notif_source_char.handle_value;
    }
    
    return NRF_SUCCESS;
}
