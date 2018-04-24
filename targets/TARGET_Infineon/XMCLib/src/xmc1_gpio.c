/**
 * @file xmc1_gpio.c
 * @date 2015-06-20
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial draft <br>
 *      
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 *
 * @endcond
 *
 */

#include "xmc_gpio.h"

#if UC_FAMILY == XMC1

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define PORT_PHCR_Msk             PORT0_PHCR0_PH0_Msk
#define PORT_PHCR_Size            PORT0_PHCR0_PH0_Msk
#define PORT_HWSEL_Msk            PORT0_HWSEL_HW0_Msk

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

void XMC_GPIO_Init(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_CONFIG_t *const config)
{
  XMC_ASSERT("XMC_GPIO_Init: Invalid port", XMC_GPIO_CHECK_PORT(port));
  XMC_ASSERT("XMC_GPIO_Init: Invalid mode", XMC_GPIO_IsModeValid(config->mode));
  XMC_ASSERT("XMC_GPIO_Init: Invalid input hysteresis", XMC_GPIO_CHECK_INPUT_HYSTERESIS(config->input_hysteresis));
  
  /* Switch to input */
  port->IOCR[pin >> 2U] &= ~(uint32_t)((uint32_t)PORT_IOCR_PC_Msk << (PORT_IOCR_PC_Size * (pin & 0x3U)));

  /* HW port control is disabled */
  port->HWSEL &= ~(uint32_t)((uint32_t)PORT_HWSEL_Msk << ((uint32_t)pin << 1U));

  /* Set input hysteresis */
  port->PHCR[(uint32_t)pin >> 3U] &= ~(uint32_t)((uint32_t)PORT_PHCR_Msk << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U)));
  port->PHCR[(uint32_t)pin >> 3U] |= (uint32_t)config->input_hysteresis << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U));
    
  /* Enable digital input */
  if (XMC_GPIO_CHECK_ANALOG_PORT(port))
  {    
    port->PDISC &= ~(uint32_t)((uint32_t)0x1U << pin);
  }
  /* Set output level */
  port->OMR = (uint32_t)config->output_level << pin;
  
  /* Set mode */
  port->IOCR[pin >> 2U] |= (uint32_t)config->mode << (PORT_IOCR_PC_Size * (pin & 0x3U));
}

void XMC_GPIO_SetInputHysteresis(XMC_GPIO_PORT_t *const port, 
                                 const uint8_t pin, 
                                 const XMC_GPIO_INPUT_HYSTERESIS_t hysteresis)
{
  XMC_ASSERT("XMC_GPIO_SetInputHysteresis: Invalid port", XMC_GPIO_CHECK_PORT(port));
  XMC_ASSERT("XMC_GPIO_SetInputHysteresis: Invalid input hysteresis", XMC_GPIO_CHECK_INPUT_HYSTERESIS(hysteresis));

  port->PHCR[(uint32_t)pin >> 3U] &= ~(uint32_t)((uint32_t)PORT_PHCR_Msk << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U)));
  port->PHCR[(uint32_t)pin >> 3U] |= (uint32_t)hysteresis << ((uint32_t)PORT_PHCR_Size * ((uint32_t)pin & 0x7U));
}

#endif /* UC_FAMILY == XMC1 */
