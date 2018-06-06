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

#ifndef NRF_DRV_RNG_H__
#define NRF_DRV_RNG_H__

#include <stdbool.h>
#include <stdint.h>

#include "nrf_rng.h"
#include "sdk_errors.h"
#include "nrf_drv_config.h"

/**
 * @addtogroup nrf_rng RNG HAL and driver
 * @ingroup nrf_drivers
 * @brief Random number generator (RNG) APIs.
 * @details The RNG HAL provides basic APIs for accessing the registers of the random number generator. 
 * The RNG driver provides APIs on a higher level.
 *
 * @defgroup nrf_drv_rng RNG driver
 * @{
 * @ingroup nrf_rng
 * @brief Driver for managing the random number generator (RNG).
 */

/**@brief Struct for RNG configuration. */
typedef struct
{
    bool     error_correction;      /**< Error correction flag. */
    uint8_t  interrupt_priority;    /**< interrupt priority */
} nrf_drv_rng_config_t;

/**@brief RNG default configuration. */
#define NRF_DRV_RNG_DEFAULT_CONFIG                                                    \
    {                                                                                 \
        .error_correction   = RNG_CONFIG_ERROR_CORRECTION,                            \
        .interrupt_priority = RNG_CONFIG_IRQ_PRIORITY,                                \
    }

/**
 * @brief Function for initializing the nrf_drv_rng module.
 *
 * @param[in]  p_config                   Initial configuration. Default configuration used if NULL.
 *
 * @retval  NRF_SUCCESS                       Driver was successfully initialized.
 * @retval  NRF_ERROR_INVALID_STATE           Driver was already initialized.
 * @retval  NRF_ERROR_INVALID_LENGTH          Pool size have to be a power of 2.
 * @retval  NRF_ERROR_SOFTDEVICE_NOT_ENABLED  SoftDevice is present, but not enabled.
 */
ret_code_t nrf_drv_rng_init(nrf_drv_rng_config_t const * p_config);

/**
 * @brief Function for uninitializing the nrf_drv_rng module.
 */
void nrf_drv_rng_uninit(void);

/**
 * @brief Function for getting the number of currently available random bytes.
 *
 * @param[out] p_bytes_available                    The number of bytes currently available in the pool.
 *
 * @retval     NRF_SUCCESS                          If the number of available random bytes was written to p_bytes_available.
 */
ret_code_t nrf_drv_rng_bytes_available(uint8_t * p_bytes_available);

/**
 * @brief Function for querying the capacity of the application random pool.
 *
 * @param[out] p_pool_capacity                      The capacity of the pool.
 *
 * @retval     NRF_SUCCESS                          If the capacity of the pool was written to p_pool_capacity.
 */
ret_code_t nrf_drv_rng_pool_capacity(uint8_t * p_pool_capacity);

/**
 * @brief Function for getting the vector of random numbers.
 *
 * @param[out] p_buff                               Pointer to uint8_t buffer for storing the bytes.
 * @param[in]  length                               Number of bytes to take from the pool and place in p_buff.
 *
 * @retval     NRF_SUCCESS                          If the requested bytes were written to p_buff.
 * @retval     NRF_ERROR_NO_MEM                     If no bytes were written to the buffer
 *                                                  because there were not enough bytes available in p_buff.
 * @retval     NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES If no bytes were written to the buffer
 *                                                  because there were not enough bytes available in the pool.
 */
ret_code_t nrf_drv_rng_rand(uint8_t * p_buff, uint8_t length);

/**
 * @brief Blocking function for getting an arbitrary array of random numbers.
 *
 * @note This function may execute for a substantial amount of time depending on the length of the buffer
 *       required and on the state of the current internal pool of random numbers.
 *
 * @param[out] p_buff                               Pointer to uint8_t buffer for storing the bytes.
 * @param[in]  length                               Number of bytes place in p_buff.
 *
 * @retval     NRF_SUCCESS                          If the requested bytes were written to p_buff.
 */
ret_code_t nrf_drv_rng_block_rand(uint8_t * p_buff, uint32_t length);

/**
 *@}
 **/
#endif // NRF_DRV_RNG_H__
