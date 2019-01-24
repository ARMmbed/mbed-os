/**
  ******************************************************************************
  * @file    clock.h
  * @author  AMS - VMA RF Application team
  * @version V1.0.0
  * @date    21-Sept-2015
  * @brief   Header file for clock APIs based on SySTick
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
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "BlueNRG_x_device.h"

/** 
 * @brief Number of clocks in one seconds.
 */
extern const uint32_t CLOCK_SECOND;

typedef uint32_t tClockTime;

/** 
 * @brief This function initializes the clock library based on SysTick.
 * 
 * @param None
 *
 * @retval None
 */
void Clock_Init(void);

/** 
 * @brief This function returns the current system clock time.
 * 
 * @param None
 *
 * @retval It returns current clock time, measured in system ticks.
 */
tClockTime Clock_Time(void);


/** 
 * @brief This function waits for a given number of milliseconds.
 * 
 * @param i delay in milliseconds
 *
 * @retval None
 */
void Clock_Wait(uint32_t i);

/**
 * @brief This function is called on SysTick_Handler()
 *
 */
void SysCount_Handler(void);

#endif /* __CLOCK_H__ */

