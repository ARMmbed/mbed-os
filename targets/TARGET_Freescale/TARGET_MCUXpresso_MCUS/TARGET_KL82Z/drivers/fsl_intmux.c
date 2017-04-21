/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_intmux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get instance number for INTMUX.
 *
 * @param base INTMUX peripheral base address.
 */
static uint32_t INTMUX_GetInstance(INTMUX_Type *base);

/*!
 * @brief Handle INTMUX all channels IRQ.
 *
 * The handler reads the INTMUX channel's active vector register. This returns the offset
 * from the start of the vector table to the vector for the INTMUX channel's highest priority
 * pending source interrupt. After a check for spurious interrupts (an offset of 0), the
 * function address at the vector offset is read and jumped to.
 *
 * @param instance INTMUX instance number.
 * @param channel INTMUX channel number.
 */
static void INTMUX_CommonIRQHandler(uint32_t instance, uint32_t channel);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map INTMUX instance number to base pointer. */
static INTMUX_Type *const s_intmuxBases[] = INTMUX_BASE_PTRS;

/*! @brief Array to map INTMUX instance number to clock name. */
static const clock_ip_name_t s_intmuxClockName[] = INTMUX_CLOCKS;

/*! @brief Array to map INTMUX instance number to IRQ number. */
static const IRQn_Type s_intmuxIRQNumber[] = INTMUX_IRQS;

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t INTMUX_GetInstance(INTMUX_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < FSL_FEATURE_SOC_INTMUX_COUNT; instance++)
    {
        if (s_intmuxBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < FSL_FEATURE_SOC_INTMUX_COUNT);

    return instance;
}

static void INTMUX_CommonIRQHandler(uint32_t instance, uint32_t channel)
{
    INTMUX_Type *intmuxBase;
    uint32_t pendingIrqOffset;

    intmuxBase = s_intmuxBases[instance];
    pendingIrqOffset = intmuxBase->CHANNEL[channel].CHn_VEC;
    if (pendingIrqOffset)
    {
        uint32_t isr = *(uint32_t *)(SCB->VTOR + pendingIrqOffset);
        ((void (*)(void))isr)();
    }
}

void INTMUX_Init(INTMUX_Type *base)
{
    uint32_t channel;

    /* Enable clock gate. */
    CLOCK_EnableClock(s_intmuxClockName[INTMUX_GetInstance(base)]);
    /* Reset all channels and enable NVIC vectors for all INTMUX channels. */
    for (channel = 0; channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT; channel++)
    {
        INTMUX_ResetChannel(base, channel);
        NVIC_EnableIRQ(s_intmuxIRQNumber[channel]);
    }
}

void INTMUX_Deinit(INTMUX_Type *base)
{
    uint32_t channel;

    /* Disable clock gate. */
    CLOCK_DisableClock(s_intmuxClockName[INTMUX_GetInstance(base)]);
    /* Disable NVIC vectors for all of the INTMUX channels. */
    for (channel = 0; channel < FSL_FEATURE_INTMUX_CHANNEL_COUNT; channel++)
    {
        NVIC_DisableIRQ(s_intmuxIRQNumber[channel]);
    }
}

void INTMUX0_0_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 0);
}

void INTMUX0_1_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 1);
}

void INTMUX0_2_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 2);
}

void INTMUX0_3_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 3);
}

#if defined(INTMUX1)
void INTMUX1_0_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 0);
}

void INTMUX1_1_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 1);
}

void INTMUX1_2_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 2);
}

void INTMUX1_3_DriverIRQHandler(void)
{
    INTMUX_CommonIRQHandler(0, 3);
}
#endif
