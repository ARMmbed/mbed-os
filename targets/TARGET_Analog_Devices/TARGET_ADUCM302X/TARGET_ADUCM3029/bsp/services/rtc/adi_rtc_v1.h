/*!
 *****************************************************************************
 @file:    adi_rtc_v1.h
 @brief:   Include file for Real-Time Clock Device driver API Definitions
 @version: $Revision$
 @date:    $Date$
 -----------------------------------------------------------------------------
Copyright (c) 2010-2017 Analog Devices, Inc.

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

/*! \addtogroup RTC_Service RTC Service
 *  @{
 */

#ifndef __ADI_RTC_V1_H__
#define __ADI_RTC_V1_H__

#include <adi_types.h>
#include <services/int/adi_int.h>

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

/*
   Define the ADI_RTC_USE_IAR_SYSTEM_TIME_HOOKS macro in the C preprocessor to use
   IAR time.h, time_t type and associated C-Runtime time interfaces, which rely on
   underlying RTC run-time library overrides provided by this implementation.
*/
#if defined (__ICCARM__)
#define ADI_RTC_USE_IAR_SYSTEM_TIME_HOOKS
#endif

/*! Interrupt  bit position-1*/
#define ADI_RTC_INTERRUPT_OFFSET                16

/*! Interrupt  bit position-2*/
#define ADI_RTC_INTERRUPT_OFFSET_IO_CAPTURE     21

/*!
 * RTC  API return codes
 */
typedef enum
{
    /*!  No Error, API suceeded */
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
    /*!  Specfied operation not allowed */
    ADI_RTC_OPERATION_NOT_ALLOWED,
    /*!  One of the parameters is invalid */
    ADI_RTC_INVALID_PARAM,
    /*!  Input/output channel is invalide for the specified operation */
    ADI_RTC_INVALID_CHANNEL
    
} ADI_RTC_RESULT;


/*!
 * RTC Interrupt Enable Bits.
 */
typedef enum
{
    /*!  Alarm interrupt enable bit */
    ADI_RTC_ALARM_INT              =  1<< BITP_RTC_CR0_ALMINTEN,
   /*!  modulo 60 Alarm interrupt ebabke */
    ADI_RTC_MOD60ALM_INT           =  1 << BITP_RTC_CR0_MOD60ALMINTEN,
    /*!  Power isolation done interrupt enable */
    ADI_RTC_ISO_DONE_INT           =  1 << BITP_RTC_CR0_ISOINTEN,
    /*!  Write pend error interrupt enable */
    ADI_RTC_WRITE_PENDERR_INT      =  1 << BITP_RTC_CR0_WPNDERRINTEN,
    /*!  Write sync interrupt enable */
    ADI_RTC_WRITE_SYNC_INT         =  1 << BITP_RTC_CR0_WSYNCINTEN,
    /*!  Write pend interrupt enable  */
    ADI_RTC_WRITE_PEND_INT         =  1 << BITP_RTC_CR0_WPNDINTEN,
    /*!  RTC count interrupt source enable */
    ADI_RTC_COUNT_INT              =  1 << (BITP_RTC_CR1_CNTINTEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!  Enable for the prescaled, modulo-1 interrupt source */
    ADI_RTC_MOD_1_INT              =  1 << (BITP_RTC_CR1_PSINTEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!  Enable for the RTC trim interrupt source */
    ADI_RTC_TRIM_INT                = 1 << (BITP_RTC_CR1_TRMINTEN + ADI_RTC_INTERRUPT_OFFSET),
    /*! Enable for the RTC count roll-over interrupt source */
    ADI_RTC_COUNT_ROLLOVER_INT      = 1 << (BITP_RTC_CR1_CNTROLLINTEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Enable for the RTC modulo-60 count roll-over interrupt source */
    ADI_RTC_MOD60_ROLLOVER_INT      = 1 << (BITP_RTC_CR1_CNTMOD60ROLLINTEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Enable interrupt for output compare channel -1*/
    ADI_RTC_OUTPUT_COMPARE_CH1_INT      = 1 << (BITP_RTC_CR3SS_SS1IRQEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Enable interrupt for input capture channel -0*/
    ADI_RTC_INPUT_CAPTURE_CH0_INT      = 1 << (BITP_RTC_CR2IC_IC0IRQEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Enable interrupt for input capture channel -2*/
    ADI_RTC_INPUT_CAPTURE_CH2_INT      = 1 << (BITP_RTC_CR2IC_IC2IRQEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Enable interrupt for input capture channel -3*/
    ADI_RTC_INPUT_CAPTURE_CH3_INT      = 1 << (BITP_RTC_CR2IC_IC3IRQEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Enable interrupt for input capture channel -4*/
    ADI_RTC_INPUT_CAPTURE_CH4_INT      = 1 << (BITP_RTC_CR2IC_IC4IRQEN + ADI_RTC_INTERRUPT_OFFSET),
    /*!Interrupt for LFXTL failure. LFXTL failure interrupt is mapped to RTC1 interrupt.*/
    ADI_LFXTL_FAILURE_INT              = (int32_t)(1u << BITP_CLKG_OSC_CTL_LFXTAL_MON_FAIL_STAT)
      
} ADI_RTC_INT_TYPE;

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
 * RTC output compare  channels.
 */
typedef enum
{
    /*!  output compare channel-1  */
    ADI_RTC_OUTPUT_CHANNEL_1           = 1 << BITP_RTC_CR3SS_SS1EN,

}ADI_RTC_OUTPUT_CHANNEL;

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
               bool_t bEnable
               );

ADI_RTC_RESULT adi_rtc_EnableMod60Alaram(
               ADI_RTC_HANDLE const hDevice,
               bool_t bEnable
               );

ADI_RTC_RESULT adi_rtc_Enable(
               ADI_RTC_HANDLE const hDevice,
               bool_t bEnable
               );

ADI_RTC_RESULT adi_rtc_EnableInterrupts(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INT_TYPE Interrupts,
               bool_t bEnable
               );

ADI_RTC_RESULT adi_rtc_EnableTrim(
               ADI_RTC_HANDLE const hDevice,
               bool_t bEnable
               );
               
ADI_RTC_RESULT adi_rtc_EnableAutoReload(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_OUTPUT_CHANNEL eOutChannel,
               bool_t bEnable);

ADI_RTC_RESULT adi_rtc_EnableOutputCompare   (
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_OUTPUT_CHANNEL eOutChannel, 
               bool_t bEnable);

ADI_RTC_RESULT adi_rtc_EnableInputCapture (
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INPUT_CHANNEL eInpChannel,
               bool_t bEnable);

ADI_RTC_RESULT adi_rtc_EnableOutputChannelMask(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_OUTPUT_CHANNEL eOutChannel, 
               bool_t bEnable);
               
ADI_RTC_RESULT adi_rtc_EnableOverwriteSnapshot (
               ADI_RTC_HANDLE const hDevice,
               bool_t bEnable);
               
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
               float32_t  fAlarm
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
               
ADI_RTC_RESULT adi_rtc_SetOutputChannelMask(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_OUTPUT_CHANNEL eOutChannel, 
               uint8_t nMask);

ADI_RTC_RESULT adi_rtc_SetAutoReloadValue(
                ADI_RTC_HANDLE const hDevice,
                ADI_RTC_OUTPUT_CHANNEL eOutChannel, 
                uint16_t nValue);
                
ADI_RTC_RESULT adi_rtc_SetInputCapturePolarity (
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INPUT_CHANNEL eInpChannel,
               bool_t bEnable);
               
ADI_RTC_RESULT adi_rtc_SetOutputCompareValue(
                ADI_RTC_HANDLE const hDevice,
                ADI_RTC_OUTPUT_CHANNEL eOutChannel, 
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
               float32_t *pAlarm);

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
               float32_t *pfCount
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

ADI_RTC_RESULT adi_rtc_GetInterruptStatus(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INT_TYPE *pStatus
               );

ADI_RTC_RESULT adi_rtc_GetWritePendStatus(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_WRITE_STATUS *pPendBits
               );

ADI_RTC_RESULT adi_rtc_GetWriteSyncStatus(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_WRITE_STATUS *pSyncBits
               );
               
ADI_RTC_RESULT adi_rtc_GetOutputCompareValue(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_OUTPUT_CHANNEL eChannel, 
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

ADI_RTC_RESULT adi_rtc_ClearInterruptStatus(
               ADI_RTC_HANDLE const hDevice,
               ADI_RTC_INT_TYPE Status
               );

ADI_RTC_RESULT adi_rtc_RegisterCallback(
               ADI_RTC_HANDLE const  hDevice,
               ADI_CALLBACK   const   pfCallback,
               void        *const     pCBparam
               );

#ifdef __cplusplus
}
#endif

#endif /* include guard */

/*
** EOF
*/

/*! @} */
