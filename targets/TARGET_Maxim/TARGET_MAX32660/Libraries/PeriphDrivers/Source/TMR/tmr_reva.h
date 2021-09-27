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

/* **** Includes **** */
#include <stddef.h>
#include "mxc_assert.h"
#include "tmr.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "mxc_lock.h"
#include "tmr_reva_regs.h"

/* **** Functions **** */
void MXC_TMR_RevA_Init(mxc_tmr_reva_regs_t *tmr, mxc_tmr_cfg_t* cfg);
void MXC_TMR_RevA_Shutdown(mxc_tmr_reva_regs_t *tmr);
void MXC_TMR_RevA_Start(mxc_tmr_reva_regs_t* tmr);
void MXC_TMR_RevA_Stop(mxc_tmr_reva_regs_t* tmr);
int MXC_TMR_RevA_SetPWM(mxc_tmr_reva_regs_t* tmr, uint32_t pwm);
uint32_t MXC_TMR_RevA_GetCompare(mxc_tmr_reva_regs_t* tmr);
uint32_t MXC_TMR_RevA_GetCapture(mxc_tmr_reva_regs_t* tmr);
uint32_t MXC_TMR_RevA_GetCount(mxc_tmr_reva_regs_t* tmr);
void MXC_TMR_RevA_ClearFlags(mxc_tmr_reva_regs_t* tmr);
uint32_t MXC_TMR_RevA_GetFlags(mxc_tmr_reva_regs_t* tmr);
void MXC_TMR_RevA_SetCompare(mxc_tmr_reva_regs_t *tmr, uint32_t cmp_cnt);
void MXC_TMR_RevA_SetCount(mxc_tmr_reva_regs_t *tmr, uint32_t cnt);
void MXC_TMR_RevA_TO_Start(mxc_tmr_reva_regs_t *tmr, unsigned long us);
int MXC_TMR_RevA_GetTime(mxc_tmr_reva_regs_t *tmr, uint32_t ticks, uint32_t *time, mxc_tmr_unit_t *units);

