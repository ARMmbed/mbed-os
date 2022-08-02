/* *****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *
 **************************************************************************** */
#ifdef __CC_ARM  // Keil 
#pragma diag_suppress 188  // enumerated type mixed with another type
#endif


#include "tmr.h"
#include "tmr_revb.h"
#include "tmr_common.h"
#include "stdbool.h"

int MXC_TMR_Init(mxc_tmr_regs_t* tmr, mxc_tmr_cfg_t* cfg, bool init_pins)
{
    int tmr_id = MXC_TMR_GET_IDX(tmr);
    uint8_t clockSource = MXC_TMR_CLK0;

    if(cfg == NULL) {
        return E_NULL_PTR;
    }
    
    MXC_ASSERT(tmr_id >= 0);
    
    switch (cfg->clock) {
    case MXC_TMR_EXT_CLK:
        clockSource = MXC_TMR_CLK1;
        MXC_GPIO_Config(&gpio_cfg_extclk);     
        break;
        
    case MXC_TMR_32K_CLK:
        if(tmr_id < 5) {
            return E_NOT_SUPPORTED;
        }
        clockSource = MXC_TMR_CLK2;
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_ERTCO);    
        break;
        
    case MXC_TMR_80K_CLK:
        if(tmr_id < 5) {
            return E_NOT_SUPPORTED;
        }
        clockSource = MXC_TMR_CLK3;
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_INRO);    
        break;
        
    case MXC_TMR_8M_CLK:
        if(tmr_id > 4) {
            return E_NOT_SUPPORTED;
        }
        clockSource = MXC_TMR_CLK2;
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_IBRO);    
        break;
        
    case MXC_TMR_32M_CLK:
        if(tmr_id > 4) {
            return E_NOT_SUPPORTED;
        }
        clockSource = MXC_TMR_CLK3;
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_ERFO);    
        break;     

    default:
        break;
    }
    
    //enable peripheral clock and configure gpio pins
    switch (tmr_id) {
    case 0:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TMR0);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR0);
        
        if (init_pins) {
            if (cfg->bitMode != TMR_BIT_MODE_16B) {
                MXC_GPIO_Config(&gpio_cfg_tmr0);
            }
        }
        
        break;
        
    case 1:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TMR1);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR1);
        
        if (init_pins) {
            if (cfg->bitMode != TMR_BIT_MODE_16B) {
                MXC_GPIO_Config(&gpio_cfg_tmr1);
            }
        }
        
        break;
        
    case 2:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TMR2);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR2);
        
        if (init_pins) {
            if (cfg->bitMode != TMR_BIT_MODE_16B) {
                MXC_GPIO_Config(&gpio_cfg_tmr2);
            }
        }
        
        break;
        
    case 3:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TMR3);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR3);
        
        if (init_pins) {
            if (cfg->bitMode != TMR_BIT_MODE_16B) {
                MXC_GPIO_Config(&gpio_cfg_tmr3);
            }
        }
        
        break;
        
    case 4:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_TMR4);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR4);
        
        if (init_pins) {
            if (cfg->bitMode != TMR_BIT_MODE_16B) {
                MXC_GPIO_Config(&gpio_cfg_tmr4);
            }
            else {
                return E_NOT_SUPPORTED;
            }
        }
        
        break;
        
    case 5:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_TMR5);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR5);
        
        if (init_pins) {
            if (cfg->bitMode != TMR_BIT_MODE_16B) {
                MXC_GPIO_Config(&gpio_cfg_tmr5);
            }
            else {
                return E_NOT_SUPPORTED;
            }
        }
        
        break;
    }
    
    return MXC_TMR_RevB_Init ((mxc_tmr_revb_regs_t*) tmr, cfg, clockSource);
}

void MXC_TMR_Shutdown(mxc_tmr_regs_t* tmr)
{
    int tmr_id = MXC_TMR_GET_IDX(tmr);
    
    MXC_ASSERT(tmr_id >= 0);
    
    MXC_TMR_RevB_Shutdown ((mxc_tmr_revb_regs_t*) tmr);
    
    // System settigns
    //diasble peripheral clock
    switch (tmr_id) {
    case 0:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR0);
        break;
        
    case 1:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR1);
        break;
        
    case 2:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR2);
        break;
        
    case 3:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR3);
        break;
        
    case 4:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR4);
        break;
        
    case 5:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR5);
        break;
    }
}

void MXC_TMR_Start(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevB_Start ((mxc_tmr_revb_regs_t*) tmr);
}

void MXC_TMR_Stop(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevB_Stop ((mxc_tmr_revb_regs_t*) tmr);
}

int MXC_TMR_SetPWM(mxc_tmr_regs_t* tmr, uint32_t pwm)
{
    return MXC_TMR_RevB_SetPWM((mxc_tmr_revb_regs_t*) tmr, pwm);
}

uint32_t MXC_TMR_GetCompare(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevB_GetCompare((mxc_tmr_revb_regs_t*)tmr);
}

uint32_t MXC_TMR_GetCapture(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevB_GetCompare ((mxc_tmr_revb_regs_t*) tmr);
}

uint32_t MXC_TMR_GetPeriod(mxc_tmr_regs_t* tmr, mxc_tmr_clock_t clock, uint32_t prescalar, uint32_t frequency)
{
    uint32_t clockFrequency = PeripheralClock;
    int tmr_id = MXC_TMR_GET_IDX(tmr);
    
    MXC_ASSERT(tmr_id >= 0);
    
    if (tmr_id > 3) {
        switch (clock) {
        case MXC_TMR_APB_CLK:
            clockFrequency = (PeripheralClock / 4);
            break;
            
        case MXC_TMR_32K_CLK:
            clockFrequency = ERTCO_FREQ;
            break;
            
        case MXC_TMR_80K_CLK:
            clockFrequency = INRO_FREQ;
            break;
            
        default:
            break;
        }
    }
    else {
        switch (clock) {
        case MXC_TMR_APB_CLK:
            clockFrequency = PeripheralClock;
            break;
            
        case MXC_TMR_8M_CLK:
            clockFrequency = IBRO_FREQ;
            break;
            
        case MXC_TMR_32M_CLK:
            clockFrequency = 16000000;      // Clock Frequency 16 MHz
            break;
            
        default:
            break;
        }
    }
    
    return MXC_TMR_RevB_GetPeriod((mxc_tmr_revb_regs_t*)tmr, clockFrequency, prescalar, frequency);
}

uint32_t MXC_TMR_GetCount(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevB_GetCount((mxc_tmr_revb_regs_t*)tmr);
}

void MXC_TMR_ClearFlags(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevB_ClearFlags((mxc_tmr_revb_regs_t*)tmr);
}

uint32_t MXC_TMR_GetFlags(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevB_GetFlags((mxc_tmr_revb_regs_t*)tmr);
}

void MXC_TMR_EnableInt(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevB_EnableInt((mxc_tmr_revb_regs_t*)tmr);
}

void MXC_TMR_DisableInt(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevB_DisableInt((mxc_tmr_revb_regs_t*)tmr);
}

void MXC_TMR_EnableWakeup (mxc_tmr_regs_t* tmr, mxc_tmr_cfg_t* cfg)
{
    MXC_TMR_RevB_EnableWakeup ((mxc_tmr_revb_regs_t*)tmr, cfg);
}

void MXC_TMR_DisableWakeup (mxc_tmr_regs_t* tmr, mxc_tmr_cfg_t* cfg)
{
    MXC_TMR_RevB_DisableWakeup ((mxc_tmr_revb_regs_t*)tmr, cfg);
}

void MXC_TMR_SetCompare(mxc_tmr_regs_t* tmr, uint32_t cmp_cnt)
{
    MXC_TMR_RevB_SetCompare((mxc_tmr_revb_regs_t*)tmr, cmp_cnt);
}

void MXC_TMR_SetCount(mxc_tmr_regs_t* tmr, uint32_t cnt)
{
    MXC_TMR_RevB_SetCount((mxc_tmr_revb_regs_t*)tmr, cnt);
}

void MXC_TMR_Delay(mxc_tmr_regs_t* tmr, unsigned long us)
{
    MXC_TMR_Common_Delay(tmr, us);
}

void MXC_TMR_TO_Start(mxc_tmr_regs_t* tmr, unsigned long us)
{
    MXC_TMR_RevB_TO_Start((mxc_tmr_revb_regs_t*)tmr, us);
}

int MXC_TMR_TO_Check(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_Common_TO_Check(tmr);
}

void MXC_TMR_TO_Stop(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_Common_TO_Stop(tmr);
}

void MXC_TMR_TO_Clear(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_Common_TO_Clear(tmr);
}

unsigned int MXC_TMR_TO_Elapsed(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_Common_TO_Elapsed(tmr);
}

unsigned int MXC_TMR_TO_Remaining(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_Common_TO_Remaining(tmr);
}

void MXC_TMR_SW_Start(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_Common_SW_Start(tmr);
}

unsigned int MXC_TMR_SW_Stop(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_Common_SW_Stop(tmr);
}

int MXC_TMR_GetTime(mxc_tmr_regs_t* tmr, uint32_t ticks, uint32_t* time, mxc_tmr_unit_t* units)
{
    return MXC_TMR_RevB_GetTime((mxc_tmr_revb_regs_t*)tmr, ticks, time, units);
}
