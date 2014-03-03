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
 * @defgroup ble_sdk_lib_advdata Advertising Data Encoder
 * @{
 * @ingroup ble_sdk_lib
 * @brief Function for encoding the advertising data and/or scan response data, and passing them to
 *        the stack.
 */

#ifndef BLE_ADVDATA_H__
#define BLE_ADVDATA_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "nordic_global.h"
#include "ble.h"
#include "app_util.h"

/**@brief Advertising data name type. This contains the options available for the device name inside
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
    uint16_t                     min_conn_interval;                   /**< Minimum Connection Interval, in units of 1.25ms, range 6 to 3200 (i.e. 7.5ms to 4s). */
    uint16_t                     max_conn_interval;                   /**< Maximum Connection Interval, in units of 1.25ms, range 6 to 3200 (i.e. 7.5ms to 4s). Value of 0xFFFF indicates no specific maximum. */
} ble_advdata_conn_int_t;

/**@brief Manufacturer specific data structure. */
typedef struct
{
    uint16_t                     company_identifier;                  /**< Company Identifier Code. */
    uint8_array_t                data;                                /**< Additional manufacturer specific data. */
} ble_advdata_manuf_data_t;

/**@brief Service data structure. */
typedef struct
{
    uint16_t                     service_uuid;                        /**< Service UUID. */
    uint8_array_t                data;                                /**< Additional service data. */
} ble_advdata_service_data_t;

/**@brief Advertising data structure. This contains all options and data needed for encoding and
 *        setting the advertising data. */
typedef struct
{
    ble_advdata_name_type_t      name_type;                           /**< Type of device name. */
    uint8_t                      short_name_len;                      /**< Length of short device name (if short type is specified). */
    bool                         include_appearance;                  /**< Determines if Appearance shall be included. */
    uint8_array_t                flags;                               /**< Advertising data Flags field. */
    int8_t *                     p_tx_power_level;                    /**< TX Power Level field. */
    ble_advdata_uuid_list_t      uuids_more_available;                /**< List of UUIDs in the 'More Available' list. */
    ble_advdata_uuid_list_t      uuids_complete;                      /**< List of UUIDs in the 'Complete' list. */
    ble_advdata_uuid_list_t      uuids_solicited;                     /**< List of solcited UUIDs. */
    ble_advdata_conn_int_t *     p_slave_conn_int;                    /**< Slave Connection Interval Range. */
    ble_advdata_manuf_data_t *   p_manuf_specific_data;               /**< Manufacturer specific data. */
    ble_advdata_service_data_t * p_service_data_array;                /**< Array of Service data structures. */
    uint8_t                      service_data_count;                  /**< Number of Service data structures. */
} ble_advdata_t;

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
 * @return      NRF_SUCCESS on success, NRF_ERROR_DATA_SIZE if not all the requested data could fit
 *              into the advertising packet. The maximum size of the advertisement packet is @ref
 *              BLE_GAP_ADV_MAX_SIZE.
 *
 * @warning This API may override application's request to use the long name and use a short name
 * instead. This truncation will occur in case the long name does not fit advertisement data size.
 * Application is permitted to specify a preferred short name length in case truncation is required.
 * For example, if the complete device name is ABCD_HRMonitor, application can specify short name 
 * length to 8 such that short device name appears as ABCD_HRM instead of ABCD_HRMo or ABCD_HRMoni
 * etc if available size for short name is 9 or 12 respectively to have more apporpriate short name.
 * However, it should be noted that this is just a preference that application can specify and
 * if the preference too large to fit in Advertisement Data, this can be further truncated. 
 */
uint32_t ble_advdata_set(const ble_advdata_t * p_advdata, const ble_advdata_t * p_srdata);

#endif // BLE_ADVDATA_H__

/** @} */
