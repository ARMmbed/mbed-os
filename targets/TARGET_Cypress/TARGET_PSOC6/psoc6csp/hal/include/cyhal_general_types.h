/***************************************************************************//**
* \file cyhal_general_types.h
*
* \brief
* Provides basic types that are used across different HAL drivers.
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
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

/**
* \addtogroup group_hal_general_types General Types
* \ingroup group_hal_types
* \{
* This section documents the basic types that are used by multiple HAL drivers.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_results Result Codes
* \ingroup group_hal_types
* \{
* HAL specific return codes definitions for all drivers. HAL drivers are generally
* built on top of a lower level driver. In this case the errors returned from the
* HAL could be one of the values below or a value from that lower level driver.
* See the device specific documentation for details about the other error codes.
*/

/** \cond INTERNAL */
/** Generate a result code specific to the HAL driver */
#define CYHAL_RSLT_CREATE(type, driver, code)           \
    (CY_RSLT_CREATE(type, CY_RSLT_MODULE_ABSTRACTION_HAL_BASE, ((uint16_t)driver | (uint16_t)code)))
/** \endcond */

/**
 * Enum to specify module IDs for \ref cy_rslt_t values returned from the HAL.
 */
enum cyhal_rslt_module_chip
{
    CYHAL_RSLT_MODULE_HWMGR         = (0x01 << 8),  //!< An error occurred in hardware management module
    CYHAL_RSLT_MODULE_ADC           = (0x02 << 8),  //!< An error occurred in ADC module
    CYHAL_RSLT_MODULE_CLOCK         = (0x03 << 8),  //!< An error occurred in Clock module
    CYHAL_RSLT_MODULE_COMP          = (0x04 << 8),  //!< An error occurred in comparator module
    CYHAL_RSLT_MODULE_CRC           = (0x05 << 8),  //!< An error occurred in crypto CRC module
    CYHAL_RSLT_MODULE_DAC           = (0x06 << 8),  //!< An error occurred in DAC module
    CYHAL_RSLT_MODULE_DMA           = (0x07 << 8),  //!< An error occurred in DMA module
    CYHAL_RSLT_MODULE_EZI2C         = (0x08 << 8),  //!< An error occurred in EZI2C module
    CYHAL_RSLT_MODULE_FLASH         = (0x09 << 8),  //!< An error occurred in flash module
    CYHAL_RSLT_MODULE_GPIO          = (0x0A << 8),  //!< An error occurred in GPIO module
    CYHAL_RSLT_MODULE_I2C           = (0x0B << 8),  //!< An error occurred in I2C module
    CYHAL_RSLT_MODULE_I2S           = (0x0C << 8),  //!< An error occurred in I2S module
    CYHAL_RSLT_MODULE_INTERCONNECT  = (0x0D << 8),  //!< An error occurred in Interconnect module
    CYHAL_RSLT_MODULE_OPAMP         = (0x0E << 8),  //!< An error occurred in OpAmp module
    CYHAL_RSLT_MODULE_PDMPCM        = (0x0F << 8),  //!< An error occurred in PDM/PCM module
    CYHAL_RSLT_MODULE_PWM           = (0x10 << 8),  //!< An error occurred in PWM module
    CYHAL_RSLT_MODULE_QSPI          = (0x11 << 8),  //!< An error occurred in QSPI module
    CYHAL_RSLT_MODULE_QUADDEC       = (0x12 << 8),  //!< An error occurred in Quadrature Decoder module
    CYHAL_RSLT_MODULE_RTC           = (0x13 << 8),  //!< An error occurred in RTC module
    CYHAL_RSLT_MODULE_SDHC          = (0x14 << 8),  //!< An error occurred in SDHC module
    CYHAL_RSLT_MODULE_SDIO          = (0x15 << 8),  //!< An error occurred in SDIO module
    CYHAL_RSLT_MODULE_SPI           = (0x16 << 8),  //!< An error occurred in SPI module
    CYHAL_RSLT_MODULE_SYSPM         = (0x17 << 8),  //!< An error occurred in SysPM module
    CYHAL_RSLT_MODULE_SYSTEM        = (0x18 << 8),  //!< An error occurred in System module
    CYHAL_RSLT_MODULE_TIMER         = (0x19 << 8),  //!< An error occurred in Timer module
    CYHAL_RSLT_MODULE_TRNG          = (0x1A << 8),  //!< An error occurred in RNG module
    CYHAL_RSLT_MODULE_UART          = (0x1B << 8),  //!< An error occurred in UART module
    CYHAL_RSLT_MODULE_USB           = (0x1C << 8),  //!< An error occurred in USB module
    CYHAL_RSLT_MODULE_WDT           = (0x1D << 8),  //!< An error occurred in WDT module
};

/**
 * Enum to specify all of the digital output signals supported by different hardware peripherals. These can be used
 * as inputs to other peripherals if the selected device has internal routing resources.
 */
typedef enum
{
    CYHAL_SIGNAL_DMA_COMPLETE,      //!< DMA complete signal

    CYHAL_SIGNAL_PWM_OUT,           //!< PWM output signal
    CYHAL_SIGNAL_PWM_OUT_INV,       //!< PWM output signal inverted
    CYHAL_SIGNAL_PWM_OVERFLOW,      //!< PWM overflow signal
    CYHAL_SIGNAL_PWM_UNDERFLOW,     //!< PWM underflow signal
    CYHAL_SIGNAL_PWM_COMPARE,       //!< PWM period match signal

    CYHAL_SIGNAL_TIMER_OVERFLOW,    //!< Timer overflow signal
    CYHAL_SIGNAL_TIMER_UNDERFLOW,   //!< Timer underflow signal
    CYHAL_SIGNAL_TIMER_CAPTURE,     //!< Timer capture match signal

    CYHAL_SIGNAL_QUADDEC_TC,        //!< Quadrature Decoder terminal count signal. High on index event,
                                    //!< or when counter reaches min/max value.
} cyhal_signal_digital_out_t;

/**
 * \} group_hal_results
 */

/** Modes that can be used for asynchronous transfers */
typedef enum {
    /** Use DMA if available. Otherwise fall back to software transfer. (Default) */
    CYHAL_ASYNC_DMA,
    /** Always perform a software transfer */
    CYHAL_ASYNC_SW,
} cyhal_async_mode_t;

/** @brief Selectable power levels.
  *
  * Power levels are defined relative to others. Higher power levels
  * offer better performance but consume more power.
  *
  * Not all hardware supports four discrete power levels. If fewer
  * power levels are supported, the values will be mapped as follows:
  * | 4 Levels       | 3 Levels       | 2 Levels                      |
  * | ---------------| -------------- | ----------------------------- |
  * | Off            | Off            | Off                           |
  * | Low            | Low = Medium   | Low = Medium = High = Default |
  * | Medium         | High = Default |                               |
  * | High = Default |                |                               |
  * See the implementation specific documentation for details.
  */
typedef enum
{
    /** Power-off the comparator, while retaining configuration */
    CYHAL_POWER_LEVEL_OFF,
    /** Low comparator power and speed */
    CYHAL_POWER_LEVEL_LOW,
    /** Medium comparator power and speed */
    CYHAL_POWER_LEVEL_MEDIUM,
    /** High comparator power and speed */
    CYHAL_POWER_LEVEL_HIGH,
    /** Default comparator power and speed */
    CYHAL_POWER_LEVEL_DEFAULT
} cyhal_power_level_t;

/**
 * \addtogroup group_hal_syspm System Power Management
 * \ingroup group_hal
 * \{
 */

/** Flags enum for the cpu state in a custom callback. This is used to indicate what
 * states a callback should be called in. When a callback is called, only one of these
 * will be set at a time.
 */
typedef enum
{
    CYHAL_SYSPM_CB_CPU_SLEEP         = 0x01U,   /**< Flag for MCU sleep callback. */
    CYHAL_SYSPM_CB_CPU_DEEPSLEEP     = 0x02U,   /**< Flag for MCU deep sleep callback. */
    CYHAL_SYSPM_CB_SYSTEM_HIBERNATE  = 0x04U,   /**< Flag for Hibernate callback. */
    CYHAL_SYSPM_CB_SYSTEM_NORMAL     = 0x08U,   /**< Flag for Normal mode callback. */
    CYHAL_SYSPM_CB_SYSTEM_LOW        = 0x10U,   /**< Flag for Low power mode callback. */
} cyhal_syspm_callback_state_t;

/** Define for enabling all system and MCU state callbacks .*/
#define CYHAL_SYSPM_CALLBACK_STATE_ALL (CYHAL_SYSPM_CB_CPU_SLEEP\
                                        | CYHAL_SYSPM_CB_CPU_DEEPSLEEP\
                                        | CYHAL_SYSPM_CB_SYSTEM_HIBERNATE\
                                        | CYHAL_SYSPM_CB_SYSTEM_NORMAL\
                                        | CYHAL_SYSPM_CB_SYSTEM_LOW)

/** Enumeration of the transition modes in custom callback. The general sequence
 * is: CHECK_READY, BEFORE_TRANSITION, AFTER_TRANSITION.
 * If any callback indicates that it is not able to change state as part of
 * CHECK_READY, CHECK_FAIL will be run instead of the BEFORE/AFTER_TRANSITION.
 */
typedef enum
{
    CYHAL_SYSPM_CHECK_READY         = 0x01U, /**< Callbacks with this mode are executed before entering into the
                                                low power mode. The purpose of this transition state is to check
                                                if the device is ready to enter the low power mode. The application
                                                must not perform any actions that would prevent transition after
                                                returning true for this mode. */
    CYHAL_SYSPM_CHECK_FAIL          = 0x02U, /**< Callbacks with this mode are only executed if the callback returned true
                                                for CYHAL_SYSPM_CHECK_READY and a later callback returns false for
                                                CYHAL_SYSPM_CHECK_READY. This mode should roll back any changes made
                                                to avoid blocking transition made in CYHAL_SYSPM_CHECK_READY mode*/
    CYHAL_SYSPM_BEFORE_TRANSITION   = 0x04U, /**< Callbacks with this mode are executed after the CYHAL_SYSPM_CHECK_READY
                                                callbacks' execution returns true. In this mode, the application must
                                                perform the actions to be done before entering into the low power mode. */
    CYHAL_SYSPM_AFTER_TRANSITION    = 0x08U, /**< In this mode, the application must perform the actions to be done after
                                                exiting the low power mode. */
} cyhal_syspm_callback_mode_t;

/** The system wide custom action power callback type.
*
* @param[in] state          State the system or CPU is being transitioned into.
* @param[in] mode           Callback mode.
* @param[in] callback_arg   User argument passed as part of registering callback in @ref cyhal_syspm_register_callback.
* @return If mode is @ref CYHAL_SYSPM_CHECK_READY, then this indicates whether the low power mode should be allowed (true) or not (false).
          Otherwise the return value is ignored.
*/
typedef bool (*cyhal_syspm_callback_t)(cyhal_syspm_callback_state_t state, cyhal_syspm_callback_mode_t mode, void* callback_arg);

/** Power management callback data object. This is used to register a new
 * callback handler for power management transitions. The specific power
 * states to register for can be specified as well as any modes
 */
typedef struct cyhal_syspm_callback_data
{
    /** Callback to run on power state change */
    cyhal_syspm_callback_t              callback;
    /** Power states that should trigger calling the callback. Multiple
     * \ref cyhal_syspm_callback_state_t values can be ored together. */
    cyhal_syspm_callback_state_t        states;
    /** Modes to ignore invoking the callback for. Multiple
     * \ref cyhal_syspm_callback_mode_t values can be ored together. */
    cyhal_syspm_callback_mode_t         ignore_modes;
    /** Argument value to provide to the callback. */
    void                                *args;
    /** Pointer to the next callback strucure. This should be initialized to NULL. */
    struct cyhal_syspm_callback_data    *next;
} cyhal_syspm_callback_data_t;

/**
 * \} group_hal_syspm
 */

/**
  * \addtogroup group_hal_clock
  * \{
  */

/** Enum defining the different ways of specifying the acceptable clock tolerance. */
typedef enum
{
    CYHAL_TOLERANCE_HZ,         //!< Clock tolerance specified directly in Hertz.
    CYHAL_TOLERANCE_PPM,        //!< Clock tolerance specified in parts-per-million.
    CYHAL_TOLERANCE_PERCENT,    //!< Clock tolerance specified in a percent.
} cyhal_clock_tolerance_unit_t;

/** Structure defining a clock tolerance. */
typedef struct
{
    cyhal_clock_tolerance_unit_t type;  //!< The type of the clock tolerance value.
    uint32_t                     value; //!< The tolerance value to use.
} cyhal_clock_tolerance_t;

/**
 * \} group_hal_clock
 */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_general_types */
