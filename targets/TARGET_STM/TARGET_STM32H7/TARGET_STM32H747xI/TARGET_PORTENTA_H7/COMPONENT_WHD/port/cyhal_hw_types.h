/***************************************************************************//**
* \file cyhal_hw_types_template.h
*
* \brief
* Provides a template for configuration resources used by the HAL. Items
* here need to be implemented for each HAL port. It is up to the environment
* being ported into what the actual types are. There are some suggestions below
* but these are not required. All that is required is that the type is defined;
* it does not matter to the HAL what type is actually chosen for the
* implementation
* All TODOs and references to 'PORT' need to be replaced by with meaningful
* values for the device being supported.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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
 * \addtogroup group_hal_hw_types PORT Hardware Types
 * \ingroup group_hal_PORT
 * \{
 * Struct definitions for configuration resources in the PORT.
 *
 * \defgroup group_hal_hw_types_data_structures Data Structures
 */

#pragma once
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "PinNames.h"

/*
 #include "TODO: Port specific header file"
 */

#ifdef __cplusplus
extern "C" {
#endif


#ifndef CYHAL_ISR_PRIORITY_DEFAULT
/** Priority that is applied by default to all drivers when initalized. Priorities can be
 * overridden on each driver as part of enabling events.
 */
#define CYHAL_ISR_PRIORITY_DEFAULT  (7)
#endif

/**
 * \addtogroup group_hal_hw_types_data_structures
 * \{
 */

typedef int32_t  cyhal_gpio_t;

/** GPIO object */
typedef struct
{
  GPIO_TypeDef      *port;
  GPIO_InitTypeDef  config;
} pinconfig_t;

/** Clock divider object */
typedef struct
{
    /* TODO: replace with port specific items */
    void *div_type;
} cyhal_clock_divider_t;

/** Clock object */
typedef struct
{
    //For backwards compatibility with cyhal_clock_divider_t only. Do not use going forward.
    int                     div_type;                /*!< Deprecated */
    uint8_t                 div_num;                 /*!< Deprecated */
    //End BWC items
    int                     block;
    uint8_t                 channel;
    bool                    reserved;
} cyhal_clock_t;

/** SDIO object */
typedef   void*  *cyhal_sdio_t;

/** SPI object */
typedef struct
{
    /* TODO: replace with port specific items */
    void *empty;
} cyhal_spi_t;


#include "whd_debug.h"

#if defined(WPRINT_ENABLE_WHD_ERROR) || defined(WPRINT_ENABLE_WHD_INFO) || defined(WPRINT_ENABLE_WHD_DEBUG)
/** \} group_hal_hw_types_data_structures */
#define PRINTF(...)   do { \
                      (void) printf(__VA_ARGS__); \
                      } while (0)
#else
#define PRINTF(...)
#endif

#include "whd_config.h"

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/** \} group_hal_hw_types */
