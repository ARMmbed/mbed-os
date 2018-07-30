/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#if DEVICE_LPTICKER
#include "lp_ticker_api.h"
#include "mtu2_iobitmask.h"
#include "mbed_drv_cfg.h"
#include "mtu2.h"

#if (LP_TICKER_MTU2_CH == 2)
 #define LP_TICKER_TIMER_IRQn (TGI2A_IRQn)
 #define MTU2TCR              (MTU2TCR_2)
 #define MTU2TCNT             (MTU2TCNT_2)
 #define MTU2TIER             (MTU2TIER_2)
 #define MTU2TGRA             (MTU2TGRA_2)
 #define MTU2TSR              (MTU2TSR_2)
 #define MTU2_TSTR_CST        MTU2_TSTR_CST2
 #define MTU2_TCR_TPSC        (0x07)
#elif (LP_TICKER_MTU2_CH == 3)
 #define LP_TICKER_TIMER_IRQn (TGI3A_IRQn)
 #define MTU2TCR              (MTU2TCR_3)
 #define MTU2TCNT             (MTU2TCNT_3)
 #define MTU2TIER             (MTU2TIER_3)
 #define MTU2TGRA             (MTU2TGRA_3)
 #define MTU2TSR              (MTU2TSR_3)
 #define MTU2_TSTR_CST        MTU2_TSTR_CST3
 #define MTU2_TCR_TPSC        (0x05)
#elif (LP_TICKER_MTU2_CH == 4)
 #define LP_TICKER_TIMER_IRQn (TGI4A_IRQn)
 #define MTU2TCR              (MTU2TCR_4)
 #define MTU2TCNT             (MTU2TCNT_4)
 #define MTU2TIER             (MTU2TIER_4)
 #define MTU2TGRA             (MTU2TGRA_4)
 #define MTU2TSR              (MTU2TSR_4)
 #define MTU2_TSTR_CST        MTU2_TSTR_CST4
 #define MTU2_TCR_TPSC        (0x05)
#else
 #error "Invalid number : LP_TICKER_MTU2_CH (2-4)"
#endif

static int lp_ticker_inited = 0;

void lp_ticker_init(void)
{
    GIC_DisableIRQ(LP_TICKER_TIMER_IRQn);
    GIC_ClearPendingIRQ(LP_TICKER_TIMER_IRQn);

    /* Power Control for Peripherals      */
    mtu2_init();

    if (lp_ticker_inited) return;
    lp_ticker_inited = 1;

    MTU2TCR   = MTU2_TCR_TPSC;
    MTU2TSTR  |= MTU2_TSTR_CST;
    MTU2TIER  |= MTU2_TIER_n_TGIEA;

    // INTC settings
    InterruptHandlerRegister(LP_TICKER_TIMER_IRQn, (void (*)(uint32_t))lp_ticker_irq_handler);
    GIC_SetPriority(LP_TICKER_TIMER_IRQn, 5);
    GIC_SetConfiguration(LP_TICKER_TIMER_IRQn, 3);
}

void lp_ticker_free(void)
{
    GIC_DisableIRQ(LP_TICKER_TIMER_IRQn);
    GIC_ClearPendingIRQ(LP_TICKER_TIMER_IRQn);

    MTU2TIER  &= ~MTU2_TIER_n_TGIEA;
    lp_ticker_inited = 0;
    mtu2_free();
}

uint32_t lp_ticker_read()
{
    return (uint32_t)MTU2TCNT;
}

void lp_ticker_set_interrupt(timestamp_t timestamp)
{
    MTU2TSR = (MTU2TSR & 0xFE);
    MTU2TGRA = (uint16_t)timestamp;
    GIC_EnableIRQ(LP_TICKER_TIMER_IRQn);
}

void lp_ticker_fire_interrupt(void)
{
    GIC_SetPendingIRQ(LP_TICKER_TIMER_IRQn);
    GIC_EnableIRQ(LP_TICKER_TIMER_IRQn);
}

void lp_ticker_disable_interrupt(void)
{
    GIC_DisableIRQ(LP_TICKER_TIMER_IRQn);
}

void lp_ticker_clear_interrupt(void)
{
    MTU2TSR = (MTU2TSR & 0xFE);
    GIC_ClearPendingIRQ(LP_TICKER_TIMER_IRQn);
}

const ticker_info_t* lp_ticker_get_info()
{
    static const ticker_info_t info = {
        (uint32_t)((float)RENESAS_RZ_A1_P0_CLK / 1024.0f + 0.5f),
        16
    };
    return &info;
}

#endif // DEVICE_LPTICKER
