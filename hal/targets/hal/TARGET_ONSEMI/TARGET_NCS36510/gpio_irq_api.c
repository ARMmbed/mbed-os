/**
 ******************************************************************************
 * @file gpio_irq_api.c
 * @brief Implementation of a GPIO irq handlers
 * @internal
 * @author ON Semiconductor
 * $Rev: 
 * $Date: 2015-11-04 $
 ******************************************************************************
 * @copyright (c) 2012 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup gpio
 *
 * @details
 *
 * @internal
 * <h1> Reference document(s) </h1>
 * <p>
 * Reference document: IPC7203 APB GPIO Design Specification v1.2</a>
 * </p>
 * @endinternal
 *
 * <h1> Functional description (internal) </h1>
 * <p>
 * Each GPIO line can be independently programmed as an input or an output. Separate Set
 * and Clear registers are provided since it is likely that different software tasks may be
 * servicing different I/O signals. Inputs are synchronized to the system clock 
 * through a pair of flip-flops.  Each input can be programmed
 * to cause an interrupt to be generated.  The interrupt can be programmed to be level-sensitive
 * or edge-sensitive and the level (high or low) or edge (rising, falling or either) that causes
 * the interrupt can be selected.  Interrupts can be individually enabled or disabled.
 * Level-sensitive interrupts stay asserted until the interrupting condition is cleared.
 * Edge-triggered interrupts are cleared by writing to the GPIO interrupt clear register.
 * </p>
 *
 * <h1> Use of GPIO driver in SW </h1>
 * <p>
 * The user of the GPIO driver should set the pin as GPIO, using crossbar.
 * Init the GPIO and configure the mode and direction.This will return a device pointer.  One device controls all GPIO's.  It is not
 * needed nor supported to create a device per GPIO.
 * Next, the user should call the fGpioOpen function with the device and options as paramter.
 * </p>
 * <p>
 * Use the device driver fGpioIoctl function to change the behavior of the GPIO's and to register an
 * interrupt handler for each IO that has an interrupt enabled.  There is one interrupt for all GPIO's.
 * The GPIO driver will look up what IO caused the interrupt and call the respective interrupt handler.
 * </p>
 */



#include "gpio.h"

/* Include files from the mbed-hal layer */
#include "gpio_irq_api.h"

#include "device.h"  

#if DEVICE_INTERRUPTIN

/* Handler for the GPIO pin */
static gpio_irq_handler irq_handler;
static uint32_t gpioIds[NUMBER_OF_GPIO] = {0};

/** Main GPIO IRQ handler called from vector table handler
 *
 * @param gpioBase	The GPIO register base address
 * @return 			void
 */
void fGpioHandler(void)
{
  uint8_t index;
  uint32_t active_interrupts = 0;
  gpio_irq_event event;
  GpioReg_pt gpioBase;

  /* Enable the GPIO clock */
  CLOCK_ENABLE(CLOCK_GPIO);

  gpioBase = GPIOREG;

  /** - Store all active interrupts */
  active_interrupts = gpioBase->R_IRQ_W_CLEAR;

  for (index=0; index < NUMBER_OF_GPIO; index++)
  {

    /* Check the pin for which IRQ is raised */
    if ((active_interrupts >> index) & 0x01)
    {
      /* Check if it is edge triggered and clear the interrupt */
      if ((gpioBase->IRQ_EDGE >> index) &  0x01){
        if ((gpioBase->IRQ_POLARITY_SET >> index) &0x01)  {
          /* Edge triggered high */
          event = IRQ_RISE;
        } else if ((gpioBase->IRQ_POLARITY_CLEAR >> index) &0x01) {
          /* Edge triggered low */
          event = IRQ_FALL;
        } else {
          /* Edge none */
          event = IRQ_NONE;
        }
      }
      gpioBase->IRQ_CLEAR |= (0x1 << index);

      /* Call the handler registered to the pin */
      irq_handler(gpioIds[index], event);
    }

  }
}

/** Initialize the GPIO IRQ pin
 *
 * @param obj     The GPIO object to initialize
 * @param pin     The GPIO pin name
 * @param handler The handler to be attached to GPIO IRQ
 * @param id      The object ID
 * @return -1 if pin is NC, 0 otherwise
 */
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
  /* If Pin is not connected; then return -1 */
  if (pin == NC)
  {
    return(-1);
  }

  /* Store the pin for which handler is registered */
  obj->pin = pin;
  obj->pinMask = (0x1 << pin);
  
  /* Store the ID, this is required by registered handler function */
  gpioIds[pin] = id;

  /* Enable the GPIO clock */
  CLOCK_ENABLE(CLOCK_GPIO);

  /* Initialize the GPIO membase */
  obj->GPIOMEMBASE = GPIOREG;

  /* Set default values for the pin interrupt */
  /* TODO: Only one DIO line is configured using this function; overrides other DIO line setting
   * If mbed layer wants to call this function repeatedly for setting multiple DIO lines as input
   * then change this setting to  obj->GPIOMEMBASE->W_IN |= obj->pinMask. All parameter setting needs to change from = to |=
   */
  obj->GPIOMEMBASE->W_IN = obj->pinMask;
  obj->GPIOMEMBASE->IRQ_ENABLE_SET = obj->pinMask;
  obj->GPIOMEMBASE->IRQ_EDGE = obj->pinMask;
  obj->GPIOMEMBASE->IRQ_POLARITY_SET = (obj->pinMask);
  obj->GPIOMEMBASE->ANYEDGE_SET = IO_NONE;

  /* Register the handler for this pin */
  irq_handler = handler;

  /* Enable interruption associated with the gpio */
  NVIC_ClearPendingIRQ(Gpio_IRQn);
  NVIC_EnableIRQ(Gpio_IRQn);

  return(0);
}

/** Release the GPIO IRQ PIN
 *
 * @param obj The gpio object
 */
void gpio_irq_free(gpio_irq_t *obj)
{
	/* Enable the GPIO clock */
	CLOCK_ENABLE(CLOCK_GPIO);

	obj->GPIOMEMBASE->W_IN = (IO_ALL ^ (obj->pinMask));
	gpioIds[obj->pin] = 0;
}

/** Enable/disable pin IRQ event
 *
 * @param obj    The GPIO object
 * @param event  The GPIO IRQ event
 * @param enable The enable flag
 */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{

	/* Enable the GPIO clock */
	CLOCK_ENABLE(CLOCK_GPIO);

	switch(event)
	{
	  case IRQ_RISE:
		obj->GPIOMEMBASE->IRQ_EDGE = (obj->pinMask);
		obj->GPIOMEMBASE->IRQ_LEVEL = (IO_ALL ^ (obj->pinMask));
		/* Enable is an integer; hence checking for 1 or 0*/
		if (enable == 1)
		{
		  /* Enable rising edge */
		  obj->GPIOMEMBASE->IRQ_POLARITY_SET = (obj->pinMask);
		}
		else if (enable == 0)
		{
		  /* Disable rising edge */
		  obj->GPIOMEMBASE->IRQ_POLARITY_SET = (IO_ALL ^ (obj->pinMask));
		}
		break;

	  case IRQ_FALL:
		obj->GPIOMEMBASE->IRQ_EDGE = (obj->pinMask);
		obj->GPIOMEMBASE->IRQ_LEVEL = (IO_ALL ^ (obj->pinMask));
		/* Enable is an integer; hence checking for 1 or 0*/
		if (enable == 1)
		{
		  /* Enable falling edge */
		  obj->GPIOMEMBASE->IRQ_POLARITY_CLEAR = (obj->pinMask);
		}
		else if (enable == 0)
		{
		  /* Disable falling edge */
		  obj->GPIOMEMBASE->IRQ_POLARITY_CLEAR = (IO_ALL ^ (obj->pinMask));
		}
		break;

	  default:
		/* No event is set */
		break;
	}

}

/** Enable GPIO IRQ
 *
 * This is target dependent, as it might enable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_enable(gpio_irq_t *obj)
{
	/* Enable the GPIO clock */
	CLOCK_ENABLE(CLOCK_GPIO);

	obj->GPIOMEMBASE->IRQ_ENABLE_SET = (obj->pinMask);
}

/** Disable GPIO IRQ
 *
 * This is target dependent, as it might disable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_disable(gpio_irq_t *obj)
{
	/* Enable the GPIO clock */
	CLOCK_ENABLE(CLOCK_GPIO);

	obj->GPIOMEMBASE->IRQ_ENABLE_CLEAR = (obj->pinMask);
}

#endif //DEVICE_INTERRUPTIN
