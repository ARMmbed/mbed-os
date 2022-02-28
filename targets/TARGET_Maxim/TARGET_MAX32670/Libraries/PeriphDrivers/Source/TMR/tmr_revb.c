/* *****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files(the "Software"), 
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
 *
 **************************************************************************** */
#ifdef __CC_ARM  // Keil 
#pragma diag_suppress 188  // enumerated type mixed with another type
#endif

/* **** Includes **** */
#include <stddef.h>
#include "mxc_assert.h"
#include "tmr.h"
#include "tmr_revb.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "mxc_lock.h"

/* **** Definitions **** */
#define TIMER_16A_OFFSET   0
#define TIMER_16B_OFFSET   16


/* **** Functions **** */
int MXC_TMR_RevB_Init(mxc_tmr_revb_regs_t *tmr, mxc_tmr_cfg_t* cfg, uint8_t clk_src)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    if(cfg == NULL){
        return E_NULL_PTR;
    }
    
    uint32_t timerOffset;
    
    if (cfg->bitMode == TMR_BIT_MODE_16B) {
        timerOffset = TIMER_16B_OFFSET;
    }
    else {
        timerOffset = TIMER_16A_OFFSET;
    }
    
    // Default 32 bit timer
    if(cfg->bitMode & (TMR_BIT_MODE_16A | TMR_BIT_MODE_16B)){
        tmr->ctrl1 &= ~MXC_F_TMR_REVB_CTRL1_CASCADE;
    }else{
        tmr->ctrl1 |= MXC_F_TMR_REVB_CTRL1_CASCADE;
    }
    
    // Clear interrupt flag
    tmr->intfl |= (MXC_F_TMR_REVB_INTFL_IRQ_A | MXC_F_TMR_REVB_INTFL_IRQ_B);
    
    // Set the prescale
    tmr->ctrl0 |= (cfg->pres << timerOffset);
    
    // Select clock Source
    tmr->ctrl1 |= ((clk_src << MXC_F_TMR_REVB_CTRL1_CLKSEL_A_POS) << timerOffset);
    
    //TIMER_16B only supports compare, oneshot and continuous modes.
    switch(cfg->mode) {
    case TMR_MODE_ONESHOT:
        MXC_TMR_RevB_ConfigGeneric((mxc_tmr_revb_regs_t*) tmr, cfg);
        break;
        
    case TMR_MODE_CONTINUOUS:
        MXC_TMR_RevB_ConfigGeneric((mxc_tmr_revb_regs_t*) tmr, cfg);
        break;
        
    case TMR_MODE_COUNTER:
        if (cfg->bitMode == TMR_BIT_MODE_16B) {
            return E_NOT_SUPPORTED;
        }
        
        MXC_TMR_RevB_ConfigGeneric(tmr, cfg);
        break;
        
    case TMR_MODE_CAPTURE:
        if (cfg->bitMode == TMR_BIT_MODE_16B) {
            return E_NOT_SUPPORTED;
        }
        
        MXC_TMR_RevB_ConfigGeneric(tmr, cfg);
        break;
        
    case TMR_MODE_COMPARE:
        MXC_TMR_RevB_ConfigGeneric((mxc_tmr_revb_regs_t*) tmr, cfg);
        break;
        
    case TMR_MODE_GATED:
        if (cfg->bitMode == TMR_BIT_MODE_16B) {
            return E_NOT_SUPPORTED;
        }
        
        MXC_TMR_RevB_ConfigGeneric(tmr, cfg);
        break;
        
    case TMR_MODE_CAPTURE_COMPARE:
        if (cfg->bitMode == TMR_BIT_MODE_16B) {
            return E_NOT_SUPPORTED;
        }
        
        MXC_TMR_RevB_ConfigGeneric(tmr, cfg);
        break;
        
    case TMR_MODE_PWM:
        if (cfg->bitMode == TMR_BIT_MODE_16B) {
            return E_NOT_SUPPORTED;
        }
        MXC_TMR_RevB_ConfigGeneric((mxc_tmr_revb_regs_t*) tmr, cfg);
        break;
    }
    
    return E_NO_ERROR;
}

void MXC_TMR_RevB_ConfigGeneric(mxc_tmr_revb_regs_t *tmr, mxc_tmr_cfg_t* cfg)
{
    uint32_t timerOffset;
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    if(cfg == NULL){
        return;
    }
    
    if(cfg->bitMode == TMR_BIT_MODE_16B) {
        timerOffset = TIMER_16B_OFFSET;
    }
    else {
        timerOffset = TIMER_16A_OFFSET;
    }

    tmr->ctrl0 |= (MXC_F_TMR_REVB_CTRL0_CLKEN_A << timerOffset);
    while(!(tmr->ctrl1 & (MXC_F_TMR_REVB_CTRL1_CLKRDY_A << timerOffset)));

    tmr->ctrl0 |= (cfg->mode << timerOffset);
    tmr->ctrl0 |= ((cfg->pol << MXC_F_TMR_REVB_CTRL0_POL_A_POS) << timerOffset);
    //enable timer interrupt if needed
    tmr->cnt = (0x1 << timerOffset);
    while(!(tmr->intfl & (MXC_F_TMR_REVB_INTFL_WRDONE_A << timerOffset)));

    tmr->cmp = (cfg->cmp_cnt << timerOffset);
#if TARGET_NUM == 32655 || TARGET_NUM == 78000 || TARGET_NUM == 32690 || TARGET_NUM == 78002
    tmr->ctrl1 &= ~(MXC_F_TMR_REVB_CTRL1_OUTEN_A << timerOffset);
#else
    tmr->ctrl1 |= (MXC_F_TMR_REVB_CTRL1_OUTEN_A << timerOffset);
#endif

    // If configured as TIMER_16B then enable the interrupt and start the timer
    if(cfg->bitMode == TMR_BIT_MODE_16B){
        tmr->ctrl1 |= MXC_F_TMR_REVB_CTRL1_IE_B;

        tmr->ctrl0 |= MXC_F_TMR_REVB_CTRL0_EN_B;
        while(!(tmr->ctrl1 & MXC_F_TMR_REVB_CTRL1_CLKEN_B));
    }
}

void MXC_TMR_RevB_Shutdown(mxc_tmr_revb_regs_t *tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);
    
    // Disable timer and clear settings
    tmr->ctrl0 = 0;
    while(tmr->ctrl1 & MXC_F_TMR_REVB_CTRL1_CLKRDY_A);
}

void MXC_TMR_RevB_Start(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->ctrl0 |= MXC_F_TMR_REVB_CTRL0_EN_A;
    while(!(tmr->ctrl1 & MXC_F_TMR_REVB_CTRL1_CLKEN_A));
}

void MXC_TMR_RevB_Stop(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->ctrl0 &= ~MXC_F_TMR_REVB_CTRL0_EN_A;
}

int MXC_TMR_RevB_SetPWM(mxc_tmr_revb_regs_t* tmr, uint32_t pwm)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);
    
    if(pwm >(tmr->cmp)) {
        return E_BAD_PARAM;
    }
    
    while(tmr->cnt >= pwm);
    
    tmr->pwm = pwm;
    while(!(tmr->intfl & MXC_F_TMR_REVB_INTFL_WRDONE_A));
    
    return E_NO_ERROR;
}

uint32_t MXC_TMR_RevB_GetCompare(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    return tmr->cmp;
}

uint32_t MXC_TMR_RevB_GetCapture(mxc_tmr_revb_regs_t* tmr)
{
    uint32_t pwm;
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    // read pwm register twice
    pwm = tmr->pwm;
    pwm = tmr->pwm;
    return pwm; 
}

uint32_t MXC_TMR_RevB_GetCount(mxc_tmr_revb_regs_t* tmr)
{
    uint32_t cnt;
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    // read cnt register twice
    cnt = tmr->cnt;
    cnt = tmr->cnt;
    return cnt;
}

uint32_t MXC_TMR_RevB_GetPeriod(mxc_tmr_revb_regs_t* tmr, uint32_t clk_frequency, uint32_t prescalar, uint32_t frequency)
{
    uint32_t periodTicks;
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    periodTicks = clk_frequency /(frequency * prescalar);
    
    return periodTicks;
}

void MXC_TMR_RevB_ClearFlags(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->intfl |= (MXC_F_TMR_REVB_INTFL_IRQ_A | MXC_F_TMR_REVB_INTFL_IRQ_B);
}

uint32_t MXC_TMR_RevB_GetFlags(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    return(tmr->intfl & (MXC_F_TMR_REVB_INTFL_IRQ_A | MXC_F_TMR_REVB_INTFL_IRQ_B));
}

void MXC_TMR_RevB_EnableInt(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->ctrl1 |= MXC_F_TMR_REVB_CTRL1_IE_A | MXC_F_TMR_REVB_CTRL1_IE_B;
}

void MXC_TMR_RevB_DisableInt(mxc_tmr_revb_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->ctrl1 &= ~(MXC_F_TMR_REVB_CTRL1_IE_A | MXC_F_TMR_REVB_CTRL1_IE_B);
}

void MXC_TMR_RevB_EnableWakeup(mxc_tmr_revb_regs_t* tmr, mxc_tmr_cfg_t* cfg)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    // Enable Timer wake-up source
    if(cfg->bitMode == TMR_BIT_MODE_16B)
    {
        tmr->ctrl1  |= MXC_F_TMR_REVB_CTRL1_WE_B;    
    }
    else
    {
        tmr->ctrl1  |= MXC_F_TMR_REVB_CTRL1_WE_A;    
    }
}

void MXC_TMR_RevB_DisableWakeup(mxc_tmr_revb_regs_t* tmr, mxc_tmr_cfg_t* cfg)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    // Disable Timer wake-up source
    if(cfg->bitMode == TMR_BIT_MODE_16B)
    {
        tmr->ctrl1  &= ~MXC_F_TMR_REVB_CTRL1_WE_B;    
    }
    else
    {
        tmr->ctrl1  &= ~MXC_F_TMR_REVB_CTRL1_WE_A;    
    }
}

void MXC_TMR_RevB_SetCompare(mxc_tmr_revb_regs_t *tmr, uint32_t cmp_cnt)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->cmp = cmp_cnt;
}

void MXC_TMR_RevB_SetCount(mxc_tmr_revb_regs_t *tmr, uint32_t cnt)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    tmr->cnt = cnt;
    while(!(tmr->intfl & MXC_F_TMR_REVB_INTFL_WRDONE_A));
}

void MXC_TMR_RevB_TO_Start(mxc_tmr_revb_regs_t *tmr, unsigned long us)
{
    uint64_t ticks;
    int clk_shift = 0;

    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    if(us == 0){
        return;
    }   

    ticks = (uint64_t) us *(uint64_t) PeripheralClock /(uint64_t) 1000000;
    
    while(ticks > 0xFFFFFFFFUL) {
        ticks >>= 1;
        ++clk_shift;
    }
    
    mxc_tmr_pres_t prescale = (mxc_tmr_pres_t) clk_shift << MXC_F_TMR_REVB_CTRL0_CLKDIV_A_POS;
    mxc_tmr_cfg_t cfg;
    
    // Initialize the timer in one-shot mode
    cfg.pres = prescale;
    cfg.mode = TMR_MODE_ONESHOT;
    cfg.bitMode = TMR_BIT_MODE_32;
    cfg.clock = MXC_TMR_APB_CLK;
    cfg.cmp_cnt = ticks;
    cfg.pol = 0;
    
    MXC_TMR_Stop((mxc_tmr_regs_t*) tmr);
    MXC_TMR_Init((mxc_tmr_regs_t*) tmr, &cfg, false);
    tmr->ctrl1 |= MXC_F_TMR_REVB_CTRL1_CASCADE;
    MXC_TMR_ClearFlags((mxc_tmr_regs_t*) tmr);
    MXC_TMR_Start((mxc_tmr_regs_t*) tmr);
}

int MXC_TMR_RevB_GetTime(mxc_tmr_revb_regs_t *tmr, uint32_t ticks, uint32_t *time, mxc_tmr_unit_t *units)
{
    int tmr_id = MXC_TMR_GET_IDX((mxc_tmr_regs_t*) tmr);
    (void)tmr_id;
    MXC_ASSERT(tmr_id >= 0);

    uint64_t temp_time = 0;
    uint32_t timerClock = PeripheralClock;
    uint32_t prescale = (tmr->ctrl0 & MXC_F_TMR_REVB_CTRL0_CLKDIV_A) >> MXC_F_TMR_REVB_CTRL0_CLKDIV_A_POS;
                        
    temp_time = (uint64_t) ticks * 1000 *(1 <<(prescale & 0xF)) /(timerClock / 1000000);
    
    if(!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = TMR_UNIT_NANOSEC;
        return E_NO_ERROR;
    }
    
    temp_time = (uint64_t) ticks * 1000 *(1 <<(prescale & 0xF)) /(timerClock / 1000);
    
    if(!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = TMR_UNIT_MICROSEC;
        return E_NO_ERROR;
    }
    
    temp_time = (uint64_t) ticks * 1000 *(1 <<(prescale & 0xF)) / timerClock;
    
    if(!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = TMR_UNIT_MILLISEC;
        return E_NO_ERROR;
    }
    
    temp_time = (uint64_t) ticks *(1 <<(prescale & 0xF)) / timerClock;
    
    if(!(temp_time & 0xffffffff00000000)) {
        *time = temp_time;
        *units = TMR_UNIT_SEC;
        return E_NO_ERROR;
    }
    
    return E_INVALID;
}

int MXC_TMR_RevB_GetTicks(mxc_tmr_revb_regs_t *tmr, uint32_t time, mxc_tmr_unit_t units, uint32_t *ticks)
{
    uint32_t unit_div0, unit_div1;
    uint32_t timerClock;
    uint32_t prescale;
    uint64_t temp_ticks;
    
    timerClock = PeripheralClock;

    prescale = ((tmr->ctrl0 & MXC_F_TMR_CTRL0_CLKDIV_A) >> MXC_F_TMR_CTRL0_CLKDIV_A_POS);      
    
    switch (units) {
        case TMR_UNIT_NANOSEC:
            unit_div0 = 1000000;
            unit_div1 = 1000;
            break;
        case TMR_UNIT_MICROSEC:
            unit_div0 = 1000;
            unit_div1 = 1000;
            break;
        case TMR_UNIT_MILLISEC:
            unit_div0 = 1;
            unit_div1 = 1000;
            break;
        case TMR_UNIT_SEC:
            unit_div0 = 1;
            unit_div1 = 1;
            break;
        default:
            return E_BAD_PARAM;
    }
    
    temp_ticks = (uint64_t)time * (timerClock / unit_div0) / (unit_div1 * (1 << (prescale & 0xF)));
    
    //make sure ticks is within a 32 bit value
    if (!(temp_ticks & 0xffffffff00000000)  && (temp_ticks & 0xffffffff)) {
        *ticks = temp_ticks;
        return E_NO_ERROR;
    }
    
    return E_INVALID;
}
