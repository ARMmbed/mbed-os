//*****************************************************************************
//
//  am_hal_uart.h
//! @file
//!
//! @brief Functions for accessing and configuring the UART.
//!
//! @addtogroup uart3 UART
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_HAL_UART_H
#define AM_HAL_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// CMSIS-style macro for handling a variable MSPI module number.
//
#define UARTn(n)    ((UART0_Type*)(UART0_BASE + (n * (UART1_BASE - UART0_BASE))))
//*****************************************************************************

//*****************************************************************************
//
// UART configuration options.
//
//*****************************************************************************
typedef struct
{
    //
    // Standard UART options.
    //
    uint32_t ui32BaudRate;
    uint32_t ui32DataBits;
    uint32_t ui32Parity;
    uint32_t ui32StopBits;
    uint32_t ui32FlowControl;

    //
    // Additional options.
    //
    uint32_t ui32FifoLevels;

    //
    // Buffers
    //
    uint8_t *pui8TxBuffer;
    uint32_t ui32TxBufferSize;
    uint8_t *pui8RxBuffer;
    uint32_t ui32RxBufferSize;
}
am_hal_uart_config_t;

//*****************************************************************************
//
// @brief UART transfer structure.
//
// This structure describes a UART transaction that can be performed by \e
// am_hal_uart_transfer()
//
//*****************************************************************************
typedef struct
{
    //! Determines whether data should be read or written.
    //!
    //! Should be either AM_HAL_UART_WRITE or AM_HAL_UART_READ
    uint32_t ui32Direction;

    //! Pointer to data to be sent, or space to fill with received data.
    uint8_t *pui8Data;

    //! Number of bytes to send or receive.
    uint32_t ui32NumBytes;

    //! Timeout in milliseconds.
    //!
    //! Given a timeout value, the \e am_hal_uart_transfer() function will keep
    //! trying to transfer data until either the number of bytes is satisfied,
    //! or the time runs out. If provided with a value of zero, the transfer
    //! function will only send as much data as it can immediately deal with.
    //! If provided with a timeout value of \e AM_HAL_UART_WAIT_FOREVER, the
    //! function will block until either the final "read" byte is received or
    //! the final "write" byte is placed in the output buffer.
    uint32_t ui32TimeoutMs;

    //! Number of bytes successfully transferred.
    uint32_t *pui32BytesTransferred;
}
am_hal_uart_transfer_t;

//*****************************************************************************
//
// Maximum baudrate supported is 921600 for Apollo3-A1 and 1.5Mbaud for
// Apollo3-B0.
//
//*****************************************************************************
#define AM_HAL_UART_MAXIMUM_BAUDRATE_A1       921600
#define AM_HAL_UART_MAXIMUM_BAUDRATE_B0       1500000

//*****************************************************************************
//
// Uart transfer options.
//
//*****************************************************************************
#define AM_HAL_UART_WRITE                   0
#define AM_HAL_UART_READ                    1
#define AM_HAL_UART_WAIT_FOREVER            0xFFFFFFFF

//*****************************************************************************
//
// UART conficuration option values.
//
//*****************************************************************************

// Data bits.
#define AM_HAL_UART_DATA_BITS_8             (_VAL2FLD(UART0_LCRH_WLEN, 3))
#define AM_HAL_UART_DATA_BITS_7             (_VAL2FLD(UART0_LCRH_WLEN, 2))
#define AM_HAL_UART_DATA_BITS_6             (_VAL2FLD(UART0_LCRH_WLEN, 1))
#define AM_HAL_UART_DATA_BITS_5             (_VAL2FLD(UART0_LCRH_WLEN, 0))

// Parity.
#define AM_HAL_UART_PARITY_NONE             0
#define AM_HAL_UART_PARITY_ODD              UART0_LCRH_PEN_Msk
#define AM_HAL_UART_PARITY_EVEN             (UART0_LCRH_PEN_Msk |   \
                                             UART0_LCRH_EPS_Msk)
// Stop Bits
#define AM_HAL_UART_ONE_STOP_BIT            (_VAL2FLD(UART0_LCRH_STP2, 0))
#define AM_HAL_UART_TWO_STOP_BITS           (_VAL2FLD(UART0_LCRH_STP2, 1))

// Flow control
#define AM_HAL_UART_FLOW_CTRL_NONE          0
#define AM_HAL_UART_FLOW_CTRL_CTS_ONLY      UART0_CR_CTSEN_Msk
#define AM_HAL_UART_FLOW_CTRL_RTS_ONLY      UART0_CR_RTSEN_Msk
#define AM_HAL_UART_FLOW_CTRL_RTS_CTS       (UART0_CR_CTSEN_Msk |   \
                                             UART0_CR_RTSEN_Msk)
// FIFO enable/disable.
#define AM_HAL_UART_FIFO_ENABLE             (_VAL2FLD(UART0_LCRH_FEN, 1))
#define AM_HAL_UART_FIFO_DISABLE            (_VAL2FLD(UART0_LCRH_FEN, 0))

// TX FIFO interrupt level settings.
#define AM_HAL_UART_TX_FIFO_1_8             (_VAL2FLD(UART0_IFLS_TXIFLSEL, 0))
#define AM_HAL_UART_TX_FIFO_1_4             (_VAL2FLD(UART0_IFLS_TXIFLSEL, 1))
#define AM_HAL_UART_TX_FIFO_1_2             (_VAL2FLD(UART0_IFLS_TXIFLSEL, 2))
#define AM_HAL_UART_TX_FIFO_3_4             (_VAL2FLD(UART0_IFLS_TXIFLSEL, 3))
#define AM_HAL_UART_TX_FIFO_7_8             (_VAL2FLD(UART0_IFLS_TXIFLSEL, 4))

// RX FIFO interrupt level settings.
#define AM_HAL_UART_RX_FIFO_1_8             (_VAL2FLD(UART0_IFLS_RXIFLSEL, 0))
#define AM_HAL_UART_RX_FIFO_1_4             (_VAL2FLD(UART0_IFLS_RXIFLSEL, 1))
#define AM_HAL_UART_RX_FIFO_1_2             (_VAL2FLD(UART0_IFLS_RXIFLSEL, 2))
#define AM_HAL_UART_RX_FIFO_3_4             (_VAL2FLD(UART0_IFLS_RXIFLSEL, 3))
#define AM_HAL_UART_RX_FIFO_7_8             (_VAL2FLD(UART0_IFLS_RXIFLSEL, 4))

//*****************************************************************************
//
// UART interrupts.
//
//*****************************************************************************
#define AM_HAL_UART_INT_OVER_RUN            UART0_IER_OEIM_Msk
#define AM_HAL_UART_INT_BREAK_ERR           UART0_IER_BEIM_Msk
#define AM_HAL_UART_INT_PARITY_ERR          UART0_IER_PEIM_Msk
#define AM_HAL_UART_INT_FRAME_ERR           UART0_IER_FEIM_Msk
#define AM_HAL_UART_INT_RX_TMOUT            UART0_IER_RTIM_Msk
#define AM_HAL_UART_INT_TX                  UART0_IER_TXIM_Msk
#define AM_HAL_UART_INT_RX                  UART0_IER_RXIM_Msk
#define AM_HAL_UART_INT_DSRM                UART0_IER_DSRMIM_Msk
#define AM_HAL_UART_INT_DCDM                UART0_IER_DCDMIM_Msk
#define AM_HAL_UART_INT_CTSM                UART0_IER_CTSMIM_Msk
#define AM_HAL_UART_INT_TXCMP               UART0_IER_TXCMPMIM_Msk

//*****************************************************************************
//
//! @name UART Flag Register
//! @brief Macro definitions for UART Flag Register Bits.
//!
//! They may be used with the \e am_hal_uart_flags_get() function.
//!
//! @{
//
//*****************************************************************************
#define AM_HAL_UART_FR_TX_EMPTY             (_VAL2FLD(UART0_FR_TXFE, UART0_FR_TXFE_XMTFIFO_EMPTY))
#define AM_HAL_UART_FR_RX_FULL              (_VAL2FLD(UART0_FR_RXFF, UART0_FR_RXFF_RCVFIFO_FULL))
#define AM_HAL_UART_FR_TX_FULL              (_VAL2FLD(UART0_FR_TXFF, UART0_FR_TXFF_XMTFIFO_FULL))
#define AM_HAL_UART_FR_RX_EMPTY             (_VAL2FLD(UART0_FR_RXFE, UART0_FR_RXFE_RCVFIFO_EMPTY))
#define AM_HAL_UART_FR_BUSY                 (_VAL2FLD(UART0_FR_BUSY, UART0_FR_BUSY_BUSY))
#define AM_HAL_UART_FR_DCD_DETECTED         (_VAL2FLD(UART0_FR_DCD,  UART0_FR_DCD_DETECTED))
#define AM_HAL_UART_FR_DSR_READY            (_VAL2FLD(UART0_FR_DSR,  UART0_FR_DSR_READY))
#define AM_HAL_UART_FR_CTS                  UART0_FR_CTS_Msk

//! @}

//*****************************************************************************
//
// UART FIFO size for Apollo3.
//
//*****************************************************************************
#define AM_HAL_UART_FIFO_MAX                32

//*****************************************************************************
//
// Turn timeouts into indefinite waits.
//
//*****************************************************************************
#define AM_HAL_UART_WAIT_FOREVER            0xFFFFFFFF

//*****************************************************************************
//
//! @brief Initialize the UART interface.
//!
//! @param ui32Module is the module number for the UART to initialize.
//! @param ppHandle is the location to write the UART handle.
//!
//! This function sets internal tracking variables associated with a specific
//! UART module. It should be the first UART API called for each UART module in
//! use. The handle can be used to interact with the UART
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_initialize(uint32_t ui32Module, void **ppHandle);

//*****************************************************************************
//
//! @brief Deinitialize the UART interface.
//!
//! @param pHandle is a previously initialized UART handle.
//!
//! This function effectively disables future calls to interact with the UART
//! refered to by \e pHandle. The user may call this function if UART operation
//! is no longer desired.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_deinitialize(void *pHandle);

//*****************************************************************************
//
//! @brief Change the power state of the UART module.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param ePowerstate is the desired power state of the UART.
//! @parame bRetainState is a flag to ask the HAL to save UART registers.
//!
//! This function can be used to switch the power to the UART on or off. If \e
//! bRetainState is true during a powerdown operation, it will store the UART
//! configuration registers to SRAM, so it can restore them on power-up.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_power_control(void *pHandle,
                                          am_hal_sysctrl_power_state_e ePowerState,
                                          bool bRetainState);

//*****************************************************************************
//
//! @brief Used to configure basic UART settings.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param psConfig is a structure of UART configuration options.
//!
//! This function takes the options from an \e am_hal_uart_config_t structure,
//! and applies them to the UART referred to by \e pHandle.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_configure(void *pHandle,
                                      const am_hal_uart_config_t *psConfig);

extern uint32_t am_hal_uart_configure_fifo(void *pHandle,
                                      const am_hal_uart_config_t *psConfig,
                                      bool bEnableFIFO);

//*****************************************************************************
//
//! @brief Transfer data through the UART interface.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param am_hal_uart_transfer_t is a structure describing the operation.
//!
//! This function executes a transaction as described by the \e
//! am_hal_uart_transfer_t structure. It can either read or write, and it will
//! take advantage of any buffer space provided by the \e
//! am_hal_uart_configure() function.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_transfer(void *pHandle,
                                     const am_hal_uart_transfer_t *pTransfer);

//*****************************************************************************
//
//! @brief Wait for the UART TX to become idle
//!
//! @param pHandle is the handle for the UART to operate on.
//!
//! This function waits (polling) for all data in the UART TX FIFO and UART TX
//! buffer (if configured) to be fully sent on the physical UART interface.
//! This is not the most power-efficient way to wait for UART idle, but it can be
//! useful in simpler applications, or where power-efficiency is less important.
//!
//! Once this function returns, the UART can be safely disabled without
//! interfering with any previous transmissions.
//!
//! For a more power-efficient way to shut down the UART, check the
//! \e am_hal_uart_interrupt_service() function.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_tx_flush(void *pHandle);

//*****************************************************************************
//
//! @brief Read the UART flags.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param pui32Flags is the destination pointer for the UART flags.
//!
//! The UART hardware provides some information about the state of the physical
//! interface at all times. This function provides a way to read that data
//! directly. Below is a list of all possible UART flags.
//!
//! These correspond directly to the bits in the UART_FR register.
//!
//! @code
//!
//! AM_HAL_UART_FR_TX_EMPTY
//! AM_HAL_UART_FR_RX_FULL
//! AM_HAL_UART_FR_TX_FULL
//! AM_HAL_UART_FR_RX_EMPTY
//! AM_HAL_UART_FR_BUSY
//! AM_HAL_UART_FR_DCD_DETECTED
//! AM_HAL_UART_FR_DSR_READY
//! AM_HAL_UART_FR_CTS
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_flags_get(void *pHandle, uint32_t *pui32Flags);

//*****************************************************************************
//
//! @brief Read the UART FIFO directly.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param pui8Data is a pointer where the UART data should be written.
//! @param ui32NumBytes is the number of bytes to transfer.
//! @param pui32NumBytesRead is the nubmer of bytes actually transferred.
//!
//! This function reads the UART FIFO directly, and writes the resulting bytes
//! to pui8Data. Since the UART FIFO hardware has no direct size indicator, the
//! caller can only specify the maximum number of bytes they can handle. This
//! function will try to read as many bytes as possible. At the end of the
//! transfer, the number of bytes actually transferred will be written to the
//! pui32NumBytesRead parameter.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART error.
//
//*****************************************************************************
extern uint32_t am_hal_uart_fifo_read(void *pHandle,
                                      uint8_t *pui8Data,
                                      uint32_t ui32NumBytes,
                                      uint32_t *pui32NumBytesRead);

//*****************************************************************************
//
//! @brief Write the UART FIFO directly.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param pui8Data is a pointer where the UART data should be written.
//! @param ui32NumBytes is the number of bytes to transfer.
//! @param pui32NumBytesWritten is the nubmer of bytes actually transferred.
//!
//! This function reads from pui8Data, and writes the requested number of bytes
//! directly to the UART FIFO. Since the UART FIFO hardware has no register to
//! tell us how much free space it has, the caller can only specify the number
//! of bytes they would like to send. This function will try to write as many
//! bytes as possible up to the requested number. At the end of the transfer,
//! the number of bytes actually transferred will be written to the
//! pui32NumBytesWritten parameter.
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART error.
//
//*****************************************************************************
extern uint32_t am_hal_uart_fifo_write(void *pHandle,
                                       uint8_t *pui8Data,
                                       uint32_t ui32NumBytes,
                                       uint32_t *pui32NumBytesWritten);

//*****************************************************************************
//
//! @brief This function handles the UART buffers during UART interrupts.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param ui32Status is the interrupt status at the time of ISR entry.
//! @param pui32UartTxIdle can be used to store the UART idle status.
//!
//! The main purpose of this function is to manage the UART buffer system. Any
//! buffers configured by \e am_hal_uart_buffer_configure will be managed by
//! this service routine. Data queued for transmit will be added to the UART TX
//! FIFO as space allows, and data stored in the UART RX FIFO will be copied
//! out and stored in the RX buffer. This function will skip this transfer for
//! any buffer that has not been configured.
//!
//! In addition, this function can be used to alert the caller when the UART
//! becomes idle via the optional \e pui32UartTxIdle argument. This function
//! will set this variable any time it completes its operation and the UART TX
//! channel is no longer in use (including both the FIFO and any configured
//! buffer). To make sure this happens as early as possible, the user may
//! enable the UART_TXCMP interrupt as shown below.
//!
//! For RTOS-enabled cases, this function does not necessarily need to be
//! called inside the actual ISR for the UART, but it should be called promptly
//! in response to the receipt of a UART TX, RX, or RX timeout interrupt. If
//! the service routine is not called quickly enough, the caller risks an RX
//! FIFO overflow (data can be lost here), or a TX FIFO underflow (usually not
//! harmful).
//!
//! @code
//!
//! void
//! am_uart_isr(void)
//! {
//!     //
//!     // Service the FIFOs as necessary, and clear the interrupts.
//!     //
//!     uint32_t ui32Status;
//!     uint32_t ui32UartIdle;
//!
//!     am_hal_uart_interrupt_status_get(UART, &ui32Status, true);
//!     am_hal_uart_interrupt_clear(UART, ui32Status);
//!     am_hal_uart_interrupt_service(UART, ui32Status, &ui32UartIdle);
//!
//!     ui32TXDoneFlag = ui32UartIdle;
//! }
//!
//! int
//! main(void)
//! {
//!     ...
//!
//!     // Initialize, power up, and configure the UART.
//!     am_hal_uart_initialize(0, &UART);
//!     am_hal_uart_power_control(UART, AM_HAL_SYSCTRL_WAKE, false);
//!     am_hal_uart_configure(UART, &sUartConfig);
//!
//!     ...
//!
//!     // Enable the UART0 interrupt vector.
//!     am_hal_uart_interrupt_enable(UART, AM_REG_UART_IER_TXCMPMIM_M);
//!     am_hal_interrupt_enable(AM_HAL_INTERRUPT_UART0);
//!     am_hal_interrupt_master_enable();
//! }
//!
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_interrupt_service(void *pHandle,
                                              uint32_t ui32Status,
                                              uint32_t *pui32UartTxIdle);

//*****************************************************************************
//
//! @brief Enable interrupts.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param ui32IntMask is the bitmask of interrupts to enable.
//!
//! This function enables the UART interrupt(s) given by ui32IntMask. If
//! multiple interrupts are desired, they can be OR'ed together.
//!
//! @note This function need not be called for UART FIFO interrupts if the UART
//! buffer service provided by \e am_hal_uart_buffer_configure() and \e
//! am_hal_uart_interrupt_service() is already in use. Non-FIFO-related
//! interrupts do require the use of this function.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_UART_INT_OVER_RUN
//! AM_HAL_UART_INT_BREAK_ERR
//! AM_HAL_UART_INT_PARITY_ERR
//! AM_HAL_UART_INT_FRAME_ERR
//! AM_HAL_UART_INT_RX_TMOUT
//! AM_HAL_UART_INT_TX
//! AM_HAL_UART_INT_RX
//! AM_HAL_UART_INT_DSRM
//! AM_HAL_UART_INT_DCDM
//! AM_HAL_UART_INT_CTSM
//! AM_HAL_UART_INT_TXCMP
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_interrupt_enable(void *pHandle,
                                             uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief Disable interrupts.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param ui32IntMask is the bitmask of interrupts to disable.
//!
//! This function disables the UART interrupt(s) given by ui32IntMask. If
//! multiple interrupts need to be disabled, they can be OR'ed together.
//!
//! @note This function need not be called for UART FIFO interrupts if the UART
//! buffer service provided by \e am_hal_uart_buffer_configure() and \e
//! am_hal_uart_interrupt_service() is already in use. Non-FIFO-related
//! interrupts do require the use of this function.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_UART_INT_OVER_RUN
//! AM_HAL_UART_INT_BREAK_ERR
//! AM_HAL_UART_INT_PARITY_ERR
//! AM_HAL_UART_INT_FRAME_ERR
//! AM_HAL_UART_INT_RX_TMOUT
//! AM_HAL_UART_INT_TX
//! AM_HAL_UART_INT_RX
//! AM_HAL_UART_INT_DSRM
//! AM_HAL_UART_INT_DCDM
//! AM_HAL_UART_INT_CTSM
//! AM_HAL_UART_INT_TXCMP
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_interrupt_disable(void *pHandle,
                                              uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief Clear interrupt status.
//!
//! @param pHandle is the handle for the UART to operate on.
//! @param ui32IntMask is the bitmask of interrupts to clear.
//!
//! This function clears the UART interrupt(s) given by ui32IntMask. If
//! multiple interrupts need to be cleared, they can be OR'ed together.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_UART_INT_OVER_RUN
//! AM_HAL_UART_INT_BREAK_ERR
//! AM_HAL_UART_INT_PARITY_ERR
//! AM_HAL_UART_INT_FRAME_ERR
//! AM_HAL_UART_INT_RX_TMOUT
//! AM_HAL_UART_INT_TX
//! AM_HAL_UART_INT_RX
//! AM_HAL_UART_INT_DSRM
//! AM_HAL_UART_INT_DCDM
//! AM_HAL_UART_INT_CTSM
//! AM_HAL_UART_INT_TXCMP
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_interrupt_clear(void *pHandle,
                                            uint32_t ui32IntMask);

//*****************************************************************************
//
//! @brief Read interrupt status.
//!
//! @param pHandle is the handle for the UART to operate on.
//!
//! @param pui32Status is the returned interrupt status (all bits OR'ed
//! together)
//!
//! @param bEnabled determines whether to read interrupts that were not
//! enabled.
//!
//! This function reads the status the UART interrupt(s) if \e bEnabled is
//! true, it will only return the status of the enabled interrupts. Otherwise,
//! it will return the status of all interrupts, enabled or disabled.
//!
//! The full list of interrupts is given by the following:
//!
//! @code
//!
//! AM_HAL_UART_INT_OVER_RUN
//! AM_HAL_UART_INT_BREAK_ERR
//! AM_HAL_UART_INT_PARITY_ERR
//! AM_HAL_UART_INT_FRAME_ERR
//! AM_HAL_UART_INT_RX_TMOUT
//! AM_HAL_UART_INT_TX
//! AM_HAL_UART_INT_RX
//! AM_HAL_UART_INT_DSRM
//! AM_HAL_UART_INT_DCDM
//! AM_HAL_UART_INT_CTSM
//! AM_HAL_UART_INT_TXCMP
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_interrupt_status_get(void *pHandle,
                                                 uint32_t *pui32Status,
                                                 bool bEnabledOnly);

typedef enum
{
    AM_HAL_UART_STATUS_BUS_ERROR = AM_HAL_STATUS_MODULE_SPECIFIC_START,
    AM_HAL_UART_STATUS_RX_QUEUE_FULL,
    AM_HAL_UART_STATUS_CLOCK_NOT_CONFIGURED,
    AM_HAL_UART_STATUS_BAUDRATE_NOT_POSSIBLE,
}
am_hal_uart_errors_t;

//*****************************************************************************
//
//! @brief Check to see which interrupts are enabled.
//!
//! @param pHandle is the handle for the UART to operate on.
//!
//! @param pui32IntMask is the current set of interrupt enable bits (all bits
//!                     OR'ed together)
//!
//! This function checks the UART Interrupt Enable Register to see which UART
//! interrupts are currently enabled. The result will be an interrupt mask with
//! one bit set for each of the currently enabled UART interrupts.
//!
//! The full set of UART interrupt bits is given by the list below:
//!
//! @code
//!
//! AM_HAL_UART_INT_OVER_RUN
//! AM_HAL_UART_INT_BREAK_ERR
//! AM_HAL_UART_INT_PARITY_ERR
//! AM_HAL_UART_INT_FRAME_ERR
//! AM_HAL_UART_INT_RX_TMOUT
//! AM_HAL_UART_INT_TX
//! AM_HAL_UART_INT_RX
//! AM_HAL_UART_INT_DSRM
//! AM_HAL_UART_INT_DCDM
//! AM_HAL_UART_INT_CTSM
//! AM_HAL_UART_INT_TXCMP
//!
//! @endcode
//!
//! @return AM_HAL_STATUS_SUCCESS or applicable UART errors.
//
//*****************************************************************************
extern uint32_t am_hal_uart_interrupt_enable_get(void *pHandle, uint32_t *pui32IntMask);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_UART_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
