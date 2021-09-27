/* ****************************************************************************
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

#include <stddef.h>
#include "mxc_device.h"
#include "rtc.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "gpio_regs.h"
#include "mxc_errors.h"
#include "rtc_reva.h"

#if TARGET_NUM == 32650
    #include "pwrseq_regs.h"
#endif

int MXC_RTC_CheckBusy(void)
{
    // Time-out transfer if it takes > BUSY_TIMEOUT microseconds
    MXC_DelayAsync(MXC_DELAY_USEC(MXC_BUSY_TIMEOUT), NULL);

    while (MXC_RTC_REVA_IS_BUSY) {
        if (MXC_DelayCheck() != E_BUSY) {
            return E_BUSY;
        }
    }

    MXC_DelayAbort();
    return E_SUCCESS;
}

int MXC_RTC_RevA_EnableInt (mxc_rtc_reva_regs_t *rtc, uint32_t mask)
{
    mask &= (MXC_RTC_INT_EN_LONG |  MXC_RTC_INT_EN_SHORT | MXC_RTC_INT_EN_READY);
    
    if (!mask) {
        /* No bits set? Wasn't something we can enable. */
        return E_BAD_PARAM;
    }
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl |= mask;
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_DisableInt (mxc_rtc_reva_regs_t *rtc, uint32_t mask)
{
    mask &= (MXC_RTC_INT_EN_LONG |  MXC_RTC_INT_EN_SHORT | MXC_RTC_INT_EN_READY);
    
    if (!mask) {
        /* No bits set? Wasn't something we can enable. */
        return E_BAD_PARAM;
    }
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl &= ~mask;
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_SetTimeofdayAlarm (mxc_rtc_reva_regs_t *rtc, uint32_t ras)
{
    // ras can only be written if BUSY = 0 & (RTCE = 0 or ADE = 0);
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->toda = (ras << MXC_F_RTC_REVA_TODA_TOD_ALARM_POS) & MXC_F_RTC_REVA_TODA_TOD_ALARM;
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_SetSubsecondAlarm (mxc_rtc_reva_regs_t *rtc, uint32_t rssa)
{
    // ras can only be written if BUSY = 0 & (RTCE = 0 or ASE = 0);
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->sseca = (rssa << MXC_F_RTC_REVA_SSECA_SSEC_ALARM_POS) & MXC_F_RTC_REVA_SSECA_SSEC_ALARM;
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_Start (mxc_rtc_reva_regs_t *rtc)
{
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl |= MXC_F_RTC_REVA_CTRL_WR_EN;       // Allow writing to registers
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    // Can only write if WE=1 and BUSY=0
    rtc->ctrl |= MXC_F_RTC_REVA_CTRL_EN;    // setting RTCE = 1
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_WR_EN;       // Prevent Writing...
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_Stop (mxc_rtc_reva_regs_t *rtc)
{
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl |= MXC_F_RTC_REVA_CTRL_WR_EN;      // Allow writing to registers
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    // Can only write if WE=1 and BUSY=0
    rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_EN;  // setting RTCE = 0
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_WR_EN;       // Prevent Writing...
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_Init (mxc_rtc_reva_regs_t *rtc, uint32_t sec, uint8_t ssec)
{
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl = MXC_F_RTC_REVA_CTRL_WR_EN;        //  Allow Writes
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl = MXC_RTC_REVA_CTRL_RESET_DEFAULT;  // Start with a Clean Register
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl |= MXC_F_RTC_REVA_CTRL_WR_EN;      // Set Write Enable, allow writing to reg.
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ssec = ssec;
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->sec = sec;
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_WR_EN;       // Prevent Writing...
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_SquareWave (mxc_rtc_reva_regs_t *rtc, mxc_rtc_reva_sqwave_en_t sqe, mxc_rtc_freq_sel_t ft)
{
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl |= MXC_F_RTC_REVA_CTRL_WR_EN;   // Allow writing to registers
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    if (sqe == MXC_RTC_REVA_SQUARE_WAVE_ENABLED) {
        if (ft == MXC_RTC_F_32KHZ) {       // if 32KHz output is selected...
            rtc->oscctrl |= MXC_F_RTC_REVA_OSCCTRL_SQW_32K;   // Enable 32KHz wave
            
            if (MXC_RTC_CheckBusy()) {
                return E_BUSY;
            }
            
            rtc->ctrl |= MXC_F_RTC_REVA_CTRL_SQW_EN;                  // Enable output on the pin
        }
        else {                    // if 1Hz, 512Hz, 4KHz output is selected
        
            rtc->oscctrl &= ~MXC_F_RTC_REVA_OSCCTRL_SQW_32K;  // Must make sure that the 32KHz is disabled
            
            if (MXC_RTC_CheckBusy()) {
                return E_BUSY;
            }
            
            rtc->ctrl &= ~ MXC_F_RTC_REVA_CTRL_SQW_SEL;
            
            if (MXC_RTC_CheckBusy()) {
                return E_BUSY;
            }
            
            rtc->ctrl |= (MXC_F_RTC_REVA_CTRL_SQW_EN | ft);  // Enable Sq. wave,
        }
        
        if (MXC_RTC_CheckBusy()) {
            return E_BUSY;
        }
        
        rtc->ctrl |= MXC_F_RTC_REVA_CTRL_EN;     // Enable Real Time Clock
    }
    else {   // Turn off the square wave output on the pin
    
        rtc->oscctrl &= ~MXC_F_RTC_REVA_OSCCTRL_SQW_32K;     // Must make sure that the 32KHz is disabled
        
        if (MXC_RTC_CheckBusy()) {
            return E_BUSY;
        }
        
        rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_SQW_EN;  // No sq. wave output
    }
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_WR_EN;       // Disable Writing to register
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_Trim (mxc_rtc_reva_regs_t *rtc, int8_t trim)
{

    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl |= MXC_F_RTC_REVA_CTRL_WR_EN;
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    MXC_SETFIELD (rtc->trim, MXC_F_RTC_REVA_TRIM_TRIM, trim << MXC_F_RTC_REVA_TRIM_TRIM_POS);
    
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    rtc->ctrl &= ~MXC_F_RTC_REVA_CTRL_WR_EN;       // Disable Writing to register
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_GetFlags(void)
{
    return MXC_RTC->ctrl & (MXC_RTC_INT_FL_LONG | MXC_RTC_INT_FL_SHORT | MXC_RTC_INT_FL_READY);
}

int MXC_RTC_RevA_ClearFlags(int flags)
{
    if (MXC_RTC_CheckBusy()) {
        return E_BUSY;
    }
    
    MXC_RTC->ctrl &= ~(flags & (MXC_RTC_INT_FL_LONG | MXC_RTC_INT_FL_SHORT | MXC_RTC_INT_FL_READY));
    
    return E_SUCCESS;
}

int MXC_RTC_RevA_GetSubSecond(void)
{
#if TARGET_NUM == 32650
    int ssec;
    if(ChipRevision > 0xA1){
        ssec = ((MXC_PWRSEQ->ctrl >> 12)& 0xF00) | (MXC_RTC->ssec & 0xFF);
    }else{
        ssec = MXC_RTC->ssec;
    }
        return ssec;
#else
    return MXC_RTC->ssec;
#endif
}

int MXC_RTC_RevA_GetSecond(void)
{
    return MXC_RTC->sec;
}

int MXC_RTC_RevA_GetTime(uint32_t* sec, uint32_t* subsec)
{
    uint32_t temp_sec;
    
    if (sec == NULL || subsec == NULL) {
        return E_NULL_PTR;
    }
    
    do {
        // Check if an update is about to happen.
        if (!(MXC_RTC->ctrl & MXC_F_RTC_REVA_CTRL_RDY)) {
            return E_BUSY;
        }
        
        // Read the seconds count.
        temp_sec = MXC_RTC_RevA_GetSecond();
        
        // Check if an update is about to happen.
        if (!(MXC_RTC->ctrl & MXC_F_RTC_REVA_CTRL_RDY)) {
            return E_BUSY;
        }
        
        // Read the sub-seconds count.
        *subsec = MXC_RTC_RevA_GetSubSecond();
        
        // Check if an update is about to happen.
        if (!(MXC_RTC->ctrl & MXC_F_RTC_REVA_CTRL_RDY)) {
            return E_BUSY;
        }
        
        // Read the seconds count.
        *sec = MXC_RTC_RevA_GetSecond();
        
        // Repeat until a steady state is reached.
    }
    while (temp_sec != *sec);
    
    return E_NO_ERROR;
}
