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

#include "fsl_common.h"
#include "fsl_flexcomm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to real IRQ handlers installed by drivers for each instance. */
static flexcomm_irq_handler_t s_flexcommIrqHandler[FSL_FEATURE_SOC_FLEXCOMM_COUNT];

/*! @brief Pointers to handles for each instance to provide context to interrupt routines */
static void *s_flexcommHandle[FSL_FEATURE_SOC_FLEXCOMM_COUNT];

/*! @brief Array to map FLEXCOMM instance number to IRQ number. */
IRQn_Type const kFlexcommIrqs[] = USART_IRQS;

/*! @brief Array to map FLEXCOMM instance number to base address. */
static const uint32_t s_flexcommBaseAddrs[FSL_FEATURE_SOC_FLEXCOMM_COUNT] = FLEXCOMM_BASE_ADDRS;

/*! @brief IDs of clock for each FLEXCOMM module */
static const clock_ip_name_t s_flexcommClocks[] = FLEXCOMM_CLOCKS;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* check whether flexcomm supports peripheral type */
static bool FLEXCOMM_PeripheralIsPresent(FLEXCOMM_Type *base, FLEXCOMM_PERIPH_T periph)
{
    if (periph == FLEXCOMM_PERIPH_NONE)
    {
        return true;
    }
    else if ((periph >= FLEXCOMM_PERIPH_USART) && (periph <= FLEXCOMM_PERIPH_I2S_TX))
    {
        return (base->PSELID & (uint32_t)(1 << ((uint32_t)periph + 3))) > 0 ? true : false;
    }
    else if (periph == FLEXCOMM_PERIPH_I2S_RX)
    {
        return (base->PSELID & (1 << 7)) > 0 ? true : false;
    }
    else
    {
        return false;
    }
}

/* Get the index corresponding to the FLEXCOMM */
uint32_t FLEXCOMM_GetInstance(void *base)
{
    int i;

    for (i = 0; i < FSL_FEATURE_SOC_FLEXCOMM_COUNT; i++)
    {
        if ((uint32_t)base == s_flexcommBaseAddrs[i])
        {
            return i;
        }
    }

    assert(false);
    return 0;
}

/* Changes FLEXCOMM mode */
status_t FLEXCOMM_SetPeriph(FLEXCOMM_Type *base, FLEXCOMM_PERIPH_T periph, int lock)
{
    /* Check whether peripheral type is present */
    if (!FLEXCOMM_PeripheralIsPresent(base, periph))
    {
        return kStatus_OutOfRange;
    }

    /* Flexcomm is locked to different peripheral type than expected  */
    if ((base->PSELID & FLEXCOMM_PSELID_LOCK_MASK) && ((base->PSELID & FLEXCOMM_PSELID_PERSEL_MASK) != periph))
    {
        return kStatus_Fail;
    }

    /* Check if we are asked to lock */
    if (lock)
    {
        base->PSELID = (uint32_t)periph | FLEXCOMM_PSELID_LOCK_MASK;
    }
    else
    {
        base->PSELID = (uint32_t)periph;
    }

    return kStatus_Success;
}

status_t FLEXCOMM_Init(void *base, FLEXCOMM_PERIPH_T periph)
{
    int idx = FLEXCOMM_GetInstance(base);

    if (idx < 0)
    {
        return kStatus_InvalidArgument;
    }

    /* Enable the peripheral clock */
    CLOCK_EnableClock(s_flexcommClocks[idx]);

    /* Set the FLEXCOMM to given peripheral */
    return FLEXCOMM_SetPeriph((FLEXCOMM_Type *)base, periph, 0);
}

void FLEXCOMM_SetIRQHandler(void *base, flexcomm_irq_handler_t handler, void *handle)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(base);

    /* Clear handler first to avoid execution of the handler with wrong handle */
    s_flexcommIrqHandler[instance] = NULL;
    s_flexcommHandle[instance] = handle;
    s_flexcommIrqHandler[instance] = handler;
}

/* IRQ handler functions overloading weak symbols in the startup */
void FLEXCOMM0_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[0]);
    s_flexcommIrqHandler[0]((void *)s_flexcommBaseAddrs[0], s_flexcommHandle[0]);
}

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 1U)
void FLEXCOMM1_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[1]);
    s_flexcommIrqHandler[1]((void *)s_flexcommBaseAddrs[1], s_flexcommHandle[1]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 2U)
void FLEXCOMM2_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[2]);
    s_flexcommIrqHandler[2]((void *)s_flexcommBaseAddrs[2], s_flexcommHandle[2]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 3U)
void FLEXCOMM3_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[3]);
    s_flexcommIrqHandler[3]((void *)s_flexcommBaseAddrs[3], s_flexcommHandle[3]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 4U)
void FLEXCOMM4_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[4]);
    s_flexcommIrqHandler[4]((void *)s_flexcommBaseAddrs[4], s_flexcommHandle[4]);
}

#endif

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 5U)
void FLEXCOMM5_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[5]);
    s_flexcommIrqHandler[5]((void *)s_flexcommBaseAddrs[5], s_flexcommHandle[5]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 6U)
void FLEXCOMM6_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[6]);
    s_flexcommIrqHandler[6]((void *)s_flexcommBaseAddrs[6], s_flexcommHandle[6]);
}
#endif

#if (FSL_FEATURE_SOC_FLEXCOMM_COUNT > 7U)
void FLEXCOMM7_DriverIRQHandler(void)
{
    assert(s_flexcommIrqHandler[7]);
    s_flexcommIrqHandler[7]((void *)s_flexcommBaseAddrs[7], s_flexcommHandle[7]);
}
#endif
