/*******************************************************************************
 * mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "sleep_api.h"

#ifdef DEVICE_SLEEP

#include "adi_pwr.h"
#include "adi_pwr_def_v1.h"


/**
 * Function to put processor into sleep (FLEXI mode only).
 */
static void go_into_WFI(void)
{
	uint32_t savedPriority;
	uint16_t savedWDT;
	uint32_t scrSetBits = 0u;
	uint32_t scrClrBits = 0u;

	/* pre-calculate the sleep-on-exit set/clear bits, FLEXI mode only */
	scrSetBits |= SLEEPONEXIT_BIT;
	scrClrBits |= (SLEEPDEEP_BIT | SLEEPONEXIT_BIT);

	/* put all the power mode and system control mods inside a critical section */
	ADI_ENTER_CRITICAL_REGION();

    { /* these three lines must be in a success-checking loop if they are not inside critical section */
        /* Uninterruptable unlock sequence */
        pADI_PMG0->PWRKEY = ADI_PMG_KEY;

        /* Clear the previous mode and set new mode */
        pADI_PMG0->PWRMOD =(uint32_t) ( ( pADI_PMG0->PWRMOD & (uint32_t) (~BITM_PMG_PWRMOD_MODE) ) | PowerMode );
    }

    /* Update the SCR (sleepdeep and sleep-on-exit bits) */
    SCB->SCR = ((SCB->SCR | scrSetBits) & ~scrClrBits);

    /* save current Base Priority Level */
    savedPriority = __get_BASEPRI();

    /* NOTE: the watchdog timer (WDT) of the GlueMicro (ADuCM302x) is reset
       by the core hardware with every exit from low-power mode.  Therefore,
       even though we may have disabled it during startup, it will reset
       itself on exit from every hibernation state.  Therefore, to avoid
       unintended system resets every 30 seconds because of unexpected WDT
       timeouts, we save/restore the WDT control register around
       hibernation entry and exit.
    */

    /* save WDT control register */
    savedWDT = pADI_WDT0->CTL;

    /* Set caller's priority threshold (left-justified) */
    __set_BASEPRI(0);

    /* if we are in the software looping mode, loop on the user's flag until set */
	/* SAR-51938: insure WDT is fully synchronized or hibernate may lock out the sync bits */
	while ((pADI_WDT0->STAT & (BITM_WDT_STAT_COUNTING | BITM_WDT_STAT_LOADING | BITM_WDT_STAT_CLRIRQ)) != 0u)
	{
	}

	__DSB();  /* bus sync to insure register writes from interrupt handlers are always complete before WFI */

	/* NOTE: aggressive compiler optimizations can muck up critical timing here, so reduce if hangs are present */

	/* Wait for interrupt */
	__WFI();

	/* Recycle critical section so that interrupts are dispatched.  This
	 * allows *pbInterruptOccurred to be set during interrupt handling.
	 */
	ADI_EXIT_CRITICAL_REGION();
	/* nop */
	ADI_ENTER_CRITICAL_REGION();
    /* ...still within critical section... */

    /* Restore previous base priority */
    __set_BASEPRI(savedPriority);

    /* restore WDT control register */
    pADI_WDT0->CTL = savedWDT;

    /* clear sleep-on-exit bit to avoid sleeping on exception return to thread level */
    SCB->SCR &= ~SLEEPONEXIT_BIT;

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

    switch (eClk)
    {
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
    if (enable)
        pADI_CLKG0_CLK->CTL5 |= flag;
    else
        pADI_CLKG0_CLK->CTL5 &= (~flag);
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
void sleep(void)
{
    // set to go into the FLEXI mode where the processor is asleep and all peripherals are
    // still active
    go_into_WFI();
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
void deepsleep(void)
{
    // set clock gating to all the peripheral clocks
    set_clock_gating(PCLK, 1);

    // set to go into the hibernation mode
    go_into_WFI();

    // when exiting, clear all peripheral clock gating bits. This is done to enable clocks that aren't
    // automatically re-enabled out of sleep such as the GPIO clock.
    pADI_CLKG0_CLK->CTL5 = 0;
}

#endif  // #ifdef DEVICE_SLEEP
