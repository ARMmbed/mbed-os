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
 * @defgroup crc_compute CRC compute
 * @{
 * @ingroup hci_transport
 *
 * @brief    This module implements CRC-16-CCITT (polynomial 0x1021) with 0xFFFF initial value.
 *           The data can be passed in multiple blocks.
 */
 
#ifndef CRC16_H__
#define CRC16_H__

#include <stdint.h>

/**@brief Function for calculating CRC-16 in blocks.
 *
 * Feed each consecutive data block into this function, along with the current value of p_crc as
 * returned by the previous call of this function. The first call of this function should pass NULL
 * as the initial value of the crc in p_crc.
 *
 * @param[in] p_data The input data block for computation.
 * @param[in] size   The size of the input data block in bytes.
 * @param[in] p_crc  The previous calculated CRC-16 value or NULL if first call.
 *
 * @return The updated CRC-16 value, based on the input supplied.
 */
uint16_t crc16_compute(uint8_t const * p_data, uint32_t size, uint16_t const * p_crc);

#endif // CRC16_H__
 
/** @} */
