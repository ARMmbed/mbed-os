/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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
 * @defgroup ble_sdk_srv_ln_db Location and Navigation database
 * @{
 * @ingroup ble_sdk_srv
 * @brief Location and Navigation route database
 */

#ifndef BLE_LN_DB__
#define BLE_LN_DB__

#include "ble_lns.h"

/**@brief Function for initializing the route database.
 *
 * @details This call initializes the database holding route records.
 *
 * @return      NRF_SUCCESS on success.
 */
void ble_ln_db_init(void);

/**@brief Function for getting the number of records in the database.
 *
 * @details This call returns the number of records in the database.
 *
 * @return      Number of records in the database.
 */
uint16_t ble_ln_db_num_records_get(void);

/**@brief Function for getting a record from the database.
 *
 * @details This call returns a specified record from the database.
 *
 * @param[in]   record_num    Index of the record to retrieve.
 * @param[out]  p_rec         Pointer to record structure where retrieved record is copied to.
 *
 * @return      NRF_SUCCESS on success.
 */
ret_code_t ble_ln_db_record_get(uint8_t record_num, ble_lns_route_t * p_rec);

/**@brief Function for getting a record name from the database.
 *
 * @details This call returns a specified record name from the database.
 *
 * @param[in]   rec_ndx       Index of the record to retrieve.
 * @param[out]  p_buf         Pointer to array where retrieved record name is copied to.
 *
 * @return      NRF_SUCCESS on success.
 */
ret_code_t ble_ln_db_record_name_get(uint8_t rec_ndx, uint8_t ** p_buf);

/**@brief Function for adding a record at the end of the database.
 *
 * @details This call adds a record as the last record in the database.
 *
 * @param[in]   p_rec   Pointer to record to add to database.
 *
 * @return      NRF_SUCCESS on success.
 */
ret_code_t ble_ln_db_record_add(ble_lns_route_t * p_rec);

/**@brief Function for deleting a database entry.
 *
 * @details This call deletes an record from the database.
 *
 * @param[in]   record_num   Index of record to delete.
 *
 * @return      NRF_SUCCESS on success.
 */
ret_code_t ble_ln_db_record_delete(uint8_t record_num);

#endif // BLE_LN_DB_H__

/** @} */
