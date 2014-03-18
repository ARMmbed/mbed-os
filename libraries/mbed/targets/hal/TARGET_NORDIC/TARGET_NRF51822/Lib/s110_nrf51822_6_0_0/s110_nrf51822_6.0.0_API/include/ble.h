/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
/**
  @addtogroup BLE_COMMON BLE SoftDevice Common
  @{
  @defgroup ble_api Events, type definitions and API calls
  @{

  @brief Module independent events, type definitions and API calls for the S110 SoftDevice.

 */

#ifndef BLE_H__
#define BLE_H__

#include "ble_ranges.h"
#include "ble_types.h"
#include "ble_gap.h"
#include "ble_l2cap.h"
#include "ble_gatt.h"
#include "ble_gattc.h"
#include "ble_gatts.h"

/**
 * @brief Common API SVC numbers.
 */
enum BLE_COMMON_SVCS
{
  SD_BLE_EVT_GET  = BLE_SVC_BASE,       /**< Get an event from the pending events queue. */
  SD_BLE_TX_BUFFER_COUNT_GET,           /**< Get the total number of available application transmission buffers from the stack. */
  SD_BLE_UUID_VS_ADD,                   /**< Add a Vendor Specific UUID. */
  SD_BLE_UUID_DECODE,                   /**< Decode UUID bytes. */
  SD_BLE_UUID_ENCODE,                   /**< Encode UUID bytes. */
  SD_BLE_VERSION_GET,                   /**< Get the local version information (company id, LMP Version, LMP Subversion). */
  SD_BLE_USER_MEM_REPLY,                /**< User Memory Reply. */
};

/** @brief  Required pointer alignment for BLE Events.
*/
#define BLE_EVTS_PTR_ALIGNMENT    4

/** @defgroup BLE_USER_MEM_TYPES User Memory Types
 * @{ */
#define BLE_USER_MEM_TYPE_INVALID               0x00  /**< Invalid User Memory Types. */
#define BLE_USER_MEM_TYPE_GATTS_QUEUED_WRITES   0x01  /**< User Memory for GATTS queued writes. */
/** @} */

/** @brief  Maximum number of Vendor Specific UUIDs.
*/
#define BLE_UUID_VS_MAX_COUNT     10

/**
 * @brief BLE Module Independent Event IDs.
 */
enum BLE_COMMON_EVTS
{
  BLE_EVT_TX_COMPLETE  = BLE_EVT_BASE,  /**< Transmission Complete. */
  BLE_EVT_USER_MEM_REQUEST,             /**< User Memory request. */
  BLE_EVT_USER_MEM_RELEASE              /**< User Memory release. */
};

/**@brief User Memory Block. */
typedef struct
{
  uint8_t*          p_mem;      /**< Pointer to the start of the user memory block. */
  uint16_t          len;        /**< Length in bytes of the user memory block. */
} ble_user_mem_block_t;

/**
 * @brief TX complete event.
 */
typedef struct
{
  uint8_t count;                        /**< Number of packets transmitted. */
} ble_evt_tx_complete_t;

/**@brief Event structure for BLE_EVT_USER_MEM_REQUEST. */
typedef struct
{
  uint8_t                     type;     /**< User memory type, see @ref BLE_USER_MEM_TYPES. */
} ble_evt_user_mem_request_t;

/**@brief Event structure for BLE_EVT_USER_MEM_RELEASE. */
typedef struct
{
  uint8_t                     type;       /**< User memory type, see @ref BLE_USER_MEM_TYPES. */
  ble_user_mem_block_t        mem_block;  /**< User memory block */
} ble_evt_user_mem_release_t;


/**@brief Event structure for events not associated with a specific function module. */
typedef struct
{
  uint16_t conn_handle;                 /**< Connection Handle on which this event occured. */
  union
  {
    ble_evt_tx_complete_t           tx_complete;        /**< Transmission Complete. */
    ble_evt_user_mem_request_t      user_mem_request;   /**< User Memory Request Event Parameters. */
    ble_evt_user_mem_release_t      user_mem_release;   /**< User Memory Release Event Parameters. */
  } params;
} ble_common_evt_t;

/**@brief BLE Event header. */
typedef struct
{
  uint16_t evt_id;                      /**< Value from a BLE_<module>_EVT series. */
  uint16_t evt_len;                     /**< Length in octets excluding this header. */
} ble_evt_hdr_t;

/**@brief Common BLE Event type, wrapping the module specific event reports. */
typedef struct
{
  ble_evt_hdr_t header;                 /**< Event header. */
  union
  {
    ble_common_evt_t  common_evt;         /**< Common Event, evt_id in BLE_EVT_* series. */
    ble_gap_evt_t     gap_evt;            /**< GAP originated event, evt_id in BLE_GAP_EVT_* series. */
    ble_l2cap_evt_t   l2cap_evt;          /**< L2CAP originated event, evt_id in BLE_L2CAP_EVT* series. */
    ble_gattc_evt_t   gattc_evt;          /**< GATT client originated event, evt_id in BLE_GATTC_EVT* series. */
    ble_gatts_evt_t   gatts_evt;          /**< GATT server originated event, evt_id in BLE_GATTS_EVT* series. */
  } evt;
} ble_evt_t;


/**
 * @brief Version Information.
 */
typedef struct
{
  uint8_t   version_number;             /**< LMP Version number for BT 4.0 spec is 6 (https://www.bluetooth.org/technical/assignednumbers/link_layer.htm). */
  uint16_t  company_id;                 /**< Company ID, Nordic Semiconductor's company ID is 89 (0x0059) (https://www.bluetooth.org/apps/content/Default.aspx?doc_id=49708). */
  uint16_t  subversion_number;          /**< LMP Sub Version number corresponds to the SoftDevice Config ID. */
} ble_version_t;


/**@brief Get an event from the pending events queue.
 *
 * @param[in] p_dest Pointer to buffer to be filled in with an event, or NULL to retrieve the event length. This buffer <b>must be 4-byte aligned in memory</b>.
 * @param[in, out] p_len Pointer the length of the buffer, on return it is filled with the event length.
 *
 * @details This call allows the application to pull a BLE event from the BLE stack. The application is signalled that an event is 
 * available from the BLE Stack by the triggering of the SD_EVT_IRQn interrupt (mapped to IRQ 22).
 * The application is free to choose whether to call this function from thread mode (main context) or directly from the Interrupt Service Routine
 * that maps to SD_EVT_IRQn. In any case however, and because the BLE stack runs at a higher priority than the application, this function should be called
 * in a loop (until @ref NRF_ERROR_NOT_FOUND is returned) every time SD_EVT_IRQn is raised to ensure that all available events are pulled from the stack. 
 * Failure to do so could potentially leave events in the internal queue without the application being aware of this fact.
 * Sizing the p_dest buffer is equally important, since the application needs to provide all the memory necessary for the event to be copied into
 * application memory. If the buffer provided is not large enough to fit the entire contents of the event, @ref NRF_ERROR_DATA_SIZE will be returned
 * and the application can then call again with a larger buffer size.
 * Please note that because of the variable length nature of some events, sizeof(ble_evt_t) will not always be large enough to fit certain events, 
 * and so it is the application's responsability to provide an amount of memory large enough so that the relevant event is copied in full.
 * The application may "peek" the event length by providing p_dest as a NULL pointer and inspecting the value of *p_len upon return.
 *
 * @note The pointer supplied must be aligned to the extend defined by @ref BLE_EVTS_PTR_ALIGNMENT
 *
 * @return @ref NRF_SUCCESS Event pulled and stored into the supplied buffer.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid or not sufficiently aligned pointer supplied.
 * @return @ref NRF_ERROR_NOT_FOUND No events ready to be pulled.
 * @return @ref NRF_ERROR_DATA_SIZE Event ready but could not fit into the supplied buffer.
 */
SVCALL(SD_BLE_EVT_GET, uint32_t, sd_ble_evt_get(uint8_t* p_dest, uint16_t *p_len));


/**@brief Get the total number of available application transmission buffers in the BLE stack.
 *
 * @details This call allows the application to obtain the total number of
 *          transmission buffers available for application data. Please note that
 *          this does not give the number of free buffers, but rather the total amount of them.
 *          The application has two options to handle its own application transmission buffers:
 *          - Use a simple arithmetic calculation: at boot time the application should use this function
 *          to find out the total amount of buffers available to it and store it in a variable.
 *          Every time a packet that consumes an application buffer is sent using any of the 
 *          exposed functions in this BLE API, the application should decrement that variable.
 *          Conversely, whenever a @ref BLE_EVT_TX_COMPLETE event is received by the application
 *          it should retrieve the count field in such event and add that number to the same
 *          variable storing the number of available packets.
 *          This mechanism allows the application to be aware at any time of the number of
 *          application packets available in the BLE stack's internal buffers, and therefore
 *          it can know with certainty whether it is possible to send more data or it has to
 *          wait for a @ref BLE_EVT_TX_COMPLETE event before it proceeds.
 *          - Choose to simply not keep track of available buffers at all, and instead handle the 
 *          @ref BLE_ERROR_NO_TX_BUFFERS error by queueing the packet to be transmitted and 
 *          try again as soon as a @ref BLE_EVT_TX_COMPLETE event arrives.
 *
 *          The API functions that <b>may</b> consume an application buffer depending on 
 *          the parameters supplied to them can be found below:
 *
 *          - @ref sd_ble_gattc_write (write witout response only)
 *          - @ref sd_ble_gatts_hvx (notifications only)
 *          - @ref sd_ble_l2cap_tx (all packets)
 *
 * @param[out] p_count Pointer to a uint8_t which will contain the number of application transmission buffers upon
 *                     successful return.
 *
 * @return @ref NRF_SUCCESS Number of application transmission buffers retrieved successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 */
SVCALL(SD_BLE_TX_BUFFER_COUNT_GET, uint32_t, sd_ble_tx_buffer_count_get(uint8_t* p_count));


/**@brief Add a Vendor Specific UUID.
 *
 * @details This call enables the application to add a vendor specific UUID to the BLE stack's table,
 *          for later use all other modules and APIs. This then allows the application to use the shorter,
 *          24-bit @ref ble_uuid_t format when dealing with both 16-bit and 128-bit UUIDs without having to
 *          check for lengths and having split code paths. The way that this is accomplished is by extending the 
 *          grouping mechanism that the Bluetooth SIG standard base UUID uses for all other 128-bit UUIDs. The 
 *          type field in the @ref ble_uuid_t structure is an index (relative to @ref BLE_UUID_TYPE_VENDOR_BEGIN) 
 *          to the table populated by multiple calls to this function, and the uuid field in the same structure 
 *          contains the 2 bytes at indices 12 and 13. The number of possible 128-bit UUIDs available to the 
 *          application is therefore the number of Vendor Specific UUIDs added with the help of this function times 65536, 
 *          although restricted to modifying bytes 12 and 13 for each of the entries in the supplied array.
 *
 * @note Bytes 12 and 13 of the provided UUID will not be used internally, since those are always replaced by 
 * the 16-bit uuid field in @ref ble_uuid_t.
 *
 *
 * @param[in]  p_vs_uuid    Pointer to a 16-octet (128-bit) little endian Vendor Specific UUID disregarding
 *                          bytes 12 and 13.
 * @param[out] p_uuid_type  Pointer where the type field in @ref ble_uuid_t corresponding to this UUID will be stored.
 *
 * @return @ref NRF_SUCCESS Successfully added the Vendor Specific UUID.
 * @return @ref NRF_ERROR_INVALID_ADDR If p_vs_uuid or p_uuid_type is NULL or invalid.
 * @return @ref NRF_ERROR_NO_MEM If there are no more free slots for VS UUIDs.
 * @return @ref NRF_ERROR_FORBIDDEN If p_vs_uuid has already been added to the VS UUID table.
 */
SVCALL(SD_BLE_UUID_VS_ADD, uint32_t, sd_ble_uuid_vs_add(ble_uuid128_t const * const p_vs_uuid, uint8_t * const p_uuid_type));


/** @brief Decode little endian raw UUID bytes (16-bit or 128-bit) into a 24 bit @ref ble_uuid_t structure.
 * 
 * @details The raw UUID bytes excluding bytes 12 and 13 (i.e. bytes 0-11 and 14-15) of p_uuid_le are compared 
 * to the corresponding ones in each entry of the table of vendor specific UUIDs pouplated with @ref sd_ble_uuid_vs_add 
 * to look for a match. If there is such a match, bytes 12 and 13 are returned as p_uuid->uuid and the index 
 * relative to @ref BLE_UUID_TYPE_VENDOR_BEGIN as p_uuid->type. 
 *
 * @note If the UUID length supplied is 2, then the type set by this call will always be @ref BLE_UUID_TYPE_BLE.
 *
 * @param[in]      uuid_le_len Length in bytes of the buffer pointed to by p_uuid_le (must be 2 or 16 bytes).
 * @param[in]      p_uuid_le   Pointer pointing to little endian raw UUID bytes.
 * @param[in,out]  p_uuid      Pointer to a @ref ble_uuid_t structure to be filled in.
 *
 * @return @ref NRF_SUCCESS Successfully decoded into the @ref ble_uuid_t structure.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_LENGTH Invalid UUID length.
 * @return @ref NRF_ERROR_NOT_FOUND For a 128-bit UUID, no match in the populated table of UUIDs.
 */                                                 
SVCALL(SD_BLE_UUID_DECODE, uint32_t, sd_ble_uuid_decode(uint8_t uuid_le_len, uint8_t const * const p_uuid_le, ble_uuid_t * const p_uuid));


/** @brief Encode a @ref ble_uuid_t structure into little endian raw UUID bytes (16-bit or 128-bit).
 *
 * @note The pointer to the destination buffer p_uuid_le may be NULL, in which case only the validitiy and size of p_uuid is computed.
 *
 * @param[in]      p_uuid        Pointer to a @ref ble_uuid_t structure that will be encoded into bytes.
 * @param[out]     p_uuid_le_len Pointer to a uint8_t that will be filled with the encoded length (2 or 16 bytes).
 * @param[out]     p_uuid_le     Pointer to a buffer where the little endian raw UUID bytes (2 or 16) will be stored.
 *
 * @return @ref NRF_SUCCESS Successfully encoded into the buffer.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_INVALID_PARAM Invalid UUID type.
 */
SVCALL(SD_BLE_UUID_ENCODE, uint32_t, sd_ble_uuid_encode(ble_uuid_t const * const p_uuid, uint8_t * const  p_uuid_le_len, uint8_t * const p_uuid_le));


/**@brief Get Version Information.
 *
 * @details This call allows the application to get the BLE stack version information.
 *
 * @param[in] p_version Pointer to ble_version_t structure to be filled in.
 *
 * @return @ref NRF_SUCCESS  Version information stored successfully.
 * @return @ref NRF_ERROR_INVALID_ADDR Invalid pointer supplied.
 * @return @ref NRF_ERROR_BUSY The stack is busy (typically doing a locally-initiated disconnection procedure).
 */
SVCALL(SD_BLE_VERSION_GET, uint32_t, sd_ble_version_get(ble_version_t * p_version));


/**@brief Provide a user memory block.
 *
 * @note This call can only be used as a response to a @ref BLE_EVT_USER_MEM_REQUEST event issued to the application.
 *
 * @param[in] conn_handle                 Connection handle.
 * @param[in] p_block                     Pointer to a user memory block structure.
 *
 * @return @ref NRF_SUCCESS               Successfully queued a response to the peer.
 * @return @ref BLE_ERROR_INVALID_CONN_HANDLE Invalid Connection Handle.
 * @return @ref NRF_ERROR_INVALID_STATE   No execute write request pending.
 */
SVCALL(SD_BLE_USER_MEM_REPLY, uint32_t, sd_ble_user_mem_reply(uint16_t conn_handle, ble_user_mem_block_t *p_block));

#endif /* BLE_H__ */

/**
  @}
  @}
*/
