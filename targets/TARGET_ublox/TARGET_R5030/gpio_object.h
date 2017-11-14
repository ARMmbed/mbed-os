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
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#include "mbed_assert.h"
#include "KM_app.h"
#include "pio.h"


#ifdef __cplusplus
extern "C" {
#endif


// ---------------------------------------------------------------------------
// Macros
// ---------------------------------------------------------------------------

/** KM can support up to 64 PIO channels */
#define MAX_NUM_PIO_CHANNELS     64

/** Each register is 32 bits which can have 32 PIO channels */
#define PIO_CHANNEL_SUB_32_MASK     0x1F

/** Right shift this value to get the value for multiple of 32, which can be used to calculat the PIO register base for a specific Channel */
#define PIO_CHANNEL_OVER_32_SHIFT     5

/** GPIO base address */
#define gpio_base     app_ss_pio_onfi_wrap_maps_pio

#define ERROR     0
#define SUCCESS     1


/** GPIO channels */
#define GPIIO_0_CHANNEL     0
#define GPIIO_1_CHANNEL     1
#define GPIIO_2_CHANNEL     2
#define GPIIO_3_CHANNEL     3
#define GPIIO_4_CHANNEL     4
#define GPIIO_5_CHANNEL     5
#define GPIIO_6_CHANNEL     6
#define GPIIO_10_CHANNEL     10
#define GPIIO_11_CHANNEL     11
#define GPIIO_12_CHANNEL     12
#define GPIIO_13_CHANNEL     13
#define GPIIO_14_CHANNEL     14
#define GPIIO_15_CHANNEL     15


typedef struct {
    PinName  pin;
    uint32_t mask;

	__IO struct pio_s *reg_base;
} gpio_t;


/**
 * Enum for peripheral muxing: refer to top-level excel pinlist
 * for the possible peripheral connections for each channel.
 */
typedef enum PioPeriphMux_e {
  PIO_MUX_FALSE,
  PIO_MUX_GPIO,
  PIO_MUX_PERIPH_0,
  PIO_MUX_PERIPH_1,
  PIO_MUX_PERIPH_2,
  PIO_MUX_PERIPH_3,
  PIO_MUX_LAST
} PioPeriphMux;


/*****************************************************************************
								gpio_write:
						    Write data on gpio
*****************************************************************************/
static inline void gpio_write(gpio_t *obj, int value)
{
  MBED_ASSERT(obj->pin != (PinName)NC);

  if (value)
  {
  	obj->reg_base->pio_sodr_0 = obj->mask;  // Set output pin
  }
  else
  {
  	obj->reg_base->pio_codr_0 = obj->mask; // Clear output pin
  }
}

/*****************************************************************************
								gpio_read:
						    Read data on gpio
*****************************************************************************/
static inline int gpio_read(gpio_t *obj)
{
  MBED_ASSERT(obj->pin != (PinName)NC);

  return ((obj->reg_base->pio_pdsr_0 & obj->mask) ? 1 : 0);
}


static inline int gpio_is_connected(const gpio_t *obj) {
    return obj->pin != (PinName)NC;
}



uint32_t gpio_periph_mux_set(uint8_t mux, uint8_t pioChannel, bool periphPullUpDownOn);
uint8_t gpio_channel_select(PinName pin);


#ifdef __cplusplus
}
#endif

#endif
