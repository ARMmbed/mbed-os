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

/** @file
 *
 * @defgroup ble_sdk_srv_sc_ctrlpt Speed and Cadence Control Point
 * @{
 * @ingroup ble_sdk_srv
 * @brief Speed and Cadence Control Point module.
 *
 * @details This module implements the Speed and Cadence control point behavior. It is used
 *          by the @ref ble_sdk_srv_csc module and the ble_sdk_srv_rsc module for control point
 *          mechanisms like setting a cumulative value, Start an automatic calibration,
 *          Update the sensor location or request the supported locations.
 *
 * @note Attention! 
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile 
 *  qualification listings, this section of source code must not be modified.
 */

#ifndef BLE_SC_CTRLPT_H__
#define BLE_SC_CTRLPT_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_sensor_location.h"

#define BLE_SC_CTRLPT_MAX_LEN                                      19                     /**< maximum lenght for Speed and cadence control point characteristic value. */
#define BLE_SC_CTRLPT_MIN_LEN                                      1                      /**< minimum length for Speed and cadence control point characteristic value. */

// Forward declaration of the ble_sc_ctrlpt_t type.
typedef struct ble_sc_ctrlpt_s ble_sc_ctrlpt_t;


/**@brief Speed and Cadence Control Point event type. */
typedef enum
{
    BLE_SC_CTRLPT_EVT_UPDATE_LOCATION,                                                    /**< rcvd update location opcode (the control point handles the change of location automatically, the event just informs the application in case it needs to adjust its algorithm). */
    BLE_SC_CTRLPT_EVT_SET_CUMUL_VALUE,                                                    /**< rcvd set cumulative value opcode, it is then up to the application to use the new cumulative value. */
    BLE_SC_CTRLPT_EVT_START_CALIBRATION,                                                  /**< rcvd start calibration opcode, the application needs, at the end ot the calibration to call ble_sc_ctrlpt_send_rsp. */
} ble_sc_ctrlpt_evt_type_t;


/**@brief Speed and Cadence Control point event. */
typedef struct
{
    ble_sc_ctrlpt_evt_type_t evt_type;                                                    /**< Type of event. */
    union
    {
        ble_sensor_location_t update_location;
        uint32_t              cumulative_value;
    }params;
} ble_sc_ctrlpt_evt_t;


/** Speed and Cadence Control Point operator code  (see RSC service specification)*/
typedef enum {
    BLE_SCPT_SET_CUMULATIVE_VALUE                   = 0x01,                               /**< Operator to set a given cumulative value. */
    BLE_SCPT_START_AUTOMATIC_CALIBRATION            = 0x02,                               /**< Operator to start automatic calibration. */
    BLE_SCPT_UPDATE_SENSOR_LOCATION                 = 0x03,                               /**< Operator to update the sensor location. */
    BLE_SCPT_REQUEST_SUPPORTED_SENSOR_LOCATIONS     = 0x04,                               /**< Operator to request the supported sensor locations. */
    BLE_SCPT_RESPONSE_CODE                          = 0x10,                               /**< Response Code. */
} ble_scpt_operator_t;


/** Speed and Cadence Control Point response parameter  (see RSC service specification)*/
typedef enum {
    BLE_SCPT_SUCCESS                                = 0x01,                               /**< Sucess Response. */
    BLE_SCPT_OP_CODE_NOT_SUPPORTED                  = 0x02,                               /**< Error Response received opcode not supported. */
    BLE_SCPT_INVALID_PARAMETER                      = 0x03,                               /**< Error Response received parameter invalid. */
    BLE_SCPT_OPERATION_FAILED                       = 0x04,                               /**< Error Response operation failed. */
} ble_scpt_response_t;


/** Speed and Cadence Control Point procedure status (indicates is a procedure is in progress or not and which procedure is in progress*/
typedef enum {
    BLE_SCPT_NO_PROC_IN_PROGRESS                    = 0x00,                               /**< No procedure in progress. */
    BLE_SCPT_AUTOMATIC_CALIB_IN_PROGRESS            = 0x01,                               /**< Automatic Calibration is in progress. */
    BLE_SCPT_INDICATION_PENDING                     = 0x02,                               /**< Control Point Indication is pending. */
    BLE_SCPT_IND_CONFIRM_PENDING                    = 0x03,                               /**< Waiting for the indication confirmation. */
}ble_scpt_procedure_status_t;

/**@brief Speed and Cadence Control point event handler type. */
typedef ble_scpt_response_t (*ble_sc_ctrlpt_evt_handler_t) (ble_sc_ctrlpt_t * p_sc_ctrlpt,
                                             ble_sc_ctrlpt_evt_t * p_evt);


typedef struct{
    ble_scpt_operator_t   opcode;
    uint32_t              cumulative_value;
    ble_sensor_location_t location;
}ble_sc_ctrlpt_val_t;


typedef struct{
    ble_scpt_operator_t   opcode;
    ble_scpt_response_t   status;
    ble_sensor_location_t location_list[BLE_NB_MAX_SENSOR_LOCATIONS];
}ble_sc_ctrlpt_rsp_t;


/**
 * \defgroup BLE_SRV_SC_CTRLPT_SUPP_FUNC Control point functionalities.
 *@{
 */
#define BLE_SRV_SC_CTRLPT_SENSOR_LOCATIONS_OP_SUPPORTED  0x01                             /**< Support for sensor location related operations */
#define BLE_SRV_SC_CTRLPT_CUM_VAL_OP_SUPPORTED           0x02                             /**< Support for setting cumulative value related operations */
#define BLE_SRV_SC_CTRLPT_START_CALIB_OP_SUPPORTED       0x04                             /**< Support for starting calibration related operations */
/**
  *@}
  */

/**@brief Speed and Cadence Control Point init structure. This contains all options and data
*         needed for initialization of the Speed and Cadence Control Point module. */
typedef struct
{
    ble_srv_cccd_security_mode_t sc_ctrlpt_attr_md;                                       /**< Initial security level for cycling speed and cadence control point attribute */
    uint8_t                      supported_functions;                                     /**< supported control point functionalities see @ref BLE_SRV_SC_CTRLPT_SUPP_FUNC. */
    uint16_t                     service_handle;                                          /**< Handle of the parent service (as provided by the BLE stack). */
    ble_sc_ctrlpt_evt_handler_t  evt_handler;                                             /**< event handler */
    ble_sensor_location_t        *list_supported_locations;                               /**< list of supported sensor locations.*/
    uint8_t                      size_list_supported_locations;                           /**< number of supported sensor locations in the list.*/
    uint16_t                     sensor_location_handle;                                  /**< handle for the sensor location characteristic (if sensor_location related operation are supported).*/
    ble_srv_error_handler_t      error_handler;                                           /**< Function to be called in case of an error. */
} ble_cs_ctrlpt_init_t;


/**@brief Speed and Cadence Control Point response indication structure. */
typedef struct
{
    ble_scpt_response_t          status;                                                  /**< control point response status .*/
    uint8_t                      len;                                                     /**< control point response length .*/
    uint8_t                      encoded_ctrl_rsp[BLE_SC_CTRLPT_MAX_LEN];                 /**< control point encoded response.*/
}ble_sc_ctrlpt_resp_t;


/**@brief Speed and Cadence Control Point structure. This contains various status information for
 *        the Speed and Cadence Control Point behavior. */
struct ble_sc_ctrlpt_s
{
    uint8_t                      supported_functions;                                     /**< supported control point functionalities see @ref BLE_SRV_SC_CTRLPT_SUPP_FUNC. */
    uint16_t                     service_handle;                                          /**< Handle of the parent service (as provided by the BLE stack). */
    ble_gatts_char_handles_t     sc_ctrlpt_handles;                                       /**< Handles related to the Speed and Cadence Control Point characteristic. */
    uint16_t                     conn_handle;                                             /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    ble_sensor_location_t        list_supported_locations[BLE_NB_MAX_SENSOR_LOCATIONS];   /**< list of supported sensor locations.*/
    uint8_t                      size_list_supported_locations;                           /**< number of supported sensor locations in the list.*/
    ble_sc_ctrlpt_evt_handler_t  evt_handler;                                             /**< Handle of the parent service (as provided by the BLE stack). */
    uint16_t                     sensor_location_handle;                                  /**< handle for the sensor location characteristic (if sensor_location related operation are supported).*/
    ble_scpt_procedure_status_t  procedure_status;                                        /**< status of possible procedure*/
    ble_srv_error_handler_t      error_handler;                                           /**< Function to be called in case of an error. */
    ble_sc_ctrlpt_resp_t         response;                                                /**< pending response data.*/
};

#define SCPT_OPCODE_POS                   0                                               /**< Request opcode position. */
#define SCPT_PARAMETER_POS                1                                               /**< Request parameter position. */

#define SCPT_RESPONSE_REQUEST_OPCODE_POS  1                                               /**< Response position of requested opcode. */
#define SCPT_RESPONSE_CODE_POS            2                                               /**< Response position of response code. */
#define SCPT_RESPONSE_PARAMETER           3                                               /**< Response position of response parameter. */

#define SCPT_MIN_RESPONSE_SIZE            3                                               /**< Minimum size for control point response. */
#define SCPT_MAX_RESPONSE_SIZE  (SCPT_MIN_RESPONSE_SIZE + NB_MAX_SENSOR_LOCATIONS)        /**< Maximum size for control point response. */


/**@brief Function for Initializing the Speed and Cadence Control Point.
 *
 * @details Function for Initializing the Speed and Cadence Control Point.
 * @param[in]   p_sc_ctrlpt   Speed and Cadence Control Point structure.
 * @param[in]   p_sc_ctrlpt_init   Information needed to initialize the control point behavior.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_sc_ctrlpt_init(ble_sc_ctrlpt_t *            p_sc_ctrlpt,
                            const ble_cs_ctrlpt_init_t * p_sc_ctrlpt_init);


/**@brief Function for sending a control point response.
 *
 * @details Function for sending a control point response when the control point received was
 *          BLE_SCPT_START_AUTOMATIC_CALIBRATION. To be called after the calibration procedure is finished.
 *
 * @param[in]   p_sc_ctrlpt      Speed and Cadence Control Point structure.
 * @param[in]   response_status  status to include in the control point response.
 */
uint32_t ble_sc_ctrlpt_rsp_send(ble_sc_ctrlpt_t * p_sc_ctrlpt, ble_scpt_response_t response_status);


/**@brief Speed and Cadence Control Point BLE stack event handler.
 *
 * @details Handles all events from the BLE stack of interest to the Speed and Cadence Control Point.
 *
 * @param[in]   p_sc_ctrlpt   Speed and Cadence Control Point structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_sc_ctrlpt_on_ble_evt(ble_sc_ctrlpt_t * p_sc_ctrlpt, ble_evt_t * p_ble_evt);


#endif // BLE_SC_CTRLPT_H__

/** @} */
