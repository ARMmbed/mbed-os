/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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

#include "sleep_api.h"
#include "cmsis.h"
#include "pwrman_regs.h"
#include "pwrseq_regs.h"
#include "clkman_regs.h"
#include "ioman_regs.h"
#include "rtc_regs.h"
#include "wait_api.h"

#define REVISION_A3 2
#define REVISION_A4 3

static mxc_uart_regs_t *stdio_uart = (mxc_uart_regs_t*)STDIO_UART;

void sleep(void)
{
    // Normal sleep mode for ARM core
    SCB->SCR = 0;

    __DSB();
    __WFI();
}

// Low-power stop mode
void deepsleep(void)
{
    /* Deep Sleep is not yet supported. */
    sleep();

#if 0
    unsigned int part_rev = MXC_PWRMAN->mask_id0 & MXC_F_PWRMAN_MASK_ID0_REVISION_ID;

    // Deep sleep is not working properly on Revisions A3 and earlier
    if (part_rev <= REVISION_A3) {
        sleep();
        return;
    }

    // Wait for all STDIO characters to be sent. The UART clock will stop.
    while ( (((stdio_uart->tx_fifo_ctrl & MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY)
                >> MXC_F_UART_TX_FIFO_CTRL_FIFO_ENTRY_POS) > 0) || 
                (!(stdio_uart->intfl & MXC_F_UART_INTFL_TX_DONE)) );

    __disable_irq();

    // Make sure RTC is not pending before sleeping, if its still synchronizing
    // we might not wake up.
    while (MXC_RTCTMR->ctrl & MXC_F_RTC_CTRL_PENDING);

    // Clear any active GPIO Wake Up Events
    if (MXC_PWRSEQ->flags & MXC_F_PWRSEQ_FLAGS_PWR_IOWAKEUP) {
        // NOTE: These must be cleared before clearing IOWAKEUP
        MXC_PWRSEQ->reg1 |= MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH;
        MXC_PWRSEQ->reg1 &= ~MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH;

        MXC_PWRSEQ->flags |= MXC_F_PWRSEQ_FLAGS_PWR_IOWAKEUP;
    }

    // Set the LP1 select bit so CPU goes to LP1 during SLEEPDEEP
    if (part_rev == REVISION_A4) {
        MXC_PWRSEQ->reg0 &= ~MXC_F_PWRSEQ_REG0_PWR_LP1; // A4 requires part to go to pseudo LP0
    } else {
        MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_LP1;
    }

    // The SLEEPDEEP bit will cause a WFE() to trigger LP0/LP1 (depending on ..._REG0_PWR_LP1 state)
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    if (part_rev == REVISION_A4) {
        // WORKAROUND: Toggle SVM bits, which send extra clocks to the power sequencer to fix retention controller
        MXC_PWRSEQ->msk_flags |= MXC_F_PWRSEQ_MSK_FLAGS_PWR_TVDD12_RST_BAD;
        MXC_PWRSEQ->reg0 &= ~(MXC_F_PWRSEQ_REG0_PWR_SVMTVDD12EN_RUN);
        MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_SVMTVDD12EN_RUN;
        MXC_PWRSEQ->msk_flags &= ~(MXC_F_PWRSEQ_MSK_FLAGS_PWR_TVDD12_RST_BAD);
    }

    // Enable Retention controller
    MXC_PWRSEQ->retn_ctrl0 |= MXC_F_PWRSEQ_RETN_CTRL0_RETN_CTRL_EN;

    // Freeze GPIO using MBUS so that it doesn't flail while digital core is asleep
    MXC_PWRSEQ->reg1 |= MXC_F_PWRSEQ_REG1_PWR_MBUS_GATE;

    // Dummy read to make sure SSB writes are complete
    if (MXC_PWRSEQ->reg0 & MXC_F_PWRSEQ_REG0_PWR_SYS_REBOOT) {}

    if (part_rev == REVISION_A4) {
        // Note: ARM deep-sleep requires a specific sequence to clear event latches,
        // otherwise the CPU will not enter sleep.
        __SEV();
        __WFE();
        __WFI();
    }
    else {
        // Note: ARM deep-sleep requires a specific sequence to clear event latches,
        //  otherwise the CPU will not enter sleep.
        __SEV();
        __WFE();
        __WFE();
    }

    // We'll wakeup here ...

    // Unfreeze the GPIO by clearing MBUS_GATE
    MXC_PWRSEQ->reg1 &= ~(MXC_F_PWRSEQ_REG1_PWR_MBUS_GATE);

    // Clear power sequencer event flags (write-1-to-clear)
    // RTC and GPIO flags are cleared in their interrupts handlers
    // NOTE: We are ignoring all of these potential wake up types
    MXC_PWRSEQ->flags = (MXC_F_PWRSEQ_FLAGS_PWR_POWER_FAIL |
                         MXC_F_PWRSEQ_FLAGS_PWR_BOOT_FAIL |
                         MXC_F_PWRSEQ_FLAGS_PWR_FLASH_DISCHARGE |
                         MXC_F_PWRSEQ_FLAGS_PWR_VDD12_RST_BAD |
                         MXC_F_PWRSEQ_FLAGS_PWR_VDD18_RST_BAD |
                         MXC_F_PWRSEQ_FLAGS_PWR_VRTC_RST_BAD |
                         MXC_F_PWRSEQ_FLAGS_PWR_VDDB_RST_BAD |
                         MXC_F_PWRSEQ_FLAGS_PWR_TVDD12_RST_BAD |
                         MXC_F_PWRSEQ_FLAGS_PWR_POR18Z_FAIL_LATCH |
                         MXC_F_PWRSEQ_FLAGS_PWR_USB_PLUG_WAKEUP |
                         MXC_F_PWRSEQ_FLAGS_PWR_USB_REMOVE_WAKEUP |
                         MXC_F_PWRSEQ_FLAGS_PWR_TVDD12_BAD);

    __enable_irq();
#endif
}
