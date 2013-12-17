/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
 */
#include "pinmap.h"
#include "error.h"

// Alternate-function mapping
static const uint32_t AF_mapping[] = {
  0,                      // 0 = No AF
  GPIO_Remap_SPI1,        // 1
  GPIO_Remap_I2C1,        // 2
  GPIO_Remap_USART1,      // 3
  GPIO_Remap_USART2,      // 4
  GPIO_FullRemap_TIM2,    // 5
  GPIO_FullRemap_TIM3,    // 6
  GPIO_PartialRemap_TIM3, // 7
  GPIO_Remap_I2C1         // 8
};

/**
 * Set the pin function (input, output, alternate function or analog) + output speed + AF
 */
void pin_function(PinName pin, int data) {
    if (pin == NC) return;

    // Get the pin mode and alternate-function number
    uint32_t mode  = STM_PIN_MODE(data);
    uint32_t afnum = STM_PIN_AFNUM(data);

    // Get GPIO structure base address
    uint32_t pin_number = (uint32_t)pin;
    uint32_t pin_index  = (pin_number & 0xF);
    uint32_t port_index = (pin_number >> 4);
    GPIO_TypeDef *gpio  = ((GPIO_TypeDef *)(GPIOA_BASE + (port_index << 10)));

    // Enable GPIO and AFIO clocks
    RCC_APB2PeriphClockCmd((uint32_t)(RCC_APB2Periph_GPIOA << port_index), ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
    // Configure GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = (uint16_t)(1 << pin_index);
    GPIO_InitStructure.GPIO_Mode  = (GPIOMode_TypeDef)mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio, &GPIO_InitStructure);

    // Configure Alternate Function
    if (afnum > 0) {
        GPIO_PinRemapConfig(AF_mapping[afnum], ENABLE);
    }
    
    // Disconnect JTAG-DP + SW-DP signals.
    // Warning: Need to reconnect under reset
    if ((pin == PA_13) || (pin == PA_14)) {
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    }
    if ((pin == PA_15) || (pin == PB_3) || (pin == PB_4)) {
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    }    
}

/**
 * Set the pin mode (open-drain/push-pull + pull-up/pull-down)
 */
void pin_mode(PinName pin, PinMode mode) {
    if (pin == NC) return;

    GPIO_InitTypeDef GPIO_InitStructure;
  
    // Get GPIO structure base address
    uint32_t pin_number = (uint32_t)pin;
    uint32_t pin_index  = (pin_number & 0xF);
    uint32_t port_index = (pin_number >> 4);
    GPIO_TypeDef *gpio  = ((GPIO_TypeDef *)(GPIOA_BASE + (port_index << 10)));

    // Enable GPIO clock
    RCC_APB2PeriphClockCmd((uint32_t)(RCC_APB2Periph_GPIOA << port_index), ENABLE);
  
    // Configure open-drain and pull-up/down
    switch (mode) {
      case PullNone:       
        return;
      case PullUp:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
      break;
      case PullDown:
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
      break;
      case OpenDrain:
        if (pin_index < 8) {
            if ((gpio->CRL & (0x03 << (pin_index * 4))) > 0) { // MODE bits = Output mode
                gpio->CRL |= (0x04 << (pin_index * 4)); // Set open-drain
            }
        }
        else {
            if ((gpio->CRH & (0x03 << ((pin_index % 8) * 4))) > 0) { // MODE bits = Output mode          
                gpio->CRH |= (0x04 << ((pin_index % 8) * 4)); // Set open-drain
            }
        }
        return;
      default:
      break;
    }
    
    // Configure GPIO
    GPIO_InitStructure.GPIO_Pin   = (uint16_t)(1 << pin_index);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio, &GPIO_InitStructure);    
}
