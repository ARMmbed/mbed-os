/*
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
 */

/** @file
 *  Provides SCL functionality to communicate with Network Processor
 */

#include "scl_common.h"
#include "cy_device.h"
#include "cy_sysint.h"
#include "cy_ipc_drv.h"
#include "scl_wifi_api.h"
#include "ip4_addr.h"

#ifndef INCLUDED_SCL_IPC_H
#define INCLUDED_SCL_IPC_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/
/**
 * Hardware address of IPC_DATA0 register
 */
#define REG_IPC_STRUCT_DATA0(base)             (((IPC_STRUCT_V2_Type*)(base))->DATA0)
/**
 * Default timeout value (in ms) for SCL operations
 */
#define TIMER_DEFAULT_VALUE                    (100)
/**
 * Default timeout value (in ms) for Wi-Fi on
 */
#define WIFI_ON_TIMEOUT                        (5000)
/**
 * Default timeout value (in seconds) for Wi-Fi connection
 */
#define NW_CONNECT_TIMEOUT                     (30)
/**
 * Default interval (in micro seconds) for polling the Network Processor
 */
#define NW_DELAY_TIME_US                       (3000000)
/**
 * Default parameter length
 */
#define PARAM_LEN                              (20)

/******************************************************
*               Variables
******************************************************/
/**
 * Network parameters structure.
 */
typedef struct network_params {
    char ip_address[PARAM_LEN]; /**< IP address */
    char netmask[PARAM_LEN]; /**< Netmask */
    char gateway[PARAM_LEN]; /**< Gateway */
    int  connection_status; /**< Connection status */
} network_params_t;

/******************************************************
*             Function Declarations
******************************************************/

/** @addtogroup communication SCL communication API
 *  APIs for communicating with Network Processor
 *  @{
 */

/** Initializes the SCL thread and necessary artifacts
 *
 *  @return SCL_SUCCESS on successful initialization or SCL_ERROR otherwise
 */
extern scl_result_t scl_init(void);

/** Sends the SCL data and respective command to Network Processor
 *
 *  @param index           Index of the command.
 *  @param buffer          Data to be sent.
 *  @param timeout         The maximum time (in ms) to wait for the Network Processor to release IPC channel.
 *
 *  @return SCL_SUCCESS on successful communication within SCL timeout duration or SCL_ERROR
 */
extern scl_result_t scl_send_data(int index, char *buffer, uint32_t timeout);

/** Terminates the SCL thread and disables the interrupts
 *
 *  @return SCL_SUCCESS on successful termination of SCL thread and disabling of interrupts or SCL_ERROR on timeout
 */
extern scl_result_t scl_end(void);

/** Gets the network parameters like IP Address, Netmask, and Gateway from Network Processor
 *
 *  @param  nw_param      structure pointer of type @a network_params_t
 *
 *  @return SCL_SUCCESS on successful communication or SCL_ERROR
 */
extern scl_result_t scl_get_nw_parameters(network_params_t *nw_param);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* ifndef INCLUDED_SCL_IPC_H */
