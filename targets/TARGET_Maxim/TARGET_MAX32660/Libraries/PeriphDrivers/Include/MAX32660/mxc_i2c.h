/**
* @file     i2c.h
* @brief    Inter-integrated circuit (I2C) communications interface driver.
*/

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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
#ifndef _MXC_I2C_H_
#define _MXC_I2C_H_

#include <stdint.h>
#include "mxc_sys.h"
#include "i2c_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup i2c I2C
 * @ingroup periphlibs
 * @{
 */

typedef struct _i2c_req_t mxc_i2c_req_t;

/**
 * @brief   The callback used by the MXC_I2C_ReadByteInteractive() function.
 *
 * The callback routine used by the MXC_I2C_ReadByteInteractive() function. This
 * function allows the application to determine whether the byte received
 * should be acknowledged or not.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   byte        The byte received.
 *
 * @return  0 if the byte should not be acknowledged (NACK), non-zero to
 *          acknowledge the byte.
 */
typedef int (*mxc_i2c_getAck_t) (mxc_i2c_regs_t* i2c, unsigned char byte);

/**
 * @brief   The callback routine used by the MXC_I2C_MasterTransactionAsync()
 *          function to indicate the transaction has completed.
 *
 * @param   req         The details of the transaction.
 * @param   result      0 if all bytes are acknowledged, 1 if any byte
 *                      transmitted is not acknowledged, negative if error.
 *                      See \ref MXC_Error_Codes for the list of error codes.
 */
typedef void (*mxc_i2c_complete_cb_t) (mxc_i2c_req_t* req, int result);

/**
 * @brief   The callback routine used by the I2C Read/Write FIFO DMA
 *          functions to indicate the transaction has completed.
 *
 * @param   len         The length of data actually read/written
 * @param   result      See \ref MXC_Error_Codes for the list of error codes.
 */
typedef void (*mxc_i2c_dma_complete_cb_t) (int len, int result);

/**
 * @brief   The information required to perform a complete I2C transaction as
 *          the bus master.
 *
 * The information required to perform a complete I2C transaction as the bus
 * master. This structure is used by the MXC_I2C_MasterTransaction() and
 * MXC_I2C_MasterTransactionAsync() functions.
 */
struct _i2c_req_t {
    mxc_i2c_regs_t* i2c;        ///< Pointer to I2C registers (selects the
    ///< I2C block used.)
    unsigned int addr;          ///< The 7-bit or 10-bit address of the slave.
    unsigned char* tx_buf;      ///< The buffer containing the bytes to write.
    unsigned int tx_len;        ///< The number of bytes to write. On return
    ///< from the function, this will be set to
    ///< the number of bytes actually transmitted.
    unsigned char* rx_buf;      ///< The buffer to read the data into.
    unsigned int rx_len;        ///< The number of bytes to read.  On return
    ///< from the function, this will be set to
    ///< the number of bytes actually received.
    int restart;                ///< Controls whether the transaction is
    ///< terminated with a stop or repeated start
    ///< condition.  Use 0 for a stop, non-zero
    ///< for repeated start.
    mxc_i2c_complete_cb_t callback;    ///< The callback used to indicate the
    ///< transaction is complete or an error has
    ///< occurred. This field may be set to NULL
    ///< if no indication is necessary. This
    ///< field is only used by the
    ///< MXC_I2C_MasterTransactionAsync() function.
    ///< MXC_I2C_MasterTransaction() ignores the
    ///< callback field.
};

/**
 * @brief   The list of events reported by the MXC_I2C_SlaveTransaction() and
 *          MXC_I2C_SlaveTransactionAsync() functions.
 *
 * The list of events reported by the MXC_I2C_SlaveTransaction() and
 * MXC_I2C_SlaveTransactionAsync() functions.  It is up to the calling
 * application to handle these events.
 */
typedef enum {
    MXC_I2C_EVT_MASTER_WR,  ///< A slave address match occurred with the master
    ///< requesting a write to the slave.
    MXC_I2C_EVT_MASTER_RD,  ///< A slave address match occurred with the master
    ///< requesting a read from the slave.
    MXC_I2C_EVT_RX_THRESH,  ///< The receive FIFO contains more bytes than its
    ///< threshold level.
    MXC_I2C_EVT_TX_THRESH,  ///< The transmit FIFO contains fewer bytes than its
    ///< threshold level.
    MXC_I2C_EVT_TRANS_COMP, ///< The transaction has ended.
    MXC_I2C_EVT_UNDERFLOW,  ///< The master has attempted a read when the
    ///< transmit FIFO was empty.
    MXC_I2C_EVT_OVERFLOW,   ///< The master has written data when the receive
    ///< FIFO was already full.
} mxc_i2c_slave_event_t;

/**
 * @brief   The callback routine used by the MXC_I2C_SlaveTransaction() and
 *          MXC_I2C_SlaveTransactionAsync functions to handle the various I2C
 *          slave events.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   event       The event that occurred to trigger this callback.
 * @param   data        This field is used to pass Success/Fail for the
 *                      MXC_I2C_EVT_TRANS_COMP event.
 *
 * @return  The return value is only used in the case of an MXC_I2C_EVT_RX_THRESH
 *          event. In this case, the return specifies if the last byte
 *          received should be acknowledged or not. Return 0 to acknowledge,
 *          non-zero to not acknowledge.  The return value is ignored for all
 *          other event types.
 */
typedef int (*mxc_i2c_slave_handler_t) (mxc_i2c_regs_t* i2c,
                                        mxc_i2c_slave_event_t event, void* data);
                                        
/***** Function Prototypes *****/

/* ************************************************************************* */
/* Control/Configuration functions                                           */
/* ************************************************************************* */

/**
 * @brief   Initialize and enable I2C peripheral.
 * @note    This function sets the I2C Speed to 100kHz,  if another speed is
 *          desired use the MXC_I2C_SetFrequency() function to set it.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   masterMode  Whether to put the device in master or slave mode. Use
 *                      non-zero
 *                      for master mode, and zero for slave mode.
 * @param   slaveAddr   7-bit or 10-bit address to use when in slave mode.
 *                      This parameter is ignored when masterMode is non-zero.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_Init (mxc_i2c_regs_t* i2c, int masterMode, unsigned int slaveAddr);

/**
 * @brief   Set slave address for I2C instances acting as slaves on the bus.
 * @note    Set idx to zero, multiple I2C instances acting as slaves on the
 *          bus is not yet supported.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   slaveAddr   7-bit or 10-bit address to use when in slave mode.
 *                      This parameter is ignored when masterMode is non-zero.
 * @param   idx         Index of the I2C slave. 
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_SetSlaveAddr(mxc_i2c_regs_t* i2c, unsigned int slaveAddr, int idx);

/**
 * @brief   Disable and shutdown I2C peripheral.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_Shutdown (mxc_i2c_regs_t* i2c);

/**
 * @brief   Set the frequency of the I2C interface.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   hz          The desired frequency in Hertz.
 *
 * @return  Negative if error, otherwise actual speed set. See \ref
 *          MXC_Error_Codes for the list of error return codes.
 */
int MXC_I2C_SetFrequency (mxc_i2c_regs_t* i2c, unsigned int hz);

/**
 * @brief   Get the frequency of the I2C interface.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  The I2C bus frequency in Hertz
 */
unsigned int MXC_I2C_GetFrequency (mxc_i2c_regs_t* i2c);

/**
 * @brief   Checks if the given I2C bus can be placed in sleep more.
 *
 * This functions checks to see if there are any on-going I2C transactions in
 * progress. If there are transactions in progress, the application should
 * wait until the I2C bus is free before entering a low-power state.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  #E_NO_ERROR if ready, and non-zero if busy or error. See \ref
 *          MXC_Error_Codes for the list of error return codes.
 */
int MXC_I2C_ReadyForSleep (mxc_i2c_regs_t* i2c);

/**
 * @brief   Enables or disables clock stretching by the slave.
 *
 * Enables or disables clock stretching by the slave. This function has no
 * affect when operating as the master.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   enable      Enables clock stretching if non-zero, disables if zero.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_SetClockStretching (mxc_i2c_regs_t* i2c, int enable);

/**
 * @brief   Determines if clock stretching has been enabled.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  Zero if clock stretching is disabled, non-zero otherwise
 */
int MXC_I2C_GetClockStretching (mxc_i2c_regs_t* i2c);

/* ************************************************************************* */
/* Low-level functions                                                       */
/* ************************************************************************* */

/**
 * @brief   Generate a start (or repeated start) condition on the I2C bus.
 *
 * Generate a start (or repeated start) condition on the I2C bus. This
 * function may opt to delay the actual generation of the start condition
 * until data is actually transferred.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_Start (mxc_i2c_regs_t* i2c);

/**
 * @brief   Generate a stop condition on the I2C bus.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_Stop (mxc_i2c_regs_t* i2c);

/**
 * @brief   Write a single byte to the I2C bus.
 *
 * Write a single byte to the I2C bus. This function assumes the I2C bus is
 * already in the proper state (i.e. a start condition has already been
 * generated and the bus is in the write phase of an I2C transaction). If any
 * bytes are pending in the FIFO (i.e. in the case of clock stretching), this
 * function will return E_OVERFLOW.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   byte        The byte to transmit.
 *
 * @return  0 if byte is acknowledged, 1 if not acknowledged, negative if
 *          error. See \ref MXC_Error_Codes for the list of error return codes.
 */
int MXC_I2C_WriteByte (mxc_i2c_regs_t* i2c, unsigned char byte);

/**
 * @brief   Read a single byte from the I2C bus.
 *
 * Read a single byte from the I2C bus. This function assumes the I2C bus is
 * already in the proper state (i.e. a start condition has already been
 * generated and the bus is in the read phase of an I2C transaction). If the FIFO
 * is empty, this function will return E_UNDERFLOW.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   byte        Pointer to the byte to read into.
 * @param   ack         Whether or not to acknowledge the byte once received.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_ReadByte (mxc_i2c_regs_t* i2c, unsigned char* byte, int ack);

/**
 * @brief   Read a single byte from the I2C bus.
 *
 * Read a single byte from the I2C bus. After the byte is received, the
 * provided callback will be used to determine if the byte should be
 * acknowledged or not before continuing with the rest of the transaction.
 * This function assumes the I2C bus is already in the proper state (i.e. a
 * start condition has already been generated and the bus is in the read
 * phase of an I2C transaction). This function must be called with clock
 * stretching enabled.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   byte        Pointer to the byte to read into.
 * @param   getAck      A function to be called to determine whether or not
 *                      to acknowledge the byte once received.  A non-zero
 *                      return value will acknowledge the byte.  If this
 *                      parameter is set to NULL or its return value is 0,
 *                      the byte received will not be acknowledged (i.e., it
 *                      will be NACKed).
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_ReadByteInteractive (mxc_i2c_regs_t* i2c, unsigned char* byte,
                                 mxc_i2c_getAck_t getAck);
                                 
/**
 * @brief   Write multiple bytes to the I2C bus.
 *
 * Write multiple bytes to the I2C bus.  This function assumes the I2C bus is
 * already in the proper state (i.e. a start condition has already been
 * generated and the bus is in the write phase of an I2C transaction).
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   bytes       The buffer containing the bytes to transmit.
 * @param   len         The number of bytes to write. On return from the
 *                      function, this will be set to the number of bytes
 *                      actually transmitted.
 *
 * @return  0 if all bytes are acknowledged, 1 if any byte transmitted is not
 *          acknowledged, negative if error. See \ref MXC_Error_Codes for the
 *          list of error return codes.
 */
int MXC_I2C_Write (mxc_i2c_regs_t* i2c, unsigned char* bytes, unsigned int* len);

/**
 * @brief   Read multiple bytes from the I2C bus.
 *
 * Read multiple byte from the I2C bus.  This function assumes the I2C bus is
 * already in the proper state (i.e. a start condition has already been
 * generated and the bus is in the read phase of an I2C transaction).
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   bytes       The buffer to read the data into.
 * @param   len         The number of bytes to read. On return from the
 *                      function, this will be set to the number of bytes
 *                      actually received.
 * @param   ack         Whether or not to acknowledge the last byte once it is
 *                      received.  All previous bytes will be acknowledged.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_Read (mxc_i2c_regs_t* i2c, unsigned char* bytes, unsigned int* len,
                  int ack);
                  
/**
 * @brief   Unloads bytes from the receive FIFO.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   bytes       The buffer to read the data into.
 * @param   len         The number of bytes to read.
 *
 * @return  The number of bytes actually read.
 */
int MXC_I2C_ReadRXFIFO (mxc_i2c_regs_t* i2c, volatile unsigned char* bytes,
                                 unsigned int len);
                                 
/**
 * @brief   Unloads bytes from the receive FIFO using DMA for longer reads.
 *
 * @note    The operation is not complete until the callback has been called
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   bytes       The buffer to read the data into.
 * @param   len         The number of bytes to read.
 * @param   callback    The function to call when the read is complete
 *
 * @return  See \ref MXC_Error_Codes for a list of return values.
 */
int MXC_I2C_ReadRXFIFODMA (mxc_i2c_regs_t* i2c, unsigned char* bytes,
                           unsigned int len, mxc_i2c_dma_complete_cb_t callback);
                           
/**
 * @brief   Get the number of bytes currently available in the receive FIFO.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  The number of bytes available.
 */
int MXC_I2C_GetRXFIFOAvailable (mxc_i2c_regs_t* i2c);

/**
 * @brief   Loads bytes into the transmit FIFO.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   bytes       The buffer containing the bytes to write
 * @param   len         The number of bytes to write.
 *
 * @return  The number of bytes actually written.
 */
int MXC_I2C_WriteTXFIFO (mxc_i2c_regs_t* i2c, volatile unsigned char* bytes,
                                  unsigned int len);
                                  
/**
 * @brief   Loads bytes into the transmit FIFO using DMA for longer writes.
 *
 * @note    The operation is not complete until the callback has been called
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   bytes       The buffer containing the bytes to write
 * @param   len         The number of bytes to write.
 * @param   callback    The function to call when the read is complete
 *
 * @return  See \ref MXC_Error_Codes for a list of return values
 */
int MXC_I2C_WriteTXFIFODMA (mxc_i2c_regs_t* i2c, unsigned char* bytes,
                            unsigned int len, mxc_i2c_dma_complete_cb_t callback);
                            
/**
 * @brief   Get the amount of free space available in the transmit FIFO.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  The number of bytes available.
 */
int MXC_I2C_GetTXFIFOAvailable (mxc_i2c_regs_t* i2c);

/**
 * @brief   Removes and discards all bytes currently in the receive FIFO.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_ClearRXFIFO (mxc_i2c_regs_t* i2c);

/**
 * @brief   Removes and discards all bytes currently in the transmit FIFO.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_ClearTXFIFO (mxc_i2c_regs_t* i2c);

/**
 * @brief   Get the presently set interrupt flags.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   flags0      Pointer to the variable to store the current status of the interrupt flags in intfl0.
 * @param   flags1      Pointer to the variable to store the current status of the interrupt flags in intfl0.
 *
 * @return  See \ref MXC_Error_Codes for a list of return values
 */
int MXC_I2C_GetFlags (mxc_i2c_regs_t* i2c, unsigned int *flags0, unsigned int *flags1);

/**
 * @brief   Clears the Interrupt Flags.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   flags0      Flags to clear in the intfl0 interrupt register.
 * @param   flags1      Flags to clear in the intfl1 interrupt register.
 */
void MXC_I2C_ClearFlags (mxc_i2c_regs_t* i2c, unsigned int flags0, unsigned int flags1);

/**
 * @brief   Enable Interrupts.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   flags0      Interrupts to be enabled in int->en0
 * @param   flags1      Interrupts to be enabled in int->en1
 */
void MXC_I2C_EnableInt (mxc_i2c_regs_t* i2c, unsigned int flags0, unsigned int flags1);

/**
 * @brief   Disable Interrupts.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   flags0      Interrupts to be disabled in int->en0
 * @param   flags1      Interrupts to be disabled in int->en1
 */
void MXC_I2C_DisableInt (mxc_i2c_regs_t* i2c, unsigned int flags0, unsigned int flags1);

/**
 * @brief   Enables the slave preload mode
 *
 * Use this mode to preload the slave TX FIFO with data that can be sent when
 * the slave is addressed for a read operation without software intervention.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_EnablePreload (mxc_i2c_regs_t* i2c);

/**
 * @brief   Disable the slave preload mode
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_DisablePreload (mxc_i2c_regs_t* i2c);

/**
 * @brief   Enables the slave to respond to the general call address
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_EnableGeneralCall (mxc_i2c_regs_t* i2c);

/**
 * @brief   Prevents the slave from responding to the general call address
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_DisableGeneralCall (mxc_i2c_regs_t* i2c);

/**
 * @brief   Set the I2C Timeout
 *
 * The I2C timeout determines the amount of time the master will wait while the
 * slave is stretching the clock, and the amount of time the slave will stretch
 * the clock while waiting for software to unload the fifo.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   timeout     Timeout in uS
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
void MXC_I2C_SetTimeout (mxc_i2c_regs_t* i2c, unsigned int timeout);

/**
 * @brief   Get the current I2C timeout
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  The current timeout in uS
 */
unsigned int MXC_I2C_GetTimeout (mxc_i2c_regs_t* i2c);

/**
 * @brief   Attempts to recover the I2C bus, ensuring the I2C lines are idle.
 *
 * Attempts to recover and reset an I2C bus by sending I2C clocks. During
 * each clock cycle, the SDA line is cycled to determine if the master has
 * control of the line. The following steps are performed to create one SCL
 * clock cycle:
 *   1. Drive SCL low
 *   2. Verify SCL is low
 *   3. Drive SDA low
 *   4. Verify SDA is low
 *   5. Release SDA allowing it to return high
 *   6. Verify SDA is high
 *   7. Release SCL allowing it to return high.
 *   8. Verify SCL is high
 * If any of the steps fail, the bus is considered to still be busy and the
 * sequence is repeated up to the requested number of times. If all steps
 * succeed, a final stop condition is generated on the I2C bus.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   retries     Number of times to attempt the clock cycle sequence.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_Recover (mxc_i2c_regs_t* i2c, unsigned int retries);

/* ************************************************************************* */
/* Transaction level functions                                               */
/* ************************************************************************* */

/**
 * @brief   Performs a blocking I2C Master transaction.
 *
 * Performs a blocking I2C transaction.  These actions will be performed:
 *   1. If necessary, generate a start condition on the bus.
 *   2. Send the slave address with the low bit set to 0 (indicating a write).
 *   3. Transmit req->tx_len bytes of req->tx_buff.
 *   4. Generate a repeated start condition on the bus.
 *   5. Send the slave address with the low bit set to 1 (indicating a read).
 *   6. Receive req->rx_len bytes into req->rx_buf, acknowledging each byte.
 *   7. Generate a stop (or repeated start) condition on the bus.
 *   Steps 3-6 will be skipped if req->tx_len and req->rx_len are both 0.
 *   Steps 2-4 will be skipped if req->tx_len equals 0.
 *   Steps 4-6 will be skipped if req->rx_len equals 0.
 *
 * @param   req         Pointer to details of the transaction
 *
 * @return  0 if all bytes are acknowledged, 1 if any byte transmitted is not
 *          acknowledged, negative if error. See \ref MXC_Error_Codes for the
 *          list of error return codes.
 */
int MXC_I2C_MasterTransaction (mxc_i2c_req_t* req);

/**
 * @brief   Performs a non-blocking I2C Master transaction.
 *
 * Performs a non-blocking I2C transaction.  These actions will be performed:
 *   1. If necessary, generate a start condition on the bus.
 *   2. Send the slave address with the low bit set to 0 (indicating a write).
 *   3. Transmit req->tx_len bytes of req->tx_buff.
 *   4. Generate a repeated start condition on the bus.
 *   5. Send the slave address with the low bit set to 1 (indicating a read).
 *   6. Receive req->rx_len bytes into req->rx_buf, acknowledging each byte.
 *   7. Generate a stop (or repeated start) condition on the bus.
 *   8. Execute req->callback to indicate the transaction is complete.
 *   Steps 3-6 will be skipped if tx_len and rx_len are both 0.
 *   Steps 2-4 will be skipped if tx_len equals 0.
 *   Steps 4-6 will be skipped if rx_len equals 0.
 *
 * @note    MXC_I2C_AsyncHandler() must be called periodically for this function
 *          to operate properly. Ideally from the I2C ISR.
 *
 * @param   req         Pointer to details of the transaction.  The memory
 *                      used by this parameter must remain available until
 *                      the callback is executed.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_MasterTransactionAsync (mxc_i2c_req_t* req);

/**
 * @brief   Performs a non-blocking I2C Master transaction using DMA for reduced time
 *          in the ISR.
 *
 * Performs a non-blocking I2C transaction.  These actions will be performed:
 *   1. If necessary, generate a start condition on the bus.
 *   2. Send the slave address with the low bit set to 0 (indicating a write).
 *   3. Transmit req->tx_len bytes of req->tx_buff.
 *   4. Generate a repeated start condition on the bus.
 *   5. Send the slave address with the low bit set to 1 (indicating a read).
 *   6. Receive req->rx_len bytes into req->rx_buf, acknowledging each byte.
 *   7. Generate a stop (or repeated start) condition on the bus.
 *   8. Execute req->callback to indicate the transaction is complete.
 *   Steps 3-6 will be skipped if tx_len and rx_len are both 0.
 *   Steps 2-4 will be skipped if tx_len equals 0.
 *   Steps 4-6 will be skipped if rx_len equals 0.
 *
 * @note    MXC_I2C_AsyncHandler() must be called periodically for this function
 *          to operate properly. Ideally from the I2C ISR.
 *
 * @param   req         Pointer to details of the transaction.  The memory
 *                      used by this parameter must remain available until
 *                      the callback is executed.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_MasterTransactionDMA (mxc_i2c_req_t* req);

/**
 * @brief   Performs a blocking I2C Slave transaction.
 *
 * Performs a blocking I2C transaction. This function will block until a
 * complete transaction with this slave has been performed. A transaction
 * begins with the master addressing the slave and ends with a repeated start
 * condition, a stop condition, or a bus error. The provided callback
 * function will be called for these events:
 *      - A slave address match occurs with the master requesting a write to
 *        the slave.
 *      - A slave address match occurs with the master requesting a read from
 *        the slave.
 *      - The receive FIFO crosses the set threshold (see
 *        MXC_I2C_SetRXThreshold()). The callback code should unload the receive
 *        FIFO (see MXC_I2C_ReadFIFO()) to allow the master to send more data.
 *        The return value of the callback function will determine if the
 *        last byte received should be acknowledged or not. Return 0 to
 *        acknowledge, non-zero to not acknowledge.
 *      - The transmit FIFO crosses the set threshold (see
 *        MXC_I2C_SetTXThreshold()). If the master is expected to read more
 *        data from this slave, the callback code should add data to the
 *        transmit FIFO (see MXC_I2C_WriteFIFO()).
 *      - The transaction ends.  If the master was writing to the slave, the
 *        receive FIFO may still contain valid data that needs to be
 *        retreived (see MXC_I2C_ReadFIFO()).
 *      - The transmit FIFO underflows because the master requests data when
 *        the transmit FIFO is empty.
 *      - The receive FIFO overflows because the master writes data while the
 *        receive FIFO was full.
 *
 * If clock stretching is disabled, careful attention must be paid to the timing
 * of the callback to avoid losing data on write or unintentionally nacking a read.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   callback    The function to be called when an I2C event occurs.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_SlaveTransaction (mxc_i2c_regs_t* i2c, mxc_i2c_slave_handler_t callback);

/**
 * @brief   Performs a non-blocking I2C Slave transaction.
 *
 * Performs a non-blocking I2C transaction. This request will remain active
 * until a complete transaction with this slave has been performed.  A
 * transaction begins with the masterMXC_I2C_MasterTransactionDMA addressing the
 * slave and ends with a repeated start condition, a stop condition, or a bus
 * error. The provided callback function will be called for these events:
 *      - A slave address match occurs with the master requesting a write to
 *        the slave.
 *      - A slave address match occurs with the master requesting a read from
 *        the slave.
 *      - The receive FIFO crosses the set threshold (see
 *        MXC_I2C_SetRXThreshold()). The callback code should unload the receive
 *        FIFO (see MXC_I2C_ReadFIFO()) to allow the master to send more data.
 *        The return value of the callback function will determine if the
 *        last byte received should be acknowledged or not. Return 0 to
 *        acknowledge, non-zero to not acknowledge.
 *      - The transmit FIFO crosses the set threshold (see
 *        MXC_I2C_SetTXThreshold()). If the master is expected to read more
 *        data from this slave, the callback code should add data to the
 *        transmit FIFO (see MXC_I2C_WriteFIFO()).
 *      - The transaction ends.  If the master was writing to the slave, the
 *        receive FIFO may still contain valid data that needs to be
 *        retreived (see MXC_I2C_ReadFIFO()).
 *      - The transmit FIFO underflows because the master requests data when
 *        the transmit FIFO is empty.
 *      - The receive FIFO overflows because the master writes data while the
 *        receive FIFO was full.
 *
 * If clock stretching is disabled, careful attention must be paid to the timing
 * of the callback to avoid losing data on write or unintentionally nacking a read.
 *
 * @note    MXC_I2C_AsyncHandler() must be called peridocally for this function
 *          to operate properly.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   callback    The function to be called when an I2C event occurs.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_SlaveTransactionAsync (mxc_i2c_regs_t* i2c, mxc_i2c_slave_handler_t callback);

/**
 * @brief   Set the receive threshold level.
 *
 * When operating as a master, the function sets the receive threshold level
 * for when the master should unload the receive FIFO. Smaller values may
 * consume more CPU cycles, but decrease the chances of the master delaying
 * the generation of I2C bus clocks because it has no room in the FIFO to
 * receive data. Larger values may consume fewer CPU cycles, but risk delays
 * of the I2C clock. When operating as a slave, this function sets the number
 * of bytes the slave transaction  functions should receive before issuing a
 * call to their callback function. Smaller values may consume more CPU
 * cycles, but reduce the risk of missing data from the master due to the
 * recieve FIFO being full. Larger values may reduce the number of CPU
 * cycles, but may cause bytes sent from the master to be missed.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   numBytes    The threshold level to set. This value must be
 *                      between 0 and 8 inclusive.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_SetRXThreshold (mxc_i2c_regs_t* i2c, unsigned int numBytes);

/**
 * @brief   Get the current receive threshold level.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  The receive threshold value (in bytes).
 */
unsigned int MXC_I2C_GetRXThreshold (mxc_i2c_regs_t* i2c);

/**
 * @brief   Set the transmit threshold level.
 *
 * When operating as a master, the function sets the transmit threshold level
 * for when the master should add additional bytes to the transmit FIFO.
 * Larger values may consume more CPU cycles, but decrease the chances of the
 * master delaying the generation of I2C bus clocks because it has no data in
 * the FIFO to transmit. Smaller values may consume fewer CPU cycles, but
 * risk delays of the I2C clock. When operating as a slave, this function
 * sets the number of bytes the slave transaction functions should transmit
 * before issuing a call to their callback function. Larger values may
 * consume more CPU cycles, but reduce the risk of not having data ready when
 * the master requests it.  Smaller values may reduce the number of CPU
 * cycles, but may cause the master to read from an empty FIFO.  (The master
 * will read 0xFF in this case.)
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 * @param   numBytes    The threshold level to set.  This value must be
 *                      between 0 and 8 inclusive.
 *
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2C_SetTXThreshold (mxc_i2c_regs_t* i2c, unsigned int numBytes);

/**
 * @brief   Get the current transmit threshold level.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 *
 * @return  The transmit threshold value (in bytes).
 */
unsigned int MXC_I2C_GetTXThreshold (mxc_i2c_regs_t* i2c);

/**
 * @brief   Abort any asynchronous requests in progress.
 *
 * Abort any asynchronous requests in progress. Any callbacks associated with
 * the active transaction will be executed to indicate when the transaction
 * has been terminated.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_AbortAsync (mxc_i2c_regs_t* i2c);

/**
 * @brief   The processing function for asynchronous transactions.
 *
 * When using the asynchronous functions, the application must call this
 * function periodically. This can be done from within the I2C interrupt
 * handler or periodically by the application if I2C interrupts are disabled.
 *
 * @param   i2c         Pointer to I2C registers (selects the I2C block used.)
 */
void MXC_I2C_AsyncHandler (mxc_i2c_regs_t* i2c);

/**
 * @brief   The processing function for DMA transactions.
 *
 * When using the DMA functions, the application must call this
 * function periodically. This can be done from within the DMA Interrupt Handler.
 *
 * @param   ch          DMA channel
 * @param   error       Error status
 */
void MXC_I2C_DMACallback (int ch, int error);


/**@} end of group i2c */


#ifdef __cplusplus
}
#endif

#endif /* _MXC_I2C_H_ */
