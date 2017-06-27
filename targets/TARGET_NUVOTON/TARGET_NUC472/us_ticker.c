/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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


static void tmr0_vec(void);
static void tmr1_vec(void);
static void us_ticker_arm_cd(void);

static int us_ticker_inited = 0;
static volatile uint32_t counter_major = 0;
static volatile uint32_t cd_major_minor_us = 0;
static volatile uint32_t cd_minor_us = 0;

// NOTE: PCLK is set up in mbed_sdk_init(), invocation of which must be before C++ global object constructor. See init_api.c for details.
// NOTE: Choose clock source of timer:
//       1. HIRC: Be the most accurate but might cause unknown HardFault.
//       2. HXT: Less accurate and cannot pass mbed-drivers test.
//       3. PCLK(HXT): Less accurate but can pass mbed-drivers test.
// NOTE: TIMER_0 for normal counter, TIMER_1 for countdown.
static const struct nu_modinit_s timer0hires_modinit = {TIMER_0, TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK, 0, TMR0_RST, TMR0_IRQn, (void *) tmr0_vec};
static const struct nu_modinit_s timer1hires_modinit = {TIMER_1, TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_PCLK, 0, TMR1_RST, TMR1_IRQn, (void *) tmr1_vec};

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
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->CTL = TIMER_PERIODIC_MODE | prescale_timer0 | TIMER_CTL_CNTDATEN_Msk;
    ((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname))->CMP = cmp_timer0;
    
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

        // NOTE: As TIMER_CNT = TIMER_CMP and counter_major has increased by one, TIMER_CNT doesn't change to 0 for one tick time.
        // NOTE: As TIMER_CNT = TIMER_CMP or TIMER_CNT = 0, counter_major (ISR) may not sync with TIMER_CNT. So skip and fetch stable one at the cost of 1 clock delay on this read.
        do {
            core_util_critical_section_enter();
            
            // NOTE: Order of reading minor_us/carry here is significant.
            minor_us = TIMER_GetCounter(timer0_base) * US_PER_TMR0HIRES_CLK;
            uint32_t carry = (timer0_base->INTSTS & TIMER_INTSTS_TIF_Msk) ? 1 : 0;
            // When TIMER_CNT approaches TIMER_CMP and will wrap soon, we may get carry but TIMER_CNT not wrapped. Hanlde carefully carry == 1 && TIMER_CNT is near TIMER_CMP.
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

static void tmr0_vec(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer0hires_modinit.modname));
    counter_major ++;
}

static void tmr1_vec(void)
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
      
    // Reset 8-bit PSC counter, 24-bit up counter value and CNTEN bit
    timer1_base->CTL |= TIMER_CTL_RSTCNT_Msk;
    // One-shot mode, Clock = 1 MHz 
    uint32_t clk_timer1 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer1hires_modinit.modname));
    uint32_t prescale_timer1 = clk_timer1 / TMR1HIRES_CLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer1 != (uint32_t) -1) && prescale_timer1 <= 127);
    MBED_ASSERT((clk_timer1 % TMR1HIRES_CLK_PER_SEC) == 0);
    timer1_base->CTL &= ~(TIMER_CTL_OPMODE_Msk | TIMER_CTL_PSC_Msk | TIMER_CTL_CNTDATEN_Msk);
    timer1_base->CTL |= TIMER_ONESHOT_MODE | prescale_timer1 | TIMER_CTL_CNTDATEN_Msk;
    
    uint32_t cmp_timer1 = cd_minor_us / US_PER_TMR1HIRES_CLK;
    cmp_timer1 = NU_CLAMP(cmp_timer1, TMR_CMP_MIN, TMR_CMP_MAX);
    timer1_base->CMP = cmp_timer1;
    
    TIMER_EnableInt(timer1_base);
    TIMER_Start(timer1_base);
}
