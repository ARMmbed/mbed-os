/* *****************************************************************************
 * Copyright(C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "wdt_revb.h"

/* **** Functions **** */

int MXC_WDT_RevB_Init(mxc_wdt_revb_regs_t* wdt, mxc_wdt_revb_cfg_t *cfg)
{
    if (wdt == NULL || cfg == NULL) {
        return E_NULL_PTR;
    }

    if(cfg->mode & MXC_WDT_REVB_WINDOWED){
        wdt->ctrl |= MXC_F_WDT_REVB_CTRL_WIN_EN;
    }
    else{
        wdt->ctrl &= ~(MXC_F_WDT_REVB_CTRL_WIN_EN);
    }
    
    return E_NO_ERROR;
}

void MXC_WDT_RevB_SetIntPeriod(mxc_wdt_revb_regs_t* wdt, mxc_wdt_revb_cfg_t *cfg)
{
    MXC_SETFIELD(wdt->ctrl, MXC_F_WDT_REVB_CTRL_INT_LATE_VAL, cfg->upperIntPeriod);

    if(cfg->mode & MXC_WDT_REVB_WINDOWED){
        MXC_SETFIELD(wdt->ctrl, MXC_F_WDT_REVB_CTRL_INT_EARLY_VAL, \
           (cfg->lowerIntPeriod<<MXC_F_WDT_REVB_CTRL_INT_EARLY_VAL_POS));
    }
}

void MXC_WDT_RevB_SetResetPeriod(mxc_wdt_revb_regs_t* wdt, mxc_wdt_revb_cfg_t *cfg)
{
    MXC_SETFIELD(wdt->ctrl, MXC_F_WDT_REVB_CTRL_RST_LATE_VAL, \
       (cfg->upperResetPeriod << MXC_F_WDT_REVB_CTRL_RST_LATE_VAL_POS));

    if(cfg->mode & MXC_WDT_REVB_WINDOWED){
        MXC_SETFIELD(wdt->ctrl, MXC_F_WDT_REVB_CTRL_RST_EARLY_VAL, \
           (cfg->lowerResetPeriod << MXC_F_WDT_REVB_CTRL_RST_EARLY_VAL_POS));
    }
}

void MXC_WDT_RevB_Enable(mxc_wdt_revb_regs_t* wdt)
{
    wdt->ctrl |= MXC_F_WDT_REVB_CTRL_EN;
}

void MXC_WDT_RevB_Disable(mxc_wdt_revb_regs_t* wdt)
{
    wdt->ctrl &= ~(MXC_F_WDT_REVB_CTRL_EN);
}

void MXC_WDT_RevB_EnableInt(mxc_wdt_revb_regs_t* wdt, mxc_wdt_revb_en_t enable)
{
    if(enable) {
        wdt->ctrl |= MXC_F_WDT_REVB_CTRL_WDT_INT_EN;
    }
    else {
        wdt->ctrl &= ~(MXC_F_WDT_REVB_CTRL_WDT_INT_EN);
    }
}

void MXC_WDT_RevB_EnableReset(mxc_wdt_revb_regs_t* wdt, mxc_wdt_revb_en_t enable)
{
    if(enable) {
        wdt->ctrl |= MXC_F_WDT_REVB_CTRL_WDT_RST_EN;
    }
    else {
        wdt->ctrl &= ~(MXC_F_WDT_REVB_CTRL_WDT_RST_EN);
    }
}

void MXC_WDT_RevB_ResetTimer(mxc_wdt_revb_regs_t* wdt)
{
    wdt->rst = 0x00A5;
    wdt->rst = 0x005A;
}

int MXC_WDT_RevB_GetResetFlag(mxc_wdt_revb_regs_t* wdt)
{
    return (wdt->ctrl &(MXC_F_WDT_REVB_CTRL_RST_LATE | MXC_F_WDT_REVB_CTRL_RST_EARLY));
}

void MXC_WDT_RevB_ClearResetFlag(mxc_wdt_revb_regs_t* wdt)
{
    wdt->ctrl &= ~(MXC_F_WDT_REVB_CTRL_RST_LATE | MXC_F_WDT_REVB_CTRL_RST_EARLY);
}

int MXC_WDT_RevB_GetIntFlag(mxc_wdt_revb_regs_t* wdt)
{
    return !!(wdt->ctrl &(MXC_F_WDT_REVB_CTRL_INT_LATE | MXC_F_WDT_REVB_CTRL_INT_EARLY));
}

void MXC_WDT_RevB_ClearIntFlag(mxc_wdt_revb_regs_t* wdt)
{
    wdt->ctrl &= ~(MXC_F_WDT_REVB_CTRL_INT_LATE | MXC_F_WDT_REVB_CTRL_INT_EARLY);
}
