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
 *
 *************************************************************************** */

/**
 * @file mxc_sys.c
 * @brief      System layer driver.
 * @details    This driver is used to control the system layer of the device.
 */

/* **** Includes **** */
#include <stddef.h>
#include "mxc_errors.h"
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "gcr_regs.h"
#include "fcr_regs.h"

/**
 * @ingroup mxc_sys
 * @{
 */

/* **** Definitions **** */
#define MXC_SYS_CLOCK_TIMEOUT       MXC_DELAY_MSEC(1)

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int MXC_SYS_IsClockEnabled (mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 for the pclkdis1 register. */
    if (clock > 31) {
        clock -= 32;
        return ! (MXC_GCR->pclk_dis1 & (0x1 << clock));
    }
    else {
        return ! (MXC_GCR->pclk_dis0 & (0x1 << clock));
    }
}

/* ************************************************************************** */
void MXC_SYS_ClockDisable (mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 for the pclkdis1 register. */
    if (clock > 31) {
        clock -= 32;
        MXC_GCR->pclk_dis1 |= (0x1 << clock);
    }
    else {
        MXC_GCR->pclk_dis0 |= (0x1 << clock);
    }
}

/* ************************************************************************** */
void MXC_SYS_ClockEnable (mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 for the pclkdis1 register. */
    if (clock > 31) {
        clock -= 32;
        MXC_GCR->pclk_dis1 &= ~ (0x1 << clock);
    }
    else {
        MXC_GCR->pclk_dis0 &= ~ (0x1 << clock);
    }
}
/* ************************************************************************** */
void MXC_SYS_RTCClockEnable()
{
    MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_X32K_EN;
}

/* ************************************************************************** */
int MXC_SYS_RTCClockDisable (void)
{
    /* Check that the RTC is not the system clock source */
    if ((MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_CLKSEL) != MXC_S_GCR_CLK_CTRL_CLKSEL_HFXIN) {
        MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_X32K_EN;
        return E_NO_ERROR;
    }
    else {
        return E_BAD_STATE;
    }
}

/******************************************************************************/
int MXC_SYS_ClockSourceEnable (mxc_sys_system_clock_t clock)
{
    switch (clock) {
    case MXC_SYS_CLOCK_HIRC:
        MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_HIRC_EN;
        return MXC_SYS_Clock_Timeout (MXC_F_GCR_CLK_CTRL_HIRC_RDY);
        break;
        
    case MXC_SYS_CLOCK_HFXIN:
        MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_X32K_EN;
        return MXC_SYS_Clock_Timeout (MXC_F_GCR_CLK_CTRL_X32K_RDY);
        break;
        
    case MXC_SYS_CLOCK_NANORING:
        // MXC_GCR->clk_ctrl |= MXC_F_GCR_CLKCTRL_EXTCLK_EN;
        // return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_EXTCLK_RDY);
        return E_NOT_SUPPORTED;
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }
}

/******************************************************************************/
int MXC_SYS_ClockSourceDisable (mxc_sys_system_clock_t clock)
{
    uint32_t current_clock;
    
    current_clock = MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_CLKSEL;
    
    // Don't turn off the clock we're running on
    if (clock == current_clock) {
        return E_BAD_PARAM;
    }
    
    switch (clock) {
    case MXC_SYS_CLOCK_HIRC:
        MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_HIRC_EN;
        break;
        
    case MXC_SYS_CLOCK_HFXIN:
        MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLK_CTRL_X32K_EN;
        break;
        
    case MXC_SYS_CLOCK_NANORING:
        // MXC_GCR->clk_ctrl &= ~MXC_F_GCR_CLKCTRL_EXTCLK_EN;
        return E_BAD_PARAM;
        
    default:
        return E_BAD_PARAM;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int MXC_SYS_Clock_Timeout (uint32_t ready)
{
    // Start timeout, wait for ready
    MXC_DelayAsync (MXC_SYS_CLOCK_TIMEOUT, NULL);
    
    do {
        if (MXC_GCR->clk_ctrl & ready) {
            MXC_DelayAbort();
            return E_NO_ERROR;
        }
    }
    while (MXC_DelayCheck() == E_BUSY);
    
    return E_TIME_OUT;
}

/* ************************************************************************** */
int MXC_SYS_Clock_Select (mxc_sys_system_clock_t clock)
{
    uint32_t current_clock;
    
    // Save the current system clock
    current_clock = MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_CLKSEL;
    
    switch (clock) {
    case MXC_SYS_CLOCK_HIRC:
    
        // Enable HIRC clock
        if (! (MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_HIRC_EN)) {
        
            MXC_GCR->clk_ctrl |= MXC_F_GCR_CLK_CTRL_HIRC_EN;
            
            // Check if HIRC clock is ready
            if (MXC_SYS_Clock_Timeout (MXC_F_GCR_CLK_CTRL_HIRC_RDY) != E_NO_ERROR) {
                return E_TIME_OUT;
            }
        }
        
        // Set HIRC clock as System Clock
        MXC_SETFIELD (MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_CLKSEL, MXC_S_GCR_CLK_CTRL_CLKSEL_HIRC);
        
        break;
        
    case MXC_SYS_CLOCK_HFXIN:
    
        // Enable HFXtal clock
        if (! (MXC_GCR->clk_ctrl & MXC_F_GCR_CLK_CTRL_X32K_EN)) {
            MXC_GCR->clk_ctrl |=MXC_F_GCR_CLK_CTRL_X32K_EN;
            
            // Check if HFXtal clock is ready
            if (MXC_SYS_Clock_Timeout (MXC_F_GCR_CLK_CTRL_X32K_RDY) != E_NO_ERROR) {
                return E_TIME_OUT;
            }
        }
        
        // Set HFXtal clock as System Clock
        MXC_SETFIELD (MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_CLKSEL, MXC_S_GCR_CLK_CTRL_CLKSEL_HFXIN);
        
        break;
        
    case MXC_SYS_CLOCK_NANORING:
        // Enable HIRC clock
        // if(!(MXC_GCR->clk_ctrl & MXC_F_GCR_CLKCTRL_EXTCLK_EN)) {
        //     MXC_GCR->clk_ctrl |=MXC_F_GCR_CLKCTRL_EXTCLK_EN;
        
        //     // Check if HIRC clock is ready
        //     if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_EXTCLK_RDY) != E_NO_ERROR) {
        //         return E_TIME_OUT;
        //     }
        // }
        
        // Set HIRC clock as System Clock
        // MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK);
        MXC_SETFIELD(MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_CLKSEL, MXC_S_GCR_CLK_CTRL_CLKSEL_NANORING);
        
        break;
        
    default:
        return E_BAD_PARAM;
    }
    
    // Wait for system clock to be ready
    if (MXC_SYS_Clock_Timeout (MXC_F_GCR_CLK_CTRL_CLKRDY) != E_NO_ERROR) {
    
        // Restore the old system clock if timeout
        MXC_SETFIELD (MXC_GCR->clk_ctrl, MXC_F_GCR_CLK_CTRL_CLKSEL, current_clock);
        
        return E_TIME_OUT;
    }
    
    // Update the system core clock
    SystemCoreClockUpdate();
    
    return E_NO_ERROR;
}


/* ************************************************************************** */
void MXC_SYS_Reset_Periph (mxc_sys_reset_t reset)
{
    /* The mxc_sys_reset_t enum uses enum values that are the offset by 32 for the rstr1 register. */
    if (reset > 31) {
        reset -= 32;
        MXC_GCR->rst1 = (0x1 << reset);
    }
    else {
        MXC_GCR->rst0 = (0x1 << reset);
    }
}
/**@} end of mxc_sys */

