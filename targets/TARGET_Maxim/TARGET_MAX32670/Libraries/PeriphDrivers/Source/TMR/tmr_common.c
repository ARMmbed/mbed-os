/* *****************************************************************************
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
 *
 **************************************************************************** */

/* **** Includes **** */
#include <stddef.h>
#include "mxc_assert.h"
#include "tmr.h"
#include "tmr_common.h"

/* **** Functions **** */

void MXC_TMR_Common_Delay(mxc_tmr_regs_t* tmr, unsigned long us)
{
    // Return immediately if delay is 0
    if (!us) {
        return;
    }
    
    MXC_TMR_TO_Start(tmr, us);
    
    while (MXC_TMR_TO_Check(tmr) != E_TIME_OUT) {}
}

int MXC_TMR_Common_TO_Check(mxc_tmr_regs_t* tmr)
{
    if (MXC_TMR_GetFlags(tmr)) {
        return E_TIME_OUT;
    }
    
    return E_NO_ERROR;
}

void MXC_TMR_Common_TO_Stop(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_Stop(tmr);
    MXC_TMR_SetCount(tmr, 0x0);
}

void MXC_TMR_Common_TO_Clear(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_ClearFlags(tmr);
    MXC_TMR_SetCount(tmr, 0x0);
}

unsigned int MXC_TMR_Common_TO_Remaining(mxc_tmr_regs_t* tmr)
{
    uint32_t remaining_ticks, remaining_time;
    mxc_tmr_unit_t units;
    
    remaining_ticks = MXC_TMR_GetCompare(tmr) - MXC_TMR_GetCount(tmr);
    MXC_TMR_GetTime(tmr, remaining_ticks, &remaining_time, &units);
    
    switch (units) {
    case TMR_UNIT_NANOSEC:
    default:
        return (remaining_time / 1000);
        
    case TMR_UNIT_MICROSEC:
        return (remaining_time);
        
    case TMR_UNIT_MILLISEC:
        return (remaining_time * 1000);
        
    case TMR_UNIT_SEC:
        return (remaining_time * 1000000);
    }
}

void MXC_TMR_Common_SW_Start(mxc_tmr_regs_t* tmr)
{
    MXC_TMR_TO_Start(tmr, 0xFFFFFFFF);
}

unsigned int MXC_TMR_Common_SW_Stop(mxc_tmr_regs_t* tmr)
{
    unsigned int elapsed = MXC_TMR_TO_Elapsed(tmr);
    MXC_TMR_TO_Stop(tmr);
    return elapsed;
}

unsigned int MXC_TMR_Common_TO_Elapsed(mxc_tmr_regs_t* tmr)
{
    uint32_t elapsed;
    mxc_tmr_unit_t units;
    MXC_TMR_GetTime (tmr, tmr->cnt, &elapsed, &units);
    
    switch (units) {
    case TMR_UNIT_NANOSEC:
    default:
        return (elapsed / 1000);
        
    case TMR_UNIT_MICROSEC:
        return (elapsed);
        
    case TMR_UNIT_MILLISEC:
        return (elapsed * 1000);
        
    case TMR_UNIT_SEC:
        return (elapsed * 1000000);
    }
}
