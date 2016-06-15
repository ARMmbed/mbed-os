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

/**@file
 *
 * @defgroup ble_sdk_srv_dfu Device Firmware Update Service
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Device Firmware Update Service
 *
 * @details  The Device Firmware Update (DFU) service is a GATT based service that can be used for
 *           performing firmware updates over BLE. Note that this implementation uses vendor
 *           specific UUIDs for service and characteristics and is intended to demonstrate the
 *           firmware updates over BLE. Refer @ref bledfu_transport_bleservice and @ref
 *            bledfu_transport_bleprofile for more information on the service and profile respectively.
 */

#ifndef BLE_DFU_H__
#define BLE_DFU_H__

#include <stdint.h>
#include "ble_gatts.h"
#include "ble_gap.h"
#include "ble.h"
#include "ble_srv_common.h"

#define BLE_DFU_SERVICE_UUID                 0x1530                       /**< The UUID of the DFU Service. */
#define BLE_DFU_PKT_CHAR_UUID                0x1532                       /**< The UUID of the DFU Packet Characteristic. */
#define BLE_DFU_CTRL_PT_UUID                 0x1531                       /**< The UUID of the DFU Control Point. */
#define BLE_DFU_STATUS_REP_UUID              0x1533                       /**< The UUID of the DFU Status Report Characteristic. */
#define BLE_DFU_REV_CHAR_UUID                0x1534                       /**< The UUID of the DFU Revision Characteristic. */

/**@brief   DFU Event type.
 *
 * @details This enumeration contains the types of events that will be received from the DFU Service.
 */
typedef enum
{
    BLE_DFU_START,                                                      /**< The event indicating that the peer wants the application to prepare for a new firmware update. */
    BLE_DFU_RECEIVE_INIT_DATA,                                          /**< The event indicating that the peer wants the application to prepare to receive init parameters. */
    BLE_DFU_RECEIVE_APP_DATA,                                           /**< The event indicating that the peer wants the application to prepare to receive the new firmware image. */
    BLE_DFU_VALIDATE,                                                   /**< The event indicating that the peer wants the application to validate the newly received firmware image. */
    BLE_DFU_ACTIVATE_N_RESET,                                           /**< The event indicating that the peer wants the application to undergo activate new firmware and restart with new valid application */
    BLE_DFU_SYS_RESET,                                                  /**< The event indicating that the peer wants the application to undergo a reset and start the currently valid application image.*/
    BLE_DFU_PKT_RCPT_NOTIF_ENABLED,                                     /**< The event indicating that the peer has enabled packet receipt notifications. It is the responsibility of the application to call @ref ble_dfu_pkts_rcpt_notify each time the number of packets indicated by num_of_pkts field in @ref ble_dfu_evt_t is received.*/
    BLE_DFU_PKT_RCPT_NOTIF_DISABLED,                                    /**< The event indicating that the peer has disabled the packet receipt notifications.*/
    BLE_DFU_PACKET_WRITE,                                               /**< The event indicating that the peer has written a value to the 'DFU Packet' characteristic. The data received from the peer will be present in the @ref BLE_DFU_PACKET_WRITE element contained within @ref ble_dfu_evt_t.*/
    BLE_DFU_BYTES_RECEIVED_SEND                                         /**< The event indicating that the peer is requesting for the number of bytes of firmware data last received by the application. It is the responsibility of the application to call @ref ble_dfu_pkts_rcpt_notify in response to this event. */
} ble_dfu_evt_type_t;

/**@brief   DFU Procedure type.
 *
 * @details This enumeration contains the types of DFU procedures.
 */
typedef enum
{
    BLE_DFU_START_PROCEDURE        = 1,                                 /**< DFU Start procedure.*/
    BLE_DFU_INIT_PROCEDURE         = 2,                                 /**< DFU Initialization procedure.*/
    BLE_DFU_RECEIVE_APP_PROCEDURE  = 3,                                 /**< Firmware receiving procedure.*/
    BLE_DFU_VALIDATE_PROCEDURE     = 4,                                 /**< Firmware image validation procedure .*/
    BLE_DFU_PKT_RCPT_REQ_PROCEDURE = 8                                  /**< Packet receipt notification request procedure. */
} ble_dfu_procedure_t;

/**@brief   DFU Response value type.
 */
typedef enum
{
    BLE_DFU_RESP_VAL_SUCCESS = 1,                                       /**< Success.*/
    BLE_DFU_RESP_VAL_INVALID_STATE,                                     /**< Invalid state.*/
    BLE_DFU_RESP_VAL_NOT_SUPPORTED,                                     /**< Operation not supported.*/
    BLE_DFU_RESP_VAL_DATA_SIZE,                                         /**< Data size exceeds limit.*/
    BLE_DFU_RESP_VAL_CRC_ERROR,                                         /**< CRC Error.*/
    BLE_DFU_RESP_VAL_OPER_FAILED                                        /**< Operation failed.*/
} ble_dfu_resp_val_t;


/**@brief   DFU Packet structure.
 *
 * @details This structure contains the value of the DFU Packet characteristic as written by the
 *          peer and the length of the value written. It will be filled by the DFU Service when the
 *          peer writes to the DFU Packet characteristic.
 */
typedef struct
{
    uint8_t *                    p_data;                                /**< Pointer to the received packet. This will point to a word aligned memory location.*/
    uint8_t                      len;                                   /**< Length of the packet received. */
} ble_dfu_pkt_write_t;

/**@brief   Packet receipt notification request structure.
 *
 * @details This structure contains the contents of the packet receipt notification request
 *          sent by the DFU Controller.
 */
typedef struct
{
    uint16_t                     num_of_pkts;                           /**< The number of packets of firmware data to be received by application before sending the next Packet Receipt Notification to the peer. */
} ble_pkt_rcpt_notif_req_t;

/**@brief   DFU Event structure.
 *
 * @details This structure contains the event generated by the DFU Service based on the data
 *          received from the peer.
 */
typedef struct
{
    ble_dfu_evt_type_t           ble_dfu_evt_type;                      /**< Type of the event.*/
    union
    {
        ble_dfu_pkt_write_t      ble_dfu_pkt_write;                     /**< The DFU packet received. This field is when the @ref ble_dfu_evt_type field is set to @ref BLE_DFU_PACKET_WRITE.*/
        ble_pkt_rcpt_notif_req_t pkt_rcpt_notif_req;                    /**< Packet receipt notification request. This field is when the @ref ble_dfu_evt_type field is set to @ref BLE_DFU_PKT_RCPT_NOTIF_ENABLED.*/
    } evt;
} ble_dfu_evt_t;

// Forward declaration of the ble_dfu_t type.
typedef struct ble_dfu_s ble_dfu_t;

/**@brief DFU Service event handler type. */
typedef void (*ble_dfu_evt_handler_t) (ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt);

/**@brief   DFU service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_dfu_s
{
    uint16_t                     conn_handle;                           /**< Handle of the current connection (as provided by the SoftDevice). This will be BLE_CONN_HANDLE_INVALID when not in a connection. */
    uint16_t                     revision;                              /**< Handle of DFU Service (as provided by the SoftDevice). */
    uint16_t                     service_handle;                        /**< Handle of DFU Service (as provided by the SoftDevice). */
    uint8_t                      uuid_type;                             /**< UUID type assigned for DFU Service by the SoftDevice. */
    ble_gatts_char_handles_t     dfu_pkt_handles;                       /**< Handles related to the DFU Packet characteristic. */
    ble_gatts_char_handles_t     dfu_ctrl_pt_handles;                   /**< Handles related to the DFU Control Point characteristic. */
    ble_gatts_char_handles_t     dfu_status_rep_handles;                /**< Handles related to the DFU Status Report characteristic. */
    ble_gatts_char_handles_t     dfu_rev_handles;                       /**< Handles related to the DFU Revision characteristic. */
    ble_dfu_evt_handler_t        evt_handler;                           /**< The event handler to be called when an event is to be sent to the application.*/
    ble_srv_error_handler_t      error_handler;                         /**< Function to be called in case of an error. */
};

/**@brief      DFU service initialization structure.
 *
 * @details    This structure contains the initialization information for the DFU Service. The
 *             application needs to fill this structure and pass it to the DFU Service using the
 *             @ref ble_dfu_init function.
 */
typedef struct
{
    uint16_t                     revision;                              /**< Revision number to be exposed by the DFU service. */
    ble_dfu_evt_handler_t        evt_handler;                           /**< Event handler to be called for handling events in the Device Firmware Update Service. */
    ble_srv_error_handler_t      error_handler;                         /**< Function to be called in case of an error. */
} ble_dfu_init_t;

/**@brief      Function for handling a BLE event.
 *
 * @details    The DFU service expects the application to call this function each time an event
 *             is received from the SoftDevice. This function processes the event, if it is
 *             relevant for the DFU service and calls the DFU event handler of the application if
 *             necessary.
 *
 * @param[in]  p_dfu        Pointer to the DFU service structure.
 * @param[in]  p_ble_evt    Pointer to the event received from SoftDevice.
 */
void ble_dfu_on_ble_evt(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt);

/**@brief      Function for initializing the DFU service.
 *
 * @param[out] p_dfu        Device Firmware Update service structure. This structure will have to be
 *                          supplied by the application. It will be initialized by this function,
 *                          and will later be used to identify the service instance.
 * @param[in]  p_dfu_init   Information needed to initialize the service.
 *
 * @return     NRF_SUCCESS if the DFU service and its characteristics were successfully added to the
 *             SoftDevice. Otherwise an error code.
 *             This function returns NRF_ERROR_NULL if the value of evt_handler in p_dfu_init
 *             structure provided is NULL or if the pointers supplied as input are NULL.
 */
uint32_t ble_dfu_init(ble_dfu_t * p_dfu, ble_dfu_init_t * p_dfu_init);

/**@brief       Function for sending response to a control point command.
 *
 * @details     This function will encode a DFU Control Point response using the given input
 *              parameters and will send a notification of the same to the peer.
 *
 * @param[in]   p_dfu       Pointer to the DFU service structure.
 * @param[in]   dfu_proc    Procedure for which this response is to be sent.
 * @param[in]   resp_val    Response value.
 *
 * @return      NRF_SUCCESS if the DFU Service has successfully requested the SoftDevice to
 *              send the notification. Otherwise an error code.
 *              This function returns NRF_ERROR_INVALID_STATE if the device is not connected to a
 *              peer or if the DFU service is not initialized or if the notification of the DFU
 *              Status Report characteristic was not enabled by the peer. It returns NRF_ERROR_NULL
 *              if the pointer p_dfu is NULL.
 */
uint32_t ble_dfu_response_send(ble_dfu_t *          p_dfu,
                               ble_dfu_procedure_t  dfu_proc,
                               ble_dfu_resp_val_t   resp_val);

/**@brief      Function for notifying the peer about the number of bytes of firmware data received.
 *
 * @param[in]  p_dfu                      Pointer to the DFU service structure.
 * @param[in]  num_of_firmware_bytes_rcvd Number of bytes.
 *
 * @return     NRF_SUCCESS if the DFU Service has successfully requested the SoftDevice to send
 *             the notification. Otherwise an error code.
 *             This function returns NRF_ERROR_INVALID_STATE if the device is not connected to a
 *             peer or if the DFU service is not initialized or if the notification of the DFU
 *             Status Report characteristic was not enabled by the peer. It returns NRF_ERROR_NULL
 *             if the pointer p_dfu is NULL.
 */
uint32_t ble_dfu_bytes_rcvd_report(ble_dfu_t * p_dfu, uint32_t num_of_firmware_bytes_rcvd);

/**@brief      Function for sending Packet Receipt Notification to the peer.
 *
 *             This function will encode the number of bytes received as input parameter into a
 *             notification of the control point characteristic and send it to the peer.
 *
 * @param[in]  p_dfu                      Pointer to the DFU service structure.
 * @param[in]  num_of_firmware_bytes_rcvd Number of bytes of firmware image received.
 *
 * @return     NRF_SUCCESS if the DFU Service has successfully requested the SoftDevice to send
 *             the notification. Otherwise an error code.
 *             This function returns NRF_ERROR_INVALID_STATE if the device is not connected to a
 *             peer or if the DFU service is not initialized or if the notification of the DFU
 *             Status Report characteristic was not enabled by the peer. It returns NRF_ERROR_NULL
 *             if the pointer p_dfu is NULL.
 */
uint32_t ble_dfu_pkts_rcpt_notify(ble_dfu_t * p_dfu, uint32_t num_of_firmware_bytes_rcvd);

#endif // BLE_DFU_H__

/** @} */
