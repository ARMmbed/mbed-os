/**
 * @file    rtc.h
 * @brief   Real Time Clock (RTC) functions and prototypes.
 */

/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _RTC_H_
#define _RTC_H_

/* **** Includes **** */
#include <stdint.h>
#include "mxc_device.h"
#include "rtc_regs.h"
#include "mxc_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup rtc Real Time Clock (RTC)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */
/**
 * @brief   Bitmasks for each of the RTC's Frequency.
 */
typedef enum {
    MXC_RTC_F_1HZ   = MXC_S_RTC_CTRL_SQW_SEL_FREQ1HZ,   ///< 1Hz (Compensated)   
    MXC_RTC_F_512HZ = MXC_S_RTC_CTRL_SQW_SEL_FREQ512HZ, ///< 512Hz (Compensated) 
    MXC_RTC_F_4KHZ  = MXC_S_RTC_CTRL_SQW_SEL_FREQ4KHZ,  ///< 4Khz                
    MXC_RTC_F_32KHZ = 32,                               ///< 32Khz               
} mxc_rtc_freq_sel_t;

/**
 * @brief   Bitmasks for each of the RTC's interrupt enables.
 */
typedef enum {
    MXC_RTC_INT_EN_LONG  = MXC_F_RTC_CTRL_TOD_ALARM_IE,     ///< Long-interval alarm interrupt enable 
    MXC_RTC_INT_EN_SHORT = MXC_F_RTC_CTRL_SSEC_ALARM_IE,    ///< Short-interval alarm interrupt enable 
    MXC_RTC_INT_EN_READY = MXC_F_RTC_CTRL_RDY_IE,           ///< Timer ready interrupt enable 
} mxc_rtc_int_en_t;

/**
 * @brief     Bitmasks for each of the RTC's interrupt flags.
 */
typedef enum {
    MXC_RTC_INT_FL_LONG  = MXC_F_RTC_CTRL_TOD_ALARM,        ///< Long-interval alarm interrupt flag 
    MXC_RTC_INT_FL_SHORT = MXC_F_RTC_CTRL_SSEC_ALARM,       ///< Short-interval alarm interrupt flag 
    MXC_RTC_INT_FL_READY = MXC_F_RTC_CTRL_RDY,              ///< Timer ready interrupt flag 
} mxc_rtc_int_fl_t;

/**
 * @brief     Set Time-of-Day alarm value and enable Interrupt
 * @param     ras    20-bit value 0-0xFFFFF
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_SetTimeofdayAlarm (uint32_t ras);

/**
 * @brief     Set Sub-Second alarm value and enable interrupt,
 * @brief     this is to be called after the init_rtc() function
 * @param     rssa   32-bit value 0-0xFFFFFFFF
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_SetSubsecondAlarm (uint32_t rssa);

/**
 * @brief     Start the Real Time Clock (Blocking function)
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_Start (void);
/**
 * @brief     Stop the Real Time Clock (Blocking function)
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_Stop (void);

/**
 * @brief     Initialize the sec and ssec registers and enable RTC (Blocking function)
 * @param     sec    set the RTC Sec counter (32-bit)
 * @param     ssec   set the RTC Sub-second counter (8-bit)
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_Init (uint32_t sec, uint8_t ssec);

/**
 * @brief     Allow generation of Square Wave on the SQW pin (Blocking function)
 * @param     fq     Frequency output selection
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_SquareWaveStart (mxc_rtc_freq_sel_t fq);
                             
/**
 * @brief     Stop the generation of square wave (Blocking function)
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_SquareWaveStop (void);

/**
 * @brief     Set Trim register value (Blocking function)
 * @param     trm    set the RTC Trim (8-bit, +/- 127)
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_Trim (int8_t trm);

/**
 * @brief     Enable Interurpts (Blocking function)
 * @param     mask   The bitwise OR of interrupts to enable.
 *                   See #mxc_rtc_int_en_t for available choices.
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_EnableInt (uint32_t mask);

/**
 * @brief     Disable Interurpts (Blocking function)
 * @param     mask   The mask of interrupts to disable.
 *                   See #mxc_rtc_int_en_t for available choices.
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_DisableInt (uint32_t mask);

/**
 * @brief     Gets interrupt flags.
 * @retval    The bitwise OR of any interrupts flags that are
 *            currently set. See \ref mxc_rtc_int_fl_t for the list
 *            of possible flags.
 */
int MXC_RTC_GetFlags (void);

/**
 * @brief     Clear interrupt flags.
 * @param     flags The bitwise OR of the interrupts flags to cleear.
 *            See #mxc_rtc_int_fl_t for the list of possible flags.
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_ClearFlags (int flags);

/**
 * @brief     Get SubSecond
 * @retval    Returns subsecond value or E_BUSY, see /ref MXC_ERROR_CODES
 */
int MXC_RTC_GetSubSecond (void);

/**
 * @brief     Get Second
 * @retval    returns second value or E_BUSY, see /ref MXC_ERROR_CODES
 */
int MXC_RTC_GetSecond (void);

/**
 * @brief     Get the time using nuclear fusion. Or atomically. Something like that.
 * @param     sec pointer to store seconds value
 * @param     subsec pointer to store subseconds value
 * @retval    returns Success or Fail, see \ref MXC_Error_Codes
 */
int MXC_RTC_GetTime (uint32_t* sec, uint32_t* subsec);

/**
 * @brief     Get RTC busy flag.
 * @retval    returns Success or E_BUSY, see /ref MXC_ERROR_CODES
 */
int MXC_RTC_GetBusyFlag(void);

/**@} end of group rtc */
#ifdef __cplusplus
}
#endif

#endif /* _RTC_H_ */
