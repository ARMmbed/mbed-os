/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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

#if DEVICE_LPTICKER

#include <stdio.h>
#include <string.h>
#include "rtc_api.h"
#include "lp_ticker_api.h"
#include "rtc.h"
#include "lp.h"
#include "mxc_sys.h"

#if 0

#define LP_TIMER_RATE_HZ  4096
#define LP_TIMER_WIDTH    32

/* Converts a time in milleseconds to the equivalent RSSA register value. */
#define MSEC_TO_RSSA(x) (0 - ((x * 4096) / 1000))

static uint32_t g_nb_irq_tick = 0;

//******************************************************************************
void lp_ticker_init(void)
{    
    MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_X32K_EN;

    while(MXC_RTC_DisableInt(MXC_RTC_INT_EN_SHORT) == E_BUSY) {
        ;
    }

    NVIC_SetVector(RTC_IRQn, (uint32_t)lp_ticker_irq_handler);
    NVIC_EnableIRQ(RTC_IRQn);

    while(MXC_RTC_Start() == E_BUSY) {
       ;
    }

    MXC_LP_EnableRTCAlarmWakeup();

    MXC_RTC->rssa = 0;
}

//******************************************************************************
void lp_ticker_free(void)
{
    while(MXC_RTC_DisableInt(MXC_RTC_INT_EN_SHORT) == E_BUSY) {
        ;
    }
}

//******************************************************************************
uint32_t lp_ticker_read(void)
{
    uint32_t tick;

    tick =  g_nb_irq_tick + g_nb_irq_tick - (0-MXC_RTC->rssa);

    return tick;
}

//******************************************************************************
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    while(MXC_RTC_DisableInt(MXC_RTC_INT_EN_SHORT) == E_BUSY) {
        ;
    }

    g_nb_irq_tick = timestamp ? timestamp : 1;

    while(MXC_RTC_SetSubsecondAlarm(0-g_nb_irq_tick) == E_BUSY) {
        ;
    }
    
    while(MXC_RTC_EnableInt(MXC_RTC_INT_EN_SHORT) == E_BUSY) {
        ;
    }
   
    while(MXC_RTC_Start() == E_BUSY) {
        ;
    }
}

//******************************************************************************
void lp_ticker_disable_interrupt(void)
{
    while(MXC_RTC_DisableInt(MXC_RTC_INT_EN_SHORT) == E_BUSY) {
        ;
    }
    NVIC_DisableIRQ(RTC_IRQn);
}

//******************************************************************************
void lp_ticker_clear_interrupt(void)
{
    MXC_RTC->ctrl &= ~(MXC_F_RTC_CTRL_ALSF);
    MXC_RTC->ctrl &= ~(MXC_F_RTC_CTRL_ALDF);
}

//******************************************************************************
void lp_ticker_fire_interrupt(void)
{
    MXC_RTC->ctrl |= MXC_F_RTC_CTRL_ALSF;
    NVIC_SetPendingIRQ(RTC_IRQn);
}

//******************************************************************************
const ticker_info_t *lp_ticker_get_info(void)
{
    static const ticker_info_t info = {
        LP_TIMER_RATE_HZ,
        LP_TIMER_WIDTH
    };

    return &info;
}
#else

#include "tmr.h"

#define LP_TIMER          MXC_TMR2
#define LP_TIMER_IRQn     TMR2_IRQn
#define LP_TIMER_PRESCALE TMR_PRES_1024
#define LP_TIMER_FREQ     ((HIRC96_FREQ/2) >> (MXC_V_TMR_CN_PRES_DIV_BY_1024+1))
#define LP_TIMER_WIDTH    32

//******************************************************************************
void lp_ticker_init(void)
{    
    mxc_tmr_cfg_t cfg;
    unsigned int count;

    cfg.pres    = LP_TIMER_PRESCALE;   
    cfg.mode    = TMR_MODE_COMPARE;        
    cfg.bitMode = TMR_BIT_MODE_32;  
    cfg.clock   = MXC_TMR_HFIO_CLK;        
    cfg.cmp_cnt = 0;//MXC_TMR_GetCompare(LP_TIMER);              
    cfg.pol     = 0;                  

    // Disable and deconfigure
    MXC_TMR_Shutdown(LP_TIMER);
    MXC_TMR_ClearFlags(LP_TIMER);

    count = MXC_TMR_GetCount(LP_TIMER);

    // Configure and enable
    MXC_TMR_Init(LP_TIMER, &cfg);
    MXC_TMR_SetCount(LP_TIMER, count);
    MXC_TMR_Start(LP_TIMER);

    // Enable interrupts
    NVIC_SetVector(LP_TIMER_IRQn, (uint32_t)lp_ticker_irq_handler);
    NVIC_EnableIRQ(LP_TIMER_IRQn);
}

//******************************************************************************
void lp_ticker_free(void)
{
    MXC_TMR_Shutdown(LP_TIMER);
}

//******************************************************************************
uint32_t lp_ticker_read(void)
{
    return LP_TIMER->cnt;
}

//******************************************************************************
void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    MXC_TMR_SetCompare(LP_TIMER, (timestamp) ? timestamp : 1);
}

//******************************************************************************
void lp_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(LP_TIMER_IRQn);
}

//******************************************************************************
void lp_ticker_clear_interrupt(void)
{
    MXC_TMR_ClearFlags(LP_TIMER);
}

//******************************************************************************
void lp_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(LP_TIMER_IRQn);
}

//******************************************************************************
const ticker_info_t *lp_ticker_get_info(void)
{
    static const ticker_info_t info = {
        LP_TIMER_FREQ,
        LP_TIMER_WIDTH
    };

    return &info;
}

#endif

#endif // DEVICE_LPTICKER