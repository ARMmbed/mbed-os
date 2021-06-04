/***************************************************************************//**
* \file cy_pd_ppu.c
* \version 1.0
*
* This file provides the source code for ARM PPU Platform PD specific driver
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

#include <stdbool.h>
#include <ppu_v1.h>
#include <cy_pd_ppu.h>
#include "cy_syslib.h"

/* Static Functions */
static cy_pd_ppu_status_t cy_pd_ppu_enable_irq(struct ppu_v1_reg *ppu);
static cy_pd_ppu_status_t cy_pd_ramc_ppu0_irq_enable();
static cy_pd_ppu_status_t cy_pd_ramc_ppu1_irq_enable();
static cy_pd_ppu_status_t cy_pd_ramc_ppu2_irq_enable();
static cy_pd_ppu_status_t cy_pd_ppu_main_irq_enable(void);
static void cy_pd_ramc_ppu0_isr(void);
static void cy_pd_ramc_ppu1_isr(void);
static void cy_pd_ramc_ppu2_isr(void);
static void cy_pd_ppu_main_isr(void);
static void cy_pd_ppu_interrupt_handler(struct ppu_v1_reg *ppu);

/**
* \addtogroup group_pd_ppu_functions
* \{
*/

/*******************************************************************************
* Function Name: cy_pd_ppu_init
****************************************************************************//**
*
* Initializes the PD PPU Driver:
*
*  \param ppu
*  This Parameter contains PPU base pointer for which the intialization has 
*  to be done, it will point to one of the below PPU's
*  Main PPU
*  RAMC_PPU0
*  RAMC_PPU1
*  RAMC_PPU2
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

cy_pd_ppu_status_t cy_pd_ppu_init(struct ppu_v1_reg *ppu)
{
    cy_pd_ppu_status_t status;
    uint32_t state;
    CY_ASSERT(ppu != NULL);

    ppu_v1_init(ppu);

    cy_pd_ppu_enable_irq(ppu);

    status = cy_pd_ppu_get_power_mode(ppu, &state);
    if (status != CY_PD_PPU_SUCCESS)
        return status;

    if (state == PPU_V1_MODE_ON) {
        ppu_v1_interrupt_unmask(ppu, PPU_V1_IMR_DYN_POLICY_MIN_IRQ_MASK);
        ppu_v1_dynamic_enable(ppu, PPU_V1_MODE_OFF);
    }

    return CY_PD_PPU_SUCCESS;
}

/*******************************************************************************
* Function Name: cy_pd_ppu_get_power_mode
****************************************************************************//**
*
* Gets the current power mode of the particular PPU
*
*  \param ppu
*  This Parameter contains PPU base pointer for which the intialization has 
*  to be done, it will point to one of the below PPU's
*  Main PPU
*  RAMC_PPU0
*  RAMC_PPU1
*  RAMC_PPU2
*
*  \param mode
*  Contains the current mode of the PPU.
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

cy_pd_ppu_status_t cy_pd_ppu_get_power_mode(struct ppu_v1_reg *ppu, uint32_t *mode)
{
    CY_ASSERT(ppu != NULL);
    *mode = ppu_v1_get_power_mode(ppu);
    CY_ASSERT(*mode < PPU_V1_MODE_COUNT);

    return CY_PD_PPU_SUCCESS;
}

/*******************************************************************************
* Function Name: cy_pd_ppu_set_power_mode
****************************************************************************//**
*
* Sets the required power mode of the particular PPU
*
*  \param ppu
*  This Parameter contains PPU base pointer for which the intialization has 
*  to be done, it will point to one of the below PPU's
*  Main PPU
*  RAMC_PPU0
*  RAMC_PPU1
*  RAMC_PPU2
*
*  \param mode
*  Contains the future power mode to be set for the PPU.
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

cy_pd_ppu_status_t cy_pd_ppu_set_power_mode(struct ppu_v1_reg *ppu, uint32_t mode)
{
    cy_pd_ppu_status_t status = CY_PD_PPU_INVALID_STATE;
    CY_ASSERT(ppu != NULL);
    CY_ASSERT(mode < PPU_V1_MODE_COUNT);

    switch (mode) {
    case PPU_V1_MODE_OFF:
        ppu_v1_set_input_edge_sensitivity(ppu,
                                          PPU_V1_MODE_ON,
                                          PPU_V1_EDGE_SENSITIVITY_MASKED);
        ppu_v1_interrupt_mask(ppu, PPU_V1_IMR_DYN_POLICY_MIN_IRQ_MASK);
        status = (cy_pd_ppu_status_t)ppu_v1_set_power_mode(ppu, PPU_V1_MODE_OFF);
        ppu_v1_lock_off_disable(ppu);
        ppu_v1_off_unlock(ppu);

        CY_ASSERT(status == CY_PD_PPU_SUCCESS);
        break;

    case PPU_V1_MODE_ON:
        ppu_v1_interrupt_unmask(ppu, PPU_V1_IMR_DYN_POLICY_MIN_IRQ_MASK);
        ppu_v1_set_input_edge_sensitivity(ppu,
                                          PPU_V1_MODE_ON,
                                          PPU_V1_EDGE_SENSITIVITY_MASKED);
        status = (cy_pd_ppu_status_t)ppu_v1_set_power_mode(ppu, PPU_V1_MODE_ON);
        ppu_v1_dynamic_enable(ppu, PPU_V1_MODE_OFF);

        CY_ASSERT(status == CY_PD_PPU_SUCCESS);
        break;

    case PPU_V1_MODE_MEM_RET:
        status = (cy_pd_ppu_status_t)ppu_v1_set_power_mode(ppu, PPU_V1_MODE_MEM_RET);
        CY_ASSERT(status == CY_PD_PPU_SUCCESS);
        break;

    case PPU_V1_MODE_FULL_RET:
        status = (cy_pd_ppu_status_t)ppu_v1_set_power_mode(ppu, PPU_V1_MODE_FULL_RET);
        CY_ASSERT(status == CY_PD_PPU_SUCCESS);
        break;

    default:
        return CY_PD_PPU_BAD_PARAM;
    }

    return status;
}

/*******************************************************************************
* Function Name: cy_pd_ppu_reset
****************************************************************************//**
*
* Resets the PD using PPU
*
*  \param ppu
*  This Parameter contains PPU base pointer for which the intialization has 
*  to be done, it will point to one of the below PPU's
*  Main PPU
*  RAMC_PPU0
*  RAMC_PPU1
*  RAMC_PPU2
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

cy_pd_ppu_status_t cy_pd_ppu_reset(struct ppu_v1_reg *ppu)
{
    cy_pd_ppu_status_t status;
    CY_ASSERT(ppu != NULL);

    status = cy_pd_ppu_set_power_mode(ppu, PPU_V1_MODE_OFF);
    if (status == CY_PD_PPU_SUCCESS)
        status = cy_pd_ppu_set_power_mode(ppu, PPU_V1_MODE_ON);

    return status;
}

/*******************************************************************************
* Function Name: cy_pd_ppu_enable_irq
****************************************************************************//**
*
* High level API , which will call the PD specific PPU IRQ enables
*
*  \param ppu
*  This Parameter contains PPU base pointer for which the intialization has 
*  to be done, it will point to one of the below PPU's
*  Main PPU
*  RAMC_PPU0
*  RAMC_PPU1
*  RAMC_PPU2
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

static cy_pd_ppu_status_t cy_pd_ppu_enable_irq(struct ppu_v1_reg *ppu)
{
    if((uint32_t)ppu == (uint32_t)CY_PPU_MAIN_BASE)
        return cy_pd_ppu_main_irq_enable();
    else if((uint32_t)ppu == (uint32_t)CY_RAMC_PPU0_BASE)
        return cy_pd_ramc_ppu0_irq_enable();
    else if((uint32_t)ppu == (uint32_t)CY_RAMC_PPU1_BASE)
        return cy_pd_ramc_ppu1_irq_enable();
    else if((uint32_t)ppu == (uint32_t)CY_RAMC_PPU2_BASE)
        return cy_pd_ramc_ppu2_irq_enable();

    return CY_PD_PPU_ERROR;
}

/*******************************************************************************
* Function Name: cy_pd_ramc_ppu0_irq_enable
****************************************************************************//**
*
* Enables the IRQ for RAMC_PPU0
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

static cy_pd_ppu_status_t cy_pd_ramc_ppu0_irq_enable(void)
{
    cy_stc_sysint_t ramc_ppu0_irq_cfg =
    {
        .intrSrc = cpuss_interrupt_ppu_sramc0_IRQn,
        .intrPriority = 7UL
    };

    /* Hook interrupt service routine. */
    if(Cy_SysInt_Init(&ramc_ppu0_irq_cfg, &cy_pd_ramc_ppu0_isr))
        return CY_PD_PPU_ERROR;

    /* Enable interrupt in NVIC. */
    NVIC_EnableIRQ((IRQn_Type) ramc_ppu0_irq_cfg.intrSrc);

    return CY_PD_PPU_SUCCESS;
}

/*******************************************************************************
* Function Name: cy_pd_ramc_ppu1_irq_enable
****************************************************************************//**
*
* Enables the IRQ for RAMC_PPU1
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

static cy_pd_ppu_status_t cy_pd_ramc_ppu1_irq_enable(void)
{
    cy_stc_sysint_t ramc_ppu1_irq_cfg =
    {
        .intrSrc = cpuss_interrupt_ppu_sramc1_IRQn,
        .intrPriority = 7UL
    };

    /* Hook interrupt service routine. */
    if(Cy_SysInt_Init(&ramc_ppu1_irq_cfg, &cy_pd_ramc_ppu1_isr))
        return CY_PD_PPU_ERROR;

    /* Enable interrupt in NVIC. */
    NVIC_EnableIRQ((IRQn_Type) ramc_ppu1_irq_cfg.intrSrc);

    return CY_PD_PPU_SUCCESS;
}

/*******************************************************************************
* Function Name: cy_pd_ramc_ppu2_irq_enable
****************************************************************************//**
*
* Enables the IRQ for RAMC_PPU2
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

static cy_pd_ppu_status_t cy_pd_ramc_ppu2_irq_enable(void)
{
    cy_stc_sysint_t ramc_ppu2_irq_cfg =
    {
        .intrSrc = cpuss_interrupt_ppu_sramc2_IRQn,
        .intrPriority = 7UL
    };

    /* Hook interrupt service routine. */
    if(Cy_SysInt_Init(&ramc_ppu2_irq_cfg, &cy_pd_ramc_ppu2_isr))
        return CY_PD_PPU_ERROR;

    /* Enable interrupt in NVIC. */
    NVIC_EnableIRQ((IRQn_Type) ramc_ppu2_irq_cfg.intrSrc);

    return CY_PD_PPU_SUCCESS;
}

/*******************************************************************************
* Function Name: cy_pd_ppu_main_irq_enable
****************************************************************************//**
*
* Enables the IRQ for MAIN PPU
*
*  \return the PD PPU API status \ref cy_pd_ppu_status_t.
*
*******************************************************************************/

static cy_pd_ppu_status_t cy_pd_ppu_main_irq_enable(void)
{
    cy_stc_sysint_t ppu_main_irq_cfg =
    {
        .intrSrc = srss_interrupt_main_ppu_IRQn,
        .intrPriority = 7UL
    };

    /* Hook interrupt service routine. */
    if(Cy_SysInt_Init(&ppu_main_irq_cfg, &cy_pd_ppu_main_isr))
        return CY_PD_PPU_ERROR;

    /* Enable interrupt in NVIC. */
    NVIC_EnableIRQ((IRQn_Type) ppu_main_irq_cfg.intrSrc);

    return CY_PD_PPU_SUCCESS;
}

/*******************************************************************************
* Function Name: cy_pd_ramc_ppu0_isr
****************************************************************************//**
*
* ISR Handler for RAMC_PPU0 IRQ
*
*******************************************************************************/

static void cy_pd_ramc_ppu0_isr(void)
{
    cy_pd_ppu_interrupt_handler((struct ppu_v1_reg *)RAMC_PPU0_BASE);
}

/*******************************************************************************
* Function Name: cy_pd_ramc_ppu1_isr
****************************************************************************//**
*
* ISR Handler for RAMC_PPU1 IRQ
*
*******************************************************************************/

static void cy_pd_ramc_ppu1_isr(void)
{
    cy_pd_ppu_interrupt_handler((struct ppu_v1_reg *)RAMC_PPU1_BASE);
}

/*******************************************************************************
* Function Name: cy_pd_ramc_ppu2_isr
****************************************************************************//**
*
* ISR Handler for RAMC_PPU2 IRQ
*
*******************************************************************************/

static void cy_pd_ramc_ppu2_isr(void)
{
    cy_pd_ppu_interrupt_handler((struct ppu_v1_reg *)RAMC_PPU2_BASE);
}

/*******************************************************************************
* Function Name: cy_pd_ppu_main_isr
****************************************************************************//**
*
* ISR Handler for MAIN PPU IRQ
*
*******************************************************************************/

static void cy_pd_ppu_main_isr(void)
{
    cy_pd_ppu_interrupt_handler((struct ppu_v1_reg *)PWRMODE_PPU_MAIN);
}

/*******************************************************************************
* Function Name: cy_pd_ppu_interrupt_handler
****************************************************************************//**
*
* Common IR Handler for all the PPU IRQ's
*
*  \param ppu
*  This Parameter contains PPU base pointer for which the intialization has 
*  to be done, it will point to one of the below PPU's
*  Main PPU
*  RAMC_PPU0
*  RAMC_PPU1
*  RAMC_PPU2
*
*******************************************************************************/

static void cy_pd_ppu_interrupt_handler(struct ppu_v1_reg *ppu)
{
    CY_ASSERT(ppu != NULL);

    /* ON request interrupt */
    if (ppu_v1_is_power_active_edge_interrupt(ppu, PPU_V1_MODE_ON)) {
        ppu_v1_ack_power_active_edge_interrupt(ppu, PPU_V1_MODE_ON);
        ppu_v1_set_input_edge_sensitivity(ppu,
                                          PPU_V1_MODE_ON,
                                          PPU_V1_EDGE_SENSITIVITY_MASKED);
        ppu_v1_interrupt_unmask(ppu, PPU_V1_IMR_DYN_POLICY_MIN_IRQ_MASK);

    /* Minimum policy reached interrupt */
    } else if (ppu_v1_is_dyn_policy_min_interrupt(ppu)) {
        ppu_v1_ack_interrupt(ppu, PPU_V1_ISR_DYN_POLICY_MIN_IRQ);
        ppu_v1_interrupt_mask(ppu, PPU_V1_IMR_DYN_POLICY_MIN_IRQ_MASK);

        /*
         * Enable the core PACTIVE ON signal rising edge interrupt then check if
         * the PACTIVE ON signal is high. If it is high, we may have missed the
         * transition from low to high. In that case, just disable the interrupt
         * and acknowledge it in case it is pending. There is no need to send an
         * update request as one has already been queued.
         */
        ppu_v1_set_input_edge_sensitivity(ppu,
                                          PPU_V1_MODE_ON,
                                          PPU_V1_EDGE_SENSITIVITY_RISING_EDGE);
        if (ppu_v1_is_power_devactive_high(ppu, PPU_V1_MODE_ON)) {
            ppu_v1_set_input_edge_sensitivity(ppu,
                                              PPU_V1_MODE_ON,
                                              PPU_V1_EDGE_SENSITIVITY_MASKED);
            ppu_v1_ack_power_active_edge_interrupt(ppu, PPU_V1_MODE_ON);
            ppu_v1_interrupt_unmask(ppu, PPU_V1_IMR_DYN_POLICY_MIN_IRQ_MASK);
        }
    }
}
/** \} group_pd_ppu_functions */

#endif /* CY_IP_MXS28SRSS */
