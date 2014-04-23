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

#include <assert.h>
#include "fsl_interrupt_manager.h"
#include "cmsis_nvic.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Counter to manage the nested callings of global disable/enable interrupt.
 */
uint32_t g_interrupt_disable_count = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : interrupt_register_handler
 * Description   : Install an interrupt handler routine for a given IRQ number
 * This function will let application to register/replace the interrupt 
 * handler for specified IRQ number. IRQ number is different with Vector
 * number. IRQ 0 will start from Vector 16 address. Refer to reference
 * manual for details. Also refer to startup_MKxxxx.s file for each chip
 * family to find out the default interrut handler for each device. This
 * function will convert the IRQ number to vector number by adding 16 to
 * it. 
 * 
 *END**************************************************************************/
void interrupt_register_handler(IRQn_Type irqNumber, void (*handler)(void))
{
    NVIC_SetVector(irqNumber, (uint32_t)handler);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : interrupt_enable_global
 * Description   : Enable system interrupt
 * This function will enable the global interrupt by calling the core API
 * 
 *END**************************************************************************/
void interrupt_enable_global(void)
{
    /* check and update */
    if (g_interrupt_disable_count > 0)
    {
        g_interrupt_disable_count--;

        if (g_interrupt_disable_count > 0)
        {
            return;
        }

        /* call core API to enable the global interrupt*/
        __enable_irq();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : interrupt_disable_global
 * Description   : Disnable system interrupt
 * This function will disable the global interrupt by calling the core API
 * 
 *END**************************************************************************/
void interrupt_disable_global(void)
{
    /* call core API to disable the global interrupt*/
    __disable_irq();

    /* update counter*/
    g_interrupt_disable_count++;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

