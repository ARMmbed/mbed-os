/**
 * @file xmc_gpio.c
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

/*******************************************************************************
 * HEADER FILES
 *******************************************************************************/

#include <xmc_gpio.h>

/*******************************************************************************
 * MACROS
 *******************************************************************************/

#define PORT_HWSEL_Msk PORT0_HWSEL_HW0_Msk

/*******************************************************************************
 * API IMPLEMENTATION
 *******************************************************************************/

void XMC_GPIO_SetMode(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_MODE_t mode)
{
  XMC_ASSERT("XMC_GPIO_SetMode: Invalid port", XMC_GPIO_CHECK_PORT(port));
  XMC_ASSERT("XMC_GPIO_SetMode: Invalid mode", XMC_GPIO_IsModeValid(mode));

  port->IOCR[(uint32_t)pin >> 2U] &= ~(uint32_t)((uint32_t)PORT_IOCR_PC_Msk << ((uint32_t)PORT_IOCR_PC_Size * ((uint32_t)pin & 0x3U)));
  port->IOCR[(uint32_t)pin >> 2U] |= (uint32_t)mode << ((uint32_t)PORT_IOCR_PC_Size * ((uint32_t)pin & 0x3U));
}

void XMC_GPIO_SetHardwareControl(XMC_GPIO_PORT_t *const port, const uint8_t pin, const XMC_GPIO_HWCTRL_t hwctrl)
{
  XMC_ASSERT("XMC_GPIO_SetHardwareControl: Invalid port", XMC_GPIO_CHECK_PORT(port));
  XMC_ASSERT("XMC_GPIO_SetHardwareControl: Invalid hwctrl", XMC_GPIO_CHECK_HWCTRL(hwctrl));

  port->HWSEL &= ~(uint32_t)((uint32_t)PORT_HWSEL_Msk << ((uint32_t)pin << 1U));
  port->HWSEL |= (uint32_t)hwctrl << ((uint32_t)pin << 1U);
}
