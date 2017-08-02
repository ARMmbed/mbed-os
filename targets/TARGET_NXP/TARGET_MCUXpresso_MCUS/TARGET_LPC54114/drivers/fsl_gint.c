/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_gint.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to GINT bases for each instance. */
static GINT_Type *const s_gintBases[FSL_FEATURE_SOC_GINT_COUNT] = GINT_BASE_PTRS;

/*! @brief Clocks for each instance. */
static const clock_ip_name_t s_gintClocks[FSL_FEATURE_SOC_GINT_COUNT] = GINT_CLOCKS;

/*! @brief Resets for each instance. */
static const reset_ip_name_t s_gintResets[FSL_FEATURE_SOC_GINT_COUNT] = GINT_RSTS;

/* @brief Irq number for each instance */
static const IRQn_Type s_gintIRQ[FSL_FEATURE_SOC_GINT_COUNT] = GINT_IRQS;

/*! @brief Callback function array for GINT(s). */
static gint_cb_t s_gintCallback[FSL_FEATURE_SOC_GINT_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t GINT_GetInstance(GINT_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_GINT_COUNT; instance++)
    {
        if (s_gintBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < FSL_FEATURE_SOC_GINT_COUNT);

    return instance;
}

void GINT_Init(GINT_Type *base)
{
    uint32_t instance;

    instance = GINT_GetInstance(base);

    s_gintCallback[instance] = NULL;

    /* Enable the peripheral clock */
    CLOCK_EnableClock(s_gintClocks[instance]);

    /* Reset the peripheral */
    RESET_PeripheralReset(s_gintResets[instance]);
}

void GINT_SetCtrl(GINT_Type *base, gint_comb_t comb, gint_trig_t trig, gint_cb_t callback)
{
    uint32_t instance;

    instance = GINT_GetInstance(base);

    base->CTRL = (GINT_CTRL_COMB(comb) | GINT_CTRL_TRIG(trig));

    /* Save callback pointer */
    s_gintCallback[instance] = callback;
}

void GINT_GetCtrl(GINT_Type *base, gint_comb_t *comb, gint_trig_t *trig, gint_cb_t *callback)
{
    uint32_t instance;

    instance = GINT_GetInstance(base);

    *comb = (gint_comb_t)((base->CTRL & GINT_CTRL_COMB_MASK) >> GINT_CTRL_COMB_SHIFT);
    *trig = (gint_trig_t)((base->CTRL & GINT_CTRL_TRIG_MASK) >> GINT_CTRL_TRIG_SHIFT);
    *callback = s_gintCallback[instance];
}

void GINT_ConfigPins(GINT_Type *base, gint_port_t port, uint32_t polarityMask, uint32_t enableMask)
{
    base->PORT_POL[port] = polarityMask;
    base->PORT_ENA[port] = enableMask;
}

void GINT_GetConfigPins(GINT_Type *base, gint_port_t port, uint32_t *polarityMask, uint32_t *enableMask)
{
    *polarityMask = base->PORT_POL[port];
    *enableMask = base->PORT_ENA[port];
}

void GINT_EnableCallback(GINT_Type *base)
{
    uint32_t instance;

    instance = GINT_GetInstance(base);
    /* If GINT is configured in "AND" mode a spurious interrupt is generated.
       Clear status and pending interrupt before enabling the irq in NVIC. */
    GINT_ClrStatus(base);
    NVIC_ClearPendingIRQ(s_gintIRQ[instance]);
    EnableIRQ(s_gintIRQ[instance]);
}

void GINT_DisableCallback(GINT_Type *base)
{
    uint32_t instance;

    instance = GINT_GetInstance(base);
    DisableIRQ(s_gintIRQ[instance]);
    GINT_ClrStatus(base);
    NVIC_ClearPendingIRQ(s_gintIRQ[instance]);
}

void GINT_Deinit(GINT_Type *base)
{
    uint32_t instance;

    instance = GINT_GetInstance(base);

    /* Cleanup */
    GINT_DisableCallback(base);
    s_gintCallback[instance] = NULL;

    /* Reset the peripheral */
    RESET_PeripheralReset(s_gintResets[instance]);

    /* Disable the peripheral clock */
    CLOCK_DisableClock(s_gintClocks[instance]);
}

/* IRQ handler functions overloading weak symbols in the startup */
void GINT0_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[0]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[0] != NULL)
    {
        s_gintCallback[0]();
    }
}

#if (FSL_FEATURE_SOC_GINT_COUNT > 1U)
void GINT1_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[1]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[1] != NULL)
    {
        s_gintCallback[1]();
    }
}
#endif

#if (FSL_FEATURE_SOC_GINT_COUNT > 2U)
void GINT2_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[2]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[2] != NULL)
    {
        s_gintCallback[2]();
    }
}
#endif

#if (FSL_FEATURE_SOC_GINT_COUNT > 3U)
void GINT3_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[3]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[3] != NULL)
    {
        s_gintCallback[3]();
    }
}
#endif

#if (FSL_FEATURE_SOC_GINT_COUNT > 4U)
void GINT4_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[4]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[4] != NULL)
    {
        s_gintCallback[4]();
    }
}
#endif

#if (FSL_FEATURE_SOC_GINT_COUNT > 5U)
void GINT5_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[5]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[5] != NULL)
    {
        s_gintCallback[5]();
    }
}
#endif

#if (FSL_FEATURE_SOC_GINT_COUNT > 6U)
void GINT6_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[6]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[6] != NULL)
    {
        s_gintCallback[6]();
    }
}
#endif

#if (FSL_FEATURE_SOC_GINT_COUNT > 7U)
void GINT7_DriverIRQHandler(void)
{
    /* Clear interrupt before callback */
    s_gintBases[7]->CTRL |= GINT_CTRL_INT_MASK;
    /* Call user function */
    if (s_gintCallback[7] != NULL)
    {
        s_gintCallback[7]();
    }
}
#endif
