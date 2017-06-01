/*!
 *****************************************************************************
 @file:    adi_i2c.h
 @brief:   I2C device driver definitions
 @details  This is the primary header file for the I2C driver, which contains the
           API declarations, data and constant definitions used in the APIs.
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.
THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef ADI_I2C_H
#define ADI_I2C_H

 /*! \cond PRIVATE */
#include <adi_processor.h>
#include <rtos_map/adi_rtos_map.h>  /* for ADI_SEM_SIZE */
/*! \endcond */


/** @addtogroup I2C_Driver I2C Driver
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__ICCARM__)
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): Types which specify sign and size should be used
*   We use bool which is accepted by MISRA but the toolchain does not accept it
*
*/
#pragma diag_suppress=Pm011

#endif

/*!
 *****************************************************************************
 * \enum ADI_I2C_RESULT
 *
 * I2C Device Error Codes.  #ADI_I2C_SUCCESS is always zero
 * The return value of all I2C APIs returning #ADI_I2C_RESULT
 * should always be tested at the application level for success or failure.
 * Specific I2C bus error conditions are returned as elements of
 * #ADI_I2C_RESULT.
 *
 *****************************************************************************/
typedef enum
{
    ADI_I2C_SUCCESS = 0,                         /*!< The API call succeeded.                                  */
    ADI_I2C_BAD_BITRATE,                         /*!< The bit rate is invalid.                                 */
    ADI_I2C_BAD_DEVICE_HANDLE,                   /*!< The device handle is invalid.                            */
    ADI_I2C_BAD_DEVICE_NUMBER,                   /*!< The device number is invalid.                            */
    ADI_I2C_BAD_SYS_CLOCK,                       /*!< Unable to obtain system clock rate.                      */
    ADI_I2C_DEVICE_IN_USE,                       /*!< The device is in use.                                    */
    ADI_I2C_DEVICE_NOT_OPEN,                     /*!< The device is not open.                                  */
    ADI_I2C_FAILURE,                             /*!< Generic API failure code.                                */
    ADI_I2C_HW_ERROR_DETECTED,                   /*!< An I2C hardware error occurred.  See #ADI_I2C_HW_ERRORS. */
    ADI_I2C_INSUFFICIENT_MEMORY,                 /*!< The application supplied memory size is insufficient.    */
    ADI_I2C_INVALID_PARAMETER,                   /*!< An invalid parameter is passed to the function.          */
    ADI_I2C_INVALID_SLAVE_ADDRESS,               /*!< The application supplied slave address is too wide.      */
    ADI_I2C_INVALID_SUBMIT_API,                  /*!< Unmatched read/write vs. submit/get API call.            */
    ADI_I2C_SEMAPHORE_FAILED                     /*!< Semaphore operation failed.                              */

} ADI_I2C_RESULT;


/*!
 *****************************************************************************
 * \enum ADI_I2C_HW_ERRORS
 *
 * I2C Device Hardware Error Codes.  Contains one or more hardware (I2C protocol)
 * errors.  Use this enum to decode hardware errors when the main #ADI_I2C_RESULT
 * return result value is #ADI_I2C_HW_ERROR_DETECTED.
 *
 *****************************************************************************/
typedef enum
{
    ADI_I2C_HW_ERROR_NONE             = 0,         /*!< No hardware error.                                       */
    ADI_I2C_HW_ERROR_NACK_ADDR        = 0x0001,    /*!< A no-acknowledgement occurred for the address.           */
    ADI_I2C_HW_ERROR_NACK_DATA        = 0x0002,    /*!< A no-acknowledgement occurred for the data.              */
    ADI_I2C_HW_ERROR_ARBITRATION_LOST = 0x0004,    /*!< I2C bus arbitration was Lost.                            */
    ADI_I2C_HW_ERROR_UNEXPECTED_ERROR = 0x0008,    /*!< An unexpected error occurred.                            */

} ADI_I2C_HW_ERRORS;


/*! A device handle used in all API functions to identify the I2C device. */
typedef struct __ADI_I2C_DEV_DATA_TYPE* ADI_I2C_HANDLE;

/*! Use macro "ADI_I2C_MEMORY_SIZE" to know how much memory to
   provide the i2c driver during the "adi_i2c_Open()" driver
   initialization call.  This memory is used to store internal
   driver state data.  Use map file to verify.
*/
#define ADI_I2C_MEMORY_SIZE (44u + ADI_SEM_SIZE)


/*!
  * \struct ADI_I2C_TRANSACTION
  *****************************************************************************
  * I2C Device Command/Data Transaction Structure.  This is the called-provided
  * data structure used by the blocking #adi_i2c_ReadWrite() and non-blocking
  * #adi_i2c_SubmitBuffer() calls to describe the caller's transaction parameters,
  * consisting of prologue data and size (the addressing phase), transmit/receive
  * data pointer and size (the data phase), and various transaction control parameters.
  *
  * Each transaction may optionally be prefaced with a prologue block, which may
  * describe a read/write memory/register address, a slave-specific command, or
  * some other slave-specific protocol that may precede the actual read/write
  * data.  Set the prologue size to zero if no prologue is desired.
  *
  * Each call to #adi_i2c_ReadWrite or #adi_i2c_SubmitBuffer() must populate the
  * following fields of the ADI_I2C_TRANSACTION block:
  *
  * @par pPrologue
  * Byte pointer to an application-supplied prologue byte array. If the value is
  * zero, prologue data is ignored.
  *
  * @par nPrologueSize
  * The number of prologue bytes to be transmitted ahead of the data phase. If the
  * value is zero, prologue data is ignored.
  *
  * @par pData
  * Byte pointer to the application-supplied data byte array.  This buffer is
  * either the source or destination address of the data being transmitted or
  * received, respectively.
  *
  * @par nDataSize
  * The number of data bytes to be transmitted or received during the data phase.
  * If the value is zero, the data phase is ignored.
  *
  * @par bReadNotWrite
  * Direction control for data phase.  If "true", data phase is a read (from
  * the slave), if "false", data phase is a write (to the slave).  Pertains only
  * to the data phase.  Any prologue data (addressing/command phase) is always
  * transmitted (written to the slave) prior to the data phase.
  *
  * @par bRepeatStart
  * Controls suppression of a Stop Condition between the addressing phase and the
  * data phase of an I2C transaction.  After the prologue (if present), a
  * unidirectional data stream (I2C is a half-duplex protocol) is either
  * transmitted or received (depending on the transfer direction).  Frequently, a
  * Repeat-Start Condition (in reality, just the absence of a Stop Condition
  * following the prologue/addressing phase) is required between the addressing
  * phase (prologue) and the data phase of a transaction to meet slave device
  * protocol requirements.  The Repeat-Start requirement can be driven by the
  * slave device communications protocol, or simply to just prevent any other
  * I2C master from rearbitrating the bus between the prologue (addressing) and
  * data phases of a so-called "COMBINED FORMAT" (write-followed-by-read).
  * When bRepeatStart is set "true", the usual Stop Condition between the addressing
  * phase and the data phase is suppressed and the I2C bus controller issues a
  * second Start Condition (Repeat-Start) for the data phase.  Without
  * Repeat-Start (bRepeatStart "false"), the addressing phase ends with a normal
  * Stop Condition ahead of the data phase.  Repeat-Start conditions are used
  * when "turning the bus around" as in writing a read address (for example),
  * immediately followed by a data stream from that read address... without
  * releasing bus arbitration.
  *
  *****************************************************************************/
typedef struct {
    uint8_t    *pPrologue;      /*!< Prologue pointer.                      */
    uint16_t    nPrologueSize;  /*!< Prologue byte count.                   */
    uint8_t    *pData;          /*!< Data pointer.                          */
    uint16_t    nDataSize;      /*!< Data byte count.                       */
    bool        bReadNotWrite;  /*!< Read/write flag.                       */
    bool        bRepeatStart;   /*!< Repeat start flag.                     */
} ADI_I2C_TRANSACTION;


/*! Maximum supported bitrate is "FAST" mode (400 kHz). */
#define ADI_I2C_MAX_RATE (400000u)

/***************************************************************
 * Eliminable user API that may be optimized out by the linker *
 ***************************************************************/
ADI_I2C_RESULT  adi_i2c_Open               (uint32_t const DeviceNum, void* const pMemory, uint32_t const MemorySize, ADI_I2C_HANDLE* const phDevice);
ADI_I2C_RESULT  adi_i2c_Close              (ADI_I2C_HANDLE const hDevice);

/* blocking calls... */
ADI_I2C_RESULT  adi_i2c_ReadWrite          (ADI_I2C_HANDLE const hDevice, ADI_I2C_TRANSACTION* const pTransaction, uint32_t* const pHwErrors);

/* non-blocking calls... */
ADI_I2C_RESULT  adi_i2c_SubmitBuffer       (ADI_I2C_HANDLE const hDevice, ADI_I2C_TRANSACTION* const pTransaction);
ADI_I2C_RESULT  adi_i2c_IsBufferAvailable  (ADI_I2C_HANDLE const hDevice, bool* const pbCompletionState);
ADI_I2C_RESULT  adi_i2c_GetBuffer          (ADI_I2C_HANDLE const hDevice, uint32_t* const pHwErrors);

/* other (blocking) calls... */
ADI_I2C_RESULT  adi_i2c_Reset              (ADI_I2C_HANDLE const hDevice);
ADI_I2C_RESULT  adi_i2c_SetBitRate         (ADI_I2C_HANDLE const hDevice, uint32_t const requestedBitRate32);
ADI_I2C_RESULT  adi_i2c_SetSlaveAddress    (ADI_I2C_HANDLE const hDevice, uint16_t const SlaveAddress);
ADI_I2C_RESULT  adi_i2c_IssueGeneralCall   (ADI_I2C_HANDLE const hDevice, uint8_t* const pData, uint8_t const nDataSize, uint32_t* const pHwErrors);


#if defined (__ICCARM__)
#pragma diag_default=Pm011
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* ADI_I2C_H */
