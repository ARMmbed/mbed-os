/*
 * serial_device.h
 *
 *  Created on: 25 jan 2019
 *      Author: Antonio O.
 */

#if DEVICE_SLEEP

#include "sleep_api.h"

void hal_sleep(void)
{
	//only CPU halt, wakeup from any interrupt source
	//wakeup timer is not available

    // Disable IRQs
    core_util_critical_section_enter();
    wakeupFromSleepFlag = 0;

    // Store the watchdog configuration and the disable it to avoid reset during CPU halt.
    uint32_t WDG_CR_saved = WDG->CR;
    WDG->CR = 0;

    // Wait for interrupt is called: the core execution is halted until an event occurs.
    __WFI();

    // Restore the watchdog functionality.
    WDG->CR= WDG_CR_saved;

    // Unmask all the interrupt
    core_util_critical_section_exit();
}

void hal_deepsleep(void)
{
//    /*  WORKAROUND:
//     *  MBED serial driver does not handle deepsleep lock
//     *  to prevent entering deepsleep until HW serial FIFO is empty.
//     *  This is tracked in mbed issue 4408.
//     *  For now, we're checking all Serial HW FIFO. If any transfer is ongoing
//     *  we're not entering deep sleep and returning immediately. */
//    if (serial_is_tx_ongoing()) {
//        return;
//    }
//
//    // Disable IRQs
//    core_util_critical_section_enter();
//
//    //I don't know if it is possible to implement deepsleep
//    core_util_critical_section_exit();
}

#endif //DEVICE_SLEEP
