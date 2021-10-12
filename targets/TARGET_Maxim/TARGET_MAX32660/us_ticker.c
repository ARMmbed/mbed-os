/*******************************************************************************
 * Copyright (c) Maxim Integrated Products, Inc., All Rights Reserved.
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
 *******************************************************************************
 */

#include <stddef.h>
#include "us_ticker_api.h"
#include "tmr.h"

#define US_TIMER          MXC_TMR1
#define US_TIMER_IRQn     TMR1_IRQn
#define US_TIMER_PRESCALE TMR_PRES_32
#define US_TIMER_FREQ     ((HIRC96_FREQ/2) >> MXC_V_TMR_CN_PRES_DIV_BY_32)
#define US_TIMER_WIDTH    32


//******************************************************************************
void us_ticker_init(void)
{
    mxc_tmr_cfg_t cfg;
    unsigned int count;

    cfg.pres    = US_TIMER_PRESCALE;   
    cfg.mode    = TMR_MODE_COMPARE;        
    cfg.bitMode = TMR_BIT_MODE_32;  
    cfg.clock   = MXC_TMR_HFIO_CLK;        
    cfg.cmp_cnt = 0;//MXC_TMR_GetCompare(US_TIMER);              
    cfg.pol     = 0;                  

    // Disable and deconfigure
    MXC_TMR_Shutdown(US_TIMER);
    MXC_TMR_ClearFlags(US_TIMER);

    count = MXC_TMR_GetCount(US_TIMER);

    // Configure and enable
    MXC_TMR_Init(US_TIMER, &cfg);
    MXC_TMR_SetCount(US_TIMER, count);
    MXC_TMR_Start(US_TIMER);

    // Enable interrupts
    NVIC_SetVector(US_TIMER_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(US_TIMER_IRQn);
}

//******************************************************************************
void us_ticker_free(void)
{
    MXC_TMR_Shutdown(US_TIMER);
}

//******************************************************************************
uint32_t us_ticker_read(void)
{
    return US_TIMER->cnt;
}

//******************************************************************************
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    MXC_TMR_SetCompare(US_TIMER, (timestamp) ? timestamp : 1);
}

//******************************************************************************
void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(US_TIMER_IRQn);
}

//******************************************************************************
void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(US_TIMER_IRQn);
}

//******************************************************************************
void us_ticker_clear_interrupt(void)
{
    MXC_TMR_ClearFlags(US_TIMER);
}

//******************************************************************************
const ticker_info_t* us_ticker_get_info(void)
{
    static const ticker_info_t info = {
        US_TIMER_FREQ,
        US_TIMER_WIDTH
    };

    return &info;
}
