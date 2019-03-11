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

/* Includes ------------------------------------------------------------------*/

#include "clock.h"
#include "gp_timer.h"

/*---------------------------------------------------------------------------*/

void Timer_Set(struct timer *t, tClockTime interval)
{
  t->interval = interval;
  t->start = Clock_Time();
}

void Timer_Reset(struct timer *t)
{
  t->start += t->interval;
}

void Timer_Restart(struct timer *t)
{
  t->start = Clock_Time();
}

uint8_t Timer_Expired(struct timer *t)
{
  tClockTime diff = (Clock_Time() - t->start) + 1;
  return (t->interval < diff);

}

tClockTime Timer_Remaining(struct timer *t)
{
  return (t->start + t->interval - Clock_Time());
}



