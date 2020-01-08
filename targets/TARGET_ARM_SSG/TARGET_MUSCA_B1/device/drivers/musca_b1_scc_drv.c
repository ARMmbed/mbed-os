/*
 * Copyright (c) 2018-2019 Arm Limited. All rights reserved.
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

#include "musca_b1_scc_drv.h"

/* The first pin of the upper pins (out of 64) */
#define GPIO_UPPER_PINS_MIN_VALUE          32U

struct musca_b1_scc_reg_map_t {
    volatile uint32_t clk_ctrl_sel;                    /* 0x00 RW Clock Control Select */
    volatile uint32_t clk_pll_prediv_ctrl;             /* 0x04 RW clk_pll_prediv_ctrl */
    volatile uint32_t reserved1;                       /* 0x08 Reserved */
    volatile uint32_t clk_postdiv_ctrl_flash;          /* 0x0C RW clk_postdiv_ctrl_flash */
    volatile uint32_t clk_postdiv_qspi;                /* 0x10 RW clk_postdiv_qspi */
    volatile uint32_t clk_postdiv_rtc;                 /* 0x14 RW clk_postdiv_rtc */
    volatile uint32_t clk_postdiv_sd;                  /* 0x18 RW clk_postdiv_sd */
    volatile uint32_t clk_postdiv_test;                /* 0x1C RW clk_postdiv_test */
    volatile uint32_t ctrl_bypass_div;                 /* 0x20 RW ctrl_bypass_div */
    volatile uint32_t pll_ctrl_pll0_clk;               /* 0x24 RW pll_ctrl_pll0_clk */
    volatile uint32_t pll_postdiv_ctrl_pll0_clk;       /* 0x28 RW pll_postdiv_ctrl_pll0_clk */
    volatile uint32_t pll_ctrl_mult_pll0_clk;          /* 0x2C RW pll_ctrl_mult_pll0_clk */
    volatile uint32_t clk_ctrl_enable;                 /* 0x30 RW Clock Control Enable */
    volatile uint32_t clk_status;                      /* 0x34 RO Clock Status */
    volatile uint32_t reserved2[2];                    /* 0x38 Reserved */
    volatile uint32_t reset_ctrl;                      /* 0x40 RW Reset Control Register */
    volatile uint32_t reserved3;                       /* 0x44 Reserved */
    volatile uint32_t dbg_ctrl;                        /* 0x48 RW Debug Control Register */
    volatile uint32_t sram_ctrl;                       /* 0x4C RW SRAM Control Register */
    volatile uint32_t intr_ctrl;                       /* 0x50 RW Interupt Control Register */
    volatile uint32_t clk_test_ctrl;                   /* 0x54 RW Clock Test Control Register */
    volatile uint32_t cpu0_vtor;                       /* 0x58 RW Reset vector for CPU0 */
    volatile uint32_t reserved4;                       /* 0x5C RW Reset vector for KX CPU */
    volatile uint32_t cpu1_vtor;                       /* 0x60 Reserved */
    volatile uint32_t az_cpu_vtor;                     /* 0x64 RW Reset vector for AZ CPU */
    volatile uint32_t iomux_main_insel_0;              /* 0x68 RW Main function in data select */
    volatile uint32_t iomux_main_insel_1;              /* 0x6C RW Main function in data select */
    volatile uint32_t iomux_main_outsel_0;             /* 0x70 RW Main function out data select */
    volatile uint32_t iomux_main_outsel_1;             /* 0x74 RW Main function out data select */
    volatile uint32_t iomux_main_oensel_0;             /* 0x78 RW Main function out enable select */
    volatile uint32_t iomux_main_oensel_1;             /* 0x7C RW Main function out enable select */
    volatile uint32_t iomux_main_default_in_0;         /* 0x80 RW Main function default in select */
    volatile uint32_t iomux_main_default_in_1;         /* 0x84 RW Main function default in select */
    volatile uint32_t iomux_altf1_insel_0;             /* 0x88 RW Alt function 1 in data select */
    volatile uint32_t iomux_altf1_insel_1;             /* 0x8C RW Alt function 1 in data select */
    volatile uint32_t iomux_altf1_outsel_0;            /* 0x90 RW Alt function 1 out data select */
    volatile uint32_t iomux_altf1_outsel_1;            /* 0x94 RW Alt function 1 out data select */
    volatile uint32_t iomux_altf1_oensel_0;            /* 0x98 RW Alt function 1 out enable select */
    volatile uint32_t iomux_altf1_oensel_1;            /* 0x9C RW Alt function 1 out enable select */
    volatile uint32_t iomux_altf1_default_in_0;        /* 0xA0 RW Alt function 1 default in select */
    volatile uint32_t iomux_altf1_default_in_1;        /* 0xA4 RW Alt function 1 default in select */
    volatile uint32_t iomux_altf2_insel_0;             /* 0xA8 RW Alt function 2 in data select */
    volatile uint32_t iomux_altf2_insel_1;             /* 0xAC RW Alt function 2 in data select */
    volatile uint32_t iomux_altf2_outsel_0;            /* 0xB0 RW Alt function 2 out data select */
    volatile uint32_t iomux_altf2_outsel_1;            /* 0xB4 RW Alt function 2 out data select */
    volatile uint32_t iomux_altf2_oensel_0;            /* 0xB8 RW Alt function 2 out enable select */
    volatile uint32_t iomux_altf2_oensel_1;            /* 0xBC RW Alt function 2 out enable select */
    volatile uint32_t iomux_altf2_default_in_0;        /* 0xC0 RW Alt function 2 default in select */
    volatile uint32_t iomux_altf2_default_in_1;        /* 0xC4 RW Alt function 2 default in select */
    volatile uint32_t reserved5[8];                    /* 0xC8 Reserved */
    volatile uint32_t iopad_ds0_0;                     /* 0xE8 RW Drive Select 0 */
    volatile uint32_t iopad_ds0_1;                     /* 0xEC RW Drive Select 0 */
    volatile uint32_t iopad_ds1_0;                     /* 0xF0 RW Drive Select 1 */
    volatile uint32_t iopad_ds1_1;                     /* 0xF4 RW Drive Select 1 */
    volatile uint32_t iopad_pe_0;                      /* 0xF8 RW Pull Enable */
    volatile uint32_t iopad_pe_1;                      /* 0xFC RW Pull Enable */
    volatile uint32_t iopad_ps_0;                      /* 0x100 RW Pull Select */
    volatile uint32_t iopad_ps_1;                      /* 0x104 RW Pull Select */
    volatile uint32_t iopad_sr_0;                      /* 0x108 RW Slew Select */
    volatile uint32_t iopad_sr_1;                      /* 0x10C RW Slew Select */
    volatile uint32_t iopad_is_0;                      /* 0x110 RW Input Select */
    volatile uint32_t iopad_is_1;                      /* 0x114 RW Input Select */
    volatile uint32_t pvt_ctrl;                        /* 0x118 RW PVT control register */
    volatile uint32_t reserved6[5];                    /* 0x11C Reserved */
    volatile uint32_t spare0;                          /* 0x130 RW spare0 */
    volatile uint32_t reserved7[2];                    /* 0x134 Reserved */
    volatile uint32_t static_conf_sig1;                /* 0x13C RW static_conf_sig1 */
    volatile uint32_t reserved8[24];                   /* 0x140 Reserved */
    volatile uint32_t flash0_din_0;                    /* 0x1A0 RW flash0_or 1 din_0 */
    volatile uint32_t flash0_din_1;                    /* 0x1A4 RW flash0 or 1 din_1 */
    volatile uint32_t flash0_din_2;                    /* 0x1A8 RW flash0_or 1 din_2 */
    volatile uint32_t flash0_din_3;                    /* 0x1AC RW flash0_or 1 din_3 */
    volatile uint32_t reserved9[4];                    /* 0x1B0 Reserved */
    volatile uint32_t flash0_dout_0;                   /* 0x1C0 RO flash0_dout_0 */
    volatile uint32_t flash0_dout_1;                   /* 0x1C4 RO flash0_dout_1 */
    volatile uint32_t flash0_dout_2;                   /* 0x1C8 RO flash0_dout_2 */
    volatile uint32_t flash0_dout_3;                   /* 0x1CC RO flash0_dout_3 */
    volatile uint32_t flash1_dout_0;                   /* 0x1D0 RO flash1_dout_0 */
    volatile uint32_t flash1_dout_1;                   /* 0x1D4 RO flash1_dout_1 */
    volatile uint32_t flash1_dout_2;                   /* 0x1D8 RO flash1_dout_2 */
    volatile uint32_t flash1_dout_3;                   /* 0x1DC RO flash1_dout_2 */
    volatile uint32_t selection_control_reg;           /* 0x1E0 RW Selection Control Register */
    volatile uint32_t az_rom_remap_mask;               /* 0x1E4 RW az_rom_remap_mask */
    volatile uint32_t az_rom_remap_offset;             /* 0x1E8 RW az_rom_remap_offset */
    volatile uint32_t az_code_remap_mask;              /* 0x1EC RW az_code_remap_mask */
    volatile uint32_t az_code_remap_offset;            /* 0x1F0 RW az_code_remap_offset */
    volatile uint32_t az_sys_remap_mask;               /* 0x1F4 RW az_sys_remap_mask */
    volatile uint32_t az_sys_remap_offset;             /* 0x1F8 RW az_sys_remap_offset */
    volatile uint32_t reserved10;                      /* 0x1FC RW Kalix Control */
    volatile uint32_t az_ctrl;                         /* 0x200 RW  Alcatraz Control */
    volatile uint32_t reserved11;                      /* 0x204 Reserved */
    volatile uint32_t sse200_otp_rd_data;              /* 0x208 RO SSE200_otp_ctrl */
    volatile uint32_t reserved12;                      /* 0x20C Reserved */
    volatile uint32_t az_otp_rd_data;                  /* 0x210 RO az_otp_rd_data */
    volatile uint32_t reserved13[2];                   /* 0x214 Reserved */
    volatile uint32_t spare_ctrl0;                     /* 0x21C RW spare_ctrl0 */
    volatile uint32_t spare_ctrl1;                     /* 0x220 RW spare_ctrl1 */
    volatile uint32_t reserved[119];                   /* 0x224 Reserved */
    volatile uint32_t chip_id;                         /* 0x400 RO Chip ID 0x07D00477 */
};

/**
 * \brief Clears selected alternate functions for selected pins
 *
 * \param[in] scc_regs   SCC register map pointer \ref musca_b1_scc_reg_map_t
 * \param[in] func_mask  Bitmask of alternate functions to clear
 *                       \ref gpio_altfunc_mask_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 * \param[in] upper_pins True if pin_mask represents the upper 32 pins
 */
static void scc_clear_alt_func(struct musca_b1_scc_reg_map_t* scc_regs,
                               enum gpio_altfunc_mask_t func_mask,
                               uint32_t pin_mask, bool upper_pins)
{
    /* The pin select pointers are pointing to the registers
     * which control the lower 32 pins of the whole 64. If we need to reach
     * the upper 32 pins, the pointers need to be incremented.
     * Registers are 32 bites wide so offset is in 4 byte steps.
     */
    uint32_t reg_offset = (upper_pins ? 1U : 0U);

    if (func_mask & GPIO_MAIN_FUNC_MASK) {
        *(&scc_regs->iomux_main_insel_0 + reg_offset) &= (~pin_mask);
        *(&scc_regs->iomux_main_outsel_0 + reg_offset) &= (~pin_mask);
        *(&scc_regs->iomux_main_oensel_0 + reg_offset) &= (~pin_mask);
    }
    if (func_mask & GPIO_ALTFUNC_1_MASK) {
        *(&scc_regs->iomux_altf1_insel_0 + reg_offset) &= (~pin_mask);
        *(&scc_regs->iomux_altf1_outsel_0 + reg_offset) &= (~pin_mask);
        *(&scc_regs->iomux_altf1_oensel_0 + reg_offset) &= (~pin_mask);
    }
    if (func_mask & GPIO_ALTFUNC_2_MASK) {
        *(&scc_regs->iomux_altf2_insel_0 + reg_offset) &= (~pin_mask);
        *(&scc_regs->iomux_altf2_outsel_0 + reg_offset) &= (~pin_mask);
        *(&scc_regs->iomux_altf2_oensel_0 + reg_offset) &= (~pin_mask);
    }
}

/**
 * \brief Sets selected alternate functions for selected (max 32) pins
 *
 * \param[in] scc_regs   SCC register map pointer \ref musca_b1_scc_reg_map_t
 * \param[in] altfunc    Alternate function to set \ref gpio_altfunc_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 * \param[in] upper_pins True if pin_mask represents the upper 32 pins
 *
 * \note This function doesn't check if scc dev is NULL.
 * \note If no alternate function is selected then this API won't do anything
 */
static void scc_set_alt_func(struct musca_b1_scc_reg_map_t* scc_regs,
                             enum gpio_altfunc_t altfunc, uint32_t pin_mask,
                             bool upper_pins)
{
    enum gpio_altfunc_mask_t altfunc_to_clear = GPIO_ALTFUNC_NONE;
    volatile uint32_t *insel = NULL;
    volatile uint32_t *outse = NULL;
    volatile uint32_t *oense = NULL;

    switch (altfunc) {
        case GPIO_MAIN_FUNC:
            insel = &scc_regs->iomux_main_insel_0;
            outse = &scc_regs->iomux_main_outsel_0;
            oense = &scc_regs->iomux_main_oensel_0;
            altfunc_to_clear = GPIO_MAIN_FUNC_NEG_MASK;
            break;

        case GPIO_ALTFUNC_1:
            insel = &scc_regs->iomux_altf1_insel_0;
            outse = &scc_regs->iomux_altf1_outsel_0;
            oense = &scc_regs->iomux_altf1_oensel_0;
            altfunc_to_clear = GPIO_ALTFUNC_1_NEG_MASK;
            break;

        case GPIO_ALTFUNC_2:
            insel = &scc_regs->iomux_altf2_insel_0;
            outse = &scc_regs->iomux_altf2_outsel_0;
            oense = &scc_regs->iomux_altf2_oensel_0;
            altfunc_to_clear = GPIO_ALTFUNC_2_NEG_MASK;
            break;

        case GPIO_ALTFUNC_3:
            /* Nothing to do, clearing the other functions enables ALTFUNC 3 */
            altfunc_to_clear = GPIO_ALTFUNC_3_NEG_MASK;
            scc_clear_alt_func(scc_regs, altfunc_to_clear, pin_mask, upper_pins);
            return;
        default:
            return;
    }

    if (upper_pins == true) {
        /* All the pin select pointers are pointing to the registers
         * which control the lower 32 pins of the whole 64. If we need to reach
         * the upper 32 pins, the pointers need to be incremented.
         */
        insel++;
        outse++;
        oense++;
    }

    /* Select the wanted function's output enable bit first.
     * This way the output won't be disabled which is desired
     * if we switch from output to output function
     */
    *oense |= pin_mask;

    /* Clear all alternate function registers which are not selected */
    scc_clear_alt_func(scc_regs, altfunc_to_clear, pin_mask, upper_pins);

    /* Enable input and output data line */
    *insel |= pin_mask;
    *outse |= pin_mask;
}

enum musca_b1_scc_error_t
musca_b1_scc_set_alt_func(struct musca_b1_scc_dev_t* dev,
                          enum gpio_altfunc_t altfunc, uint64_t pin_mask)
{
    struct musca_b1_scc_reg_map_t* scc_regs =
                                (struct musca_b1_scc_reg_map_t*) dev->cfg->base;
    uint32_t pin_mask_lo = (uint32_t)pin_mask;
    uint32_t pin_mask_hi = (uint32_t)(pin_mask >> GPIO_UPPER_PINS_MIN_VALUE);

    if (altfunc >= GPIO_ALTFUNC_MAX) {
        /* Invalid input */
        return SCC_INVALID_ARG;
    }

    /* Set alternate functions for the lower 32 pins - if needed */
    if (pin_mask_lo != 0U) {
        scc_set_alt_func(scc_regs, altfunc, pin_mask_lo, false);
    }

    /* Set alternate functions for the upper 32 pins - if needed */
    if (pin_mask_hi != 0U) {
        scc_set_alt_func(scc_regs, altfunc, pin_mask_hi, true);
    }

    return SCC_ERR_NONE;
}

/**
 * \brief Sets pinmode for the given pins
 *
 * \param[in] scc_regs   SCC register map pointer \ref musca_b1_scc_reg_map_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 * \param[in] mode       Pin mode to set \ref pinmode_select_t
 * \param[in] upper_pins True if the pinmask is for the upper 32 pins,
 *                       false otherwise
 */
static void scc_set_pinmode(struct musca_b1_scc_reg_map_t* scc_regs,
                            uint32_t pin_mask,
                            enum pinmode_select_t mode, bool upper_pins)
{
    /* The pin select pointers are pointing to the registers
     * which control the lower 32 pins of the whole 64. If we need to reach
     * the upper 32 pins, the pointers need to be incremented.
     * Registers are 32 bites wide so offset is in 4 byte steps.
     */
    uint32_t reg_offset = (upper_pins ? 1U : 0U);

    switch (mode) {
        case PINMODE_NONE:
            *(&scc_regs->iopad_pe_0 + reg_offset) &= (~pin_mask);
            break;
        case PINMODE_PULL_DOWN:
            /* If the pull select bit is set to 0 it means pull down */
            *(&scc_regs->iopad_ps_0 + reg_offset) &= (~pin_mask);
            *(&scc_regs->iopad_pe_0 + reg_offset) |= pin_mask;
            break;
        case PINMODE_PULL_UP:
            /* If the pull select bit is set to 1 it means pull up */
            *(&scc_regs->iopad_ps_0 + reg_offset) |= pin_mask;
            *(&scc_regs->iopad_pe_0 + reg_offset) |= pin_mask;
            break;
        default:
            break;
    }
}

enum musca_b1_scc_error_t
musca_b1_scc_set_pinmode(struct musca_b1_scc_dev_t* dev, uint64_t pin_mask,
                         enum pinmode_select_t mode)
{
    struct musca_b1_scc_reg_map_t* scc_regs =
                                (struct musca_b1_scc_reg_map_t*) dev->cfg->base;
    uint32_t pin_mask_lo = (uint32_t)pin_mask;
    uint32_t pin_mask_hi = (uint32_t)(pin_mask >> 32);

    if (mode >= PINMODE_MAX) {
        /* Invalid input */
        return SCC_INVALID_ARG;
    }

    /* Set pin mode for the lower 32 pins - if needed */
    if (pin_mask_lo != 0U) {
        scc_set_pinmode(scc_regs, pin_mask_lo, mode, false);
    }

    /* Set pin mode for the lower 32 pins - if needed */
    if (pin_mask_hi != 0U) {
        scc_set_pinmode(scc_regs, pin_mask_hi, mode, true);
    }

    return SCC_ERR_NONE;
}

enum musca_b1_scc_error_t
musca_b1_scc_set_default_in(struct musca_b1_scc_dev_t* dev,
                            enum gpio_altfunc_t altfunc,
                            uint32_t pin_num,
                            bool default_in_value)
{
    struct musca_b1_scc_reg_map_t* scc_regs =
                                (struct musca_b1_scc_reg_map_t*) dev->cfg->base;
    uint32_t iomux_value = 0U;
    uint32_t pin_mask = 0U;
    uint32_t default_in_mask = 0U;
    uint32_t reg_offset = 0U;

    if ((altfunc >= GPIO_ALTFUNC_MAX) || (pin_num >= GPIO_ALTFUNC_MAX_PINS)) {
        /* Invalid input */
        return SCC_INVALID_ARG;
    }

    /* Check if pin is the upper section */
    if (pin_num >= GPIO_UPPER_PINS_MIN_VALUE) {
        pin_num -= GPIO_UPPER_PINS_MIN_VALUE;
        /* The pin select pointers are pointing to the registers
         * which control the lower 32 pins of the whole 64. If we need
         * to reach the upper 32 pins, the pointers need to be incremented.
         * Registers are 32 bites wide so offset is in 4 byte steps.
         */
        reg_offset = 1U;
    }

    pin_mask = (1U << pin_num);
    default_in_mask = ((default_in_value ? 1U : 0U) << pin_num);

    switch (altfunc) {
        case GPIO_MAIN_FUNC:
            iomux_value =
               *(&scc_regs->iomux_main_default_in_0 + reg_offset) & (~pin_mask);
            *(&scc_regs->iomux_main_default_in_0 + reg_offset) =
                                                (iomux_value | default_in_mask);
            *(&scc_regs->iomux_main_insel_0 + reg_offset) =
                  (*(&scc_regs->iomux_main_insel_0 + reg_offset) & (~pin_mask));
            break;
        case GPIO_ALTFUNC_1:
            iomux_value =
              *(&scc_regs->iomux_altf1_default_in_0 + reg_offset) & (~pin_mask);
            *(&scc_regs->iomux_altf1_default_in_0 + reg_offset) =
                                                (iomux_value | default_in_mask);
            *(&scc_regs->iomux_altf1_insel_0 + reg_offset) =
                 (*(&scc_regs->iomux_altf1_insel_0 + reg_offset) & (~pin_mask));
            break;
        case GPIO_ALTFUNC_2:
            iomux_value =
              *(&scc_regs->iomux_altf2_default_in_0 + reg_offset) & (~pin_mask);
            *(&scc_regs->iomux_altf2_default_in_0 + reg_offset) =
                                                (iomux_value | default_in_mask);
            *(&scc_regs->iomux_altf2_insel_0 + reg_offset) =
                 (*(&scc_regs->iomux_altf2_insel_0 + reg_offset) & (~pin_mask));
            break;
        case GPIO_ALTFUNC_3:
            /* Reserved */
            break;
        default:
            /* Code should already returned error but just to be safe
             * return here as well
             */
            return SCC_INVALID_ARG;
    }

    return SCC_ERR_NONE;
}
