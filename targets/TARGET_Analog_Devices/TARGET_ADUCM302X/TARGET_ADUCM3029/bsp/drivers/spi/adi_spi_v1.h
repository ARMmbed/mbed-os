/*!
 ******************************************************************************
 * @file:    adi_spi_v1.h
 * @brief:   SPI Device Definitions for ADuCxxx
 * @version: $Revision: 35166 $
 * @date:    $Date: 2016-07-27 10:35:03 +0100 (Wed, 27 Jul 2016) $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2016 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/** @addtogroup SPI_Driver SPI Device Driver
 *  @{
 */
#ifndef __ADI_SPI_V1_H__
#define __ADI_SPI_V1_H__

#include <adi_spi_config.h>
#include <services/int/adi_int.h>
#include <adi_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if (ADI_SPI_CFG_ENABLE_DMA_SUPPORT == 1)
/*! Amount of memory(In bytes) required by the SPI device driver for managing the operation
 *in DMA mode. This memory is completely owned by the driver till the end of
 *\n  the operation. Please note that it varies based on the user configuration. For example 
 *\n  SPI driver requires more memory if it is expected work in DMA mode.
 */
#define ADI_SPI_MEMORY_SIZE (320u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#else
/*! Amount of memory(In bytes) required by the SPI device driver  for managing the operation
 *\n  in interrupt mode. This memory is completely owned by the driver till the end of
 *\n  the operation.Please note that it varies based on the user configuration. For example 
 *\n  SPI driver requires more memory if it is expected work in DMA mode.
 */
#define ADI_SPI_MEMORY_SIZE (72u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#endif

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
    /*! DMA is not defined for this device. */
    ADI_SPI_ERR_NO_DMA_FOR_DEVICE,
    /*! SPI device is uninitialized.        */
    ADI_SPI_ERR_NOT_INITIALIZED,
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
    ADI_SPI_BUFFER_NOT_SUBMITTED
} ADI_SPI_RESULT;

/*!
 *****************************************************************************
 * \enum ADI_SPI_EVENT
 *
 * Enumeration of events notified in the application provided callback.
 *****************************************************************************/
typedef enum
{
    /*!< The given buffer is processed. Application can use this event to submit
         the next buffer to be transmitted. */
    ADI_SPI_EVENT_BUFFER_PROCESSED  = 0,
    /*!  Enable interrupt on every CS edge in slave CON mode */
    ADI_SPI_EVENT_CS_EDGE           =  BITM_SPI_IEN_CS,
   /*!  Tx-underflow interrupt enable */
    ADI_SPI_EVENT_TX_UNDERFLOW      =  BITM_SPI_IEN_TXUNDR,
    /*!  Rx-overflow interrupt enable */
    ADI_SPI_EVENT_RX_OVERFLOW      =  BITM_SPI_IEN_RXOVR,
    /*! Ready signal edge interrupt enable. This enables the RDY interrupt whenever
     *\n an active edge occurs on RDY/MISO signals */
    ADI_SPI_EVENT_READY_EDGE       =  BITM_SPI_IEN_RDY,
    /*!  SPI transmit done interrupt enable.   */
    ADI_SPI_EVENT_TX_DONE          =  BITM_SPI_IEN_TXDONE,
    /*!  SPI transfer completion interrupt enable */
    ADI_SPI_EVENT_TRANSFER_COMPLETED  =  BITM_SPI_IEN_XFRDONE,
    /*!  Tx-FIFO Empty interrupt enable */
    ADI_SPI_EVENT_TX_EMPTY         =  BITM_SPI_IEN_TXEMPTY

} ADI_SPI_EVENT;

/*!
 *****************************************************************************
 * \enum ADI_SPI_IRQ_MODE
 *
 * SPI driver interrupt mode.  Allows designation through the
 * #adi_spi_SetInterruptMode() API of when the SPI controller generates interrupts,
 * based on the number of transmit and receive bytes transacted.  This is helpful
 * in managing the rate of interrupts vs. the the data FIFO "fullness", the idea being
 * to minimize the number of interrupts while not allowing either FIFO to overflow or underflow.
 * Only the 4 combinations listed are supported by the hardware.
 *
 *****************************************************************************/
typedef enum
{
    /*! Interrupt every 1-byte tx, 1-byte rx. */
    ADI_SPI_IRQMODE_T1R1 = 0 << BITP_SPI_IEN_IRQMODE,
    /*! Interrupt every 2-byte tx, 2-byte rx. */
    ADI_SPI_IRQMODE_T2R2 = 1 << BITP_SPI_IEN_IRQMODE,
    /*! Interrupt every 3-byte tx, 3-byte rx. */
    ADI_SPI_IRQMODE_T3R3 = 2 << BITP_SPI_IEN_IRQMODE,
    /*! Interrupt every 4-byte tx, 4-byte rx. */
    ADI_SPI_IRQMODE_T4R4 = 3 << BITP_SPI_IEN_IRQMODE,
#ifdef SPI_IRQ_MODE_1_0_1 
    /*! Interrupt every 1-byte tx, 1-byte rx. */
    ADI_SPI_IRQMODE_T1R4 = 4 << BITP_SPI_IEN_IRQMODE,
   /*!  Interrupt every 2-byte tx, 2-byte rx. */
    ADI_SPI_IRQMODE_T2R5 = 5 << BITP_SPI_IEN_IRQMODE,
    /*! Interrupt every 3-byte tx, 3-byte rx. */
    ADI_SPI_IRQMODE_T3R6 = 6 << BITP_SPI_IEN_IRQMODE,
   /*! Interrupt every 4-byte tx, 4-byte rx. */
    ADI_SPI_IRQMODE_T4R7 = 7 << BITP_SPI_IEN_IRQMODE
#else
    /*! Interrupt every 5-byte tx, 5-byte rx. */
    ADI_SPI_IRQMODE_T5R5 = 4 << BITP_SPI_IEN_IRQMODE,
   /*!  Interrupt every 6-byte tx, 6-byte rx. */
    ADI_SPI_IRQMODE_T6R6 = 5 << BITP_SPI_IEN_IRQMODE,
    /*! Interrupt every 7-byte tx, 7-byte rx. */
    ADI_SPI_IRQMODE_T7R7 = 6 << BITP_SPI_IEN_IRQMODE,
   /*! Interrupt every 8-byte tx, 8-byte rx. */
    ADI_SPI_IRQMODE_T8R8 = 7 << BITP_SPI_IEN_IRQMODE
#endif    
} ADI_SPI_IRQ_MODE;

/*!
 * RTC Interrupt Enable Bits.
 */
typedef enum
{
    /*!  No interrupt will be enabled. */
    ADI_SPI_INT_ENA_NONE                 =  0,
    /*!  Enable interrupt on every CS edge in slave CON mode */
    ADI_SPI_CS_EDGE_INT_ENA               =  BITM_SPI_IEN_CS,
   /*!  Tx-underflow interrupt enable */
    ADI_SPI_TX_UNDERFLOW_INT_ENA         =  BITM_SPI_IEN_TXUNDR,
    /*!  Rx-overflow interrupt enable */
    ADI_SPI_RX_OVERFLOW_INT_ENA          =  BITM_SPI_IEN_RXOVR,
    /*! Ready signal edge interrupt enable. This enables the RDY interrupt whenever
        an active edge occurs on RDY/MISO signals */
    ADI_SPI_READY_EDGE_INT_ENA           =  BITM_SPI_IEN_RDY,
    /*!  SPI transmit done interrupt enable.   */
    ADI_SPI_TX_DONE_INT_ENA              =  BITM_SPI_IEN_TXDONE,
    /*!  SPI transfer completion interrupt enable */
    ADI_SPI_TRANSFER_COMPLETED_INT_ENA   =  BITM_SPI_IEN_XFRDONE,
    /*!  Tx-FIFO Empty interrupt enable */
    ADI_SPI_TX_EMPTY_INT_ENA             =  BITM_SPI_IEN_TXEMPTY
} ADI_SPI_INT_TYPE;
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

/*!
 *  Enum for selecting the override control option
 */

typedef enum
{
    /*! CS is not forced.                                   */
    ADI_SPI_CS_NOT_FORCED  =          0,
    /*! CS is forced to drive 1'b1.                         */
    ADI_SPI_CS_FORCED_HIGH =          1,
    /*! CS is forced to drive 1'b0.                         */
    ADI_SPI_CS_FORCED_LOW  =          2,
    /*! CS is not forced, which is same as the first enum.  */
    ADI_SPI_CS_NOT_FORCED_RDNT  =     3
} ADI_SPI_CS_OVERRIDE;

/*!
  * Enum for selecting the flow control mode
 */

typedef enum
{
   /*! Flow control is disabled.               */
    ADI_SPI_FLOW_MODE_NONE,
   /*! Flow control is based on timer.         */
    ADI_SPI_FLOW_MODE_TIMER,
    /*! Flow control is based on RDY signal.   */
    ADI_SPI_FLOW_MODE_RDY,
    /*! Flow control is based on MISO pin.     */
    ADI_SPI_FLOW_MODE_MISO
} ADI_SPI_FLOW_MODE;

/*! SPI Device instance private data handle typedef. */
typedef struct __ADI_SPI_DEV_DATA_TYPE* ADI_SPI_HANDLE;


/*!
  * \struct ADI_SPI_TRANSCEIVER
 *****************************************************************************
 *\n SPI Device Command/Data Transceiver Structure.  Data structure used by
 *\n  the #adi_spi_ReadWrite(),#adi_spi_SlaveTransfer(),#adi_spi_MasterTransfer()
 *\n API to convey all parameters, consisting of
 *\n  prologue, transmit and receive data and size, and buffer increment flags.
 *****************************************************************************/
typedef struct
{
    /*! Pointer to transmit data.        */
    uint8_t*    pTransmitter;
    /*! Pointer to receive data.         */
    uint8_t*    pReceiver;
   /*! Data size  for TX(bytes).          */
    uint32_t    TransmitterBytes;
    /*! Data size  for RX(bytes).          */
    uint32_t    ReceiverBytes;
   /*!  Transmit pointer increment flag. */
    uint32_t       nTxIncrement;
   /*! Receive pointer increment flag.  */
    uint32_t      nRxIncrement;
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

ADI_SPI_RESULT adi_spi_ReadWrite(
               ADI_SPI_HANDLE const       hDevice,
               ADI_SPI_TRANSCEIVER* const pXfr
               );

ADI_SPI_RESULT adi_spi_RegisterCallback(
               ADI_SPI_HANDLE const hDevice,  
               ADI_CALLBACK   const pfCallback, 
               void *const          pCBParam 
               );

/* Slave Mode APIs */
ADI_SPI_RESULT adi_spi_SlaveTransfer(
               ADI_SPI_HANDLE const hDevice,
               ADI_SPI_TRANSCEIVER* const pXfr
               );

/* Hardware Configuration Interface */
ADI_SPI_RESULT adi_spi_SetClockPhase(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetClockPolarity(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetContinousMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetFifoRxFlush(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
                );

ADI_SPI_RESULT adi_spi_SetFifoTxFlush(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetInterruptMode(
               ADI_SPI_HANDLE const hDevice,
               const ADI_SPI_IRQ_MODE Mode
               );

ADI_SPI_RESULT adi_spi_SetLoopback(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetLsbFirst(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetMasterMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetMisoOutput(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetReceiveOverflow(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetTransferInitiateMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetTransmitUnderflow(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetWiredOrMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetInterrruptMask(
               ADI_SPI_HANDLE const hDevice,
               ADI_SPI_INT_TYPE eInterrupt
               );

ADI_SPI_RESULT adi_spi_SetThreepinMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetTransmitBytes(
               ADI_SPI_HANDLE const hDevice,
               uint8_t nBytes
               );

ADI_SPI_RESULT adi_spi_SetOverlapMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetReadCommand(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetBurstSize(
               ADI_SPI_HANDLE const hDevice,
               uint8_t nBytes
               );

ADI_SPI_RESULT adi_spi_SetReadySignalPolarity(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );

ADI_SPI_RESULT adi_spi_SetFlowMode(
               ADI_SPI_HANDLE const hDevice,
               ADI_SPI_FLOW_MODE eFlowMode
               );

ADI_SPI_RESULT adi_spi_WaitTime(
               ADI_SPI_HANDLE const hDevice,
               uint16_t nWaitTime
               );

/* Mode Configuration Interface */
ADI_SPI_RESULT adi_spi_SetBitrate(
               ADI_SPI_HANDLE const hDevice,
               const uint32_t Hertz
               );
ADI_SPI_RESULT adi_spi_SetChipSelect(
               ADI_SPI_HANDLE const hDevice,
               const ADI_SPI_CHIP_SELECT eChipSelect
               );
ADI_SPI_RESULT adi_spi_EnableDmaMode(
               ADI_SPI_HANDLE const hDevice,
               const bool_t bFlag
               );
ADI_SPI_RESULT adi_spi_GetBitrate(
               ADI_SPI_HANDLE const hDevice,
               uint32_t* const pnBitrate
               );
ADI_SPI_RESULT adi_spi_GetChipSelect(
               ADI_SPI_HANDLE const hDevice,
               ADI_SPI_CHIP_SELECT *peChipSelect
               );
/* Query function for the data transfer completion  */
ADI_SPI_RESULT adi_spi_MasterComplete(
               ADI_SPI_HANDLE const hDevice,
               bool_t* const bComplete
               );

ADI_SPI_RESULT adi_spi_SlaveComplete(
               ADI_SPI_HANDLE const hDevice,
               bool_t* const bComplete
               );

ADI_SPI_RESULT adi_spi_AssertChipSelect(
               ADI_SPI_HANDLE const hDevice,
               ADI_SPI_CS_OVERRIDE CSOverride
               );

/* Command/Data transceiver API */
ADI_SPI_RESULT adi_spi_MasterTransfer(
                ADI_SPI_HANDLE const hDevice,
                ADI_SPI_TRANSCEIVER* const pXfr
                );

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*@}*/
