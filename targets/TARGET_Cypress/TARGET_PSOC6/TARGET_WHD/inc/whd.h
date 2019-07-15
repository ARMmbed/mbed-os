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

/** @file whd.h
 *  Provides abstract pointer type to act as instance for: driver, interface, buffer funcs, network funcs, resource funcs and bus funcs.
 */

#include "whd_types.h"

#ifndef INCLUDED_WHD_H
#define INCLUDED_WHD_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Abstract pointer type that acts as a handle to an instance of the driver
 */
typedef struct whd_driver *whd_driver_t;

/**
 * Abstract pointer type to handle instance of whd interface
 */
typedef struct whd_interface *whd_interface_t;

/**
 * Abstract type that acts as a handle to an instance of a buffer function
 */
typedef struct whd_buffer_funcs whd_buffer_funcs_t;

/**
 * Abstract type that acts as a handle to an instance of a network interface function
 */
typedef struct whd_netif_funcs whd_netif_funcs_t;

/**
 * Abstract type that acts as a handle to an instance of a resource function
 */
typedef struct whd_resource_source whd_resource_source_t;

/**
 * Abstract type that acts as a handle to an instance of a bus function used for SDIO specific functionality
 */
typedef struct whd_bus_funcs whd_sdio_funcs_t;

/**
 * Abstract type that acts as a handle to an instance of a bus function used for SPI specific functionality
 */
typedef struct whd_bus_funcs whd_spi_funcs_t;

/**
 * Structure for storing WHD init configurations
 */
typedef struct whd_init_config
{
    void *thread_stack_start;   /**< Pointer to the WHD thread stack  */
    uint32_t thread_stack_size; /**< Size of the WHD thread stack  */
    uint32_t thread_priority;   /**< Priority to be set to WHD Thread */
    whd_country_code_t country; /**< Variable to strore country code information */
} whd_init_config_t;

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* INCLUDED_WHD_H */

