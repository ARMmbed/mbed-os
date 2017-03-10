/*!
 *****************************************************************************
 @file    adi_rtc.h
 @brief   Primary include file for Real Time Clock Services.
 @version $Revision: 29004 $
 @date    $Date: 2014-12-06 10:37:26 -0500 (Sat, 06 Dec 2014) $
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
#ifndef ADI_RTC_H__
#define ADI_RTC_H__
#include "adi_processor.h"

#include <stdint.h>
#include <adi_callback.h>
#include <adi_rtc_config.h>

/*! \addtogroup RTC_Driver RTC Driver
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/*! Amount of memory(In bytes) required by the RTC  device driver  for managing the operation.
 * This memory is completely owned by the driver till the end of the operation.
 */
#define ADI_RTC_MEMORY_SIZE    (24u)

/*! Emergency flush command to gatweay register */
#define ADI_RTC_GATEWAY_FLUSH    0xa2c5

/*! A device handle used in all API functions to identify the RTC device. */
typedef void*   ADI_RTC_HANDLE;

/*! Interrupt  bit position-1*/
#define ADI_RTC_INTERRUPT_OFFSET                16

/*! Interrupt  bit position-2*/
#define ADI_RTC_INTERRUPT_OFFSET_IO_CAPTURE     21

/*!
 * RTC  API return codes
 */
typedef enum
{
    /*!  No Error, API succeeded */
    ADI_RTC_SUCCESS,
    /*!  Generic failure */
    ADI_RTC_FAILURE,
    /*!  RTC is in failsafe mode and not reliable */
    ADI_RTC_CLOCK_FAILSAFE,
    /*!  RTC is already initialized */
    ADI_RTC_IN_USE,
    /*!  Invalid device handle passed */
    ADI_RTC_INVALID_HANDLE,
    /*!  Asking to initialize an unknown instance */
    ADI_RTC_INVALID_INSTANCE,
    /*!  Parameter is out of range          */
    ADI_RTC_INVALID_OPTION,
    /*!  Specified operation not allowed */
    ADI_RTC_OPERATION_NOT_ALLOWED,
    /*!  One of the parameters is invalid */
    ADI_RTC_INVALID_PARAM,
    /*!  Input/SensorStrobe channel is invalid for the specified operation */
    ADI_RTC_INVALID_CHANNEL
    
} ADI_RTC_RESULT;


/*!
 * RTC Interrupt Enable Bits.
 */


typedef uint32_t ADI_RTC_INT_TYPE;

#define ADI_RTC_ALARM_INT                  0x00000001u    /*!< Alarm interrupt enable bit */
#define ADI_RTC_MOD60ALM_INT               0x00000002u    /*!< modulo 60 Alarm interrupt enable */
#define ADI_RTC_ISO_DONE_INT               0x00000004u    /*!< Power isolation done interrupt enable */
#define ADI_RTC_WRITE_PENDERR_INT          0x00000008u    /*!< Write pend error interrupt enable */
#define ADI_RTC_WRITE_SYNC_INT             0x00000010u    /*!< Write sync interrupt enable */
#define ADI_RTC_WRITE_PEND_INT             0x00000020u    /*!< Write pend interrupt enable  */
#define ADI_RTC_COUNT_INT                  0x00000040u    /*!< RTC count interrupt source enable */
#define ADI_RTC_PSI_INT                    0x00000080u    /*!< Precaled Module 1 interrupt */
#define ADI_RTC_TRIM_INT                   0x00000100u    /*!< Enable for the RTC trim interrupt source */
#define ADI_RTC_COUNT_ROLLOVER_INT         0x00000200u    /*!< Enable for the RTC count roll-over interrupt source */
#define ADI_RTC_MOD60_ROLLOVER_INT         0x00000400u    /*!< Enable for the RTC modulo-60 count roll-over interrupt source */
#define ADI_RTC_SENSOR_STROBE_CH1_INT      0x00000800u    /*!< Enable interrupt for sensor strobe channel -1*/
#define ADI_RTC_SENSOR_STROBE_CH2_INT      0x00001000u    /*!< Enable interrupt for sensor strobe channel -2*/
#define ADI_RTC_SENSOR_STROBE_CH3_INT      0x00002000u    /*!< Enable interrupt for sensor strobe channel -3*/
#define ADI_RTC_SENSOR_STROBE_CH4_INT      0x00004000u    /*!< Enable interrupt for sensor strobe channel -4*/
#define ADI_RTC_INPUT_CAPTURE_CH0_INT      0x00008000u    /*!< Enable interrupt for input capture channel -0*/
#define ADI_RTC_INPUT_CAPTURE_CH2_INT      0x00010000u    /*!< Enable interrupt for input capture channel -2*/
#define ADI_RTC_INPUT_CAPTURE_CH3_INT      0x00020000u    /*!< Enable interrupt for input capture channel -3*/
#define ADI_RTC_INPUT_CAPTURE_CH4_INT      0x00040000u    /*!< Enable interrupt for input capture channel -4*/
#define ADI_RTC_LFXTL_FAILURE_INT          0x00080000u    /*!< Interrupt for LFXTL failure. LFXTL failure interrupt is mapped to RTC1 interrupt.*/
#define ADI_RTC_RTCSS4_FE_INT              0x00100000u    /*!< Enable interrupt for Sensor Strobe channel 3*/
#define ADI_RTC_RTCSS3_FE_INT              0x00200000u    /*!< Enable interrupt for Sensor Strobe channel 3*/
#define ADI_RTC_RTCSS2_FE_INT              0x00400000u    /*!< Enable interrupt for Sensor Strobe channel 2*/
#define ADI_RTC_RTCSS1_FE_INT              0x00800000u    /*!< Enable interrupt for Sensor Strobe channel 2*/
#define ADI_RTC_RTCSS4MSKEN                0x01000000u    /*!< Enable interrupt for Sensor Strobe channel 4 Mask */
#define ADI_RTC_RTCSS3MSKEN                0x02000000u    /*!< Enable interrupt for Sensor Strobe channel 3 Mask */
#define ADI_RTC_RTCSS2MSKEN                0x04000000u    /*!< Enable interrupt for Sensor Strobe channel 2 Mask */
#define ADI_RTC_RTCSS1MSKEN                0x08000000u    /*!< Enable interrupt for Sensor Strobe channel 1 Mask */
#define ADI_RTC_CR5OCS_SS3SMPMTCHIRQEN     0x10000000u    /*!< Sample activity Interrupt enable for RTC Sensor Strobe Channel 3 */
#define ADI_RTC_CR5OCS_SS2SMPMTCHIRQEN     0x20000000u    /*!< Sample activity Interrupt enable for RTC Sensor Strobe Channel 2 */
#define ADI_RTC_CR5OCS_SS1SMPMTCHIRQEN     0x40000000u    /*!< Sample activity Interrupt enable for RTC Sensor Strobe Channel 1. */


#define ADI_RTC_NUM_INTERRUPTS             31             /*!< Number of RTC interrupts. */


/*!
 * RTC Posted Write Status Bits.
 */
typedef enum
{
    /*!  Posted write control register-0 status bit */
    ADI_RTC_WRITE_STATUS_CONTROL0      = 1 << BITP_RTC_SR0_WSYNCCR0,
    /*!  Posted write status0 register status bit */
    ADI_RTC_WRITE_STATUS_STATUS0       = 1 << BITP_RTC_SR0_WSYNCSR0,
    /*!  Posted write count0 register status bit */
    ADI_RTC_WRITE_STATUS_COUNT0        = 1 << BITP_RTC_SR0_WSYNCCNT0,
    /*!  Posted write count1 register status bit */
    ADI_RTC_WRITE_STATUS_COUNT1        = 1 << BITP_RTC_SR0_WSYNCCNT1,
    /*!  Posted write alarm0 register status bit */
    ADI_RTC_WRITE_STATUS_ALARM0        = 1 << BITP_RTC_SR0_WSYNCALM0,
    /*!  Posted write alarm1 register status bit */
    ADI_RTC_WRITE_STATUS_ALARM1        = 1 << BITP_RTC_SR0_WSYNCALM1,
    /*!  Posted write trim register status bit */
    ADI_RTC_WRITE_STATUS_TRIM          = 1 << BITP_RTC_SR0_WSYNCTRM
} ADI_RTC_WRITE_STATUS;


/*!
 * RTC Trim intervals.
 */
typedef enum
{
    /*!  Trim interval is 2^2 seconds */
    ADI_RTC_TRIM_INTERVAL_2           = (2 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^3 seconds */
    ADI_RTC_TRIM_INTERVAL_3           = (3 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^4 seconds */
    ADI_RTC_TRIM_INTERVAL_4           = (4 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^5 seconds */
    ADI_RTC_TRIM_INTERVAL_5           = (5 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
      /*!  Trim interval is 2^6 seconds */
    ADI_RTC_TRIM_INTERVAL_6           = (6 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^7 seconds */
    ADI_RTC_TRIM_INTERVAL_7           = (7 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^8 seconds */
    ADI_RTC_TRIM_INTERVAL_8           = (8 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^9 seconds */
    ADI_RTC_TRIM_INTERVAL_9           = (9 << BITP_RTC_TRM_IVL2EXPMIN    | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^10 seconds */
    ADI_RTC_TRIM_INTERVAL_10           = (10 << BITP_RTC_TRM_IVL2EXPMIN  | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^11 seconds */
    ADI_RTC_TRIM_INTERVAL_11           = (11 << BITP_RTC_TRM_IVL2EXPMIN  | 0x1 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^12 seconds */
    ADI_RTC_TRIM_INTERVAL_12           = (12 << BITP_RTC_TRM_IVL2EXPMIN  | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^13 seconds */
    ADI_RTC_TRIM_INTERVAL_13           = (13 << BITP_RTC_TRM_IVL2EXPMIN  | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^14 seconds */
    ADI_RTC_TRIM_INTERVAL_14           = (14 << BITP_RTC_TRM_IVL2EXPMIN  | 0x0 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^15 seconds */
    ADI_RTC_TRIM_INTERVAL_15           = (14 << BITP_RTC_TRM_IVL2EXPMIN  | 0x1 << BITP_RTC_TRM_IVL),
    /*!  Trim interval is 2^16 seconds */
    ADI_RTC_TRIM_INTERVAL_16           = (14 << BITP_RTC_TRM_IVL2EXPMIN  | 0x2 << BITP_RTC_TRM_IVL ),
    /*!  Trim interval is 2^17 seconds */
    ADI_RTC_TRIM_INTERVAL_17           = (14 << BITP_RTC_TRM_IVL2EXPMIN  | 0x3 << BITP_RTC_TRM_IVL)

} ADI_RTC_TRIM_INTERVAL;

/*!
 * RTC input capture  channels.
 */
typedef enum
{
    /*!  Input capture channel-0  */
    ADI_RTC_INPUT_CHANNEL_0           = 1 << BITP_RTC_CR2IC_IC0EN,
    /*!  Input capture channel-2  */
    ADI_RTC_INPUT_CHANNEL_2           = 1 << BITP_RTC_CR2IC_IC2EN,
    /*!  Input capture channel-3  */
    ADI_RTC_INPUT_CHANNEL_3           = 1 << BITP_RTC_CR2IC_IC3EN,
    /*!  Input capture channel-4  */
    ADI_RTC_INPUT_CHANNEL_4           = 1 << BITP_RTC_CR2IC_IC4EN

}ADI_RTC_INPUT_CHANNEL;

/*!
 * RTC Sensor Strobe  channels.
 */
typedef enum
{
    /*!  Sensor Strobe channel-1  */
    ADI_RTC_SS_CHANNEL_1               = 1 << BITP_RTC_CR3SS_SS1EN,
    /*!  Sensor Strobe channel-2  */
    ADI_RTC_SS_CHANNEL_2               = 1 << BITP_RTC_CR3SS_SS2EN,
    /*!  Sensor Strobe channel-3  */
    ADI_RTC_SS_CHANNEL_3               = 1 << BITP_RTC_CR3SS_SS3EN,
    /*!  Sensor Strobe channel-4  */
    ADI_RTC_SS_CHANNEL_4               = 1 << BITP_RTC_CR3SS_SS4EN,

}ADI_RTC_SS_CHANNEL;

/*!
 * RTC Trim polarity.
 */
typedef enum
{
    /*!  Trim value is added every trim interval */
    ADI_RTC_TRIM_ADD                   = (1 << BITP_RTC_TRM_ADD),
    /*!  Trim value is subtracted every trim interval */
    ADI_RTC_TRIM_SUB                   = (0 << BITP_RTC_TRM_ADD),
} ADI_RTC_TRIM_POLARITY;

/*!
 * RTC Trim values.
 */
typedef enum
{
    /*!  Trim value is +/- 0 */
    ADI_RTC_TRIM_0                     = (0 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 1 */
    ADI_RTC_TRIM_1                     = (1 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 2 */
    ADI_RTC_TRIM_2                     = (2 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 3 */
    ADI_RTC_TRIM_3                     = (3 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 4 */
    ADI_RTC_TRIM_4                     = (4 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 5 */
    ADI_RTC_TRIM_5                     = (5 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 6 */
    ADI_RTC_TRIM_6                     = (6 << BITP_RTC_TRM_VALUE),
    /*!  Trim value is +/- 7 */
    ADI_RTC_TRIM_7                     = (7 << BITP_RTC_TRM_VALUE)
} ADI_RTC_TRIM_VALUE;

/*!
 * RTC control register set.
 */
typedef enum
{
  /*! Specify the RTC-Control register-0 */
  ADI_RTC_CONTROL_REGISTER_0,
  /*! Specify the RTC-Control register-1 */
  ADI_RTC_CONTROL_REGISTER_1
} ADI_RTC_CONTROL_REGISTER;

/*================ E X T E R N A L S ==================*/

/*
 */

/*************************************/
/*    RTC API                       */
/*************************************/
ADI_RTC_RESULT adi_rtc_Open(
               uint32_t         DeviceNumber,
               void            *pDeviceMemory,
               uint32_t         MemorySize,
               ADI_RTC_HANDLE  *phDevice
               );

ADI_RTC_RESULT adi_rtc_Close(
               ADI_RTC_HANDLE const hDevice
               );

/*************************************/
/*    Enable APIs for RTC Device     */
/*************************************/
               
ADI_RTC_RESULT adi_rtc_EnableAlarm(
               ADI_RTC_HANDLE const hDevice,
               bool bEnable
               );

ADI_RTC_RESULT adi_rtc_EnableMod60Alarm(
               ADI_RTC_HANDLE const hDevice,
               bool bEnable
               );

ADI_RTC_RESULT adi_rtc_Enable(
               ADI_RTC_HANDLE const hDevice,
               bool bEnable
               );

ADI_RTC_RESULT adi_rtc_EnableInterrupts(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INT_TYPE Interrupts,
               bool bEnable
               );

ADI_RTC_RESULT adi_rtc_EnableTrim(
               ADI_RTC_HANDLE const hDevice,
               bool bEnable
               );
               
ADI_RTC_RESULT adi_rtc_EnableAutoReload(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_SS_CHANNEL   eSSChannel,
               bool bEnable);

ADI_RTC_RESULT adi_rtc_EnableSensorStrobeOutput (
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_SS_CHANNEL   eSSChannel, 
               bool bEnable);

ADI_RTC_RESULT adi_rtc_EnableInputCapture (
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INPUT_CHANNEL eInpChannel,
               bool bEnable);

ADI_RTC_RESULT adi_rtc_EnableSensorStrobeChannelMask(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_SS_CHANNEL   eSSChannel, 
               bool bEnable);
               
ADI_RTC_RESULT adi_rtc_EnableOverwriteSnapshot (
               ADI_RTC_HANDLE const hDevice,
               bool bEnable);
               
/*************************************/
/*    Set APIs for RTC Device     */
/*************************************/


ADI_RTC_RESULT adi_rtc_SetMod60AlarmPeriod(
               ADI_RTC_HANDLE const hDevice,
               uint8_t nPeriod
               );

ADI_RTC_RESULT adi_rtc_SetAlarm(
               ADI_RTC_HANDLE const hDevice,
               uint32_t nAlarm
               );

ADI_RTC_RESULT adi_rtc_SetAlarmEx(
               ADI_RTC_HANDLE const hDevice,
               float  fAlarm
               );


ADI_RTC_RESULT adi_rtc_SetControlRegister(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_CONTROL_REGISTER eRegister,
               uint32_t Control
               );

ADI_RTC_RESULT adi_rtc_SetCount(
               ADI_RTC_HANDLE const hDevice,
               uint32_t nCount
               );

ADI_RTC_RESULT adi_rtc_SetGateway(
               ADI_RTC_HANDLE const hDevice,
               uint16_t Command
               );


ADI_RTC_RESULT adi_rtc_SetPreScale(
               ADI_RTC_HANDLE const hDevice,
               uint8_t nPreScale
               );

ADI_RTC_RESULT adi_rtc_SetTrim(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_TRIM_INTERVAL eInterval,
               ADI_RTC_TRIM_VALUE  eTrimValue,
               ADI_RTC_TRIM_POLARITY eOperation
               );
               
ADI_RTC_RESULT adi_rtc_SetSensorStrobeChannelMask(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_SS_CHANNEL   eSSChannel, 
               uint8_t nMask);

ADI_RTC_RESULT adi_rtc_SetAutoReloadValue(
                ADI_RTC_HANDLE const hDevice,
                ADI_RTC_SS_CHANNEL   eSSChannel, 
                uint16_t nValue);
                
ADI_RTC_RESULT adi_rtc_SetInputCapturePolarity (
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INPUT_CHANNEL eInpChannel,
               bool bEnable);
               
ADI_RTC_RESULT adi_rtc_SetSensorStrobeValue(
                ADI_RTC_HANDLE const hDevice,
                ADI_RTC_SS_CHANNEL   eSSChannel, 
                uint16_t nValue);

/*************************************/
/*    Get APIs for RTC Device     */
/*************************************/

ADI_RTC_RESULT adi_rtc_GetAlarm (
               ADI_RTC_HANDLE hDevice,
               uint32_t *pAlarm
               );

ADI_RTC_RESULT adi_rtc_GetAlarmEx (
               ADI_RTC_HANDLE hDevice,
               float *pAlarm);

ADI_RTC_RESULT adi_rtc_GetControl (
               ADI_RTC_HANDLE hDevice,
               ADI_RTC_CONTROL_REGISTER eRegister ,
               uint32_t *pControl);
               
ADI_RTC_RESULT adi_rtc_GetTrim(
               ADI_RTC_HANDLE hDevice,
               ADI_RTC_TRIM_VALUE *peTrim
               );
               
ADI_RTC_RESULT adi_rtc_GetCount(
               ADI_RTC_HANDLE const hDevice,
               uint32_t *pCount
               );

ADI_RTC_RESULT adi_rtc_GetCountEx(
               ADI_RTC_HANDLE const hDevice,
               float *pfCount
               );

ADI_RTC_RESULT adi_rtc_GetSnapShot(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INPUT_CHANNEL eChannel, 
               uint32_t *pValue,
               uint16_t *pFraction);

ADI_RTC_RESULT adi_rtc_GetInputCaptureValue(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INPUT_CHANNEL eChannel,
               uint16_t *pValue);

ADI_RTC_RESULT adi_rtc_GetWritePendStatus(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_WRITE_STATUS *pPendBits
               );

ADI_RTC_RESULT adi_rtc_GetWriteSyncStatus(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_WRITE_STATUS *pSyncBits
               );
               
ADI_RTC_RESULT adi_rtc_GetSensorStrobeValue(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_SS_CHANNEL   eSSChannel, 
               uint16_t *pValue);
               
ADI_RTC_RESULT adi_rtc_GetCountRegs(
               ADI_RTC_HANDLE const hDevice, 
               uint32_t *pnCount,
               uint32_t *pfCount);        
/************************************************/
/*    RTC APIs for managing interrupt/sync     */
/***********************************************/
               
ADI_RTC_RESULT adi_rtc_SynchronizeAllWrites(
               ADI_RTC_HANDLE const hDevice
               );

ADI_RTC_RESULT adi_rtc_RegisterCallback(
               ADI_RTC_HANDLE const  hDevice,
               ADI_CALLBACK   const   pfCallback,
               void        *const     pCBparam
               );

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* ADI_RTC_H__ */


