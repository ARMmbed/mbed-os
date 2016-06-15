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

/** @file
 *
 * @defgroup ble_sdk_lib_advdata Advertising and Scan Response Data Encoder
 * @{
 * @ingroup ble_sdk_lib
 * @brief Functions for encoding data in the Advertising and Scan Response Data format,
 *        and for passing the data to the stack.
 */

#ifndef BLE_ADVDATA_H__
#define BLE_ADVDATA_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ble.h"
#include "app_util.h"


#define ADV_LENGTH_FIELD_SIZE              1UL                                 /**< Advertising Data and Scan Response format contains 1 octet for the length. */
#define ADV_AD_TYPE_FIELD_SIZE             1UL                                 /**< Advertising Data and Scan Response format contains 1 octet for the AD type. */
#define ADV_AD_DATA_OFFSET                 (ADV_LENGTH_FIELD_SIZE + \
                                            ADV_AD_TYPE_FIELD_SIZE)            /**< Offset for the AD data field of the Advertising Data and Scan Response format. */
#define AD_TYPE_TK_VALUE_DATA_SIZE         (sizeof(ble_advdata_tk_value_t))    /**< Data size (in octets) of the Security Manager TK value AD type. */
#define AD_TYPE_TK_VALUE_SIZE              (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_TK_VALUE_DATA_SIZE)        /**< Size (in octets) of the Security Manager TK value AD type. */
#define AD_TYPE_LE_ROLE_DATA_SIZE          1UL                                 /**< Data size (in octets) of the LE Bluetooth Device Address AD type. */
#define AD_TYPE_LE_ROLE_SIZE               (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_LE_ROLE_DATA_SIZE)         /**< Size (in octets) of the LE Bluetooth Device Address AD type. */
#define AD_TYPE_BLE_DEVICE_ADDR_TYPE_SIZE  1UL                                 /**< Data size (in octets) of the Address type of the LE Bluetooth Device Address AD type. */
#define AD_TYPE_BLE_DEVICE_ADDR_DATA_SIZE  (BLE_GAP_ADDR_LEN + \
                                            AD_TYPE_BLE_DEVICE_ADDR_TYPE_SIZE) /**< Data size (in octets) of the LE Bluetooth Device Address AD type. */
#define AD_TYPE_BLE_DEVICE_ADDR_SIZE       (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_BLE_DEVICE_ADDR_DATA_SIZE) /**< Size (in octets) of the LE Bluetooth Device Address AD type. */
#define AD_TYPE_APPEARANCE_DATA_SIZE       2UL                                 /**< Data size (in octets) of the Appearance AD type. */
#define AD_TYPE_APPEARANCE_SIZE            (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_APPEARANCE_DATA_SIZE)      /**< Size (in octets) of the Appearance AD type. */
#define AD_TYPE_FLAGS_DATA_SIZE            1UL                                 /**< Data size (in octets) of the Flags AD type. */
#define AD_TYPE_FLAGS_SIZE                 (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_FLAGS_DATA_SIZE)           /**< Size (in octets) of the Flags AD type. */
#define AD_TYPE_TX_POWER_LEVEL_DATA_SIZE   1UL                                 /**< Data size (in octets) of the TX Power Level AD type. */
#define AD_TYPE_TX_POWER_LEVEL_SIZE        (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_TX_POWER_LEVEL_DATA_SIZE)  /**< Size (in octets) of the TX Power Level AD type. */
#define AD_TYPE_CONN_INT_DATA_SIZE         4UL                                 /**< Data size (in octets) of the Slave Connection Interval Range AD type. */
#define AD_TYPE_CONN_INT_SIZE              (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_CONN_INT_DATA_SIZE)        /**< Data size (in octets) of the Slave Connection Interval Range AD type. */
#define AD_TYPE_MANUF_SPEC_DATA_ID_SIZE    2UL                                 /**< Size (in octets) of the Company Identifier Code, which is a part of the Manufacturer Specific Data AD type. */
#define AD_TYPE_SERV_DATA_16BIT_UUID_SIZE  2UL                                 /**< Size (in octets) of the 16-bit UUID, which is a part of the Service Data AD type. */
#define AD_TYPE_OOB_FLAGS_DATA_SIZE        1UL                                 /**< Data size (in octets) of the Security Manager OOB Flags AD type. */
#define AD_TYPE_OOB_FLAGS_SIZE             (ADV_AD_DATA_OFFSET + \
                                            AD_TYPE_OOB_FLAGS_DATA_SIZE)       /**< Size (in octets) of the Security Manager OOB Flags AD type. */

#define AD_TYPE_SEC_MGR_OOB_FLAG_SET                   1U                      /**< Security Manager OOB Flag set. Flag selection is done using _POS defines */
#define AD_TYPE_SEC_MGR_OOB_FLAG_CLEAR                 0U                      /**< Security Manager OOB Flag clear. Flag selection is done using _POS defines */
#define AD_TYPE_SEC_MGR_OOB_FLAG_OOB_DATA_PRESENT_POS  0UL                     /**< Security Manager OOB Data Present Flag position. */
#define AD_TYPE_SEC_MGR_OOB_FLAG_OOB_LE_SUPPORTED_POS  1UL                     /**< Security Manager OOB Low Energy Supported Flag position. */
#define AD_TYPE_SEC_MGR_OOB_FLAG_SIM_LE_AND_EP_POS     2UL                     /**< Security Manager OOB Simultaneous LE and BR/EDR to Same Device Capable Flag position. */
#define AD_TYPE_SEC_MGR_OOB_ADDRESS_TYPE_PUBLIC        0UL                     /**< Security Manager OOB Public Address type. */
#define AD_TYPE_SEC_MGR_OOB_ADDRESS_TYPE_RANDOM        1UL                     /**< Security Manager OOB Random Address type. */
#define AD_TYPE_SEC_MGR_OOB_FLAG_ADDRESS_TYPE_POS      3UL                     /**< Security Manager OOB Address type Flag (0 = Public Address, 1 = Random Address) position. */


/**@brief Security Manager TK value. */
typedef struct
{
  uint8_t tk[BLE_GAP_SEC_KEY_LEN];   /**< Array containing TK value. */
} ble_advdata_tk_value_t;

/**@brief Advertising data LE Role types. This enumeration contains the options available for the LE role inside
 *        the advertising data. */
typedef enum
{
    BLE_ADVDATA_ROLE_NOT_PRESENT = 0,                                 /**< LE Role AD structure not present. */
    BLE_ADVDATA_ROLE_ONLY_PERIPH,                                     /**< Only Peripheral Role supported. */
    BLE_ADVDATA_ROLE_ONLY_CENTRAL,                                    /**< Only Central Role supported. */
    BLE_ADVDATA_ROLE_BOTH_PERIPH_PREFERRED,                           /**< Peripheral and Central Role supported. Peripheral Role preferred for connection establishment. */
    BLE_ADVDATA_ROLE_BOTH_CENTRAL_PREFERRED                           /**< Peripheral and Central Role supported. Central Role preferred for connection establishment */
} ble_advdata_le_role_t;

/**@brief Advertising data name type. This enumeration contains the options available for the device name inside
 *        the advertising data. */
typedef enum
{
    BLE_ADVDATA_NO_NAME,                                              /**< Include no device name in advertising data. */
    BLE_ADVDATA_SHORT_NAME,                                           /**< Include short device name in advertising data. */
    BLE_ADVDATA_FULL_NAME                                             /**< Include full device name in advertising data. */
} ble_advdata_name_type_t;

/**@brief UUID list type. */
typedef struct
{
    uint16_t                     uuid_cnt;                            /**< Number of UUID entries. */
    ble_uuid_t *                 p_uuids;                             /**< Pointer to UUID array entries. */
} ble_advdata_uuid_list_t;

/**@brief Connection interval range structure. */
typedef struct
{
    uint16_t                     min_conn_interval;                   /**< Minimum connection interval, in units of 1.25 ms, range 6 to 3200 (7.5 ms to 4 s). */
    uint16_t                     max_conn_interval;                   /**< Maximum connection interval, in units of 1.25 ms, range 6 to 3200 (7.5 ms to 4 s). The value 0xFFFF indicates no specific maximum. */
} ble_advdata_conn_int_t;

/**@brief Manufacturer specific data structure. */
typedef struct
{
    uint16_t                     company_identifier;                  /**< Company identifier code. */
    uint8_array_t                data;                                /**< Additional manufacturer specific data. */
} ble_advdata_manuf_data_t;

/**@brief Service data structure. */
typedef struct
{
    uint16_t                     service_uuid;                        /**< Service UUID. */
    uint8_array_t                data;                                /**< Additional service data. */
} ble_advdata_service_data_t;

/**@brief Advertising data structure. This structure contains all options and data needed for encoding and
 *        setting the advertising data. */
typedef struct
{
    ble_advdata_name_type_t      name_type;                           /**< Type of device name. */
    uint8_t                      short_name_len;                      /**< Length of short device name (if short type is specified). */
    bool                         include_appearance;                  /**< Determines if Appearance shall be included. */
    uint8_t                      flags;                               /**< Advertising data Flags field. */
    int8_t *                     p_tx_power_level;                    /**< TX Power Level field. */
    ble_advdata_uuid_list_t      uuids_more_available;                /**< List of UUIDs in the 'More Available' list. */
    ble_advdata_uuid_list_t      uuids_complete;                      /**< List of UUIDs in the 'Complete' list. */
    ble_advdata_uuid_list_t      uuids_solicited;                     /**< List of solicited UUIDs. */
    ble_advdata_conn_int_t *     p_slave_conn_int;                    /**< Slave Connection Interval Range. */
    ble_advdata_manuf_data_t *   p_manuf_specific_data;               /**< Manufacturer specific data. */
    ble_advdata_service_data_t * p_service_data_array;                /**< Array of Service data structures. */
    uint8_t                      service_data_count;                  /**< Number of Service data structures. */
    bool                         include_ble_device_addr;             /**< Determines if LE Bluetooth Device Address shall be included. */
    ble_advdata_le_role_t        le_role;                             /**< LE Role field. Included when different from @ref BLE_ADVDATA_ROLE_NOT_PRESENT. @warning This field can be used only for NFC. For BLE advertising, set it to NULL. */
    ble_advdata_tk_value_t *     p_tk_value;                          /**< Security Manager TK value field. Included when different from NULL. @warning This field can be used only for NFC. For BLE advertising, set it to NULL.*/
    uint8_t *                    p_sec_mgr_oob_flags;                 /**< Security Manager Out Of Band Flags field. Included when different from NULL. @warning This field can be used only for NFC. For BLE advertising, set it to NULL.*/
} ble_advdata_t;

/**@brief Function for encoding data in the Advertising and Scan Response data format
 *        (AD structures).
 *
 * @details This function encodes data into the Advertising and Scan Response data format
 *          (AD structures) based on the selections in the supplied structures. This function can be used to
 *          create a payload of Advertising packet or Scan Response packet, or a payload of NFC
 *          message intended for initiating the Out-of-Band pairing.
 *
 * @param[in]      p_advdata       Pointer to the structure for specifying the content of encoded data.
 * @param[out]     p_encoded_data  Pointer to the buffer where encoded data will be returned.
 * @param[in,out]  p_len           \c in: Size of \p p_encoded_data buffer.
 *                                 \c out: Length of encoded data.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_INVALID_PARAM If the operation failed because a wrong parameter was provided in \p p_advdata.
 * @retval NRF_ERROR_DATA_SIZE     If the operation failed because not all the requested data could fit into the
 *                                 provided buffer or some encoded AD structure is too long and its
 *                                 length cannot be encoded with one octet.
 *
 * @warning This API may override the application's request to use the long name and use a short name
 * instead. This truncation will occur in case the long name does not fit the provided buffer size.
 * The application can specify a preferred short name length if truncation is required.
 * For example, if the complete device name is ABCD_HRMonitor, the application can specify the short name
 * length to be 8, so that the short device name appears as ABCD_HRM instead of ABCD_HRMo or ABCD_HRMoni
 * if the available size for the short name is 9 or 12 respectively, to have a more appropriate short name.
 * However, it should be noted that this is just a preference that the application can specify, and
 * if the preference is too large to fit in the provided buffer, the name can be truncated further.
 */
uint32_t adv_data_encode(ble_advdata_t const * const p_advdata,
                         uint8_t             * const p_encoded_data,
                         uint16_t            * const p_len);

/**@brief Function for encoding and setting the advertising data and/or scan response data.
 *
 * @details This function encodes advertising data and/or scan response data based on the selections
 *          in the supplied structures, and passes the encoded data to the stack.
 *
 * @param[in]   p_advdata   Structure for specifying the content of the advertising data.
 *                          Set to NULL if advertising data is not to be set.
 * @param[in]   p_srdata    Structure for specifying the content of the scan response data.
 *                          Set to NULL if scan response data is not to be set.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_INVALID_PARAM If the operation failed because a wrong parameter was provided in \p p_advdata.
 * @retval NRF_ERROR_DATA_SIZE     If the operation failed because not all the requested data could fit into the
 *                                 advertising packet. The maximum size of the advertisement packet
 *                                 is @ref BLE_GAP_ADV_MAX_SIZE.
 *
 * @warning This API may override the application's request to use the long name and use a short name
 * instead. This truncation will occur in case the long name does not fit the provided buffer size.
 * The application can specify a preferred short name length if truncation is required.
 * For example, if the complete device name is ABCD_HRMonitor, the application can specify the short name
 * length to be 8, so that the short device name appears as ABCD_HRM instead of ABCD_HRMo or ABCD_HRMoni
 * if the available size for the short name is 9 or 12 respectively, to have a more appropriate short name.
 * However, it should be noted that this is just a preference that the application can specify, and
 * if the preference is too large to fit in the provided buffer, the name can be truncated further.
 */
uint32_t ble_advdata_set(const ble_advdata_t * p_advdata, const ble_advdata_t * p_srdata);

#endif // BLE_ADVDATA_H__

/** @} */
