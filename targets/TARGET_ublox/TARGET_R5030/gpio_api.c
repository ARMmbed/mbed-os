/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

#include "mbed_assert.h"
#include "gpio_api.h"
#include "pinmap.h"


/*****************************************************************************
								gpio_set:
						Set the given pin as GPIO
*****************************************************************************/
uint32_t gpio_set(PinName pin)
{
  MBED_ASSERT(pin != (PinName)NC);

  return (uint32_t) 1;
}

/*****************************************************************************
								gpio_init:
						   Initialize the GPIO
*****************************************************************************/
void gpio_init(gpio_t *obj, PinName pin)
{
  uint32_t status;
  uint8_t pioChannel;

  MBED_ASSERT(pin != (PinName)NC);

  obj->reg_base = gpio_base;

  obj->pin = pin;

  pioChannel = gpio_channel_select(obj->pin);
  obj->mask = (1 << (pioChannel & PIO_CHANNEL_SUB_32_MASK));

  status = gpio_periph_mux_set(PIO_MUX_GPIO,pioChannel,true);
  
  MBED_ASSERT(status == SUCCESS);
}

/*****************************************************************************
								gpio_mode:
						  Set the mode of gpio
*****************************************************************************/
void gpio_mode(gpio_t *obj, PinMode mode)
{
	MBED_ASSERT(obj->pin != (PinName)NC);

	switch (mode) {
		case PullNone:
		{
			obj->reg_base->pio_phdr_0 = obj->mask; // Pull up disable
			obj->reg_base->pio_pldr_0 = obj->mask; // Pull down disable
		}
		break;
		case PullDown:
		{
			obj->reg_base->pio_pler_0 = obj->mask; // Set pull down
		}
		break;
		case PullUp:
		{
			obj->reg_base->pio_pher_0 = obj->mask; // Set pull up
		}
		break;
		default:
		break;
	}
}

/*****************************************************************************
								gpio_dir:
						Set the direction of gpio
*****************************************************************************/
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    MBED_ASSERT(obj->pin != (PinName)NC);

	switch (direction) {
		case PIN_INPUT:
		{
			obj->reg_base->pio_iner_0 = obj->mask; // Enable input
		}
		break;
		case PIN_OUTPUT:
		{
			obj->reg_base->pio_oer_0 = obj->mask; // Enable output
		}
		break;
	}
}

/*****************************************************************************
							pio_periph_muxing_set:
						    GPIO channel setection
*****************************************************************************/
uint8_t gpio_channel_select(PinName pin)
{	
    switch(pin)
    {
      case LED1:
        return GPIIO_1_CHANNEL;	
        break;

      case LED2:
        return GPIIO_2_CHANNEL;	
        break;
		
      case LED3:
        return GPIIO_3_CHANNEL;	
        break;

      case LED4:
        return GPIIO_4_CHANNEL;	
        break;

      default:
        return ERROR;
    }
}

/*****************************************************************************
							pio_periph_muxing_set:
								 Enable mux
*****************************************************************************/
uint32_t gpio_periph_mux_set(uint8_t mux, uint8_t pioChannel, bool periphPullUpDownOn)
{
  uint32_t status = SUCCESS;
  uint8_t channel_offset_in_reg = (pioChannel & PIO_CHANNEL_SUB_32_MASK);
  struct pio_s *pio_channel_regbase;

	/*PIO base address */
    if((pioChannel >> PIO_CHANNEL_OVER_32_SHIFT) == 0){
		pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
	}
	else{
		pio_channel_regbase = (struct pio_s *)(PIO_CONTROL_BASE + 0x220);
	}

	/*Mux Enable */
    switch(mux)
    {
      case PIO_MUX_GPIO:
			pio_channel_regbase->pio_per_0 |= (1 << channel_offset_in_reg);
        break;

      case PIO_MUX_PERIPH_0:
			pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);
			pio_channel_regbase->pio_asr_0 |= (1 << channel_offset_in_reg);
        break;

      case PIO_MUX_PERIPH_1:
			pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);
			pio_channel_regbase->pio_bsr_0 |= (1 << channel_offset_in_reg);
        break;

      case PIO_MUX_PERIPH_2:
			pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);
			pio_channel_regbase->pio_csr_0 |= (1 << channel_offset_in_reg);
        break;

      case PIO_MUX_PERIPH_3:
			pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);
			pio_channel_regbase->pio_dsr_0 |= (1 << channel_offset_in_reg);
        break;

      default:
        return ERROR;
    }

	/*Peripheral Pull UP/Down */
	if(periphPullUpDownOn)
	{
		pio_channel_regbase->pio_percper_0 = (1 << channel_offset_in_reg);
	}
	else
	{
		pio_channel_regbase->pio_percpdr_0 = (1 << channel_offset_in_reg);
	}

  return status;
}
