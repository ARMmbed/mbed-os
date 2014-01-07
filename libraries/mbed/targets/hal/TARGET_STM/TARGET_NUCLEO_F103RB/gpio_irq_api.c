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
#include <stddef.h>
#include "cmsis.h"

#include "gpio_irq_api.h"
#include "error.h"

#define EDGE_NONE (0)
#define EDGE_RISE (1)
#define EDGE_FALL (2)
#define EDGE_BOTH (3)

#define CHANNEL_NUM (16)

static uint32_t channel_ids[CHANNEL_NUM] = {0};

static gpio_irq_handler irq_handler;

static void handle_interrupt_in(uint32_t channel) {
    if (channel_ids[channel] == 0) return;
  
    uint32_t exti_line = (uint32_t)(1 << channel);
    if (EXTI_GetITStatus(exti_line) != RESET)
    {
        EXTI_ClearITPendingBit(exti_line);
    }
    
    // Warning:
    // On this device we don't know if a rising or falling event occured.
    // In case both rise and fall events are set, only the FALL event will be reported. 
    if (EXTI->FTSR & (uint32_t)(1 << channel)) {
        irq_handler(channel_ids[channel], IRQ_FALL);
    }
    else {
        irq_handler(channel_ids[channel], IRQ_RISE);
    }
}

static void gpio_irq0(void) {handle_interrupt_in(0);}
static void gpio_irq1(void) {handle_interrupt_in(1);}
static void gpio_irq2(void) {handle_interrupt_in(2);}
static void gpio_irq3(void) {handle_interrupt_in(3);}
static void gpio_irq4(void) {handle_interrupt_in(4);}
static void gpio_irq5(void) {handle_interrupt_in(5);}
static void gpio_irq6(void) {handle_interrupt_in(6);}
static void gpio_irq7(void) {handle_interrupt_in(7);}
static void gpio_irq8(void) {handle_interrupt_in(8);}
static void gpio_irq9(void) {handle_interrupt_in(9);}
static void gpio_irq10(void) {handle_interrupt_in(10);}
static void gpio_irq11(void) {handle_interrupt_in(11);}
static void gpio_irq12(void) {handle_interrupt_in(12);}
static void gpio_irq13(void) {handle_interrupt_in(13);}
static void gpio_irq14(void) {handle_interrupt_in(14);}
static void gpio_irq15(void) {handle_interrupt_in(15);}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
    IRQn_Type irq_n = (IRQn_Type)0;  
    uint32_t vector = 0;
  
    if (pin == NC) return -1;

    uint32_t pin_number = (uint32_t)pin;
    uint32_t pin_index  = (pin_number & 0xF);  
    uint32_t port_index = (pin_number >> 4); 
  
    // Select irq number and vector
    switch (pin_index) {
        case 0:
            irq_n = EXTI0_IRQn;
            vector = (uint32_t)&gpio_irq0;
            break;
        case 1:
            irq_n = EXTI1_IRQn;
            vector = (uint32_t)&gpio_irq1;
            break;
        case 2:
            irq_n = EXTI2_IRQn;
            vector = (uint32_t)&gpio_irq2;
            break;
        case 3:
            irq_n = EXTI3_IRQn;
            vector = (uint32_t)&gpio_irq3;        
            break;
        case 4:
            irq_n = EXTI4_IRQn;
            vector = (uint32_t)&gpio_irq4;        
            break;
        case 5:
            irq_n = EXTI9_5_IRQn;
            vector = (uint32_t)&gpio_irq5;        
            break;          
        case 6:
            irq_n = EXTI9_5_IRQn;
            vector = (uint32_t)&gpio_irq6;        
            break;          
        case 7:
            irq_n = EXTI9_5_IRQn;
            vector = (uint32_t)&gpio_irq7;        
            break;          
        case 8:
            irq_n = EXTI9_5_IRQn;
            vector = (uint32_t)&gpio_irq8;        
            break;          
        case 9:       
            irq_n = EXTI9_5_IRQn;
            vector = (uint32_t)&gpio_irq9;
            break;
        case 10:
            irq_n = EXTI15_10_IRQn;
            vector = (uint32_t)&gpio_irq10;
            break;          
        case 11:
            irq_n = EXTI15_10_IRQn;
            vector = (uint32_t)&gpio_irq11;        
            break;          
        case 12:
            irq_n = EXTI15_10_IRQn;
            vector = (uint32_t)&gpio_irq12;        
            break;          
        case 13:
            irq_n = EXTI15_10_IRQn;
            vector = (uint32_t)&gpio_irq13;        
            break;          
        case 14:
            irq_n = EXTI15_10_IRQn;
            vector = (uint32_t)&gpio_irq14;
            break;          
        case 15:
            irq_n = EXTI15_10_IRQn;
            vector = (uint32_t)&gpio_irq15;
            break;        
        default:
            return -1;
    }
    
    // Enable GPIO and AFIO clocks
    RCC_APB2PeriphClockCmd((uint32_t)(RCC_APB2Periph_GPIOA << port_index), ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
    // Connect EXTI line to pin
    GPIO_EXTILineConfig(port_index, pin_index);

    // Configure EXTI line
    EXTI_InitTypeDef EXTI_InitStructure;    
    EXTI_InitStructure.EXTI_Line = (uint32_t)(1 << pin_index);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // Enable and set EXTI interrupt to the lowest priority
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = irq_n;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);

    // Save for future use
    obj->ch = pin_index;
    obj->irq_n = irq_n;
    obj->event = EDGE_NONE;
    
    channel_ids[obj->ch] = id;
  
    irq_handler = handler; 
  
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
    // Disable EXTI line
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_Init(&EXTI_InitStructure);  
    obj->event = EDGE_NONE;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {
    EXTI_InitTypeDef EXTI_InitStructure;
    
    EXTI_InitStructure.EXTI_Line = (uint32_t)(1 << obj->ch);
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    
    if (event == IRQ_RISE) {
        if ((obj->event == EDGE_FALL) || (obj->event == EDGE_BOTH)) {
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
            obj->event = EDGE_BOTH;
        }
        else { // NONE or RISE
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
            obj->event = EDGE_RISE;
        }
    }
    
    if (event == IRQ_FALL) {
        if ((obj->event == EDGE_RISE) || (obj->event == EDGE_BOTH)) {
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
            obj->event = EDGE_BOTH;
        }
        else { // NONE or FALL
            EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
            obj->event = EDGE_FALL;
        }
    }
    
    if (enable) {
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    }
    else {
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    }
    
    EXTI_Init(&EXTI_InitStructure);
}

void gpio_irq_enable(gpio_irq_t *obj) {
    NVIC_EnableIRQ(obj->irq_n);
}

void gpio_irq_disable(gpio_irq_t *obj) {
    NVIC_DisableIRQ(obj->irq_n);
    obj->event = EDGE_NONE;
}
