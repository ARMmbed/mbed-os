/**
  ******************************************************************************
  * @file    gp_timer.h
  * @author  AMS - RF Application team
  * @version V1.1.0
  * @date    26-March-2018
  * @brief   Header file for general purpose timer library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
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
