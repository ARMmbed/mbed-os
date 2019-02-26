/*
 * Copyright (c) 2017-2019 Arm Limited
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

#include "musca_a1_scc_drv.h"

struct musca_a1_scc_reg_map_t {
    volatile uint32_t reset_ctrl;             /* 0x00 RW Reset Control Register */
    volatile uint32_t clk_ctrl;               /* 0x04 RW Clock Control Register*/
    volatile uint32_t pwr_ctrl;               /* 0x08 RW Power Control Register*/
    volatile uint32_t pll_ctrl;               /* 0x0C RW Power Control Register */
    volatile uint32_t dbg_ctrl;               /* 0x10 RW Debug Control Register */
    volatile uint32_t sram_ctrl;              /* 0x14 RW SRAM Control Register */
    volatile uint32_t intr_ctrl;              /* 0x18 RW Interupt Control Register */
    volatile uint32_t reserved1;              /* 0x1C RW reserved */
    volatile uint32_t cpu0_vtor_sram;         /* 0x20 RW Reset vector for CPU0 Secure Mode */
    volatile uint32_t cpu0_vtor_flash;        /* 0x24 RW Reset vector for CPU0 Secure Mode */
    volatile uint32_t cpu1_vtor_sram;         /* 0x28 RW Reset vector for CPU1 Secure Mode */
    volatile uint32_t cpu1_vtor_flash;        /* 0x2C RW Reset vector for CPU0 Secure Mode */
    volatile uint32_t iomux_main_insel;       /* 0x30 RW Main function in data select */
    volatile uint32_t iomux_main_outsel;      /* 0x34 RW Main function out data select */
    volatile uint32_t iomux_main_oensel;      /* 0x38 RW Main function out enable select */
    volatile uint32_t iomux_main_default_in;  /* 0x3C RW Main function default in select */
    volatile uint32_t iomux_altf1_insel;      /* 0x40 RW Alt function 1 in data select */
    volatile uint32_t iomux_altf1_outsel;     /* 0x44 RW Alt function 1 out data select */
    volatile uint32_t iomux_altf1_oensel;     /* 0x48 RW Alt function 1 out enable select */
    volatile uint32_t iomux_altf1_default_in; /* 0x4C RW Alt function 1 default in select */
    volatile uint32_t iomux_altf2_insel;      /* 0x50 RW Alt function 2 in data select */
    volatile uint32_t iomux_altf2_outsel;     /* 0x54 RW Alt function 2 out data select */
    volatile uint32_t iomux_altf2_oensel;     /* 0x58 RW Alt function 2 out enable select */
    volatile uint32_t iomux_altf2_default_in; /* 0x5C RW Alt function 2 default in select */
    volatile uint32_t pvt_ctrl;               /* 0x60 RW PVT control register */
    volatile uint32_t spare0;                 /* 0x64 RW reserved */
    volatile uint32_t iopad_ds0;              /* 0x68 RW Drive Select 0 */
    volatile uint32_t iopad_ds1;              /* 0x6C RW Drive Select 1 */
    volatile uint32_t iopad_pe;               /* 0x70 RW Pull Enable */
    volatile uint32_t iopad_ps;               /* 0x74 RW Pull Select */
    volatile uint32_t iopad_sr;               /* 0x78 RW Slew Select */
    volatile uint32_t iopad_is;               /* 0x7C RW Input Select */
    volatile uint32_t sram_rw_margine;        /* 0x80 RW reserved */
    volatile uint32_t static_conf_sig0;       /* 0x84 RW Static configuration */
    volatile uint32_t static_conf_sig1;       /* 0x88 RW Static configuration */
    volatile uint32_t req_set;                /* 0x8C RW External Event Enable */
    volatile uint32_t req_clear;              /* 0x90 RW External Event Clear */
    volatile uint32_t iomux_altf3_insel;      /* 0x94 RW Alt function 3 in data select */
    volatile uint32_t iomux_altf3_outsel;     /* 0x98 RW Alt function 3 out data select */
    volatile uint32_t iomux_altf3_oensel;     /* 0x9C RW Alt function 3 out enable select */
    volatile uint32_t iomux_altf3_default_in; /* 0xA0 RW Alt function 3 default in select */
    volatile uint32_t pcsm_ctrl_override;     /* 0xA4 RW Q-Channels QACTIVE Override */
    volatile uint32_t pd_cpu0_iso_override;   /* 0xA8 RW CPU0 Isolation Override */
    volatile uint32_t pd_cpu1_iso_override;   /* 0xAC RW CPU1 Isolation Override */
    volatile uint32_t sys_sram_rw_assist0;    /* 0xB0 RW CPU0 icache sram ldata */
    volatile uint32_t sys_sram_rw_assist1;    /* 0xB4 RW CPU0 icache sram tag */
    volatile uint32_t sys_sram_rw_assist2;    /* 0xB8 RW CPU1 icache sram ldata */
    volatile uint32_t sys_sram_rw_assist3;    /* 0xBC RW CPU1 icache sram tag */
    volatile uint32_t sys_sram_rw_assist4;    /* 0xC0 RW System sram */
    volatile uint32_t sys_sram_rw_assist5;    /* 0xC4 RW System sram */
    volatile uint32_t reserved2[3];           /* reserved */
    volatile uint32_t crypto_sram_rw_assist0; /* 0xD4 RW Crypto ram */
    volatile uint32_t crypto_sram_rw_assist1; /* 0xD8 RW Crypto sec sram */
    volatile uint32_t crypto_sram_rw_assist2; /* 0xDC RW Reserved */
    volatile uint32_t req_edge_sel;           /* 0xC0 RW Power clock request edge select */
    volatile uint32_t req_enable;             /* 0xC4 RW Power clock request enable */
    volatile uint32_t reserved3[28];          /* reserved */
    volatile uint32_t chip_id;                /* 0x100 RO Chip ID 0x0797_0477 */
    volatile uint32_t clock_status;           /* 0x104 RO Clock status */
    volatile uint32_t io_in_status;           /* 0x108 RO I/O in status */
};

/**
 * \brief Clears selected alternate functions for selected pins
 *
 * \param[in] dev        SCC registers base address \ref musca_a1_scc_reg_map_t
 * \param[in] func_mask  Bitmask of alternate functions to clear
 *                       \ref gpio_altfunc_mask_t
 * \param[in] pin_mask   Pin mask for the alternate functions
 */
static void scc_clear_alt_func(struct musca_a1_scc_reg_map_t* scc_regs,
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
    if (func_mask & GPIO_ALTFUNC_3_MASK) {
        scc_regs->iomux_altf3_insel &= ~pin_mask;
        scc_regs->iomux_altf3_outsel &= ~pin_mask;
        scc_regs->iomux_altf3_oensel &= ~pin_mask;
    }
}

void musca_a1_scc_set_alt_func(struct musca_a1_scc_dev_t* dev,
                          enum gpio_altfunc_t altfunc, uint32_t pin_mask)
{
    struct musca_a1_scc_reg_map_t* scc_regs =
                                    (struct musca_a1_scc_reg_map_t*) dev->cfg->base;
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

        case GPIO_ALTFUNC_3:
            insel = &scc_regs->iomux_altf3_insel;
            outsel = &scc_regs->iomux_altf3_outsel;
            oensel = &scc_regs->iomux_altf3_oensel;
            altfunc_to_clear = GPIO_ALTFUNC_3_NEG_MASK;
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

void musca_a1_scc_set_pinmode(struct musca_a1_scc_dev_t* dev, uint32_t pin_mask,
                         enum pinmode_select_t mode)
{
    struct musca_a1_scc_reg_map_t* scc_regs =
                                    (struct musca_a1_scc_reg_map_t*) dev->cfg->base;

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

void musca_a1_scc_set_default_in(struct musca_a1_scc_dev_t* dev,
                            enum gpio_altfunc_t altfunc,
                            uint32_t default_in_mask,
                            uint32_t default_in_value)
{
    struct musca_a1_scc_reg_map_t* scc_regs =
                                    (struct musca_a1_scc_reg_map_t*) dev->cfg->base;
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
            scc_regs->iomux_main_insel = (scc_regs->iomux_main_insel & ~default_in_mask);
            break;

        case GPIO_ALTFUNC_1:
            iomux_value = scc_regs->iomux_altf1_default_in & ~default_in_mask;
            iomux_value |= (default_in_value & default_in_mask);
            scc_regs->iomux_altf1_default_in = iomux_value;
            scc_regs->iomux_altf1_insel = (scc_regs->iomux_altf1_insel & ~default_in_mask);
            break;

        case GPIO_ALTFUNC_2:
            iomux_value = scc_regs->iomux_altf2_default_in & ~default_in_mask;
            iomux_value |= (default_in_value & default_in_mask);
            scc_regs->iomux_altf2_default_in = iomux_value;
            scc_regs->iomux_altf2_insel = (scc_regs->iomux_altf2_insel & ~default_in_mask);
            break;

        case GPIO_ALTFUNC_3:
            iomux_value = scc_regs->iomux_altf3_default_in & ~default_in_mask;
            iomux_value |= (default_in_value & default_in_mask);
            scc_regs->iomux_altf3_default_in = iomux_value;
            scc_regs->iomux_altf3_insel = (scc_regs->iomux_altf3_insel & ~default_in_mask);
            break;
        default:
        break;
    }
}
