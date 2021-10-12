/*******************************************************************************
 * Copyright (c) Maxim Integrated Products, Inc., All Rights Reserved.
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
#ifdef DEVICE_WATCHDOG

#include "watchdog_api.h"
#include "mbed_error.h"

#include "mxc_sys.h"
#include "wdt_regs.h"
#include "wdt.h"
#include "system_max32660.h"


watchdog_status_t hal_watchdog_init(const watchdog_config_t *config)
{
    int i;

    // WDT Disable
    MXC_WDT_Disable(MXC_WDT0);

    uint32_t number_of_tick = ( (float)config->timeout_ms * (float)PeripheralClock) /1000;

    // Find top bit index
    for (i=31; i >= 16; i--) {
        if ( number_of_tick & (1<<i) ) {  
            if (number_of_tick & ~(1<<i)) {
                i+=1; // round up if is there any more tick
            }
            break;
        }
    }

    if (i > 31) {
        i = 31; // max
    } else if (i < 16) {
        i = 16; // min
    }

    MXC_WDT_SetResetPeriod(MXC_WDT0, (mxc_wdt_period_t)(31-i) );
    
    hal_watchdog_kick();

    // WDT Enable RESET
    MXC_WDT_EnableReset(MXC_WDT0);
    
    // WDT Enable
    MXC_WDT_Enable(MXC_WDT0);  

    return WATCHDOG_STATUS_OK;
}

void hal_watchdog_kick(void)
{    
    MXC_WDT0->rst = 0x00A5;
    MXC_WDT0->rst = 0x005A;
}

watchdog_status_t hal_watchdog_stop(void)
{
    MXC_WDT_Disable(MXC_WDT0);

    return WATCHDOG_STATUS_OK;
}

uint32_t hal_watchdog_get_reload_value(void)
{
    uint32_t rst_period = (MXC_WDT0->ctrl & MXC_F_WDT_CTRL_RST_PERIOD) >> MXC_F_WDT_CTRL_RST_PERIOD_POS;
    uint32_t val = (1<<(31-rst_period));

    return ( (float)val / (float)PeripheralClock) * 1000;
}

watchdog_features_t hal_watchdog_get_platform_features(void)
{
    uint32_t max_wdt_val = 1<<31; // max val
    watchdog_features_t features;

    features.max_timeout = ((float)max_wdt_val / (float)PeripheralClock)*1000;
    features.update_config = true;
    features.disable_watchdog = true;
    features.clock_typical_frequency = PeripheralClock;
    features.clock_max_frequency = PeripheralClock;

    return features;
}

#endif // DEVICE_WATCHDOG
