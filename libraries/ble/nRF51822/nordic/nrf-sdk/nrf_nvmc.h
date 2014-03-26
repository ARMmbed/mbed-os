/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 17685 $
 */

/**
 * @file
 * @brief NMVC driver API.
 */

#ifndef NRF_NVMC_H__
#define NRF_NVMC_H__

#include <stdint.h>
#include "nordic_global.h"


/**
 * @defgroup nrf_nvmc Non-volatile memory controller
 * @{
 * @ingroup nrf_drivers
 * @brief Driver for the nRF51 NVMC peripheral.
 *
 * This driver allows writing to the non-volatile memory (NVM) regions
 * of the nRF51. In order to write to NVM the controller must be powered
 * on and the relevant page must be erased.
 *
 */


/**
 * @brief Erase a page in flash. This is required before writing to any
 * address in the page.
 *
 * @param address Start address of the page. 
 */
void nrf_nvmc_page_erase(uint32_t address);


/**
 * @brief Write a single byte to flash.
 *
 * The function reads the word containing the byte, and then
 * rewrites the entire word.
 *
 * @param address Address to write to.
 * @param value   Value to write.
 */
void nrf_nvmc_write_byte(uint32_t address , uint8_t value);


/**
 * @brief Write a 32-bit word to flash. 
 * @param address Address to write to.
 * @param value   Value to write.
 */
void nrf_nvmc_write_word(uint32_t address, uint32_t value);


/**
 * @brief Write consecutive bytes to flash.
 *
 * @param address   Address to write to.
 * @param src       Pointer to data to copy from.
 * @param num_bytes Number of bytes in src to write.
 */
void nrf_nvmc_write_bytes(uint32_t  address, const uint8_t * src, uint32_t num_bytes);


/**
 @ @brief Write consecutive words to flash.
 * 
 * @param address   Address to write to.
 * @param src       Pointer to data to copy from.
 * @param num_words Number of bytes in src to write.
 */
void nrf_nvmc_write_words(uint32_t address, const uint32_t * src, uint32_t num_words);


#endif // NRF_NVMC_H__
/** @} */


