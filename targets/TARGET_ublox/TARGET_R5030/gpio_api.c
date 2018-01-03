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
    int pioChannel;
    PioPeriphMux mux;

    MBED_ASSERT(pin != (PinName)NC);

    obj->pin = pin;
    obj->reg_base = gpio_base;

    pioChannel = gpio_channel_select(obj->pin,PinChannelMap);
    MBED_ASSERT(pioChannel != ERROR);

    obj->mask = (1 << (pioChannel & PIO_CHANNEL_SUB_32_MASK));

    mux = pio_periph_muxing_get(pioChannel);
    MBED_ASSERT(mux != PIO_MUX_FALSE);

    status = gpio_periph_mux_set(mux,pioChannel,true);
    MBED_ASSERT(status == SUCCESS);
}

/*****************************************************************************
                                gpio_mode:
                          Set the mode of gpio
*****************************************************************************/
void gpio_mode(gpio_t *obj, PinMode mode)
{
    MBED_ASSERT(obj->pin != (PinName)NC);

    switch (mode)
    {
      case PullNone:
    	  obj->reg_base->pio_phdr_0 = obj->mask; // Pull up disable
    	  obj->reg_base->pio_pldr_0 = obj->mask; // Pull down disable
    	  break;

      case PullDown:
    	  obj->reg_base->pio_pler_0 = obj->mask; // Set pull down
    	  break;

      case PullUp:
    	  obj->reg_base->pio_pher_0 = obj->mask; // Set pull up
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

    switch (direction)
    {
      case PIN_INPUT:
    	  obj->reg_base->pio_iner_0 = obj->mask; // Enable input
    	  break;

      case PIN_OUTPUT:
    	  obj->reg_base->pio_oer_0 = obj->mask; // Enable output
    	  break;

      default:
    	  break;
    }
}

/*****************************************************************************
                            gpio_channel_select:
                           GPIO channel selection
*****************************************************************************/
int gpio_channel_select(PinName pin, const PinToChannel* map)
{
    while (map->pin != NC)
    {
        if (map->pin == pin)
            return map->channel;
        map++;
    }

    return ERROR;
}

/*****************************************************************************
                           pio_periph_muxing_set:
                                 Enable mux
*****************************************************************************/
int gpio_periph_mux_set(PioPeriphMux mux, uint8_t pioChannel, bool periphPullUpDownOn)
{
    uint8_t channel_offset_in_reg = (pioChannel & PIO_CHANNEL_SUB_32_MASK);
    struct pio_s *pio_channel_regbase;

    /*PIO base address */
    if((pioChannel >> PIO_CHANNEL_OVER_32_SHIFT) == 0)
    {
    	pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
    }
    else
    {
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

    return SUCCESS;
}

/*****************************************************************************
                            pio_periph_muxing_get:
                         Find out which mux is used
*****************************************************************************/
PioPeriphMux pio_periph_muxing_get(uint8_t pioChannel)
{
    uint8_t muxSel;
    PioPeriphMux mux = PIO_MUX_FALSE;
    struct pio_s *pio_channel_regbase;
    uint8_t channel_offset_in_reg = (pioChannel & PIO_CHANNEL_SUB_32_MASK);

    /*PIO base address */
    if((pioChannel >> PIO_CHANNEL_OVER_32_SHIFT) == 0)
    {
    	pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
    }
    else
    {
    	pio_channel_regbase = (struct pio_s *)(PIO_CONTROL_BASE + 0x220);
    }


    if(PIO_CHANNEL_BIT_GET(pio_channel_regbase->pio_psr_0, channel_offset_in_reg) == 1)
    {
    	mux = PIO_MUX_GPIO;
    }
    else /* configured for peripheral - find out which one */
    {
    	muxSel = (uint8_t)((PIO_CHANNEL_BIT_GET(pio_channel_regbase->pio_mselsr_0, channel_offset_in_reg) << 1) | PIO_CHANNEL_BIT_GET(pio_channel_regbase->pio_lselsr_0, channel_offset_in_reg));

       /*Selecting mux */
       switch(muxSel)
       {
         case 0:
             mux = PIO_MUX_PERIPH_0;
             break;
			 
         case 1:
             mux = PIO_MUX_PERIPH_1;
             break;
			 
         case 2:
             mux = PIO_MUX_PERIPH_2;
             break;
			 
         case 3:
             mux = PIO_MUX_PERIPH_3;
             break;
			 
         default:
             return mux;
       }

    }

    return mux;
}
