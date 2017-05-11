/*! *****************************************************************************
 * @file    adi_uart.h
 * @brief   UART device driver global include file.
 * @details This a global file which includes a specific file based on the processor family.
 *          This included file will be containing UART device driver functions.
 -----------------------------------------------------------------------------
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
#ifndef ADI_UART_H
#define ADI_UART_H

/** @addtogroup UART_Driver UART Driver
*  @{
*/

/*! \cond PRIVATE */

/*=============  I N C L U D E S   =============*/

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <adi_processor.h>
#include <adi_uart_config.h>
#include <rtos_map/adi_rtos_map.h>
#include <drivers/dma/adi_dma.h>
#include <drivers/pwr/adi_pwr.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! \endcond */

/*! Amount of memory(bytes) required by the UART device driver for operating unidirectionally(Either RX or TX).
 *  This memory is completely owned by the driver until the end of the operation.
 */
#define ADI_UART_UNIDIR_MEMORY_SIZE  (48u + (60u + ADI_SEM_SIZE))

/*! Amount of memory(bytes) required by the UART device driver for operating bidirectionally(Both RX and TX).
 *  This memory is completely owned by the driver until the end of the operation.
 */
#define ADI_UART_BIDIR_MEMORY_SIZE  (48u + (60u + ADI_SEM_SIZE)*2u)

/*!
 * Handle for managing the UART device typedef.
 */
typedef struct _ADI_UART_DEVICE* ADI_UART_HANDLE;

/*!
 * Handle for managing the UART device typedef 'const' version.
 */
typedef const struct _ADI_UART_DEVICE* ADI_UART_CONST_HANDLE;

/*!
 *****************************************************************************
 *  \enum ADI_UART_DIRECTION
 *   Enumeration for the UART direction.
 *****************************************************************************/
typedef enum
{
    ADI_UART_DIR_TRANSMIT,                              /*!< UART is only transmitting.   */

    ADI_UART_DIR_RECEIVE,                               /*!< UART is only receiving.      */

    ADI_UART_DIR_BIDIRECTION                            /*!< UART in bidirectional.       */

} ADI_UART_DIRECTION;


/*!
 *****************************************************************************
 *  \enum ADI_UART_EVENT
 *   Enumeration of events notified in the application provided callback.
 *****************************************************************************/
 typedef enum
{
    ADI_UART_EVENT_RX_BUFFER_PROCESSED,                 /*!< Rx buffer is processed.                                   */

    ADI_UART_EVENT_TX_BUFFER_PROCESSED,                 /*!< Tx buffer is processed.                                   */

    ADI_UART_EVENT_NO_RX_BUFFER_EVENT,                  /*!< No Rx buffer but data is in FIFO.                         */

    ADI_UART_EVENT_AUTOBAUD_COMPLETE,                   /*!< Autobaud is complete.                                     */

    ADI_UART_EVENT_HW_ERROR_DETECTED,                   /*!< Hardware error detected.                                  */
    
    ADI_UART_EVENT_AUTOBAUD_ERROR_DETECTED              /*!< Autobaud error detected.                                  */
    
}ADI_UART_EVENT;


/*!
 *****************************************************************************
 *  \enum ADI_UART_RESULT
 *   Enumeration for result code returned from the UART device driver functions.
 *   The return value of all UART APIs returning #ADI_UART_RESULT
 *   should always be tested at the application level for success or failure.
 *****************************************************************************/
 typedef enum
{

    ADI_UART_SUCCESS,                           /*!< Generic success.                                               */

    ADI_UART_FAILED,                            /*!< Generic failure.                                               */

    ADI_UART_SEMAPHORE_FAILED,                  /*!< Semaphore error.                                               */

    ADI_UART_INVALID_HANDLE,                    /*!< Invalid device handle.                                         */

    ADI_UART_DEVICE_IN_USE,                     /*!< UART device in use.                                            */

    ADI_UART_INVALID_DEVICE_NUM,                /*!< Invalid device number.                                         */

    ADI_UART_INVALID_POINTER,                   /*!< NULL data pointer is not allowed.                              */

    ADI_UART_INSUFFICIENT_MEMORY,               /*!< Insufficent memory.                                            */

    ADI_UART_INVALID_DIR,                       /*!< Invalid UART direction.                                        */

    ADI_UART_OPERATION_NOT_ALLOWED,             /*!< Invalid operation.                                             */

    ADI_UART_INVALID_PARAMETER,                 /*!< Invalid parameter.                                             */

    ADI_UART_BUFFER_NOT_SUBMITTED,              /*!< Buffer not submitted.                                          */

    ADI_UART_INVALID_DATA_TRANSFER_MODE,        /*!< Invalid transfer mode.
                                                     Adi_uart_Read()/adi_uart_Write() is used in nonblocking mode
                                                     or adi_uart_SubmitRxBuffer()/adi_uart_SubmitTxBuffer()
                                                     is used in blocking mode.                                      */

    ADI_UART_HW_ERROR_DETECTED,                 /*!< Hardware error detected.                                       */

    ADI_UART_AUTOBAUD_ERROR_DETECTED,           /*!< Autobaud error detected.                                       */
      
    ADI_UART_ERR_DMA_REGISTER,                  /*!< Error while registering the DMA callback.                      */
      
    ADI_UART_INVALID_DATA_SIZE                  /*!< Invalid transfer size. Must be less than 1025 bytes            */

} ADI_UART_RESULT;

/*!
 *****************************************************************************
 *  \enum ADI_UART_HW_ERRORS
 *   Enumeration for UART hardware errors. If hardware error(s) occur in
 *   either callback or interrupt mode, they are mapped to #ADI_UART_HW_ERRORS.
 *   Interpretation of the break condition is application specific.
 *****************************************************************************/
typedef enum
{
    ADI_UART_NO_HW_ERROR                     = 0x00,   /*!< No hardware error.                                             */

    ADI_UART_HW_ERR_FRAMING                  = 0x10,   /*!< Rx framing error.                                              */

    ADI_UART_HW_ERR_PARITY                   = 0x20,   /*!< Rx parity error.                                               */

    ADI_UART_HW_ERR_OVERRUN                  = 0x40,   /*!< Receive overrun.                                               */
 
    ADI_UART_BREAK_INTERRUPT                 = 0x80,   /*!< Break condition.                                               */
      
    ADI_UART_HW_ERR_RX_CHAN_DMA_BUS_FAULT     = 0x100,  /*!< Rx DMA channel bus fault detected.                             */
    
    ADI_UART_HW_ERR_TX_CHAN_DMA_BUS_FAULT     = 0x200,  /*!< Tx DMA channel bus fault detected.                             */
   
    ADI_UART_HW_ERR_RX_CHAN_DMA_INVALID_DESCR = 0x400,  /*!< Rx DMA channel invalid descriptor detected.                    */
    
    ADI_UART_HW_ERR_TX_CHAN_DMA_INVALID_DESCR = 0x800,  /*!< Tx DMA channel invalid descriptor detected.                    */
    
    ADI_UART_HW_ERR_RX_CHAN_DMA_UNKNOWN_ERROR = 0x1000, /*!< Rx DMA channel unknown error detected.                         */
  
    ADI_UART_HW_ERR_TX_CHAN_DMA_UNKNOWN_ERROR = 0x2000, /*!< Tx DMA channel unknown error detected.                         */

}ADI_UART_HW_ERRORS;

/*!
 *****************************************************************************
 *  \enum ADI_UART_AUTOBAUD_ERRORS
 *   Enumeration for UART autobaud errors. If autobaud related error(s) occur
 *   they are mapped to #ADI_UART_AUTOBAUD_ERRORS.
 *****************************************************************************/
typedef enum
{
    ADI_UART_AUTOBAUD_NO_ERROR              = 0x000,  /*!< No autobaud error.                                           */

    ADI_UART_AUTOBAUD_TIMEOUT_NO_START_EDGE = 0x100,  /*!< Timeout due to no valid start edge found during autobaud.    */

    ADI_UART_AUTOBAUD_TIMEOUT_LONGBREAK     = 0x200,  /*!< Timeout due to break condition detected during autobaud.     */

    ADI_UART_AUTOBAUD_TIMEOUT_NO_END_EDGE   = 0x400   /*!< Timeout due to no valid end edge found during autobaud.      */

}ADI_UART_AUTOBAUD_ERRORS;

/*!
 *****************************************************************************
 *  \enum ADI_UART_TRIG_LEVEL
 *   Enumeration for the FIFO trigger level.
 *****************************************************************************/
typedef enum
{

    ADI_UART_RX_FIFO_TRIG_LEVEL_1BYTE   =  0 << BITP_UART_FCR_RFTRIG,  /*!< 1-byte to trigger RX interrupt.  */

    ADI_UART_RX_FIFO_TRIG_LEVEL_4BYTE   =  1 << BITP_UART_FCR_RFTRIG,  /*!< 4-byte to trigger RX interrupt.  */
    
    ADI_UART_RX_FIFO_TRIG_LEVEL_8BYTE   =  2 << BITP_UART_FCR_RFTRIG,  /*!< 8-byte to trigger RX interrupt.  */
    
    ADI_UART_RX_FIFO_TRIG_LEVEL_14BYTE  =  3 << BITP_UART_FCR_RFTRIG   /*!< 14-byte to trigger RX interrupt. */

}ADI_UART_TRIG_LEVEL;

/*!
 *****************************************************************************
 *  \enum ADI_UART_WORDLEN
 *   Enumeration for data width. 
 *****************************************************************************/
typedef enum
{
    ADI_UART_WORDLEN_5BITS,             /*!< 5 bits wide. */

    ADI_UART_WORDLEN_6BITS,             /*!< 6 bits wide. */

    ADI_UART_WORDLEN_7BITS,             /*!< 7 bits wide. */

    ADI_UART_WORDLEN_8BITS              /*!< 8 bits wide. */

} ADI_UART_WORDLEN;

/*!
 *****************************************************************************
 *  \enum ADI_UART_PARITY
 *   Enumeration for parity check.
 *****************************************************************************/
typedef enum
{
    ADI_UART_NO_PARITY          =  0x0,  /*!< No parity.          */

    ADI_UART_ODD_PARITY         =  0x8,  /*!< Odd parity.         */

    ADI_UART_EVEN_PARITY        =  0x18, /*!< Even Parity.        */

    ADI_UART_ODD_PARITY_STICKY  =  0x28, /*!< Sticky odd parity.  */

    ADI_UART_EVEN_PARITY_STICKY =  0x38  /*!< Sticky even parity. */
      
} ADI_UART_PARITY;

/*!
 *****************************************************************************
 *  \enum ADI_UART_STOPBITS
 *   Enumeration for the number of stop bits.
 *****************************************************************************/
typedef enum
{
    
    ADI_UART_ONE_STOPBIT               = 0x00,          /*! One stop bit regardless of the word length                           */
   
    ADI_UART_ONE_AND_HALF_TWO_STOPBITS = 0x04           /*! Number of stop bits based on word length. 1.5 stop bits 
                                                            for word length of 5 bits and 2 for rest( 6,7,8 bit word length)     */
      
} ADI_UART_STOPBITS;

/*!
 *****************************************************************************
 * \enum  ADI_UART_TRANSFER_MODE
 *  Enumeration for data transfer mode.
 *****************************************************************************/
typedef enum
{
    
    ADI_UART_DATA_TRANSFER_MODE_NONE,       /*! Mode of data transfer is not selected.                             */ 
    
    ADI_UART_DATA_TRANSFER_MODE_BLOCKING,   /*! Blocking mode. Only calls to adi_uart_Read or adi_uart_write  
                                                are allowed for sending or receiving data.                         */
    
    ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING /*! Non-Blocking mode. Only calls to adi_uart_SubmitRxBuffer or
                                                adi_uart_SubmitTxBuffer are allowed for sending or receiving data. */

} ADI_UART_TRANSFER_MODE;


/******************************************************************************
 * UART Device external API function prototypes
 *****************************************************************************/

/*
 * Device initialization and uninitialization interfaces.
*/
ADI_UART_RESULT adi_uart_Open(
                uint32_t           const nDeviceNum,
                ADI_UART_DIRECTION const eDirection,
                void                    *pMemory,
                uint32_t           const nMemSize,
                ADI_UART_HANDLE   *const phDevice
);

ADI_UART_RESULT adi_uart_Close(
                ADI_UART_HANDLE  const   hDevice
);


/******************************************************************************
 * Eliminatable functions that may be optimized out by the linker
 *****************************************************************************/

/*
 * Non-blocking mode functions.
*/

ADI_UART_RESULT adi_uart_SubmitTxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void           *const    pBuffer,
                uint32_t        const    nBufSize,
                bool            const    bDMA
);

ADI_UART_RESULT adi_uart_SubmitRxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void           *const    pBuffer,
                uint32_t        const    nBufSize,
                bool            const    bDMA
);

ADI_UART_RESULT adi_uart_GetTxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void          **const    ppBuffer,
                uint32_t                *pHwError
);

ADI_UART_RESULT adi_uart_GetRxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void          **const    ppBuffer,
                uint32_t                *pHwError
);
ADI_UART_RESULT adi_uart_IsTxBufferAvailable(
                ADI_UART_HANDLE const    hDevice,
                bool           *const    pbAvailable
);

ADI_UART_RESULT adi_uart_IsRxBufferAvailable(
                ADI_UART_HANDLE const    hDevice,
                bool           *const    pbAvailable
);

/*
 * Blocking mode functions.
*/

ADI_UART_RESULT adi_uart_Write(
                ADI_UART_HANDLE const    hDevice,
                void           *const    pBuffer,
                uint32_t        const    nBufSize,
                bool            const    bDMA,
                uint32_t                *pHwError
);

ADI_UART_RESULT adi_uart_Read(
                ADI_UART_HANDLE const    hDevice,
                void           *const    pBuffer,
                uint32_t        const    nBufSize,
                bool            const    bDMA,
                uint32_t                *pHwError
);


/*
 * Configuration interface functions.
*/

ADI_UART_RESULT adi_uart_EnableLoopBack(
                ADI_UART_HANDLE const     hDevice,
                bool            const     bEnable
);

ADI_UART_RESULT adi_uart_EnableAutobaud(
                ADI_UART_HANDLE const     hDevice,
                bool            const     bEnable,
                bool            const     bAutobaudCallbackMode
);

ADI_UART_RESULT adi_uart_SetRxFifoTriggerLevel(
                ADI_UART_CONST_HANDLE     const hDevice,
                ADI_UART_TRIG_LEVEL       const eTriglevel
);

ADI_UART_RESULT adi_uart_EnableFifo(
                ADI_UART_HANDLE const     hDevice,
                bool            const     bEnable
);

ADI_UART_RESULT adi_uart_GetBaudRate(
                ADI_UART_HANDLE const     hDevice,
                uint32_t                 *pnBaudRate,
                uint32_t                 *pAutobaudError
);

ADI_UART_RESULT adi_uart_ForceTxBreak(
                ADI_UART_HANDLE const     hDevice,
                bool            const     bEnable
);

ADI_UART_RESULT adi_uart_SetConfiguration(
                ADI_UART_HANDLE   const  hDevice,
                ADI_UART_PARITY   const  eParity,
                ADI_UART_STOPBITS const  eStopBits,
                ADI_UART_WORDLEN  const  eWordLength
);

ADI_UART_RESULT adi_uart_ConfigBaudRate(
                ADI_UART_HANDLE   const hDevice,
                uint16_t          const nDivC,
                uint8_t           const nDivM,
                uint16_t          const nDivN,
                uint8_t           const nOSR  
);

/*
 * Channel data control functions.
*/

ADI_UART_RESULT adi_uart_FlushTxFifo(
                ADI_UART_CONST_HANDLE const  hDevice
);

ADI_UART_RESULT adi_uart_FlushRxFifo(
                ADI_UART_CONST_HANDLE const  hDevice
);

ADI_UART_RESULT adi_uart_FlushRxChannel(
                ADI_UART_CONST_HANDLE const  hDevice
);


ADI_UART_RESULT adi_uart_FlushTxChannel(
                ADI_UART_CONST_HANDLE const  hDevice
);

ADI_UART_RESULT adi_uart_IsTxComplete(
                ADI_UART_HANDLE const hDevice,
                bool           *const pbComplete
);

/*
 * Callback functions.
*/

ADI_UART_RESULT adi_uart_RegisterCallback(
                ADI_UART_HANDLE const    hDevice,
                const ADI_CALLBACK       pfCallback,
                void           *const    pCBParam
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*@}*/

#endif /* ADI_UART_H */


