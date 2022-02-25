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
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "wdt.h"
#include "wdt_reva.h"
#include "gcr_regs.h"

/* **** Functions **** */

int MXC_WDT_Init (mxc_wdt_regs_t* wdt)
{
    if (wdt == MXC_WDT0) {
        return E_NO_ERROR;
    }
    else {
        return E_BAD_PARAM;
    }
    
    return E_NO_ERROR;
}

int MXC_WDT_Shutdown (mxc_wdt_regs_t* wdt)
{
    if (wdt == MXC_WDT0) {
        return E_NO_ERROR;
    }
    else {
        return E_BAD_PARAM;
    }
    
    return E_NO_ERROR;
}
void MXC_WDT_SetIntPeriod (mxc_wdt_regs_t* wdt, mxc_wdt_period_t period)
{
    MXC_WDT_RevA_SetIntPeriod ((mxc_wdt_reva_regs_t*) wdt,period);
}

void MXC_WDT_SetResetPeriod (mxc_wdt_regs_t* wdt, mxc_wdt_period_t period)
{
    MXC_WDT_RevA_SetResetPeriod ((mxc_wdt_reva_regs_t*) wdt,period);
}

void MXC_WDT_Enable (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_Enable ((mxc_wdt_reva_regs_t*) wdt);
}

void MXC_WDT_Disable (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_Disable ((mxc_wdt_reva_regs_t*) wdt);
}

void MXC_WDT_EnableInt (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_EnableInt ((mxc_wdt_reva_regs_t*) wdt, MXC_WDT_REVA_ENABLE);
}

void MXC_WDT_EnableReset (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_EnableReset ((mxc_wdt_reva_regs_t*) wdt, MXC_WDT_REVA_ENABLE);
}

void MXC_WDT_DisableInt (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_EnableInt ((mxc_wdt_reva_regs_t*) wdt, MXC_WDT_REVA_DISABLE);
}

void MXC_WDT_DisableReset (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_EnableReset ((mxc_wdt_reva_regs_t*) wdt, MXC_WDT_REVA_DISABLE);
}

void MXC_WDT_ResetTimer (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_ResetTimer ((mxc_wdt_reva_regs_t*) wdt);
}

int MXC_WDT_GetResetFlag (mxc_wdt_regs_t* wdt)
{
    return MXC_WDT_RevA_GetResetFlag ((mxc_wdt_reva_regs_t*) wdt);
}

void MXC_WDT_ClearResetFlag (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_ClearResetFlag ((mxc_wdt_reva_regs_t*) wdt);
}

int MXC_WDT_GetIntFlag (mxc_wdt_regs_t* wdt)
{
    return MXC_WDT_RevA_GetIntFlag ((mxc_wdt_reva_regs_t*) wdt);
}

void MXC_WDT_ClearIntFlag (mxc_wdt_regs_t* wdt)
{
    MXC_WDT_RevA_ClearIntFlag ((mxc_wdt_reva_regs_t*) wdt);
}
