/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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
 
#include <stdlib.h>
#include <assert.h>
#include "fsl_pit_driver.h"

/*!
 * @addtogroup pit_irq
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined (KL25Z4_SERIES)
/*!
 * @brief Table to save PIT IRQ enum numbers defined in CMSIS files. 
 *
 * They are used by pit_init_channel to enable or disable PIT interrupts. This table is
 * indexed by channel number which could return PIT IRQ numbers.
 */
const IRQn_Type pit_irq_ids[FSL_FEATURE_PIT_TIMER_COUNT] = 
{
    PIT_IRQn, PIT_IRQn
};
#elif defined (K64F12_SERIES) || defined (K70F12_SERIES) || defined (K22F51212_SERIES)
const IRQn_Type pit_irq_ids[FSL_FEATURE_PIT_TIMER_COUNT] = 
{
    PIT0_IRQn, PIT1_IRQn, PIT2_IRQn, PIT3_IRQn
};
#endif

/*!
 * @brief Function table to save PIT isr callback function pointers.
 *
 * Call pit_register_isr_callback_function to install isr callback functions.
 */
pit_isr_callback_t pit_isr_callback_table[FSL_FEATURE_PIT_TIMER_COUNT] = {NULL};

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined (KL25Z4_SERIES)
/*! 
 * @brief System default IRQ handler defined in startup code.
 *
 * Users can either edit this handler or define a callback function. Furthermore,
 * interrupt manager could be used to re-map the IRQ handler to another function. 
 */
void PIT_IRQHandler(void)
{
    uint32_t i;
    for(i=0; i < FSL_FEATURE_PIT_TIMER_COUNT; i++)
    {
        /* Run callback function if it exists.*/
        if (pit_isr_callback_table[i])
        {
            (*pit_isr_callback_table[i])();
        }

        /* Clear interrupt flag.*/
        pit_hal_clear_interrupt_flag(i);

    }
}

#elif defined (K64F12_SERIES) || defined (K70F12_SERIES) || defined (K22F51212_SERIES)

void PIT0_IRQHandler(void)
{
    /* Run callback function if it exists.*/
    if (pit_isr_callback_table[0])
    {
        (*pit_isr_callback_table[0])();
    }

    /* Clear interrupt flag.*/
    pit_hal_clear_interrupt_flag(0);
}

void PIT1_IRQHandler(void)
{
    /* Run callback function if it exists.*/
    if (pit_isr_callback_table[1])
    {
        (*pit_isr_callback_table[1])();
    }

    /* Clear interrupt flag.*/
    pit_hal_clear_interrupt_flag(1);
}

void PIT2_IRQHandler(void)
{
    /* Run callback function if it exists.*/
    if (pit_isr_callback_table[2])
    {
        (*pit_isr_callback_table[2])();
    }
    
    /* Clear interrupt flag.*/
    pit_hal_clear_interrupt_flag(2);
}

void PIT3_IRQHandler(void)
{
    /* Run callback function if it exists.*/
    if (pit_isr_callback_table[3])
    {
        (*pit_isr_callback_table[3])();
    }

    /* Clear interrupt flag.*/
    pit_hal_clear_interrupt_flag(3);
}
#endif

/*! @} */

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_register_isr_callback_function 
 * Description   : Register pit isr callback function. 
 * System default ISR interfaces are already defined in fsl_pit_irq.c. Users 
 * can either edit these ISRs or use this function to register a callback
 * function. The default ISR will run the callback function it there is one
 * installed here.

 *END**************************************************************************/
void pit_register_isr_callback_function(uint32_t timer, pit_isr_callback_t function)
{
    assert(timer < FSL_FEATURE_PIT_TIMER_COUNT);
    assert(function != NULL);
    
    pit_isr_callback_table[timer] = function;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

