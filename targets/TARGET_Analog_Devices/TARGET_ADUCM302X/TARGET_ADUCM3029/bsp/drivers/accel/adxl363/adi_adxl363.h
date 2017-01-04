/*!
 ******************************************************************************
 * @file:    adi_adxl363.h
 * @brief:   ADXL363 API header file
 * @version: $Revision: 33422 $
 * @date:    $Date: 2016-02-04 12:42:05 +0000 (Thu, 04 Feb 2016) $
 *-----------------------------------------------------------------------------
 *
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

#ifndef _ADI_ADXL363_H_
#define _ADI_ADXL363_H_

/** @addtogroup adxl363_Driver adxl363 Device Driver
 *  @{
 */

#include <stdint.h>
#include <services/int/adi_int.h>
#include <drivers/spi/adi_spi.h>
#include <services/gpio/adi_gpio.h>
#include "adi_adxl363_reg.h"
#include <adi_adxl363_config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Memory required by the ADXL363 driver including the memory required for its dependent drivers */
#define ADI_ADXL363_MEMORY_SIZE  (28U)

/*! ADXL363 device handle. */
typedef void*  ADI_ADXL363_HANDLE;

/*! Enumeration of return codes from ADXL363 driver. */
typedef enum
{
    /*! The API is successful. */
    ADI_ADXL363_SUCCESS,

    /*! The API failed to complete the operation. */
    ADI_ADXL363_FAILURE,

    /*! A hardware error was detected. */
    ADI_ADXL363_HW_ERROR,

    /*! The given device number is outside the number of instances supported
     * by the driver.*/
    ADI_ADXL363_INVALID_DEVICE_NUM,

    /*! The given device handle is invalid. */
    ADI_ADXL363_INVALID_HANDLE,

    /*! The given memory is not sufficient to operate the device. */
    ADI_ADXL363_INSUFFICIENT_MEMORY,

    /*! One of the given pointer parameter points to NULL */
    ADI_ADXL363_NULL_POINTER,

    /*! The given pointer to the device memory is not aligned to word boundary. */
    ADI_ADXL363_UNALIGNED_MEM_PTR,

    /*! The given device instance is already opened. */
    ADI_ADXL363_DEVICE_ALREADY_OPENED,

    /*! The given device instance is not opened. */
    ADI_ADXL363_DEVICE_NOT_OPENED,

    /*! Failed to register or unregister callback with GPIO service */
    ADI_ADXL363_GPIO_FAILED,

    /*! The given operation is not permitted in the current state of the driver. */
    ADI_ADXL363_INVALID_OPERATION,

    /*! The given axis is invalid. */
    ADI_ADXL363_INVALID_AXIS,

    /*! Failed to read or write the device registers. */
    ADI_ADXL363_DEVICE_ACCESS_FAILED,

    /*! Communication device (SPI) related failure. */
    ADI_ADXL363_SPI_DEV_FAILED,

    /*! The given power saving mode is invalid. */
    ADI_ADXL363_INVALID_PWR_MODE,

    /*! The given activity threshold is outside the valid range. */
    ADI_ADXL363_INVALID_ACT_THRESH,

    /*! The given inactivity threshold is outside the valid range. */
    ADI_ADXL363_INVALID_INACT_THRESH,

    /*! If the given number of FIFO samples is outside the valid range. */
    ADI_ADXL363_INVALID_FIFO_SAMPLES,

    /*! If trying to perform a read when another read/write in progress. */
    ADI_ADXL363_PENDING_IO,

    /*! If trying to enable the DMA mode when communication device is TWI. */
    ADI_ADXL363_DMA_NOT_SUPPORTED

} ADI_ADXL363_RESULT;


/*! Enumeration of events generated by ADXL363.
 *
 *
 * */
typedef enum
{
    /*! Interrupt is generated via interrupt pin1. */
    ADI_ADXL363_EVENT_INT1,

    /*! Interrupt is generated via interrupt pin2. */
    ADI_ADXL363_EVENT_INT2

} ADI_ADXL363_EVENT;


/*! Enumeration of different sources for generation of the interrupts.
 *
 * The drivers enables the interrupt sources appropriately depending upon the modes.
 * But application can override this by setting the mask using the API
 * adi_adxl363_SetInterrupt. It is not recommended to set the mask explicitly as
 * it can affect the functionality of the driver.
 *
 * ADXL363 allows each of the interrupt sources to be mapped to one of the two
 * interrupt pins. The map can be set using adi_adxl363_SetIntMap.
 *
 * When an interrupt occurs it is notified as an event (please refer
 * #ADI_ADXL363_EVENT) to the application via the callback.
 * Upon getting the callback application should call the function
 * adi_adxl363_GetIntSource to get the source(s) for the interrupt. Once the
 * interrupt source is identified, application may have to call additional APIs
 * to get more information related to the interrupt source. The applicable APIs are
 * specified in the documentation of each interrupt source.If more there are more than
 * one interrupt is registered, all interrupts are ORed and notified as single event.
 *
 * */
typedef enum
{
    /*! Data is ready to read.
     *
     * This interrupt is generated when new data is available for reading. This
     * will cleared when the data is read.
     *
     * The data can be read using the blocking adi_adxl363_Read API.
     * */
    ADI_ADXL363_INT_DATA_READY = 0x01 << BITP_ADXL363_INT_DATA_READY,
    /*! FIFO is ready and to read.
     *
     * This interrupt is generated there is at least one valid sample available in the FIFO output buffer.
     * This bit is cleared when no valid data is available in the FIFO.
     * The data can be read using the blocking adi_adxl363_Read API.
     * */
    ADI_ADXL363_INT_FIFO_READY = 0x01 << BITP_ADXL363_INT_FIFO_READY,

    /*! FIFO watermark level reached.
     *
     * This interrupt is generated when the number of entries in the FIFO has
     * reached the specified watermark level. The watermark level can be set using
     * API adi_adxl363_ConfigFIFO.
     *
     * To clear the watermark interrupt application should read the data from the
     * FIFO. The number of entries in the FIFO can be obtained using
     * adi_adxl363_GetFIFOStatus. The data can be read using blocking adi_adxl363_Read.
     *
     * */
    ADI_ADXL363_INT_WATERMARK = 0x1 << BITP_ADXL363_INT_FIFO_WATERMART,

    /*! FIFO overrun occurred.
     *
     * This interrupt is generated when hardware FIFO in ADXL363 has overflowed.
     * To clear the overflow interrupt application should read the data from the
     * FIFO. The number of entries in the FIFO can be obtained using
     * adi_adxl363_GetFIFOStatus. The data can be read using the blocking
     * adi_adxl363_Read API.
     *
     * */
    ADI_ADXL363_INT_OVERRUN = 0x1<< BITP_ADXL363_INT_FIFO_OVERRUN,

    /*! Activity detected.
     *
     * This interrupt is generated when acceleration greater than the activity
     * threshold is detected on any participating axes. The participating axis
     * and threshold can be configured using adi_adxl363_ConfigActivity.
     *
     * The first axis that contributed to the activity interrupt can be obtained
     * by using the API adi_adxl363_GetActivityStatus.
     * */
    ADI_ADXL363_INT_ACTIVITY = 0x1 << BITP_ADXL363_INT_ACTIVITY,

    /*! Inactivity detected.
     *
     * This interrupt is generated when the acceleration on all the participating
     * axes is less than the inactivity threshold for more than the specified amount
     * of time. The time, threshold and participating axes can be configured using
     * adi_adxl363_ConfigInactivity.
     * */
    ADI_ADXL363_INT_INACTIVITY = 0x1 << BITP_ADXL363_INT_INACTIVITY,

    /*!  Awake Interrupt.
     *
     * When the ADXL363 detects activity, the awake bit becomes high.
     * */
    ADI_ADXL363_INT_AWAKE = 0x1 << BITP_ADXL363_INT_AWAKE


} ADI_ADXL363_INT;


/*! Enumeration of possible hardware error types */
typedef enum
{
    /*! DMA error detected. */
    ADI_ADXL363_HW_ERR_DMA,
    /*! SPI peripheral error detected. */
    ADI_ADXL363_HW_ERR_SPI,

}ADI_ADXL363_HW_ERR_TYPE;

/*!Enumeration of different data rates that ADXL363 can operate on.
 */
typedef enum
{
    /*!  12.5Hz data rate. */
    ADI_ADXL363_DATA_RATE_12_5HZ,
    /*!  25  Hz data rate. */
    ADI_ADXL363_DATA_RATE_25_0HZ,
    /*!  50  Hz data rate. */
    ADI_ADXL363_DATA_RATE_50_00HZ,
    /*!  100 Hz data rate. */
    ADI_ADXL363_DATA_RATE_100_HZ,
    /*!  200 Hz data rate. */
    ADI_ADXL363_DATA_RATE_200_HZ,
    /*!  400 Hz data rate. */
    ADI_ADXL363_DATA_RATE_400_HZ
}ADI_ADXL363_DATA_RATE;

/*! Enumeration of different modes the internal FIFO can operate. */
typedef enum
{
    /*! In this mode, no data is stored in FIFO and any data already stored in it is cleared. */
    ADI_ADXL363_DISABLE_FIFO    =        0x00 << BITP_ADXL363_FIFO_MODE,

    /*! In oldest saved mode, the FIFO accumulates data until it is full and then stops.
       Additional data is collected only when space is made available by reading samples
       out of the FIFO buffer. (This mode of operation is sometimes referred to as first N.) */
    ADI_ADXL363_FIFO_OLDEST_SAVED    =   0x01 << BITP_ADXL363_FIFO_MODE,

    /*!In stream mode, the FIFO always contains the most recent data.
      The oldest sample is discarded when space is needed to make room for a newer sample.
      (This mode of operation is sometimes referred to as last N.) */
    ADI_ADXL363_FIFO_MODE_STREAM    =    0x02 << BITP_ADXL363_FIFO_MODE,

    /*!In triggered mode, the FIFO saves samples surrounding an activity detection event.  */
    ADI_ADXL363_FIFO_MODE_TRIGGER   =    0x03 << BITP_ADXL363_FIFO_MODE

} ADI_ADXL363_FIFO_MODE;

/*! Enumeration for measurement range.
 *
 * */
typedef enum
{
    /*! Measurement Range +/- 2g */
    ADI_ADXL363_RANGE_2G =   0x0 << BITP_ADXL363_RNAGE,
    /*! Measurement Range +/- 4g */
    ADI_ADXL363_RANGE_4G =   0x1 << BITP_ADXL363_RNAGE,
    /*! Measurement Range +/- 8g */
    ADI_ADXL363_RANGE_8G =   0x2 << BITP_ADXL363_RNAGE

} ADI_MEASUREMENT_RANGE;

/*! Enumeration for filter Bandwidth
 *
 * */
typedef enum
{
    /*! 0 = the bandwidth of the filters is set to ½ the ODR for a wider bandwidth.*/	
    ADI_ADXL363_BW_HALF    =   0x0 << BITP_ADXL363_HALF_BW_EN,
    /*! The bandwidth of the antialiasing filters is set to 1/4 the output data rate (ODR) for more conservative filtering. */
    ADI_ADXL363_BW_QUARTER =   0x1 << BITP_ADXL363_HALF_BW_EN,

} ADI_FILTER_BW;

/*! Enumeration for Noise mode(To Power vs. Noise Tradeoff)
 *
 */
typedef enum
{
    /*! Normal operation.*/
    ADI_ADXL363_NOISE_NORMAL    =   0x0 << BITP_ADXL363_LINK_LOOP,
    /*! Low noise mode.*/
    ADI_ADXL363_NOISE_LOW       =   0x1 << BITP_ADXL363_LINK_LOOP,
    /*! Ultralow noise mode.*/
    ADI_ADXL363_NOISE_ULTRA_LOW =   0x3 << BITP_ADXL363_LINK_LOOP

} ADI_NOISE_MODE;
/*! Enumeration for Noise mode(To Power vs. Noise Tradeoff)
 *
 */
typedef enum
{
    /*! Activity and inactivity detection are both enabled, and their interrupts
       (if mapped) must be acknowledged by the host processor by reading the status register.*/
    ADI_ACT_INACT_MODE_NORMAL    =   0x0 << BITP_ADXL363_LOW_NOISE,
    /*! Activity and inactivity detection are linked sequentially such that only one is enabled at a time.*/
    ADI_ACT_INACT_MODE_LINK       =   0x1 << BITP_ADXL363_LOW_NOISE,
    /*!Activity and inactivity detection are linked sequentially such that only one is enabled at a time,
       and their interrupts are internally acknowledged*/
    ADI_ACT_INACT_MODE_LOOP       =    0x2 << BITP_ADXL363_LOW_NOISE
} ADI_ACT_INACT_MODE;

/*! Enumeration for inactivity mode.
 *
 * */
typedef enum
{
    /*! Activity/Inactivity detection function operates in absolute mode */
    ADI_ADXL363_ABSOULTE_DETECTION  =   0x0,
    /*! Activity/Inactivity detection function operates in referenced mode */
    ADI_ADXL363_REFERENCE_DETECTION =   0x1

} ADI_ADXL363_DETECTION_MODE;

/*! Enumeration of interrupts pins.
 *
 * The ADXL363 has two interrupt pin outs to the host processor. The FIFO trigger
 * event can be mapped to one of the interrupts pins.
 *
 * */
typedef enum
{
    /*! The interrupt output pin 1. */
    ADI_ADXL363_INTPIN1 =   0x00,          
    /*! The interrupt output pin 2. */    
    ADI_ADXL363_INTPIN2 =   0x01            

} ADI_ADXL363_INTPIN;

ADI_ADXL363_RESULT adi_adxl363_Open (
    uint32_t                        nDeviceNum,
    void * const                    pDeviceMemory,
    uint32_t                        nMemSize,
    ADI_CALLBACK                    pfCallback,
    void * const                    pCBParam,
    ADI_SPI_HANDLE                  hSPIDevice,
    ADI_ADXL363_HANDLE * const      phDevice
);

ADI_ADXL363_RESULT adi_adxl363_Close (
    ADI_ADXL363_HANDLE  const       hDevice
);

ADI_ADXL363_RESULT adi_adxl363_Enable(

    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_Read (
    ADI_ADXL363_HANDLE  const       hDevice,
    void *const                     pBuffer,
    uint32_t const                  nSize
);

ADI_ADXL363_RESULT adi_adxl363_SetDataRate (
    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_ADXL363_DATA_RATE           eDataRate
);


ADI_ADXL363_RESULT adi_adxl363_ConfigActivity (

    ADI_ADXL363_HANDLE  const       hDevice,
    uint16_t                        nThreshold,
    uint8_t                         nTime,
    ADI_ADXL363_DETECTION_MODE      eMode
);

ADI_ADXL363_RESULT adi_adxl363_EnableActivity(
    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_ConfigInactivity (

    ADI_ADXL363_HANDLE  const       hDevice,
    uint16_t                        nThreshold,
    uint16_t                        nTime,
    ADI_ADXL363_DETECTION_MODE      eMode
);

ADI_ADXL363_RESULT adi_adxl363_ActInactMode (

    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_ACT_INACT_MODE              eMode
);

ADI_ADXL363_RESULT adi_adxl363_EnableInactivity(
    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_EnableWakeUp(

    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_EnableAutoSleep(

    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_SetFilterBandWidth(
    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_FILTER_BW  eBandWidth
);

ADI_ADXL363_RESULT adi_adxl363_EnableTemperatureFIFO(
    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_GetTemperature(
    ADI_ADXL363_HANDLE  const       hDevice,
    uint16_t                        *nPdata
);

ADI_ADXL363_RESULT adi_adxl363_SetRange(
    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_MEASUREMENT_RANGE           eRange
);

ADI_ADXL363_RESULT adi_adxl363_SetIntPolarity (
    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_ADXL363_INTPIN              eIntPin,
    bool_t                          bActiveLow
);

ADI_ADXL363_RESULT adi_adxl363_EnableSelfTest (
    ADI_ADXL363_HANDLE  const       hDevice,
    bool_t                          bEnable
);

ADI_ADXL363_RESULT adi_adxl363_ConfigFIFO (
    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_ADXL363_FIFO_MODE           eFIFOMode,
    uint16_t                        nSamples
);

ADI_ADXL363_RESULT adi_adxl363_SetIntMap (

    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_ADXL363_INTPIN              eIntPin,
    uint8_t                         nIntMap
);

ADI_ADXL363_RESULT adi_adxl363_ResetDevice(
    ADI_ADXL363_HANDLE  const           hDevice
);


ADI_ADXL363_RESULT adi_adxl363_GetIntMap (

    ADI_ADXL363_HANDLE  const       hDevice,
    ADI_ADXL363_INTPIN              eIntPin,
    uint8_t * const                 pnIntMask
);

ADI_ADXL363_RESULT adi_adxl363_GetIntStatus (

    ADI_ADXL363_HANDLE  const       hDevice,
    uint8_t    *const               pnIntSource
);


ADI_ADXL363_RESULT adi_adxl363_GetEntriesInFIFO (

    ADI_ADXL363_HANDLE  const       hDevice,
    uint16_t * const                pnEntries
);

ADI_ADXL363_RESULT adi_adxl363_ConfigIntPin (

    ADI_ADXL363_HANDLE  const           hDevice,
    IRQn_Type                           eIrq,
    ADI_ADXL363_INTPIN                  eIntPin,
    ADI_GPIO_PORT                       eGPIOPort,
    ADI_GPIO_DATA                       nGPIOPin
);
ADI_ADXL363_RESULT adi_adxl363_ReadRegister (
    ADI_ADXL363_HANDLE  const           hDevice,
    uint32_t                            nAddress,
    uint8_t                            *pData,
    uint8_t                             nSize
);

#ifdef __cplusplus
}
#endif

/* @} */

#endif /* _ADI_ADXL363_H_ */
