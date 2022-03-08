/**
 * @file    spi.h
 * @brief   Serial Peripheral Interface (SPI) communications driver.
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
#ifndef _SPI_H_
#define _SPI_H_

/***** includes *******/
#include "spi_regs.h"
#include "mxc_sys.h"
#include "mxc_assert.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "mxc_lock.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup spi SPI
 * @ingroup periphlibs
 * @{
 */

/***** Definitions *****/

/**
 * @brief   The list of SPI Widths supported
 *
 * The SPI Width can be set on a per-transaction basis.
 * An example use case of SPI_WIDTH_STANDARD_HALFDUPLEX is
 * given.
 *
 * Using a MAX31865 RTD-to-SPI IC, read back the temperature
 * The IC requires a SPI Read to be executed as
 * 1. Assert SS
 * 2. Write an 8bit register address
 * 3. Read back the 8 bit register
 * 4. Deassert SS
 * This can be accomplished with the STANDARD_HALFDUPLEX width
 * 1. set txData to the address, txLen=1
 * 2. set rxData to a buffer of 1 byte, rxLen=1
 * 3. The driver will transmit the txData, and after completion of
 *    txData begin to recieve data, padding MOSI with DefaultTXData
 *
 */
typedef enum {
    SPI_WIDTH_3WIRE,                ///< 1 Data line, half duplex
    SPI_WIDTH_STANDARD,             ///< MISO/MOSI, full duplex
    SPI_WIDTH_DUAL,                 ///< 2 Data lines, half duplex
    SPI_WIDTH_QUAD,                 ///< 4 Data lines, half duplex
} mxc_spi_width_t;

/**
 * @brief The list of SPI modes
 * 
 * SPI supports four combinations of clock and phase polarity
 * 
 * Clock polarity is controlled using the bit SPIn_CTRL2.cpol 
 * and determines if the clock is active high or active low
 * 
 * Clock phase determines when the data must be stable for sampling
 *  
 */
typedef enum {
    SPI_MODE_0,                     ///< clock phase = 0, clock polarity = 0 
    SPI_MODE_1,                     ///< clock phase = 0, clock polarity = 1
    SPI_MODE_2,                     ///< clock phase = 1, clock polarity = 0
    SPI_MODE_3,                     ///< clock phase = 1, clock polarity = 1
} mxc_spi_mode_t;

typedef struct _mxc_spi_req_t mxc_spi_req_t;

/**
 * @brief   The callback routine used to indicate the transaction has terminated.
 *
 * @param   req         The details of the transaction.
 * @param   result      See \ref MXC_Error_Codes for the list of error codes.
 */
typedef void (*spi_complete_cb_t) (void * req, int result);

/**
 * @brief   The information required to perform a complete SPI transaction
 *
 * This structure is used by blocking, async, and DMA based transactions.
 * @note    "completeCB" only needs to be initialized for interrupt driven (Async) and DMA transactions.
 */
struct _mxc_spi_req_t {
    mxc_spi_regs_t*     spi;            ///<Point to SPI registers
    int                 ssIdx;          ///< Slave select line to use (Master only, ignored in slave mode)
    int                 ssDeassert;     ///< 1 - Deassert SS at end of transaction, 0 - leave SS asserted
    uint8_t            *txData;         ///< Buffer containing transmit data. For character sizes
    ///< < 8 bits, pad the MSB of each byte with zeros. For
    ///< character sizes > 8 bits, use two bytes per character
    ///< and pad the MSB of the upper byte with zeros
    uint8_t            *rxData;         ///< Buffer to store received data For character sizes
    ///< < 8 bits, pad the MSB of each byte with zeros. For
    ///< character sizes > 8 bits, use two bytes per character
    ///< and pad the MSB of the upper byte with zeros
    uint32_t            txLen;          ///< Number of bytes to be sent from txData
    uint32_t            rxLen;          ///< Number of bytes to be stored in rxData
    uint32_t            txCnt;          ///< Number of bytes actually transmitted from txData
    uint32_t            rxCnt;          ///< Number of bytes stored in rxData

    spi_complete_cb_t   completeCB;     ///< Pointer to function called when transaction is complete
};

/* ************************************************************************* */
/* Control/Configuration functions                                           */
/* ************************************************************************* */

/**
 * @brief   Initialize and enable SPI peripheral.
 *
 * This function initializes everything necessary to call a SPI transaction function.
 * Some parameters are set to defaults as follows:
 * SPI Mode - 0
 * SPI Width - SPI_WIDTH_STANDARD (even if quadModeUsed is set)
 *
 * These parameters can be modified after initialization using low level functions
 *
 * @param   spi             Pointer to SPI registers (selects the SPI block used.)
 * @param   masterMode      Whether to put the device in master or slave mode. Use
 *                          non-zero for master mode, and zero for slave mode.
 * @param   quadModeUsed    Whether to obtain control of the SDIO2/3 pins. Use
 *                          non-zero if the pins are needed (if Quad Mode will
 *                          be used), and zero if they are not needed (quad mode
 *                          will never be used).
 * @param   numSlaves       The number of slaves used, if in master mode. This
 *                          is used to obtain control of the necessary SS pins.
 *                          In slave mode this is ignored and SS1 is used.
 * @param   ssPolarity      This field sets the SS active polarity for each
 *                          slave, each bit position corresponds to each SS line.
 * @param   hz              The requested clock frequency. The actual clock frequency
 *                          will be returned by the function if successful. Used in
 *                          master mode only.
 *
 * @return  If successful, the actual clock frequency is returned. Otherwise, see
 *          \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_Init (mxc_spi_regs_t* spi, int masterMode, int quadModeUsed, int numSlaves,
                  unsigned ssPolarity, unsigned int hz, unsigned int drv_ssel);

/**
 * @brief   Disable and shutdown SPI peripheral.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_Shutdown (mxc_spi_regs_t* spi);

/**
 * @brief   Checks if the given SPI bus can be placed in sleep mode.
 *
 * This functions checks to see if there are any on-going SPI transactions in
 * progress. If there are transactions in progress, the application should
 * wait until the SPI bus is free before entering a low-power state.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  #E_NO_ERROR if ready, and non-zero if busy or error. See \ref
 *          MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_ReadyForSleep (mxc_spi_regs_t* spi);

/**
 * @brief   Returns the frequency of the clock used as the bit rate generator for a given SPI instance.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Frequency of the clock used as the bit rate generator
 */
int MXC_SPI_GetPeripheralClock(mxc_spi_regs_t* spi);

/**
 * @brief   Set the frequency of the SPI interface.
 *
 * This function is applicable in Master mode only
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   hz          The desired frequency in Hertz.
 *
 * @return  Negative if error, otherwise actual speed set. See \ref
 *          MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_SetFrequency (mxc_spi_regs_t* spi, unsigned int hz);

/**
 * @brief   Get the frequency of the SPI interface.
 *
 * This function is applicable in Master mode only
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  The SPI bus frequency in Hertz
 */
unsigned int MXC_SPI_GetFrequency (mxc_spi_regs_t* spi);

/**
 * @brief   Sets the number of bits per character
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   dataSize    The number of bits per character
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_SetDataSize (mxc_spi_regs_t* spi, int dataSize);

/**
 * @brief   Gets the number of bits per character
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_GetDataSize (mxc_spi_regs_t* spi);


/* ************************************************************************* */
/* Low-level functions                                                       */
/* ************************************************************************* */

/**
 * @brief   Sets the slave select (SS) line used for transmissions
 *
 * This function is applicable in Master mode only
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   ssIdx       Slave select index
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_SetSlave (mxc_spi_regs_t* spi, int ssIdx);

/**
 * @brief   Gets the slave select (SS) line used for transmissions
 *
 * This function is applicable in Master mode only
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  slave slect
 */
int MXC_SPI_GetSlave (mxc_spi_regs_t* spi);

/**
 * @brief   Sets the SPI width used for transmissions
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   spiWidth    SPI Width (3-Wire, Standard, Dual SPI, Quad SPI)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_SetWidth (mxc_spi_regs_t* spi, mxc_spi_width_t spiWidth);

/**
 * @brief   Gets the SPI width used for transmissions
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Spi Width   \ref mxc_spi_width_t
 */
mxc_spi_width_t MXC_SPI_GetWidth (mxc_spi_regs_t* spi);

/**
 * @brief   Sets the spi mode using clock polarity and clock phase
 * 
 * @param spi           Pointer to SPI registers (selects the SPI block used.)
 * @param spiMode       \ref mxc_spi_mode_t
 *  
 * @return Success/Fail, see \ref MXC_Error_Codes for a list of return codes. 
 */
int MXC_SPI_SetMode (mxc_spi_regs_t* spi, mxc_spi_mode_t spiMode);

/**
 * @brief   Gets the spi mode
 * 
 * @param spi           Pointer to SPI registers (selects the SPI block used.)
 * 
 * @return mxc_spi_mode_t   \ref mxc_spi_mode_t
 */
mxc_spi_mode_t MXC_SPI_GetMode (mxc_spi_regs_t* spi);

/**
 * @brief   Starts a SPI Transmission
 *
 * This function is applicable in Master mode only
 *
 * The user must ensure that there are no ongoing transmissions before
 * calling this function
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_StartTransmission (mxc_spi_regs_t* spi);

/**
 * @brief   Checks the SPI Peripheral for an ongoing transmission
 *
 * This function is applicable in Master mode only
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Active/Inactive, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_GetActive (mxc_spi_regs_t* spi);

/**
 * @brief   Aborts an ongoing SPI Transmission
 *
 * This function is applicable in Master mode only
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_AbortTransmission (mxc_spi_regs_t* spi);

/**
 * @brief   Unloads bytes from the receive FIFO.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   bytes       The buffer to read the data into.
 * @param   len         The number of bytes to read.
 *
 * @return  The number of bytes actually read.
 */
unsigned int MXC_SPI_ReadRXFIFO (mxc_spi_regs_t* spi, unsigned char* bytes,
                                 unsigned int len);

/**
 * @brief   Get the number of bytes currently available in the receive FIFO.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  The number of bytes available.
 */
unsigned int MXC_SPI_GetRXFIFOAvailable (mxc_spi_regs_t* spi);

/**
 * @brief   Loads bytes into the transmit FIFO.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   bytes       The buffer containing the bytes to write
 * @param   len         The number of bytes to write.
 *
 * @return  The number of bytes actually written.
 */
unsigned int MXC_SPI_WriteTXFIFO (mxc_spi_regs_t* spi, unsigned char* bytes,
                                  unsigned int len);

/**
 * @brief   Get the amount of free space available in the transmit FIFO.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  The number of bytes available.
 */
unsigned int MXC_SPI_GetTXFIFOAvailable (mxc_spi_regs_t* spi);

/**
 * @brief   Removes and discards all bytes currently in the receive FIFO.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 */
void MXC_SPI_ClearRXFIFO (mxc_spi_regs_t* spi);

/**
 * @brief   Removes and discards all bytes currently in the transmit FIFO.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 */
void MXC_SPI_ClearTXFIFO (mxc_spi_regs_t* spi);

/**
 * @brief   Set the receive threshold level.
 *
 * RX FIFO Receive threshold. Smaller values will cause
 * interrupts to occur more often, but reduce the possibility
 * of losing data because of a FIFO overflow. Larger values
 * will reduce the time required by the ISR, but increase the
 * possibility of data loss. Passing an invalid value will
 * cause the driver to use the value already set in the
 * appropriate register.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   numBytes    The threshold level to set. This value must be
 *                      between 0 and 8 inclusive.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_SetRXThreshold (mxc_spi_regs_t* spi, unsigned int numBytes);

/**
 * @brief   Get the current receive threshold level.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  The receive threshold value (in bytes).
 */
unsigned int MXC_SPI_GetRXThreshold (mxc_spi_regs_t* spi);

/**
 * @brief   Set the transmit threshold level.
 *
 * TX FIFO threshold. Smaller values will cause interrupts
 * to occur more often, but reduce the possibility of terminating
 * a transaction early in master mode, or transmitting invalid data
 * in slave mode. Larger values will reduce the time required by
 * the ISR, but increase the possibility errors occurring. Passing
 * an invalid value will cause the driver to use the value already
 * set in the appropriate register.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   numBytes    The threshold level to set.  This value must be
 *                      between 0 and 8 inclusive.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_SetTXThreshold (mxc_spi_regs_t* spi, unsigned int numBytes);

/**
 * @brief   Get the current transmit threshold level.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return  The transmit threshold value (in bytes).
 */
unsigned int MXC_SPI_GetTXThreshold (mxc_spi_regs_t* spi);

/**
 * @brief   Gets the interrupt flags that are currently set
 *
 * These functions should not be used while using non-blocking Transaction Level
 * functions (Async or DMA)
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 *
 * @return The interrupt flags
 */
unsigned int MXC_SPI_GetFlags (mxc_spi_regs_t* spi);

/**
 * @brief   Clears the interrupt flags that are currently set
 *
 * These functions should not be used while using non-blocking Transaction Level
 * functions (Async or DMA)
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 */
void MXC_SPI_ClearFlags (mxc_spi_regs_t* spi);

/**
 * @brief   Enables specific interrupts
 *
 * These functions should not be used while using non-blocking Transaction Level
 * functions (Async or DMA)
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   mask        The interrupts to be enabled
 */
void MXC_SPI_EnableInt (mxc_spi_regs_t* spi, unsigned int mask);

/**
 * @brief   Disables specific interrupts
 *
 * These functions should not be used while using non-blocking Transaction Level
 * functions (Async or DMA)
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 * @param   mask        The interrupts to be disabled
 */
void MXC_SPI_DisableInt (mxc_spi_regs_t* spi, unsigned int mask);

/* ************************************************************************* */
/* Transaction level functions                                               */
/* ************************************************************************* */

/**
 * @brief   Performs a blocking SPI transaction.
 *
 * Performs a blocking SPI transaction.
 * These actions will be performed in Master Mode:
 * 1. Assert the specified SS
 * 2. In Full Duplex Modes, send TX data while receiving RX Data
 *      if rxLen > txLen, pad txData with DefaultTXData
 *      if txLen > rxLen, discard rxData where rxCnt > rxLen
 * 3. In Half Duplex Modes, send TX Data, then receive RX Data
 * 4. Deassert the specified SS
 *
 * These actions will be performed in Slave Mode:
 * 1. Fill FIFO with txData
 * 2. Wait for SS Assert
 * 3. If needed, pad txData with DefaultTXData
 * 4. Unload RX FIFO as needed
 * 5. On SS Deassert, return
 *
 * @param   req         Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_MasterTransaction (mxc_spi_req_t* req);

/**
 * @brief   Setup an interrupt-driven SPI transaction
 *
 * The TX FIFO will be filled with txData, padded with DefaultTXData if necessary
 * Relevant interrupts will be enabled, and relevant registers set (SS, Width, etc)
 *
 * @param   req         Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_MasterTransactionAsync (mxc_spi_req_t* req);

/**
 * @brief   Setup a DMA driven SPI transaction
 *
 * The TX FIFO will be filled with txData, padded with DefaultTXData if necessary
 * Relevant interrupts will be enabled, and relevant registers set (SS, Width, etc)
 *
 * The lowest-indexed unused DMA channel will be acquired (using the DMA API) and
 * set up to load/unload the FIFOs with as few interrupt-based events as
 * possible. The channel will be reset and returned to the system at the end of
 * the transaction.
 *
 * @param   req             Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_MasterTransactionDMA (mxc_spi_req_t* req);

/**
 * @brief   Performs a blocking SPI transaction.
 *
 * Performs a blocking SPI transaction.
 * These actions will be performed in Slave Mode:
 * 1. Fill FIFO with txData
 * 2. Wait for SS Assert
 * 3. If needed, pad txData with DefaultTXData
 * 4. Unload RX FIFO as needed
 * 5. On SS Deassert, return
 *
 * @param   req         Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_SlaveTransaction (mxc_spi_req_t* req);

/**
 * @brief   Setup an interrupt-driven SPI transaction
 *
 * The TX FIFO will be filled with txData, padded with DefaultTXData if necessary
 * Relevant interrupts will be enabled, and relevant registers set (SS, Width, etc)
 *
 * @param   req         Pointer to details of the transactionz
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_SlaveTransactionAsync (mxc_spi_req_t* req);

/**
 * @brief   Setup a DMA driven SPI transaction
 *
 * The TX FIFO will be filled with txData, padded with DefaultTXData if necessary
 * Relevant interrupts will be enabled, and relevant registers set (SS, Width, etc)
 *
 * The lowest-indexed unused DMA channel will be acquired (using the DMA API) and
 * set up to load/unload the FIFOs with as few interrupt-based events as
 * possible. The channel will be reset and returned to the system at the end of
 * the transaction.
 *
 * @param   req             Pointer to details of the transaction
 *
 * @return  See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_SPI_SlaveTransactionDMA (mxc_spi_req_t* req);

/**
 * @brief   Sets the TX data to transmit as a 'dummy' byte
 *
 * In single wire master mode, this data is transmitted on MOSI when performing
 * an RX (MISO) only transaction. This defaults to 0.
 *
 * @param   spi             Pointer to SPI registers (selects the SPI block used.)
 * @param   defaultTXData   Data to shift out in RX-only transactions
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_SPI_SetDefaultTXData (mxc_spi_regs_t* spi, unsigned int defaultTXData);

/**
 * @brief   Abort any asynchronous requests in progress.
 *
 * Abort any asynchronous requests in progress. Any callbacks associated with
 * the active transaction will be executed to indicate when the transaction
 * has been terminated.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 */
void MXC_SPI_AbortAsync (mxc_spi_regs_t* spi);

/**
 * @brief   The processing function for asynchronous transactions.
 *
 * When using the asynchronous functions, the application must call this
 * function periodically. This can be done from within the SPI interrupt
 * handler or periodically by the application if SPI interrupts are disabled.
 *
 * @param   spi         Pointer to SPI registers (selects the SPI block used.)
 */
void MXC_SPI_AsyncHandler (mxc_spi_regs_t* spi);

/**@} end of group spi */

#ifdef __cplusplus
}
#endif

#endif /* _PT_H_ */
