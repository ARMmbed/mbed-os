/*! *****************************************************************************
 * @file    adi_spi.h
 * @brief   Main include file for SPI Device driver definitions
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
    which differ from those listed here.ADI_SEM_SIZE
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


#ifndef ADI_SPI_H__
#define ADI_SPI_H__

#include <adi_processor.h>
#include <rtos_map/adi_rtos_map.h>
#include <adi_spi_config.h>

/** @addtogroup SPI_Driver SPI Driver
 *  @{
 */


#include <adi_spi_config.h>
#include <adi_callback.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*! Amount of memory(In bytes) required by the SPI device driver for managing the operation
 *  of a SPI controller. The memory is passed to the driver when the driver is opened.
 * The memory is completely owned by the driver till the the driver is closed.
 *                                               
 */
   
#define ADI_SPI_MEMORY_SIZE (40u + ADI_SEM_SIZE)
        

/*!
 *****************************************************************************
 * \enum ADI_SPI_RESULT
 *
 * SPI Device Error Codes.  #ADI_SPI_SUCCESS is always zero
 * The return value of all SPI APIs returning #ADI_SPI_RESULT
 * should always be tested at the application level for success or failure.
 *
 *****************************************************************************/
typedef enum
{
    /*! Generic success.                    */
    ADI_SPI_SUCCESS,
    /*! Generic Failure.                      */
    ADI_SPI_FAILURE,
    /*! SPI device is already initialized.  */
    ADI_SPI_IN_USE,
    /*! Invalid device handle.              */
    ADI_SPI_INVALID_HANDLE,
    /*! Invalid device ID.                  */
    ADI_SPI_INVALID_DEVICE_NUM,
    /*! DMA configuration failure.          */
    ADI_SPI_DMA_ERROR     ,
    /*! NULL data pointer not allowed.      */
    ADI_SPI_INVALID_POINTER,
    /*! Parameter is out of range.          */
    ADI_SPI_INVALID_PARAM,
    /*! Unsupported mode of operation.      */
    ADI_SPI_UNSUPPORTED_MODE,
    /*! Semaphore in error           .      */
    ADI_SPI_SEMAPHORE_FAILED,
    /*! Invalid operation       */
    ADI_SPI_INVALID_OPERATION,
    /*! Buffer Not submitted */
    ADI_SPI_BUFFER_NOT_SUBMITTED,
    /*! Could not obtain the system clock */
    ADI_SPI_BAD_SYS_CLOCK,
    /*! Blocking PEND failed */
    ADI_SPI_PEND_FAILED,
    /*! DMA callback register failed */
    ADI_SPI_DMA_REG_FAILED,
    /*! Hardware error occurred */
    ADI_SPI_HW_ERROR_OCCURRED
} ADI_SPI_RESULT;

/*!
 *****************************************************************************
 * \enum ADI_SPI_HW_ERRORS
 *
 * Enumeration of events notified in the application provided callback.
 * More than one event can be recorded at a time so the enumerator symbols
 * have to be assigned values of 2^N
 *****************************************************************************/
typedef enum
{
    /*!< The given buffer is processed. Application can use this event to submit
         the next buffer to be transmitted. */
    ADI_SPI_HW_ERROR_NONE                       = 0u,
   /*!  Tx-underflow interrupt enable */
    ADI_SPI_HW_ERROR_TX_UNDERFLOW               = 1u,
    /*!  Rx-overflow interrupt enable */
    ADI_SPI_HW_ERROR_RX_OVERFLOW                = 2u,
    /*!  Rx DMA channel bus fault detected */
    ADI_SPI_HW_ERROR_RX_CHAN_DMA_BUS_FAULT      = 4u,
    /*!  Tx DMA channel bus fault detected */
    ADI_SPI_HW_ERROR_TX_CHAN_DMA_BUS_FAULT      = 8u,
    /*!  Rx DMA channel bus fault detected */
    ADI_SPI_HW_ERROR_RX_CHAN_DMA_INVALID_DESCR  = 16u,
    /*!  Tx DMA channel bus fault detected */
    ADI_SPI_HW_ERROR_TX_CHAN_DMA_INVALID_DESCR  = 32u,
    /*!  Rx DMA channel unkown error detected */
    ADI_SPI_HW_ERROR_RX_CHAN_DMA_UNKNOWN_ERROR  = 64u,
    /*!  Tx DMA channel unkown error detected */
    ADI_SPI_HW_ERROR_TX_CHAN_DMA_UNKNOWN_ERROR  = 128u          

} ADI_SPI_HW_ERRORS;

/*!
 *****************************************************************************
 * \enum ADI_SPI_CHIP_SELECT
 *
 * SPI Device Chip Select Enumeration.  Allows designation of an external
 * SPI slave device chip select pin to be driven by the SPI controller.
 * Multiple external slave SPI devices may be present on a shared SPI bus,
 * and the chip select pin allows each of them to be assigned dedicated selects.
 * Use the #adi_spi_SetChipSelect() API to configure the active chip select.
 * Note that SPI0 is an internal channel dedicated to the UHF controller and
 * hence, has a dedicated SPI0 chip select pin that is not available externally.
 *
 *****************************************************************************/
typedef enum
{
   /*! No  Slave Chip Select for SPI.               */
    ADI_SPI_CS_NONE = 0,
    /*! CS0 Slave Chip Select for SPI.               */
    ADI_SPI_CS0 = 1,
   /*! CS1 Slave Chip Select for SPI.               */
    ADI_SPI_CS1 = 2,
    /*! CS2 Slave Chip Select for SPI.               */
    ADI_SPI_CS2 = 4,
    /*! CS3 Slave Chip Select for SPI.               */
    ADI_SPI_CS3 = 8
} ADI_SPI_CHIP_SELECT;


/*! SPI Device instance private data handle typedef. */
typedef       struct __ADI_SPI_DEV_DATA_TYPE*  ADI_SPI_HANDLE;
/*! SPI Device instance private data handle typedef. 'const' version */
typedef const struct __ADI_SPI_DEV_DATA_TYPE*  ADI_SPI_CONST_HANDLE;


/*!
  * \struct ADI_SPI_TRANSCEIVER
 *****************************************************************************
 * SPI Device Command/Data Transceiver Structure.  Data structure used by
 * the #adi_spi_MasterReadWrite(),#adi_spi_MasterSubmitBuffer()
 * API to convey all parameters, consisting of
 * prologue, transmit and receive data and size, and buffer increment flags.
 * DMA and Half-Duplex operation are also specified in this structure as T/F.
 *
 * Each call to #adi_spi_MasterReadWrite or #adi_spi_MasterSubmitBuffer() must populate the following fields of the
 * ADI_SPI_TRANSCEIVER block:
 *
 * @par TransmitterBytes
 * The number of  bytes to be transmitted. If the value is zero, data will not be transmitted from the
 * buffer pointed by pTransmitter.
 *
 * @par ReceiverBytes
 * The number of  bytes to be received. If the value is zero, data will not be stored in the
 * buffer pointed by pReceiver.
 *
 * @par pTransmitter
 * Pointer to the application-defined transmit data buffer.  This is the data sent out
 * over the SPI transmit wire (MOSI for Master-mode, MISO for Slave-mode) during the SPI transaction.
 * For  SPI DMA mode (which is 16-bit based), the transmit buffer must be 16-bit aligned.
 *
 * @par pReceiver
 * Pointer to the application-defined receive data buffer.  This is where the receive data
 * will be stored from the SPI receive wire (MISO for Master-mode, MOSI for Slave-mode)
 * during the SPI transaction.
 * For SPI DMA mode (which is 16-bit based), the receive buffer must be 16-bit aligned.
 *
 * @par bTxIncrement
 * Increment to be done for the transmit buffer after every transaction . The transmit data buffer
 * pointer is advanced as each byte is sent.  If it is set to zero, the transmit data pointer is stationary.
 * A stationary buffer pointer is useful for sending the same data to an external device or if
 * the source data is from a fixed memory address.
 *
 * @par bRxIncrement
 * Increment to be done for the receive buffer.   The transmit data buffer
 * pointer is advanced as each byte is sent. If it is value is set to zero, the receive
 * data pointer is stationary.  A stationary buffer pointer is useful for monitoring commands
 * from an external device or if the receive data is going to a fixed memory address.
 *
 * @par bDMA
 * Indicate whether the transaction is to use DMA (true) or not (false). If using DMA SPI
 * transactions are limited to 2048 bytes. If more than 2048 bytes are needed then the application
 * must use multiple transactions (DMA ping pong mode is not supported in the driver).
 * For  SPI DMA mode (which is 16-bit based), TransmitterBytes/ReceiverBytes is rounded up to an
 * even number by the SPI driver before submitting to DMA.
 * Please align the buffer to 16 bit word boundary since the data transfer is 16bit.
 *
 *
 * @par bRD_CTL
 * Indicate whether the transaction should enable RD_CTL (true) or not (false).
 * RD_CTL effectively provides half-duplex operation as outlined in the HRM.

 *****************************************************************************/
typedef struct
{
    /*! Pointer to transmit data.        */
    uint8_t*    pTransmitter;
    /*! Pointer to receive data.         */
    uint8_t*    pReceiver;
   /*! Data size  for TX(bytes).          */
    uint16_t    TransmitterBytes;
    /*! Data size  for RX(bytes).          */
    uint16_t    ReceiverBytes;
   /*!  Transmit pointer increment flag. */
    uint8_t     nTxIncrement;
   /*! Receive pointer increment flag.  */
    uint8_t     nRxIncrement;
    /*! DMA mode operation */
    bool        bDMA;
    /*! RD_CTL, half-duplex, operation */
    bool        bRD_CTL;

} ADI_SPI_TRANSCEIVER;



/******************************************************************************
 * SPI Device External API function prototypes
 *****************************************************************************/

/* Device Initialization and Uninitialization Interfaces */
ADI_SPI_RESULT adi_spi_Open(
               uint32_t nDeviceNum,
               void *pDevMemory,
               uint32_t nMemorySize,
               ADI_SPI_HANDLE* const phDevice
               );

ADI_SPI_RESULT adi_spi_Close(
               ADI_SPI_HANDLE const hDevice
               );

/******************************************************************
 * Eliminatable functions that may be optimized out by the linker *
 *****************************************************************/

ADI_SPI_RESULT adi_spi_MasterReadWrite(
               ADI_SPI_HANDLE const       hDevice,
               const ADI_SPI_TRANSCEIVER* const pXfr
               );


ADI_SPI_RESULT adi_spi_SetMasterMode(
               ADI_SPI_CONST_HANDLE const hDevice,
               const bool bFlag
               );

/* Slave Mode APIs */
ADI_SPI_RESULT adi_spi_SlaveReadWrite(
               ADI_SPI_HANDLE const hDevice,
               const ADI_SPI_TRANSCEIVER* const pXfr
               );

/* Command/Data transceiver API */
ADI_SPI_RESULT adi_spi_MasterSubmitBuffer(
              ADI_SPI_HANDLE const hDevice, 
              const ADI_SPI_TRANSCEIVER* const pXfr
                );

ADI_SPI_RESULT adi_spi_SlaveSubmitBuffer(
               ADI_SPI_HANDLE const hDevice, 
               const ADI_SPI_TRANSCEIVER* 
               const pXfr
                 );

ADI_SPI_RESULT adi_spi_RegisterCallback (
               ADI_SPI_HANDLE const hDevice,  
               ADI_CALLBACK const pfCallback, 
               void *const pCBParam 
                 );


/* Turn a non-blocking call into a blocking call. Wait for the transaction to complete  */
ADI_SPI_RESULT adi_spi_GetBuffer(
               ADI_SPI_HANDLE const hDevice,
               uint32_t * const pHWErrors
               );

/* Hardware Configuration Interface */
ADI_SPI_RESULT adi_spi_SetClockPhase(
               ADI_SPI_HANDLE const hDevice,
               const bool bFlag
               );

ADI_SPI_RESULT adi_spi_SetClockPolarity(
               ADI_SPI_HANDLE const hDevice,
               const bool bFlag
               );
			   
/* Query function for the data transfer completion  */
ADI_SPI_RESULT adi_spi_isBufferAvailable(
               ADI_SPI_CONST_HANDLE const hDevice,
               bool* const bComplete
               );



ADI_SPI_RESULT adi_spi_SetContinuousMode(
               ADI_SPI_CONST_HANDLE const hDevice,
               const bool bFlag
               );


ADI_SPI_RESULT adi_spi_SetLoopback(
               ADI_SPI_CONST_HANDLE const hDevice,
               const bool bFlag
               );

ADI_SPI_RESULT adi_spi_SetIrqmode (
               ADI_SPI_CONST_HANDLE const hDevice, 
               const uint8_t nMode);

ADI_SPI_RESULT adi_spi_SetReceiveOverflow(
               ADI_SPI_CONST_HANDLE const hDevice,
               const bool bFlag
               );

ADI_SPI_RESULT adi_spi_SetTransmitUnderflow(
               ADI_SPI_CONST_HANDLE const hDevice,
               const bool bFlag
               );

/* Mode Configuration Interface */
ADI_SPI_RESULT adi_spi_SetBitrate(
               ADI_SPI_CONST_HANDLE const hDevice,
               const uint32_t Hertz
               );
ADI_SPI_RESULT adi_spi_SetChipSelect(
               ADI_SPI_HANDLE const hDevice,
               const ADI_SPI_CHIP_SELECT eChipSelect
               );

ADI_SPI_RESULT adi_spi_GetBitrate(
               ADI_SPI_CONST_HANDLE const hDevice,
               uint32_t* const pnBitrate
               );


#ifdef __cplusplus
}
#endif


/**@}*/


#endif /* ADI_SPI_H__ */

