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
 
#include "fsl_gpio_hal.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : gpio_hal_set_pin_direction
 * Description   : Set individual gpio pin to general input or output.
 *
 *END**************************************************************************/
void gpio_hal_set_pin_direction(uint32_t instance, uint32_t pin,
                                gpio_pin_direction_t direction)
{
    assert(instance < HW_GPIO_INSTANCE_COUNT);
    assert(pin < 32);

    if (kGpioDigitalOutput == direction)
    {
        HW_GPIO_PDDR_WR(instance, ((uint32_t)1 << pin) | HW_GPIO_PDDR_RD(instance));  
    }
    else
    {
        HW_GPIO_PDDR_WR(instance, (~((uint32_t)1 << pin)) & HW_GPIO_PDDR_RD(instance));
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : gpio_hal_write_pin_output
 * Description   : Set output level of individual gpio pin to logic 1 or 0.
 *
 *END**************************************************************************/
void gpio_hal_write_pin_output(uint32_t instance, uint32_t pin, uint32_t output)
{
    assert(instance < HW_GPIO_INSTANCE_COUNT);
    assert(pin < 32);
    if (output != 0U)
    {
        gpio_hal_set_pin_output(instance, pin); /* Set pin output to high level.*/
    }
    else
    {
        gpio_hal_clear_pin_output(instance, pin); /* Set pin output to low level.*/
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
