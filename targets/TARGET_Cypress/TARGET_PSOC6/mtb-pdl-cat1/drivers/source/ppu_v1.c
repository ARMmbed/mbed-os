/***************************************************************************//**
* \file ppu_v1.c
* \version 1.0
*
* This file provides the source code for ARM PPU driver
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXS28SRSS)

#include <stddef.h>
#include <ppu_v1.h>

void ppu_v1_init(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    /* Set edge sensitivity to masked for all input edges */
    ppu->IESR = 0;

    /* Mask all interrupts */
    ppu->IMR = PPU_V1_IMR_MASK;

    /* Acknowledge any interrupt left pending */
    ppu->ISR = PPU_V1_ISR_MASK;
}

/*
 * PWPR and PWSR registers
 */
int ppu_v1_request_power_mode(struct ppu_v1_reg *ppu, enum ppu_v1_mode ppu_mode)
{
    uint32_t power_policy;
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(ppu_mode < PPU_V1_MODE_COUNT);

    power_policy = ppu->PWPR & ~(PPU_V1_PWPR_POLICY | PPU_V1_PWPR_DYNAMIC_EN);
    ppu->PWPR = power_policy | ppu_mode;

    return CY_RET_SUCCESS;
}

int ppu_v1_set_power_mode(struct ppu_v1_reg *ppu, enum ppu_v1_mode ppu_mode)
{
    int status;

    status = ppu_v1_request_power_mode(ppu, ppu_mode);
    if (status != CY_RET_SUCCESS)
        return status;

    while ((ppu->PWSR & (PPU_V1_PWSR_PWR_STATUS | PPU_V1_PWSR_PWR_DYN_STATUS))
            != ppu_mode)
        continue;

    return CY_RET_SUCCESS;
}

int ppu_v1_request_operating_mode(struct ppu_v1_reg *ppu,
                                  enum ppu_v1_opmode op_mode)
{
    uint32_t power_policy;
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(op_mode < PPU_V1_OPMODE_COUNT);

    power_policy = ppu->PWPR & ~(PPU_V1_PWPR_OP_POLICY | PPU_V1_PWPR_OP_DYN_EN);
    ppu->PWPR = power_policy | (op_mode << PPU_V1_PWPR_OP_POLICY_POS);

    return CY_RET_SUCCESS;
}

void ppu_v1_opmode_dynamic_enable(struct ppu_v1_reg *ppu,
                                  enum ppu_v1_opmode min_dyn_mode)
{
    uint32_t power_policy;

    CY_ASSERT(ppu != NULL);
    CY_ASSERT(min_dyn_mode < PPU_V1_OPMODE_COUNT);

    power_policy = ppu->PWPR & ~PPU_V1_PWPR_OP_POLICY;
    ppu->PWPR = power_policy |
                PPU_V1_PWPR_OP_DYN_EN |
                (min_dyn_mode << PPU_V1_PWPR_OP_POLICY_POS);
    while ((ppu->PWSR & PPU_V1_PWSR_OP_DYN_STATUS) == 0)
        continue;
}

void ppu_v1_dynamic_enable(struct ppu_v1_reg *ppu,
                           enum ppu_v1_mode min_dyn_state)
{
    uint32_t power_policy;

    CY_ASSERT(ppu != NULL);
    CY_ASSERT(min_dyn_state < PPU_V1_MODE_COUNT);

    power_policy = ppu->PWPR & ~PPU_V1_PWPR_POLICY;
    ppu->PWPR = power_policy | PPU_V1_PWPR_DYNAMIC_EN | min_dyn_state;
    while ((ppu->PWSR & PPU_V1_PWSR_PWR_DYN_STATUS) == 0)
        continue;
}

void ppu_v1_lock_off_enable(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    ppu->PWPR |= PPU_V1_PWPR_OFF_LOCK_EN;
}

void ppu_v1_lock_off_disable(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    ppu->PWPR &= ~PPU_V1_PWPR_OFF_LOCK_EN;
}

enum ppu_v1_mode ppu_v1_get_power_mode(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return (enum ppu_v1_mode)(ppu->PWSR & PPU_V1_PWSR_PWR_STATUS);
}

enum ppu_v1_mode ppu_v1_get_programmed_power_mode(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return (enum ppu_v1_mode)(ppu->PWPR & PPU_V1_PWPR_POLICY);
}

enum ppu_v1_opmode ppu_v1_get_operating_mode(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return (enum ppu_v1_opmode)
        ((ppu->PWSR & PPU_V1_PWSR_OP_STATUS) >> PPU_V1_PWSR_OP_STATUS_POS);
}

enum ppu_v1_opmode ppu_v1_get_programmed_operating_mode(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return (enum ppu_v1_opmode)
        ((ppu->PWPR & PPU_V1_PWPR_OP_POLICY) >> PPU_V1_PWPR_OP_POLICY_POS);
}

bool ppu_v1_is_dynamic_enabled(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return ((ppu->PWSR & PPU_V1_PWSR_PWR_DYN_STATUS) != 0);
}

bool ppu_v1_is_locked(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return ((ppu->PWSR & PPU_V1_PWSR_OFF_LOCK_STATUS) != 0);
}

/*
 * DISR register
 */
bool ppu_v1_is_power_devactive_high(struct ppu_v1_reg *ppu,
                                    enum ppu_v1_mode ppu_mode)
{
    CY_ASSERT(ppu != NULL);

    return (ppu->DISR &
            (1 << (ppu_mode + PPU_V1_DISR_PWR_DEVACTIVE_STATUS_POS))) != 0;
}

bool ppu_v1_is_op_devactive_high(struct ppu_v1_reg *ppu,
                                 enum ppu_v1_op_devactive op_devactive)
{
    CY_ASSERT(ppu != NULL);

    return (ppu->DISR &
            (1 << (op_devactive + PPU_V1_DISR_OP_DEVACTIVE_STATUS_POS))) != 0;
}

/*
 * UNLK register
 */
void ppu_v1_off_unlock(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    ppu->UNLK = PPU_V1_UNLK_OFF_UNLOCK;
}

/*
 * PWCR register
 */
void ppu_v1_disable_devactive(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    ppu->PWCR &= ~PPU_V1_PWCR_DEV_ACTIVE_EN;
}

void ppu_v1_disable_handshake(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    ppu->PWCR &= ~PPU_V1_PWCR_DEV_REQ_EN;
}

/*
 * Interrupt registers: IMR, AIMR, ISR, AISR, IESR, OPSR
 */
void ppu_v1_interrupt_mask(struct ppu_v1_reg *ppu, unsigned int mask)
{
    CY_ASSERT(ppu != NULL);

    ppu->IMR |= mask & PPU_V1_IMR_MASK;
}

void ppu_v1_additional_interrupt_mask(struct ppu_v1_reg *ppu, unsigned int mask)
{
    CY_ASSERT(ppu != NULL);

    ppu->AIMR |= mask & PPU_V1_AIMR_MASK;
}

void ppu_v1_interrupt_unmask(struct ppu_v1_reg *ppu, unsigned int mask)
{
    CY_ASSERT(ppu != NULL);

    ppu->IMR &= ~(mask & PPU_V1_IMR_MASK);
}

void ppu_v1_additional_interrupt_unmask(struct ppu_v1_reg *ppu,
    unsigned int mask)
{
    CY_ASSERT(ppu != NULL);

    ppu->AIMR &= ~(mask & PPU_V1_AIMR_MASK);
}

bool ppu_v1_is_additional_interrupt_pending(struct ppu_v1_reg *ppu,
    unsigned int mask)
{
    return (ppu->AISR & (mask & PPU_V1_AISR_MASK)) != 0;
}

void ppu_v1_ack_interrupt(struct ppu_v1_reg *ppu, unsigned int mask)
{
    CY_ASSERT(ppu != NULL);

    ppu->ISR &= mask & PPU_V1_IMR_MASK;
}

void ppu_v1_ack_additional_interrupt(struct ppu_v1_reg *ppu, unsigned int mask)
{
    CY_ASSERT(ppu != NULL);

    ppu->AISR &= mask & PPU_V1_AIMR_MASK;
}

void ppu_v1_set_input_edge_sensitivity(struct ppu_v1_reg *ppu,
    enum ppu_v1_mode ppu_mode, enum ppu_v1_edge_sensitivity edge_sensitivity)
{
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(ppu_mode < PPU_V1_MODE_COUNT);
    CY_ASSERT((edge_sensitivity & ~PPU_V1_EDGE_SENSITIVITY_MASK) == 0);

    /* Clear current settings */
    ppu->IESR &= ~(PPU_V1_EDGE_SENSITIVITY_MASK <<
                   (ppu_mode * PPU_V1_BITS_PER_EDGE_SENSITIVITY));

    /* Update settings */
    ppu->IESR |= edge_sensitivity <<
                 (ppu_mode * PPU_V1_BITS_PER_EDGE_SENSITIVITY);
}

enum ppu_v1_edge_sensitivity ppu_v1_get_input_edge_sensitivity(
    struct ppu_v1_reg *ppu, enum ppu_v1_mode ppu_mode)
{
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(ppu_mode < PPU_V1_MODE_COUNT);

    return (enum ppu_v1_edge_sensitivity)(
            (ppu->IESR >> (ppu_mode * PPU_V1_BITS_PER_EDGE_SENSITIVITY)) &
            PPU_V1_EDGE_SENSITIVITY_MASK);
}

void ppu_v1_ack_power_active_edge_interrupt(struct ppu_v1_reg *ppu,
    enum ppu_v1_mode ppu_mode)
{
    ppu->ISR = 1 << (ppu_mode + PPU_V1_ISR_ACTIVE_EDGE_POS);
}

bool ppu_v1_is_power_active_edge_interrupt(struct ppu_v1_reg *ppu,
    enum ppu_v1_mode ppu_mode)
{
    return ppu->ISR & (1 << (ppu_mode + PPU_V1_ISR_ACTIVE_EDGE_POS));
}

void ppu_v1_set_op_active_edge_sensitivity(struct ppu_v1_reg *ppu,
    enum ppu_v1_op_devactive op_devactive,
    enum ppu_v1_edge_sensitivity edge_sensitivity)
{
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(op_devactive < PPU_V1_OP_DEVACTIVE_COUNT);
    CY_ASSERT((edge_sensitivity & ~PPU_V1_EDGE_SENSITIVITY_MASK) == 0);

    /* Clear current settings */
    ppu->OPSR &= ~(PPU_V1_EDGE_SENSITIVITY_MASK <<
                   (op_devactive * PPU_V1_BITS_PER_EDGE_SENSITIVITY));

    /* Update settings */
    ppu->OPSR |= edge_sensitivity <<
                 (op_devactive * PPU_V1_BITS_PER_EDGE_SENSITIVITY);
}

enum ppu_v1_edge_sensitivity ppu_v1_get_op_active_edge_sensitivity(
    struct ppu_v1_reg *ppu, enum ppu_v1_op_devactive op_devactive)
{
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(op_devactive < PPU_V1_OP_DEVACTIVE_COUNT);

    return (enum ppu_v1_edge_sensitivity)(
            (ppu->OPSR >> (op_devactive * PPU_V1_BITS_PER_EDGE_SENSITIVITY)) &
            PPU_V1_EDGE_SENSITIVITY_MASK);
}

void ppu_v1_ack_op_active_edge_interrupt(struct ppu_v1_reg *ppu,
    enum ppu_v1_op_devactive op_devactive)
{
    ppu->ISR = 1 << (op_devactive + PPU_V1_ISR_OP_ACTIVE_EDGE_POS);
}

bool ppu_v1_is_op_active_edge_interrupt(struct ppu_v1_reg *ppu,
    enum ppu_v1_op_devactive op_devactive)
{
    return ppu->ISR & (1 << (op_devactive + PPU_V1_ISR_OP_ACTIVE_EDGE_POS));
}

bool ppu_v1_is_dyn_policy_min_interrupt(struct ppu_v1_reg *ppu)
{
    return ppu->ISR & PPU_V1_ISR_DYN_POLICY_MIN_IRQ;
}

/*
 * IDR0 register
 */
unsigned int ppu_v1_get_num_opmode(struct ppu_v1_reg *ppu)
{
    return ((ppu->IDR0 & PPU_V1_IDR0_NUM_OPMODE)
            >> PPU_V1_IDR0_NUM_OPMODE_POS) + 1;
}

/*
 * AIDR register
 */
unsigned int ppu_v1_get_arch_id(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    return (ppu->AIDR & (PPU_V1_AIDR_ARCH_REV_MINOR |
                         PPU_V1_AIDR_ARCH_REV_MAJOR));
}
#endif /* CY_IP_MXS28SRSS */
