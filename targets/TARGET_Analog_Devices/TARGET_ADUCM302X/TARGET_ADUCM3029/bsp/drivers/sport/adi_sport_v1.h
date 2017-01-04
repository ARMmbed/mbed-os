/*! *****************************************************************************
 * @file:    adi_sport_v1.h
 * @brief:   SPORT(Serial Port) Device driver definitions for ADuCM302x processor
 * @details: Header File for the SPORT driver API functions and definitions
 * @version: $Revision:
 * @date:    $Date:
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
/** @addtogroup SPORT_Driver SPORT Device Driver
 *  @{
 */


#ifndef _ADI_SPORT_V1_H_
#define _ADI_SPORT_V1_H_

/*=============  I N C L U D E S   =============*/

#include <string.h>
#include <assert.h>

#include <adi_sport_config.h>
#include <services/dma/adi_dma.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*==============  D E F I N E S  ===============*/
/*! Amount of memory(In bytes) required by the SPORT device driver  for managing the operation
 *  in interrupt mode. This memory is completely owned by the driver till the end of
 *  the operation.
 */
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
#define ADI_SPORT_MEMORY_SIZE    (228u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#else
#define ADI_SPORT_MEMORY_SIZE    (104u+ADI_DEVICE_DRIVER_RTOS_MEMORY)
#endif


/*! Handle to the SPORT Device
 */
typedef void*  ADI_SPORT_HANDLE;


/**
 *  Enumeration of different channel of the SPORT
 *
 */
typedef enum
{
    /*! First half sport  */
    ADI_HALF_SPORT_A      = 0,
    /*! Second half sport */
    ADI_HALF_SPORT_B      = 1
} ADI_SPORT_CHANNEL;

/**
 *  Enumeration for the direction of operation.
*/
typedef enum
{
    /*! Sport in Rx mode */
    ADI_SPORT_DIR_RX,
   /*! Sport in Tx mode */
    ADI_SPORT_DIR_TX
} ADI_SPORT_DIRECTION;

/**
  *  Enumeration for enabling packing.
 */
typedef enum
{
    /*! No Packing. */
    ADI_SPORT_NO_PACKING = 0x0,
    /*! 8bit packing  */
    ADI_SPORT_8BIT_PACKING = ENUM_SPORT_CTL_A_CTL_PACK_8BIT,
    /*! 16 Bit packing */
    ADI_SPORT_16BIT_PACKING= ENUM_SPORT_CTL_A_CTL_PACK_16BIT
} ADI_SPORT_PACKING_MODE;

/*!
 *   Enumeration for Hardware Error encountered by the SPORT device.
 */
 typedef enum
{
    /*! No Hardware error */
    ADI_SPORT_HW_NO_ERR           = 0x00,
    /*! Data overflow  for Rx*/
    ADI_SPORT_HW_ERR_RX_OVERFLOW  = 0x02,
    /*! Data underflow for Tx */
    ADI_SPORT_HW_ERR_TX_UNDERFLOW = 0x04,
    /*! Frame sync error */
    ADI_SPORT_HW_ERR_FS           = 0x08,
    /*! Frame sync error */
    ADI_SPORT_HW_ERR_SYSDATAERR   = 0x10,
    /*! Processed the submitted RX buffer  */
    ADI_SPORT_EVENT_RX_BUFFER_PROCESSED = 0x20,
    /*!  Processed the submitted TX  buffer  */
    ADI_SPORT_EVENT_TX_BUFFER_PROCESSED = 0x40
}ADI_SPORT_EVENT;

/**
 *  Enumeration for result code returned from the SPORT device driver functions.
 */
typedef enum
{
    ADI_SPORT_SUCCESS,                       /*!< Success. */

    ADI_SPORT_FAILED,                        /*!< Generic Failure to indicate an call to SPORT driver function returned unsuccessful. */

    ADI_SPORT_INVALID_HANDLE,                /*!< The given handle is invalid. */

    ADI_SPORT_DEVICE_IN_USE,                 /*!< The specified sport channel is in use. */

    ADI_SPORT_INVALID_CHANNEL_ID,            /*!< The specified channel is invalid. */

    ADI_SPORT_INVALID_CLOCK_RATIO,           /*!< Invalid Clock Ratio. */

    ADI_SPORT_BUFFERS_NOT_SUBMITTED,         /*!< Buffer submission failed. */

    ADI_SPORT_INVALID_WORD_LENGTH,           /*!< Invalid word size. */

    ADI_SPORT_INVALID_DATA_TYPE,             /*!< Invalid data type. */

    ADI_SPORT_INVALID_FRAME_DELAY,           /*!< Invalid frame delay(returns code only in  Multi channel mode).*/

    ADI_SPORT_INVALID_WINDOW_SIZE,            /*!< Invalid window size(returns code only in Multi channel mode).*/

    ADI_SPORT_INVALID_WINDOW_OFFSET,         /*!< Invalid window offset(returns code only in Multi channel mode).*/

    ADI_SPORT_INVALID_MODE,                  /*!< Specified mode is invalid. */

    ADI_SPORT_INVALID_DEVICE_NUM ,           /*!< Invalid device number. */

    ADI_SPORT_INVALID_NULL_POINTER,          /*!< Specified pointer is invalid. */

    ADI_SPORT_MODE_NOT_PERMITTED,            /*!< Not permitted in  the mode with  which device is opened. */

    ADI_SPORT_DMA_FAILED,                    /*!< Failed to close the DMA channel associated with specified SPORT device. */

    ADI_SPORT_INVALID_REGISTER,              /*!< Specified  register is invalid. */

    ADI_SPORT_ISUFFICIENT_MEMORY,            /*!< specified memory is not sufficient for managing the device. */

    ADI_SPORT_INVALID_DIR,                   /*!< Invalide Direction. */

    ADI_SPORT_OPERATION_NOT_ALLOWED,         /*!< Specified operation is not allowed when data flow is enabled. */

    ADI_SPORT_PERIPHERAL_ERROR,               /*!< SPORT device encountered an peripheral error. */

    ADI_SPORT_INVALID_PARAMETER               /*!< Specified parameter is not valid */
} ADI_SPORT_RESULT;

/**
 *  \enum ADI_SPORT_REGISTER
 *  Enumeration for SPORT control register. User can use these enumeration to specify the which
 *  control registers need to be modified with the given value.
 *
 * */
typedef enum
{
    /*! "Main control register" for SPORT. Controls the clock, data format, MSB/LSB first selection etc. */
    ADI_SPORT_CONTROL_REGISTER,
    /*! The divisior register  contains divisor values that determine frequencies of internally-generated clocks and frame syncs for sport  */
    ADI_SPORT_DIVISIOR_REGISTER,
    /*! The Interrupt enable register that contains all the fields related for enabling various interrupts related to errors and data requests. */
    ADI_SPORT_INTERRUPT_REGISTER,
    /*!This register contains the settings related to the CONVT signal generation */
    ADI_SPORT_TIMER_CONVT

} ADI_SPORT_REGISTER;

/**********************************************************
  General SPORT APIs
**********************************************************/
/*
 * To open the device
*/
ADI_SPORT_RESULT adi_sport_Open(
                 uint32_t              nDeviceNum,
                 ADI_SPORT_CHANNEL     eChannel,
                 ADI_SPORT_DIRECTION   eDirection,
                 void                 *pMemory,
                 uint32_t              nMemSize,
                 ADI_SPORT_HANDLE   *const   phDevice
                 );
/*
 * To close the device
*/

ADI_SPORT_RESULT adi_sport_Close(
                 ADI_SPORT_HANDLE const hDevice
                 );
/*
 * To enable/disalbe the data flow.
*/
ADI_SPORT_RESULT adi_sport_Enable(
                 ADI_SPORT_HANDLE const  hDevice,
                 bool_t                  bEnable
                 );
/*
 * Submit an buffer to the driver.
*/
ADI_SPORT_RESULT adi_sport_SubmitBuffer(
                 ADI_SPORT_HANDLE const  hDevice,
                 void            *const  pBuffer,
                 uint32_t                nBufSize
		 );
/*
 * Get the processed buffer from the driver.
*/
ADI_SPORT_RESULT adi_sport_GetBuffer(
                 ADI_SPORT_HANDLE const   hDevice,
                 void         **const     ppBuffer
                 );
/*
 * peek function to know whether an processed buffer is avilable.
*/

ADI_SPORT_RESULT adi_sport_IsBufferAvailable(
                 ADI_SPORT_HANDLE const hDevice,
                 bool_t            *const pbAvailable
                 );
/*
 * To register the callback function
*/
ADI_SPORT_RESULT adi_sport_RegisterCallback(
                 ADI_SPORT_HANDLE const hDevice,
                 const ADI_CALLBACK     pfCallback,
                 void         *const    pCBparam
                 );
/*
 * To enable/disable the dma mode of operation.
*/
ADI_SPORT_RESULT adi_sport_EnableDMAMode(
                 ADI_SPORT_HANDLE const hDevice,
                 bool_t             bEnableDMAmode
                 );
/*
 * Configure the data
*/
ADI_SPORT_RESULT adi_sport_ConfigData(
                 ADI_SPORT_HANDLE  const  hDevice,
                 uint8_t            nWordLength,
                 ADI_SPORT_PACKING_MODE  ePackMode,
                 bool_t               bLSBFirst
                 );
/*
 * Configure the Clock
*/
ADI_SPORT_RESULT adi_sport_ConfigClock(
                 ADI_SPORT_HANDLE const  hDevice,
                 uint16_t                nClockRatio,
                 bool_t                  bUseIntlClock,
                 bool_t                  bRisingEdge,
                 bool_t                  bGatedClk
                 );
/*
 * Configure the frame sync.
*/
ADI_SPORT_RESULT adi_sport_ConfigFrameSync(
                 ADI_SPORT_HANDLE const hDevice,
                 uint16_t               nFsDivisor,
                 bool_t                   bFSRequired,
                 bool_t                   bInternalFS,
                 bool_t                   bDataFS,
                 bool_t                   bActiveLowFS,
                 bool_t                   bLateFS,
                 bool_t                   bFSErrorOperation
                 );
/*
 * To get the Hardware error(To be used only in non-blocking mode).
*/
ADI_SPORT_RESULT adi_sport_GetHWErrorStatus (
                 ADI_SPORT_HANDLE  const   hDevice,
                 uint32_t                 *pHwError,
                 void *                   *pArg
                 );
/*
 * Set the control word to specified register.
*/
ADI_SPORT_RESULT adi_sport_SetRegister(
                 ADI_SPORT_HANDLE  const  hDevice,
                 ADI_SPORT_REGISTER   eRegister,
                 uint32_t                 nControlWord
                 );
/*
 * Get the control word from the  specified register.
*/
ADI_SPORT_RESULT adi_sport_GetRegister(
                 ADI_SPORT_HANDLE  const  hDevice,
                 ADI_SPORT_REGISTER   eRegister,
                 uint32_t                *pControlWord
                 );

/*
 * To mux the halfsport. This makes the device to use FS and Clock from other halfsport.
*/

ADI_SPORT_RESULT adi_sport_MultiplexSportSignal(
                 ADI_SPORT_HANDLE   hDevice,
                 bool_t             bUseOtherFS,
                 bool_t             bUseOtherClk
                 );

/*
 * To confuigure the SPORT in timer mode.
*/

ADI_SPORT_RESULT adi_sport_ConfigTimerMode(
                 ADI_SPORT_HANDLE   hDevice,
                 uint8_t nFSDuration,
                 uint8_t nWidth,
                 bool_t bActiveLow
                 );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ADI_SPORT_V1_H_ */

/*@}*/
