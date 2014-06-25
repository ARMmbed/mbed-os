/***************************************************************************//**
 * @file gpiointerrupt.c
 * @brief GPIOINT API implementation
 * @version 3.20.5
 *
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "em_gpio.h"
#include "em_int.h"
#include "gpiointerrupt.h"
#include "em_assert.h"

/*******************************************************************************
 ********************************   MACROS   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Macro return index of the LSB flag which is set. */
#if ((__CORTEX_M == 3) || (__CORTEX_M == 4))
#define GPIOINT_MASK2IDX(mask) (__CLZ(__RBIT(mask)))
#elif __CORTEX_M == 0
#define GPIOINT_MASK2IDX(mask) (countTrailingZeros(mask))
__STATIC_INLINE uint32_t countTrailingZeros(uint32_t mask)
{
  uint32_t zeros;
  for(zeros=0; (zeros<32) && (0 == (mask&0x1)); zeros++, mask>>=1);
  return zeros;
}
#else
#error Unsupported architecture.
#endif

/** @endcond */

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

typedef struct
{
  /* Pin number in range of 0 to 15 */
  uint32_t pin;

  /* Pointer to the callback function */
  GPIOINT_IrqCallbackPtr_t callback;

} GPIOINT_CallbackDesc_t;


/*******************************************************************************
 ********************************   GLOBALS   **********************************
 ******************************************************************************/

/* Array of user callbacks. One for each pin. */
static GPIOINT_IrqCallbackPtr_t gpioCallbacks[16] = {0};

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags);

/** @endcond */

/*******************************************************************************
 ***************************   GLOBAL FUNCTIONS   ******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Initialization of GPIOINT module.
 *
 ******************************************************************************/
void GPIOINT_Init(void)
{
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}


/***************************************************************************//**
 * @brief
 *   Registers user callback for given pin number.
 *
 * @details
 *   Use this function to register a callback which shall be called upon
 *   interrupt generated from given pin number (port is irrelevant). Interrupt
 *   itself must be configured externally. Function overwrites previously
 *   registered callback.
 *
 * @param[in] pin
 *   Pin number for the callback.
 * @param[in] callbackPtr
 *   A pointer to callback function.
 ******************************************************************************/
void GPIOINT_CallbackRegister(uint8_t pin, GPIOINT_IrqCallbackPtr_t callbackPtr)
{
  INT_Disable();

  /* Dispatcher is used */
  gpioCallbacks[pin] = callbackPtr;

  INT_Enable();
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Function calls users callback for registered pin interrupts.
 *
 * @details
 *   This function is called when GPIO interrupts are handled by the dispatcher.
 *   Function gets even or odd interrupt flags and calls user callback
 *   registered for that pin. Function iterates on flags starting from MSB.
 *
 * @param iflags
 *  Interrupt flags which shall be handled by the dispatcher.
 *
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags)
{
  uint32_t irqIdx;

  /* check for all flags set in IF register */
  while(iflags)
  {
    irqIdx = GPIOINT_MASK2IDX(iflags);

    /* clear flag*/
    iflags &= ~(1 << irqIdx);

    if (gpioCallbacks[irqIdx])
    {
      /* call user callback */
      gpioCallbacks[irqIdx](irqIdx);
    }
  }
}

/***************************************************************************//**
 * @brief
 *   GPIO EVEN interrupt handler. Interrupt handler clears all IF even flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  uint32_t iflags;

  /* Get all even interrupts. */
  iflags = GPIO_IntGetEnabled() & 0x00005555;

  /* Clean only even interrupts. */
  GPIO_IntClear(iflags);

  GPIOINT_IRQDispatcher(iflags);
}


/***************************************************************************//**
 * @brief
 *   GPIO ODD interrupt handler. Interrupt handler clears all IF odd flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  uint32_t iflags;

  /* Get all odd interrupts. */
  iflags = GPIO_IntGetEnabled() & 0x0000AAAA;

  /* Clean only even interrupts. */
  GPIO_IntClear(iflags);

  GPIOINT_IRQDispatcher(iflags);
}

/** @endcond */

/** @} (end addtogroup GPIOINT */
/** @} (end addtogroup EM_Drivers) */


/******** THE REST OF THE FILE IS DOCUMENTATION ONLY !**********************//**
 * @{

@page gpioint_doc GPIOINT General Purpose Input/Output Interrupt dispatcher

  The source files for the GPIO interrupt dispatcher library resides in the
  emdrv/gpiointerrupt folder, and are named gpiointerrupt.c and gpiointerrupt.h.

  @li @ref gpioint_intro
  @li @ref gpioint_api
  @li @ref gpioint_example

@n @section gpioint_intro Introduction
 * EFM32 has two GPIO interrupts lines, Odd and Even. If more
 * than two interrupts are used then interrupt routine must dispatch from a callback
 * register. This module provides small dispatcher for both GPIO interrupts enabling
 * handling of up to 16 GPIO pin interrupts.
 *
 * It is up to the user to configure and enable interrupt on given pin. This can be done 
 * using the GPIO library (emlib). This module handles the dispatch register and clearing of
 * interrupt flags.
 *
 * In order to use this dispatcher, it has to be initialized first by
 * calling GPIOINT_Init(). Then each pin must be configured by first registering
 * the callback function for given pin and then configure and enabling the interrupt in GPIO module.

@n @section gpioint_api The API
  This section contain brief descriptions of the functions in the API. You will
  find detailed information on parameters by clicking on the hyperlinked function names. 

  Your application code must include one header file: @em gpiointerrupt.h.

  @ref GPIOINT_Init() @n
    This functions initializes the dispatcher register. Typically
    @htmlonly GPIOINT_Init() @endhtmlonly is called once in your startup code.

  @ref GPIOINT_CallbackRegister() @n
    Register a callback function on a pin number.

  @ref GPIOINT_CallbackUnRegister() @n
    Un-register a callback function on a pin number.

@n @section gpioint_example Example
  @verbatim

#include "em_gpio.h"
#include "em_int.h"
#include "gpiointerrupt.h"

int main(void)
{
  CHIP_Init();

  // Enable clock for GPIO module, initialize GPIOINT
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIOINT_Init();

  // Register callback functions and enable interrupts
  GPIOINT_CallbackRegister(1, gpioCallback1);
  GPIOINT_CallbackRegister(3, gpioCallback3);
  GPIOINT_CallbackRegister(8, gpioCallback8);
  GPIO_IntEnable(1<<1 | 1<<3 | 1<<8);

  while(true);
}

  @endverbatim
    
 * @}**************************************************************************/
