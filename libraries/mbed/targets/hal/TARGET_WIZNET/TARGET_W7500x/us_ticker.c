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

// 32-bit timer selection
#define TIM_MST7    PWM_CH7
#define TIM_MST6    PWM_CH6
#define IRQn_PWM6   PWM6_IRQn

static PWM_TimerModeInitTypeDef TimMasterHandle_CH7;
static PWM_TimerModeInitTypeDef TimMasterHandle_CH6;

static int us_ticker_inited = 0;


#ifdef __cplusplus
extern "C"{
#endif
void PWM6_Handler(void)
{
    uint32_t IntFlag = 0;

    IntFlag = PWM_CHn_GetIntFlagStatus(TIM_MST6);

    /* If overflow interrupt is occurred */
    if( (IntFlag & PWM_CHn_IER_OI_Msk) != 0 )
    {
        /* Clear overflow interrupt */
        PWM_CH6_ClearOverflowInt();
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
    TimMasterHandle_CH7.PWM_CHn_PR = (GetSystemClock() / 1000000) -1;
    TimMasterHandle_CH7.PWM_CHn_LR = 0xFFFFFFFF;
    TimMasterHandle_CH7.PWM_CHn_PDMR = 1;

    PWM_TimerModeInit(TIM_MST7, &TimMasterHandle_CH7);
    PWM_CHn_Start(TIM_MST7);
}


uint32_t us_ticker_read()
{
    if (!us_ticker_inited) us_ticker_init();

    return (TIM_MST7->TCR);
}


void us_ticker_set_interrupt(timestamp_t timestamp)
{
    int32_t dev = 0;
    if (!us_ticker_inited)
    {
        us_ticker_init();
    }
    
    dev = (int32_t)(timestamp - (us_ticker_read() + 150));

    if(dev <= 0)
    {
        us_ticker_irq_handler();
    	return;
    }

    PWM_CHn_Stop(TIM_MST6);

    SystemCoreClockUpdate();
    TimMasterHandle_CH6.PWM_CHn_PR = (GetSystemClock() / 1000000) -1;
    TimMasterHandle_CH6.PWM_CHn_LR = dev;

    TimMasterHandle_CH6.PWM_CHn_UDMR = 0;
    TimMasterHandle_CH6.PWM_CHn_PDMR = 0;

    NVIC_EnableIRQ(IRQn_PWM6);

    PWM_CHn_IntConfig(TIM_MST6, PWM_CHn_IER_OIE, ENABLE);
    PWM_IntConfig(TIM_MST6, ENABLE);
    PWM_TimerModeInit(TIM_MST6, &TimMasterHandle_CH6);

    PWM_CHn_Start(TIM_MST6);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(IRQn_PWM6);

    PWM_CHn_IntConfig(TIM_MST6, PWM_CHn_IER_OIE, DISABLE);
    PWM_IntConfig(TIM_MST6, DISABLE);
}

void us_ticker_clear_interrupt(void)
{
    PWM_CHn_ClearInt(TIM_MST6, PWM_CHn_IER_OIE);
}
