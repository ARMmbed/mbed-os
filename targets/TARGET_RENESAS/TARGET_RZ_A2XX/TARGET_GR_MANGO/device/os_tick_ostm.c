/**************************************************************************//**
 * @file     os_tick_ostm.c
 * @brief    CMSIS OS Tick implementation for OS Timer
 * @version  V1.0.1
 * @date     19. September 2017
 ******************************************************************************/
/*
 * Copyright (c) 2017-2020 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "irq_ctrl.h"

#include <MBRZA2M.h>

#include <cmsis.h>


// Define OS TImer interrupt priority
#ifndef OSTM_IRQ_PRIORITY
#define OSTM_IRQ_PRIORITY           0xFFU
#endif

// Define OS Timer channel and interrupt number
#define OSTM                        (OSTM0)
#define OSTM_IRQn                   ((IRQn_ID_t)OSTMI0_IRQn)

#ifdef MBED_CONF_RTOS_PRESENT

#include "os_tick.h"

static uint32_t OSTM_Clock;         // Timer tick frequency
static uint8_t  OSTM_PendIRQ;       // Timer interrupt pending flag


// Setup OS Tick.
int32_t OS_Tick_Setup(uint32_t freq, IRQHandler_t handler)
{
    uint32_t clock;
    uint32_t prio;
    uint32_t bits;
    volatile uint8_t dummy_buf;

    if (freq == 0U) {
        return (-1);
    }

    OSTM_PendIRQ = 0U;

    // Get CPG.FRQCR[IFC] bits
    clock = (CPG.FRQCR.WORD >> 8) & 0x03;

    // Determine Divider 2 output clock by using CM0_RENESAS_RZ_A2_P0_CLK
    clock = (CM0_RENESAS_RZ_A2_P0_CLK * (2 << clock));

    // Determine tick frequency
    clock = clock / freq;

    // Save frequency for later
    OSTM_Clock = clock;

    // Enable OSTM clock
    CPG.STBCR3.BYTE  &= ~(0x40u); /* [1], OSTM0, OSTM1, OSTM3,                            */
    /* MTU3, CAN-FD, ADC, GPT                               */
    dummy_buf = CPG.STBCR3.BYTE;  /* (Dummy read)                                         */
    (void)dummy_buf;

    // Stop the OSTM counter
    OSTM.OSTMnTT.BYTE  = 0x01U;

    // Set interval timer mode and disable interrupts when counting starts
    OSTM.OSTMnCTL.BYTE = 0x00U;

    // Set compare value
    OSTM.OSTMnCMP.LONG = clock - 1U;

    // Disable corresponding IRQ
    IRQ_Disable(OSTM_IRQn);
    IRQ_ClearPending(OSTM_IRQn);

    // Determine number of implemented priority bits
    IRQ_SetPriority(OSTM_IRQn, 0xFFU);

    prio = IRQ_GetPriority(OSTM_IRQn);

    // At least bits [7:4] must be implemented
    if ((prio & 0xF0U) == 0U) {
        return (-1);
    }

    for (bits = 0; bits < 4; bits++) {
        if ((prio & 0x01) != 0) {
            break;
        }
        prio >>= 1;
    }

    // Adjust configured priority to the number of implemented priority bits
    prio = (OSTM_IRQ_PRIORITY << bits) & 0xFFUL;

    // Set OSTM interrupt priority
    IRQ_SetPriority(OSTM_IRQn, prio - 1U);

    // Set edge-triggered, non-secure, single CPU targeted IRQ
    IRQ_SetMode(OSTM_IRQn, IRQ_MODE_TRIG_EDGE);

    // Register tick interrupt handler function
    IRQ_SetHandler(OSTM_IRQn, (IRQHandler_t)handler);

    // Enable corresponding IRQ
    IRQ_Enable(OSTM_IRQn);

    return (0);
}

/// Enable OS Tick.
void  OS_Tick_Enable(void)
{

    if (OSTM_PendIRQ != 0U) {
        OSTM_PendIRQ = 0U;
        IRQ_SetPending(OSTM_IRQn);
    }

    // Start the OSTM counter
    OSTM.OSTMnTS.BYTE = 0x01U;
}

/// Disable OS Tick.
void  OS_Tick_Disable(void)
{

    // Stop the OSTM counter
    OSTM.OSTMnTT.BYTE = 0x01U;

    if (IRQ_GetPending(OSTM_IRQn) != 0) {
        IRQ_ClearPending(OSTM_IRQn);
        OSTM_PendIRQ = 1U;
    }
}

// Acknowledge OS Tick IRQ.
void OS_Tick_AcknowledgeIRQ(void)
{
    IRQ_ClearPending(OSTM_IRQn);
}

// Get OS Tick IRQ number.
int32_t  OS_Tick_GetIRQn(void)
{
    return (OSTM_IRQn);
}

// Get OS Tick clock.
uint32_t OS_Tick_GetClock(void)
{
    return (OSTM_Clock);
}

// Get OS Tick interval.
uint32_t OS_Tick_GetInterval(void)
{
    return (OSTM.OSTMnCMP.LONG + 1U);
}

// Get OS Tick count value.
uint32_t OS_Tick_GetCount(void)
{
    uint32_t cmp = OSTM.OSTMnCMP.LONG;
    return (cmp - OSTM.OSTMnCNT.LONG);
}

// Get OS Tick overflow status.
uint32_t OS_Tick_GetOverflow(void)
{
    return (IRQ_GetPending(OSTM_IRQn));
}
#endif

// Get Cortex-A9 OS Timer interrupt number
IRQn_ID_t mbed_get_a9_tick_irqn()
{
    return OSTM_IRQn;
}

