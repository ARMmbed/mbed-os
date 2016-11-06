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

#include "max32620.h"
#include "clkman_regs.h"
#include "icc_regs.h"
#include "pwrseq_regs.h"
#include "pwrman_regs.h"
#include "adc_regs.h"
#include "flc_regs.h"
#include "trim_regs.h"
#include "rtc_regs.h"

/* SCB CPACR Register Definitions */
/* Note: Added by Maxim Integrated, as these are missing from CMSIS/Core/Include/core_cm4.h */
#define SCB_CPACR_CP10_Pos      20                              /* SCB CPACR: Coprocessor 10 Position */
#define SCB_CPACR_CP10_Msk      (0x3UL << SCB_CPACR_CP10_Pos)   /* SCB CPACR: Coprocessor 10 Mask */
#define SCB_CPACR_CP11_Pos      22                              /* SCB CPACR: Coprocessor 11 Position */
#define SCB_CPACR_CP11_Msk      (0x3UL << SCB_CPACR_CP11_Pos)   /* SCB CPACR: Coprocessor 11 Mask */

static uint8_t running;

// NOTE: Setting the CMSIS SystemCoreClock value to the actual value it will
// be AFTER SystemInit() runs.  This is required so the hal drivers will have
// the correct value when the DATA sections are initialized.
uint32_t SystemCoreClock = RO_FREQ / 2;

void SystemCoreClockUpdate(void)
{
    switch ((MXC_CLKMAN->clk_ctrl & MXC_F_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT) >> MXC_F_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT_POS) {

        case MXC_V_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT_96MHZ_RO_DIV_2:
        default:
            SystemCoreClock = RO_FREQ / 2;
            break;
        case MXC_V_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT_96MHZ_RO:
            SystemCoreClock = RO_FREQ;
            break;
    }
}

void Trim_ROAtomic(void)
{
    uint32_t trim;

    // Step 1: enable 32KHz RTC
    running = MXC_PWRSEQ->reg0 & MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;
    MXC_PWRSEQ->reg0 |= MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;

    // Step 2: enable RO calibration complete interrupt
    MXC_ADC->intr = (MXC_ADC->intr & 0xFFFF) | MXC_F_ADC_INTR_RO_CAL_DONE_IE;

    // Step 3: clear RO calibration complete interrupt
    MXC_ADC->intr = (MXC_ADC->intr & 0xFFFF) | MXC_F_ADC_INTR_RO_CAL_DONE_IF;

    /* Step 4: -- NO LONGER NEEDED / HANDLED BY STARTUP CODE -- */

    /* Step 5: write initial trim to frequency calibration initial condition register */
    trim = (MXC_PWRSEQ->reg6 & MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF) >> MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF_POS;
    MXC_ADC->ro_cal1 = (MXC_ADC->ro_cal1 & ~MXC_F_ADC_RO_CAL1_TRM_INIT) |
                       ((trim << MXC_F_ADC_RO_CAL1_TRM_INIT_POS) & MXC_F_ADC_RO_CAL1_TRM_INIT);

    // Step 6: load initial trim to active frequency trim register
    MXC_ADC->ro_cal0 |= MXC_F_ADC_RO_CAL0_RO_CAL_LOAD;

    // Step 7: enable frequency loop to control RO trim
    MXC_ADC->ro_cal0 |= MXC_F_ADC_RO_CAL0_RO_CAL_EN;

    // Step 8: run frequency calibration in atomic mode
    MXC_ADC->ro_cal0 |= MXC_F_ADC_RO_CAL0_RO_CAL_ATOMIC;

    // Step 9: waiting for ro_cal_done flag
    while (!(MXC_ADC->intr & MXC_F_ADC_INTR_RO_CAL_DONE_IF));

    // Step 10: stop frequency calibration
    MXC_ADC->ro_cal0 &= ~MXC_F_ADC_RO_CAL0_RO_CAL_RUN;

    // Step 11: disable RO calibration complete interrupt
    MXC_ADC->intr = (MXC_ADC->intr & 0xFFFF) & ~MXC_F_ADC_INTR_RO_CAL_DONE_IE;

    // Step 12: read final frequency trim value
    trim = (MXC_ADC->ro_cal0 & MXC_F_ADC_RO_CAL0_RO_TRM) >> MXC_F_ADC_RO_CAL0_RO_TRM_POS;

    /* Step 13: write final trim to RO flash trim shadow register */
    MXC_PWRSEQ->reg6 = (MXC_PWRSEQ->reg6 & ~MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF) |
                       ((trim << MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF_POS) & MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF);

    // Step 14: restore RTC status
    if (!running) {
        MXC_PWRSEQ->reg0 &= ~MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN;
    }

    // Step 15: disable frequency loop to control RO trim
    MXC_ADC->ro_cal0 &= ~MXC_F_ADC_RO_CAL0_RO_CAL_EN;
}

static void ICC_Enable(void)
{
    /* Invalidate cache and wait until ready */
    MXC_ICC->invdt_all = 1;
    while (!(MXC_ICC->ctrl_stat & MXC_F_ICC_CTRL_STAT_READY));

    /* Enable cache */
    MXC_ICC->ctrl_stat |= MXC_F_ICC_CTRL_STAT_ENABLE;

    /* Must invalidate a second time for proper use */
    MXC_ICC->invdt_all = 1;
}

// This function to be implemented by the hal
extern void low_level_init(void);

// Note: This is called before C run-time initialization. Do not use any initialized variables.
void SystemInit(void)
{
    ICC_Enable();

    low_level_init();

    // Select 48MHz ring oscillator as clock source
    uint32_t reg = MXC_CLKMAN->clk_ctrl;
    reg &= ~MXC_F_CLKMAN_CLK_CTRL_SYSTEM_SOURCE_SELECT;
    MXC_CLKMAN->clk_ctrl = reg;

    // Copy trim information from shadow registers into power manager registers
    // NOTE: Checks have been added to prevent bad/missing trim values from being loaded
    if ((MXC_FLC->ctrl & MXC_F_FLC_CTRL_INFO_BLOCK_VALID) &&
            (MXC_TRIM->for_pwr_reg5 != 0xffffffff) &&
            (MXC_TRIM->for_pwr_reg6 != 0xffffffff)) {
        MXC_PWRSEQ->reg5 = MXC_TRIM->for_pwr_reg5;
        MXC_PWRSEQ->reg6 = MXC_TRIM->for_pwr_reg6;
    } else {
        /* No valid info block, use some reasonable defaults */
        MXC_PWRSEQ->reg6 &= ~MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF;
        MXC_PWRSEQ->reg6 |= (0x1e0 << MXC_F_PWRSEQ_REG6_PWR_TRIM_OSC_VREF_POS);
    }

    // Use ASYNC flags, and ASYNC Reset of flags to improve synchronization speed
    // between RTC and ARM core. Also avoid delayed RTC interrupts after lp wake.
    MXC_RTCTMR->ctrl |= (MXC_F_RTC_CTRL_USE_ASYNC_FLAGS | MXC_F_RTC_CTRL_AGGRESSIVE_RST);

    /* Clear the GPIO WUD event if not waking up from LP0 */
    /* this is necessary because WUD flops come up in undetermined state out of POR or SRST*/
    if (MXC_PWRSEQ->reg0 & MXC_F_PWRSEQ_REG0_PWR_FIRST_BOOT || !(MXC_PWRMAN->pwr_rst_ctrl & MXC_F_PWRMAN_PWR_RST_CTRL_POR)) {
        /* Clear GPIO WUD event and configuration registers, globally */
        MXC_PWRSEQ->reg1 |= (MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH | MXC_F_PWRSEQ_REG1_PWR_CLR_IO_CFG_LATCH);
        MXC_PWRSEQ->reg1 &= ~(MXC_F_PWRSEQ_REG1_PWR_CLR_IO_EVENT_LATCH | MXC_F_PWRSEQ_REG1_PWR_CLR_IO_CFG_LATCH);
    } else {
        /* Unfreeze the GPIO by clearing MBUS_GATE, when returning from LP0 */
        MXC_PWRSEQ->reg1 &= ~(MXC_F_PWRSEQ_REG1_PWR_MBUS_GATE);
    }

    // Turn on retention regulator
    MXC_PWRSEQ->reg0 |= (MXC_F_PWRSEQ_REG0_PWR_RETREGEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RETREGEN_SLP);

    // Clear all unused wakeup sources
    // Beware! Do not change any flag not mentioned here, as they will gate important power sequencer signals
    MXC_PWRSEQ->msk_flags &= ~(MXC_F_PWRSEQ_MSK_FLAGS_PWR_USB_PLUG_WAKEUP |
                               MXC_F_PWRSEQ_MSK_FLAGS_PWR_USB_REMOVE_WAKEUP);

    // RTC sources are inverted, so a 1 will disable them
    MXC_PWRSEQ->msk_flags |= (MXC_F_PWRSEQ_MSK_FLAGS_RTC_CMPR1 |
                              MXC_F_PWRSEQ_MSK_FLAGS_RTC_PRESCALE_CMP);

    /* Enable RTOS Mode: Enable 32kHz clock synchronizer to SysTick external clock input */
    MXC_CLKMAN->clk_ctrl |= MXC_F_CLKMAN_CLK_CTRL_RTOS_MODE;

    // Enable real-time clock during sleep mode
    MXC_PWRSEQ->reg0 |= (MXC_F_PWRSEQ_REG0_PWR_RTCEN_RUN | MXC_F_PWRSEQ_REG0_PWR_RTCEN_SLP);

#if (__FPU_PRESENT == 1)
    /* Enable FPU on Cortex-M4, which occupies coprocessor slots 10 & 11 */
    /* Grant full access, per "Table B3-24 CPACR bit assignments". */
    /* DDI0403D "ARMv7-M Architecture Reference Manual" */
    SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
    __DSB();
    __ISB();
#endif

    // Trim ring oscillator
    Trim_ROAtomic();
}
