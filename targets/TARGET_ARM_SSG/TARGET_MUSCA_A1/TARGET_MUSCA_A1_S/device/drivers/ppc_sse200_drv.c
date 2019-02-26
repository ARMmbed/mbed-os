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

#include "ppc_sse200_drv.h"

/* SPCTRL PPCs control memory mapped registers access structure */
struct arm_spctrl_ppc_sse200_t {
    volatile uint32_t reserved[8];
    volatile uint32_t secppcintstat;  /* Secure PPC Interrupt Status */
    volatile uint32_t secppcintclr;   /* Secure PPC Interrupt Clear */
    volatile uint32_t secppcinten;    /* Secure PPC Interrupt Enable */
    volatile uint32_t reserved1[9];
    volatile uint32_t ahbnsppc0;      /* Non-Secure Access AHB slave Peripheral
                                         Protection Control #0 */
    volatile uint32_t reserved2[3];   /* Reserved for Future Non-secure Access
                                         AHB Slave Peripheral Protection
                                         Control */
    volatile uint32_t ahbnsppcexp0;   /* Expansion 0 Non_Secure Access AHB
                                         slave Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp1;   /* Expansion 1 Non_Secure Access AHB
                                         slave Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp2;   /* Expansion 2 Non_Secure Access AHB
                                         slave Peripheral Protection Control */
    volatile uint32_t ahbnsppcexp3;   /* Expansion 3 Non_Secure Access AHB
                                         slave Peripheral Protection Control */
    volatile uint32_t apbnsppc0;      /* Non-Secure Access APB slave Peripheral
                                         Protection Control 0 */
    volatile uint32_t apbnsppc1;      /* Non-Secure Access APB slave Peripheral
                                         Protection Control 1 */
    volatile uint32_t reserved3[2];   /* Non-Secure Access APB slave Peripheral
                                         Protection Control [3:1] */
    volatile uint32_t apbnsppcexp0;   /* Expansion 0 Non_Secure Access APB
                                         slave Peripheral Protection Control */
    volatile uint32_t apbnsppcexp1;   /* Expansion 1 Non_Secure Access APB
                                         slave Peripheral Protection Control */
    volatile uint32_t apbnsppcexp2;   /* Expansion 2 Non_Secure Access APB
                                         slave Peripheral Protection Control */
    volatile uint32_t apbnsppcexp3;   /* Expansion 3 Non_Secure Access APB
                                         slave Peripheral Protection Control */
    volatile uint32_t ahbspppc0;      /* Secure Unprivileged Access AHB slave
                                         Peripheral Protection Control 0 */
    volatile uint32_t reserved4[3];   /* Reserved for Future Secure Unprivileged
                                         Access AHB slave Peripheral Protection
                                         Control */
    volatile uint32_t ahbspppcexp0;   /* Expansion 0 Secure Unprivileged Access
                                         AHB slave Peripheral Protection
                                         Control */
    volatile uint32_t ahbspppcexp1;   /* Expansion 1 Secure Unprivileged Access
                                         AHB slave Peripheral Protection
                                         Control */
    volatile uint32_t ahbspppcexp2;   /* Expansion 2 Secure Unprivileged Access
                                         AHB slave Peripheral Protection
                                         Control */
    volatile uint32_t ahbspppcexp3;   /* Expansion 3 Secure Unprivileged Access
                                         AHB slave Peripheral Protection
                                         Control */
    volatile uint32_t apbspppc0;      /* Secure Unprivileged Access APB slave
                                         Peripheral 0 */
    volatile uint32_t apbspppc1;      /* Secure Unprivileged Access APB slave
                                         Peripheral 1 */
    volatile uint32_t reserved5[2];   /* Reserved for Future Secure Unprivileged
                                         Access APB slave Peripheral Protection
                                         Control */
    volatile uint32_t apbspppcexp0;   /* Expansion 0 Secure Unprivileged Access
                                         APB slave Peripheral Protection
                                         Control */
    volatile uint32_t apbspppcexp1;   /* Expansion 1 Secure Unprivileged Access
                                         APB slave Peripheral Protection
                                         Control */
    volatile uint32_t apbspppcexp2;   /* Expansion 2 Secure Unprivileged Access
                                         APB slave Peripheral Protection
                                         Control */
    volatile uint32_t apbspppcexp3;   /* Expansion 3 Secure Unprivileged Access
                                         APB slave Peripheral Protection
                                         Control */
};

/* NSPCTRL PPCs memory mapped register access structure */
struct arm_nspctrl_ppc_sse200_t {
    volatile uint32_t reserved[36];
    volatile uint32_t ahbnspppc0;
    volatile uint32_t reserved1[3];
    volatile uint32_t ahbnspppcexp0;
    volatile uint32_t ahbnspppcexp1;
    volatile uint32_t ahbnspppcexp2;
    volatile uint32_t ahbnspppcexp3;
    volatile uint32_t apbnspppc0;
    volatile uint32_t apbnspppc1;
    volatile uint32_t reserved2[2];
    volatile uint32_t apbnspppcexp0;
    volatile uint32_t apbnspppcexp1;
    volatile uint32_t apbnspppcexp2;
    volatile uint32_t apbnspppcexp3;
};

/* PPC interrupt position mask */
#define APB_PPC0_INT_POS_MASK     (1UL << 0)
#define APB_PPC1_INT_POS_MASK     (1UL << 1)
/* Reseved bits 2:3 */
#define APB_PPCEXP0_INT_POS_MASK  (1UL << 4)
#define APB_PPCEXP1_INT_POS_MASK  (1UL << 5)
#define APB_PPCEXP2_INT_POS_MASK  (1UL << 6)
#define APB_PPCEXP3_INT_POS_MASK  (1UL << 7)
/* Reseved bits 8:15 */
#define AHB_PPC0_INT_POS_MASK     (1UL << 16)
/* Reseved bits 17:19 */
#define AHB_PPCEXP0_INT_POS_MASK  (1UL << 20)
#define AHB_PPCEXP1_INT_POS_MASK  (1UL << 21)
#define AHB_PPCEXP2_INT_POS_MASK  (1UL << 22)
#define AHB_PPCEXP3_INT_POS_MASK  (1UL << 23)
/* Reseved bits 24:31 */

/* ARM PPC state definitions */
#define PPC_SSE200_INITIALIZED  (1 << 0)

/* Default peripheral states */
#define SECURE_AS_DEFAULT_PERIPHERAL_STATE  1
#define PRIVILEGE_ONLY_AS_DEFAULT_PERIPHERAL_STATE  1

void ppc_sse200_init(struct ppc_sse200_dev_t* dev,
                     enum ppc_sse200_name_t ppc_name)
{
    struct arm_spctrl_ppc_sse200_t* p_spctrl =
                         (struct arm_spctrl_ppc_sse200_t*)dev->cfg->spctrl_base;
    struct arm_nspctrl_ppc_sse200_t* p_nspctrl =
                       (struct arm_nspctrl_ppc_sse200_t*)dev->cfg->nspctrl_base;

    switch(ppc_name) {
        case AHB_PPC0:
            dev->data->p_ns_ppc  = &p_spctrl->ahbnsppc0;
            dev->data->p_sp_ppc  = &p_spctrl->ahbspppc0;
            dev->data->p_nsp_ppc = &p_nspctrl->ahbnspppc0;
            dev->data->int_bit_mask = AHB_PPC0_INT_POS_MASK;
            break;
        case AHB_PPC_EXP0:
            dev->data->p_ns_ppc  = &p_spctrl->ahbnsppcexp0;
            dev->data->p_sp_ppc  = &p_spctrl->ahbspppcexp0;
            dev->data->p_nsp_ppc = &p_nspctrl->ahbnspppcexp0;
            dev->data->int_bit_mask = AHB_PPCEXP0_INT_POS_MASK;
            break;
        case AHB_PPC_EXP1:
            dev->data->p_ns_ppc  = &p_spctrl->ahbnsppcexp1;
            dev->data->p_sp_ppc  = &p_spctrl->ahbspppcexp1;
            dev->data->p_nsp_ppc = &p_nspctrl->ahbnspppcexp1;
            dev->data->int_bit_mask = AHB_PPCEXP1_INT_POS_MASK;
            break;
        case AHB_PPC_EXP2:
            dev->data->p_ns_ppc  = &p_spctrl->ahbnsppcexp2;
            dev->data->p_sp_ppc  = &p_spctrl->ahbspppcexp2;
            dev->data->p_nsp_ppc = &p_nspctrl->ahbnspppcexp2;
            dev->data->int_bit_mask = AHB_PPCEXP2_INT_POS_MASK;
            break;
        case AHB_PPC_EXP3:
            dev->data->p_ns_ppc  = &p_spctrl->ahbnsppcexp3;
            dev->data->p_sp_ppc  = &p_spctrl->ahbspppcexp3;
            dev->data->p_nsp_ppc = &p_nspctrl->ahbnspppcexp3;
            dev->data->int_bit_mask = AHB_PPCEXP3_INT_POS_MASK;
            break;
        case APB_PPC0:
            dev->data->p_ns_ppc  = &p_spctrl->apbnsppc0;
            dev->data->p_sp_ppc  = &p_spctrl->apbspppc0;
            dev->data->p_nsp_ppc = &p_nspctrl->apbnspppc0;
            dev->data->int_bit_mask = APB_PPC0_INT_POS_MASK;
            break;
        case APB_PPC1:
            dev->data->p_ns_ppc  = &p_spctrl->apbnsppc1;
            dev->data->p_sp_ppc  = &p_spctrl->apbspppc1;
            dev->data->p_nsp_ppc = &p_nspctrl->apbnspppc1;
            dev->data->int_bit_mask = APB_PPC1_INT_POS_MASK;
            break;
        case APB_PPC_EXP0:
            dev->data->p_ns_ppc  = &p_spctrl->apbnsppcexp0;
            dev->data->p_sp_ppc  = &p_spctrl->apbspppcexp0;
            dev->data->p_nsp_ppc = &p_nspctrl->apbnspppcexp0;
            dev->data->int_bit_mask = APB_PPCEXP0_INT_POS_MASK;
            break;
        case APB_PPC_EXP1:
            dev->data->p_ns_ppc  = &p_spctrl->apbnsppcexp1;
            dev->data->p_sp_ppc  = &p_spctrl->apbspppcexp1;
            dev->data->p_nsp_ppc = &p_nspctrl->apbnspppcexp1;
            dev->data->int_bit_mask = APB_PPCEXP1_INT_POS_MASK;
            break;
        case APB_PPC_EXP2:
            dev->data->p_ns_ppc  = &p_spctrl->apbnsppcexp2;
            dev->data->p_sp_ppc  = &p_spctrl->apbspppcexp2;
            dev->data->p_nsp_ppc = &p_nspctrl->apbnspppcexp2;
            dev->data->int_bit_mask = APB_PPCEXP2_INT_POS_MASK;
            break;
        case APB_PPC_EXP3:
            dev->data->p_ns_ppc  = &p_spctrl->apbnsppcexp3;
            dev->data->p_sp_ppc  = &p_spctrl->apbspppcexp3;
            dev->data->p_nsp_ppc = &p_nspctrl->apbnspppcexp3;
            dev->data->int_bit_mask = APB_PPCEXP3_INT_POS_MASK;
            break;
        /* default:  The default is not defined intentionally to force the
         *           compiler to check that all enumeration values are
         *           covered in the switch.*/
    }

    dev->data->state = PPC_SSE200_INITIALIZED;
}

enum ppc_sse200_error_t ppc_sse200_config_peripheral(
                                          struct ppc_sse200_dev_t* dev,
                                          uint8_t periph,
                                          enum ppc_sse200_sec_attr_t sec_attr,
                                          enum ppc_sse200_priv_attr_t priv_attr)
{
    if(dev->data->state != PPC_SSE200_INITIALIZED) {
        return PPC_SSE200_NOT_INIT;
    }

    if(sec_attr == PPC_SSE200_SECURE_ONLY) {
        /* Sets secure attribute */
        *(dev->data->p_ns_ppc) &= ~(1U << periph);

        /* Uses secure unprivileged access address (SPCTRL) to set privilege
         * attribute */
        if(priv_attr == PPC_SSE200_PRIV_ONLY) {
            *(dev->data->p_sp_ppc) &= ~(1U << periph);
        } else {
            *(dev->data->p_sp_ppc) |= (1U << periph);
        }
    } else {
        /* Sets secure attribute */
        *(dev->data->p_ns_ppc) |= (1U << periph);

        /* Uses non-secure unprivileged access address (NSPCTRL) to set
         * privilege attribute */
        if(priv_attr == PPC_SSE200_PRIV_ONLY) {
            *(dev->data->p_nsp_ppc) &= ~(1U << periph);
        } else {
            *(dev->data->p_nsp_ppc) |= (1U << periph);
        }
    }

    return PPC_SSE200_ERR_NONE;
}

uint32_t ppc_sse200_is_periph_secure(struct ppc_sse200_dev_t* dev,
                                     uint8_t periph)
{
    if(dev->data->state != PPC_SSE200_INITIALIZED) {
        return SECURE_AS_DEFAULT_PERIPHERAL_STATE;
    }

    return ((*(dev->data->p_ns_ppc) & (1U << periph)) == 0);
}

uint32_t ppc_sse200_is_periph_priv_only(struct ppc_sse200_dev_t* dev,
                                        uint8_t periph)
{
    if(dev->data->state != PPC_SSE200_INITIALIZED) {
        return PRIVILEGE_ONLY_AS_DEFAULT_PERIPHERAL_STATE;
    }

    if ((*(dev->data->p_ns_ppc) & (1U << periph)) == 0) {
        /* Returns secure unprivileged access address (SPCTRL) */
        return ((*(dev->data->p_sp_ppc) & (1U << periph)) == 0);
    } else {
        /* Returns non-secure unprivileged access address (NSPCTRL) */
        return ((*(dev->data->p_nsp_ppc) & (1U << periph)) == 0);
    }
}

enum ppc_sse200_error_t ppc_sse200_irq_enable(struct ppc_sse200_dev_t* dev)
{
    struct arm_spctrl_ppc_sse200_t* p_spctrl =
                         (struct arm_spctrl_ppc_sse200_t*)dev->cfg->spctrl_base;

    if(dev->data->state != PPC_SSE200_INITIALIZED) {
        return PPC_SSE200_NOT_INIT;
    }

    p_spctrl->secppcinten |= dev->data->int_bit_mask;

    return PPC_SSE200_ERR_NONE;
}

void ppc_sse200_irq_disable(struct ppc_sse200_dev_t* dev)
{
    struct arm_spctrl_ppc_sse200_t* p_spctrl =
                         (struct arm_spctrl_ppc_sse200_t*)dev->cfg->spctrl_base;

    if(dev->data->state == PPC_SSE200_INITIALIZED) {
        p_spctrl->secppcinten &= ~(dev->data->int_bit_mask);
    }
}

void ppc_sse200_clear_irq(struct ppc_sse200_dev_t* dev)
{
    struct arm_spctrl_ppc_sse200_t* p_spctrl =
                         (struct arm_spctrl_ppc_sse200_t*)dev->cfg->spctrl_base;

    if(dev->data->state == PPC_SSE200_INITIALIZED) {
        p_spctrl->secppcintclr = dev->data->int_bit_mask;
    }
}

uint32_t ppc_sse200_irq_state(struct ppc_sse200_dev_t* dev)
{
    struct arm_spctrl_ppc_sse200_t* p_spctrl =
                         (struct arm_spctrl_ppc_sse200_t*)dev->cfg->spctrl_base;

    if(dev->data->state != PPC_SSE200_INITIALIZED) {
        return 0;
    }

    return ((p_spctrl->secppcintstat & dev->data->int_bit_mask) != 0);
}
