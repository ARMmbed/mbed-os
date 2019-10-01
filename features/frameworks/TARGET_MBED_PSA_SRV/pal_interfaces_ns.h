/** @file
 * Copyright (c) 2018-2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/
#ifndef _PAL_INTERFACES_NS_H_
#define _PAL_INTERFACES_NS_H_

#include "val.h"
#include "val_client_defs.h"
#include <stdarg.h>

/**
    @brief    - This function will read peripherals using SPI commands
    @param    - addr : address of the peripheral
              - data : read buffer
              - len  : length of the read buffer in bytes
    @return   - error status
**/
int pal_spi_read(addr_t addr, uint8_t *data, uint32_t len);

/**
 * @brief    - Retrieve the version of the PSA Framework API that is implemented.
 *             This is a wrapper API for psa_framework_version API.
 * @param    - void
 * @return   - The PSA Framework API version.
 *             Note - Return PAL_STATUS_ERROR if PSA IPC is not implemented.
 */
uint32_t pal_ipc_framework_version(void);

/**
 * @brief    - Retrieve the minor version of a Root of Trust Service by its SID.
 *             This is a wrapper API for the psa_version API.
 * @param    - sid The Root of Trust Service ID
 * @return   - Minor version of Root of Trust Service or PSA_VERSION_NONE if Root of Trust Service
 *             not present on the system.
 *             Note - Return PAL_STATUS_ERROR if PSA IPC is not implemented.
 */
uint32_t pal_ipc_version(uint32_t sid);

/**
 * @brief   - Connect to given sid.
 *            This is a wrapper API for the psa_connect API.
 * @param   - sid : RoT service id
 *          - minor_version : minor_version of RoT service
 * @return  - psa_handle_t : return connection handle
 *            Note - Return PSA_NULL_HANDLE if PSA IPC is not implemented.
 */
psa_handle_t pal_ipc_connect(uint32_t sid, uint32_t minor_version);

/**
 * @brief  - Call a connected Root of Trust Service.
 *          This is a wrapper API for the psa_call API. The caller must provide an array of
 *          psa_invec_t structures as the input payload.
 * @param  - handle:   Handle for the connection.
 *         - in_vec:   Array of psa_invec structures.
 *         - in_len:   Number of psa_invec structures in in_vec.
 *         - out_vec:  Array of psa_outvec structures for optional Root of Trust Service response.
 *         - out_len:  Number of psa_outvec structures in out_vec.
 * @return - psa_status_t
 */

psa_status_t pal_ipc_call(psa_handle_t handle,
                         const psa_invec *in_vec,
                         size_t in_len,
                         psa_outvec *out_vec,
                         size_t out_len);

/**
 * @brief - Close a connection to a Root of Trust Service.
 *          This is a wrapper API for the psa_close API.
 *          Sends the PSA_IPC_DISCONNECT message to the Root of Trust Service
 *          so it can clean up resources.
 * @param - handle Handle for the connection.
 * @return - void
 */
void pal_ipc_close(psa_handle_t handle);
/**
    @brief    - This function initializes the UART
    @param    - uart base addr
    @return   - SUCCESS/FAILURE
**/
int pal_uart_init_ns(uint32_t uart_base_addr);

/**
 *   @brief    - This function parses the input string and writes bytes into UART TX FIFO
 *   @param    - str      : Input String
 *             - data     : Value for format specifier
 *   @return   - SUCCESS/FAILURE
**/

int pal_print_ns(char *str, uint32_t data);

/**
 *   @brief           - Initializes an hardware watchdog timer
 *   @param           - base_addr       : Base address of the watchdog module
 *                    - time_us         : Time in micro seconds
 *                    - timer_tick_us   : Number of ticks per micro second
 *   @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_init_ns(addr_t base_addr, uint32_t time_us, uint32_t timer_tick_us);

/**
 *   @brief           - Enables a hardware watchdog timer
 *   @param           - base_addr       : Base address of the watchdog module
 *   @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_enable_ns(addr_t base_addr);

/**
 *   @brief           - Disables a hardware watchdog timer
 *   @param           - base_addr  : Base address of the watchdog module
 *   @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_disable_ns(addr_t base_addr);

/**
 *   @brief    - Reads from given non-volatile address.
 *   @param    - base    : Base address of nvmem
 *               offset  : Offset
 *               buffer  : Pointer to source address
 *               size    : Number of bytes
 *   @return   - SUCCESS/FAILURE
**/
int pal_nvmem_read_ns(addr_t base, uint32_t offset, void *buffer, int size);

/**
 *   @brief    - Writes into given non-volatile address.
 *   @param    - base    : Base address of nvmem
 *               offset  : Offset
 *               buffer  : Pointer to source address
 *               size    : Number of bytes
 *   @return   - SUCCESS/FAILURE
**/
int pal_nvmem_write_ns(addr_t base, uint32_t offset, void *buffer, int size);

/**
 *   @brief    - This API will call the requested crypto function
 *   @param    - type    : function code
 *               valist  : variable argument list
 *   @return   - error status
**/
int32_t pal_crypto_function(int type, va_list valist);

/**
 *   @brief    - This API will call the requested internal trusted storage function
 *   @param    - type    : function code
 *               valist  : variable argument list
 *   @return   - error status
**/
uint32_t pal_its_function(int type, va_list valist);

/**
 *   @brief    - This API will call the requested protected storage function
 *   @param    - type    : function code
 *               valist  : variable argument list
 *   @return   - error status
**/
uint32_t pal_ps_function(int type, va_list valist);

/**
 *   @brief    - This API will call the requested attestation function
 *   @param    - type    : function code
 *               valist  : variable argument list
 *   @return   - error status
**/
int32_t pal_attestation_function(int type, va_list valist);

/**
 *   @brief    - Terminates the simulation at the end of all tests completion.
 *               By default, it put cpus into power down mode.
 *   @param    - void
 *   @return   - void
**/
void pal_terminate_simulation(void);
#endif
