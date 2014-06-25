/***************************************************************************//**
 * @file gpiointerrupt.h
 * @brief GPIOINT API definition
 * @version 3.20.5
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

#ifndef __EMDRV_GPIOINTERRUPT_H
#define __EMDRV_GPIOINTERRUPT_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup EM_Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup GPIOINT
 * @brief GPIOINT General Purpose Input/Output Interrupt dispatcher, see
 *        @ref gpioint_doc page for detailed
 *        documentation.
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

/**
 * @brief
 *  GPIO interrupt callback function pointer.
 * @details
 *   Parameters:
 *   @li pin - The pin index the callback function is invoked for.
 */
typedef void (*GPIOINT_IrqCallbackPtr_t)(uint8_t pin);

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/
void GPIOINT_Init(void);
void GPIOINT_CallbackRegister(uint8_t pin, GPIOINT_IrqCallbackPtr_t callbackPtr);
static __INLINE void GPIOINT_CallbackUnRegister(uint8_t pin);

/***************************************************************************//**
 * @brief
 *   Unregisters user callback for given pin number.
 *
 * @details
 *   Use this function to unregister a callback.
 *
 * @param[in] pin
 *   Pin number for the callback.
 *
 ******************************************************************************/
static __INLINE void GPIOINT_CallbackUnRegister(uint8_t pin)
{
  GPIOINT_CallbackRegister(pin,0);
}

/** @} (end addtogroup GPIOINT */
/** @} (end addtogroup EM_Drivers) */
#ifdef __cplusplus
}
#endif

#endif /* __EMDRV_GPIOINTERRUPT_H */
