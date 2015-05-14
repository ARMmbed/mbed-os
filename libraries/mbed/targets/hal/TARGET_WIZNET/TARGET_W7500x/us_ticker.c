/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

    dev = (timestamp - us_ticker_read() + 0x5153);
    if(dev <= 0)
    {
        us_ticker_irq_handler();
    	return;
    }

    PWM_CHn_Stop(TIM_MST6);

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
