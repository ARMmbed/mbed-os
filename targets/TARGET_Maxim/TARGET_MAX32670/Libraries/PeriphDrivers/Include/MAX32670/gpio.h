/**
 * @file    gpio.h
 * @brief   General-Purpose Input/Output (GPIO) function prototypes and data types.
 */

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _GPIO_H_
#define _GPIO_H_

/* **** Includes **** */
#include "gpio_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup gpio General-Purpose Input/Output (GPIO)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */
/**
 * @defgroup gpio_port_pin Port and Pin Definitions
 * @ingroup gpio
 * @{
 * @defgroup gpio_port Port Definitions
 * @ingroup gpio_port_pin
 * @{
 */
#define MXC_GPIO_PORT_0      ((uint32_t)(1UL << 0))        ///< Port 0  Define
/**@} end of gpio_port group*/
/**
 * @defgroup gpio_pin Pin Definitions
 * @ingroup gpio_port_pin
 * @{
 */
#define MXC_GPIO_PIN_0       ((uint32_t)(1UL << 0))       ///< Pin 0 Define 
#define MXC_GPIO_PIN_1       ((uint32_t)(1UL << 1))       ///< Pin 1 Define 
#define MXC_GPIO_PIN_2       ((uint32_t)(1UL << 2))       ///< Pin 2 Define 
#define MXC_GPIO_PIN_3       ((uint32_t)(1UL << 3))       ///< Pin 3 Define 
#define MXC_GPIO_PIN_4       ((uint32_t)(1UL << 4))       ///< Pin 4 Define 
#define MXC_GPIO_PIN_5       ((uint32_t)(1UL << 5))       ///< Pin 5 Define 
#define MXC_GPIO_PIN_6       ((uint32_t)(1UL << 6))       ///< Pin 6 Define 
#define MXC_GPIO_PIN_7       ((uint32_t)(1UL << 7))       ///< Pin 7 Define 
#define MXC_GPIO_PIN_8       ((uint32_t)(1UL << 8))       ///< Pin 8 Define 
#define MXC_GPIO_PIN_9       ((uint32_t)(1UL << 9))       ///< Pin 9 Define 
#define MXC_GPIO_PIN_10      ((uint32_t)(1UL << 10))      ///< Pin 10 Define 
#define MXC_GPIO_PIN_11      ((uint32_t)(1UL << 11))      ///< Pin 11 Define 
#define MXC_GPIO_PIN_12      ((uint32_t)(1UL << 12))      ///< Pin 12 Define 
#define MXC_GPIO_PIN_13      ((uint32_t)(1UL << 13))      ///< Pin 13 Define
/**@} end of gpio_pin group */
/**@} end of gpio_port_pin group */

/**
 * @brief      Type alias for a GPIO callback function with prototype:
 * @code
    void callback_fn(void *cbdata);
 * @endcode
 * @param      cbdata  A void pointer to the data type as registered when
 *                     MXC_GPIO_RegisterCallback() was called.
 */
typedef void (*mxc_gpio_callback_fn) (void *cbdata);

/**
 * @brief   Enumeration type for the GPIO Function Type
 */
typedef enum {
    MXC_GPIO_FUNC_IN,       ///< GPIO Input 
    MXC_GPIO_FUNC_OUT,      ///< GPIO Output 
    MXC_GPIO_FUNC_ALT1,     ///< Alternate Function Selection 
    MXC_GPIO_FUNC_ALT2,     ///< Alternate Function Selection 
    MXC_GPIO_FUNC_ALT3,     ///< Alternate Function Selection 
    MXC_GPIO_FUNC_ALT4,     ///< Alternate Function Selection 
} mxc_gpio_func_t;

/**
 * @brief   Enumeration type for the voltage level on a given pin.
 */
typedef enum {
    MXC_GPIO_VSSEL_VDDIO,        ///< Set pin to VIDDIO voltage 
    MXC_GPIO_VSSEL_VDDIOH,       ///< Set pin to VIDDIOH voltage 
} mxc_gpio_vssel_t;

/**
 * @brief   Enumeration type for the type of GPIO pad on a given pin.
 */
typedef enum {
    MXC_GPIO_PAD_NONE,          ///< No pull-up or pull-down 
    MXC_GPIO_PAD_PULL_UP,       ///< Set pad to weak pull-up 
    MXC_GPIO_PAD_PULL_DOWN,     ///< Set pad to weak pull-down 
} mxc_gpio_pad_t;

/**
 * @brief   Structure type for configuring a GPIO port.
 */
typedef struct {
    mxc_gpio_regs_t* port;      ///< Pointer to GPIO regs 
    uint32_t mask;              ///< Pin mask (multiple pins may be set) 
    mxc_gpio_func_t func;       ///< Function type 
    mxc_gpio_pad_t pad;         ///< Pad type 
    mxc_gpio_vssel_t vssel;     ///< Voltage select 
} mxc_gpio_cfg_t;

/**
 * @brief   Enumeration type for the interrupt modes.
 */
typedef enum {
    MXC_GPIO_INT_LEVEL,   ///< Interrupt is level sensitive 
    MXC_GPIO_INT_EDGE     ///< Interrupt is edge sensitive 
} mxc_gpio_int_mode_t;

/**
 * @brief   Enumeration type for the interrupt polarity.
 */
typedef enum {
    MXC_GPIO_INT_FALLING,                 ///< Interrupt triggers on falling edge 
    MXC_GPIO_INT_HIGH,                    ///< Interrupt triggers when level is high 
    MXC_GPIO_INT_RISING,                  ///< Interrupt triggers on rising edge 
    MXC_GPIO_INT_LOW,                     ///< Interrupt triggers when level is low 
    MXC_GPIO_INT_BOTH                     ///< Interrupt triggers on either edge 
} mxc_gpio_int_pol_t;

/* **** Function Prototypes **** */

/**
 * @brief      Initialize GPIO.
 * @param      portMask     Mask for the port to be initialized
 * @return     #E_NO_ERROR if everything is successful. See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_GPIO_Init (uint32_t portMask);

/**
 * @brief      Shutdown GPIO.
 * @param      portMask     Mask for the port to be initialized
 * @return     #E_NO_ERROR if everything is successful. See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_GPIO_Shutdown (uint32_t portMask);

/**
 * @brief      Reset GPIO.
 * @param      portMask     Mask for the port to be initialized
 * @return     #E_NO_ERROR if everything is successful. See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_GPIO_Reset (uint32_t portMask);

/**
 * @brief      Configure GPIO pin(s).
 * @param      cfg   Pointer to configuration structure describing the pin.
 * @return     #E_NO_ERROR if everything is successful. See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_GPIO_Config (const mxc_gpio_cfg_t *cfg);

/**
 * @brief      Gets the pin(s) input state.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to read
 * @return     The requested pin state.
 */
uint32_t MXC_GPIO_InGet (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Sets the pin(s) to a high level output.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to set
 */
void MXC_GPIO_OutSet (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Clears the pin(s) to a low level output.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to clear
 */
void MXC_GPIO_OutClr (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Gets the pin(s) output state.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to read the output state of
 * @return     The state of the requested pin.
 *
 */
uint32_t MXC_GPIO_OutGet (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Write the pin(s) to a desired output level.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to set output level of
 * @param      val   Desired output level of the pin(s). This will be masked
 *                   with the configuration mask.
 */
void MXC_GPIO_OutPut (mxc_gpio_regs_t* port, uint32_t mask, uint32_t val);

/**
 * @brief      Toggles the the pin(s) output level.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to toggle
 */
void MXC_GPIO_OutToggle (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Configure GPIO interrupt(s)
 * @param      cfg   Pointer to configuration structure describing the pin.
 * @param      pol   Requested interrupt polarity.
 * @return     #E_NO_ERROR if everything is successful. See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_GPIO_IntConfig (const mxc_gpio_cfg_t *cfg, mxc_gpio_int_pol_t pol);

/**
 * @brief      Enables the specified GPIO interrupt
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to enable interrupts for
 * 
 */
void MXC_GPIO_EnableInt (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Disables the specified GPIO interrupt.
 * @param      port  Pointer to the GPIO port registers
 * @param      mask  Mask of the pin(s) to disable interrupts for
 */
void MXC_GPIO_DisableInt (mxc_gpio_regs_t* port, uint32_t mask);

/**
 * @brief      Gets the interrupt(s) status on a GPIO port
 *
 * @param      port   Pointer to the port requested
 *
 * @return     The requested interrupt status.
 */
uint32_t MXC_GPIO_GetFlags (mxc_gpio_regs_t* port);

/**
 * @brief      Gets the interrupt(s) status on a GPIO port
 *
 * @param      port   Pointer to the port requested
 * @param      flags  The flags to clear
 */
void MXC_GPIO_ClearFlags (mxc_gpio_regs_t* port, uint32_t flags);

/**
 * @brief      Registers a callback for the interrupt on a given port and pin.
 * @param      cfg       Pointer to configuration structure describing the pin
 * @param      callback  A pointer to a function of type #callback_fn.
 * @param      cbdata    The parameter to be passed to the callback function, #callback_fn, when an interrupt occurs.
 *
 */
void MXC_GPIO_RegisterCallback (const mxc_gpio_cfg_t *cfg, mxc_gpio_callback_fn callback, void *cbdata);

/**
 * @brief      GPIO IRQ Handler. @note If a callback is registered for a given
 *             interrupt, the callback function will be called.
 *
 * @param      port Number of the port that generated the interrupt service routine.
 *
 */
void MXC_GPIO_Handler (unsigned int port);

/**
 * @brief      Set Voltage select for pins to VDDIO or VDDIOH
 *
 * @param      port   The GPIO port
 * @param[in]  vssel  VDDIO or VDDIOH to set the voltatge to
 * @param[in]  mask   Pins in the GPIO port that will be set to the voltage.
 * 
 * @return     #E_NO_ERROR if everything is successful. See \ref MXC_Error_Codes for the list of error codes.
 */
int MXC_GPIO_SetVSSEL (mxc_gpio_regs_t* port, mxc_gpio_vssel_t vssel, uint32_t mask);

/**@} end of group gpio */

#ifdef __cplusplus
}
#endif

#endif /* _GPIO_H_ */
