/* mbed Microcontroller Library 
 *******************************************************************************
 * Copyright (c) 2015 WIZnet Co.,Ltd. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of ARM Limited nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "system_W7500x.h"
#include "W7500x_dualtimer.h"
#include "W7500x_pwm.h"

#define TIMER_0         DUALTIMER0_0
#define TIMER_1         PWM_CH1
#define TIMER_IRQn      DUALTIMER0_IRQn

static PWM_TimerModeInitTypeDef TimerInitType;
static DUALTIMER_InitTypDef TimerHandler;

static int us_ticker_inited = 0;


#ifdef __cplusplus
extern "C"{
#endif

void DUALTIMER0_Handler(void)
{
   if(DUALTIMER_GetIntStatus(DUALTIMER0_0))
    {
        DUALTIMER_IntClear(DUALTIMER0_0);
        us_ticker_irq_handler();
    }
}

#ifdef __cplusplus
}
#endif

void us_ticker_init(void)
{
    if (us_ticker_inited) return;
    us_ticker_inited = 1;

    SystemCoreClockUpdate();
    TimerInitType.PWM_CHn_PR = (GetSystemClock() / 1000000) -1;
    TimerInitType.PWM_CHn_LR = 0xFFFFFFFF;
    TimerInitType.PWM_CHn_PDMR = 1;

    PWM_TimerModeInit(TIMER_1, &TimerInitType);
    PWM_CHn_Start(TIMER_1);
}


uint32_t us_ticker_read()
{
    if (!us_ticker_inited) us_ticker_init();
    return (TIMER_1->TCR);
}


void us_ticker_set_interrupt(timestamp_t timestamp)
{
    int32_t dev = 0;
    
    if (!us_ticker_inited)
    {
        us_ticker_init();
    }
    
    dev = (int32_t)(timestamp - us_ticker_read());
    dev = dev * ((GetSystemClock() / 1000000) / 16);     

    if(dev <= 0)
    {
        us_ticker_irq_handler();
        return;
    }
    
    DUALTIMER_ClockEnable(TIMER_0);
    DUALTIMER_Stop(TIMER_0);
    
    TimerHandler.TimerControl_Mode       = DUALTIMER_TimerControl_Periodic;
    TimerHandler.TimerControl_OneShot    = DUALTIMER_TimerControl_OneShot;
    TimerHandler.TimerControl_Pre        = DUALTIMER_TimerControl_Pre_16;
    TimerHandler.TimerControl_Size       = DUALTIMER_TimerControl_Size_32;
    
    TimerHandler.TimerLoad      = (uint32_t)dev;
    
    DUALTIMER_Init(TIMER_0, &TimerHandler);
    
    DUALTIMER_IntConfig(TIMER_0, ENABLE);
    
    NVIC_EnableIRQ(TIMER_IRQn);
    
    DUALTIMER_Start(TIMER_0);
    

}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(TIMER_IRQn);
    
    DUALTIMER_IntConfig(TIMER_0, DISABLE);
}

void us_ticker_clear_interrupt(void)
{
    DUALTIMER_IntClear(TIMER_0);
}
