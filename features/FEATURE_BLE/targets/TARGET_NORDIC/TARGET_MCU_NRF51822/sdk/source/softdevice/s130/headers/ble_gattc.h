/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
  @addtogroup BLE_GATTC Generic Attribute Profile (GATT) Client
  @{
  @brief  Definitions and prototypes for the GATT Client interface.
 */

#ifndef BLE_GATTC_H__
#define BLE_GATTC_H__

#include "ble_gatt.h"
#include "ble_types.h"
#include "ble_ranges.h"
#include "nrf_svc.h"

/** @addtogroup BLE_GATTC_ENUMERATIONS Enumerations
 * @{ */

/**@brief GATTC API SVC numbers. */
enum BLE_GATTC_SVCS
{
  SD_BLE_GATTC_PRIMARY_SERVICES_DISCOVER = BLE_GATTC_SVC_BASE, /**< Primary Service Discovery. */
  SD_BLE_GATTC_RELATIONSHIPS_DISCOVER,                         /**< Relationship Discovery. */
  SD_BLE_GATTC_CHARACTERISTICS_DISCOVER,                       /**< Characteristic Discovery. */
  SD_BLE_GATTC_DESCRIPTORS_DISCOVER,                           /**< Characteristic Descriptor Discovery. */
  SD_BLE_GATTC_CHAR_VALUE_BY_UUID_READ,                        /**< Read Characteristic Value by UUID. */
  SD_BLE_GATTC_READ,                                           /**< Generic read. */
  SD_BLE_GATTC_CHAR_VALUES_READ,                               /**< Read multiple Characteristic Values. */
  SD_BLE_GATTC_WRITE,                                          /**< Generic write. */
  SD_BLE_GATTC_HV_CONFIRM                                      /**< Handle Value Confirmation. */
};

/**
 * @brief GATT Client Event IDs.
 */
enum BLE_GATTC_EVTS
{
  BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP = BLE_GATTC_EVT_BASE,  /**< Primary Service Discovery Response event. @ref ble_gattc_evt_prim_srvc_disc_rsp_t */
  BLE_GATTC_EVT_REL_DISC_RSP,                             /**< Relationship Discovery Response event. @ref ble_gattc_evt_rel_disc_rsp_t */
  BLE_GATTC_EVT_CHAR_DISC_RSP,                            /**< Characteristic Discovery Response event. @ref ble_gattc_evt_char_disc_rsp_t */
  BLE_GATTC_EVT_DESC_DISC_RSP,                            /**< Descriptor Discovery Response event. @ref ble_gattc_evt_desc_disc_rsp_t */
  BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP,                /**< Read By UUID Response event. @ref ble_gattc_evt_char_val_by_uuid_read_rsp_t */
  BLE_GATTC_EVT_READ_RSP,                                 /**< Read Response event. @ref ble_gattc_evt_read_rsp_t */
  BLE_GATTC_EVT_CHAR_VALS_READ_RSP,                       /**< Read multiple Response event. @ref ble_gattc_evt_char_vals_read_rsp_t */
  BLE_GATTC_EVT_WRITE_RSP,                                /**< Write Response event. @ref ble_gattc_evt_write_rsp_t */
  BLE_GATTC_EVT_HVX,                                      /**< Handle Value Notification or Indication event. @ref ble_gattc_evt_hvx_t */
  BLE_GATTC_EVT_TIMEOUT                                   /**< Timeout event. @ref ble_gattc_evt_timeout_t */
};

/** @} */

/** @addtogroup BLE_GATTC_DEFINES Defines
 * @{ */

/** @defgroup BLE_ERRORS_GATTC SVC return values specific to GATTC
 * @{ */
#define BLE_ERROR_GATTC_PROC_NOT_PERMITTED    (NRF_GATTC_ERR_BASE + 0x000) /**< Procedure not Permitted. */
/** @} */

/**@brief Last Attribute Handle. */
#define BLE_GATTC_HANDLE_END                0xFFFF

/** @} */

/** @addtogroup BLE_GATTC_STRUCTURES Structures
 * @{ */

/**@brief Operation Handle Range. */
typedef struct
{
  uint16_t          start_handle; /**< Start Handle. */
  uint16_t          end_handle;   /**< End Handle. */
} ble_gattc_handle_range_t;


/**@brief GATT service. */
typedef struct
{
  ble_uuid_t               uuid;          /**< Service UUID. */
  ble_gattc_handle_range_t handle_range;  /**< Service Handle Range. */
} ble_gattc_service_t;


/**@brief  GATT include. */
typedef struct
{
  uint16_t            handle;           /**< Include Handle. */
  ble_gattc_service_t included_srvc;    /**< Handle of the included service. */
} ble_gattc_include_t;


/**@brief GATT characteristic. */
typedef struct
{
  ble_uuid_t              uuid;                 /**< Characteristic UUID. */
  ble_gatt_char_props_t   char_props;           /**< Characteristic Properties. */
  uint8_t                 char_ext_props : 1;   /**< Extended properties present. */
  uint16_t                handle_decl;          /**< Handle of the Characteristic Declaration. */
  uint16_t                handle_value;         /**< Handle of the Characteristic Value. */
} ble_gattc_char_t;


/**@brief GATT descriptor. */
typedef struct
{
  uint16_t          handle;         /**< Descriptor Handle. */
  ble_uuid_t        uuid;           /**< Descriptor UUID. */
} ble_gattc_desc_t;


/**@brief Write Parameters. */
typedef struct
{
  uint8_t    write_op;                 /**< Write Operation to be performed, see @ref BLE_GATT_WRITE_OPS. */
  uint8_t    flags;                    /**< Flags, see @ref BLE_GATT_EXEC_WRITE_FLAGS. */
  uint16_t   handle;                   /**< Handle to the attribute to be written. */
  uint16_t   offset;                   /**< Offset in bytes. @note For WRITE_CMD and WRITE_REQ, offset must be 0. */
  uint16_t   len;                      /**< Length of data in bytes. */
  uint8_t   *p_value;                  /**< Pointer to the value data. */
} ble_gattc_write_params_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP. */
typedef struct
{
  uint16_t             count;           /**< Service count. */
  ble_gattc_service_t services[1];      /**< Service data, variable length. */
} ble_gattc_evt_prim_srvc_disc_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_REL_DISC_RSP. */
typedef struct
{
  uint16_t             count;           /**< Include count. */
  ble_gattc_include_t includes[1];      /**< Include data, variable length. */
} ble_gattc_evt_rel_disc_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_CHAR_DISC_RSP. */
typedef struct
{
  uint16_t            count;          /**< Characteristic count. */
  ble_gattc_char_t    chars[1];       /**< Characteristic data, variable length. */
} ble_gattc_evt_char_disc_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_DESC_DISC_RSP. */
typedef struct
{
  uint16_t            count;          /**< Descriptor count. */
  ble_gattc_desc_t    descs[1];       /**< Descriptor data, variable length. */
} ble_gattc_evt_desc_disc_rsp_t;

/**@brief GATT read by UUID handle value pair. */
typedef struct 
{
  uint16_t            handle;          /**< Attribute Handle. */
  uint8_t             *p_value;        /**< Pointer to value, variable length (length available as value_len in @ref ble_gattc_evt_char_val_by_uuid_read_rsp_t). 
                                            Please note that this pointer is absolute to the memory provided by the user when retrieving the event,
                                            so it will effectively point to a location inside the handle_value array. */
} ble_gattc_handle_value_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_CHAR_VAL_BY_UUID_READ_RSP. */
typedef struct
{
  uint16_t                  count;            /**< Handle-Value Pair Count. */
  uint16_t                  value_len;        /**< Length of the value in Handle-Value(s) list. */
  ble_gattc_handle_value_t  handle_value[1];  /**< Handle-Value(s) list, variable length. */
} ble_gattc_evt_char_val_by_uuid_read_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_READ_RSP. */
typedef struct
{
  uint16_t            handle;         /**< Attribute Handle. */
  uint16_t            offset;         /**< Offset of the attribute data. */
  uint16_t            len;            /**< Attribute data length. */
  uint8_t             data[1];        /**< Attribute data, variable length. */
} ble_gattc_evt_read_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_CHAR_VALS_READ_RSP. */
typedef struct
{
  uint16_t            len;            /**< Concatenated Attribute values length. */
  uint8_t             values[1];      /**< Attribute values, variable length. */
} ble_gattc_evt_char_vals_read_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_WRITE_RSP. */
typedef struct
{
  uint16_t            handle;           /**< Attribute Handle. */
  uint8_t             write_op;         /**< Type of write operation, see @ref BLE_GATT_WRITE_OPS. */
  uint16_t            offset;           /**< Data offset. */
  uint16_t            len;              /**< Data length. */
  uint8_t             data[1];          /**< Data, variable length. */
} ble_gattc_evt_write_rsp_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_HVX. */
typedef struct
{
  uint16_t            handle;         /**< Handle to which the HVx operation applies. */
  uint8_t             type;           /**< Indication or Notification, see @ref BLE_GATT_HVX_TYPES. */
  uint16_t            len;            /**< Attribute data length. */
  uint8_t             data[1];        /**< Attribute data, variable length. */
} ble_gattc_evt_hvx_t;

/**@brief Event structure for @ref BLE_GATTC_EVT_TIMEOUT. */
typedef struct
{
  uint8_t          src;                       /**< Timeout source, see @ref BLE_GATT_TIMEOUT_SOURCES. */
} ble_gattc_evt_timeout_t;

/**@brief GATTC event structure. */
typedef struct
{
  uint16_t            conn_handle;                /**< Connection Handle on which event occured. */
  uint16_t            gatt_status;                /**< GATT status code for the operation, see @ref BLE_GATT_STATUS_CODES. */
  uint16_t            error_handle;               /**< In case of error: The handle causing the error. In all other cases @ref BLE_GATT_HANDLE_INVALID. */
  union
  {
    ble_gattc_evt_prim_srvc_disc_rsp_t          prim_srvc_disc_rsp;         /**< Primary Service Discovery Response Event Parameters. */
    ble_gattc_evt_rel_disc_rsp_t                rel_disc_rsp;               /**< Relationship Discovery Response Event Parameters. */
    ble_gattc_evt_char_disc_rsp_t               char_disc_rsp;              /**< Characteristic Discovery Response Event Parameters. */
    ble_gattc_evt_desc_disc_rsp_t               desc_disc_rsp;              /**< Descriptor Discovery Response Event Parameters. */
    ble_gattc_evt_char_val_by_uuid_read_rsp_t   char_val_by_uuid_read_rsp;  /**< Characteristic Value Read by UUID Response Event Parameters. */
    ble_gattc_evt_read_rsp_t                    read_rsp;                   /**< Read Response Event Parameters. */
    ble_gattc_evt_char_vals_read_rsp_t          char_vals_read_rsp;         /**< Characteristic Values Read Response Event Parameters. */
    ble_gattc_evt_write_rsp_t                   write_rsp;                  /**< Write Response Event Parameters. */
    ble_gattc_evt_hvx_t                         hvx;                        /**< Handle Value Notification/Indication Event Parameters. */
    ble_gattc_evt_timeout_t                     timeout;                    /**< Timeout Event Parameters. */
  } params;                                                                 /**< Event Parameters. @note Only valid if @ref gatt_status == @ref BLE_GATT_STATUS_SUCCESS. */
} ble_gattc_evt_t;
/** @} */

/** @addtogroup BLE_GATTC_FUNCTIONS Functions
 * @{ */

/**@brief Initiate or continue a GATT Primary Service Discovery procedure.
 *
 * @details This function initiates or resumes a Primary Service discovery procedure, starting from the supplied handle. 
 *          If the last service has not been reached, this function must be called again with an updated start handle value to continue the search.
 *
 * @note If any of the discovered services have 128-bit UUIDs which are not present in the table provided to ble_vs_uuids_assign, a UUID structure with
 *       type @ref BLE_UUID_TYPE_UNKNOWN will be received in the corresponding event.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] start_handle Handle to start searching from.
 * @param[in] p_srvc_uuid Pointer to the service UUID to be found. If it is NULL, all primary services will be returned.
 *
 * @retval ::NRF_SUCCESS Successfully started or resumed the Primary Service Discovery procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_PRIMARY_SERVICES_DISCOVER, uint32_t, sd_ble_gattc_primary_services_discover(uint16_t conn_handle, uint16_t start_handle, ble_uuid_t const *p_srvc_uuid));


/**@brief Initiate or continue a GATT Relationship Discovery procedure.
 *
 * @details This function initiates or resumes the Find Included Services sub-procedure. If the last included service has not been reached,
 *          this must be called again with an updated handle range to continue the search.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] p_handle_range A pointer to the range of handles of the Service to perform this procedure on.
 *
 * @retval ::NRF_SUCCESS Successfully started or resumed the Relationship Discovery procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_RELATIONSHIPS_DISCOVER, uint32_t, sd_ble_gattc_relationships_discover(uint16_t conn_handle, ble_gattc_handle_range_t const *p_handle_range));


/**@brief Initiate or continue a GATT Characteristic Discovery procedure.
 *
 * @details This function initiates or resumes a Characteristic discovery procedure. If the last Characteristic has not been reached,
 *          this must be called again with an updated handle range to continue the discovery.
 *
 * @note If any of the discovered characteristics have 128-bit UUIDs which are not present in the table provided to ble_vs_uuids_assign, a UUID structure with
 *       type @ref BLE_UUID_TYPE_UNKNOWN will be received in the corresponding event.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] p_handle_range A pointer to the range of handles of the Service to perform this procedure on.
 *
 * @retval ::NRF_SUCCESS Successfully started or resumed the Characteristic Discovery procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_CHARACTERISTICS_DISCOVER, uint32_t, sd_ble_gattc_characteristics_discover(uint16_t conn_handle, ble_gattc_handle_range_t const *p_handle_range));


/**@brief Initiate or continue a GATT Characteristic Descriptor Discovery procedure.
 *
 * @details This function initiates or resumes a Characteristic Descriptor discovery procedure. If the last Descriptor has not been reached,
 *          this must be called again with an updated handle range to continue the discovery.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] p_handle_range A pointer to the range of handles of the Characteristic to perform this procedure on.
 *
 * @retval ::NRF_SUCCESS Successfully started or resumed the Descriptor Discovery procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_DESCRIPTORS_DISCOVER, uint32_t, sd_ble_gattc_descriptors_discover(uint16_t conn_handle, ble_gattc_handle_range_t const *p_handle_range));


/**@brief Initiate or continue a GATT Read using Characteristic UUID procedure.
 *
 * @details This function initiates or resumes a Read using Characteristic UUID procedure. If the last Characteristic has not been reached,
 *          this must be called again with an updated handle range to continue the discovery.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] p_uuid Pointer to a Characteristic value UUID to read.
 * @param[in] p_handle_range A pointer to the range of handles to perform this procedure on.
 *
 * @retval ::NRF_SUCCESS Successfully started or resumed the Read using Characteristic UUID procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_CHAR_VALUE_BY_UUID_READ, uint32_t, sd_ble_gattc_char_value_by_uuid_read(uint16_t conn_handle, ble_uuid_t const *p_uuid, ble_gattc_handle_range_t const *p_handle_range));


/**@brief Initiate or continue a GATT Read (Long) Characteristic or Descriptor procedure.
 *
 * @details This function initiates or resumes a GATT Read (Long) Characteristic or Descriptor procedure. If the Characteristic or Descriptor
 *          to be read is longer than ATT_MTU - 1, this function must be called multiple times with appropriate offset to read the 
 *          complete value.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] handle The handle of the attribute to be read.
 * @param[in] offset Offset into the attribute value to be read.
 *
 * @retval ::NRF_SUCCESS Successfully started or resumed the Read (Long) procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_READ, uint32_t, sd_ble_gattc_read(uint16_t conn_handle, uint16_t handle, uint16_t offset));


/**@brief Initiate a GATT Read Multiple Characteristic Values procedure.
 *
 * @details This function initiates a GATT Read Multiple Characteristic Values procedure. 
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] p_handles A pointer to the handle(s) of the attribute(s) to be read.
 * @param[in] handle_count The number of handles in p_handles.
 *
 * @retval ::NRF_SUCCESS Successfully started the Read Multiple Characteristic Values procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_BUSY Client procedure already in progress.
 */
SVCALL(SD_BLE_GATTC_CHAR_VALUES_READ, uint32_t, sd_ble_gattc_char_values_read(uint16_t conn_handle, uint16_t const *p_handles, uint16_t handle_count));


/**@brief Perform a Write (Characteristic Value or Descriptor, with or without response, signed or not, long or reliable) procedure.
 *
 * @details This function can perform all write procedures described in GATT. 
 *
 * @note    It is important to note that a write without response will <b>consume an application buffer</b>, and will therefore 
 *          generate a @ref BLE_EVT_TX_COMPLETE event when the packet has been transmitted. A write (with response) on the other hand will use the 
 *          standard client internal buffer and thus will only generate a @ref BLE_GATTC_EVT_WRITE_RSP event as soon as the write response 
 *          has been received from the peer. Please see the documentation of @ref sd_ble_tx_buffer_count_get for more details.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] p_write_params A pointer to a write parameters structure.
 *
 * @retval ::NRF_SUCCESS Successfully started the Write procedure.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State.
 * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied.
 * @retval ::NRF_ERROR_DATA_SIZE Invalid data size(s) supplied.
 * @retval ::NRF_ERROR_BUSY Procedure already in progress.
 * @retval ::BLE_ERROR_NO_TX_BUFFERS There are no available buffers left.
 */
SVCALL(SD_BLE_GATTC_WRITE, uint32_t, sd_ble_gattc_write(uint16_t conn_handle, ble_gattc_write_params_t const *p_write_params));


/**@brief Send a Handle Value Confirmation to the GATT Server.
 *
 * @param[in] conn_handle The connection handle identifying the connection to perform this procedure on.
 * @param[in] handle The handle of the attribute in the indication.
 *
 * @retval ::NRF_SUCCESS Successfully queued the Handle Value Confirmation for transmission.
 * @retval ::BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @retval ::NRF_ERROR_INVALID_STATE Invalid Connection State or no Indication pending to be confirmed.
 * @retval ::BLE_ERROR_INVALID_ATTR_HANDLE Invalid attribute handle.
 */
SVCALL(SD_BLE_GATTC_HV_CONFIRM, uint32_t, sd_ble_gattc_hv_confirm(uint16_t conn_handle, uint16_t handle));

/** @} */

#endif /* BLE_GATTC_H__ */

/**
  @}
  @}
*/
