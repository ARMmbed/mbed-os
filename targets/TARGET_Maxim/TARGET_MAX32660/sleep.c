/*******************************************************************************
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
 *******************************************************************************
 */

#include <stdio.h>
#include "sleep_api.h"
#include "lp.h"
#include "uart.h"

#if 0
static void prep_for_sleep(void)
{
    while(MXC_UART_ReadyForSleep(MXC_UART1) != E_NO_ERROR) {
        ;
    }
    while(MXC_UART_ReadyForSleep(MXC_UART0) != E_NO_ERROR) {
        ;
    }
}
#else
#define prep_for_sleep()    
#endif

//******************************************************************************
void hal_sleep(void)
{
    prep_for_sleep();

    /*  Call MXC_LP_DisableBlockDetect function if the system runs from 
        a single supply only and VCORE is not connected to an external supply. 
        Bypassing the hardware detection of an external supply on VCORE 
        enables a faster wakeup time.
    */
    MXC_LP_DisableBlockDetect();
    
    /* Switch to sleep mode */
    MXC_LP_EnterSleepMode();
}

void hal_deepsleep(void)
{
#if 0
    prep_for_sleep();

    //MXC_LP_DisableBandGap();
    //MXC_LP_DisableVCorePORSignal();
    //MXC_LP_EnableRamRetReg();
    
    MXC_LP_EnableFastWk();
    MXC_LP_ClearWakeStatus();
    
    MXC_LP_DisableBlockDetect();
    MXC_LP_EnterDeepSleepMode();
#else
    hal_sleep();
#endif
}
