/**
 ******************************************************************************
 * @file    rtc.h
 * @brief    (API) Public header of RTC driver
 * @internal
 * @author   ON Semiconductor
 * $Rev: 3485 $
 * $Date: 2015-07-14 15:20:11 +0530 (Tue, 14 Jul 2015) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup rtc
 */

#ifndef RTC_H_
#define RTC_H_

#include "rtc_map.h"
#include "clock.h"
#include "memory_map.h"

#define RTC_CLOCK_HZ                            32768
#define RTC_SEC_TO_US                           1000000
#define RTC_SUB_SEC_MASK                        0x7FFF
#define RTC_SEC_MASK                            0xFFFFFFFF
#define RTC_SEC_INT_STATUS_MASK                 0x2

#define RTC_SUBSEC_INTERRUPT_BIT_VAL            0x1
#define RTC_SEC_INTERRUPT_BIT_VAL               0x2
#define RTC_ALL_INTERRUPT_BIT_VAL               0x3

#define RTC_INT_CLR_SUB_SEC_BIT_POS             0
#define RTC_INT_CLR_SEC_BIT_POS                 1

#define RTC_CONTROL_SUBSEC_CNT_START_BIT_POS    0
#define RTC_CONTROL_SEC_CNT_START_BIT_POS       1
#define RTC_CONTROL_SUBSEC_CNT_INT_BIT_POS      2
#define RTC_CONTROL_SEC_CNT_INT_BIT_POS         3

#define RTC_STATUS_SUB_SEC_ALARM_WRT_BIT_POS    6
#define RTC_STATUS_SEC_ALARM_WRT_BIT_POS        7
#define RTC_STATUS_CONTROL_WRT_BIT_POS          8
#define RTC_STATUS_SUB_SEC_INT_CLR_WRT_BIT_POS  9
#define RTC_STATUS_SEC_INT_CLR_WRT_BIT_POS      10

#define SUB_SEC_MASK    0x7FFF


/* FUnction pointer for call back */
typedef void (* fRtcCallBack)(void);

/**
 * @details
 * Initializes RTC, enable RTC & register call back function
 *
 * @param RtcCallBack Function pointer for RTC call back funtion
 * @return None
 */
void fRtcInit(void);

/**
 * @details
 * Disable RTC
 *
 * @param None
 * @return None
 */
void fRtcFree(void);

/**
 * @details
 * Set up alram for RTC interrupt in micro second
 * Pre-requisite: Both second & sub second interrupts should be cleared.
 * @param TimeStamp in micro seconds
 * @return None
 */
void fRtcSetInterrupt(uint32_t timestamp);

/**
 * @details
 * Disable interrupt
 *
 * @param None
 * @return None
 */
void fRtcDisableInterrupt(void);

/**
 * @details
 * Enable interrupt
 *
 * @param None
 * @return None
 */
void fRtcEnableInterrupt(void);

/**
 * @details
 * Clear interrupt
 *
 * @param None
 * @return None
 */
void fRtcClearInterrupt(void);

/**
 * @details
 * Returns RTC time in micro seconds
 *
 * @param None
 * @return RTC Time in micro second
 */
uint64_t fRtcRead(void);

/**
 * @details
 * Set RTC time in micro seconds
 *
 * @param RtcTime in micro Seconds
 * @return None
 */
void fRtcWrite(uint64_t RtcTimeus);

/**
 * @details
 * RTC interrupt handler
 *
 * @param None
 * @return None
 */
void fRtcHandler(void);

/**
 * @details
 * Is RTC enabled?
 *
 * @param None
 * @return RTC status
 */
boolean fIsRtcEnabled(void);
#endif /* RTC_H_ */
