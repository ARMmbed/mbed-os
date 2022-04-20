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

/* **** Includes **** */
#include <stddef.h>
#include "mxc_assert.h"
#include "tmr.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "mxc_lock.h"
#include "tmr_revb_regs.h"


typedef enum {
    MXC_TMR_CLK0,
    MXC_TMR_CLK1,
    MXC_TMR_CLK2,
    MXC_TMR_CLK3, 
} mxc_tmr_clksel_t;

/* **** Functions **** */
int MXC_TMR_RevB_Init (mxc_tmr_revb_regs_t *tmr, mxc_tmr_cfg_t* cfg, uint8_t clk_src);
void MXC_TMR_RevB_ConfigGeneric(mxc_tmr_revb_regs_t *tmr, mxc_tmr_cfg_t* cfg);
void MXC_TMR_RevB_Shutdown (mxc_tmr_revb_regs_t *tmr);
void MXC_TMR_RevB_Start (mxc_tmr_revb_regs_t* tmr);
void MXC_TMR_RevB_Stop (mxc_tmr_revb_regs_t* tmr);
int MXC_TMR_RevB_SetPWM (mxc_tmr_revb_regs_t* tmr, uint32_t pwm);
uint32_t MXC_TMR_RevB_GetCompare (mxc_tmr_revb_regs_t* tmr);
uint32_t MXC_TMR_RevB_GetCapture (mxc_tmr_revb_regs_t* tmr);
uint32_t MXC_TMR_RevB_GetCount (mxc_tmr_revb_regs_t* tmr);
uint32_t MXC_TMR_RevB_GetPeriod (mxc_tmr_revb_regs_t* tmr, uint32_t clk_frequency, uint32_t prescalar, uint32_t frequency);
void MXC_TMR_RevB_ClearFlags (mxc_tmr_revb_regs_t* tmr);
uint32_t MXC_TMR_RevB_GetFlags (mxc_tmr_revb_regs_t* tmr);
void MXC_TMR_RevB_EnableInt (mxc_tmr_revb_regs_t* tmr);
void MXC_TMR_RevB_DisableInt (mxc_tmr_revb_regs_t* tmr);
void MXC_TMR_RevB_EnableWakeup (mxc_tmr_revb_regs_t* tmr, mxc_tmr_cfg_t* cfg);
void MXC_TMR_RevB_DisableWakeup (mxc_tmr_revb_regs_t* tmr, mxc_tmr_cfg_t* cfg);
void MXC_TMR_RevB_SetCompare (mxc_tmr_revb_regs_t *tmr, uint32_t cmp_cnt);
void MXC_TMR_RevB_SetCount (mxc_tmr_revb_regs_t *tmr, uint32_t cnt);
void MXC_TMR_RevB_TO_Start (mxc_tmr_revb_regs_t *tmr, unsigned long us);
int MXC_TMR_RevB_GetTime (mxc_tmr_revb_regs_t *tmr, uint32_t ticks, uint32_t *time, mxc_tmr_unit_t *units);
int MXC_TMR_RevB_GetTicks(mxc_tmr_revb_regs_t *tmr, uint32_t time, mxc_tmr_unit_t units, uint32_t *ticks);
