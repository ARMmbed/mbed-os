/* Copyright (c) 2011 Nordic Semiconductor. All Rights Reserved.
*
* The information contained herein is property of Nordic Semiconductor ASA.
* Terms and conditions of usage are described in detail in NORDIC
* SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
*
* Licensees are granted free, non-transferable use of the information. NO
* WARRANTY of ANY KIND is provided. This heading must NOT be removed from
* the file.
*/

/* Attention! 
*  To maintain compliance with Nordic Semiconductor ASA’s Bluetooth profile 
*  qualification listings, this section of source code must not be modified.
*/

/** @file
 * @brief Contains definition of ble_date_time structure.
 */

/** @file
 *
 * @defgroup ble_sdk_srv_date_time BLE Date Time characteristic type
 * @{
 * @ingroup ble_sdk_lib
 * @brief Definition of ble_date_time_t type.
 */

#ifndef BLE_DATE_TIME_H__
#define BLE_DATE_TIME_H__

#include <stdint.h>

/**@brief Date and Time structure. */
typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
} ble_date_time_t;

static __INLINE uint8_t ble_date_time_encode(const ble_date_time_t * p_date_time,
                                             uint8_t *               p_encoded_data)
{
    uint8_t len = uint16_encode(p_date_time->year, p_encoded_data);
    
    p_encoded_data[len++] = p_date_time->month;
    p_encoded_data[len++] = p_date_time->day;
    p_encoded_data[len++] = p_date_time->hours;
    p_encoded_data[len++] = p_date_time->minutes;
    p_encoded_data[len++] = p_date_time->seconds;
    
    return len;
}

static __INLINE uint8_t ble_date_time_decode(ble_date_time_t * p_date_time,
                                             const uint8_t *   p_encoded_data)
{
    uint8_t len = sizeof(uint16_t);
    
    p_date_time->year    = uint16_decode(p_encoded_data);
    p_date_time->month   = p_encoded_data[len++];
    p_date_time->day     = p_encoded_data[len++]; 
    p_date_time->hours   = p_encoded_data[len++];
    p_date_time->minutes = p_encoded_data[len++];
    p_date_time->seconds = p_encoded_data[len++];
    
    return len;
}

#endif // BLE_DATE_TIME_H__

/** @} */
