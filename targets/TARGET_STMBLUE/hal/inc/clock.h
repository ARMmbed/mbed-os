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

