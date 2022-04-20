/**
 * @file    uart.h
 * @brief   Serial Peripheral Interface (UART) communications driver.
 */

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */
 
 /* Define to prevent redundant inclusion */
#ifndef _MXC_UART_H_
#define _MXC_UART_H_

/***** Definitions *****/
#include "uart_regs.h"
#include "mxc_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup uart UART
 * @ingroup periphlibs
 * @{
 */

typedef struct _mxc_uart_req_t mxc_uart_req_t;
/**
 * @brief   The list of UART stop bit lengths supported
 * 
 */
typedef enum {
    MXC_UART_STOP_1,    ///< UART Stop 1 clock cycle 
    MXC_UART_STOP_2,    ///< UART Stop 2 clock cycle (1.5 clocks for 5 bit characters)
} mxc_uart_stop_t;

/**
 * @brief   The list of UART Parity options supported
 * 
 */
typedef enum {
    MXC_UART_PARITY_DISABLE,    ///< UART Parity Disabled
    MXC_UART_PARITY_EVEN_0,     ///< UART Parity Even, 0 based
    MXC_UART_PARITY_EVEN_1,     ///< UART Parity Even, 1 based
    MXC_UART_PARITY_ODD_0,      ///< UART Parity Odd, 0 based
    MXC_UART_PARITY_ODD_1,      ///< UART Parity Odd, 1 based
} mxc_uart_parity_t;

/**
 * @brief   The list of UART flow control options supported
 * 
 */
typedef enum {
    MXC_UART_FLOW_DIS,      ///< UART Flow Control Disabled
    MXC_UART_FLOW_EN,       ///< UART Flow Control Enabled
} mxc_uart_flow_t;

/**
 * @brief      Clock settings */
typedef enum {
    MXC_UART_APB_CLK = 0,                                          
    MXC_UART_EXT_CLK = 1,              
    /*8M (IBRO) and 32M (EFRO) clocks can be used for UARTs 0,1 and 2*/
    MXC_UART_IBRO_CLK = 2,  
    MXC_UART_ERFO_CLK = 3,
    /*32K (ERTCO) and INRO clocks can only be used for UART3*/
    MXC_UART_ERTCO_CLK = 4,
    MXC_UART_INRO_CLK = 5,                   
} mxc_uart_clock_t;

/**
 * @brief   The callback routine used to indicate the transaction has terminated.
 *
 * @param   req          The details of the transaction.
 * @param   result       See \ref MXC_Error_Codes for the list of error codes.
 */
typedef void (*mxc_uart_complete_cb_t)(mxc_uart_req_t* req, int result);

/**
 * @brief   The callback routine used to indicate the transaction has terminated.
 *
 * @param   req          The details of the transaction.
 * @param   num          The number of characters actually copied
 * @param   result       See \ref MXC_Error_Codes for the list of error codes.
 */
typedef void (*mxc_uart_dma_complete_cb_t)(mxc_uart_req_t* req, int num, int result);

/**
 * @brief   The information required to perform a complete UART transaction
 *
 * This structure is used by blocking, async, and DMA based transactions.
 * @note    "callback" only needs to be initialized for interrupt driven (Async) and DMA transactions.
 */
struct _mxc_uart_req_t {
    mxc_uart_regs_t* uart;      ///<Point to UART registers
    const uint8_t  *txData;     ///< Buffer containing transmit data. For character sizes
                                ///< < 8 bits, pad the MSB of each byte with zeros. For 
                                ///< character sizes > 8 bits, use two bytes per character
                                ///< and pad the MSB of the upper byte with zeros
    uint8_t        *rxData;     ///< Buffer to store received data For character sizes
                                ///< < 8 bits, pad the MSB of each byte with zeros. For 
                                ///< character sizes > 8 bits, use two bytes per character
                                ///< and pad the MSB of the upper byte with zeros
    uint32_t        txLen;      ///< Number of bytes to be sent from txData
    uint32_t        rxLen;      ///< Number of bytes to be stored in rxData
    volatile uint32_t        txCnt;      ///< Number of bytes actually transmitted from txData
    volatile uint32_t        rxCnt;      ///< Number of bytes stored in rxData

    mxc_uart_complete_cb_t callback;  ///< Pointer to function called when transaction is complete
};

/***** Function Prototypes *****/

/* ************************************************************************* */
/* Control/Configuration functions                                           */
/* ************************************************************************* */

/**
 * @brief   Initialize and enable UART peripheral.
 * 
 * This function initializes everything necessary to call a UART transaction function.
 * Some parameters are set to defaults as follows:
 * UART Data Size    - 8 bits
 * UART Stop Bits    - 1 bit
 * UART Parity       - None
 * UART Flow Control - None
 * 
 * These parameters can be modified after initialization using low level functions
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   baud         The requested clock frequency. The actual clock frequency
 *                       will be returned by the function if successful.
 * @param   clock        Clock source
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_Init(mxc_uart_regs_t* uart, unsigned int baud, mxc_uart_clock_t clock, sys_map_t map);

/**
 * @brief   Disable and shutdown UART peripheral.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_Shutdown(mxc_uart_regs_t* uart);

/**
 * @brief   Checks if the given UART bus can be placed in sleep more.
 *
 * @note    This functions checks to see if there are any on-going UART transactions in
 *          progress. If there are transactions in progress, the application should
 *          wait until the UART bus is free before entering a low-power state.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  #E_NO_ERROR if ready, and non-zero if busy or error. See \ref
 *          MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_ReadyForSleep(mxc_uart_regs_t* uart);

/**
 * @brief   Set the frequency of the UART interface.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   baud         The desired baud rate
 * @param   clock        Clock source
 *
 * @return  Negative if error, otherwise actual speed set. See \ref
 *          MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_SetFrequency(mxc_uart_regs_t* uart, unsigned int baud, mxc_uart_clock_t clock);

/**
 * @brief   Get the frequency of the UART interface.
 *
 * @note    This function is applicable in Master mode only
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  The UART baud rate
 */
int MXC_UART_GetFrequency(mxc_uart_regs_t* uart);

/**
 * @brief   Sets the number of bits per character
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   dataSize     The number of bits per character (5-8 bits/character are valid)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_SetDataSize(mxc_uart_regs_t* uart, int dataSize);

/**
 * @brief   Sets the number of stop bits sent at the end of a character
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   stopBits     The number of stop bits used
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_SetStopBits(mxc_uart_regs_t* uart, mxc_uart_stop_t stopBits);

/**
 * @brief   Sets the type of parity generation used
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   parity       see \ref UART Parity Types for details
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_SetParity(mxc_uart_regs_t* uart, mxc_uart_parity_t parity);

/**
 * @brief   Sets the flow control used
 * 
 * @param   uart            Pointer to UART registers (selects the UART block used.)
 * @param   flowCtrl        see \ref UART Flow Control Types for details
 * @param   rtsThreshold    Number of bytes remaining in the RX FIFO when RTS is asserted
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_SetFlowCtrl(mxc_uart_regs_t* uart, mxc_uart_flow_t flowCtrl, int rtsThreshold, sys_map_t map);

/**
 * @brief   Sets the clock source for the baud rate generator
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   clock        Clock source
 *
 * @return  Actual baud rate if successful, otherwise see \ref MXC_Error_Codes 
 *          for a list of return codes.
 */
int MXC_UART_SetClockSource(mxc_uart_regs_t* uart, mxc_uart_clock_t clock);

/* ************************************************************************* */
/* Low-level functions                                                       */
/* ************************************************************************* */

/**
 * @brief   Checks the UART Peripheral for an ongoing transmission
 *
 * @note    This function is applicable in Master mode only
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  Active/Inactive, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_GetActive(mxc_uart_regs_t* uart);

/**
 * @brief   Aborts an ongoing UART Transmission
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_AbortTransmission(mxc_uart_regs_t* uart);

/**
 * @brief   Reads the next available character. If no character is available, this function
 *          will return an error.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  The character read, otherwise see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_ReadCharacterRaw(mxc_uart_regs_t* uart);

/**
 * @brief   Writes a character on the UART. If the character cannot be written because the
 *          transmit FIFO is currently full, this function returns an error.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   character         The character to write
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_WriteCharacterRaw (mxc_uart_regs_t* uart, uint8_t character);

/**
 * @brief   Reads the next available character
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  The character read, otherwise see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_ReadCharacter(mxc_uart_regs_t* uart);

/**
 * @brief   Writes a character on the UART
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   character    The character to write 
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_WriteCharacter(mxc_uart_regs_t* uart, uint8_t character);

/**
 * @brief   Reads the next available character
 * @note    This function blocks until len characters are received
 *          See MXC_UART_TransactionAsync() for a non-blocking version
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   buffer       Buffer to store data in
 * @param   len          Number of characters 
 *
 * @return  The character read, otherwise see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_Read(mxc_uart_regs_t* uart, uint8_t* buffer, int* len);

/**
 * @brief   Writes a byte on the UART
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   byte         The buffer of characters to write 
 * @param   len          The number of characters to write
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_Write(mxc_uart_regs_t* uart, const uint8_t* byte, int* len);

/**
 * @brief   Unloads bytes from the receive FIFO.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   bytes        The buffer to read the data into.
 * @param   len          The number of bytes to read.
 *
 * @return  The number of bytes actually read.
 */
unsigned int MXC_UART_ReadRXFIFO(mxc_uart_regs_t* uart, unsigned char* bytes,
                                    unsigned int len);

/**
 * @brief   Unloads bytes from the receive FIFO user DMA for longer reads.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   bytes        The buffer to read the data into.
 * @param   len          The number of bytes to read.
 * @param   callback     The function to call when the read is complete
 *
 * @return  See \ref MXC_ERROR_CODES for a list of return values
 */
int MXC_UART_ReadRXFIFODMA(mxc_uart_regs_t* uart, unsigned char* bytes,
                            unsigned int len, mxc_uart_dma_complete_cb_t callback);

/**
 * @brief   Get the number of bytes currently available in the receive FIFO.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  The number of bytes available.
 */
unsigned int MXC_UART_GetRXFIFOAvailable(mxc_uart_regs_t* uart);

/**
 * @brief   Loads bytes into the transmit FIFO.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   bytes        The buffer containing the bytes to write
 * @param   len          The number of bytes to write.
 *
 * @return  The number of bytes actually written.
 */
unsigned int MXC_UART_WriteTXFIFO(mxc_uart_regs_t* uart, const unsigned char* bytes,
                                    unsigned int len);

/**
 * @brief   Loads bytes into the transmit FIFO using DMA for longer writes
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   bytes        The buffer containing the bytes to write
 * @param   len          The number of bytes to write.
 * @param   callback     The function to call when the write is complete
 *
 * @return  See \ref MXC_ERROR_CODES for a list of return values
 */
int MXC_UART_WriteTXFIFODMA(mxc_uart_regs_t* uart, const unsigned char* bytes,
                                unsigned int len, mxc_uart_dma_complete_cb_t callback);

/**
 * @brief   Get the amount of free space available in the transmit FIFO.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 *
 * @return  The number of bytes available.
 */
unsigned int MXC_UART_GetTXFIFOAvailable(mxc_uart_regs_t* uart);

/**
 * @brief   Removes and discards all bytes currently in the receive FIFO.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_ClearRXFIFO(mxc_uart_regs_t* uart);

/**
 * @brief   Removes and discards all bytes currently in the transmit FIFO.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_ClearTXFIFO(mxc_uart_regs_t* uart);

/**
 * @brief   Set the receive threshold level.
 * 
 * @note    RX FIFO Receive threshold. Smaller values will cause
 *          interrupts to occur more often, but reduce the possibility
 *          of losing data because of a FIFO overflow. Larger values
 *          will reduce the time required by the ISR, but increase the 
 *          possibility of data loss. Passing an invalid value will
 *          cause the driver to use the value already set in the 
 *          appropriate register.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   numBytes     The threshold level to set. This value must be
 *                       between 0 and 8 inclusive.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_SetRXThreshold(mxc_uart_regs_t* uart, unsigned int numBytes);

/**
 * @brief   Get the current receive threshold level.
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  The receive threshold value (in bytes).
 */
unsigned int MXC_UART_GetRXThreshold(mxc_uart_regs_t* uart);

/**
 * @brief   Set the transmit threshold level.
 * 
 * @note    TX FIFO threshold. Smaller values will cause interrupts
 *          to occur more often, but reduce the possibility of terminating
 *          a transaction early in master mode, or transmitting invalid data
 *          in slave mode. Larger values will reduce the time required by
 *          the ISR, but increase the possibility errors occurring. Passing 
 *          an invalid value will cause the driver to use the value already 
 *          set in the appropriate register.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   numBytes     The threshold level to set.  This value must be
 *                       between 0 and 8 inclusive.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_UART_SetTXThreshold(mxc_uart_regs_t* uart, unsigned int numBytes);

/**
 * @brief   Get the current transmit threshold level.
 * 
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  The transmit threshold value (in bytes).
 */
unsigned int MXC_UART_GetTXThreshold(mxc_uart_regs_t* uart);

/**
 * @brief   Gets the interrupt flags that are currently set
 *
 * @note    These functions should not be used while using non-blocking Transaction Level 
 *          functions (Async or DMA)
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return The interrupt flags
 */
unsigned int MXC_UART_GetFlags(mxc_uart_regs_t* uart);

/**
 * @brief   Clears the interrupt flags that are currently set
 *
 * @note    These functions should not be used while using non-blocking Transaction Level 
 *          functions (Async or DMA)
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   flags        mask of flags to clear
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_ClearFlags(mxc_uart_regs_t* uart, unsigned int flags);

/**
 * @brief   Enables specific interrupts
 *
 * @note    These functions should not be used while using non-blocking Transaction Level 
 *          functions (Async or DMA)
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   mask         The interrupts to be enabled
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_EnableInt(mxc_uart_regs_t* uart, unsigned int mask);

/**
 * @brief   Disables specific interrupts
 *
 * @note    These functions should not be used while using non-blocking Transaction Level 
 *          functions (Async or DMA)
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * @param   mask         The interrupts to be disabled
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_DisableInt(mxc_uart_regs_t* uart, unsigned int mask);

/**
 * @brief   Gets the status flags that are currently set
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  The status flags
 */
unsigned int MXC_UART_GetStatus(mxc_uart_regs_t* uart);

/* ************************************************************************* */
/* Transaction level functions                                               */
/* ************************************************************************* */

/**
 * @brief   Performs a blocking UART transaction.   
 * 
 * @note    Performs a blocking UART transaction as follows.
 *          If tx_len is non-zero, transmit TX data
 *          Once tx_len has been sent, if rx_len is non-zero, receive data
 *
 * @param   req          Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_Transaction(mxc_uart_req_t* req);

/**
 * @brief   Setup an interrupt-driven UART transaction
 * 
 * @note    The TX FIFO will be filled with txData if necessary
 *          Relevant interrupts will be enabled
 *
 * @param   req          Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_TransactionAsync(mxc_uart_req_t* req);

/**
 * @brief   Setup a DMA driven UART transaction
 * 
 * @note    The TX FIFO will be filled with txData if necessary
 *          Relevant interrupts will be enabled
 *          The DMA channel indicated by the request will be set up to load/unload the FIFOs 
 *          with as few interrupt-based events as possible. The channel will be reset and 
 *          returned to the system at the end of the transaction.
 *
 * @param   req          Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_TransactionDMA(mxc_uart_req_t* req);

/**
 * @brief   The processing function for DMA transactions.
 * 
 * When using the DMA functions, the application must call this
 * function periodically. This can be done from within the DMA Interrupt Handler.
 *
 * @param   ch          DMA channel
 * @param   error       Error status
 */
void MXC_UART_DMACallback (int ch, int error);

/**
 * @brief   Async callback 
 *
 * @param   uart      The uart
 * @param   retVal    The ret value
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_AsyncCallback (mxc_uart_regs_t* uart, int retVal);

/**
 * @brief   stop any async callbacks
 *
 * @param   uart      The uart
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_AsyncStop (mxc_uart_regs_t* uart);

/**
 * @brief   Abort any asynchronous requests in progress.
 *
 * @note    Abort any asynchronous requests in progress. Any callbacks associated with
 *          the active transaction will be executed to indicate when the transaction
 *          has been terminated.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_AbortAsync(mxc_uart_regs_t* uart);

/**
 * @brief   The processing function for asynchronous transactions.
 *
 * @note    When using the asynchronous functions, the application must call this
 *          function periodically. This can be done from within the UART interrupt
 *          handler or periodically by the application if UART interrupts are disabled.
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_UART_AsyncHandler(mxc_uart_regs_t* uart);

/**
 * @brief   Provide TXCount for asynchronous transactions..
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  Returns transmit bytes (in FIFO).
 */
uint32_t MXC_UART_GetAsyncTXCount(mxc_uart_req_t* req);

/**
 * @brief   Provide RXCount for asynchronous transactions..
 *
 * @param   uart         Pointer to UART registers (selects the UART block used.)
 * 
 * @return  Returns receive bytes (in FIFO).
 */
uint32_t MXC_UART_GetAsyncRXCount(mxc_uart_req_t* req);

/**@} end of group uart */

#ifdef __cplusplus
}
#endif

#endif /* _MXC_UART_H_ */
