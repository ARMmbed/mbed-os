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

/** @file
 *
 * @defgroup ble_sdk_srv_gls_db Glucose Database Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Glucose Service module.
 *
 * @details This module implements at database of stored glucose measurement values.
 *
 * @note Attention! 
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile 
 *  qualification listings, These APIs must not be modified. However, the corresponding
 *  functions' implementations can be modified.
 */

#ifndef BLE_GLS_DB_H__
#define BLE_GLS_DB_H__

#include <stdint.h>
#include "ble_gls.h"

#define BLE_GLS_DB_MAX_RECORDS      20

/**@brief Function for initializing the glucose record database.
 *
 * @details This call initializes the database holding glucose records.
 *
 * @return      NRF_SUCCESS on success. 
 */
uint32_t ble_gls_db_init(void);

/**@brief Function for getting the number of records in the database.
 *
 * @details This call returns the number of records in the database.
 *
 * @return      Number of records in the database.
 */
uint16_t ble_gls_db_num_records_get(void);

/**@brief Function for getting a record from the database.
 *
 * @details This call returns a specified record from the database.
 *
 * @param[in]   record_num    Index of the record to retrieve.
 * @param[out]  p_rec         Pointer to record structure where retrieved record is copied to.
 * 
 * @return      NRF_SUCCESS on success.
 */
uint32_t ble_gls_db_record_get(uint8_t record_num, ble_gls_rec_t * p_rec);

/**@brief Function for adding a record at the end of the database.
 *
 * @details This call adds a record as the last record in the database.
 *
 * @param[in]   p_rec   Pointer to record to add to database.
 * 
 * @return      NRF_SUCCESS on success.
 */
uint32_t ble_gls_db_record_add(ble_gls_rec_t * p_rec);

/**@brief Function for deleting a database entry.
 *
 * @details This call deletes an record from the database.
 *
 * @param[in]   record_num   Index of record to delete.
 * 
 * @return      NRF_SUCCESS on success.
 */
uint32_t ble_gls_db_record_delete(uint8_t record_num);

#endif // BLE_GLS_DB_H__

/** @} */
