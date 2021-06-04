/***************************************************************************//**
* \file cyhal_hw_resources.h
*
* \brief
* Provides struct definitions for configuration resources in the PDL.
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

/**
* \addtogroup group_hal_impl_hw_types
* \ingroup group_hal_impl
* \{
*/

#pragma once

#include "cy_pdl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* NOTE: Any changes made to this enum must also be made to the hardware manager resource tracking */
/** Resource types that the hardware manager supports */
typedef enum
{
    CYHAL_RSC_ADC,       /*!< Analog to digital converter */
    CYHAL_RSC_BLESS,     /*!< Bluetooth communications block */
    CYHAL_RSC_CAN,       /*!< CAN communication block */
    CYHAL_RSC_CLKPATH,   /*!< Clock Path. DEPRECATED. */
    CYHAL_RSC_CLOCK,     /*!< Clock */
    CYHAL_RSC_CRYPTO,    /*!< Crypto hardware accelerator */
    CYHAL_RSC_DAC,       /*!< Digital to analog converter */
    CYHAL_RSC_DMA,       /*!< DMA controller */
    CYHAL_RSC_DW,        /*!< Datawire DMA controller */
    CYHAL_RSC_GPIO,      /*!< General purpose I/O pin */
    CYHAL_RSC_I2S,       /*!< I2S communications block */
    CYHAL_RSC_LCD,       /*!< Segment LCD controller */
    CYHAL_RSC_LPCOMP,    /*!< Low power comparator */
    CYHAL_RSC_LPTIMER,   /*!< Low power timer */
    CYHAL_RSC_OPAMP,     /*!< Opamp */
    CYHAL_RSC_PDM,       /*!< PCM/PDM communications block */
    CYHAL_RSC_SMIF,      /*!< Quad-SPI communications block */
    CYHAL_RSC_RTC,       /*!< Real time clock */
    CYHAL_RSC_SCB,       /*!< Serial Communications Block */
    CYHAL_RSC_SDHC,      /*!< SD Host Controller */
    CYHAL_RSC_TCPWM,     /*!< Timer/Counter/PWM block */
    CYHAL_RSC_UDB,       /*!< UDB Array */
    CYHAL_RSC_USB,       /*!< USB communication block */
    CYHAL_RSC_INVALID,   /*!< Placeholder for invalid type */
} cyhal_resource_t;

/* NOTE: Any changes here must also be made in cyhal_hwmgr.c */
/** Enum for the different types of clocks that exist on the device. */
typedef enum
{
    // The first four items are here for backwards compatability with old clock APIs
    CYHAL_CLOCK_BLOCK_PERIPHERAL_8BIT = CY_SYSCLK_DIV_8_BIT,        /*!< 8bit Peripheral Divider */
    CYHAL_CLOCK_BLOCK_PERIPHERAL_16BIT = CY_SYSCLK_DIV_16_BIT,      /*!< 16bit Peripheral Divider */
    CYHAL_CLOCK_BLOCK_PERIPHERAL_16_5BIT = CY_SYSCLK_DIV_16_5_BIT,  /*!< 16.5bit Peripheral Divider */
    CYHAL_CLOCK_BLOCK_PERIPHERAL_24_5BIT = CY_SYSCLK_DIV_24_5_BIT,  /*!< 24.5bit Peripheral Divider */

    CYHAL_CLOCK_BLOCK_IMO,                                          /*!< Internal Main Oscillator Input Clock */
    CYHAL_CLOCK_BLOCK_ECO,                                          /*!< External Crystal Oscillator Input Clock */
    CYHAL_CLOCK_BLOCK_EXT,                                          /*!< External Input Clock */
    CYHAL_CLOCK_BLOCK_ALTHF,                                        /*!< Alternate High Frequency Input Clock */
    CYHAL_CLOCK_BLOCK_ALTLF,                                        /*!< Alternate Low Frequency Input Clock */
    CYHAL_CLOCK_BLOCK_ILO,                                          /*!< Internal Low Speed Oscillator Input Clock */
    CYHAL_CLOCK_BLOCK_PILO,                                         /*!< Precision ILO Input Clock */
    CYHAL_CLOCK_BLOCK_WCO,                                          /*!< Watch Crystal Oscillator Input Clock */
    CYHAL_CLOCK_BLOCK_MFO,                                          /*!< Medium Frequency Oscillator Clock */

    CYHAL_CLOCK_BLOCK_PATHMUX,                                      /*!< Path selection mux for input to FLL/PLLs */

    CYHAL_CLOCK_BLOCK_FLL,                                          /*!< Frequency-Locked Loop Clock */
    CYHAL_CLOCK_BLOCK_PLL,                                          /*!< Phase-Locked Loop Clock */

    CYHAL_CLOCK_BLOCK_LF,                                           /*!< Low Frequency Clock */
    CYHAL_CLOCK_BLOCK_MF,                                           /*!< Medium Frequency Clock */
    CYHAL_CLOCK_BLOCK_HF,                                           /*!< High Frequency Clock */

    CYHAL_CLOCK_BLOCK_PUMP,                                         /*!< Analog Pump Clock */
    CYHAL_CLOCK_BLOCK_BAK,                                          /*!< Backup Power Domain Clock */
    CYHAL_CLOCK_BLOCK_TIMER,                                        /*!< Timer Clock */
    CYHAL_CLOCK_BLOCK_ALT_SYS_TICK,                                 /*!< Alternative SysTick Clock */

    CYHAL_CLOCK_BLOCK_FAST,                                         /*!< Fast Clock for CM4 */
    CYHAL_CLOCK_BLOCK_PERI,                                         /*!< Peripheral Clock */
    CYHAL_CLOCK_BLOCK_SLOW,                                         /*!< Slow Clock for CM0+ */
} cyhal_clock_block_t;

/** @brief Clock object
  * Application code should not rely on the specific contents of this struct.
  * They are considered an implementation detail which is subject to change
  * between platforms and/or HAL releases. */
typedef struct
{
    //For backwards compatibility with cyhal_clock_divider_t only. Do not use going forward.
    cy_en_divider_types_t   div_type;                /*!< Deprecated */
    uint8_t                 div_num;                 /*!< Deprecated */
    //End BWC items
    cyhal_clock_block_t     block;
    uint8_t                 channel;
    bool                    reserved;
} cyhal_clock_t;

/**
  * @brief Represents a particular instance of a resource on the chip.
  * Application code should not rely on the specific contents of this struct.
  * They are considered an implementation detail which is subject to change
  * between platforms and/or HAL releases.
  */
typedef struct
{
    cyhal_resource_t type;      //!< The resource block type
    uint8_t          block_num; //!< The resource block index
    /**
      * The channel number, if the resource type defines multiple channels
      * per block instance. Otherwise, 0 */
    uint8_t          channel_num;
} cyhal_resource_inst_t;

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_impl_hw_types */
