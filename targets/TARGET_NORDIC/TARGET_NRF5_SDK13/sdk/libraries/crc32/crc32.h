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
 * @defgroup crc32 CRC32 compute
 * @{
 * @ingroup hci_transport
 *
 * @brief    This module implements the CRC-32 calculation in the blocks.
 */

#ifndef CRC32_H__
#define CRC32_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Function for calculating CRC-32 in blocks.
 *
 * Feed each consecutive data block into this function, along with the current value of p_crc as
 * returned by the previous call of this function. The first call of this function should pass NULL
 * as the initial value of the crc in p_crc.
 *
 * @param[in] p_data The input data block for computation.
 * @param[in] size   The size of the input data block in bytes.
 * @param[in] p_crc  The previous calculated CRC-32 value or NULL if first call.
 *
 * @return The updated CRC-32 value, based on the input supplied.
 */
uint32_t crc32_compute(uint8_t const * p_data, uint32_t size, uint32_t const * p_crc);


#ifdef __cplusplus
}
#endif

#endif // CRC32_H__

/** @} */
