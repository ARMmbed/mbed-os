/***************************************************************************//**
* \file cyhal_i2c.h
*
* \brief
* Provides a high level interface for interacting with the Cypress I2C.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
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
* \addtogroup group_hal_i2c I2C (Inter-Integrated Circuit)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress I2C.
* 
* \defgroup group_hal_i2c_master Master
* \defgroup group_hal_i2c_slave Slave
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"
#include "cyhal_modules.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** The requested resource type is invalid */
#define CYHAL_I2C_RSLT_ERR_INVALID_PIN (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_I2C, 0))
/** Can not reach desired data rate */
#define CYHAL_I2C_RSLT_ERR_CAN_NOT_REACH_DR (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_I2C, 1))
/** Address size is not correct, should be 1 or two */
#define CYHAL_I2C_RSLT_ERR_INVALID_ADDRESS_SIZE (CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CYHAL_RSLT_MODULE_I2C, 2))

/** Enum to enable/disable/report interrupt cause flags. */
typedef enum
{
    CYHAL_I2C_EVENT_NONE               = 0,       /* No event */
    CYHAL_I2C_SLAVE_READ_EVENT         = 1 << 1,  /* Indicates that the slave was addressed and the master wants to read data. */
    CYHAL_I2C_SLAVE_WRITE_EVENT        = 1 << 2,  /* Indicates that the slave was addressed and the master wants to write data. */
    CYHAL_I2C_SLAVE_RD_IN_FIFO_EVENT   = 1 << 3,  /* All slave data from the configured Read buffer has been loaded into the TX FIFO. */
    CYHAL_I2C_SLAVE_RD_BUF_EMPTY_EVENT = 1 << 4,  /* The master has read all data out of the configured Read buffer. */
    CYHAL_I2C_SLAVE_RD_CMPLT_EVENT     = 1 << 5,  /* Indicates the master completed reading from the slave (set by the master NAK or Stop) */
    CYHAL_I2C_SLAVE_WR_CMPLT_EVENT     = 1 << 6,  /* Indicates the master completed writing to the slave (set by the master Stop or Restart)*/
    CYHAL_I2C_SLAVE_ERR_EVENT          = 1 << 7,  /* Indicates the I2C hardware detected an error. */
    CYHAL_I2C_MASTER_WR_IN_FIFO_EVENT  = 1 << 17, /* All data specified by Cy_SCB_I2C_MasterWrite has been loaded into the TX FIFO. */
    CYHAL_I2C_MASTER_WR_CMPLT_EVENT    = 1 << 18, /* The master write started by Cy_SCB_I2C_MasterWrite is complete.*/
    CYHAL_I2C_MASTER_RD_CMPLT_EVENT    = 1 << 19, /* The master read started by Cy_SCB_I2C_MasterRead is complete.*/
    CYHAL_I2C_MASTER_ERR_EVENT         = 1 << 20, /* Indicates the I2C hardware has detected an error. */
} cyhal_i2c_event_t;

/** Handler for I2C events */
typedef void (*cyhal_i2c_event_callback_t)(void *callback_arg, cyhal_i2c_event_t event);

/** @brief Initial I2C configuration */
typedef struct
{
    bool is_slave;            /* I2C mode, is the device a master or slave */
    uint16_t address;         /* Address of this slave device (7-bit), should be set to 0 for master */
    uint32_t frequencyhal_hz; /* Frequency that the I2C bus runs at */
} cyhal_i2c_cfg_t;

/** Initialize the I2C peripheral, and configures its specifieds pins. By default
 * it is setup as a Master running at 400kHz. This can be changed by calling
 * cyhal_i2c_configure().
 * NOTE: Master/Slave specific functions only work when the block is configured
 * to be in that mode.
 *
 * @param[out] obj The I2C object
 * @param[in]  sda The sda pin
 * @param[in]  scl The scl pin
 * @param[in]  clk The clock to use can be shared, if not provided a new clock will be allocated
 * @return The status of the init request
 */
cy_rslt_t cyhal_i2c_init(cyhal_i2c_t *obj, cyhal_gpio_t sda, cyhal_gpio_t scl, const cyhal_clock_divider_t *clk);

/** Deinitialize the i2c object
 *
 * @param[in,out] obj The i2c object
 */
void cyhal_i2c_free(cyhal_i2c_t *obj);

/** Configure the I2C block
 *
 * @param[in] obj     The I2C object
 * @param[in] cfg     Configuration settings to apply
 * @return The status of the configure request
 */
cy_rslt_t cyhal_i2c_configure(cyhal_i2c_t *obj, const cyhal_i2c_cfg_t *cfg);

/**
* \addtogroup group_hal_i2c_master
* \{
*/

/**
 * I2C master write
 *
 * @param[in]  obj        The I2C object
 * @param[in]  dev_addr   device address (7-bit)
 * @param[in]  data       i2c send data
 * @param[in]  size       i2c send data size
 * @param[in]  timeout    timeout in milisecond, set this value to 0 if you want to wait forever
 * @param[in]  send_stop  whether the stop should be send, used to support repeat start conditions
 *
 * @return The status of the master_write request
 */
cy_rslt_t cyhal_i2c_master_write(cyhal_i2c_t *obj, uint16_t dev_addr, const uint8_t *data, uint16_t size, uint32_t timeout, bool send_stop);

/**
 * I2C master read
 *
 * @param[in]   obj        The I2C object
 * @param[in]   dev_addr   device address (7-bit)
 * @param[out]  data       i2c receive data
 * @param[in]   size       i2c receive data size
 * @param[in]   timeout    timeout in milisecond, set this value to 0 if you want to wait forever
 * @param[in]   send_stop  whether the stop should be send, used to support repeat start conditions
 *
 * @return The status of the master_read request
 */
cy_rslt_t cyhal_i2c_master_read(cyhal_i2c_t *obj, uint16_t dev_addr, uint8_t *data, uint16_t size, uint32_t timeout, bool send_stop);

/** \} group_hal_i2c_master */

/**
* \addtogroup group_hal_i2c_slave
* \{
*/

/**
 * I2C slave config write buffer
 * The user needs to setup a new buffer every time (i.e. call slave_send and slave_recv every time the buffer has been used up)
 *
 * @param[in]  obj      The I2C object
 * @param[in]  data     i2c slave send data
 * @param[in]  size     i2c slave send data size
 *
 * @return The status of the slave_config_write_buff request
 */
cy_rslt_t cyhal_i2c_slave_config_write_buff(cyhal_i2c_t *obj, const uint8_t *data, uint16_t size);

/**
 * I2C slave config read buffer
 * The user needs to setup a new buffer every time (i.e. call slave_send and slave_recv every time the buffer has been used up)
 *
 * @param[in]   obj      The I2C object
 * @param[out]  data     i2c slave receive data
 * @param[in]   size     i2c slave receive data size
 *
 * @return The status of the slave_config_read_buff request
 */
cy_rslt_t cyhal_i2c_slave_config_read_buff(cyhal_i2c_t *obj, uint8_t *data, uint16_t size);

/** \} group_hal_i2c_slave */

/**
* \addtogroup group_hal_i2c_master
* \{
*/

/** Perform an i2c write using a block of data stored at the specified memory location
 *
 * @param[in]  obj            The I2C object
 * @param[in]  address        device address (7-bit)
 * @param[in]  mem_addr       mem address to store the written data
 * @param[in]  mem_addr_size  number of bytes in the mem address
 * @param[in]  data           i2c master send data
 * @param[in]  size           i2c master send data size
 * @param[in]  timeout        timeout in milisecond, set this value to 0 if you want to wait forever
 * @return The status of the write request
 */
cy_rslt_t cyhal_i2c_master_mem_write(cyhal_i2c_t *obj, uint16_t address, uint16_t mem_addr, uint16_t mem_addr_size, const uint8_t *data, uint16_t size, uint32_t timeout);

/** Perform an i2c read using a block of data stored at the specified memory location
 *
 * @param[in]  obj            The I2C object
 * @param[in]  address        device address (7-bit)
 * @param[in]  mem_addr       mem address to store the written data
 * @param[in]  mem_addr_size  number of bytes in the mem address
 * @param[out] data           i2c master send data
 * @param[in]  size           i2c master send data size
 * @param[in]  timeout        timeout in milisecond, set this value to 0 if you want to wait forever
 * @return The status of the read request
 */
cy_rslt_t cyhal_i2c_master_mem_read(cyhal_i2c_t *obj, uint16_t address, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *data, uint16_t size, uint32_t timeout);

/** Start I2C master asynchronous transfer
 *
 * @param[in]  obj      The I2C object
 * @param[in]  address  device address (7-bit)
 * @param[in]  tx       The transmit buffer
 * @param[in]  tx_size  The number of bytes to transmit
 * @param[out] rx       The receive buffer
 * @param[in]  rx_size  The number of bytes to receive
 * @return The status of the master_transfer_async request
 */
cy_rslt_t cyhal_i2c_master_transfer_async(cyhal_i2c_t *obj, uint16_t address, const void *tx, size_t tx_size, void *rx, size_t rx_size);

/** \} group_hal_i2c_master */

/** Abort asynchronous transfer
 *
 * This function does not perform any check - that should happen in upper layers.
 * @param[in] obj The I2C object
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_i2c_abort_async(cyhal_i2c_t *obj);

/** The I2C event callback handler registration
 *
 * @param[in] obj          The I2C object
 * @param[in] callback     The callback handler which will be invoked when an event triggers
 * @param[in] callback_arg Generic argument that will be provided to the callback when called
 */
void cyhal_i2c_register_callback(cyhal_i2c_t *obj, cyhal_i2c_event_callback_t callback, void *callback_arg);

/** Configure and Enable or Disable I2C Interrupt.
 *
 * @param[in] obj           The I2C object
 * @param[in] event         The I2C event type
 * @param[in] intrPriority  The priority for NVIC interrupt events
 * @param[in] enable        True to turn on interrupts, False to turn off
 */
void cyhal_i2c_enable_event(cyhal_i2c_t *obj, cyhal_i2c_event_t event, uint8_t intrPriority, bool enable);

/*******************************************************************************
* Backward compatibility macro. The following code is DEPRECATED and must 
* not be used in new projects
*******************************************************************************/
/** \cond INTERNAL */
#define cyhal_i2c_register_irq        cyhal_i2c_register_callback
#define cyhal_i2c_irq_enable(obj, event, enable)          cyhal_i2c_enable_event(obj, event, CYHAL_ISR_PRIORITY_DEFAULT, enable)
typedef cyhal_i2c_event_t       cyhal_i2c_irq_event_t;
/** \endcond */

#if defined(__cplusplus)
}
#endif

#ifdef CYHAL_I2C_IMPL_HEADER
#include CYHAL_I2C_IMPL_HEADER
#endif /* CYHAL_I2C_IMPL_HEADER */

/** \} group_hal_i2c */
