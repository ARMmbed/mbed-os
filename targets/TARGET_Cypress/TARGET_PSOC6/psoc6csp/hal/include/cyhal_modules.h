/***************************************************************************//**
* \file cyhal_modules.h
*
* \brief
* Provides an enum of all HAL modules types that can be used for generating
* custom cy_rslt_t items.
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
* \ingroup group_result
* \{
* Enum definition for all HAL resource modules.
*/

#pragma once

#include "cy_result.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** Enum to in indicate which module an errors occurred in. */
/**
 * @brief Enum to specify module IDs for @ref cy_rslt_t values returned from the HAL.
 *
 */

enum cyhal_rslt_module_chip
{
    CYHAL_RSLT_MODULE_CHIP_HWMGR = CY_RSLT_MODULE_ABSTRACTION_HAL_BASE, //!< An error occurred in hardware management module
    CYHAL_RSLT_MODULE_ADC,                                              //!< An error occurred in ADC module
    CYHAL_RSLT_MODULE_COMP,                                             //!< An error occurred in comparator module
    CYHAL_RSLT_MODULE_CRC,                                              //!< An error occurred in crypto CRC module
    CYHAL_RSLT_MODULE_DAC,                                              //!< An error occurred in DAC module
    CYHAL_RSLT_MODULE_DMA,                                              //!< An error occurred in DMA module
    CYHAL_RSLT_MODULE_FLASH,                                            //!< An error occurred in flash module
    CYHAL_RSLT_MODULE_GPIO,                                             //!< An error occurred in GPIO module
    CYHAL_RSLT_MODULE_I2C,                                              //!< An error occurred in I2C module
    CYHAL_RSLT_MODULE_I2S,                                              //!< An error occurred in I2S module
    CYHAL_RSLT_MODULE_INTERCONNECT,                                     //!< An error occurred in Interconnct module
    CYHAL_RSLT_MODULE_OPAMP,                                            //!< An error occurred in OpAmp module
    CYHAL_RSLT_MODULE_PDMPCM,                                           //!< An error occurred in PDM/PCM module
    CYHAL_RSLT_MODULE_PWM,                                              //!< An error occurred in PWM module
    CYHAL_RSLT_MODULE_QSPI,                                             //!< An error occurred in QSPI module
    CYHAL_RSLT_MODULE_RTC,                                              //!< An error occurred in RTC module
    CYHAL_RSLT_MODULE_SDHC,                                             //!< An error occurred in SDHC module
    CYHAL_RSLT_MODULE_SDIO,                                             //!< An error occurred in SDIO module
    CYHAL_RSLT_MODULE_SPI,                                              //!< An error occurred in SPI module
    CYHAL_RSLT_MODULE_SYSTEM,                                           //!< An error occurred in System module
    CYHAL_RSLT_MODULE_TIMER,                                            //!< An error occurred in Timer module
    CYHAL_RSLT_MODULE_TRNG,                                             //!< An error occurred in RNG module
    CYHAL_RSLT_MODULE_UART,                                             //!< An error occurred in UART module
    CYHAL_RSLT_MODULE_USB,                                              //!< An error occurred in USB module
    CYHAL_RSLT_MODULE_WDT,                                              //!< An error occurred in WDT module
    CYHAL_RSLT_MODULE_EZI2C,                                            //!< An error occurred in EZI2C module
};

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_modules */
