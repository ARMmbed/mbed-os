/*! *****************************************************************************
 * @file    adi_uart_v1.h
 * @brief   UUART Device driver definitions.
 * @details Header File for the UART driver API functions and definitions.
 * @version $Revision:
 * @date    $Date:
 -----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

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
/*! @addtogroup UART_Driver UART Device Driver
 *  @{
 */

#ifndef __ADI_UART_v1_H__
#define __ADI_UART_v1_H__

/*=============  I N C L U D E S   =============*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <services/dma/adi_dma.h>
#include <services/pwr/adi_pwr.h>
#include <services/int/adi_int.h>
#include <adi_uart_config.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*==============  D E F I N E S  ===============*/
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
/*! Amount of memory(In bytes) required by the UART device driver for operating in unidirection( either RX or TX ).
 *  This memory is completely owned by the driver till the end of the operation.  */
#define ADI_UART_UNIDIR_MEMORY_SIZE    (180U + ADI_DEVICE_DRIVER_RTOS_MEMORY + ADI_DMA_MEMORY_SIZE)

/*! Amount of memory(In bytes) required by the UART device driver for operating in bidirection( Both RX and TX ).
 *  This memory is completely owned by the driver till the end of the operation.  */
#define ADI_UART_BIDIR_MEMORY_SIZE     (312U + ADI_DEVICE_DRIVER_RTOS_MEMORY*2u + ADI_DMA_MEMORY_SIZE*2u)

#else
/*! Amount of memory(In bytes) required by the UART device driver for operating in unidirection( either RX or TX).
 *  This memory is completely owned by the driver till the end of the operation.  */
#define ADI_UART_UNIDIR_MEMORY_SIZE    (152U + ADI_DEVICE_DRIVER_RTOS_MEMORY)

/*! Amount of memory(In bytes) required by the UART device driver for operating in bidirection( Both RX and TX).
 *  This memory is completely owned by the driver till the end of the operation.  */
#define ADI_UART_BIDIR_MEMORY_SIZE     (256U + ADI_DEVICE_DRIVER_RTOS_MEMORY*2u)
#endif

/*!
 * Handle for managing the UART Device
 */
typedef void*  ADI_UART_HANDLE;

/*!
 *  \enum ADI_UART_DIRECTION
 *   Enumeration for  UART  direction
 * */
typedef enum
{
    ADI_UART_DIR_TRANSMIT = 0X01,              /*!< UART in transmit mode */

    ADI_UART_DIR_RECEIVE =0X02,               /*!< UART in receive  mode */

    ADI_UART_DIR_BIDIRECTION =0X03           /*!< UART in Bidirectional */

} ADI_UART_DIRECTION;

/*!
 *  \enum ADI_UART_WORDLEN
 *   Enumeration for  data width
 */
typedef enum
{

    ADI_UART_WORDLEN_5BITS,             /*!< 5 Bits wide */

    ADI_UART_WORDLEN_6BITS ,            /*!< 6 Bits Wide */

    ADI_UART_WORDLEN_7BITS,             /*!< 7 Bits Wide */

    ADI_UART_WORDLEN_8BITS              /*!< 8 Bits Wide */

} ADI_UART_WORDLEN;

/*!
 *  \enum ADI_UART_PARITY
 *   Enumeration for Parity check
 */
typedef enum
{

    ADI_UART_NO_PARITY          =  0x0, /*!< No parity */

    ADI_UART_ODD_PARITY         =  0x8, /*!< Odd parity */

    ADI_UART_EVEN_PARITY        = 0x18, /*!< Even Parity */

    ADI_UART_EVEN_PARITY_STICKY = 0x28, /*!< Sticky Even parity */

    ADI_UART_ODD_PARITY_STICKY  = 0x38  /*!< Sticky Odd parity */
} ADI_UART_PARITY;

/*!
 *  \enum ADI_UART_STOPBITS
 *   Enumeration for Stop bits.
 */
typedef enum
{
    /*! One Stop bit regardless of the Word length */
    ADI_UART_ONE_STOPBIT              = 0x00,
    /*!  Number of stop bits based on word length. 1.5 Stop bits for Word length of 5 
     \n bits and 2 for rest( 6,7,8 bit word length)*/
    ADI_UART_ONE_AND_HALF_TWO_STOPBITS = 0X04
} ADI_UART_STOPBITS;

/*!
 *  \enum ADI_UART_EVENT
 *   Enumeration for Hardware events encountered by the UART device.
 */
 typedef enum
{

    ADI_UART_HW_NO_ERR                            =        0,    /*!< No error  */

    ADI_UART_HW_ERR_OVERRUN                       =      0x02,   /*!< Receive overrun */

    ADI_UART_HW_ERR_PARITY                        =      0x04,   /*!< Detected Rx parity error */

    ADI_UART_HW_ERR_FRAMING                       =      0x08,   /*!< Detected Rx framing error */

    ADI_UART_EVENT_BREAK_INTERRUPT                =      0x10,   /*!< Detected break condition */

    ADI_UART_EVENT_RX_BUFFER_PROCESSED            =      0X20,   /*!< The given Rx buffer is processed */

    ADI_UART_EVENT_TX_BUFFER_PROCESSED            =      0X40,   /*!< The given Tx buffer is processed */

    ADI_UART_EVENT_RX_DMA_ERROR                   =      0x80,   /*!< DMA error occurred in Rx channel. */

    ADI_UART_EVENT_TX_DMA_ERROR                   =     0x100,   /*!< DMA error occurred in Tx channel. */

    ADI_UART_EVENT_AUTOBAUD_COMPLETE              =     0x200,   /*!< The initiated autobaud is complete. */

    ADI_UART_EVENT_TIMEOUT_LONGBREAK              =     0x400,   /*!< The timeout due to long break conditions during autobaud. */
    
    ADI_UART_EVENT_TIMEOUT_NO_START_EDGE          =     0x800,   /*!< The timeout due to no valid start edge found during autobaud. */

    ADI_UART_EVENT_TIMEOUT_NO_END_EDGE            =    0x1000    /*!< The timeout due to no valid end edge found during autobaud. */
    
}ADI_UART_EVENT;
/*!
 *  \enum ADI_UART_RESULT
 *   Enumeration for result code returned from the UART device driver functions.
 */
 typedef enum
{

    ADI_UART_SUCCESS,                      /*!< Success. */

    ADI_UART_FAILED,                       /*!< Generic Failure */

    ADI_UART_INVALID_HANDLE,               /*!< The given handle is invalid */

    ADI_UART_DEVICE_IN_USE,                /*!< The specified UART device is in use */

    ADI_UART_INT_REGISTRATION_FAILED,      /*!< Failed to register the specified UART device with interrupt manager*/

    ADI_UART_DMA_FAILED,                   /*!< An DMA operation resulted while operating a DMA associated with UART device */

    ADI_UART_SUBMIT_BUFFER_FAILED,         /*!< Failed to submit a Buffer for data transfer */

    ADI_UART_INVALID_WORD_LENGTH,          /*!< Specified  word size is invalid(valid range 5-8)*/

    ADI_UART_INVALID_DEVICE_NUM ,          /*!< Specified  device number  is invalid.*/

    ADI_UART_INVALID_NULL_POINTER,         /*!< Specified pointer is NULL and hence it is invalid pointer. */

    ADI_UART_ISUFFICIENT_MEMORY,           /*!< specified memory is insufficient for managing the device */

    ADI_UART_INVALID_DIR,                  /*!< Specified  Direction  of operation is invalid*/

    ADI_UART_OPERATION_NOT_ALLOWED,        /*!< Specified operation is not allowed when data flow is enabled. */

    ADI_UART_PERIPHERAL_ERROR,             /*!< UART device encountered an peripheral error. */

    ADI_UART_INVALID_STOP_BITS,            /*!< Specified  Stop bit is invalid.*/

    ADI_UART_INVALID_PARAMETER,             /*!<  Specified parameter  is invalid.*/

    ADI_UART_INVALID_DATA_TRANSFER_MODE,   /*!< Error when  trying to submit a buffer for data transfer in
                                               "Blocking mode" while device is operating in "Non blocking mode"
                                               OR vice versa.*/

    ADI_UART_BUFFER_NOT_SUBMITTED,         /*!< Enabling the device before Buffer(s) are  submitted.*/
    
    ADI_UART_DMA_ALREADY_INITIALIZED,      /*!< Device is already in DMA mode */

    ADI_UART_AUTOBAUD_IN_PROGRESS      /*!<  Autobaud detection is in progress */

} ADI_UART_RESULT;


/*! @brief Enumeration for trigger level  FIFO
 */
typedef enum
{

    ADI_UART_RX_FIFO_TRIG_LEVEL_1BYTE =  0 << BITP_UART_COMFCR_RFTRIG,  /*!< 1-byte to trigger RX interrupt. */
    ADI_UART_RX_FIFO_TRIG_LEVEL_4BYTE =  1 << BITP_UART_COMFCR_RFTRIG,  /*!< 4-byte to trigger RX interrupt. */
    ADI_UART_RX_FIFO_TRIG_LEVEL_8BYTE =  2 << BITP_UART_COMFCR_RFTRIG,  /*!< 8-byte to trigger RX interrupt. */
    ADI_UART_RX_FIFO_TRIG_LEVEL_14BYTE =  3 << BITP_UART_COMFCR_RFTRIG   /*!< 14-byte to trigger RX interrupt. */

}ADI_UART_TRIG_LEVEL;

/*
 * To open the device
*/
ADI_UART_RESULT adi_uart_Open(
                uint32_t const           nDeviceNum,
                ADI_UART_DIRECTION const eDirection,
                void                     *pMemory,
                uint32_t const           nMemSize,
                ADI_UART_HANDLE  *const  phDevice
                );

/*
 * To close the device
*/
ADI_UART_RESULT adi_uart_Close(
                ADI_UART_HANDLE  const   hDevice
                );

/*
 * To enable/disalbe the data flow for TX.
*/
ADI_UART_RESULT adi_uart_EnableTx(
                ADI_UART_HANDLE const    hDevice,
                bool_t                     bEnable
                );

/*
 * To enable/disalbe the data flow for RX.
*/
ADI_UART_RESULT adi_uart_EnableRx(
                ADI_UART_HANDLE  const   hDevice,
                bool_t                     bEnable
                );

/*
 * Submit an transmit(TX) buffer to the driver.
*/

ADI_UART_RESULT adi_uart_SubmitTxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void           *const    pBuffer,
                uint32_t                 nBufSize
                );

/*
 * Submit an receive (RX) buffer to the driver.
*/

ADI_UART_RESULT adi_uart_SubmitRxBuffer(
                ADI_UART_HANDLE  const   hDevice,
                void            *const   pBuffer,
                uint32_t                 nBufSize
                );

/*
 * Get the processed buffer from the driver (For TX).
*/
ADI_UART_RESULT adi_uart_GetTxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void          **const    ppBuffer
                );

/*
 * Get the processed   buffer from the driver(For RX).
*/
ADI_UART_RESULT adi_uart_GetRxBuffer(
                ADI_UART_HANDLE const    hDevice,
                void          **const    ppBuffer
                );

/*
 *Submit the buffer for writing (i.e For transmiting the data )
*/
ADI_UART_RESULT adi_uart_Write(
                ADI_UART_HANDLE const    hDevice,
                void           *const    pBuffer,
                uint32_t                 nBufSize
                );

/*
 *Submit the buffer for reading (i.e For receiving  the data )
*/
ADI_UART_RESULT adi_uart_Read(
                ADI_UART_HANDLE const    hDevice,
                void     *const          pBuffer,
                uint32_t                 nBufSize
                );

/*
 * peek function to know whether an processed buffer(For TX) is avilable.
*/
ADI_UART_RESULT adi_uart_IsTxBufferAvailable(
                ADI_UART_HANDLE const    hDevice,
                bool     *const          pbAvailable
                );
/*
 * peek function to know whether an processed buffer(For RX) is avilable.
*/
ADI_UART_RESULT adi_uart_IsRxBufferAvailable(
                ADI_UART_HANDLE   const  hDevice,
                bool  *const             pbAvailable
                );

/*
 * To register the callback function
*/
ADI_UART_RESULT adi_uart_RegisterCallback(
                ADI_UART_HANDLE const    hDevice,
                const ADI_CALLBACK        pfCallback,
                void           *const    pCBParam
                                       );
/*
 * To configure  the device
*/

ADI_UART_RESULT adi_uart_SetConfiguration(
                ADI_UART_HANDLE   const   hDevice,
                ADI_UART_PARITY   const  eParity,
                ADI_UART_STOPBITS const  eStopBits,
                ADI_UART_WORDLEN  const  eWordLength
                );
/*
 * To enable/disable the dma mode of operation.
*/

ADI_UART_RESULT adi_uart_EnableDMAMode(
                ADI_UART_HANDLE   const       hDevice,
                bool_t                          bEnable
                );

 /*
 * To enable/disable the dma mode of operation for TX.
*/

ADI_UART_RESULT adi_uart_EnableDMAModeForTx(
                ADI_UART_HANDLE   const       hDevice,
                bool_t                          bEnable
                );

/*
 * To enable/disable the dma mode of operation for RX.
*/

ADI_UART_RESULT adi_uart_EnableDMAModeForRx(
                ADI_UART_HANDLE   const       hDevice,
                bool_t                          bEnable
                );
/*
 * To set divisor for which in turn determines the baudrate for device
*/
ADI_UART_RESULT adi_uart_ConfigBaudRate(
                           ADI_UART_HANDLE const hDevice,
                           uint16_t         nDivC,
                           uint8_t          nDivM,
                           uint16_t         nDivN,
                           uint8_t          nOSR             
                           );
/*
 * To Get the baudrate.
*/
ADI_UART_RESULT adi_uart_GetBaudRate(
                ADI_UART_HANDLE  const        hDevice,
                uint32_t                *pnBaudRate
                );
/*
 * To force the Tx break.
*/
ADI_UART_RESULT adi_uart_ForceTxBreak(
                ADI_UART_HANDLE  const hDevice,
                bool_t                   bEnable
                );
/*
 * To Get the HW error status of the device.
*/
ADI_UART_RESULT adi_uart_GetHWErrorStatus (
                ADI_UART_HANDLE  const   hDevice,
                uint32_t                *pHwError,
                void *                  *pArg
                );

/*
 * To Get the HW error status of the device.
*/
ADI_UART_RESULT adi_uart_EnableLoopBack(
                ADI_UART_HANDLE const    hDevice,
                bool_t                    bEnable
                );
/*
 * To know whether data transmission is complete or not.
*/
ADI_UART_RESULT adi_uart_IsTxComplete(
		        ADI_UART_HANDLE const hDevice,
                bool_t      *const       pbComplete
                );
/*
 * To Enable/Disable Autobaud.
*/
ADI_UART_RESULT adi_uart_EnableAutobaud(
		        ADI_UART_HANDLE const hDevice,
                bool_t    bEnable
                );

ADI_UART_RESULT adi_uart_ConfigAutobaud(
                ADI_UART_HANDLE const hDevice,
                uint8_t         cAutoBaud
                );
                
ADI_UART_RESULT adi_uart_SetRxFifoTriggerLevel(
                ADI_UART_HANDLE  const hDevice,
                ADI_UART_TRIG_LEVEL eTriglevel
                );
                
ADI_UART_RESULT adi_uart_EnableFifo(
                ADI_UART_HANDLE const hDevice,
                bool_t bEnable
                );
                
ADI_UART_RESULT adi_uart_ClearTxFifo(
                ADI_UART_HANDLE const hDevice
                );

ADI_UART_RESULT adi_uart_ClearRxFifo(
                ADI_UART_HANDLE const hDevice
                );
                
ADI_UART_RESULT adi_uart_InvertRxLine(
                ADI_UART_HANDLE  const hDevice, 
                bool_t bEnable);
                
ADI_UART_RESULT adi_uart_DisableRxDuringTx(
                ADI_UART_HANDLE  const hDevice, 
                bool_t bEnable);
                
ADI_UART_RESULT adi_uart_HoldTxDuringRx(
                ADI_UART_HANDLE  const hDevice, 
                bool_t bEnable);
                
ADI_UART_RESULT adi_uart_SetDeassertHalfBit(
                ADI_UART_HANDLE  const hDevice, 
                bool_t bEnable);
                
ADI_UART_RESULT adi_uart_SetSoutPolarity(
                ADI_UART_HANDLE  const hDevice, 
                bool_t bEnable);                

ADI_UART_RESULT adi_uart_EnableRxStatusInterrupt(
               ADI_UART_HANDLE         const hDevice,
                bool_t                 bEnable
);

ADI_UART_RESULT adi_uart_EnableModemStatusInterrupt(
               ADI_UART_HANDLE         const hDevice,
                bool_t                 bEnable
);

ADI_UART_RESULT adi_uart_RegisterHWEventCallback(
                ADI_UART_HANDLE const    hDevice,
                const ADI_CALLBACK       pfCallback,
                void           *const    pCBParam
                                       );

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __ADI_UART_BF5XX_H__ */
/*@}*/
