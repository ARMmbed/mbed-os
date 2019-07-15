/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 */

/** @file whd_bus_types.h
 *  Defines common data types used with WHD SDIO/SPI bus
 *
 */

#ifndef INCLUDED_WHD_BUS_TYPES_H
#define INCLUDED_WHD_BUS_TYPES_H

#include "whd.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup busapi WHD Bus API
 * Allows WHD to operate with specific SDIO/SPI bus
 *  @{
 */

/**
 * WiFi device GPIO pin selections
 */
typedef enum whd_dev_gpio_sel
{
    WHD_DEV_GPIO_SEL_GPIO0 = 0, /**< Select GPIO0 */
} whd_dev_gpio_sel_t;

/**
 * Interrupt configuration structure
 */
typedef struct whd_intr_config
{
    whd_dev_gpio_sel_t dev_gpio_sel; /**< WiFi device-side GPIO pin selection */
    whd_bool_t is_falling_edge;      /**< Interrupt trigger (polarity) */
} whd_intr_config_t;

/**
 * Prototype for a user-defined function to enable interrupt to be signaled.
 *
 * This function must be provided whenever @ref WHD_BUS_SDIO_OOB_INTR is set.
 *
 * WHD will call this function to enable or disable an interrupt depending on the enable bool variable. When the interrupt
 * is asserted, the IRQ handler must call the appropriate "asserted" function with the WHD
 * context pointer associated with the IRQ.
 *
 *  @param  whd_drv          Pointer to handle instance of the driver
 *  @param  intr             Opaque interrupt signal
 *  @param  enable           WHD_TRUE means to enable the interrupt, otherwise disable.
 */
typedef void (*whd_enable_intr_func_t)(whd_driver_t whd_drv, const whd_variant_t intr, whd_bool_t enable);

/**
 * Prototype for a user-defined function to get interrupt configuration.
 *
 * WHD calls this function to get an interrupt's configuration.
 *
 * This function must be provided whenever @ref WHD_BUS_SDIO_OOB_INTR is set.
 *
 *  @param  whd_driver   Pointer to handle instance of the driver
 *  @param  intr         Opaque interrupt signal
 *  @param  config       Pointer to the interrupt configuration
 */
typedef void (*whd_get_intr_config_func_t)(whd_driver_t whd_driver, const whd_variant_t intr,
                                           whd_intr_config_t *config);

/**
 * Structure of function pointers for bus operations.
 */
struct whd_bus_funcs
{
    whd_enable_intr_func_t whd_enable_intr;          /**< Enable interrupt to be signaled */
    whd_get_intr_config_func_t whd_get_intr_config;  /**< Get interrupt configuration */
};
/* @} */

#ifdef __cplusplus
} /*extern "C" */
#endif
#endif /* INCLUDED_WHD_BUS_TYPES_H */

