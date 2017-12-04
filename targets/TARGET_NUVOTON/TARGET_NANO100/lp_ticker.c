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
 
#include "lp_ticker_api.h"

#if DEVICE_LOWPOWERTIMER

#include "sleep_api.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "mbed_critical.h"
#include "mbed_wait_api.h"

// lp_ticker tick = us = timestamp
#define US_PER_TICK             (1)
#define US_PER_SEC              (1000 * 1000)

#define US_PER_TMR2_INT         (US_PER_SEC * 10)
#define TMR2_CLK_PER_SEC        (__LXT)
#define TMR2_CLK_PER_TMR2_INT   ((uint32_t) ((uint64_t) US_PER_TMR2_INT * TMR2_CLK_PER_SEC / US_PER_SEC))
#define TMR3_CLK_PER_SEC        (__LXT)

void TMR2_IRQHandler(void);
void TMR3_IRQHandler(void);
static void lp_ticker_arm_cd(void);

static int lp_ticker_inited = 0;
static volatile uint32_t counter_major = 0;
static volatile uint32_t cd_major_minor_clks = 0;
static volatile uint32_t cd_minor_clks = 0;
static volatile uint32_t wakeup_tick = (uint32_t) -1;

// NOTE: To wake the system from power down mode, timer clock source must be ether LXT or LIRC.
// NOTE: TIMER_2 for normal counting and TIMER_3 for scheduled wakeup
static const struct nu_modinit_s timer2_modinit = {TIMER_2, TMR2_MODULE, CLK_CLKSEL2_TMR2_S_LXT, 0, TMR2_RST, TMR2_IRQn, (void *) TMR2_IRQHandler};
static const struct nu_modinit_s timer3_modinit = {TIMER_3, TMR3_MODULE, CLK_CLKSEL2_TMR3_S_LXT, 0, TMR3_RST, TMR3_IRQn, (void *) TMR3_IRQHandler};

#define TMR_CMP_MIN         2
#define TMR_CMP_MAX         0xFFFFFFu

void lp_ticker_init(void)
{
    if (lp_ticker_inited) {
        return;
    }
    lp_ticker_inited = 1;
    
    counter_major = 0;
    cd_major_minor_clks = 0;
    cd_minor_clks = 0;
    wakeup_tick = (uint32_t) -1;

    // Reset module
    SYS_ResetModule(timer2_modinit.rsetidx);
    SYS_ResetModule(timer3_modinit.rsetidx);
    
    // Select IP clock source
    CLK_SetModuleClock(timer2_modinit.clkidx, timer2_modinit.clksrc, timer2_modinit.clkdiv);
    CLK_SetModuleClock(timer3_modinit.clkidx, timer3_modinit.clksrc, timer3_modinit.clkdiv);
    // Enable IP clock
    CLK_EnableModuleClock(timer2_modinit.clkidx);
    CLK_EnableModuleClock(timer3_modinit.clkidx);

    // Configure clock
    uint32_t clk_timer2 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    uint32_t prescale_timer2 = clk_timer2 / TMR2_CLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer2 != (uint32_t) -1) && prescale_timer2 <= 127);
    MBED_ASSERT((clk_timer2 % TMR2_CLK_PER_SEC) == 0);
    uint32_t cmp_timer2 = TMR2_CLK_PER_TMR2_INT;
    MBED_ASSERT(cmp_timer2 >= TMR_CMP_MIN && cmp_timer2 <= TMR_CMP_MAX);
    // Continuous mode
    ((TIMER_T *) NU_MODBASE(timer2_modinit.modname))->CTL = TIMER_PERIODIC_MODE;
    ((TIMER_T *) NU_MODBASE(timer2_modinit.modname))->PRECNT = prescale_timer2;
    ((TIMER_T *) NU_MODBASE(timer2_modinit.modname))->CMPR = cmp_timer2;
    
    // Set vector
    NVIC_SetVector(timer2_modinit.irq_n, (uint32_t) timer2_modinit.var);
    NVIC_SetVector(timer3_modinit.irq_n, (uint32_t) timer3_modinit.var);
    
    NVIC_EnableIRQ(timer2_modinit.irq_n);
    NVIC_EnableIRQ(timer3_modinit.irq_n);
    
    TIMER_EnableInt((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    TIMER_EnableWakeup((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    
    // NOTE: TIMER_Start() first and then lp_ticker_set_interrupt(); otherwise, we may get stuck in lp_ticker_read() because
    //       timer is not running.

    // Wait 3 cycles of engine clock to ensure previous CTL write action is finish
    nu_nop(SystemCoreClock / __LXT * 3);
    // Start timer
    TIMER_Start((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    
    // Schedule wakeup to match semantics of lp_ticker_get_compare_match()
    lp_ticker_set_interrupt(wakeup_tick);
}

timestamp_t lp_ticker_read()
{    
    if (! lp_ticker_inited) {
        lp_ticker_init();
    }
    
    TIMER_T * timer2_base = (TIMER_T *) NU_MODBASE(timer2_modinit.modname);
    
    do {
        uint64_t major_minor_clks;
        uint32_t minor_clks;
        
        // NOTE: As TIMER_DR = TIMER_CMPR and counter_major has increased by one, TIMER_DR doesn't change to 0 for one tick time.
        // NOTE: As TIMER_DR = TIMER_CMPR or TIMER_DR = 0, counter_major (ISR) may not sync with TIMER_DR. So skip and fetch stable one at the cost of 1 clock delay on this read.
        do {
            core_util_critical_section_enter();
        
            // NOTE: Order of reading minor_us/carry here is significant.
            minor_clks = TIMER_GetCounter(timer2_base);
            uint32_t carry = (timer2_base->ISR & TIMER_ISR_TMR_IS_Msk) ? 1 : 0;
            // When TIMER_DR approaches TIMER_CMPR and will wrap soon, we may get carry but TIMER_DR not wrapped. Hanlde carefully carry == 1 && TIMER_DR is near TIMER_CMPR.
            if (carry && minor_clks > (TMR2_CLK_PER_TMR2_INT / 2)) {
                major_minor_clks = (counter_major + 1) * TMR2_CLK_PER_TMR2_INT;
            }
            else {
                major_minor_clks = (counter_major + carry) * TMR2_CLK_PER_TMR2_INT + minor_clks;
            }
            
            core_util_critical_section_exit();
        }
        while (minor_clks == 0 || minor_clks == TMR2_CLK_PER_TMR2_INT);

        // Add power-down compensation
        return ((uint64_t) major_minor_clks * US_PER_SEC / TMR2_CLK_PER_SEC / US_PER_TICK);
    }
    while (0);
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    uint32_t delta = timestamp - lp_ticker_read();
    wakeup_tick = timestamp;
    
    TIMER_Stop((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    cd_major_minor_clks = (uint64_t) delta * US_PER_TICK * TMR3_CLK_PER_SEC / US_PER_SEC;
    lp_ticker_arm_cd();

}

void lp_ticker_fire_interrupt(void)
{
    cd_major_minor_clks = cd_minor_clks = 0;
    /**
     * This event was in the past. Set the interrupt as pending, but don't process it here.
     * This prevents a recurive loop under heavy load which can lead to a stack overflow.
     */  
    NVIC_SetPendingIRQ(timer3_modinit.irq_n);
}

void lp_ticker_disable_interrupt(void)
{
    TIMER_DisableInt((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
}

void lp_ticker_clear_interrupt(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
}

void TMR2_IRQHandler(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    TIMER_ClearWakeupFlag((TIMER_T *) NU_MODBASE(timer2_modinit.modname));
    counter_major ++;
}

void TMR3_IRQHandler(void)
{
    TIMER_ClearIntFlag((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    TIMER_ClearWakeupFlag((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    cd_major_minor_clks = (cd_major_minor_clks > cd_minor_clks) ? (cd_major_minor_clks - cd_minor_clks) : 0;
    if (cd_major_minor_clks == 0) {
        // NOTE: lp_ticker_set_interrupt() may get called in lp_ticker_irq_handler();
        lp_ticker_irq_handler();
    }
    else {
        lp_ticker_arm_cd();
    }
}

static void lp_ticker_arm_cd(void)
{
    TIMER_T * timer3_base = (TIMER_T *) NU_MODBASE(timer3_modinit.modname);
    
    // Reset Timer's pre-scale counter, internal 24-bit up-counter and TMR_CTL [TMR_EN] bit
    timer3_base->CTL |= TIMER_CTL_SW_RST_Msk;
    // One-shot mode, Clock = 1 KHz 
    uint32_t clk_timer3 = TIMER_GetModuleClock((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    uint32_t prescale_timer3 = clk_timer3 / TMR3_CLK_PER_SEC - 1;
    MBED_ASSERT((prescale_timer3 != (uint32_t) -1) && prescale_timer3 <= 127);
    MBED_ASSERT((clk_timer3 % TMR3_CLK_PER_SEC) == 0);
    uint32_t ctl_timer3 = timer3_base->CTL;
    ctl_timer3 &= ~TIMER_CTL_MODE_SEL_Msk;
    ctl_timer3 |= TIMER_ONESHOT_MODE;
    timer3_base->PRECNT = prescale_timer3;
    
    cd_minor_clks = cd_major_minor_clks;
    cd_minor_clks = NU_CLAMP(cd_minor_clks, TMR_CMP_MIN, TMR_CMP_MAX);
    timer3_base->CMPR = cd_minor_clks;
    
    TIMER_EnableInt(timer3_base);
    TIMER_EnableWakeup((TIMER_T *) NU_MODBASE(timer3_modinit.modname));
    // Wait 2 cycles of engine clock to ensure previous CTL write action is finish
    wait_us(30 * 2);
    timer3_base->CTL |= ctl_timer3 | TIMER_CTL_TMR_EN_Msk;
}
#endif
