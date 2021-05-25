/***************************************************************************//**
* \file cy_i3c.h
* \version 1.00
*
* Provides I3C API declarations for the I3C Controller.
*
********************************************************************************
* \copyright
* Copyright 2019-2020 Cypress Semiconductor Corporation
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
* \addtogroup group_cy_i3c
* \{
* Driver API for I3C Bus Peripheral
*
* I3C - The Improved Inter-Integrated Circuit (I3C) bus is a MIPI industry-standard.
*
* The functions and other declarations used in this part of the driver are in 
* cy_i3c.h. You can also include cy_pdl.h to get access to all functions and declarations 
* in the PDL.
*
* The I3C peripheral driver provides an API to implement I3C in Slave, Main-Master,
* or Secondary-Master roles based on the DW I3C hardware block.
* This I3C bus is capable of supporting I3C as well as I2C slave devices as defined
* in the specification. 
* I2C slaves are supported with Fast-mode, and Fast-mode Plus with external 50ns
* spike filter as per I3C specification.
*
* Features:
* * An industry-standard I3C bus interface
* * Supports slave, main master, and secondary master operation
* * SCL frequency up to 12.5MHz (SDR data rate 12.5Mbps, HDR-DDR data rate at 25Mbps)
* * In master mode, number of addressable slave devices is 11
* * Works in active power mode only
* * Does not support I3C HDR-TSP/TSR modes
* * Does not support I3C CDR (clock data recovery)
* * Does not support I3C address arbitration optimizations
* * Does not support I3C IBI with data
* * Does not support I3C time stamping
* * Does not support vendor specific CCCs
* * Does not support I2C slave role
*
********************************************************************************
* \section group_cy_i3c_configuration Configuration Considerations
********************************************************************************
* The I3C driver configuration can be divided to number of sequential
* steps listed below:
* * \ref group_i3c_config
* * \ref group_i3c_pins
* * \ref group_i3c_clock
* * \ref group_i3c_data_rate
* * \ref group_i3c_intr
* * \ref group_i3c_enable
*
********************************************************************************
* \subsection group_i3c_config Configure i3c
********************************************************************************
* To set up the I3C  driver, provide the configuration parameters in the
* \ref cy_stc_i3c_config_t structure. Provide i3cMode to the select
* operation mode slave, master or secondary master-slave.
* To initialize the driver, call \ref Cy_I3C_Init function providing a 
* pointer to the populated \ref cy_stc_i3c_config_t
* structure and the allocated \ref cy_stc_i3c_context_t structure.
*
* For I3C slave setup read and write buffer before enabling its
* operation using \ref Cy_I3C_SlaveConfigReadBuf and \ref
* Cy_I3C_SlaveConfigWriteBuf appropriately. Note that the master reads
* data from the slave read buffer and writes data into the slave write buffer.
*
********************************************************************************
* \subsection group_i3c_pins Assign and Configure Pins
********************************************************************************
* Only dedicated  pins can be used for I3C operation. The HSIOM
* register must be configured to connect dedicated I3C pins to the 
* block. Also the open-drain enable pin for I3C must be configured in open-drain
* with internal pull up enabled
* (this pin configuration implies usage of external pull-up resistors)
*
********************************************************************************
* \subsection group_i3c_clock Assign Clock Divider
********************************************************************************
* A clock source must be connected to the  block to oversample input and
* output signals, in this document this clock will be referred as clk_i3c.
* You must use one of the 8-bit or 16-bit dividers. Use the \ref group_sysclk 
* driver API to do this.
*
********************************************************************************
* \subsection group_i3c_data_rate Configure Data Rate
********************************************************************************
* To get I3C slave operation with the desired data rate, the clk_i3c must be
* fast enough to provide sufficient oversampling. Use the
* \ref group_sysclk driver API to do this.
*
* To get I3C master operation with the desired data rate, the source clock
* frequency and SCL low and high phase duration must be configured. Use the
* \ref group_sysclk driver API to configure source clock frequency. Then call
* \ref Cy_I3C_SetDataRate to set the SCL low, high phase duration 
* This function sets SCL low and high phase settings based on 
* source clock frequency.
*
********************************************************************************
* \subsection group_i3c_intr Configure Interrupt
********************************************************************************
* The interrupt is mandatory for I3C operation. 
* The driver provides three interrupt functions: \ref Cy_I3C_Interrupt,
* \ref Cy_I3C_SlaveInterrupt, and \ref Cy_I3C_MasterInterrupt. One of
* these functions must be called in the interrupt handler for the selected 
* instance. Call \ref Cy_I3C_SlaveInterrupt when I3C is configured to
* operate as a slave, \ref Cy_I3C_MasterInterrupt when I3C is configured
* to operate as a master and \ref Cy_I3C_Interrupt when I3C is configured
* to operate as master and slave. Using the slave- or master-specific interrupt
* function allows reducing the flash consumed by the I3C driver. Also this
* interrupt must be enabled in the NVIC otherwise it will not work.
* \note
* The I3C driver documentation refers to the \ref Cy_I3C_Interrupt function
* when interrupt processing is mandatory for the operation. This is done to
* simplify the readability of the driver's documentation. The application should
*  call the slave- or master-specific interrupt functions \ref Cy_I3C_SlaveInterrupt
* or \ref Cy_I3C_MasterInterrupt, when appropriate.
*
********************************************************************************
* \subsection group_i3c_enable Enable I3C
********************************************************************************
* Finally, enable the I3C operation by calling \ref Cy_I3C_Enable. Then I3C
* slave starts respond to the assigned address and I3C master ready to execute
* transfers.
*
********************************************************************************
* \section group_i3c_use_cases Common Use Cases
********************************************************************************
* \subsection group_i3c_master_mode Master Operation
********************************************************************************
* Call \ref Cy_I3C_MasterRead or \ref Cy_I3C_MasterWrite to
* communicate with the slave. These functions do not block and only start a
* transaction. After a transaction starts, the \ref Cy_I3C_Interrupt
* handles further data transaction until its completion (successfully or 
* with error occurring). To monitor the transaction,
* use \ref Cy_I3C_GetBusStatus or register callback function using
* \ref Cy_I3C_RegisterEventCallback to be notified about
* \ref group_i3c_macros_callback_events.
*
********************************************************************************
* \subsection group_i3c_slave Slave Operation
********************************************************************************
* Slave operation requires the \ref Cy_I3C_Interrupt be
* called inside the interrupt handler. The read and write buffers must
* be provided for the slave to enable communication with the master. Use
* \ref Cy_I3C_SlaveConfigReadBuf and \ref Cy_I3C_SlaveConfigWriteBuf
* for this purpose. Note that after transaction completion the buffer must be
* configured again. Otherwise, the same buffer is used starting from the point
* where the master stopped a previous transaction.
* For example: The read buffer is configured to be 10 bytes and the master reads
* 8 bytes. If the read buffer is not configured again, the next master read
* will start from the 9th byte.
* To monitor the transaction status, use \ref Cy_I3C_GetBusStatus or
* use \ref Cy_I3C_RegisterEventCallback to register a callback function
* to be notified about \ref group_i3c_macros_callback_events.
*
********************************************************************************
* \section group_i3c_lp Low Power Support
********************************************************************************
* The I3C driver provides callback functions to handle power mode transition.
* The callback \ref Cy_I3C_DeepSleepCallback must be called
* during execution of \ref Cy_SysPm_CpuEnterDeepSleep \ref Cy_I3C_HibernateCallback
* must be called during execution of \ref Cy_SysPm_SystemEnterHibernate. To trigger the
* callback execution, the callback must be registered before calling the
* power mode transition function. Refer to \ref group_syspm driver for more
* information about power mode transitions and callback registration.
*
* \defgroup group_i3c_macros Macros
* \defgroup group_i3c_enums Enumerated Types
* \defgroup group_i3c_data_structures Data Structures
* \defgroup group_i3c_functions Functions
* \{
* \defgroup group_i3c_general_functions General
* \defgroup group_i3c_slave_functions Slave
* \defgroup group_i3c_master_functions Master 
* \defgroup group_i3c_secondary_master_functions Secondary Master
* \defgroup group_i3c_interrupt_functions Interrupt
* \defgroup group_i3c_low_power_functions Low Power Callbacks
* \}
*/

#if !defined (CY_I3C_H)
#define CY_I3C_H

#include "cy_device_headers.h"
#include "cy_device.h"

#if defined (CY_IP_MXI3C)

#include "cy_syspm.h"
#include "cy_i3c_ccc.h"

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
 *                              Constants                                     *
 ******************************************************************************/

/**
* \addtogroup group_i3c_macros
* \{
*/

/** Driver major version */
#define CY_I3C_DRV_VERSION_MAJOR    (1)  //What should these be?

/** Driver minor version */
#define CY_I3C_DRV_VERSION_MINOR    (0)  //What should these be??

/** I3C driver identifier */
#define CY_I3C_ID           CY_PDL_DRV_ID(0x2AU) //TBD - What should be here?

/**
* \addtogroup group_i3c_macros_common_macros Common Macros
* \{
*/

/** Max device address value */
#define CY_I3C_MAX_ADDR CY_I3C_GENMASK(6, 0)

/** Max number of slave devices addressable in master mode */
#define CY_I3C_MAX_DEVS (11)

/** I3C Broadcast Address */
#define CY_I3C_BROADCAST_ADDR    (0x7E)

/** \} group_i3c_macros_common_macros */

/**
* \defgroup group_i3c_macros_intr_macros Interrupt Status
* \{
*/

/**
* The number of empty locations in the transmit buffer is greater than or 
* equal to the specified threshold value.
*/
#define CY_I3C_INTR_TX_BUFFER_THLD_STS I3C_CORE_INTR_STATUS_TX_THLD_STS_Msk

/**
* The number of elements in the receive buffer is greater than or 
* equal to the specified threshold value.
*/
#define CY_I3C_INTR_RX_BUFFER_THLD_STS I3C_CORE_INTR_STATUS_RX_THLD_STS_Msk

/**
* The number of elements in the IBI buffer is greater than or 
* equal to the specified threshold value.
*/
#define CY_I3C_INTR_IBI_BUFFER_THLD_STS I3C_CORE_INTR_STATUS_IBI_THLD_STS_Msk

/**
* The number of empty locations in the command queue is greater than or 
* equal to the specified threshold value.
*/
#define CY_I3C_INTR_CMD_QUEUE_READY_STS I3C_CORE_INTR_STATUS_CMD_QUEUE_READY_STS_Msk

/**
* The number of elements in the response queue is greater than or 
* equal to the specified threshold value.
*/
#define CY_I3C_INTR_RESP_READY_STS I3C_CORE_INTR_STATUS_RESP_READY_STS_Msk

/**
* The transfer is aborted.
*/
#define CY_I3C_INTR_TRANSFER_ABORT_STS I3C_CORE_INTR_STATUS_TRANSFER_ABORT_STS_Msk

/**
* The CCC registers are updated by the I3C Master through CCC commands.
*/
#define CY_I3C_INTR_CCC_UPDATED_STS I3C_CORE_INTR_STATUS_CCC_UPDATED_STS_Msk

/**
* The dynamic address of the device is assigned through SETDASA or ENTDAA CCC
*/
#define CY_I3C_INTR_DYN_ADDR_ASSGN_STS I3C_CORE_INTR_STATUS_DYN_ADDR_ASSGN_STS_Msk

/**
* Error occured in the transfer. 
*/
#define CY_I3C_INTR_TRANSFER_ERR_STS I3C_CORE_INTR_STATUS_TRANSFER_ERR_STS_Msk

/**
* DEFSLV CCC is received.
*/
#define CY_I3C_INTR_DEFSLV_STS I3C_CORE_INTR_STATUS_DEFSLV_STS_Msk

/**
* The read request is received from the current master; command queue is empty.
*/
#define CY_I3C_INTR_READ_REQ_RECV_STS I3C_CORE_INTR_STATUS_READ_REQ_RECV_STS_Msk

/**
* The IBI request initiated via SIR request register is addressed and status is updated.
*/
#define CY_I3C_INTR_IBI_UPDATED_STS I3C_CORE_INTR_STATUS_IBI_UPDATED_STS_Msk

/**
* The role of the controller is changed.
*/
#define CY_I3C_INTR_BUSOWNER_UPDATED_STS I3C_CORE_INTR_STATUS_BUSOWNER_UPDATED_STS_Msk

/**
* Provides the list of allowed sources for slave mode.
*/
#define CY_I3C_SLV_INTR_Msk (CY_I3C_INTR_CCC_UPDATED_STS | CY_I3C_INTR_DYN_ADDR_ASSGN_STS | \
                             CY_I3C_INTR_RESP_READY_STS | CY_I3C_INTR_READ_REQ_RECV_STS )
                             
/** \} group_i3c_macros_intr_macros */

/**
* \defgroup group_i3c_macros_master_status I3C Master Status
* Macros to check current I3C master status returned by 
* \ref Cy_I3C_GetBusStatus function. Each I3C master status is encoded 
* in a separate bit, therefore multiple bits may be set to indicate the 
* current status.
* \{
*/

/**
* The master is busy executing operation started by \ref Cy_I3C_SendCCCCmd or
* \ref Cy_I3C_MasterRead or \ref Cy_I3C_MasterWrite or \ref Cy_I3C_MasterStartEntDaa
* \ref Cy_I3C_MasterSendHdrCmds.
*/
#define CY_I3C_MASTER_BUSY                          (0x00010000UL)

/**
* The master has currently performed a broadcast CCC write transfer
*/
#define CY_I3C_MASTER_BROADCAST_CCC_WR_XFER         (0x00000001UL)

/**
* The master has currently performed a directed CCC write transfer
*/
#define CY_I3C_MASTER_DIRECTED_CCC_WR_XFER          (0x00000002UL)

/**
* The master has currently performed a broadcast CCC read transfer
*/
#define CY_I3C_MASTER_DIRECTED_CCC_RD_XFER          (0x00000004UL)

/**
* The master has currently performed DAA
*/
#define CY_I3C_MASTER_ENTDAA_XFER                   (0x00000008UL)

/**
* The master has currently performed a privete SDR write to an I3C Slave
*/
#define CY_I3C_MASTER_I3C_SDR_WR_XFER               (0x00000010UL)

/**
* The master has currently performed a privete SDR read from an I3C Slave
*/
#define CY_I3C_MASTER_I3C_SDR_RD_XFER               (0x00000020UL)

/**
* The master has currently performed a privete SDR read to an I2C Slave
*/
#define CY_I3C_MASTER_I2C_SDR_WR_XFER               (0x00000040UL)

/**
* The master has currently performed a privete SDR read from an I2C Slave
*/
#define CY_I3C_MASTER_I2C_SDR_RD_XFER               (0x00000080UL)

/**
* The master has currently performed a private HDR write in DDR mode
*/
#define CY_I3C_MASTER_HDR_DDR_WR_XFER               (0x00000100UL)

/**
* The master has currently performed a private HDR read in DDR mode
*/
#define CY_I3C_MASTER_HDR_DDR_RD_XFER               (0x00000200UL)

/**
* The master has currently responded to a IBI request
*/
#define CY_I3C_MASTER_IBI_XFER                      (0x00000400UL)

/**
* The master is in HALT state due to error in the transfer
*/
#define CY_I3C_MASTER_HALT_STATE                    (0x00000800UL) //This is the BUS_ERR - Ask

/**
* The transaction was aborted 
*/
#define CY_I3C_MASTER_XFER_ABORTED                  (0X00001000UL)

/** \} group_i3c_macros_master_status */

/**
* \defgroup group_i3c_macros_slave_status I3C Slave Status
* Macros to check current I3C slave status returned by 
* \ref Cy_I3C_GetBusStatus function. Each I3C slave status is encoded 
* in a separate bit, therefore multiple bits may be set to indicate the 
* current status.
* \{
*/

/**
* The master has finished reading data from the slave
*/
#define CY_I3C_SLAVE_RD_CMPLT      (0x00002000UL)

/**
* The master has finished writing data into the slave
*/
#define CY_I3C_SLAVE_WR_CMPLT      (0x00004000UL)

/** There is a read transaction in progress */
#define CY_I3C_SLAVE_RD_BUSY       (0x00008000UL)
 
/** There is a write transaction in progress */
#define CY_I3C_SLAVE_WR_BUSY       (0x00010000UL)

/** \} group_i3c_macros_slave_status */

/**
* \defgroup group_i3c_macros_callback_events I3C Callback Events
* \{
* Macros to check I3C events passed by \ref cy_cb_i3c_handle_events_t callback.
* Each event is encoded in a separate bit, and therefore it is possible to
* notify about multiple events.
*/

/** The master write started by \ref Cy_I3C_MasterWrite is complete */
#define CY_I3C_MASTER_WR_CMPLT_EVENT       (0x00000001UL)

/** The master read started by \ref Cy_I3C_MasterRead is complete */
#define CY_I3C_MASTER_RD_CMPLT_EVENT       (0x00000002UL)

/** CY_I3C_MASTER_WR_EARLY_TERMINATION_EVENT */
#define CY_I3C_MASTER_WR_EARLY_TERMINATION_EVENT   (0x00000004UL)

/**
* Indicates the I3C hardware has detected an error.
*/

/** CY_I3C_CRC_ERROR_EVENT */
#define CY_I3C_CRC_ERROR_EVENT                    (0x00000008UL)
/** CY_I3C_PARITY_ERROR_EVENT */
#define CY_I3C_PARITY_ERROR_EVENT                 (0x00000010UL)
/** CY_I3C_FRAME_ERROR_EVENT */
#define CY_I3C_FRAME_ERROR_EVENT                  (0x00000020UL)
/** CY_I3C_BROADCAST_ADDR_NACK_ERROR_EVENT */
#define CY_I3C_BROADCAST_ADDR_NACK_ERROR_EVENT    (0x00000040UL)
/** CY_I3C_ADDR_NACK_ERROR_EVENT */
#define CY_I3C_ADDR_NACK_ERROR_EVENT              (0x00000080UL)
/** CY_I3C_BUFFER_OVERFLOW_ERROR_EVENT */
#define CY_I3C_BUFFER_OVERFLOW_ERROR_EVENT        (0x00000100UL)
/** CY_I3C_XFER_ABORTED_ERROR_EVENT */
#define CY_I3C_XFER_ABORTED_ERROR_EVENT           (0x00000200UL)
/** CY_I3C_I2C_SLV_WDATA_NACK_ERROR_EVENT */
#define CY_I3C_I2C_SLV_WDATA_NACK_ERROR_EVENT     (0x00000400UL)
/** CY_I3C_MASTER_EARLY_TERMINATION_EVENT */
#define CY_I3C_MASTER_EARLY_TERMINATION_EVENT     (0x00000800UL)
/** CY_I3C_MASTER_ERROR_M0_EVENT */
#define CY_I3C_MASTER_ERROR_M0_EVENT              (0X00001000UL)

/** CY_I3C_MASTER_ERR_EVENT */
#define CY_I3C_MASTER_ERR_EVENT                   (CY_I3C_CRC_ERROR_EVENT| CY_I3C_PARITY_ERROR_EVENT| CY_I3C_FRAME_ERROR_EVENT| \
                                                   CY_I3C_BROADCAST_ADDR_NACK_ERROR_EVENT| CY_I3C_ADDR_NACK_ERROR_EVENT| CY_I3C_BUFFER_OVERFLOW_ERROR_EVENT| \
                                                   CY_I3C_XFER_ABORTED_ERROR_EVENT| CY_I3C_I2C_SLV_WDATA_NACK_ERROR_EVENT| CY_I3C_MASTER_ERROR_M0 )

/** CY_I3C_SLAVE_ERR_EVENT */
#define CY_I3C_SLAVE_ERR_EVENT                    (CY_I3C_CRC_ERROR_EVENT| CY_I3C_PARITY_ERROR_EVENT| CY_I3C_FRAME_ERROR_EVENT| \
                                                   CY_I3C_BUFFER_OVERFLOW_ERROR_EVENT | CY_I3C_MASTER_EARLY_TERMINATION_EVENT)

/** The slave device is assigned a dynamic address through SETDASA or ENTDAA CCC from the master */
#define CY_I3C_SLAVE_ASSIGNED_DYN_ADDR_EVENT     (0X00002000UL)

/**
* The master has read all data out of the configured Read buffer.
* This event can be used to configure the next Read buffer. If the buffer
* remains empty, the CY_I3C_DEFAULT_TX bytes are returned to the master.
*/

/** CY_I3C_SLAVE_RD_BUF_EMPTY_EVENT */
#define CY_I3C_SLAVE_RD_BUF_EMPTY_EVENT    (0x00004000UL)

/** CY_I3C_SLAVE_MAX_RD_LEN_UPDT_EVENT */
#define CY_I3C_SLAVE_MAX_RD_LEN_UPDT_EVENT (0X00008000UL)

/** CY_I3C_SLAVE_MAX_WR_LEN_UPDT_EVENT */
#define CY_I3C_SLAVE_MAX_WR_LEN_UPDT_EVENT (0X00010000UL)

/**
* Indicates the master completed reading from the slave (set by the master NAK
* or Stop)
*/
#define CY_I3C_SLAVE_RD_CMPLT_EVENT        (0x00020000UL)

/**
* Indicates the master completed writing to the slave (set by the master Stop
* or Restart)
*/
#define CY_I3C_SLAVE_WR_CMPLT_EVENT        (0x00040000UL)

/**
* One of the CCC register is updated by I3C Master through CCC commands
*/
#define CY_I3C_SLAVE_CCC_REG_UPDATED_EVENT        (0x00080000UL)

/**
* There is no valid command in the command queue
*/
#define CY_I3C_SLAVE_NO_VALID_CMD_IN_CMDQ_EVENT        (0x00100000UL)

/**
* The data in Tx FIFO is not equal to the data length size of the command 
* or the TX_START_THLD value or the Response queue is full
*/
#define CY_I3C_SLAVE_DATA_NOT_READY_EVENT        (0x00200000UL)

/**
* DEFSLVS CCC is received.
*/
#define CY_I3C_DEFSLV_EVENT        (0x00400000UL)

/**
* Role of the controller changed from Master to Slave or vice-versa
*/
#define CY_I3C_CONTROLLER_ROLE_UPDATED_EVENT       (0x00800000UL)


/** \} group_i3c_macros_callback_events */

/** \cond INTERNAL */

/* COMMAND_QUEUE_PORT Register */
#define I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_Pos 0UL
#define I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_Msk 0x7UL
/* Transfer Command Structure */
#define I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_CMD 0x0UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TID_Pos 3UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TID_Msk 0x78UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CMD_Pos 7UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CMD_Msk 0x7F80UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CP_Pos 15UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_CP_Msk 0x8000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX_Pos 16UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_DEV_INDX_Msk 0x1F0000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED_Pos 21UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SPEED_Msk 0xE00000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Pos 26UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_ROC_Msk 0x4000000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Pos 27UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_SDAP_Msk 0x8000000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_RnW_Pos 28UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_RnW_Msk 0x10000000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Pos 30UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_CMD_TOC_Msk 0x40000000UL
/* Transfer Argument Command Structure */
#define I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_TRANSFER_ARG 0x01
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH_Pos 16UL
#define I3C_CORE_COMMAND_QUEUE_PORT_TRANSFER_ARG_DATA_LENGTH_Msk 0xFFFF0000UL
/* Short Data Argument Structure */
#define I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_SHORT_DATA_ARG 0x02UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB_Pos 3UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_BYTE_STRB_Msk 0x38UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0_Pos 8UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_0_Msk 0xFF00UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_1_Pos 16UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_1_Msk 0xFF0000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_2_Pos 23UL
#define I3C_CORE_COMMAND_QUEUE_PORT_SHORT_DATA_ARG_DATA_BYTE_2_Msk 0xFF000000UL
/* To select the valid data bytes of the Short Data Argument */
#define CY_I3C_BYTE_STROBE1 1UL
#define CY_I3C_BYTE_STROBE2 3UL
#define CY_I3C_BYTE_STROBE3 7UL
/* Address Assignment Command Structure */
#define I3C_CORE_COMMAND_QUEUE_PORT_CMD_ATTR_ADDR_ASSGN_CMD 0x03UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_TID_Pos 0x3UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_TID_Msk 0x78UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_CMD_Pos 7UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_CMD_Msk 0x7F80UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_INDX_Pos 16UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_INDX_Msk 0x1F0000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_COUNT_Pos 21UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_DEV_COUNT_Msk 0x3E00000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_ROC_Pos 26UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_ROC_Msk 0x4000000UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_TOC_Pos 30UL
#define I3C_CORE_COMMAND_QUEUE_PORT_ADDR_ASSGN_CMD_TOC_Msk 0x40000000UL

/*  RESPONSE_QUEUE_PORT Register  */
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Pos 0UL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk 0xFFFFUL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_CCCT_Pos 16UL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_CCCT_Msk 0xFF0000UL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_TID_Pos 24UL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_TID_Msk 0xF000000UL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_Pos 28UL
#define I3C_CORE_MASTER_RESPONSE_QUEUE_PORT_ERR_STS_Msk 0xF0000000UL

#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_DATA_LENGTH_Pos 0UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_DATA_LENGTH_Msk 0xFFFFUL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_CCC_HDR_HEADER_Pos 16UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_CCC_HDR_HEADER_Msk 0xFF0000UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_TID_Pos 24UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_TID_Msk 0x7000000UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_RX_RSP_Pos 27UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_RX_RSP_Msk 0x8000000UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_ERR_STS_Pos 28UL
#define I3C_CORE_SLAVE_RESPONSE_QUEUE_PORT_ERR_STS_Msk 0xF0000000UL

/* BCR Register */
#define CY_I3C_CORE_BCR_DEVICE_ROLE_Msk            (0xC0)
#define CY_I3C_CORE_BCR_I3C_SLAVE                (0 << 6)
#define CY_I3C_CORE_BCR_I3C_MASTER                (1 << 6)
#define CY_I3C_CORE_BCR_HDR_CAP_Msk                (0x20)
#define CY_I3C_CORE_BCR_BRIDGE_Msk                (0x10)
#define CY_I3C_CORE_BCR_OFFLINE_CAP_Msk            (0x08)
#define CY_I3C_CORE_BCR_IBI_PAYLOAD_Msk            (0x04)
#define CY_I3C_CORE_BCR_IBI_REQ_CAP_Msk            (0x02)
#define CY_I3C_CORE_BCR_MAX_DATA_SPEED_LIM_Msk    (0x01)

/* LVR register */
#define CY_I3C_LVR_LEGACY_I2C_INDEX_MASK        (0xE0)
#define CY_I3C_LVR_I2C_MODE_INDICATOR            (0x10)

/* IBI Identifiers */
#define CY_I3C_HOT_JOIN_ADDR                        (0x02)
#define CY_I3C_IBI_QUEUE_IBI_ADDR(x)                ((x) >> 1)
#define CY_I3C_IBI_TYPE_HOT_JOIN(x)                    (CY_I3C_IBI_QUEUE_IBI_ADDR((x)) == CY_I3C_HOT_JOIN_ADDR && !((x) & CY_I3C_BIT(0)))
#define CY_I3C_IBI_TYPE_MASTERSHIP_REQUEST(x)        (CY_I3C_IBI_QUEUE_IBI_ADDR((x)) != CY_I3C_HOT_JOIN_ADDR && !((x) & CY_I3C_BIT(0)))
#define CY_I3C_IBI_TYPE_SIR_REQUEST(x)                (CY_I3C_IBI_QUEUE_IBI_ADDR((x)) != CY_I3C_HOT_JOIN_ADDR && ((x) & CY_I3C_BIT(0)))
#define CY_I3C_IBI_SIR_REQ_ID(x)                    ((((x) & CY_I3C_GENMASK(6, 5)) >> 5) + ((x) & CY_I3C_GENMASK(4, 0)))

/* Interrupt Mask */
#define CY_I3C_INTR_MASK ( I3C_CORE_INTR_STATUS_EN_TX_THLD_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_RX_THLD_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_IBI_THLD_STS_EN_Msk    | \
                           I3C_CORE_INTR_STATUS_EN_CMD_QUEUE_READY_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_RESP_READY_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_TRANSFER_ABORT_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_CCC_UPDATED_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_DYN_ADDR_ASSGN_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_TRANSFER_ERR_STS_EN_Msk    | \
                           I3C_CORE_INTR_STATUS_EN_DEFSLV_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_READ_REQ_RECV_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_IBI_UPDATED_STS_EN_Msk  | \
                           I3C_CORE_INTR_STATUS_EN_BUSOWNER_UPDATED_STS_EN_Msk )

/* I3C states */
#define CY_I3C_IDLE              (0x10000000UL)
#define CY_I3C_IDLE_MASK         (0x10000000UL)

/* Master states */
#define CY_I3C_MASTER_ACTIVE     (0X00100000UL)
#define CY_I3C_MASTER_RX         (0x00100001UL)
#define CY_I3C_MASTER_TX         (0x00100002UL)

/* Slave states */
#define CY_I3C_SLAVE_ACTIVE      (0x00001000UL)
#define CY_I3C_SLAVE_RX          (0x00001001UL)
#define CY_I3C_SLAVE_TX          (0x00001002UL)

/* FIFO size */
#define CY_FIFO_SIZE              (256UL)

#define CY_I3C_FIFO_SIZE           CY_FIFO_SIZE
#define CY_I3C_HALF_FIFO_SIZE     (CY_FIFO_SIZE / 2UL)

#define CY_I3C_DEFAULT_RETURN    (0xFFUL)

/* Convert the timeout in milliseconds to microseconds */
#define CY_I3C_CONVERT_TIMEOUT_TO_US(timeoutMs)     ((timeoutMs) * 1000UL)

/* I3C data rates */
#define CY_I3C_SDR_DATA_RATE            (12500000U)
#define CY_I3C_SDR1_DATA_RATE            (8000000U)
#define CY_I3C_SDR2_DATA_RATE            (6000000U)
#define CY_I3C_SDR3_DATA_RATE            (4000000U)
#define CY_I3C_SDR4_DATA_RATE            (2000000U)
#define CY_I3C_HDRDDR_DATA_RATE            (25000000U)
#define CY_I3C_BUS_I2C_FM_TLOW_MIN_NS    (1300)
#define CY_I3C_BUS_I2C_FMP_TLOW_MIN_NS    (500)
#define CY_I3C_BUS_THIGH_MAX_NS            (41)
#define CY_I3C_SCL_I3C_TIMING_CNT_MIN    (5)


/* I2C data rates max (Hz): standard, fast and fast plus modes */
#define CY_I3C_I2C_FM_DATA_RATE    (400000U)
#define CY_I3C_I2C_FMP_DATA_RATE  (1000000U)

#define CY_I3C_IS_MODE_VALID(mode)      ( (CY_I3C_SLAVE  == (mode)) || \
                                              (CY_I3C_MASTER == (mode)) || \
                                              (CY_I3C_SECONDARY_MASTER == (mode)) )
#define CY_I3C_IS_BUS_MODE_VALID(mode)      ( (CY_I3C_BUS_PURE  == (mode)) || \
                                                   (CY_I3C_BUS_MIXED_FAST == (mode)) || \
                                                       (CY_I3C_BUS_MIXED_SLOW == (mode)) )
#define CY_I3C_INTR_VALID(intr, mask)            ( 0UL == ((intr) & ((uint32_t) ~(mask))) )

#define CY_I3C_IS_SDR_DATA_RATE_VALID(dataRateHz)   ( ((dataRateHz) > 0UL) && \
                                                      ((dataRateHz) <= CY_I3C_SDR_DATA_RATE) )

#define CY_I3C_IS_HDR_DATA_RATE_VALID(dataRateHz)   ( ((dataRateHz) > 0UL) && \
                                                      ((dataRateHz) <= CY_I3C_HDRDDR_DATA_RATE) )

#define CY_I3C_IS_TIMEOUT_VALID(timeoutMs)              ( (timeoutMs) <= (0xFFFFFFFFUL / 1000UL) )

#define CY_IS_I3C_ADDR_VALID(addr)              ( (0U == ((addr) & 0x80U)) )

#define CY_IS_I3C_BUFFER_VALID(buffer, size)    ( (NULL != (buffer)) && ((size) > 0UL) )

#define CY_I3C_IS_BUFFER_SIZE_VALID(size)      ( ((size) > 0UL) && \
                                                      ((size) <= CY_I3C_FIFO_SIZE) )


/* Default Values */

#define I3C_CORE_QUEUE_THLD_CTRL_DEF_VAL  (_VAL2FLD(I3C_CORE_QUEUE_THLD_CTRL_CMD_EMPTY_BUF_THLD, 1UL) | \
                                           _VAL2FLD(I3C_CORE_QUEUE_THLD_CTRL_RESP_BUF_THLD, 1UL) | \
                                           _VAL2FLD(I3C_CORE_QUEUE_THLD_CTRL_IBI_STATUS_THLD, 1UL))

#define I3C_CORE_DATA_BUFFER_THLD_CTRL_DEF_VAL (_VAL2FLD(I3C_CORE_DATA_BUFFER_THLD_CTRL_TX_EMPTY_BUF_THLD, 1UL) | \
                                                _VAL2FLD(I3C_CORE_DATA_BUFFER_THLD_CTRL_RX_BUF_THLD, 1UL) | \
                                                _VAL2FLD(I3C_CORE_DATA_BUFFER_THLD_CTRL_TX_START_THLD, 1UL) | \
                                                _VAL2FLD(I3C_CORE_DATA_BUFFER_THLD_CTRL_RX_START_THLD, 1UL))

/** \endcond */

/** \} group_i3c_macros */

/*******************************************************************************
*                            Enumerated Types
*******************************************************************************/

/**
* \addtogroup group_i3c_enums
* \{
*/
    
/** I3C status codes */
typedef enum
{
    /** Operation completed successfully */
    CY_I3C_SUCCESS = 0U,

    /** One or more of input parameters are invalid */
    CY_I3C_BAD_PARAM = (CY_I3C_ID| CY_PDL_STATUS_ERROR | 1U),

    /**
    * The master is not ready to start a new transaction.
    * Either the master is still processing a previous transaction or in the
    * master-slave mode, the slave operation is in progress.
    */
    CY_I3C_MASTER_NOT_READY = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 2U),

    /**
    * The master failed to attach the slave device to the bus
    * as there already exists maximum number of devices on the bus.
    */
    CY_I3C_MASTER_MAX_DEVS_PRESENT = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 3U),

    /**
    * The master rejected to attach an I2C device without 50ns Spike filter.
    */
    CY_I3C_MASTER_BAD_I2C_DEVICE = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 4U),

    /**
    * Unsupported CCC command 
    */
    CY_I3C_MASTER_CCC_NOT_SUPPORTED =(CY_I3C_ID | CY_PDL_STATUS_ERROR | 5U),

    /**
    * SDR Master Error Code M0 for Illegally formatted CCC Response
    */
    CY_I3C_MASTER_ERROR_M0 = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 6U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * CRC Error
    */
    CY_I3C_MASTER_CRC_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 7U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * Parity Error
    */
    CY_I3C_MASTER_PARITY_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 8U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * Frame Error
    */
    CY_I3C_MASTER_FRAME_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 9U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * Broadcast Address NACK Error
    */
    CY_I3C_MASTER_BROADCAST_ADDR_NACK_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 10U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * Address NACK Error - Slave NACKs for the dynamic address assignment during 
    * ENTDAA process.
    */
    CY_I3C_MASTER_ADDR_NACK_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 11U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * TX/RX Buffer Overflow Error - Only for HDR Transfers
    */
    CY_I3C_MASTER_BUFFER_OVERFLOW_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 12U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * Transfer Aborted Error
    */
    CY_I3C_MASTER_XFER_ABORTED_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 13U ),

    /**
    * Error Type of the proccessed command received in Response Data Structure:
    * I2C Slave Write Data NACK Error
    */
    CY_I3C_MASTER_I2C_SLV_WDATA_NACK_ERROR = ( CY_I3C_ID | CY_PDL_STATUS_ERROR | 14U ),

    /** The master NACKed the IBI */
    CY_I3C_MASTER_IBI_NACK = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 15U),
    
    /** The master ACKed the Mastership Request IBI */
    CY_I3C_MASTER_MR_IBI_ACK = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 16U),

    /** The master ACKed the Slave Interrupt Request IBI */
    CY_I3C_MASTER_SIR_IBI_ACK = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 17U),

    /** The master ACKed the Hot-join IBI */
    CY_I3C_MASTER_HOTJOIN_IBI_ACK = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 18U),
    
    /** Free address unavailable */
    CY_I3C_MASTER_FREE_ADDR_UNAVIAL = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 19U),

    /** The device is not HDR capable */
    CY_I3C_NOT_HDR_CAP = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 20U),

    /** The buffer size is greater than the FIFO size */
    CY_I3C_BAD_BUFFER_SIZE = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 21U),

    /** The secondary master responded with incorrect address to GETACCMST CCC */
    CY_I3C_ADDR_MISMATCH = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 22U),

    /** The device is not yet assigned a dynamic address */
    CY_I3C_ADDR_INVALID = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 23U),

    /** In Slave mode of operation, the Master has disabled SIR through DISEC */
    CY_I3C_SIR_DISABLED = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 24U),
    
    /** In Slave mode of operation, the Master has disabled MR through DISEC */
    CY_I3C_MR_DISABLED = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 25U),

    /** The Slave device failed to issue IBI */
    CY_I3C_IBI_NOT_ATTEMPTED = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 26U),

    /** There are no secondary master devices on the bus */
    CY_I3C_NO_SECONDARY_MASTER_DEVICES = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 27U),

    /** The slave device requested generation of hot-join event generation */ //tbd - provide more clarity in the comment
    CY_I3C_BAD_EVENT_REQ = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 28U),

    /** The device is not a secondary master */
    CY_I3C_NOT_SECONDARY_MASTER = (CY_I3C_ID | CY_PDL_STATUS_ERROR | 29U),

} cy_en_i3c_status_t;

/** I3C Operation Modes */
typedef enum
{
    CY_I3C_SLAVE                  = 1U,    /**< Configures I3C for Slave operation */
    CY_I3C_MASTER                 = 2U,    /**< Configures I3C for Main Master operation */
    CY_I3C_SECONDARY_MASTER       = 3U,    /**< Configures I3C for Secondory Master-Slave operation */
} cy_en_i3c_mode_t;

/** I3C Bus Modes */
typedef enum
{
    CY_I3C_BUS_PURE,        /**< Only I3C devices are connected to the bus. No limitation expected */
    CY_I3C_BUS_MIXED_FAST,  /**< I2C devices with 50ns spike filter are present on the bus.
                                 High SCL pulse has to stay below 50ns when transmitting I3C frames */
    CY_I3C_BUS_MIXED_SLOW,  /**< I2C devices without 50ns spike filter are present on the bus */
} cy_en_i3c_bus_mode_t;

/** I3C Address Slot Statuses */
typedef enum 
{
    CY_I3C_ADDR_SLOT_FREE,              /**< Address is free */
    CY_I3C_ADDR_SLOT_RSVD,              /**< Address is reserved */
    CY_I3C_ADDR_SLOT_I2C_DEV,           /**< Address is assigned to an I2C device */
    CY_I3C_ADDR_SLOT_I3C_DEV,           /**< Address is assigned to an I3C device */
    CY_I3C_ADDR_SLOT_STATUS_MASK = 3,   /**< Address slot mask */
}cy_en_i3c_addr_slot_status_t;

/** I3C IBI Types */
typedef enum
{
    CY_I3C_IBI_HOTJOIN,     /**< IBI Hot join Request */
    CY_I3C_IBI_SIR,         /**< IBI Slave Interrupt Request */
    CY_I3C_IBI_MASTER_REQ,  /**< IBI Master ownership Request */
} cy_en_i3c_ibi_type_t;

/** I3C Transaction IDs for HDR and SDR commands */
typedef enum
{
    CY_I3C_MASTER_SDR_WRITE_TID  = 1,     /**< TID for SDR Write Transfer */
    CY_I3C_MASTER_SDR_READ_TID   = 2,    /**< TID for SDR Read Transfer */
    CY_I3C_MASTER_HDR_WRITE_TID  = 3,    /**< TID for HDR Write Transfer */
    CY_I3C_MASTER_HDR_READ_TID   = 4,    /**< TID for HDR Read Transfer */
}cy_en_i3c_tid_t;

/** I3C Mode, SDR and HDR transfer speeds */ 
typedef enum
{
    CY_I3C_SDR0     = 0,         /**< I3C mode transfer speed - SDR0*/
    CY_I3C_SDR1     = 1,         /**< I3C mode transfer speed - SDR1*/
    CY_I3C_SDR2     = 2,         /**< I3C mode transfer speed - SDR2*/
    CY_I3C_SDR3     = 3,         /**< I3C mode transfer speed - SDR3*/
    CY_I3C_SDR4     = 4,         /**< I3C mode transfer speed - SDR4*/
    CY_I3C_HDR_DDR  = 6,         /**< I3C mode transfer speed - HDR-DDR */
}cy_en_i3c_data_speed_t;

/** I2C Mode, SDR transfer speeds */
typedef enum
{
    CY_I3C_FMP_I2C  = 0,      /**< I2C mode transfer speed - Fast Mode Plus */
    CY_I3C_FM_I2C   = 1,      /**< I2C mode transfer speed - Fast Mode */
}cy_en_i2c_data_speed_t;
/** \} group_i3c_enums */

/*******************************************************************************
*                              Type Definitions
*******************************************************************************/

/**
* \addtogroup group_i3c_data_structures
* \{
*/

/**
* I3C device IBI structure.
* Defines an IBI event.
*/
typedef struct cy_stc_i3c_ibi
{
    /** IBI event type */
    cy_en_i3c_ibi_type_t event;

    /** Address of the device requesting IBI */
    uint8_t slaveAddress;

    /** IBI Status. Indicates the master response for the received IBI */
    uint32_t status;
}cy_stc_i3c_ibi_t;

/**
* Provides the typedef for the callback function called in the
* \ref Cy_I3C_Interrupt to notify the user about occurrences of
* \ref group_i3c_macros_callback_events.
*/
typedef void (* cy_cb_i3c_handle_events_t)(uint32_t event);

/**
* Provides the typedef for the callback function called in the
* \ref Cy_I3C_Interrupt to notify the user about occurrences of
* \ref group_i3c_macros_callback_events.
*/
typedef void (* cy_cb_i3c_handle_ibi_t)(cy_stc_i3c_ibi_t *event);

/** 
* I2C device structure.
* Defines an I2C device.
* It is mandatory to provide I2C device static address and lvr information.
*/
typedef struct cy_stc_i2c_device
{
    /** Static address of the I2C device */
    uint8_t staticAddress;

    /** Legacy Virtual Register */
    uint8_t lvr;
} cy_stc_i2c_device_t;

/**
* I3C device structure.
* Defines an I3C device.
* StaticAddress of the I3C device if provided will be used by the controller
* to initiate the dynamic address assignment procedure through SETDASA.
* Desired I3C dynamic address to be used can be provided, but if set to zero,
* available free address will be assigned to the device by the controller.
*/
typedef struct cy_stc_i3c_device
{
    /** 48 bit provisional ID of I3C device */
    uint64_t provisonalID;

    /** Device Charactaristic Register of I3C device */
    uint8_t dcr;

    /** Bus Charactaristic Register of I3C device */
    uint8_t bcr;

    /** Static address of the I3C device if used */
    uint8_t staticAddress;

    /**
    * Desired dynamic address for I3C device.
    * If set to zero, available free address will be assigned by I3C controller.
    */
    uint8_t dynamicAddress;

    /** Max private SDR read length in bytes */
    uint16_t mrl;

    /** Max private SDR write length in bytes */
    uint16_t mwl;

    /** Max read speed information */
    uint8_t maxReadDs;

    /** Max write speed information */
    uint8_t maxWriteDs;

    /** Max read turn-around time in micro -seconds */
    uint8_t maxReadTurnaround[3];

    /** Supported HDR modes */
    uint8_t HDRCap;

    /** 
    * Support for HDR Mode.
    * true: HDR modes supported. HDRCap member for the device is valid.
    * false: HDR modes are not supported.
    */
    bool hdrSupport; 

    /**
    * Max data speed limitation.
    * true: Limitation on max data speed. maxReadDs, maxWriteDs, maxReadTurnaround members for the device are valid
    * false: No limitation on max data speed
    */
    bool speedLimit;
}cy_stc_i3c_device_t;

/** A local list of devices on the bus */
typedef struct cy_stc_i3c_master_devlist
{
    /** Identifies the device type */
    bool i2c;

    /** I2C/I3C device descriptor */
    union
    {     
        cy_stc_i3c_device_t i3cDevice;
        cy_stc_i2c_device_t i2cDevice;
    };
}cy_stc_i3c_master_devlist_t;

/** Device Charactersitic Table structure */
typedef struct cy_stc_i3c_dev_char
{
    uint32_t LSBProvisionalID; /**< The LSB 32-bit value of provisional ID */
    uint32_t MSBProvisionalID; /**< The MSB 16-bit value of provisional ID */
    uint32_t DCR_BCR;          /**< DCR [7:0] and BCR [15:8] of the I3C device */
    uint32_t dynamicAddress;   /**< Dynamic address [7:0] of the I3C device */
}cy_stc_i3c_dev_char_t;

/** 64-bit CCC structure */
typedef struct cy_stc_i3c_ccc
{
    uint32_t cmdHigh;     /**< cmdHigh */
    uint32_t cmdLow;      /**< cmdLow */
}cy_stc_i3c_ccc_t;

/** I3C bus configuration structure */
typedef struct cy_stc_i3c_config
{
    /** Specifies the mode of I3C controller operation */
    cy_en_i3c_mode_t i3cMode;

    /** Specifies the mode of I3C bus operation */
    cy_en_i3c_bus_mode_t i3cBusMode;

    /** Use the SDMA for Rx/Tx */
    bool useDma;

    /** The frequency of the clock connected to the I3C block in Hz. */
    uint32_t i3cClockHz;

    /** The desired I3C data Rate in Hz. */
    uint32_t i3cSclRate;
    
    /** SDR only or SDR and HDR capable 
        true: SDR and HDR
        false: SDR only
    */
    bool hdrCapable;
    
    /**
    * Max data speed limitation.
    * true: Limitation on max data speed. maxReadDs, maxWriteDs, maxReadTurnaround members for the device are valid
    * false: No limitation on max data speed
    */
    bool speedLimit;

    /** The device characteristic value of the I3C Device */
    uint8_t dcr;

    /** The Provisional ID of the Device */
    uint64_t pid;

    /** Below members are only applicable for the Slave mode */

    /** The static address of the I3C Slave Device, if present */
    uint8_t staticAddress;

    /** Enable/Disable Hot-join */
    bool hotjoinEnable;

} cy_stc_i3c_config_t;

/** I3C CCC command payload structure */
typedef struct cy_stc_i3c_ccc_payload
{
    /** Payload length */
    uint16_t len;
    
    /** Payload data */
    void *data;
} cy_stc_i3c_ccc_payload_t;

/** I3C CCC command structure */
typedef struct cy_stc_i3c_ccc_cmd
{
    /** CCC command id */
    uint8_t cmd;
    
    /** CCC command payload or NULL */
    cy_stc_i3c_ccc_payload_t *data;
    
    /** Destination address */
    uint8_t address;
}cy_stc_i3c_ccc_cmd_t;

/** I3C Master information */
typedef struct cy_stc_i3c_master
{
    /** two bits per address to depict the status of the addresses - 8*4*8 */
    unsigned long addrslotsStatusArray[((CY_I3C_MAX_ADDR + 1) * 2) / CY_I3C_BITS_PER_LONG];
    
    /** immediate last address, from the address list, assigned as dynamic address to the slave device  */
    int8_t lastAddress;
    
    /** index of the free position wrt the Device Address Table */
    uint32_t freePos;
    
    /** the number of devices on the bus */
    uint32_t devCount;
    
    /** Count of i2c devices on the bus */
    uint32_t i2cDeviceCount; 
    
    /** Number of dynamically addressed devices */
    uint32_t dynAddrDevCount;
}cy_stc_i3c_master_t;

/** I3C HDR command Structure */
typedef struct cy_stc_i3c_hdr_cmd 
{
    /**
    * Command opcode. Bit 7 encodes the direction of the data transfer, if
    * set this is a read, otherwise this is a write.
    */
    uint8_t code;
    
    /** Number of data words (a word is 16bits wide) to transfer */
    uint32_t ndatawords;
    
    /** Input/Output buffer */
    union 
    {
        uint16_t *in;
        const uint16_t *out;
    }data;
}cy_stc_i3c_hdr_cmd_t;


/**
* I3C context structure.
* All fields for the context structure are internal. Firmware never reads or
* writes these values. Firmware allocates the structure and provides the
* address of the structure to the driver in function calls. Firmware must
* ensure that the defined instance of this structure remains in scope
* while the drive is in use.
*/
typedef struct cy_stc_i3c_context
{
    /** \cond INTERNAL */

    volatile uint32_t state;    /**< The driver state */
    cy_en_i3c_mode_t i3cMode;   /** Specifies the mode of i3c controller operation */
    cy_en_i3c_bus_mode_t i3cBusMode;     /** Specifies the mode of i3c bus operation */

    uint32_t i3cSclRate;
    uint32_t i3cClockHz;

    volatile uint32_t masterStatus; /**< The master status */

    uint8_t  *masterBuffer;     /**< The pointer to the master buffer (either for a transmit or a receive operation) */
    uint32_t  masterBufferSize;         /**< The current master buffer size */
    volatile uint32_t masterBufferIdx;  /**< The current location in the master buffer */
    volatile uint8_t destDeviceAddr; /**< The destination device address in the transfers */

    volatile uint32_t slaveStatus;       /**< The slave status */
    volatile bool     slaveRdBufEmpty;   /**< Tracks slave Read buffer empty event */

    uint8_t  *slaveTxBuffer;             /**< The pointer to the slave transmit buffer (a master reads from it) */
    uint32_t  slaveTxBufferSize;         /**< The current slave transmit buffer size */
    volatile uint32_t slaveTxBufferIdx;  /**< The current location in the slave buffer */
    volatile uint32_t slaveTxBufferCnt;  /**< The number of transferred bytes */

    uint8_t  *slaveRxBuffer;             /**< The pointer to the slave receive buffer (a master writes into it) */
    uint32_t  slaveRxBufferSize;         /**< The current slave receive buffer size */
    volatile uint32_t slaveRxBufferIdx;  /**< The current location in the slave buffer */
    volatile uint32_t slaveRxBufferCnt;

    cy_stc_i3c_master_t i3cMaster;       /**< I3C Master bus details */

    cy_stc_i3c_master_devlist_t devList[CY_I3C_MAX_DEVS]; /**< The list of devices on the bus */

    cy_stc_i3c_hdr_cmd_t *hdrCmd; /** <Maintains the HDR command sent */

    /**
    * The pointer to an event callback that is called when any of
    * \ref group_i3c_macros_callback_events occurs
    */
    cy_cb_i3c_handle_events_t cbEvents;

    /**
    * The pointer to an event callback that is called when any of
    * \ref group_i3c_macros_ibi_events occurs
    */
    cy_cb_i3c_handle_ibi_t    cbIbi;

    /** \endcond */
} cy_stc_i3c_context_t;

/** The I3C Master transfer structure */
typedef struct cy_stc_i3c_master_xfer_config
{
    /** The 7-bit right justified slave address to communicate with */
    uint8_t  slaveAddress;

    /**
    * The pointer to the buffer for data to read from the slave or 
    * data to write into the slave
    */
    uint8_t  *buffer;

    /** The size of the buffer */
    uint32_t bufferSize;

    /** toc */
    bool toc;
} cy_stc_i3c_master_xfer_config_t;

/** \} group_i3c_data_structures */


/*******************************************************************************
*                            Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_i3c_general_functions
* \{
*/
cy_en_i3c_status_t Cy_I3C_Init(I3C_CORE_Type *base, cy_stc_i3c_config_t const *config, cy_stc_i3c_context_t *context);
void Cy_I3C_DeInit(I3C_CORE_Type *base,cy_stc_i3c_context_t *context);
void Cy_I3C_Enable(I3C_CORE_Type *base);
void Cy_I3C_Disable(I3C_CORE_Type *base);
void Cy_I3C_Resume(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
bool Cy_I3C_IsBusBusy(I3C_CORE_Type const *base);
bool  Cy_I3C_IsMaster(I3C_CORE_Type const *base);
cy_en_i3c_mode_t Cy_I3C_GetMode(I3C_CORE_Type const *base);
void Cy_I3C_Interrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
void Cy_I3C_RegisterEventCallback(I3C_CORE_Type const *base, cy_cb_i3c_handle_events_t callback, cy_stc_i3c_context_t *context);
void Cy_I3C_RegisterIbiCallback (I3C_CORE_Type const *base, cy_cb_i3c_handle_ibi_t callback, cy_stc_i3c_context_t *context);

/** \} group_i3c_general_functions */

/**
* \addtogroup group_i3c_master_functions
* \{
*/
cy_en_i3c_status_t Cy_I3C_MasterAttachI2CDevice(I3C_CORE_Type *base, cy_stc_i2c_device_t *i2cDevice, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterDetachI2CDevice(I3C_CORE_Type *base, cy_stc_i2c_device_t *i2cDevice, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterAttachI3CDevice(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterDetachI3CDevice(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_GetI2CDeviceCount(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_GetI3CDeviceCount(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_SetDataRate(I3C_CORE_Type *base, uint32_t dataRateHz, uint32_t i3cClockHz, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_GetDataRate(I3C_CORE_Type const *base, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterGetI2CDevices(I3C_CORE_Type *base, cy_stc_i2c_device_t *i2cDeviceList, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterGetI3CDevices(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDeviceList, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterGetFreeDeviceAddress(I3C_CORE_Type *base, uint8_t *address, cy_stc_i3c_context_t *context);
bool Cy_I3C_isCCCCmdSupported(I3C_CORE_Type *base, uint8_t cccCmd, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_SendCCCCmd(I3C_CORE_Type *base, cy_stc_i3c_ccc_cmd_t *cccCmd, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_DisableDeviceIbi(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_EnableDeviceIbi(I3C_CORE_Type *base, cy_stc_i3c_device_t *i3cDevice, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterWrite(I3C_CORE_Type *base, cy_stc_i3c_master_xfer_config_t *xferConfig, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterRead (I3C_CORE_Type *base, cy_stc_i3c_master_xfer_config_t* xferConfig, cy_stc_i3c_context_t *context);
void Cy_I3C_MasterAbortTransfer(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterWriteByte(I3C_CORE_Type *base, uint8_t slaveAddress, int8_t data, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterReadByte(I3C_CORE_Type *base, uint8_t slaveAddress, int8_t *data, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_GetBusStatus(I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context);
cy_en_i3c_status_t Cy_I3C_MasterStartEntDaa(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_MasterSendHdrCmds(I3C_CORE_Type *base, uint8_t slaveAddress, cy_stc_i3c_hdr_cmd_t *hdrCmd, cy_stc_i3c_context_t *context);

/** \} group_i3c_master_functions */


/**
* \addtogroup group_i3c_slave_functions
* \{
*/
cy_en_i3c_status_t Cy_I3C_SlaveGetDynamicAddress(I3C_CORE_Type const *base, uint8_t *address, cy_stc_i3c_context_t const *context);
uint32_t Cy_I3C_SlaveGetMaxReadLength(I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context);
uint32_t Cy_I3C_SlaveGetMaxWriteLength(I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context);
cy_en_i3c_status_t Cy_I3C_SlaveGenerateIbi(I3C_CORE_Type *base, cy_stc_i3c_ibi_t *ibitype, cy_stc_i3c_context_t *context);
void Cy_I3C_SlaveConfigReadBuf (I3C_CORE_Type const *base, uint8_t *buffer, uint32_t size, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_SlaveGetReadTransferCount (I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context);
void Cy_I3C_SlaveConfigWriteBuf(I3C_CORE_Type const *base, uint8_t *buffer, uint32_t size, cy_stc_i3c_context_t *context);
uint32_t Cy_I3C_SlaveGetWriteTransferCount (I3C_CORE_Type const *base, cy_stc_i3c_context_t const *context);

/** \} group_i3c_slave_functions */

/**
* \addtogroup group_i3c_secondary_master_functions
* \{
*/
cy_en_i3c_status_t Cy_I3C_RequestMastership(I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
cy_en_i3c_status_t Cy_I3C_DeliverMastership(I3C_CORE_Type *base, uint8_t SecMasterAddress, cy_stc_i3c_context_t *context);

/** \} group_i3c_secondary_master_functions */

/**
* \addtogroup group_i3c_interrupt_functions
* \{
*/
void Cy_I3C_SlaveInterrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
void Cy_I3C_MasterInterrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
/** \cond INTERNAL */
void Cy_I3C_SecondaryMasterInterrupt (I3C_CORE_Type *base, cy_stc_i3c_context_t *context);
/** \endcond */
/** \} group_i3c_interrupt_functions */

/**
* \addtogroup group_i3c_low_power_functions
* \{
*/
cy_en_syspm_status_t Cy_I3C_DeepSleepCallback(cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode);
cy_en_syspm_status_t Cy_I3C_HibernateCallback(cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode);
/** \} group_i3c_low_power_functions */

/*******************************************************************************
*                     In-line Function Implementation
*******************************************************************************/

/**
* \addtogroup group_i3c_general_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_I3C_GetInterruptStatus
****************************************************************************//**
* 
* Returns the interrupt request register. This register contains the current
* status of the interrupt sources.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* The current status of the interrupt sources.
* Each constant is a bit field value. The value returned may have multiple
* bits set to indicate the current status.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_GetInterruptStatus(I3C_CORE_Type *base)
{
    return(I3C_CORE_INTR_STATUS(base) & CY_I3C_INTR_MASK);
}

/*******************************************************************************
* Function Name: Cy_I3C_ClearInterrupt
****************************************************************************//**
* 
* Clears the interrupt sources in the interrupt request register.
*
* \param base
* The pointer to the I3C instance.
*
* \param interruptMask
* Interrupt sources to be cleared.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_ClearInterrupt(I3C_CORE_Type *base, uint32_t interruptMask)
{
    CY_ASSERT_L2(CY_I3C_INTR_VALID(interruptMask, CY_I3C_INTR_MASK));
    I3C_CORE_INTR_STATUS(base) = interruptMask;
}

/*******************************************************************************
* Function Name: Cy_I3C_SetInterrupt
****************************************************************************//**
* 
* Sets interrupt sources in the interrupt request register.
*
* \param base
* The pointer to the I3C instance.
*
* \param interruptMask
* The interrupt sources to set in the interrupt request register.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_SetInterrupt(I3C_CORE_Type *base, uint32_t interruptMask)
{
    CY_ASSERT_L2(CY_I3C_INTR_VALID(interruptMask, CY_I3C_INTR_MASK));
    I3C_CORE_INTR_FORCE(base) = interruptMask;
}

/*******************************************************************************
* Function Name: Cy_I3C_GetInterruptStatus
****************************************************************************//**
* 
* Returns the interrupt signal enable register.
* This register specifies which bits from the interrupt request register
* can trigger an interrupt event.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* Enabled interrupt sources.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_GetInterruptMask(I3C_CORE_Type *base)
{
    return(I3C_CORE_INTR_SIGNAL_EN(base));
}

/*******************************************************************************
* Function Name: Cy_I3C_SetInterrupt
****************************************************************************//**
* 
* Writes the I3C interrupt signal enable register. 
* This register specifies which bits
* from the I3C interrupt request register can trigger an interrupt event.
*
* \param base
* The pointer to the I3C instance.
*
* \param interruptMask
* Enabled interrupt sources.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_SetInterruptMask(I3C_CORE_Type *base, uint32_t interruptMask)
{
    CY_ASSERT_L2(CY_I3C_INTR_VALID(interruptMask, CY_I3C_INTR_MASK));
    I3C_CORE_INTR_SIGNAL_EN(base) = interruptMask;

}

/*******************************************************************************
* Function Name: SetInterruptStatusMask
****************************************************************************//**
*
* Writes the I3C interrupt status enable register. This register specifies which bits
* from the I3C interrupt request register can trigger an interrupt event.
*
* \param base
* The pointer to the I3C instance.
*
* \param interruptMask
* Enabled I3C interrupt sources.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_SetInterruptStatusMask (I3C_CORE_Type *base, uint32_t interruptMask)
{
    CY_ASSERT_L2(CY_I3C_INTR_VALID(interruptMask, CY_I3C_INTR_MASK));
    I3C_CORE_INTR_STATUS_EN(base) = interruptMask;
}

/*******************************************************************************
*  Function Name: Cy_I3C_WriteIntoDeviceAddressTable
****************************************************************************//**
*
* Writes the value into the specified location of the DAT.
*
* \param base
* The pointer to the I3C instance.
* 
* \param index
* The offset from the base of the DAT.
*
* \param value
* The value to be written at the specified location in the DAT.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_WriteIntoDeviceAddressTable(I3C_CORE_Type *base, uint8_t index, uint32_t value)
{
    volatile uint32_t *ptr;
    ptr = &(I3C_CORE_DEV_ADDR_TABLE_LOC1(base));

    ptr = ptr + index;

    *ptr = value;
}

/*******************************************************************************
* Function Name: Cy_I3C_ReadDynAddrFromDAT
****************************************************************************//**
* 
* Returns the dynamic address of the I3C device stored in the DAT.
*
* \param base
* The pointer to the I3C instance.
*
* \param index
* The offset from the base of the DAT.
*
* \return
* The dynamic address of the I3C device.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_ReadDynAddrFromDAT(I3C_CORE_Type *base, uint8_t index)
{
    volatile uint32_t *ptr;
    uint32_t addr;

    ptr = &(I3C_CORE_DEV_ADDR_TABLE_LOC1(base));

    ptr = ptr + index;

    addr = _FLD2VAL(I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_DYNAMIC_ADDR, *ptr);
    
    return (0x7F & addr);
}

/*******************************************************************************
* Function Name: Cy_I3C_ReadStaticAddrFromDAT
****************************************************************************//**
* Returns the static address of the I2C device stored in the DAT.
*
* \param base
* The pointer to the I3C instance.
*
* \param index
* The offset from the base of the DAT.
*
* \return
* The static address of the I2C device.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_ReadStaticAddrFromDAT(I3C_CORE_Type *base, uint8_t index)
{
    volatile uint32_t *ptr;
    ptr = &(I3C_CORE_DEV_ADDR_TABLE_LOC1(base));

    ptr = ptr + index;

    return ((*ptr) & I3C_CORE_DEV_ADDR_TABLE_LOC1_DEV_STATIC_ADDR_Msk);
}

/*******************************************************************************
* Function Name: Cy_I3C_ReadFromDevCharTable
****************************************************************************//**
* Populates the i3c device defined by \ref cy_stc_i3c_device_t with the 
* DCR, BCR, PID and dynamic address information from the DCT.
*
* \param base
* The pointer to the I3C instance.
*
* \param index
* The offset from the base of the DAT.
*
* \param i3cDevice
* The pointer to the i3c device description structure \ref cy_stc_i3c_device_t.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_ReadFromDevCharTable(I3C_CORE_Type *base, uint8_t index, cy_stc_i3c_device_t *i3cDevice)
{
    cy_stc_i3c_dev_char_t *ptr;
    
    ptr = (cy_stc_i3c_dev_char_t *)&(I3C_CORE_DEV_CHAR_TABLE1_LOC1(base));
    ptr = ptr + index;
    i3cDevice->provisonalID = ((uint64_t)(ptr->LSBProvisionalID));
    i3cDevice->provisonalID |= ((uint64_t)(ptr->MSBProvisionalID) << 32UL);
    i3cDevice->dcr = I3C_CORE_DEV_CHAR_TABLE1_LOC3_DCR_Msk & (ptr->DCR_BCR);
    i3cDevice->bcr = _FLD2VAL(I3C_CORE_DEV_CHAR_TABLE1_LOC3_BCR, (ptr->DCR_BCR));
    i3cDevice->dynamicAddress = ptr->dynamicAddress;
}

/*******************************************************************************
*  Function Name: Cy_I3C_UpdateI3CDevInList
****************************************************************************//**
*
* Adds the I3C device to the list of devices \ref cy_stc_i3c_master_devlist_t
* present on the bus.
* 
* \param i3cDevice
* The pointer to the I3C device description structure \ref cy_stc_i3c_device_t.
*
* \param pos
* The position of the device in the list \ref cy_stc_i3c_master_devlist_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_UpdateI3CDevInList(cy_stc_i3c_device_t *i3cDevice, uint8_t pos, cy_stc_i3c_context_t *context)
{
    context->devList[pos].i2c = false;
    context->devList[pos].i3cDevice = *i3cDevice;
}

/*******************************************************************************
*  Function Name: Cy_I3C_UpdateI2CDevInList
****************************************************************************//**
*
* Adds the I2C device to the list of devices \ref cy_stc_i3c_master_devlist_t
* present on the bus.
* 
* \param i2cDevice
* The pointer to the I2C device description structure \ref cy_stc_i2c_device_t.
*
* \param pos
* The position of the device in the list \ref cy_stc_i3c_master_devlist_t.
*
* \param context
* The pointer to the context structure \ref cy_stc_i3c_context_t allocated
* by the user. The structure is used during the I3C operation for internal
* configuration and data retention. The user must not modify anything
* in this structure.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_UpdateI2CDevInList(cy_stc_i2c_device_t *i2cDevice, uint8_t pos, cy_stc_i3c_context_t *context)
{
    context->devList[pos].i2c = true;
    context->devList[pos].i2cDevice = *i2cDevice;
}

/*******************************************************************************
* Function Name: Cy_I3C_ReadRxFIFO
****************************************************************************//**
*
* Reads a data element directly out of the RX FIFO.
* This function does not check whether the RX FIFO has data before reading it.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* Data from RX FIFO.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_ReadRxFIFO(I3C_CORE_Type *base)
{
    return (I3C_CORE_TX_RX_DATA_PORT(base));
}

/*******************************************************************************
* Function Name: Cy_I3C_WriteTxFIFO
****************************************************************************//**
*
* Writes data directly into the TX FIFO.
* This function does not check whether the TX FIFO is not full before writing
* into it.
*
* \param base
* The pointer to the I3C instance.
*
* \param data
* Data to write to the TX FIFO.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_WriteTxFIFO(I3C_CORE_Type *base, uint32_t data)
{
    I3C_CORE_TX_RX_DATA_PORT(base) = data;
}

/*******************************************************************************
* Function Name: Cy_I3C_GetFreeEntriesInTxFifo
****************************************************************************//**
*
* Provides the number of empty locations in the TX FIFO.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* Transmit buffer empty level value.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_GetFreeEntriesInTxFifo(I3C_CORE_Type *base)
{
    return _FLD2VAL(I3C_CORE_DATA_BUFFER_STATUS_LEVEL_TX_BUF_EMPTY_LOC, I3C_CORE_DATA_BUFFER_STATUS_LEVEL(base));
}

/*******************************************************************************
* Function Name: Cy_I3C_GetFreeEntriesInRxFifo
****************************************************************************//**
*
* Provides the number of empty locations in the RX FIFO.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* Receive buffer empty level value.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_GetFreeEntriesInRxFifo(I3C_CORE_Type *base)
{
    return ((CY_I3C_FIFO_SIZE/4) - _FLD2VAL(I3C_CORE_DATA_BUFFER_STATUS_LEVEL_RX_BUF_BLR, I3C_CORE_DATA_BUFFER_STATUS_LEVEL(base)));
}

/*******************************************************************************
* Function Name: Cy_I3C_SetRxFifoLevel
****************************************************************************//**
*
* Sets the RX FIFO level. When there are more data elements in the RX FIFO than
* this level, the RX FIFO level interrupt is triggered.
*
* \param base
* The pointer to the I3C instance.
*
* \param level
* When there are more data elements in the FIFO than this level, the RX level
* interrupt is triggered.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_SetRxFifoLevel(I3C_CORE_Type *base, uint32_t level)
{
    CY_REG32_CLR_SET(I3C_CORE_DATA_BUFFER_THLD_CTRL(base), I3C_CORE_DATA_BUFFER_THLD_CTRL_RX_BUF_THLD, level);
}

/*******************************************************************************
* Function Name: Cy_SCB_GetRxFifoLevel
****************************************************************************//**
*
* Returns the RX FIFO level. When there are more words in the RX FIFO than the
* threshold level, the RX FIFO level interrupt is triggered.
*
* \param base
* The pointer to the I3C instance.
*
* \return
* RX FIFO level.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_I3C_GetRxFifoLevel(CySCB_Type const *base)
{
    return _FLD2VAL(I3C_CORE_DATA_BUFFER_THLD_CTRL_RX_BUF_THLD, I3C_CORE_DATA_BUFFER_THLD_CTRL(base));
}

/*******************************************************************************
* Function Name: Cy_I3C_SetTxEmptyThldLevel
****************************************************************************//**
*
* Sets the TX FIFO level. When the number of empty locations in the TX FIFO is
* greater than this level, the TX FIFO level interrupt is triggered.
*
* \param base
* The pointer to the I3C instance.
*
* \param level
* When there are more empty locations in the TX FIFO than this level, 
* the TX level interrupt is triggered.
*
*******************************************************************************/
__STATIC_INLINE void Cy_I3C_SetTxEmptyThldLevel(CySCB_Type const *base, uint32_t level)
{
    CY_REG32_CLR_SET(I3C_CORE_DATA_BUFFER_THLD_CTRL(base), I3C_CORE_DATA_BUFFER_THLD_CTRL_TX_EMPTY_BUF_THLD, level);
}

/** \} group_i3c_general_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXI3C */

#endif /* (CY_I3C_H) */

/** \} group_i3c */

/* [] END OF FILE */

