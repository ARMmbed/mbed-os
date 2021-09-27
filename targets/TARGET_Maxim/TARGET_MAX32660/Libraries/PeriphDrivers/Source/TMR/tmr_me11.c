/* *****************************************************************************
 * Copyright(C) Maxim Integrated Products, Inc., All Rights Reserved.
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

#include "tmr.h"
#include "tmr_reva.h"
#include "tmr_common.h"

int MXC_TMR_Init(mxc_tmr_regs_t *tmr, mxc_tmr_cfg_t* cfg)
{
    int tmr_id = MXC_TMR_GET_IDX(tmr);

    MXC_ASSERT(tmr_id >= 0);
    
    switch(cfg->clock){
    case MXC_TMR_EXT_CLK:
        MXC_GPIO_Config(&gpio_cfg_32kcal);
        break;

    case MXC_TMR_HFIO_CLK:
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_HIRC);
        break;

    case MXC_TMR_NANORING_CLK:
        MXC_SYS_ClockSourceEnable(MXC_SYS_CLOCK_NANORING);
        break;

    default:
        return E_BAD_PARAM;
        break;        
    }

    //enable peripheral clock and configure gpio pins
    switch(tmr_id) {
    case 0:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TIMER0);        
        while(MXC_GCR->rst0 & MXC_F_GCR_RST0_TIMER0);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR0);
        MXC_GPIO_Config(&gpio_cfg_tmr0);
        break;
    case 1:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TIMER1);
        while(MXC_GCR->rst0 & MXC_F_GCR_RST0_TIMER1);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR1);
        break;
    case 2:
        MXC_SYS_Reset_Periph(MXC_SYS_RESET0_TIMER2);
        while(MXC_GCR->rst0 & MXC_F_GCR_RST0_TIMER2);
        MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_TMR2);
        break;
    }
    MXC_TMR_RevA_Init((mxc_tmr_reva_regs_t*) tmr, cfg);
    return E_NO_ERROR;
}

void MXC_TMR_Shutdown(mxc_tmr_regs_t *tmr)
{
    int tmr_id = MXC_TMR_GET_IDX(tmr);
    MXC_ASSERT(tmr_id >= 0);

    MXC_TMR_RevA_Shutdown((mxc_tmr_reva_regs_t*) tmr);
    
    // System settigns
    //diasble peripheral clock
    switch(tmr_id) {
    case 0:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR0);
        break;

    case 1:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR1);
        break;

    case 2:
        MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_TMR2);
        break;
    }
}

void MXC_TMR_Start(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevA_Start((mxc_tmr_reva_regs_t*) tmr);
}

void MXC_TMR_Stop(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevA_Stop((mxc_tmr_reva_regs_t*) tmr);
}

int MXC_TMR_SetPWM(mxc_tmr_regs_t* tmr, uint32_t pwm)
{
    return MXC_TMR_RevA_SetPWM((mxc_tmr_reva_regs_t*) tmr, pwm);
}

uint32_t MXC_TMR_GetCompare(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevA_GetCompare((mxc_tmr_reva_regs_t*) tmr);
}

uint32_t MXC_TMR_GetCapture(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevA_GetCapture((mxc_tmr_reva_regs_t*) tmr);
}

uint32_t MXC_TMR_GetPeriod(mxc_tmr_regs_t* tmr, mxc_tmr_clock_t clock, uint32_t prescalar, uint32_t frequency)
{
    uint32_t retVal, clkFreq;
    switch(clock) {
        case MXC_TMR_HFIO_CLK:
            clkFreq = PeripheralClock;
            break;
        case MXC_TMR_NANORING_CLK:
            clkFreq = 80000;
            break;
        case MXC_TMR_EXT_CLK:
            clkFreq = HFX_FREQ;
            break;
        default:
            clkFreq = PeripheralClock;
            break;
    }
    if(frequency == 0) {
        return 0;
    }
    else {
        retVal = clkFreq / (prescalar * frequency);
        return retVal;
    }
    return retVal;
}

uint32_t MXC_TMR_GetCount(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevA_GetCount((mxc_tmr_reva_regs_t*) tmr);
}

void MXC_TMR_ClearFlags(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_RevA_ClearFlags((mxc_tmr_reva_regs_t*) tmr);
}

uint32_t MXC_TMR_GetFlags(mxc_tmr_regs_t* tmr)
{
    return MXC_TMR_RevA_GetFlags((mxc_tmr_reva_regs_t*) tmr);
}

void MXC_TMR_SetCompare(mxc_tmr_regs_t *tmr, uint32_t cmp_cnt)
{
    MXC_TMR_RevA_SetCompare((mxc_tmr_reva_regs_t*) tmr, cmp_cnt);
}

void MXC_TMR_SetCount(mxc_tmr_regs_t *tmr, uint32_t cnt)
{
    MXC_TMR_RevA_SetCount((mxc_tmr_reva_regs_t*) tmr, cnt);
}

void MXC_TMR_Delay(mxc_tmr_regs_t *tmr, unsigned long us)
{
    MXC_TMR_Common_Delay(tmr, us);
}

void MXC_TMR_TO_Start(mxc_tmr_regs_t *tmr, unsigned long us)
{
    MXC_TMR_RevA_TO_Start((mxc_tmr_reva_regs_t*) tmr, us);
}

int MXC_TMR_TO_Check(mxc_tmr_regs_t *tmr)
{
    return MXC_TMR_Common_TO_Check(tmr);
}

void MXC_TMR_TO_Stop(mxc_tmr_regs_t *tmr)
{
    MXC_TMR_Common_TO_Stop(tmr);
}

void MXC_TMR_TO_Clear(mxc_tmr_regs_t *tmr)
{
    MXC_TMR_Common_TO_Clear(tmr);
}

unsigned int MXC_TMR_TO_Elapsed(mxc_tmr_regs_t *tmr)
{
    return MXC_TMR_Common_TO_Elapsed(tmr);
}

unsigned int MXC_TMR_TO_Remaining(mxc_tmr_regs_t *tmr)
{
    return MXC_TMR_Common_TO_Remaining(tmr);
}

void MXC_TMR_SW_Start(mxc_tmr_regs_t *tmr)
{
    return MXC_TMR_Common_SW_Start(tmr);
}

unsigned int MXC_TMR_SW_Stop(mxc_tmr_regs_t *tmr)
{
    return MXC_TMR_Common_SW_Stop(tmr);
}

int MXC_TMR_GetTime(mxc_tmr_regs_t *tmr, uint32_t ticks, uint32_t *time, mxc_tmr_unit_t *units)
{
    return MXC_TMR_RevA_GetTime((mxc_tmr_reva_regs_t*) tmr, ticks, time, units);
}
