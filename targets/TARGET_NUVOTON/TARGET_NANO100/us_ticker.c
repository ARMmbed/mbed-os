/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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
 */
 
#include "us_ticker_api.h"
#include "sleep_api.h"
#include "mbed_assert.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "mbed_critical.h"

// us_ticker tick = us = timestamp
#define US_PER_TICK             1
#define US_PER_SEC              (1000 * 1000)

#define TMR0HIRES_CLK_PER_SEC           (1000 * 1000)
#define TMR1HIRES_CLK_PER_SEC           (1000 * 1000)

#define US_PER_TMR0HIRES_CLK            (US_PER_SEC / TMR0HIRES_CLK_PER_SEC)
#define US_PER_TMR1HIRES_CLK            (US_PER_SEC / TMR1HIRES_CLK_PER_SEC)

#define US_PER_TMR0HIRES_INT            (1000 * 1000 * 10)
#define TMR0HIRES_CLK_PER_TMR0HIRES_INT ((uint32_t) ((uint64_t) US_PER_TMR0HIRES_INT * TMR0HIRES_CLK_PER_SEC / US_PER_SEC))


void TMR0_IRQHandler(void);
void TMR1_IRQHandler(void);
static void us_ticker_arm_cd(void);

static int us_ticker_inited = 0;
static volatile uint32_t counter_major = 0;
static volatile uint32_t cd_major_minor_us = 0;
static volatile uint32_t cd_minor_us = 0;

// NOTE: Choose clock source of timer:
//       1. HIRC: Be the most accurate but might cause unknown HardFault.
//       2. HXT: Less accurate and cannot pass mbed-drivers test.
// NOTE: TIMER_0 for normal counter, TIMER_1 for countdown.
static const struct nu_modinit_s timer0hires_modinit = {TIMER_0, TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0, TMR0_RST, TMR0_IRQn, (void *) TMR0_IRQHandler};
static const struct nu_modinit_s timer1hires_modinit = {TIMER_1, TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT, 0, TMR1_RST, TMR1_IRQn, (void *) TMR1_IRQHandler};

#define TMR_CMP_MIN         2
#define TMR_CMP_MAX         0xFFFFFFu

void us_ticker_init(void)
{
    if (us_ticker_inited) {
        return;
    }
    
    counter_major = 0;
    cd_major_minor_us = 0;
    cd_minor_us = 0;
    us_ticker_inited = 1;
    
    // Reset IP
    SYS_ResetModule(timer0hires_modinit.rsetidx);
    SYS_ResetModule(timer1hires_modinit.rsetidx);
    
    // Select IP clock source
    CLK_SetModuleClock(timer0hires_modinit.clkidx, timer0hires_modinit.clksrc, timer0hires_modinit.clkdiv);
    CLK_SetModuleClock(timer1hires_modinit.clkidx, timer1hires_modinit.clksrc, timer1hires_modinit.clkdiv);
    // Enable IP clock
    CLK_EnableModuleClock(timer0hires_modinit.clkidx);
    CLK_EnableModuleClock(timer1hires_modinit.clkidx);

    // Timer for normal counter
    uint32_t clk_timer0 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
    uint32_t prescale_timer0 = clk_timer0 / TMR0HIRES_CLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer0 != (uint32_t) -1) && prescale_timer0 <= 127);
    MBED_ASSERT((clk_timer0 % TMR0HIRES_CLK_PER_SEC) == 0);
    uint32_t cmp_timer0 = TMR0HIRES_CLK_PER_TMR0HIRES_INT;
    MBED_ASSERT(cmp_timer0 >= TMR_CMP_MIN && cmp_timer0 <= TMR_CMP_MAX);
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->CTL = TIMER_PERIODIC_MODE;
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->PRECNT = prescale_timer0;
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->CMPR = cmp_timer0;
    
    NVIC_SetVector(timer0hires_modinit.irq_n, (uint32_t) timer0hires_modinit.var);
    NVIC_SetVector(timer1hires_modinit.irq_n, (uint32_t) timer1hires_modinit.var);
    
    NVIC_EnableIRQ(timer0hires_modinit.irq_n);
    NVIC_EnableIRQ(timer1hires_modinit.irq_n);
    
    TIMER_EnableInt((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
    TIMER_Start((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
}

uint32_t us_ticker_read()
{
    if (! us_ticker_inited) {
        us_ticker_init();
    }
    
    TIMER_T * timer0_base = (TIMER_T *) NU_MODBASE(timer0hires_modinit.modname);
        
    do {
        uint32_t major_minor_us;
        uint32_t minor_us;

        // NOTE: As TIMER_DR = TIMER_CMPR and counter_major has increased by one, TIMER_DR doesn't change to 0 for one tick time.
        // NOTE: As TIMER_DR = TIMER_CMPR or TIMER_DR = 0, counter_major (ISR) may not sync with TIMER_DR. So skip and fetch stable one at the cost of 1 clock delay on this read.
        do {
            core_util_critical_section_enter();
            
            // NOTE: Order of reading minor_us/carry here is significant.
            minor_us = TIMER_GetCounter(timer0_base) * US_PER_TMR0HIRES_CLK;
            uint32_t carry = (timer0_base->ISR & TIMER_ISR_TMR_IS_Msk) ? 1 : 0;
            // When TIMER_DR approaches TIMER_CMPR and will wrap soon, we may get carry but TIMER_DR not wrapped. Hanlde carefully carry == 1 && TIMER_DR is near TIMER_CMPR.
            if (carry && minor_us > (US_PER_TMR0HIRES_INT / 2)) {
                major_minor_us = (counter_major + 1) * US_PER_TMR0HIRES_INT;
            }
            else {
                major_minor_us = (counter_major + carry) * US_PER_TMR0HIRES_INT + minor_us;
            }
            
            core_util_critical_section_exit();
        }
        while (minor_us == 0 || minor_us == US_PER_TMR0HIRES_INT);
        
        return (major_minor_us / US_PER_TICK);
    }
    while (0);
}

void us_ticker_disable_interrupt(void)
{
    TIMER_DisableInt((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
}

void us_ticker_clear_interrupt(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    TIMER_Stop((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
    
    uint32_t delta = timestamp - us_ticker_read();
    cd_major_minor_us = delta * US_PER_TICK;
    us_ticker_arm_cd();
}

void us_ticker_fire_interrupt(void)
{
    cd_major_minor_us = cd_minor_us = 0;
    NVIC_SetPendingIRQ(timer1hires_modinit.irq_n);
}

void TMR0_IRQHandler(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
    counter_major ++;
}

void TMR1_IRQHandler(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
    cd_major_minor_us = (cd_major_minor_us > cd_minor_us) ? (cd_major_minor_us - cd_minor_us) : 0;
    if (cd_major_minor_us == 0) {
        // NOTE: us_ticker_set_interrupt() may get called in us_ticker_irq_handler();
        us_ticker_irq_handler();
    }
    else {
        us_ticker_arm_cd();
    }
}

static void us_ticker_arm_cd(void)
{
    TIMER_T * timer1_base = (TIMER_T *) NU_MODBASE(timer1hires_modinit.modname);
    
    cd_minor_us = cd_major_minor_us;

    // Reset Timer's pre-scale counter, internal 24-bit up-counter and TMR_CTL [TMR_EN] bit
    timer1_base->CTL |= TIMER_CTL_SW_RST_Msk;
    // One-shot mode, Clock = 1 MHz 
    uint32_t clk_timer1 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
    uint32_t prescale_timer1 = clk_timer1 / TMR1HIRES_CLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer1 != (uint32_t) -1) && prescale_timer1 <= 127);
    MBED_ASSERT((clk_timer1 % TMR1HIRES_CLK_PER_SEC) == 0);
    timer1_base->CTL &= ~TIMER_CTL_MODE_SEL_Msk;
    timer1_base->CTL |= TIMER_ONESHOT_MODE;
    timer1_base->PRECNT = prescale_timer1;
    
    uint32_t cmp_timer1 = cd_minor_us / US_PER_TMR1HIRES_CLK;
    cmp_timer1 = NU_CLAMP(cmp_timer1, TMR_CMP_MIN, TMR_CMP_MAX);
    timer1_base->CMPR = cmp_timer1;
    
    TIMER_EnableInt(timer1_base);
    TIMER_Start(timer1_base);
}
