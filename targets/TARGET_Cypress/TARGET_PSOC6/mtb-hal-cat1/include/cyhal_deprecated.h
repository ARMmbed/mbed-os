/***************************************************************************//**
* \file cyhal_deprecated.h
*
* \brief
* Provides access to items that are device specific and no longer part of the
* common HAL API.
*
********************************************************************************
* \copyright
* Copyright 2018-2021 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#include "cy_pdl.h"
#include "cyhal_general_types.h"
#include "cyhal_hw_resources.h"
#include "cy_result.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* \addtogroup group_hal_impl_deprecated Deprecated
* \ingroup group_hal_impl
* \{
* The following CAT1 specific items have been deprecated and replaced by more generic items. Each item
* will continue to work for now, but will be removed in a future release. All deprecated items reference
* the item that replaces it.
*/

/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_ERROR (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 0))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_INVALID_CLK_DIVIDER (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 1))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_UNABLE_TO_SET_CLK_FREQ (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 2))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_SRC_CLK_DISABLED (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 3))
/** An error occurred in System module */
#define CYHAL_SYSTEM_RSLT_NO_VALID_DIVIDER (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SYSTEM , 4))

/**
 * Attempts to put the device to sleep until an interrupt wakes it up.
 * \warning This function is deprecated. Use \ref cyhal_syspm_sleep instead.
 */
#define cyhal_system_sleep()                    Cy_SysPm_CpuEnterSleep(CY_SYSPM_WAIT_FOR_INTERRUPT)
/**
 * Attempts to put the device into deep sleep until an interrupt wakes it up.
 * \warning This function is deprecated. Use \ref cyhal_syspm_deepsleep instead.
 */
#define cyhal_system_deepsleep()                Cy_SysPm_CpuEnterDeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT)

#define CYHAL_SDIO_RET_NO_ERRORS           (0x00)    /**< No error*/
#define CYHAL_SDIO_RET_NO_SP_ERRORS        (0x01)    /**< Non-specific error code*/
#define CYHAL_SDIO_RET_CMD_CRC_ERROR       (0x02)    /**< There was a CRC error on the Command/Response*/
#define CYHAL_SDIO_RET_CMD_IDX_ERROR       (0x04)    /**< The index for the command didn't match*/
#define CYHAL_SDIO_RET_CMD_EB_ERROR        (0x08)    /**< There was an end bit error on the command*/
#define CYHAL_SDIO_RET_DAT_CRC_ERROR       (0x10)    /**< There was a data CRC Error*/
#define CYHAL_SDIO_RET_CMD_TIMEOUT         (0x20)    /**< The command didn't finish before the timeout period was over*/
#define CYHAL_SDIO_RET_DAT_TIMEOUT         (0x40)    /**< The data didn't finish before the timeout period was over*/
#define CYHAL_SDIO_RET_RESP_FLAG_ERROR     (0x80)    /**< There was an error in the resposne flag for command 53*/

#define CYHAL_SDIO_CLOCK_ERROR             (0x100)    /**< Failed to initial clock for SDIO */
#define CYHAL_SDIO_BAD_ARGUMENT            (0x200)    /**< Bad argument passed for SDIO */
#define CYHAL_SDIO_SEMA_NOT_INITED         (0x400)    /**< Semaphore is not initiated */
#define CYHAL_SDIO_FUNC_NOT_SUPPORTED      (0x800)    /**< Function is not supported */
#define CYHAL_SDIO_CANCELED               (0x1000)    /**< Operation canceled */
#define CYHAL_SDIO_PM_PENDING_ERROR       (0x2000)    /**< Transfer cannot be initiated after power mode transition allowed.*/

/** Semaphore not initiated error define */
#define CYHAL_SDIO_RSLT_ERR_SEMA_NOT_INITED             \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SDIO, CYHAL_SDIO_SEMA_NOT_INITED))
/** Error define based on SDIO lower function return value */
#define CYHAL_SDIO_RSLT_ERR_FUNC_RET(retVal)        \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SDIO, ((uint16_t)retVal)))
/** Clock initialization error define */
#define CYHAL_SDIO_RSLT_ERR_CLOCK                   \
    (CYHAL_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_SDIO, CYHAL_SDIO_CLOCK_ERROR))

/** SDHC CARD INTERRUPT event (The synchronized value of the DAT[1] interrupt input for SD mode) */
#define CYHAL_SDHC_CARD_INTERRUPT           ((cyhal_sdhc_event_t)0x00100)
/** SDHC Asynchronous read operation is complete event (CYHAL_SDHC_XFER_COMPLETE is used instead) */
#define CYHAL_SDHC_ASYNC_READ_COMPLETE      ((cyhal_sdhc_event_t)0x10000)
/** SDHC Asynchronous write operation is complete event (CYHAL_SDHC_XFER_COMPLETE is used instead) */
#define CYHAL_SDHC_ASYNC_WRITE_COMPLETE     ((cyhal_sdhc_event_t)0x20000)

/** Enum for clock type to configure. HFCLKs are configured using different APIs and does not using this enum.
 *  \warning This type is deprecated. Use \ref cyhal_clock_block_t instead.
 */
typedef enum
{
    CYHAL_SYSTEM_CLOCK_CM4,
    CYHAL_SYSTEM_CLOCK_CM0,
    CYHAL_SYSTEM_CLOCK_PERI,
} cyhal_system_clock_t;

/** Available clock divider types.
 * \warning This type is deprecated. Use \ref cyhal_clock_block_t instead.
 */
typedef cy_en_divider_types_t cyhal_clock_divider_types_t;

/** Divider for CM4, CM0 and Peri clock. Supports values between [1, 256]
 *  \warning This type is deprecated. Use \ref cyhal_clock_t instead.
 */
typedef uint16_t cyhal_system_divider_t;

/** Peripheral clock divider type.
 *  \warning This type is deprecated. Use \ref cyhal_clock_t instead.
 */
typedef cyhal_clock_t cyhal_clock_divider_t;

/** Allocate (pick and reserve) an Clock Divider resource and provide a reference to it.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_allocate() instead.
 *
 * @param[out] obj           The resource object that was allocated
 * @param[in]  div           The type of divider to allocate
 * @param[in]  accept_larger Whether a larger type can be provided if there are none of the requested size available
 * @return The status of the reserve request
 */
cy_rslt_t cyhal_hwmgr_allocate_clock(cyhal_clock_divider_t* obj, cyhal_clock_divider_types_t div, bool accept_larger) /* __attribute__ ((deprecated)) */;

/** Free the specified Clock Divider resource and allow it be used by something else.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_free() instead.
 *
 * @param[in] obj           The resource object that was allocated
 */
void cyhal_hwmgr_free_clock(cyhal_clock_divider_t* obj) /* __attribute__ ((deprecated)) */;

/** Gets the specified clock's current frequency.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_get_frequency() instead.
 *
 * @param[in]  clock        ID of clock to configure
 * @param[out] frequency_hz The frequency the clock is currently running at
 * @return The status of the get_frequency request
 */
cy_rslt_t cyhal_system_clock_get_frequency(uint8_t clock, uint32_t *frequency_hz) /* __attribute__ ((deprecated)) */;

/** Sets the specified clock's frequency and enables it. This will turn on any
 * additional clocks needed to drive this.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_set_frequency() instead.
 *
 * @param[in]  clock        ID of clock to configure
 * @param[in]  frequency_hz The frequency to run the clock at
 * @return The status of the set_frequency request
 */
cy_rslt_t cyhal_system_clock_set_frequency(uint8_t clock, uint32_t frequency_hz) /* __attribute__ ((deprecated)) */;

/** Divides the clock frequency by the divider.
 *
 * \warning This function is deprecated. Use \ref cyhal_clock_set_divider() instead.
 *
 * @param[in] clock   The clock to configure divider value for
 * @param[in] divider The divider value to divide the frequency by
 * @return The status of the set_divider request
 */
cy_rslt_t cyhal_system_clock_set_divider(cyhal_system_clock_t clock, cyhal_system_divider_t divider) /* __attribute__ ((deprecated)) */;

/** Register the specified handler with the power manager to be notified of power
 * state changes.
 * \warning This function is deprecated. Use \ref cyhal_syspm_register_callback() instead.
 *
 * @param[in] callback   The callback configuration data
 * @return The status of the register_callback request
 */
cy_rslt_t cyhal_system_register_callback(cyhal_system_callback_t *callback);

/** Removes the registered handler from the power manager so no future notifications are made.
 *
 * \warning This function is deprecated. Use \ref cyhal_syspm_unregister_callback() instead.
 *
 * @param[in] callback   The callback configuration data
 * @return The status of the unregister_callback request
 */
cy_rslt_t cyhal_system_unregister_callback(cyhal_system_callback_t const *callback);

/** Initialize a single-ended ADC channel.
 *
 * \warning This function is deprecated. Use \ref cyhal_adc_channel_init_diff() instead with
 * @ref CYHAL_ADC_VNEG specified for the `vminus` argument.
 *
 * Configures the pin used by ADC.
 * @param[out] obj The adc channel object to initialize
 * @param[in]  adc The adc for which the channel should be initialized
 * @param[in]  pin The adc pin name
 * @return The status of the init request. The status of the init request.
 * On failure, a problem specific error code will be returned.
 * This error could be from the HAL or lower level driver.<br>
 * For all other return codes, please refer to device driver documentation available in the BSP landing page
 */
cy_rslt_t cyhal_adc_channel_init(cyhal_adc_channel_t *obj, cyhal_adc_t* adc, cyhal_gpio_t pin);

/** \} group_hal_impl_deprecated */

#if defined(__cplusplus)
}
#endif /* __cplusplus */
