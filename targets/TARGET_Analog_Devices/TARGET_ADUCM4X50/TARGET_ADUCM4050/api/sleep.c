/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "sleep_api.h"

#ifdef DEVICE_SLEEP

#include "adi_pwr.h"
#include "adi_pwr_def.h"
#include "adi_rtos_map.h"
#include "adi_ADuCM4050_device.h"
#include "sleep.h"

/**
 * Function to put processor into sleep (FLEXI mode only).
 */
static void go_into_WFI(const ADI_PWR_POWER_MODE PowerMode)
{
    uint32_t savedPriority;
    uint16_t savedWDT;
    uint16_t ActiveWDT;
    uint32_t scrSetBits = 0u;
    uint32_t scrClrBits = 0u;
    uint32_t IntStatus = 0u;

    /* pre-calculate the sleep-on-exit set/clear bits */
    scrSetBits |= SCB_SCR_SLEEPONEXIT_Msk;

    /* wfi without deepsleep or sleep-on-exit */
    scrClrBits |= (uint32_t)(BITM_NVIC_INTCON0_SLEEPDEEP | BITM_NVIC_INTCON0_SLEEPONEXIT);

    ADI_ENTER_CRITICAL_REGION();

    { /* these lines must be in a success-checking loop if they are not inside critical section */
        /* Uninterruptable unlock sequence */
        pADI_PMG0->PWRKEY = ADI_PMG_KEY;

        /* Clear the previous mode and set new mode */
        pADI_PMG0->PWRMOD = (uint32_t) ( ( pADI_PMG0->PWRMOD & (uint32_t) (~BITM_PMG_PWRMOD_MODE) ) | PowerMode );
    }

    /* Update the SCR (sleepdeep and sleep-on-exit bits) */
    SCB->SCR = ((SCB->SCR | scrSetBits) & ~scrClrBits);

    /* save/restore current Base Priority Level */
    savedPriority = __get_BASEPRI();

    /* assert caller's priority threshold (left-justified), currently set to 0, i.e. disable interrupt masking */
    __set_BASEPRI(0);

    /* save/restore WDT control register (which is not retained during hibernation) */
    savedWDT = pADI_WDT0->CTL;

    /* optimization: compute local WDT enable flag once (outside the loop) */
    ActiveWDT = ((savedWDT & BITM_WDT_CTL_EN) >> BITP_WDT_CTL_EN);

    /* SAR-51938: insure WDT is fully synchronized or looping on interrupts
       in hibernate mode may lock out the sync bits.

       In hibernate mode (during which the WDT registers are not retained),
       the WDT registers will have been reset to default values after each
       interrupt exit and we require a WDT clock domain sync.

       We also need to insure a clock domain sync before (re)entering the WFI
       in case an interrupt did a watchdog kick.

       Optimization: only incur WDT sync overhead (~100us) if the WDT is enabled.
    */
    if (ActiveWDT > 0u) {
        while ((pADI_WDT0->STAT & (uint32_t)(BITM_WDT_STAT_COUNTING | BITM_WDT_STAT_LOADING | BITM_WDT_STAT_CLRIRQ)) != 0u) {
            ;
        }
    }

    __DSB();  /* bus sync to insure register writes from interrupt handlers are always complete before WFI */

    /* NOTE: aggressive compiler optimizations can muck up critical timing here, so reduce if hangs are present */

    /* The WFI loop MUST reside in a critical section because we need to insure that the interrupt
       that is planned to take us out of WFI (via a call to adi_pwr_ExitLowPowerMode()) is not
       dispatched until we get into the WFI.  If that interrupt sneaks in prior to our getting to the
       WFI, then we may end up waiting (potentially forever) for an interrupt that has already occurred.
    */
    __WFI();

    /* Recycle the critical section so that other (non-wakeup) interrupts are dispatched.
       This allows *pnInterruptOccurred to be set from any interrupt context.
     */
    ADI_EXIT_CRITICAL_REGION();
    /* nop */
    ADI_ENTER_CRITICAL_REGION();

    /* ...still within critical section... */

    /* Restore previous base priority */
    __set_BASEPRI(savedPriority);

    /* 	conditionally, restore WDT control register.
    	avoid unnecessary WDT writes which will invoke a sync problem
    	described above as SAR-51938: going into hibernation with pending,
    	unsynchronized WDT writes may lock out the sync bits.

    	Note: it takes over 1000us to sync WDT writes between the 26MHz and
    	32kHz clock	domains, so this write may actually impact the NEXT
    	low-power entry.
    */
    if (ActiveWDT > 0u) {
        pADI_WDT0->CTL = savedWDT;
    }

    /* clear sleep-on-exit bit to avoid sleeping on exception return to thread level */
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;

    __DSB(); /* bus sync before re-enabling interrupts */

    ADI_EXIT_CRITICAL_REGION();
}


/**
 * Function to enable/disable clock gating for the available clocks.
 * PCLK overrides all the other clocks.
 */
void set_clock_gating(peripheral_clk_t eClk, int enable)
{
    uint32_t flag;

    switch (eClk) {
        case PCLK:
            flag = 1 << BITP_CLKG_CLK_CTL5_PERCLKOFF;
            break;
        case GPT0_CLOCK:
            flag = 1 << BITP_CLKG_CLK_CTL5_GPTCLK0OFF;
            break;
        case GPT1_CLOCK:
            flag = 1 << BITP_CLKG_CLK_CTL5_GPTCLK1OFF;
            break;
        case GPT2_CLOCK:
            flag = 1 << BITP_CLKG_CLK_CTL5_GPTCLK2OFF;
            break;
        case I2C_CLOCK:
            flag = 1 << BITP_CLKG_CLK_CTL5_UCLKI2COFF;
            break;
        case GPIO_CLOCK:
            flag = 1 << BITP_CLKG_CLK_CTL5_GPIOCLKOFF;
            break;
        case TIMER_RGB_CLOCK:
            flag = 1 << BITP_CLKG_CLK_CTL5_TMRRGBCLKOFF;
            break;
        default:
            return;
    }

    // if enable, set the bit otherwise clear the bit
    if (enable) {
        pADI_CLKG0_CLK->CTL5 |= flag;
    } else {
        pADI_CLKG0_CLK->CTL5 &= (~flag);
    }
}



/** Send the microcontroller to sleep
 *
 * The processor is setup ready for sleep, and sent to sleep using __WFI(). In this mode, the
 * system clock to the core is stopped until a reset or an interrupt occurs. This eliminates
 * dynamic power used by the processor, memory systems and buses. The processor, peripheral and
 * memory state are maintained, and the peripherals continue to work and can generate interrupts.
 *
 * The processor can be woken up by any internal peripheral interrupt or external pin interrupt.
 *
 * @note
 * The mbed interface semihosting is disconnected as part of going to sleep, and can not be restored.
 * Flash re-programming and the USB serial port will remain active, but the mbed program will no longer be
 * able to access the LocalFileSystem
 *
 * This mode puts the processor into FLEXI mode however the peripheral clocks are not gated
 * hence they are still active.
 */
void hal_sleep(void)
{
    // set to go into the FLEXI mode where the processor is asleep and all peripherals are
    // still active
    go_into_WFI(ADI_PWR_MODE_FLEXI);
}


/** Send the microcontroller to deep sleep
 *
 * This processor is setup ready for deep sleep, and sent to sleep using __WFI(). This mode
 * has the same sleep features as sleep plus it powers down peripherals and clocks. All state
 * is still maintained.
 *
 * The processor can only be woken up by an external interrupt on a pin or a watchdog timer.
 *
 * @note
 *  The mbed interface semihosting is disconnected as part of going to sleep, and can not be restored.
 * Flash re-programming and the USB serial port will remain active, but the mbed program will no longer be
 * able to access the LocalFileSystem
 *
 * This mode puts the processor into FLEXI mode and all the peripheral clocks are clock gated
 * hence they are inactive until interrupts are generated in which case the processor is awaken
 * from sleep.
 */
void hal_deepsleep(void)
{
    // set clock gating to all the peripheral clocks
    set_clock_gating(PCLK, 1);

    // set to go into the FLEXI mode with peripheral clocks gated.
    go_into_WFI(ADI_PWR_MODE_FLEXI);

    // when exiting, clear all peripheral clock gating bits. This is done to enable clocks that aren't
    // automatically re-enabled out of sleep such as the GPIO clock.
    pADI_CLKG0_CLK->CTL5 = 0;
}

#endif  // #ifdef DEVICE_SLEEP
