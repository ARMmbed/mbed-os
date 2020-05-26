/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <stdbool.h>
#include "s1sbp6a.h"
#include "s1sbp6a_type.h"
#include "s1sbp6a_cmu.h"

void bp6a_cmu_set_mcu_clock_gate_bypass(bool en)
{
    modifyreg32(&BP_SYSCON->MCU_CLK_GATE, CMU_MCU_CLK_GATE_BYPASS_MASK, CMU_MCU_CLK_GATE_BYPASS(en));
}

void bp6a_cmu_set_mcu_clock_src(cmu_src_clk_t clock)
{
    modifyreg32(&BP_SYSCON->MCU_CLK_CTRL, CMU_MCU_CLK_CTRL_SEL_MCU_SRC_MASK,
                CMU_MCU_CLK_CTRL_SEL_MCU_SRC(clock));
}

uint32_t bp6a_cmu_get_mcu_clock_src(void)
{
    return (cmu_src_clk_t)(getreg32(&BP_SYSCON->MCU_CLK_CTRL) &
                           CMU_MCU_CLK_CTRL_SEL_MCU_SRC_MASK);
}

uint32_t bp6a_get_mcu_src_clock_freq(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->MCU_CLK_CTRL) & CMU_MCU_CLK_CTRL_SEL_MCU_SRC_MASK;

    if (clk_ctrl == CMU_SRC_CLK_LSOSC) {
        return LSOSC_CLK_FREQ;
    } else if (clk_ctrl == CMU_SRC_CLK_EXT_4M) {
        return EXT_CLK_FREQ;
    } else if (clk_ctrl == CMU_SRC_CLK_HSOSC) {
        return HSOSC_CLK_FREQ;
    } else {
        return EXT_CLK_FREQ * CMU_PLL_AMP_GAIN;
    }
}

void bp6a_cmu_set_peri_clock_src(cmu_src_clk_t clock)
{
    modifyreg32(&BP_SYSCON->PERI_CLK_CTRL, CMU_PERI_CLK_CTRL_SEL_SRC_MASK,
                CMU_PERI_CLK_CTRL_SEL_SRC(clock));
}

uint32_t bp6a_cmu_get_peri_clock_src(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->PERI_CLK_CTRL) & CMU_PERI_CLK_CTRL_SEL_SRC_MASK;

    if (clk_ctrl == CMU_SRC_CLK_LSOSC) {
        return LSOSC_CLK_FREQ;
    } else if (clk_ctrl == CMU_SRC_CLK_EXT_4M) {
        return EXT_CLK_FREQ;
    } else if (clk_ctrl == CMU_SRC_CLK_HSOSC) {
        return HSOSC_CLK_FREQ;
    } else {
        return EXT_CLK_FREQ * CMU_PLL_AMP_GAIN;
    }
}

uint32_t bp6a_cmu_get_afe_con_clock_freq(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->AFE_CLK_CTRL) & CMU_MCU_CLK_CTRL_SEL_MCU_SRC_MASK;

    if (clk_ctrl == CMU_SRC_CLK_LSOSC) {
        return LSOSC_CLK_FREQ;
    } else if (clk_ctrl == CMU_SRC_CLK_HSOSC) {
        return HSOSC_CLK_FREQ;
    } else {
        return EXT_CLK_FREQ;
    }
}

void bp6a_set_muc_clock_div(uint32_t div)
{
    modifyreg32(&BP_SYSCON->MCU_CLK_CTRL,
                CMU_MCU_CLK_CTRL_AHBCLK_DIV_MASK |
                CMU_MCU_CLK_CTRL_APBCLK_DIV_MASK |
                CMU_MCU_CLK_CTRL_SRPCLK_DIV_MASK,
                CMU_MCU_CLK_CTRL_AHBCLK_DIV(div) |
                CMU_MCU_CLK_CTRL_APBCLK_DIV(div) |
                CMU_MCU_CLK_CTRL_SRPCLK_DIV(div));
}

void bp6a_cmu_peri_clock_div(uint32_t div)
{
    modifyreg32(&BP_SYSCON->PERI_CLK_CTRL, CMU_PERI_CLK_CTRL_CLK_DIV_MASK, CMU_PERI_CLK_CTRL_CLK_DIV(div));
}

uint32_t bp6a_cmu_get_pri_clock_div(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->PERI_CLK_CTRL);

    return ((clk_ctrl & CMU_PERI_CLK_CTRL_CLK_DIV_MASK) >> CMU_PERI_CLK_CTRL_CLK_DIV_SHIFT);
}

uint32_t bp6a_cmu_get_ahb_clock_div(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->MCU_CLK_CTRL);

    return ((clk_ctrl & CMU_MCU_CLK_CTRL_AHBCLK_DIV_MASK) & CMU_MCU_CLK_CTRL_AHBCLK_DIV_SHIFT);
}

uint32_t bp6a_cmu_get_apb_clock_div(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->MCU_CLK_CTRL);

    return ((clk_ctrl & CMU_MCU_CLK_CTRL_APBCLK_DIV_MASK) >> CMU_MCU_CLK_CTRL_APBCLK_DIV_SHIFT);
}

uint32_t bp6a_cmu_get_srp_clock_div(void)
{
    uint32_t clk_ctrl = 0;
    clk_ctrl = getreg32(&BP_SYSCON->MCU_CLK_CTRL);

    return ((clk_ctrl & CMU_MCU_CLK_CTRL_SRPCLK_DIV_MASK) >> CMU_MCU_CLK_CTRL_SRPCLK_DIV_SHIFT);
}

uint32_t bp6a_cmu_get_afe_smp_clock_div(void)
{
    uint32_t clk_ctrl = 0;

    clk_ctrl = getreg32(&BP_SYSCON->AFE_CLK_CTRL);

    return ((clk_ctrl & CMU_AFE_CLK_CTRL_SMP_CLK_DIV_MASK) >> CMU_AFE_CLK_CTRL_SMP_CLK_DIV_SHIFT);
}

int bp6a_cmu_enable_clock(cmu_clock_t clock, bool en)
{

    if ((clock < CMU_FCLK_AHBCLK) || (clock >= CMU_CLK_INVALID)) {
        return -1;
    }

    if (clock <= CMU_WATCHDOG_APBCLK) {
#ifdef CMU_CTRL_UDMA_C
        if (clock == CMU_UDMAC_AHBCLK) {
            if (en) {
                modifyreg32(&(BP_SYSCON->MCU_CLK_GATE), CMU_MCU_CLK_GATE_UDMAC_ACG_MASK, CMU_MCU_CLK_GATE_UDMAC_ACG(en));
                modifyreg32(&(BP_SYSCON->MCU_CLK_GATE), 0x01 << (clock - CMU_FCLK_AHBCLK), en << (clock - CMU_FCLK_AHBCLK));
            } else {
                modifyreg32(&(BP_SYSCON->MCU_CLK_GATE), 0x01 << (clock - CMU_FCLK_AHBCLK), en << (clock - CMU_FCLK_AHBCLK));
                modifyreg32(&(BP_SYSCON->MCU_CLK_GATE), CMU_MCU_CLK_GATE_UDMAC_ACG_MASK, CMU_MCU_CLK_GATE_UDMAC_ACG(en));

            }
        } else
#endif
            modifyreg32(&(BP_SYSCON->MCU_CLK_GATE), 0x01 << (clock - CMU_FCLK_AHBCLK), en << (clock - CMU_FCLK_AHBCLK));
    } else {
        modifyreg32(&(BP_SYSCON->PERI_CLK_GATE), 0x01 << (clock - CMU_UART0_CLK), en << (clock - CMU_UART0_CLK));
    }

    return 0;
}

int bp6a_cmu_set_peri_clock_mux(cmu_clock_t clock, cmu_mux_peri_clock_t mux)
{
    if ((clock < CMU_UART0_CLK) || (clock >= CMU_CLK_INVALID)) {
        return -1;
    }

    modifyreg32(&BP_SYSCON->PERI_CLK_MUX, 0x01 << (clock - CMU_UART0_CLK), mux << (clock - CMU_UART0_CLK));

    return 0;
}

cmu_mux_peri_clock_t CMU_GetClockMux(cmu_clock_t clock)
{
    uint32_t clk_mux = 0;

    clk_mux = getreg32(&BP_SYSCON->PERI_CLK_MUX);

    return (cmu_mux_peri_clock_t)((clk_mux >> (clock - CMU_UART0_CLK)) & 0x01);
}

void bp6a_cmu_set_smp_clock_src(cmu_sel_smp_clk_t clock)
{
    modifyreg32(&BP_SYSCON->AFE_CLK_CTRL, CMU_AFE_CLK_CTRL_SEL_AFECON32_MASK,
                CMU_AFE_CLK_CTRL_SEL_AFECON32(clock));
}

static void bp6a_cmu_set_hsosc_cal(uint8_t mode, uint8_t cal, uint8_t tol)
{
    modifyreg32(&BP_SYSCON->HSOSC_CTRL,
                CMU_HOSC_CTRL_MODE_MASK |
                CMU_HOSC_CTRL_CAL_MASK |
                CMU_HOSC_CTRL_TOL_MASK,
                CMU_HOSC_CTRL_MODE(mode) |
                CMU_HOSC_CTRL_CAL(cal) |
                CMU_HOSC_CTRL_TOL(tol));
}

static void bp6a_cmu_set_lsosc_cal(uint8_t mode, uint8_t cal, uint8_t tol)
{
    modifyreg32(&BP_SYSCON->LSOSC_CTRL,
                CMU_HOSC_CTRL_MODE_MASK |
                CMU_HOSC_CTRL_CAL_MASK |
                CMU_HOSC_CTRL_TOL_MASK,
                CMU_HOSC_CTRL_MODE(mode) |
                CMU_HOSC_CTRL_CAL(cal) |
                CMU_HOSC_CTRL_TOL(tol));
}

static uint32_t bp6a_cmu_get_clock_div(cmu_clock_t clock)
{
    uint32_t mux;
    switch (clock) {
        case CMU_SAMPLE_CLK_AFTER_DIV:
            return bp6a_cmu_get_afe_con_clock_freq();
        case CMU_SRPCLK_AFTER_DIV:
            return bp6a_cmu_get_srp_clock_div();
        case CMU_AHBCLK_AFTER_DIV:
            return bp6a_cmu_get_ahb_clock_div();
        case CMU_APBCLK_AFTER_DIV:
            return bp6a_cmu_get_apb_clock_div();
        case CMU_PERICLK_AFTER_DIV:
            return bp6a_cmu_get_pri_clock_div();
        case CMU_SRP_SRPCLK:
            return bp6a_cmu_get_srp_clock_div();
        case CMU_FCLK_AHBCLK:
        case CMU_GPIO_AHBCLK:
        case CMU_UDMAC_AHBCLK:
        case CMU_DTRNG_AHBCLK:
        case CMU_AES_AHBCLK:
        case CMU_SRC_AHBCLK:
        case CMU_QSPI_AHBCLK:
            return bp6a_cmu_get_ahb_clock_div();
        case CMU_TIMER0_APBCLK:
        case CMU_TIMER1_APBCLK:
        case CMU_TIMER2_APBCLK:
        case CMU_TIMER3_APBCLK:
        case CMU_TIMER4_APBCLK:
        case CMU_TIMER5_APBCLK:
        case CMU_TIMER6_APBCLK:
        case CMU_PWMTIMER_APBCLK:
        case CMU_DUALTIMER_APBCLK:
        case CMU_WATCHDOG_APBCLK:
            return bp6a_cmu_get_apb_clock_div();

        case CMU_UART0_CLK:
        case CMU_UART1_CLK:
        case CMU_UART2_CLK:
        case CMU_SPI0_CLK:
        case CMU_SPI1_CLK:
        case CMU_SPI2_CLK:
        case CMU_SPI3_CLK:
        case CMU_SPI4_CLK:
        case CMU_I2C0_CLK:
        case CMU_I2C1_CLK:
        case CMU_I2C2_CLK:
        case CMU_I2C3_CLK:
        case CMU_I2C4_CLK:
            mux = CMU_GetClockMux(clock);
            if (mux == CMU_MUX_MCU_CLK) {
                return bp6a_cmu_get_apb_clock_div();
            } else { //(CMU_MUX_MCU_CLK == CMU_MUX_PERI_CLK)
                return bp6a_cmu_get_pri_clock_div();
            }
        default:
            break;
    }
    return 0;
}

bool bp6a_cmu_get_clock_enabled(cmu_clock_t clock)
{
    if (clock <= CMU_WATCHDOG_APBCLK) {
        return ((getreg32(&BP_SYSCON->MCU_CLK_GATE) >> (clock - CMU_FCLK_AHBCLK)) & 0x01);
    } else {
        return ((getreg32(&BP_SYSCON->PERI_CLK_GATE) >> (clock - CMU_UART0_CLK)) & 0x01);
    }
}

uint32_t bp6a_get_clock_src_freq(cmu_clock_t clock)
{
    uint32_t mux;

    switch (clock) {
        case CMU_SAMPLE_CLK_AFTER_DIV:
            return bp6a_cmu_get_afe_con_clock_freq();
        case CMU_SRPCLK_AFTER_DIV:
            return bp6a_get_mcu_src_clock_freq();
        case CMU_AHBCLK_AFTER_DIV:
            return bp6a_get_mcu_src_clock_freq();
        case CMU_APBCLK_AFTER_DIV:
            return bp6a_get_mcu_src_clock_freq();

        case CMU_PERICLK_AFTER_DIV:
            return bp6a_cmu_get_peri_clock_src();
        case CMU_SRP_SRPCLK:
            if (bp6a_cmu_get_clock_enabled(clock)) {
                return bp6a_get_mcu_src_clock_freq();
            } else {
                return 0;
            }
        case CMU_FCLK_AHBCLK:
        case CMU_GPIO_AHBCLK:
        case CMU_UDMAC_AHBCLK:
        case CMU_DTRNG_AHBCLK:
        case CMU_AES_AHBCLK:
        case CMU_SRC_AHBCLK:
        case CMU_QSPI_AHBCLK:
            if (bp6a_cmu_get_clock_enabled(clock)) {
                return bp6a_get_mcu_src_clock_freq();
            } else {
                return 0;
            }
        case CMU_TIMER0_APBCLK:
        case CMU_TIMER1_APBCLK:
        case CMU_TIMER2_APBCLK:
        case CMU_TIMER3_APBCLK:
        case CMU_TIMER4_APBCLK:
        case CMU_TIMER5_APBCLK:
        case CMU_TIMER6_APBCLK:
        case CMU_PWMTIMER_APBCLK:
        case CMU_DUALTIMER_APBCLK:
        case CMU_WATCHDOG_APBCLK:
            if (bp6a_cmu_get_clock_enabled(clock)) {
                return bp6a_get_mcu_src_clock_freq();
            } else {
                return 0;
            }
        case CMU_UART0_CLK:
        case CMU_UART1_CLK:
        case CMU_UART2_CLK:
        case CMU_SPI0_CLK:
        case CMU_SPI1_CLK:
        case CMU_SPI2_CLK:
        case CMU_SPI3_CLK:
        case CMU_SPI4_CLK:
        case CMU_I2C0_CLK:
        case CMU_I2C1_CLK:
        case CMU_I2C2_CLK:
        case CMU_I2C3_CLK:
        case CMU_I2C4_CLK:
            if (bp6a_cmu_get_clock_enabled(clock)) {
                mux = CMU_GetClockMux(clock);
                if (mux == CMU_MUX_MCU_CLK) {
                    return bp6a_get_mcu_src_clock_freq();
                } else { //(CMU_MUX_MCU_CLK == CMU_MUX_PERI_CLK)
                    return bp6a_cmu_get_peri_clock_src();
                }
            } else {
                return 0;
            }
        default:
            break;
    }
    return 0;
}

uint32_t bp6a_cmu_get_clock_freq(cmu_clock_t clock)
{
    uint32_t src_clk = 0;
    uint32_t clk_div = 0;

    if (clock >= CMU_CLK_INVALID) {
        return 0;
    }

    src_clk = bp6a_get_clock_src_freq(clock);
    clk_div = bp6a_cmu_get_clock_div(clock);

    if (clk_div == 0) {
        clk_div = 1;
    }

    return (uint32_t)(src_clk / clk_div);
}

void bp6a_cmu_init(cmu_src_clk_t mcu_clk, cmu_src_clk_t peri_clk)
{
    int i;
    bp6a_set_muc_clock_div(1);
    bp6a_cmu_set_mcu_clock_src(mcu_clk);
    bp6a_cmu_peri_clock_div(1);
    bp6a_cmu_set_peri_clock_src(peri_clk);
    bp6a_cmu_set_hsosc_cal(1, 0, 0);
    bp6a_cmu_set_lsosc_cal(1, 0, 0);
    bp6a_cmu_set_mcu_clock_gate_bypass(false);
    /* clock gate */
    for (i = CMU_FCLK_AHBCLK ; i < CMU_I2C4_CLK; i++) {
        bp6a_cmu_enable_clock(i, (INIT_CLOCK_CONFIG >> (i - CMU_FCLK_AHBCLK)) & 0x01);
    }

    /* internal OSC calibration enable */
    modifyreg32(0x4001C60C, 0x2, 0x2);
    modifyreg32(0x4001C610, 0x2, 0x2);

    /* AFE Clock disable */
    putreg32(0x4001A000, 0);
    modifyreg32(0x4001A000, 0x08, 0x8);
}
