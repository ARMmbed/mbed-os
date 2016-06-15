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
 * @defgroup ble_sdk_lib_racp Record Access Control Point
 * @{
 * @ingroup ble_sdk_lib
 * @brief Record Access Control Point library.
 */

#ifndef BLE_RACP_H__
#define BLE_RACP_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_types.h"
#include "ble.h"

/**@brief Record Access Control Point opcodes. */
#define RACP_OPCODE_RESERVED                0       /**< Record Access Control Point opcode - Reserved for future use. */
#define RACP_OPCODE_REPORT_RECS             1       /**< Record Access Control Point opcode - Report stored records. */
#define RACP_OPCODE_DELETE_RECS             2       /**< Record Access Control Point opcode - Delete stored records. */
#define RACP_OPCODE_ABORT_OPERATION         3       /**< Record Access Control Point opcode - Abort operation. */
#define RACP_OPCODE_REPORT_NUM_RECS         4       /**< Record Access Control Point opcode - Report number of stored records. */
#define RACP_OPCODE_NUM_RECS_RESPONSE       5       /**< Record Access Control Point opcode - Number of stored records response. */
#define RACP_OPCODE_RESPONSE_CODE           6       /**< Record Access Control Point opcode - Response code. */

/**@brief Record Access Control Point operators. */
#define RACP_OPERATOR_NULL                  0       /**< Record Access Control Point operator - Null. */
#define RACP_OPERATOR_ALL                   1       /**< Record Access Control Point operator - All records. */
#define RACP_OPERATOR_LESS_OR_EQUAL         2       /**< Record Access Control Point operator - Less than or equal to. */
#define RACP_OPERATOR_GREATER_OR_EQUAL      3       /**< Record Access Control Point operator - Greater than or equal to. */
#define RACP_OPERATOR_RANGE                 4       /**< Record Access Control Point operator - Within range of (inclusive). */
#define RACP_OPERATOR_FIRST                 5       /**< Record Access Control Point operator - First record (i.e. oldest record). */
#define RACP_OPERATOR_LAST                  6       /**< Record Access Control Point operator - Last record (i.e. most recent record). */
#define RACP_OPERATOR_RFU_START             7       /**< Record Access Control Point operator - Start of Reserved for Future Use area. */

/**@brief Record Access Control Point response codes. */
#define RACP_RESPONSE_RESERVED              0       /**< Record Access Control Point response code - Reserved for future use. */
#define RACP_RESPONSE_SUCCESS               1       /**< Record Access Control Point response code - Successful operation. */
#define RACP_RESPONSE_OPCODE_UNSUPPORTED    2       /**< Record Access Control Point response code - Unsupported op code received. */
#define RACP_RESPONSE_INVALID_OPERATOR      3       /**< Record Access Control Point response code - Operator not valid for service. */
#define RACP_RESPONSE_OPERATOR_UNSUPPORTED  4       /**< Record Access Control Point response code - Unsupported operator. */
#define RACP_RESPONSE_INVALID_OPERAND       5       /**< Record Access Control Point response code - Operand not valid for service. */
#define RACP_RESPONSE_NO_RECORDS_FOUND      6       /**< Record Access Control Point response code - No matching records found. */
#define RACP_RESPONSE_ABORT_FAILED          7       /**< Record Access Control Point response code - Abort could not be completed. */
#define RACP_RESPONSE_PROCEDURE_NOT_DONE    8       /**< Record Access Control Point response code - Procedure could not be completed. */
#define RACP_RESPONSE_OPERAND_UNSUPPORTED   9       /**< Record Access Control Point response code - Unsupported operand. */

/**@brief Record Access Control Point value structure. */
typedef struct
{
    uint8_t   opcode;                               /**< Op Code. */
    uint8_t   operator;                             /**< Operator. */
    uint8_t   operand_len;                          /**< Length of the operand. */
    uint8_t * p_operand;                            /**< Pointer to the operand. */
} ble_racp_value_t;

/**@brief Function for decoding a Record Access Control Point write.
 *
 * @details This call decodes a write to the Record Access Control Point.
 *
 * @param[in]   data_len     Length of data in received write.
 * @param[in]   p_data       Pointer to received data.
 * @param[out]  p_racp_val   Pointer to decoded Record Access Control Point write.
 *                           @note This does not do a data copy. It assumes the data pointed to by
 *                                 p_data is persistant until no longer needed.
 */
void ble_racp_decode(uint8_t data_len, uint8_t * p_data, ble_racp_value_t * p_racp_val);

/**@brief Function for encoding a Record Access Control Point response.
 *
 * @details This call encodes a response from the Record Access Control Point response.
 *
 * @param[in]   p_racp_val   Pointer to Record Access Control Point to encode.
 * @param[out]  p_data       Pointer to where encoded data is written.
 *                           NOTE! It is calling routines respsonsibility to make sure.
 *
 * @return      Length of encoded data.
 */
uint8_t ble_racp_encode(const ble_racp_value_t * p_racp_val, uint8_t * p_data);

#endif // BLE_RACP_H__

/** @} */

