/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

#ifndef __GP_TIMER_H__
#define __GP_TIMER_H__

#include "clock.h"

/**
 * @brief Timer structure. Use Timer_Set() to set the timer.
 *
 */
struct timer {
  tClockTime start;
  tClockTime interval;
};

/** 
 * @brief This function sets a timer for a specific time.
 * The function Timer_Expired() returns true if 
 * the timer has expired.
 * 
 * @param[in] t         Pointer to timer 
 * @param[in] interval  The interval before the timer expires.
 *
 * @retval None
 */
void Timer_Set(struct timer *t, tClockTime interval);

/** 
 * @brief This function resets the same interval that was
 * given to the Timer_Set() function. The starting point of the interval is 
 * the last timer value when timer expired. Using this function
 * makes the timer being stable over time.
 * 
 * @param[in] t Pointer to timer 
 *
 * @retval None
 */
void Timer_Reset(struct timer *t);

/** 
 * @brief This function resets the same interval that was
 * given to the Timer_Set() function. The starting point of the interval is 
 * the current time.  For a stable timer over time, it is recommended to use 
 * the Timer_Reset() function. 
 * 
 * @param[in] t Pointer to timer 
 *
 * @retval None
 */
void Timer_Restart(struct timer *t);

/** 
 * @brief This function verifies if a timer has expired. 
 * 
 * @param[in] t Pointer to timer
 *
 * @retval 1 if the timer has expired, 0 if not expired.
 */
uint8_t Timer_Expired(struct timer *t);

/** 
 * @brief This function returns the remaining time before the timer expires.
 * 
 * @param[in] t Pointer to timer 
 *
 * @retval The remaining time. 
 */
tClockTime Timer_Remaining(struct timer *t);

#endif /* __GP_TIMER_H__ */
