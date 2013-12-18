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
#include <stddef.h>
#include "cmsis.h"

#include "gpio_irq_api.h"
#include "error.h"

#define CHANNEL_NUM    31

static uint32_t channel_ids[CHANNEL_NUM] = {0}; //each pin will be given an id, if id is 0 the pin can be ignored.
static uint8_t channel_enabled[CHANNEL_NUM] = {0};
static uint32_t  portRISE= 0; 
static uint32_t  portFALL= 0; 
static gpio_irq_handler irq_handler;

volatile uint32_t oldPortValue;
static int test=0;

void GPIOTE_IRQHandler(void)
{
	volatile uint32_t newVal = NRF_GPIO->IN;
	if ((NRF_GPIOTE->EVENTS_PORT != 0) && ((NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_PORT_Msk) != 0)){
		//NRF_GPIOTE->INTENSET &= ~(GPIOTE_INTENSET_PORT_Enabled<<GPIOTE_INTENSET_PORT_Pos);
        NRF_GPIOTE->EVENTS_PORT = 0;				
        for(uint8_t i=0;i<31;i++)
		{
			if(channel_ids[i]>0){				
				if((portRISE>>i)&1){
					if(((newVal>>i)&1) && ((oldPortValue>>i)&1)==0 ){//
						if(channel_enabled[i]){
							irq_handler(channel_ids[i], IRQ_RISE);
						}
						NRF_GPIO->PIN_CNF[i] &= ~(GPIO_PIN_CNF_SENSE_Msk);
						NRF_GPIO->PIN_CNF[i] |= (GPIO_PIN_CNF_SENSE_Low     << GPIO_PIN_CNF_SENSE_Pos) ;						
					}
					else if(((newVal>>i)&1)==0  && ((NRF_GPIO->PIN_CNF[i] >>GPIO_PIN_CNF_SENSE_Pos)&GPIO_PIN_CNF_SENSE_Low) == GPIO_PIN_CNF_SENSE_Low){
						NRF_GPIO->PIN_CNF[i] &= ~(GPIO_PIN_CNF_SENSE_Msk);
						NRF_GPIO->PIN_CNF[i] |= (GPIO_PIN_CNF_SENSE_High     << GPIO_PIN_CNF_SENSE_Pos) ;
					}
				}
				else if((portFALL>>i)&1){
					if(((oldPortValue>>i)&1) && ((newVal>>i)&1)==0 ){
						if(channel_enabled[i]){
							irq_handler(channel_ids[i], IRQ_FALL);
						}
						NRF_GPIO->PIN_CNF[i] &= ~(GPIO_PIN_CNF_SENSE_Msk);
						NRF_GPIO->PIN_CNF[i] |= (GPIO_PIN_CNF_SENSE_High     << GPIO_PIN_CNF_SENSE_Pos) ;
					}
					else if(((newVal>>i)&1) && ((NRF_GPIO->PIN_CNF[i] >>GPIO_PIN_CNF_SENSE_Pos)&GPIO_PIN_CNF_SENSE_Low) != GPIO_PIN_CNF_SENSE_Low){
						NRF_GPIO->PIN_CNF[i] &= ~(GPIO_PIN_CNF_SENSE_Msk);
						NRF_GPIO->PIN_CNF[i] |= (GPIO_PIN_CNF_SENSE_Low     << GPIO_PIN_CNF_SENSE_Pos) ;
					}
				}
			}
		}		
		oldPortValue = newVal;
		//NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Enabled<<GPIOTE_INTENSET_PORT_Pos;
    }
}
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id) {
	if (pin == NC) return -1;
	irq_handler = handler;

    obj->ch = pin;
	
	oldPortValue = NRF_GPIO->IN;
	
	//NVIC_DisableIRQ(GPIOTE_IRQn); 
    NRF_GPIOTE->EVENTS_PORT  = 0;
	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Set<<GPIOTE_INTENSET_PORT_Pos;
    NVIC_EnableIRQ(GPIOTE_IRQn);	
	channel_ids[pin]=id;
	channel_enabled[pin]=1;
}

void gpio_irq_free(gpio_irq_t *obj) {
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable) {   
	portRISE &=~(1<<obj->ch);
	portFALL &=~(1<<obj->ch);
	if(enable){
		if(event == IRQ_RISE){
			portRISE |=(1<<obj->ch);
			NRF_GPIO->PIN_CNF[obj->ch] &= ~(GPIO_PIN_CNF_SENSE_Msk);// | GPIO_PIN_CNF_PULL_Msk);
			NRF_GPIO->PIN_CNF[obj->ch] |= (GPIO_PIN_CNF_SENSE_High     << GPIO_PIN_CNF_SENSE_Pos) ;//| (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);
		}
		else if(event == IRQ_FALL){
			portFALL |=(1<<obj->ch);
			NRF_GPIO->PIN_CNF[obj->ch] &= ~(GPIO_PIN_CNF_SENSE_Msk );//| GPIO_PIN_CNF_PULL_Msk);
			NRF_GPIO->PIN_CNF[obj->ch] |= (GPIO_PIN_CNF_SENSE_Low     << GPIO_PIN_CNF_SENSE_Pos);// | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos);
		}
	}	
	oldPortValue = NRF_GPIO->IN;
}

void gpio_irq_enable(gpio_irq_t *obj) {
	channel_enabled[obj->ch]=1;
}

void gpio_irq_disable(gpio_irq_t *obj) {
	channel_enabled[obj->ch]=0;
}
