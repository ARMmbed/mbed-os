/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
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

#ifndef NRF_DRV_LPCOMP_H__
#define NRF_DRV_LPCOMP_H__

#include "nrf_lpcomp.h"
#include "sdk_errors.h"
#include "nrf_drv_common.h"
#include "nrf_drv_config.h"

/**
 * @addtogroup nrf_lpcomp LPCOMP HAL and driver
 * @ingroup nrf_drivers
 * @brief Low Power Comparator (LPCOMP) APIs.
 * @details The LPCOMP HAL provides basic APIs for accessing the registers of Low Power Comparator. 
 * The LPCOMP driver provides APIs on a higher level.
 *
 * @defgroup nrf_drivers_lpcomp LPCOMP driver
 * @{
 * @ingroup nrf_lpcomp
 * @brief Low Power Comparator (LPCOMP) driver. 
 */

/**@brief LPCOMP event handler function type.
 * @param[in] event  LPCOMP event.
 */
typedef void (* lpcomp_events_handler_t)(nrf_lpcomp_event_t event);

/**@brief LPCOMP configuration.
 */
typedef struct
{
    nrf_lpcomp_config_t    hal;                /**< LPCOMP HAL configuration. */
    nrf_lpcomp_input_t     input;              /**< Input to be monitored. */
    uint8_t                interrupt_priority; /**< LPCOMP interrupt priority. */
} nrf_drv_lpcomp_config_t;

/** @brief LPCOMP driver default configuration including the LPCOMP HAL configuration. */
#define NRF_DRV_LPCONF_DEFAULT_CONFIG                                              \
    {                                                                              \
        .hal                = {LPCOMP_CONFIG_REFERENCE , LPCOMP_CONFIG_DETECTION}, \
        .input              = LPCOMP_CONFIG_INPUT,                                 \
        .interrupt_priority = LPCOMP_CONFIG_IRQ_PRIORITY                           \
    }

/**
 * @brief Function for initializing the LPCOMP driver.
 * 
 * This function initializes the LPCOMP driver, but does not enable the peripheral or any interrupts. 
 * To start the driver, call the function nrf_drv_lpcomp_enable() after initialization.
 *
 * If no configuration structure is provided, the driver is initialized with the default settings.
 *
 * @param[in] p_config              Initial configuration. If NULL, the default configuration is used.
 * @param[in] events_handler        Handler function.
 * @retval NRF_ERROR_INVALID_PARAM If the configuration is invalid.
 * @retval NRF_ERROR_INVALID_STATE If the driver has already been initialized.
 */
ret_code_t nrf_drv_lpcomp_init(const nrf_drv_lpcomp_config_t * p_config,
                               lpcomp_events_handler_t   events_handler);


/**
 *  @brief Function for uninitializing the LCOMP driver.
 *
 *  This function uninitializes the LPCOMP driver. The LPCOMP peripheral and 
 *  its interrupts are disabled, and local variables are cleaned. After this call, you must
 *  initialize the driver again by calling nrf_drv_lpcomp_init() if you want to use it.
 *
 *  @sa nrf_drv_lpcomp_disable()
 *  @sa nrf_drv_lpcomp_init()
 */
void     nrf_drv_lpcomp_uninit(void);

/**@brief Function for enabling the LPCOMP peripheral and interrupts.
 *
 * Before calling this function, the driver must be initialized. This function
 * enables the LPCOMP peripheral and its interrupts.
 *
 * @sa nrf_drv_lpcomp_disable()
 */
void nrf_drv_lpcomp_enable(void);

/**@brief Function for disabling the LPCOMP peripheral.
 *
 * Before calling this function, the driver must be initialized. This function disables the LPCOMP
 * peripheral and its interrupts.
 *
 * @sa nrf_drv_lpcomp_enable()
 */
void nrf_drv_lpcomp_disable(void);

/**
 *@}
 **/

 #endif /* NRF_DRV_LPCOMP_H__ */
