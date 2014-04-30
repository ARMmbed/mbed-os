/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_wdog_hal.h"

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*******************************************************************************
 * Variables
 *******************************************************************************/

/*! 
 * @brief Watchdog internal config buffer to handle control and
 *  configure register's write-once-only character.
 *  Reset value of WDOG_STCTRLH is 0x1D3 
 */
static volatile uint32_t s_wdogSTCTRLH = 0x1D3;

/*******************************************************************************
 * Code
 *******************************************************************************/

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_enable 
 * Description   : Enable watchdog module. Should be called after all 
 * necessary configure have been set.
 * This function is used to enable the WDOG and must be called after all 
 * necessary configure have been set.
 *
 *END*********************************************************************/
void wdog_hal_enable(void)
{
    s_wdogSTCTRLH |= BM_WDOG_STCTRLH_WDOGEN;
    HW_WDOG_STCTRLH_WR(s_wdogSTCTRLH);
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_disable 
 * Description   : Disable watchdog module.
 * This function is used to disable the WDOG.
 *
 *END*********************************************************************/
void wdog_hal_disable(void)
{
    s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_WDOGEN;
    HW_WDOG_STCTRLH_WR(s_wdogSTCTRLH);
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_configure_interrupt 
 * Description   : Enable and disable watchdog interrupt.
 * This function is used to configure the WDOG interrupt.
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_configure_interrupt(bool isEnabled)
{
    if (isEnabled)
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_IRQRSTEN;
    }
    else
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_IRQRSTEN;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_set_clock_source 
 * Description   : set watchdog clock Source
 * This function is used to set the WDOG clock source, there are two clock sources can be used,
 * one is LPO clock and the other is bus clock.
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_set_clock_source(wdog_clock_source_t clockSource)
{
    if (kWdogDedicatedClock == clockSource)
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_CLKSRC;
    }
    else
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_CLKSRC;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_configure_window_mode 
 * Description   : Enable and disable watchdog window mode
 * This function is used to configure the WDOG window mode.
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_configure_window_mode(bool isEnabled)
{
    if (isEnabled)
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_WINEN;
    }
    else
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_WINEN;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_configure_register_update 
 * Description   : Enable and disable watchdog write-once-only register update
 * This function is used to configure the WDOG register update feature, if disable means that
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_configure_register_update(bool isEnabled)
{
    if (isEnabled)
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_ALLOWUPDATE;
    }
    else
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_ALLOWUPDATE;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_configure_enabled_in_cpu_debug_mode 
 * Description   : Set whether watchdog is working while cpu is in debug mode
 * This function is used to configure whether the WDOG is enabled in CPU debug mode. 
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_configure_enabled_in_cpu_debug_mode(bool isEnabled)
{
    if (isEnabled)
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_DBGEN;
    }
    else
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_DBGEN;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_configure_enabled_in_cpu_stop_mode 
 * Description   : Set whether watchdog is working while cpu is in stop mode
 * This function is used to configure whether the WDOG is enabled in CPU stop mode. 
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_configure_enabled_in_cpu_stop_mode(bool isEnabled)
{
    if (isEnabled)
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_STOPEN;
    }
    else
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_STOPEN;
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name : wdog_hal_configure_enabled_in_cpu_wait_mode 
 * Description   : Set whether watchdog is working while cpu is in wait mode
 * This function is used to configure whether the WDOG is enabled in CPU wait mode. 
 * Configure is saved in internal configure buffer and write back to 
 * register in wdog_hal_enable function, so this function must be 
 * called before wdog_hal_enable is called.
 *
 *END*********************************************************************/
void wdog_hal_configure_enabled_in_cpu_wait_mode(bool isEnabled)
{
    if (isEnabled)
    {
        s_wdogSTCTRLH |= BM_WDOG_STCTRLH_WAITEN;
    }
    else
    {
        s_wdogSTCTRLH &= ~BM_WDOG_STCTRLH_WAITEN;
    }
}

/*******************************************************************************
 * EOF
 *******************************************************************************/

