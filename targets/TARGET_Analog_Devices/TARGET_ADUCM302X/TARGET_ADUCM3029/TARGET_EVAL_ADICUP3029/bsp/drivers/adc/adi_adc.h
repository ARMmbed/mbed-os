/*! *****************************************************************************
 * @file    adi_adc.h
 * @brief   Main include file for ADC Device driver definitions
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

#ifndef ADI_ADC_H
#define ADI_ADC_H

#include <adi_processor.h>
#include <adi_callback.h>
#include <adi_adc_config.h>
#include <rtos_map/adi_rtos_map.h>  /* for ADI_SEM_SIZE */

/** @addtogroup  ADC_Driver ADC Driver
*  @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*! Amount of memory(In bytes) required by the ADC device driver for managing the operation
 *  of a ADC controller. The memory is passed to the driver when the driver is opended.
 * The memory is completely owned by the driver till the the driver is closed.
 *
 */
#define ADI_ADC_MEMORY_SIZE (48u + ADI_SEM_SIZE)          /*!< Memory Size of the buffer required by the ADC driver */


/*!
 * \enum ADI_ADC_RESULT
 * ADC API return codes
 */
typedef enum {
    ADI_ADC_SUCCESS                  = 0,     /*!< No Error, API suceeded */
    ADI_ADC_INVALID_DEVICE_NUM,               /*!< Invalid device number passed */
    ADI_ADC_INVALID_DEVICE_HANDLE,            /*!< Invalid device handle passed */
    ADI_ADC_INVALID_STATE,                    /*!< Invalid State */
    ADI_ADC_INSUFFICIENT_MEMORY,              /*!< Insufficient memory passed to the driver */
    ADI_ADC_IN_USE,                           /*!< ADC is alreaady in use */
    ADI_ADC_INVALID_PARAMETER,                /*!< Invalid parameter passed to the driver */
    ADI_ADC_NULL_POINTER,                     /*!< Null pointer passed when expecting a valid pointer */
    ADI_ADC_FAILURE,                          /*!< General ADC Failure */
    ADI_ADC_INVALID_SEQUENCE,                 /*!< Invalid sequence of API calls */
    ADI_ADC_ERR_RTOS,                         /*!< RTOS error occurred */
    ADI_ADC_INVALID_OPERATION,                /*!< API call is an invalid operation */
    ADI_ADC_INVALID_BUFFER,                   /*!< Buffer passed to the application is invalid */
    ADI_ADC_BUFFER_OVERFLOW,                  /*!< Buffer overflow occurred */
    ADI_ADC_DMA_ERROR,			      /*!< DMA Error occurred */
    ADI_ADC_BAD_SYS_CLOCK,                    /*!< Could not retrieve core clock value. */
} ADI_ADC_RESULT;

/*!
 * \enum ADI_ADC_VREF_SRC
 * Voltage Reference source selection.
 */
typedef enum {
    ADI_ADC_VREF_SRC_INT_1_25_V,        /*!< 1.25V Internal Voltage Reference */
    ADI_ADC_VREF_SRC_INT_2_50_V,        /*!< 2.50V Internal Voltage Reference */
    ADI_ADC_VREF_SRC_EXT,               /*!< External Voltage Reference */
    ADI_ADC_VREF_SRC_VBAT,              /*!< Battery Voltage as Voltage Reference source */
} ADI_ADC_VREF_SRC;

/*!
 * \enum ADI_ADC_RESOLUTION
 * Resolution of the ADC.
 */
typedef enum {
    ADI_ADC_RESOLUTION_12_BIT,      /*!< 12-bit ADC Resolution */
    ADI_ADC_RESOLUTION_13_BIT,      /*!< 13-bit ADC Resolution */
    ADI_ADC_RESOLUTION_14_BIT,      /*!< 14-bit ADC Resolution */
    ADI_ADC_RESOLUTION_15_BIT,      /*!< 15-bit ADC Resolution */
    ADI_ADC_RESOLUTION_16_BIT       /*!< 16-bit ADC Resolution */
} ADI_ADC_RESOLUTION;

/*!
 * \typedef ADI_ADC_CHANNEL
 * Typedef for ADC Channels
 */
typedef uint32_t ADI_ADC_CHANNEL;

/*!
 * defines for ADC Channels
 */
#define    ADI_ADC_CHANNEL_0  (1u << 0u) /*!< ADC Channel 0 */
#define    ADI_ADC_CHANNEL_1  (1u << 1u) /*!< ADC Channel 1 */
#define    ADI_ADC_CHANNEL_2  (1u << 2u) /*!< ADC Channel 2 */
#define    ADI_ADC_CHANNEL_3  (1u << 3u) /*!< ADC Channel 3 */
#define    ADI_ADC_CHANNEL_4  (1u << 4u) /*!< ADC Channel 4 */
#define    ADI_ADC_CHANNEL_5  (1u << 5u) /*!< ADC Channel 5 */
#define    ADI_ADC_CHANNEL_6  (1u << 6u) /*!< ADC Channel 6 */
#define    ADI_ADC_CHANNEL_7  (1u << 7u) /*!< ADC Channel 7 */

/*!
 * \enum ADI_ADC_EVENT
 * Callback events from the ADC driver.
 */
typedef enum {
    ADI_ADC_EVENT_CALIBRATION_DONE,         /*!< Calibration done event.   arg to the callback function will be NULL. */
    ADI_ADC_EVENT_ADC_READY,	            /*!< ADC Ready event.          arg to the callback function will be null */
    ADI_ADC_EVENT_OVERFLOW,                 /*!< Overflow event occurred.  The channel(#ADI_ADC_CHANNEL) for which the overflow occurred will be passed as arg to the callback function. */	
    ADI_ADC_EVENT_HIGH_LIMIT_CROSSED,       /*!< High Limit crossed event. The channel(#ADI_ADC_CHANNEL) for which the limit is crossed will be passed as arg to the callback function. */
    ADI_ADC_EVENT_LOW_LIMIT_CROSSED,        /*!< Low Limit crossed event.  The channel(#ADI_ADC_CHANNEL) for which the limit is crossed will be passed as arg to the callback function.  */
} ADI_ADC_EVENT;

/*! Structure which hold the details of the buffer and sampling details */
typedef struct __ADI_ADC_BUFFER {
    uint32_t  nChannels;                    /*!<    Channels to sample. Should be an ORed value of #ADI_ADC_CHANNEL enum */
    void*     pDataBuffer;                  /*!<    Pointer to the Buffer to read the sample value into. If single channel(say Channel 0) is selected
                                                    then the format of buffer will be <Chan0 conversion 0><Chan0 conversion 1><Chan0 conversion 2>.... but if
                                                    multiple channels (say Channel 1 and Channel2) are selected then the format of buffer will be
                                                    <Chan1 conversion 0><Chan2 conversion 0><Chan1 conversion 1><Chan2 conversion 1><Chan1 conversion 2><Chan2 conversion 2>....
                                                    \n The pBuffer should be 2 byte aligned.
                                                    \n
                                                    \n If N is the number of channels selected then in single iteration mode the number of samples
                                                       written to in the buffer will be N and for multiple iteration, the driver will try to fill the whole
                                                       buffer with data and it is preferred that the nBuffSize be able to accommodate a multiple of N samples.
                                              */
    uint32_t  nNumConversionPasses; /*!< Num of conversion passes */
    uint32_t  nBuffSize;            /*!< Size of the buffer supplied */
} ADI_ADC_BUFFER;

/* Type def for the ADC Handle. */
typedef struct __ADI_ADC_DEVICE* ADI_ADC_HANDLE;  /*!< ADC Device Handler */


/*=============  A P I   F U N C T I O N S    P R O T O T Y P E S  =============*/

/* Opens an ADC device instance. */
ADI_ADC_RESULT adi_adc_Open (
        uint32_t        nDeviceNum,
        void*           pMemory,
        uint32_t        nMemorySize,
        ADI_ADC_HANDLE* phDevice
);

/*  Close the given device instance */
ADI_ADC_RESULT adi_adc_Close(ADI_ADC_HANDLE hDevice);

/* Power up or power down the ADC */
ADI_ADC_RESULT adi_adc_PowerUp (ADI_ADC_HANDLE hDevice, bool bPowerUp);

/* Register the callback */
ADI_ADC_RESULT adi_adc_RegisterCallback(
    ADI_ADC_HANDLE     hDevice,
    ADI_CALLBACK       pfCallback,
    void              *pCBParam
);

/* Enables/Disables the ADC Subsystem */
 ADI_ADC_RESULT adi_adc_EnableADCSubSystem (
    ADI_ADC_HANDLE  hDevice,
    bool            bEnable
);

/* Returns whether the ADC subsytem is ready */
ADI_ADC_RESULT adi_adc_IsReady (
     ADI_ADC_HANDLE  hDevice,
     bool           *pbReady
);

/* Set the voltage reference source */
ADI_ADC_RESULT adi_adc_SetVrefSource (
    ADI_ADC_HANDLE    hDevice,
    ADI_ADC_VREF_SRC  eVrefSrc
);

/* Enable/Disable current sink */
ADI_ADC_RESULT adi_adc_SinkEnable (
    ADI_ADC_HANDLE   hDevice,
    bool             bEnable
);

/* Start the ADC Calibration */
ADI_ADC_RESULT adi_adc_StartCalibration (
    ADI_ADC_HANDLE hDevice
);

 ADI_ADC_RESULT adi_adc_IsCalibrationDone (
     ADI_ADC_HANDLE hDevice,
     bool*          pbCalibrationDone
 );


/* Set the acquisition time of ADC in ADC clock cycles */
ADI_ADC_RESULT adi_adc_SetAcquisitionTime(
    ADI_ADC_HANDLE hDevice,
    uint32_t       nAcqTimeInAClkCycles
);

/* Set the delay time of ADC in ADC cycles for multi iteration mode */
ADI_ADC_RESULT adi_adc_SetDelayTime(
    ADI_ADC_HANDLE hDevice,
    uint32_t       nDelayInAClkCycles
);

/* set the resolution of ADC. The default resolution of ADC is 12-bit and the ADC increases the resolution by oversampling */
ADI_ADC_RESULT adi_adc_SetResolution (
   ADI_ADC_HANDLE      hDevice,
   ADI_ADC_RESOLUTION  eResolution
);

/* Enable Averaging for all ADC channels */
ADI_ADC_RESULT adi_adc_EnableAveraging (
    ADI_ADC_HANDLE  hDevice,
    uint16_t        nAveragingSamples
);

/* Configure low limit for an ADC channel when it's used as a digital comparator. */
ADI_ADC_RESULT adi_adc_SetLowLimit (
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    bool             bEnable,
    uint16_t         nLowLimit
);

/* Configure high limit for an ADC channel when it's used as a digital comparator. */
ADI_ADC_RESULT adi_adc_SetHighLimit (
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    bool             bEnable,
    uint16_t         nHighLimit
);


/* Configure hysteresis for an ADC channel when it's used as a digital comparator. */
ADI_ADC_RESULT adi_adc_SetHysteresis(
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    bool             bEnable,
    uint16_t         nHysteresis
);

/* Configure number of monitor cycles for an ADC channel when it's used as a digital comparator. */
ADI_ADC_RESULT adi_adc_SetNumMonitorCycles(
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    uint32_t         nNumMonitorCycles
);

/*  Enable/Disable digital comparator for the given channel(s) */
ADI_ADC_RESULT adi_adc_EnableDigitalComparator (
    ADI_ADC_HANDLE   hDevice,
    bool             bEnableComparator
);

/* Submit buffer for sampling */
ADI_ADC_RESULT adi_adc_SubmitBuffer (
    ADI_ADC_HANDLE      hDevice,
    ADI_ADC_BUFFER*     pBuffer
);

/* Get a completed buffer from the driver */
ADI_ADC_RESULT adi_adc_GetBuffer(
    ADI_ADC_HANDLE      hDevice,
    ADI_ADC_BUFFER**    ppBuffer
);

/* Enable/Disable buffer processing */
ADI_ADC_RESULT adi_adc_Enable (
    ADI_ADC_HANDLE  hDevice,
    bool            bEnable
);

/* Check whether a completed buffer is available in the driver */
ADI_ADC_RESULT adi_adc_IsBufferAvailable(
    ADI_ADC_HANDLE      hDevice,
    bool*               pbIsBufferAvailable
);

/* Read the given channels. This will only return once the given amount of samples are collected */
ADI_ADC_RESULT adi_adc_ReadChannels (
    ADI_ADC_HANDLE   hDevice,
    uint32_t         nChannels,
    uint32_t         nNumConversionPasses,
    void*            pBuffer,
    uint32_t         nBuffLength
);

/* Get Battery Voltage */
ADI_ADC_RESULT adi_adc_GetBatteryVoltage (
    ADI_ADC_HANDLE  hDevice,
    uint32_t        nRefVoltage,
    uint32_t*       pnBatVoltage
);

/* Enable/Disable Temperature Sensor */
ADI_ADC_RESULT adi_adc_EnableTemperatureSensor (
    ADI_ADC_HANDLE hDevice,
    bool           bEnable
    );

/* Get the Temperature Value */
ADI_ADC_RESULT adi_adc_GetTemperature (
    ADI_ADC_HANDLE  hDevice,
    uint32_t        nRefVoltage,
    int32_t*        pnTemperature
    );

#ifdef __cplusplus
}
#endif

/**@}*/


#endif /* ADI_ADC_H */
