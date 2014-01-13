/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "pinmap.h"
#include "error.h"

/**
 * Configure pin (mode, speed, output type and pull-up/pull-down)
 */
void pin_function(PinName pin, int data) {
    GPIO_TypeDef *gpio;
    GPIO_InitTypeDef GPIO_InitStructure;
  
    if (pin == NC) return;

    // Get the pin informations
    uint32_t mode  = STM_PIN_MODE(data);
    uint32_t otype = STM_PIN_OTYPE(data);
    uint32_t pupd  = STM_PIN_PUPD(data);
    uint32_t afnum = STM_PIN_AFNUM(data);

    uint32_t pin_number = (uint32_t)pin;
    uint32_t pin_index  = (pin_number & 0xF);
    uint32_t port_index = (pin_number >> 4);

    // Get GPIO structure base address and enable clock  
    switch (port_index) {
        case PortA:
            gpio = (GPIO_TypeDef *)GPIOA_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
            break;
        case PortB:
            gpio = (GPIO_TypeDef *)GPIOB_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
            break;
        case PortC:
            gpio = (GPIO_TypeDef *)GPIOC_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
            break;
        case PortD:
            gpio = (GPIO_TypeDef *)GPIOD_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
            break;
        case PortH:
            gpio = (GPIO_TypeDef *)GPIOH_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
            break;
        default:
            error("GPIO port number is not correct.");
            break;          
    }
      
    // Configure GPIO
    GPIO_InitStructure.GPIO_Pin   = (uint16_t)(1 << pin_index);
    GPIO_InitStructure.GPIO_Mode  = (GPIOMode_TypeDef)mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = (GPIOOType_TypeDef)otype;
    GPIO_InitStructure.GPIO_PuPd  = (GPIOPuPd_TypeDef)pupd;
    GPIO_Init(gpio, &GPIO_InitStructure);

    // Configure Alternate Function
    if (afnum > 0) {
        GPIO_PinAFConfig(gpio, (uint16_t)(1 << pin_index), afnum);
    }
    
    // *** TODO ***
    // Disconnect JTAG-DP + SW-DP signals.
    // Warning: Need to reconnect under reset
    //if ((pin == PA_13) || (pin == PA_14)) {
    //
    //}
    //if ((pin == PA_15) || (pin == PB_3) || (pin == PB_4)) {
    //
    //}    
}

/**
 * Configure pin pull-up/pull-down
 */
void pin_mode(PinName pin, PinMode mode) {
    GPIO_TypeDef *gpio;
  
    if (pin == NC) return;
  
    uint32_t pin_number = (uint32_t)pin;   
    uint32_t port_index = (pin_number >> 4);

    // Get GPIO structure base address and enable clock  
    switch (port_index) {
        case PortA:
            gpio = (GPIO_TypeDef *)GPIOA_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
            break;
        case PortB:
            gpio = (GPIO_TypeDef *)GPIOB_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
            break;
        case PortC:
            gpio = (GPIO_TypeDef *)GPIOC_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
            break;
        case PortD:
            gpio = (GPIO_TypeDef *)GPIOD_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
            break;
        case PortH:
            gpio = (GPIO_TypeDef *)GPIOH_BASE;
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
            break;
        default:
            error("GPIO port number is not correct.");
            break;          
    }

    // Configure pull-up/pull-down resistors
    uint32_t pupd = (uint32_t)mode;
    if (pupd > 2) pupd = 0; // Open-drain = No pull-up/No pull-down
    gpio->PUPDR &= (uint32_t)(~(GPIO_PUPDR_PUPDR0 << (pin_number * 2)));
    gpio->PUPDR |= (uint32_t)(pupd << (pin_number * 2));
    
}
