/******************************************************************************
 * Copyright (C) 2023 Maxim Integrated Products, Inc., All Rights Reserved.
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
 ******************************************************************************/

#include "gpio_regs.h"
#include "mcr_regs.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_sys.h"
#include "rtc.h"
#include "rtc_regs.h"
#include "rtc_reva.h"
#include "tmr.h"

/* ***** Functions ***** */

int MXC_RTC_EnableInt(uint32_t mask)
{
    return MXC_RTC_RevA_EnableInt((mxc_rtc_reva_regs_t *)MXC_RTC, mask);
}

int MXC_RTC_DisableInt(uint32_t mask)
{
    return MXC_RTC_RevA_DisableInt((mxc_rtc_reva_regs_t *)MXC_RTC, mask);
}

int MXC_RTC_SetTimeofdayAlarm(uint32_t ras)
{
    return MXC_RTC_RevA_SetTimeofdayAlarm((mxc_rtc_reva_regs_t *)MXC_RTC, ras);
}

int MXC_RTC_SetSubsecondAlarm(uint32_t rssa)
{
    return MXC_RTC_RevA_SetSubsecondAlarm((mxc_rtc_reva_regs_t *)MXC_RTC, rssa);
}

int MXC_RTC_Start(void)
{
    return MXC_RTC_RevA_Start((mxc_rtc_reva_regs_t *)MXC_RTC);
}

int MXC_RTC_Stop(void)
{
    return MXC_RTC_RevA_Stop((mxc_rtc_reva_regs_t *)MXC_RTC);
}

int MXC_RTC_Init(uint32_t sec, uint16_t ssec)
{
    // Enable clock
    MXC_SYS_RTCClockEnable();

    return MXC_RTC_RevA_Init((mxc_rtc_reva_regs_t *)MXC_RTC, sec, (ssec & MXC_F_RTC_SSEC_SSEC));
}

int MXC_RTC_SquareWave(mxc_rtc_reva_sqwave_en_t sqe, mxc_rtc_freq_sel_t ft)
{
#if TARGET_NUM != 32675
    MXC_GPIO_Config(&gpio_cfg_rtcsqw);

    return MXC_RTC_RevA_SquareWave((mxc_rtc_reva_regs_t *)MXC_RTC, sqe, ft);
#else
    return E_NOT_SUPPORTED;
#endif
}

int MXC_RTC_SquareWaveStart(mxc_rtc_freq_sel_t fq)
{
#if TARGET_NUM != 32675
    MXC_GPIO_Config(&gpio_cfg_rtcsqw);
    return MXC_RTC_RevA_SquareWave((mxc_rtc_reva_regs_t *)MXC_RTC, MXC_RTC_REVA_SQUARE_WAVE_ENABLED,
                                   fq);
#else
    return E_NOT_SUPPORTED;
#endif
}

int MXC_RTC_SquareWaveStop(void)
{
#if TARGET_NUM != 32675
    return MXC_RTC_RevA_SquareWave((mxc_rtc_reva_regs_t *)MXC_RTC,
                                   MXC_RTC_REVA_SQUARE_WAVE_DISABLED, 0);
#else
    return E_NOT_SUPPORTED;
#endif
}

int MXC_RTC_Trim(int8_t trm)
{
    return MXC_RTC_RevA_Trim((mxc_rtc_reva_regs_t *)MXC_RTC, trm);
}

int MXC_RTC_GetFlags(void)
{
    return MXC_RTC_RevA_GetFlags((mxc_rtc_reva_regs_t *)MXC_RTC);
}

int MXC_RTC_ClearFlags(int flags)
{
    return MXC_RTC_RevA_ClearFlags((mxc_rtc_reva_regs_t *)MXC_RTC, flags);
}

int MXC_RTC_GetSubSecond(void)
{
    return MXC_RTC_RevA_GetSubSecond((mxc_rtc_reva_regs_t *)MXC_RTC);
}

int MXC_RTC_GetSecond(void)
{
    return MXC_RTC_RevA_GetSecond((mxc_rtc_reva_regs_t *)MXC_RTC);
}

int MXC_RTC_GetTime(uint32_t *sec, uint32_t *subsec)
{
    return MXC_RTC_RevA_GetTime((mxc_rtc_reva_regs_t *)MXC_RTC, sec, subsec);
}

int MXC_RTC_GetBusyFlag(void)
{
    return MXC_RTC_RevA_GetBusyFlag((mxc_rtc_reva_regs_t *)MXC_RTC);
}

int MXC_RTC_TrimCrystal(mxc_tmr_regs_t *tmr)
{
    if (MXC_TMR_GET_IDX(tmr) < 0 ||
        MXC_TMR_GET_IDX(tmr) > 4) { // Timer must support ERFO as clock source
        return E_BAD_PARAM;
    }

    mxc_tmr_cfg_t
        tmr_cfg; // Configure timer to trigger each interrupt NUM_PERIOD number of times within a second
    tmr_cfg.pres = TMR_PRES_1;
    tmr_cfg.mode = TMR_MODE_CONTINUOUS;
    tmr_cfg.bitMode = TMR_BIT_MODE_32;
    tmr_cfg.clock = MXC_TMR_32M_CLK;
    tmr_cfg.cmp_cnt = ERFO_FREQ / MXC_RTC_REVA_TRIM_PERIODS;
    tmr_cfg.pol = 0;
    MXC_TMR_Init(tmr, &tmr_cfg, false);

    return MXC_RTC_RevA_TrimCrystal((mxc_rtc_reva_regs_t *)MXC_RTC, tmr);
}
