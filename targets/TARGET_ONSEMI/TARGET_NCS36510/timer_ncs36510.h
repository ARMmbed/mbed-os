/**
 ******************************************************************************
 * @file timer.h
 * @brief (API) Public header of Timer driver
 * @internal
 * @author   ON Semiconductor
 * $Rev: 3725 $
 * $Date: 2015-09-14 14:36:27 +0530 (Mon, 14 Sep 2015) $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup timer
 *
 * @details
 *
 * <h1> General description </h1>
 * <p>
 * The APB Timer module is a 16-bit down counter with a selectable <b>prescaler</b>.
 * <b>Prescaler</b> values can be selected from 1 to 1024.
 * (<b>prescaler</b> extends the range of the timer at the expense of precision)
 * The Timer provides two modes of operation being <b>free running</b> and <b>periodic</b>.
 * In <b>free running</b> mode, when the counter reaches zero it is decremented to 0xFFFF
 * and no interrupt is generated.
 * In <b>periodic</b>, when the counter reaches zero it is decremented to load value
 * and an interruption is generated.
 * </p>
 *
 */

#ifndef TIMER_H_
#define TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "driver.h"
#include "us_ticker_api.h"
#include "clock.h"
#include "timer_map.h"
#include "types.h"
#include "cmsis_nvic.h"

/* Miscellaneous I/O and control operations codes */
#define TIMER_IOCTL_GET_LOAD    1 /**< <b>Ioctl request code</b>: Getting load value. */
#define TIMER_IOCTL_SET_LOAD    2 /**< <b>Ioctl request code</b>: Seting load value. */
#define TIMER_IOCTL_GET_VALUE   3 /**< <b>Ioctl request code</b>: Getting current timer value. */

/* Timer control bits */
#define TIMER_ENABLE_BIT        0x1
#define TIMER_PRESCALE_BIT_POS  0x2
#define TIMER_MODE_BIT_POS      0x6
#define TIMER_ENABLE_BIT_POS    0x7

/* Options defines */
// TODO (MIV): put this in an enumerated value
typedef enum {
    CLK_DIVIDER_1     = 0,
    CLK_DIVIDER_2     = 3,
    CLK_DIVIDER_8     = 4,
    CLK_DIVIDER_16    = 1,
    CLK_DIVIDER_32    = 5,
    CLK_DIVIDER_128   = 6,
    CLK_DIVIDER_256   = 2,
    CLK_DIVIDER_1024  = 7
} ClockDivider;

#define TIME_MODE_FREE_RUNNING    0x0
#define TIME_MODE_PERIODIC        0x1

typedef  void (*timer_irq_handlers_t)(void) ;

/** Options to be passed when opening a timer device instance.*/
typedef struct timer_options {
    TimerReg_pt membase;    /**< Memory base for the device's registers. */
    uint8_t irq;            /**< IRQ number of the IRQ associated to the device. */
    boolean mode;           /**< Timer mode:
                                   * - 0 = Free Run mode (no interrupt generation)
                                   * <b> # timer duration = (65535 + 1) * prescaler  * peripheral clock (PCLK) period </b>
                                   * - 1 = Periodic mode (interrupt generation)
                                   * <b> # timer duration = (load  + 1) * prescaler  * peripheral clock (PCLK) period </b> */
    uint8_t prescale;    /**< Timer prescaler: from 1 to 1024.
                                   * - CLK_DIVIDER_1 = clock not divided
                                   * - CLK_DIVIDER_2 = clock is divided by 2
                                   * - CLK_DIVIDER_8 = clock is divided by 8
                                   * - CLK_DIVIDER_16 = clock is divided by 16
                                   * - CLK_DIVIDER_32 = clock is divided by 32
                                   * - CLK_DIVIDER_128 = clock is divided by 128
                                   * - CLK_DIVIDER_256 = clock is divided by 256
                                   * - CLK_DIVIDER_1024 = clock is divided by 1024 */
    uint16_t load;        /**< Timer load: from 0 to 65535. */
    timer_irq_handlers_t handler; /**< Timer handler or call-back */
} timer_options_t, *timer_options_pt;

/** Interrupt handler for timer devices; to be called from an actual ISR.
 * @param membase Memory base for the device's registers
 */
void fTimerHandler(TimerReg_pt membase);

extern void us_timer_isr(void);
extern void us_ticker_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H_ */
