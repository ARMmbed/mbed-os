/***************************************************************************//**
* \file cyhal_timer.c
*
* \brief
* Provides a high level interface for interacting with the Cypress Timer/Counter.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#include "cyhal_timer.h"
#include "cyhal_hwmgr.h"
#include "cyhal_gpio.h"

#if defined(CY_IP_MXTCPWM_INSTANCES)

#if defined(__cplusplus)
extern "C" {
#endif

#define GET_ARRAY_INDEX(block, channel)  (cyhal_internal_data[block].channel_offset + channel)

#if (CY_IP_MXTCPWM_INSTANCES == 0)
    #define TCPWM_CHANNELS (0u)
#elif (CY_IP_MXTCPWM_INSTANCES == 1)
    #define TCPWM_CHANNELS (TCPWM0_CNT_NR)
#elif (CY_IP_MXTCPWM_INSTANCES == 2)
    #define TCPWM_CHANNELS (TCPWM0_CNT_NR + TCPWM1_CNT_NR)
#else
    #warning Unhandled TCPWM instance count
#endif


/* Callback array for TCPWM interrupts */
static cyhal_timer_irq_handler_t callbacks[TCPWM_CHANNELS];
static void *callback_args[TCPWM_CHANNELS];

typedef struct {
    TCPWM_Type *base;
    en_clk_dst_t clock_dst;
    uint32_t max_count;
    uint8_t channel_offset;
    uint8_t isr_offset;
} cyhal_internal_timer_data_t;

static const cyhal_internal_timer_data_t cyhal_internal_data[] = {
#if (CY_IP_MXTCPWM_INSTANCES > 0)
    {TCPWM0, PCLK_TCPWM0_CLOCKS0, (uint32_t)((1ULL << TCPWM0_CNT_CNT_WIDTH) - 1ULL), 0, tcpwm_0_interrupts_0_IRQn },
#endif
#if (CY_IP_MXTCPWM_INSTANCES > 1)
    {TCPWM1, PCLK_TCPWM1_CLOCKS0, (uint32_t)((1ULL << TCPWM1_CNT_CNT_WIDTH) - 1ULL), TCPWM0_CNT_NR, tcpwm_1_interrupts_0_IRQn },
#endif
#if (CY_IP_MXTCPWM_INSTANCES > 2)
    #warning Unhandled TCPWM instance count
#endif
};


const cy_stc_tcpwm_counter_config_t default_config =
{
    .period = 32768,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_NONE,
    .captureInputMode = 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
};

/*******************************************************************************
*       Dispatcher Interrrupt Service Routine
*******************************************************************************/

static void isr_tcpwm_handler(uint8_t block, uint8_t channel)
{
    TCPWM_Type *blockAddr = cyhal_internal_data[block].base;
    uint32_t index = GET_ARRAY_INDEX(block, channel);

    if (callbacks[index] != NULL)
    {
        /* Call registered callbacks here */
        (void)(callbacks[index])(callback_args[index], (cyhal_timer_irq_event_t)Cy_TCPWM_GetInterruptStatus(blockAddr, channel));
    }
    Cy_TCPWM_ClearInterrupt(blockAddr, channel, CY_TCPWM_INT_ON_CC_OR_TC);
}


/*******************************************************************************
*       (Internal) Interrrupt Service Routines
*******************************************************************************/
static void isr_tcpwm_0_00_handler(void) __attribute__((unused));
static void isr_tcpwm_0_01_handler(void) __attribute__((unused));
static void isr_tcpwm_0_02_handler(void) __attribute__((unused));
static void isr_tcpwm_0_03_handler(void) __attribute__((unused));
static void isr_tcpwm_0_04_handler(void) __attribute__((unused));
static void isr_tcpwm_0_05_handler(void) __attribute__((unused));
static void isr_tcpwm_0_06_handler(void) __attribute__((unused));
static void isr_tcpwm_0_07_handler(void) __attribute__((unused));
static void isr_tcpwm_0_08_handler(void) __attribute__((unused));
static void isr_tcpwm_0_09_handler(void) __attribute__((unused));
static void isr_tcpwm_0_10_handler(void) __attribute__((unused));
static void isr_tcpwm_0_11_handler(void) __attribute__((unused));
static void isr_tcpwm_0_12_handler(void) __attribute__((unused));
static void isr_tcpwm_0_13_handler(void) __attribute__((unused));
static void isr_tcpwm_0_14_handler(void) __attribute__((unused));
static void isr_tcpwm_0_15_handler(void) __attribute__((unused));
static void isr_tcpwm_0_16_handler(void) __attribute__((unused));
static void isr_tcpwm_0_17_handler(void) __attribute__((unused));
static void isr_tcpwm_0_18_handler(void) __attribute__((unused));
static void isr_tcpwm_0_19_handler(void) __attribute__((unused));
static void isr_tcpwm_0_20_handler(void) __attribute__((unused));
static void isr_tcpwm_0_21_handler(void) __attribute__((unused));
static void isr_tcpwm_0_22_handler(void) __attribute__((unused));
static void isr_tcpwm_0_23_handler(void) __attribute__((unused));
static void isr_tcpwm_0_24_handler(void) __attribute__((unused));
static void isr_tcpwm_0_25_handler(void) __attribute__((unused));
static void isr_tcpwm_0_26_handler(void) __attribute__((unused));
static void isr_tcpwm_0_27_handler(void) __attribute__((unused));
static void isr_tcpwm_0_28_handler(void) __attribute__((unused));
static void isr_tcpwm_0_29_handler(void) __attribute__((unused));
static void isr_tcpwm_0_30_handler(void) __attribute__((unused));
static void isr_tcpwm_0_31_handler(void) __attribute__((unused));
static void isr_tcpwm_1_00_handler(void) __attribute__((unused));
static void isr_tcpwm_1_01_handler(void) __attribute__((unused));
static void isr_tcpwm_1_02_handler(void) __attribute__((unused));
static void isr_tcpwm_1_03_handler(void) __attribute__((unused));
static void isr_tcpwm_1_04_handler(void) __attribute__((unused));
static void isr_tcpwm_1_05_handler(void) __attribute__((unused));
static void isr_tcpwm_1_06_handler(void) __attribute__((unused));
static void isr_tcpwm_1_07_handler(void) __attribute__((unused));
static void isr_tcpwm_1_08_handler(void) __attribute__((unused));
static void isr_tcpwm_1_09_handler(void) __attribute__((unused));
static void isr_tcpwm_1_10_handler(void) __attribute__((unused));
static void isr_tcpwm_1_11_handler(void) __attribute__((unused));
static void isr_tcpwm_1_12_handler(void) __attribute__((unused));
static void isr_tcpwm_1_13_handler(void) __attribute__((unused));
static void isr_tcpwm_1_14_handler(void) __attribute__((unused));
static void isr_tcpwm_1_15_handler(void) __attribute__((unused));
static void isr_tcpwm_1_16_handler(void) __attribute__((unused));
static void isr_tcpwm_1_17_handler(void) __attribute__((unused));
static void isr_tcpwm_1_18_handler(void) __attribute__((unused));
static void isr_tcpwm_1_19_handler(void) __attribute__((unused));
static void isr_tcpwm_1_20_handler(void) __attribute__((unused));
static void isr_tcpwm_1_21_handler(void) __attribute__((unused));
static void isr_tcpwm_1_22_handler(void) __attribute__((unused));
static void isr_tcpwm_1_23_handler(void) __attribute__((unused));
static void isr_tcpwm_1_24_handler(void) __attribute__((unused));
static void isr_tcpwm_1_25_handler(void) __attribute__((unused));
static void isr_tcpwm_1_26_handler(void) __attribute__((unused));
static void isr_tcpwm_1_27_handler(void) __attribute__((unused));
static void isr_tcpwm_1_28_handler(void) __attribute__((unused));
static void isr_tcpwm_1_29_handler(void) __attribute__((unused));
static void isr_tcpwm_1_30_handler(void) __attribute__((unused));
static void isr_tcpwm_1_31_handler(void) __attribute__((unused));

static void isr_tcpwm_0_00_handler (void)
{
    isr_tcpwm_handler(0, 0);
}
static void isr_tcpwm_0_01_handler (void)
{
    isr_tcpwm_handler(0, 1);
}
static void isr_tcpwm_0_02_handler (void)
{
    isr_tcpwm_handler(0, 2);
}
static void isr_tcpwm_0_03_handler (void)
{
    isr_tcpwm_handler(0, 3);
}
static void isr_tcpwm_0_04_handler (void)
{
    isr_tcpwm_handler(0, 4);
}
static void isr_tcpwm_0_05_handler (void)
{
    isr_tcpwm_handler(0, 5);
}
static void isr_tcpwm_0_06_handler (void)
{
    isr_tcpwm_handler(0, 6);
}
static void isr_tcpwm_0_07_handler (void)
{
    isr_tcpwm_handler(0, 7);
}
static void isr_tcpwm_0_08_handler (void)
{
    isr_tcpwm_handler(0, 8);
}
static void isr_tcpwm_0_09_handler (void)
{
    isr_tcpwm_handler(0, 9);
}
static void isr_tcpwm_0_10_handler (void)
{
    isr_tcpwm_handler(0, 10);
}
static void isr_tcpwm_0_11_handler (void)
{
    isr_tcpwm_handler(0, 11);
}
static void isr_tcpwm_0_12_handler (void)
{
    isr_tcpwm_handler(0, 12);
}
static void isr_tcpwm_0_13_handler (void)
{
    isr_tcpwm_handler(0, 13);
}
static void isr_tcpwm_0_14_handler (void)
{
    isr_tcpwm_handler(0, 14);
}
static void isr_tcpwm_0_15_handler (void)
{
    isr_tcpwm_handler(0, 15);
}
static void isr_tcpwm_0_16_handler (void)
{
    isr_tcpwm_handler(0, 16);
}
static void isr_tcpwm_0_17_handler (void)
{
    isr_tcpwm_handler(0, 17);
}
static void isr_tcpwm_0_18_handler (void)
{
    isr_tcpwm_handler(0, 18);
}
static void isr_tcpwm_0_19_handler (void)
{
    isr_tcpwm_handler(0, 19);
}
static void isr_tcpwm_0_20_handler (void)
{
    isr_tcpwm_handler(0, 20);
}
static void isr_tcpwm_0_21_handler (void)
{
    isr_tcpwm_handler(0, 21);
}
static void isr_tcpwm_0_22_handler (void)
{
    isr_tcpwm_handler(0, 22);
}
static void isr_tcpwm_0_23_handler (void)
{
    isr_tcpwm_handler(0, 23);
}
static void isr_tcpwm_0_24_handler (void)
{
    isr_tcpwm_handler(0, 24);
}
static void isr_tcpwm_0_25_handler (void)
{
    isr_tcpwm_handler(0, 25);
}
static void isr_tcpwm_0_26_handler (void)
{
    isr_tcpwm_handler(0, 26);
}
static void isr_tcpwm_0_27_handler (void)
{
    isr_tcpwm_handler(0, 27);
}
static void isr_tcpwm_0_28_handler (void)
{
    isr_tcpwm_handler(0, 28);
}
static void isr_tcpwm_0_29_handler (void)
{
    isr_tcpwm_handler(0, 29);
}
static void isr_tcpwm_0_30_handler (void)
{
    isr_tcpwm_handler(0, 30);
}
static void isr_tcpwm_0_31_handler (void)
{
    isr_tcpwm_handler(0, 31);
}
static void isr_tcpwm_1_00_handler (void)
{
    isr_tcpwm_handler(1, 0);
}
static void isr_tcpwm_1_01_handler (void)
{
    isr_tcpwm_handler(1, 1);
}
static void isr_tcpwm_1_02_handler (void)
{
    isr_tcpwm_handler(1, 2);
}
static void isr_tcpwm_1_03_handler (void)
{
    isr_tcpwm_handler(1, 3);
}
static void isr_tcpwm_1_04_handler (void)
{
    isr_tcpwm_handler(1, 4);
}
static void isr_tcpwm_1_05_handler (void)
{
    isr_tcpwm_handler(1, 5);
}
static void isr_tcpwm_1_06_handler (void)
{
    isr_tcpwm_handler(1, 6);
}
static void isr_tcpwm_1_07_handler (void)
{
    isr_tcpwm_handler(1, 7);
}
static void isr_tcpwm_1_08_handler (void)
{
    isr_tcpwm_handler(1, 8);
}
static void isr_tcpwm_1_09_handler (void)
{
    isr_tcpwm_handler(1, 9);
}
static void isr_tcpwm_1_10_handler (void)
{
    isr_tcpwm_handler(1, 10);
}
static void isr_tcpwm_1_11_handler (void)
{
    isr_tcpwm_handler(1, 11);
}
static void isr_tcpwm_1_12_handler (void)
{
    isr_tcpwm_handler(1, 12);
}
static void isr_tcpwm_1_13_handler (void)
{
    isr_tcpwm_handler(1, 13);
}
static void isr_tcpwm_1_14_handler (void)
{
    isr_tcpwm_handler(1, 14);
}
static void isr_tcpwm_1_15_handler (void)
{
    isr_tcpwm_handler(1, 15);
}
static void isr_tcpwm_1_16_handler (void)
{
    isr_tcpwm_handler(1, 16);
}
static void isr_tcpwm_1_17_handler (void)
{
    isr_tcpwm_handler(1, 17);
}
static void isr_tcpwm_1_18_handler (void)
{
    isr_tcpwm_handler(1, 18);
}
static void isr_tcpwm_1_19_handler (void)
{
    isr_tcpwm_handler(1, 19);
}
static void isr_tcpwm_1_20_handler (void)
{
    isr_tcpwm_handler(1, 20);
}
static void isr_tcpwm_1_21_handler (void)
{
    isr_tcpwm_handler(1, 21);
}
static void isr_tcpwm_1_22_handler (void)
{
    isr_tcpwm_handler(1, 22);
}
static void isr_tcpwm_1_23_handler (void)
{
    isr_tcpwm_handler(1, 23);
}
static void isr_tcpwm_1_24_handler (void)
{
    isr_tcpwm_handler(1, 24);
}
static void isr_tcpwm_1_25_handler (void)
{
    isr_tcpwm_handler(1, 25);
}
static void isr_tcpwm_1_26_handler (void)
{
    isr_tcpwm_handler(1, 26);
}
static void isr_tcpwm_1_27_handler (void)
{
    isr_tcpwm_handler(1, 27);
}
static void isr_tcpwm_1_28_handler (void)
{
    isr_tcpwm_handler(1, 28);
}
static void isr_tcpwm_1_29_handler (void)
{
    isr_tcpwm_handler(1, 29);
}
static void isr_tcpwm_1_30_handler (void)
{
    isr_tcpwm_handler(1, 30);
}
static void isr_tcpwm_1_31_handler (void)
{
    isr_tcpwm_handler(1, 31);
}

typedef void (*interrupt_dispatcher)(void);
static const interrupt_dispatcher interrupts_dispatcher_table[TCPWM_CHANNELS] =
{
#if (TCPWM0_CNT_NR > 0)
    isr_tcpwm_0_00_handler,
#endif
#if (TCPWM0_CNT_NR > 1)
    isr_tcpwm_0_01_handler,
#endif
#if (TCPWM0_CNT_NR > 2)
    isr_tcpwm_0_02_handler,
#endif
#if (TCPWM0_CNT_NR > 3)
    isr_tcpwm_0_03_handler,
#endif
#if (TCPWM0_CNT_NR > 4)
    isr_tcpwm_0_04_handler,
#endif
#if (TCPWM0_CNT_NR > 5)
    isr_tcpwm_0_05_handler,
#endif
#if (TCPWM0_CNT_NR > 6)
    isr_tcpwm_0_06_handler,
#endif
#if (TCPWM0_CNT_NR > 7)
    isr_tcpwm_0_07_handler,
#endif
#if (TCPWM0_CNT_NR > 8)
    isr_tcpwm_0_08_handler,
#endif
#if (TCPWM0_CNT_NR > 9)
    isr_tcpwm_0_09_handler,
#endif
#if (TCPWM0_CNT_NR > 10)
    isr_tcpwm_0_10_handler,
#endif
#if (TCPWM0_CNT_NR > 11)
    isr_tcpwm_0_11_handler,
#endif
#if (TCPWM0_CNT_NR > 12)
    isr_tcpwm_0_12_handler,
#endif
#if (TCPWM0_CNT_NR > 13)
    isr_tcpwm_0_13_handler,
#endif
#if (TCPWM0_CNT_NR > 14)
    isr_tcpwm_0_14_handler,
#endif
#if (TCPWM0_CNT_NR > 15)
    isr_tcpwm_0_15_handler,
#endif
#if (TCPWM0_CNT_NR > 16)
    isr_tcpwm_0_16_handler,
#endif
#if (TCPWM0_CNT_NR > 17)
    isr_tcpwm_0_17_handler,
#endif
#if (TCPWM0_CNT_NR > 18)
    isr_tcpwm_0_18_handler,
#endif
#if (TCPWM0_CNT_NR > 19)
    isr_tcpwm_0_19_handler,
#endif
#if (TCPWM0_CNT_NR > 20)
    isr_tcpwm_0_20_handler,
#endif
#if (TCPWM0_CNT_NR > 21)
    isr_tcpwm_0_21_handler,
#endif
#if (TCPWM0_CNT_NR > 22)
    isr_tcpwm_0_22_handler,
#endif
#if (TCPWM0_CNT_NR > 23)
    isr_tcpwm_0_23_handler,
#endif
#if (TCPWM0_CNT_NR > 24)
    isr_tcpwm_0_24_handler,
#endif
#if (TCPWM0_CNT_NR > 25)
    isr_tcpwm_0_25_handler,
#endif
#if (TCPWM0_CNT_NR > 26)
    isr_tcpwm_0_26_handler,
#endif
#if (TCPWM0_CNT_NR > 27)
    isr_tcpwm_0_27_handler,
#endif
#if (TCPWM0_CNT_NR > 28)
    isr_tcpwm_0_28_handler,
#endif
#if (TCPWM0_CNT_NR > 29)
    isr_tcpwm_0_29_handler,
#endif
#if (TCPWM0_CNT_NR > 30)
    isr_tcpwm_0_30_handler,
#endif
#if (TCPWM0_CNT_NR > 31)
    isr_tcpwm_0_31_handler,
#endif
#if (TCPWM0_CNT_NR > 32)
    #error "Unhandled TCPWM 0 channel count"
#endif
#if (TCPWM1_CNT_NR > 0)
    isr_tcpwm_1_00_handler,
#endif
#if (TCPWM1_CNT_NR > 1)
    isr_tcpwm_1_01_handler,
#endif
#if (TCPWM1_CNT_NR > 2)
    isr_tcpwm_1_02_handler,
#endif
#if (TCPWM1_CNT_NR > 3)
    isr_tcpwm_1_03_handler,
#endif
#if (TCPWM1_CNT_NR > 4)
    isr_tcpwm_1_04_handler,
#endif
#if (TCPWM1_CNT_NR > 5)
    isr_tcpwm_1_05_handler,
#endif
#if (TCPWM1_CNT_NR > 6)
    isr_tcpwm_1_06_handler,
#endif
#if (TCPWM1_CNT_NR > 7)
    isr_tcpwm_1_07_handler,
#endif
#if (TCPWM1_CNT_NR > 8)
    isr_tcpwm_1_08_handler,
#endif
#if (TCPWM1_CNT_NR > 9)
    isr_tcpwm_1_09_handler,
#endif
#if (TCPWM1_CNT_NR > 10)
    isr_tcpwm_1_10_handler,
#endif
#if (TCPWM1_CNT_NR > 11)
    isr_tcpwm_1_11_handler,
#endif
#if (TCPWM1_CNT_NR > 12)
    isr_tcpwm_1_12_handler,
#endif
#if (TCPWM1_CNT_NR > 13)
    isr_tcpwm_1_13_handler,
#endif
#if (TCPWM1_CNT_NR > 14)
    isr_tcpwm_1_14_handler,
#endif
#if (TCPWM1_CNT_NR > 15)
    isr_tcpwm_1_15_handler,
#endif
#if (TCPWM1_CNT_NR > 16)
    isr_tcpwm_1_16_handler,
#endif
#if (TCPWM1_CNT_NR > 17)
    isr_tcpwm_1_17_handler,
#endif
#if (TCPWM1_CNT_NR > 18)
    isr_tcpwm_1_18_handler,
#endif
#if (TCPWM1_CNT_NR > 19)
    isr_tcpwm_1_19_handler,
#endif
#if (TCPWM1_CNT_NR > 20)
    isr_tcpwm_1_20_handler,
#endif
#if (TCPWM1_CNT_NR > 21)
    isr_tcpwm_1_21_handler,
#endif
#if (TCPWM1_CNT_NR > 22)
    isr_tcpwm_1_22_handler,
#endif
#if (TCPWM1_CNT_NR > 23)
    isr_tcpwm_1_23_handler,
#endif
#if (TCPWM1_CNT_NR > 24)
    isr_tcpwm_1_24_handler,
#endif
#if (TCPWM1_CNT_NR > 25)
    isr_tcpwm_1_25_handler,
#endif
#if (TCPWM1_CNT_NR > 26)
    isr_tcpwm_1_26_handler,
#endif
#if (TCPWM1_CNT_NR > 27)
    isr_tcpwm_1_27_handler,
#endif
#if (TCPWM1_CNT_NR > 28)
    isr_tcpwm_1_28_handler,
#endif
#if (TCPWM1_CNT_NR > 29)
    isr_tcpwm_1_29_handler,
#endif
#if (TCPWM1_CNT_NR > 30)
    isr_tcpwm_1_30_handler,
#endif
#if (TCPWM1_CNT_NR > 31)
    isr_tcpwm_1_31_handler,
#endif
#if (TCPWM1_CNT_NR > 32)
    #error "Unhandled TCPWM 1 channel count"
#endif
};


static inline uint32_t convert_direction(cyhal_timer_direction_t direction)
{
    switch (direction)
    {
        case CYHAL_TIMER_DIR_UP:
            return CY_TCPWM_COUNTER_COUNT_UP;
        case CYHAL_TIMER_DIR_DOWN:
            return CY_TCPWM_COUNTER_COUNT_DOWN;
        case CYHAL_TIMER_DIR_UP_DOWN:
            return CY_TCPWM_COUNTER_COUNT_UP_DOWN_2;
    }
    return CY_TCPWM_COUNTER_COUNT_UP;
}


/*******************************************************************************
*       Timer HAL Functions
*******************************************************************************/

cy_rslt_t cyhal_timer_init(cyhal_timer_t *obj, cyhal_gpio_t pin, const cyhal_clock_divider_t *clk)
{
    CY_ASSERT(NULL != obj);

    //TODO: Handle Trigger mux pin assignments
    if (CYHAL_NC_PIN_VALUE != pin)
        return CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT;

    cy_rslt_t result = cyhal_hwmgr_allocate(CYHAL_RSC_TCPWM, &obj->resource);
    if (CY_RSLT_SUCCESS == result)
    {
        cyhal_resource_inst_t *timer = &obj->resource;
        obj->base = cyhal_internal_data[timer->block_num].base;

        en_clk_dst_t pclk = (en_clk_dst_t)(cyhal_internal_data[timer->block_num].clock_dst + timer->channel_num);

        if (NULL != clk)
        {
            obj->clock_hz = cy_PeriClkFreqHz / (1 + Cy_SysClk_PeriphGetDivider(obj->clock.div_type, obj->clock.div_num));
            if (CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, clk->div_type, clk->div_num))
            {
                result = CYHAL_TIMER_RSLT_ERR_CLOCK;
            }
        }
        else if (CY_RSLT_SUCCESS == (result = cyhal_hwmgr_allocate_clock(&(obj->clock), CY_SYSCLK_DIV_16_BIT, false)))
        {
            uint32_t div = cy_PeriClkFreqHz / 1000000u;
            if (0 == div ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphSetDivider(obj->clock.div_type, obj->clock.div_num, div - 1) ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphEnableDivider(obj->clock.div_type, obj->clock.div_num) ||
                CY_SYSCLK_SUCCESS != Cy_SysClk_PeriphAssignDivider(pclk, obj->clock.div_type, obj->clock.div_num))
            {
                result = CYHAL_TIMER_RSLT_ERR_CLOCK;
            }
            else
            {
                obj->clock_hz = cy_PeriClkFreqHz / div;
            }
        }

        bool configured = cyhal_hwmgr_is_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        if (CY_RSLT_SUCCESS == result && !configured)
        {
            result = Cy_TCPWM_Counter_Init(obj->base, obj->resource.channel_num, &default_config);
            if (CY_TCPWM_SUCCESS == result)
            {
                result = cyhal_hwmgr_set_configured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
            }
        }

        if (result == CY_RSLT_SUCCESS)
        {
            Cy_TCPWM_Counter_Enable(obj->base, obj->resource.channel_num);
        }
        else
        {
            cyhal_timer_free(obj);
        }
    }

    return result;
}

void cyhal_timer_free(cyhal_timer_t *obj)
{
    CY_ASSERT(NULL != obj);

    if (NULL != obj && NULL != obj->base)
    {
        cyhal_hwmgr_free(&obj->resource);
        cyhal_hwmgr_set_unconfigured(obj->resource.type, obj->resource.block_num, obj->resource.channel_num);
        obj->base = NULL;
    }
}

cy_rslt_t cyhal_timer_set_config(cyhal_timer_t *obj, const cyhal_timer_cfg_t *cfg)
{
    cy_rslt_t rslt;
    if (obj->is_continuous != cfg->is_continuous ||
        obj->direction != cfg->direction ||
        obj->is_compare != cfg->is_compare)
    {
        cy_stc_tcpwm_counter_config_t config = default_config;
        config.period = cfg->period;
        config.compare0 = cfg->compare_value;
        config.runMode = cfg->is_continuous ? CY_TCPWM_COUNTER_CONTINUOUS : CY_TCPWM_COUNTER_ONESHOT;
        config.compareOrCapture = cfg->is_compare ? CY_TCPWM_COUNTER_MODE_COMPARE : CY_TCPWM_COUNTER_MODE_CAPTURE;
        config.countDirection = convert_direction(cfg->direction);

        Cy_TCPWM_Counter_DeInit(obj->base, obj->resource.channel_num, &config);
        rslt = (cy_rslt_t)Cy_TCPWM_Counter_Init(obj->base, obj->resource.channel_num, &config);
        if (CY_TCPWM_SUCCESS == rslt)
        {
            obj->is_continuous = cfg->is_continuous;
            obj->direction = cfg->direction;
            obj->is_compare = cfg->is_compare;
        }
    }
    else
    {
        Cy_TCPWM_Counter_SetCounter(obj->base, obj->resource.channel_num, cfg->value);
        Cy_TCPWM_Counter_SetPeriod(obj->base, obj->resource.channel_num, cfg->period);
        if (cfg->is_compare)
        {
            Cy_TCPWM_Counter_SetCompare0(obj->base, obj->resource.channel_num, cfg->compare_value);
        }
        rslt = CY_RSLT_SUCCESS;
    }

    return rslt;
}

cy_rslt_t cyhal_timer_start(cyhal_timer_t *obj)
{
    if (NULL == obj)
        return CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT;
    Cy_TCPWM_TriggerReloadOrIndex(obj->base, 1u << obj->resource.channel_num);
    return CY_RSLT_SUCCESS;
}

cy_rslt_t cyhal_timer_stop(cyhal_timer_t *obj)
{
    if (NULL == obj)
        return CYHAL_TIMER_RSLT_ERR_BAD_ARGUMENT;
    Cy_TCPWM_TriggerStopOrKill(obj->base, 1u << obj->resource.channel_num);
    return CY_RSLT_SUCCESS;
}

void cyhal_timer_register_irq(cyhal_timer_t *obj, uint8_t priority, cyhal_timer_irq_handler_t handler, void *handler_arg)
{
    uint8_t index = GET_ARRAY_INDEX(obj->resource.block_num, obj->resource.channel_num);
    callbacks[index] = handler;
    callback_args[index] = handler_arg;

    IRQn_Type irqn = (IRQn_Type)(cyhal_internal_data[obj->resource.block_num].isr_offset + obj->resource.channel_num);

    /* Only enable if it's not already enabled */
    if (NVIC_GetEnableIRQ(irqn) == 0)
    {
        cy_stc_sysint_t irqCfg = {irqn, priority};
        Cy_SysInt_Init(&irqCfg, interrupts_dispatcher_table[index]);
        NVIC_EnableIRQ(irqn);
    }
    else
    {
        NVIC_SetPriority(irqn, priority);
    }
}

void cyhal_timer_irq_enable(cyhal_timer_t *obj, cyhal_timer_irq_event_t event, bool enable)
{
    Cy_TCPWM_SetInterruptMask(obj->base, obj->resource.channel_num, enable ? (uint32_t)event : 0);
}

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_MXTCPWM_INSTANCES) */
