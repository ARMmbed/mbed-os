/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
 */

#include "cmsis.h"
#include "gpio_irq_api.h"
#include "pinmap.h"
#include "BlueNRG1_gpio.h"

static gpio_irq_handler irq_handler;
static gpio_irq_t *obj_ptr;

void GPIO_Handler(void){
	//clear interrupt flag
	if(GPIO_GetITStatusBit(obj_ptr->exti.GPIO_Pin) && GPIO_GetITPendingBit(obj_ptr->exti.GPIO_Pin))
		GPIO_ClearITPendingBit(obj_ptr->exti.GPIO_Pin);
	irq_handler(obj_ptr->id, obj_ptr->exti.GPIO_Event);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
	if(pin == NC)
		return -1;
	NVIC_DisableIRQ(GPIO_IRQn);
	obj_ptr = obj;

	obj->id = id;
	GPIO_EXTIStructInit(&obj->exti); //default values
	obj->exti.GPIO_IrqSense = GPIO_IrqSense_Edge;  //fixed, mbed api does not support sense level interrupts
	obj->exti.GPIO_Event = GPIO_Event_Both;  //default value at initialization, used as control when fall and rise interrupts are required at the same time on the same pin
	obj->exti.GPIO_Pin = (uint32_t)pin;
	GPIO_EXTIConfig(&obj->exti);

	irq_handler = handler;

	NVIC_SetVector(GPIO_IRQn, (uint32_t)&GPIO_Handler);

	//GPIO_EXTICmd((uint32_t)pin, (FunctionalState)1); //////da rivedere

	NVIC_EnableIRQ(GPIO_IRQn);
	return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
	GPIO_ClearITPendingBit(obj->exti.GPIO_Pin);
	GPIO_EXTICmd(obj->exti.GPIO_Pin, DISABLE);
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
	///there is a mismatch between MBED enum gpio_irq_event and DK defines in BlueNRG1_gpio.h
	///mbed API  : none=0, rise=1, fall=2
	///DK defines: fall=0, rise=1, both=2
	///
	///A lookup table is defined
	gpio_irq_event event_new;
	FunctionalState enable_new;
	switch(event){
		case IRQ_NONE:
			event_new = (gpio_irq_event)GPIO_Event_Both;  //its initialize value
			enable_new = DISABLE;
			break;
		case IRQ_RISE:
			event_new = (gpio_irq_event)GPIO_Event_High;
			enable_new = ENABLE;
			break;
		case IRQ_FALL:
			event_new = (gpio_irq_event)GPIO_Event_Low;
			enable_new = ENABLE;
			break;
	}
	if(obj->exti.GPIO_Event != GPIO_Event_Both)
		event_new = GPIO_Event_Both;
	///end of lut
	obj->exti.GPIO_Event = event_new;
	GPIO_EXTIConfig(&obj->exti);
	obj->exti.GPIO_Event = event; //to avoid the IRQ_FALL to be mapped into IRQ_none
	GPIO_EXTICmd(obj->exti.GPIO_Pin, (FunctionalState)enable_new);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
	NVIC_EnableIRQ(GPIO_IRQn);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
	NVIC_DisableIRQ(GPIO_IRQn);
	NVIC_ClearPendingIRQ(GPIO_IRQn);
}
