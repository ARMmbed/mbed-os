/***************************************************************************//**
 * @file sleep.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_SLEEP

#include "sleep_api.h"
#include "sleepmodes.h"
#include "cmsis.h"
#include "em_emu.h"
#include "em_int.h"

uint32_t sleep_block_counter[NUM_SLEEP_MODES] = {0};

/**
 * Sleep mode.
 * Enter the lowest possible sleep mode that is not blocked by ongoing activity.
 */
void sleep(void)
{
    if (sleep_block_counter[0] > 0) {
        /* Blocked everything below EM0, so just return */
        return;
    } else if (sleep_block_counter[1] > 0) {
        /* Blocked everything below EM1, enter EM1 */
        EMU_EnterEM1();
    } else if (sleep_block_counter[2] > 0) {
        /* Blocked everything below EM2, enter EM2 */
        EMU_EnterEM2(true);
    } else {
        /* Blocked everything below EM3, enter EM3 */
        EMU_EnterEM3(true);
    } /* Never enter EM4, as mbed has no way of configuring EM4 wakeup */
    return;
}

/**
 * Deep Sleep mode.
 * Enter Energy Mode 2, turning off all high-frequency clocks.
 *
 * In EM2 the high frequency oscillator is turned off, but with the 32.768 kHz
 * oscillator running, selected low energy peripherals (LCD, RTC, LETIMER,
 * PCNT, LEUART, I2C, LESENSE, OPAMP, USB, WDOG and ACMP) are still
 * available. This gives a high degree of autonomous operation with a current
 * consumption as low as 1.1 μA with RTC enabled. Power-on Reset, Brown-out
 * Detection and full RAM and CPU retention is also included.
 */
void deepsleep(void)
{
    EMU_EnterEM2(true);
}

/** Block the microcontroller from sleeping below a certain mode
 *
 * This will block sleep() from entering an energy mode below the one given.
 * -- To be called by peripheral HAL's --
 *
 * After the peripheral is finished with the operation, it should call unblock with the same state
 *
 */
void blockSleepMode(sleepstate_enum minimumMode)
{
    INT_Disable();
    sleep_block_counter[minimumMode]++;
    INT_Enable();
}

/** Unblock the microcontroller from sleeping below a certain mode
 *
 * This will unblock sleep() from entering an energy mode below the one given.
 * -- To be called by peripheral HAL's --
 *
 * This should be called after all transactions on a peripheral are done.
 */
void unblockSleepMode(sleepstate_enum minimumMode)
{
    INT_Disable();
    if(sleep_block_counter[minimumMode] > 0) {
        sleep_block_counter[minimumMode]--;
    }
    INT_Enable();
}

#endif
