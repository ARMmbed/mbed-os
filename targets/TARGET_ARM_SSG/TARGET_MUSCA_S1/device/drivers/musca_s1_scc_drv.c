/*
 * Copyright (c) 2017-2020 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include "musca_s1_scc_drv.h"
/* Use __ISB(), __DSB() */
#include "cmsis.h"

/** Setter bit manipulation macro */
#define SET_BIT(WORD, BIT_INDEX) ((WORD) |= (1u << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT(WORD, BIT_INDEX) ((WORD) &= ~(1u << (BIT_INDEX)))

struct musca_s1_scc_reg_map_t {
    /* 0x000-0x800 Reserved (offset) */
    const uint32_t reserved0[512];
    /* 0x800 RW Clock Control Selection Register */
    volatile uint32_t clk_ctrl_sel;
    /* 0x804 RW Clock PLL Prediv Control Register */
    volatile uint32_t clk_pll_prediv_ctrl;
    /* 0x808 RW Body Bias Generator CLK Div Register */
    volatile uint32_t clk_bbgen_div_clk;
    /* 0x80C Reserved */
    const uint32_t reserved1;
    /* 0x810 RW QSPI Post PLL CLK divider Register */
    volatile uint32_t clk_postdiv_qspi;
    /* 0x814 RW RTC Post PLL CLK div ctrl Register */
    volatile uint32_t clk_postdiv_ctrl_rtc;
    /* 0x818 Reserved */
    const uint32_t reserved2;
    /* 0x81C RW TEST CLK Post PLL CLK Div Register */
    volatile uint32_t clk_postdiv_ctrl_test;
    /* 0x820 RW Post PLL CLK divider bypass Register */
    volatile uint32_t ctrl_bypass_div;
    /* 0x824 RW PLL0 Control Register */
    volatile uint32_t pll_ctrl_pll0_clk;
    /* 0x828-0x82C Reserved */
    const uint32_t reserved3[2];
    /* 0x830 RW Clock gate enable control Register */
    volatile uint32_t clk_ctrl_enable;
    /* 0x834 RW PLL Status Register */
    volatile uint32_t clk_status;
    /* 0x838-0x83C Reserved */
    const uint32_t reserved4[2];
    /* 0x840 RW Reset Control Register */
    volatile uint32_t reset_ctrl;
    /* 0x844 Reserved */
    const uint32_t reserved5;
    /* 0x848 RW Debug Control Register */
    volatile uint32_t dbg_ctrl;
    /* 0x84C RW SRAM Control Register */
    volatile uint32_t sram_ctrl;
    /* 0x850 RW MPC Interupt Control Register */
    volatile uint32_t intr_ctrl;
    /* 0x854 Reserved */
    const uint32_t reserved6;
    /* 0x858 RW Reset vector for CPU0 SRAM */
    volatile uint32_t cpu0_vtor;
    /* 0x85C RW Reset vector for CPU0 MRAM */
    volatile uint32_t cpu0_vtor_1;
    /* 0x860 RW Reset vector for CPU1 SRAM */
    volatile uint32_t cpu1_vtor;
    /* 0x864 RW Reset vector for CPU1 MRAM */
    volatile uint32_t cpu1_vtor_1;
    /* 0x868 RW Main function in data select */
    volatile uint32_t iomux_main_insel;
    /* 0x874 Reserved */
    const uint32_t reserved7;
    /* 0x870 RW Main function out data select */
    volatile uint32_t iomux_main_outsel;
    /* 0x874 Reserved */
    const uint32_t reserved8;
    /* 0x878 RW Main function out enable select */
    volatile uint32_t iomux_main_oensel;
    /* 0x87C Reserved */
    const uint32_t reserved9;
    /* 0x880 RW Main function default in select */
    volatile uint32_t iomux_main_default_in;
    /* 0x884 Reserved */
    const uint32_t reserved10;
    /* 0x888 RW Alt function 1 in data select */
    volatile uint32_t iomux_altf1_insel;
    /* 0x88C Reserved */
    const uint32_t reserved11;
    /* 0x890 RW Alt function 1 out data select */
    volatile uint32_t iomux_altf1_outsel;
    /* 0x894 Reserved */
    const uint32_t reserved12;
    /* 0x898 RW Alt function 1 out enable select */
    volatile uint32_t iomux_altf1_oensel;
    /* 0x89C Reserved */
    const uint32_t reserved13;
    /* 0x8A0 RW Alt function 1 default in select */
    volatile uint32_t iomux_altf1_default_in;
    /* 0x8A4 Reserved */
    const uint32_t reserved14;
    /* 0x8A8 RW Alt function 2 in data select */
    volatile uint32_t iomux_altf2_insel;
    /* 0x8AC Reserved */
    const uint32_t reserved15;
    /* 0x8B0 RW Alt function 2 out data select */
    volatile uint32_t iomux_altf2_outsel;
    /* 0x8B4 Reserved */
    const uint32_t reserved16;
    /* 0x8B8 RW Alt function 2 out enable select */
    volatile uint32_t iomux_altf2_oensel;
    /* 0x8BC Reserved */
    const uint32_t reserved17;
    /* 0x8C0 RW Alt function 2 default in select */
    volatile uint32_t iomux_altf2_default_in;
    /* 0x8C4-0x8E4 Reserved */
    const uint32_t reserved18[9];
    /* 0x8E8 RW Drive Select 0 */
    volatile uint32_t iopad_ds0;
    /* 0x8EC Reserved */
    const uint32_t reserved19;
    /* 0x8F0 RW Drive Select 1 */
    volatile uint32_t iopad_ds1;
    /* 0x8F4 Reserved */
    const uint32_t reserved20;
    /* 0x8F8 RW Pull Enable */
    volatile uint32_t iopad_pe;
    /* 0x8FC Reserved */
    const uint32_t reserved21;
    /* 0x900 RW Pull Select */
    volatile uint32_t iopad_ps;
    /* 0x904 Reserved */
    const uint32_t reserved22;
    /* 0x908 RW Slew Select */
    volatile uint32_t iopad_sr;
    /* 0x90C Reserved */
    const uint32_t reserved23;
    /* 0x910 RW Input Select */
    volatile uint32_t iopad_is;
    /* 0x914 Reserved */
    const uint32_t reserved24;
    /* 0x918 RW PVT control register */
    volatile uint32_t pvt_ctrl;
    /* 0x91C-0x938 Reserved */
    const uint32_t reserved25[8];
    /* 0x93C RW Static configuration */
    volatile uint32_t static_conf_sig1;
    /* 0x940-0x994 Reserved */
    const uint32_t reserved26[22];
    /* 0x998 RW eMRAM control 0 register */
    volatile uint32_t scc_mram_ctrl0;
    /* 0x99C RW eMRAM control 1 register */
    volatile uint32_t scc_mram_ctrl1;
    /* 0x9A0 RW eMRAM control 2 register */
    volatile uint32_t scc_mram_ctrl2;
    /* 0x9A4-0x9AC Reserved */
    const uint32_t reserved27[3];
    /* 0x9B0 RW eMRAM data input 0 register */
    volatile uint32_t scc_mram_din0;
    /* 0x9B4 RW eMRAM data input 1 register */
    volatile uint32_t scc_mram_din1;
    /* 0x9B8 RW eMRAM data input 2 register */
    volatile uint32_t scc_mram_din2;
    /* 0x9BC Reserved */
    const uint32_t reserved28;
    /* 0x9C0 RW eMRAM data output 0 register */
    volatile uint32_t scc_mram_dout0;
    /* 0x9C4 RW eMRAM data output 1 register */
    volatile uint32_t scc_mram_dout1;
    /* 0x9C8 RW eMRAM data output 2 register */
    volatile uint32_t scc_mram_dout2;
    /* 0x9CC-0x9DC Reserved */
    const uint32_t reserved29[5];
    /* 0x9E0 RW CLK pahes shift control register */
    volatile uint32_t selection_control_reg;
    /* 0x9E4-0xA00 Reserved */
    const uint32_t reserved30[8];
    /* 0xA04 RW SSE-200 OTP control register */
    volatile uint32_t sse_otp_ctrl;
    /* 0xA08-0xA1C Reserved */
    const uint32_t reserved31[6];
    /* 0xA20 RW Body-Biasing control register */
    volatile uint32_t bbgen_ctrl;
    /* 0xA24 RW Spaer control register */
    volatile uint32_t spare_ctrl1;
    /* 0xA28-0xBFC Reserved */
    const uint32_t reserved32[118];
    /* 0xC00 RO Chip ID 0x0797_0477 */
    const uint32_t chip_id;
    /* 0xC04 RO I/O in status */
    const uint32_t io_in_status;
};

/**
 * \brief Musca-S1 SCC eMRAM control 0 register bit fields
 */
#define SCC_MRAM_CTRL0_MRAM_CLK_EN                   0u
    /*!< eMRAM control register 0 Enable eMRAM clock offset */
#define SCC_MRAM_CTRL0_PROC_SPEC_CLK_EN              1u
    /*!< eMRAM control register 0 Enable eMRAM controller clock offset */
#define SCC_MRAM_CTRL0_AUTOSTOP_EN                   2u
    /*!< eMRAM control register 0 Enable autostop offset */
#define SCC_MRAM_CTRL0_MRAM_INV_CLK_SEL              3u
    /*!< eMRAM control register 0 Select clock inversion offset */
#define SCC_MRAM_CTRL0_FAST_READ_EN                  4u
    /*!< eMRAM control register 0 Enable fast read offset */
#define SCC_MRAM_CTRL0_MRAM_DOUT_SEL                 5u
    /*!< eMRAM control register 0 Select eMRAM0 output data offset */
#define SCC_MRAM_CTRL0_PG_VDD_0                      8u
    /*!< eMRAM control register 0 eMRAM0 PG VDD offset */
#define SCC_MRAM_CTRL0_PG_VDD18_0                    9u
    /*!< eMRAM control register 0 eMRAM1 PG VDD18_0 offset */
#define SCC_MRAM_CTRL0_PG_VDD_1                      10u
    /*!< eMRAM control register 0 eMRAM1 PG VDD offset */
#define SCC_MRAM_CTRL0_PG_VDD18_1                    11u
    /*!< eMRAM control register 0 eMRAM1 PG VDD18 offset */
#define SCC_MRAM_CTRL0_WRITE_CSN_CLKS                12u
    /*!< eMRAM control register 0 WRITE_CSN_CLKS offset */
#define SCC_MRAM_CTRL0_CSN_HIGH_CLKS                 16u
    /*!< eMRAM control register 0 CSN_HIGH_CLKS offset */
#define SCC_MRAM_CTRL0_READ_CSN_CLKS                 20u
    /*!< eMRAM control register 0 Number of clocks for single read offset */
#define SCC_MRAM_CTRL0_MRAM_OTP_CLK_EN               29u
    /*!< eMRAM control register 0 Enable eMRAM OTP clock offset */
#define SCC_MRAM_CTRL0_MRAM_CLK_SYNC_BYPASS          30u
    /*!< eMRAM control register 0 Bypass eMRAM clock divider sync offset */
#define SCC_MRAM_CTRL0_MRAM_DA_EN                    31u
    /*!< eMRAM control register 0 Enable eMRAM direct access offset */

/**
 * \brief Clears selected alternate functions for selected pins
 *
 * \param[in] dev        SCC registers base address \ref musca_s1_scc_reg_map_t
 * \param[in] func_mask  Bitmask of alternate functions to clear
 *                       \ref gpio_altfunc_mask_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 */
static void scc_clear_alt_func(struct musca_s1_scc_reg_map_t* scc_regs,
                               enum gpio_altfunc_mask_t func_mask,
                               uint32_t pin_mask)
{
    if (func_mask & GPIO_MAIN_FUNC_MASK) {
        scc_regs->iomux_main_insel &= ~pin_mask;
        scc_regs->iomux_main_outsel &= ~pin_mask;
        scc_regs->iomux_main_oensel &= ~pin_mask;
    }
    if (func_mask & GPIO_ALTFUNC_1_MASK) {
        scc_regs->iomux_altf1_insel &= ~pin_mask;
        scc_regs->iomux_altf1_outsel &= ~pin_mask;
        scc_regs->iomux_altf1_oensel &= ~pin_mask;
    }
    if (func_mask & GPIO_ALTFUNC_2_MASK) {
        scc_regs->iomux_altf2_insel &= ~pin_mask;
        scc_regs->iomux_altf2_outsel &= ~pin_mask;
        scc_regs->iomux_altf2_oensel &= ~pin_mask;
    }
}

void musca_s1_scc_set_alt_func(struct musca_s1_scc_dev_t* dev,
                               enum gpio_altfunc_t altfunc, uint32_t pin_mask)
{
    struct musca_s1_scc_reg_map_t* scc_regs =
                                (struct musca_s1_scc_reg_map_t*) dev->cfg->base;
    enum gpio_altfunc_mask_t altfunc_to_clear = GPIO_ALTFUNC_NONE;
    volatile uint32_t *insel = NULL;
    volatile uint32_t *outsel = NULL;
    volatile uint32_t *oensel = NULL;

    if (altfunc >= GPIO_ALTFUNC_MAX) {
        /* If no altfunction is selected, then nothing to do.
         * This is possible during init and we do not
         * want to change the reset values set by the HW
         */
        return;
    }

    switch (altfunc) {
        case GPIO_MAIN_FUNC:
            insel = &scc_regs->iomux_main_insel;
            outsel = &scc_regs->iomux_main_outsel;
            oensel = &scc_regs->iomux_main_oensel;
            altfunc_to_clear = GPIO_MAIN_FUNC_NEG_MASK;
            break;

        case GPIO_ALTFUNC_1:
            insel = &scc_regs->iomux_altf1_insel;
            outsel = &scc_regs->iomux_altf1_outsel;
            oensel = &scc_regs->iomux_altf1_oensel;
            altfunc_to_clear = GPIO_ALTFUNC_1_NEG_MASK;
            break;

        case GPIO_ALTFUNC_2:
            insel = &scc_regs->iomux_altf2_insel;
            outsel = &scc_regs->iomux_altf2_outsel;
            oensel = &scc_regs->iomux_altf2_oensel;
            altfunc_to_clear = GPIO_ALTFUNC_2_NEG_MASK;
            break;
        default:
        break;
    }

    /* Select the wanted function's output enable bit first.
     * This way the output won't be disabled which is desired
     * if we switch from output to output function
     */
    *oensel |= pin_mask;

    /* Clear all alternate function registers which are not selected */
    scc_clear_alt_func(scc_regs, altfunc_to_clear, pin_mask);

    /* Enable input and output data line */
    *insel |= pin_mask;
    *outsel |= pin_mask;
}

void musca_s1_scc_set_pinmode(struct musca_s1_scc_dev_t* dev, uint32_t pin_mask,
                              enum pinmode_select_t mode)
{
    struct musca_s1_scc_reg_map_t* scc_regs =
                                (struct musca_s1_scc_reg_map_t*) dev->cfg->base;

    switch (mode) {
        case PINMODE_NONE:
            scc_regs->iopad_pe &= ~pin_mask;
            break;
        case PINMODE_PULL_DOWN:
            /* If the pull select bit is set to 0 it means pull down */
            scc_regs->iopad_ps &= ~pin_mask;
            scc_regs->iopad_pe |= pin_mask;
            break;
        case PINMODE_PULL_UP:
            /* If the pull select bit is set to 1 it means pull up */
            scc_regs->iopad_ps |= pin_mask;
            scc_regs->iopad_pe |= pin_mask;
            break;
        default:
            break;
    }
}

void musca_s1_scc_set_default_in(struct musca_s1_scc_dev_t* dev,
                                 enum gpio_altfunc_t altfunc,
                                 uint32_t default_in_mask,
                                 uint32_t default_in_value)
{
    struct musca_s1_scc_reg_map_t* scc_regs =
                                (struct musca_s1_scc_reg_map_t*) dev->cfg->base;
    uint32_t iomux_value = 0;

    if (altfunc >= GPIO_ALTFUNC_MAX) {
        /* If no altfunction is selected, then nothing to do */
        return;
    }

    switch (altfunc) {
        case GPIO_MAIN_FUNC:
            iomux_value = scc_regs->iomux_main_default_in & ~default_in_mask;
            iomux_value |= (default_in_value & default_in_mask);
            scc_regs->iomux_main_default_in = iomux_value;
            scc_regs->iomux_main_insel =
                                (scc_regs->iomux_main_insel & ~default_in_mask);
            break;

        case GPIO_ALTFUNC_1:
            iomux_value = scc_regs->iomux_altf1_default_in & ~default_in_mask;
            iomux_value |= (default_in_value & default_in_mask);
            scc_regs->iomux_altf1_default_in = iomux_value;
            scc_regs->iomux_altf1_insel =
                               (scc_regs->iomux_altf1_insel & ~default_in_mask);
            break;

        case GPIO_ALTFUNC_2:
            iomux_value = scc_regs->iomux_altf2_default_in & ~default_in_mask;
            iomux_value |= (default_in_value & default_in_mask);
            scc_regs->iomux_altf2_default_in = iomux_value;
            scc_regs->iomux_altf2_insel =
                               (scc_regs->iomux_altf2_insel & ~default_in_mask);
            break;
        default:
        break;
    }
}

void musca_s1_scc_mram_fast_read_enable(struct musca_s1_scc_dev_t* dev)
{
    struct musca_s1_scc_reg_map_t* scc_regs =
                                (struct musca_s1_scc_reg_map_t*) dev->cfg->base;
    __DSB();
    SET_BIT(scc_regs->scc_mram_ctrl0, SCC_MRAM_CTRL0_FAST_READ_EN);
    __DSB();
    __ISB();
}

void musca_s1_scc_mram_fast_read_disable(struct musca_s1_scc_dev_t* dev)
{
    struct musca_s1_scc_reg_map_t* scc_regs =
                                (struct musca_s1_scc_reg_map_t*) dev->cfg->base;
    __DSB();
    CLR_BIT(scc_regs->scc_mram_ctrl0, SCC_MRAM_CTRL0_FAST_READ_EN);
    __DSB();
    __ISB();
}

bool musca_s1_scc_mram_is_fast_read_enabled(struct musca_s1_scc_dev_t* dev)
{
    struct musca_s1_scc_reg_map_t* scc_regs =
                                (struct musca_s1_scc_reg_map_t*) dev->cfg->base;
    return (bool)(scc_regs->scc_mram_ctrl0 &
                  (1u << SCC_MRAM_CTRL0_FAST_READ_EN));
}