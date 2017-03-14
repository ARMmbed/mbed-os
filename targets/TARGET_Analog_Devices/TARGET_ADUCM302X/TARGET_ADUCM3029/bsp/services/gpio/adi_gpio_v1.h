/*!
 *****************************************************************************
   @file:    adi_gpio_v1.h
   @brief:   GPIO definitions and API for ADuC302X
   @version: $Revision$
   @date:    $Date$
  -----------------------------------------------------------------------------

Copyright (c) 2014 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/
#ifndef __ADI_GPIO_V1_H__
#define __ADI_GPIO_V1_H__

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
 *
 * Pm008 (rule 2.4): sections of code should not be 'commented out'.
 *   Allow code example in doxygen comment.
 */
#pragma diag_suppress=Pm008
#endif /* __ICCARM__ */

/*! \addtogroup GPIO_Service GPIO Service
 *  GPIO service
 *  @{

  @brief    GPIO port and pin identifiers
  @details  These macros can be passed to the following functions:

                  - adi_gpio_OutputEnable()
                  - adi_gpio_GetOutputEnable()
                  - adi_gpio_PullUpEnable()
                  - adi_gpio_GetPullUpEnable()
                  - adi_gpio_SetHigh()
                  - adi_gpio_SetLow()
                  - adi_gpio_Toggle()
                  - adi_gpio_SetData()
                  - adi_gpio_GetData()

            To control a single GPIO, these macros can be passed to the functions one
            at a time. For example, to set the GPIO on port 2, pin 4 to a logical high
            level, the following is used:

                 adi_gpio_SetHigh(ADI_GPIO_PORT2, ADI_GPIO_PIN_4);

            Multiple GPIOs, so long as they reside on the same port, can be controlled
            simultaneously. These macros can be OR-ed together and passed to the
            functions. For example, to set the GPIOs on port 2, pins 3, 4 and 7 to
            a logical low level, the following is used:

                 adi_gpio_SetLow(ADI_GPIO_PORT2, ADI_GPIO_PIN_3 | ADI_GPIO_PIN_4 | ADI_GPIO_PIN_7);

            For the sensing, or adi_gpio_Getxxx, functions, the passed pValue parameter is written with
            a packed value containing the status of the requested GPIO pins on the given port.

            If information is required for a single pin, return value can be direcly used
            For example to see if pin 4 on port 2 has the pull up enabled, the following is used:
                 adi_gpio_GetPullUpEnable(ADI_GPIO_PORT2, ADI_GPIO_PIN_4, &pValue)
            pValue will contain the required information.

            If information is required for multiple pins, following method is required:
                adi_gpio_GetPullUpEnable(ADI_GPIO_PORT2, (ADI_GPIO_PIN_3 | ADI_GPIO_PIN_4 | ADI_GPIO_PIN_7), &pValue)
            To test if pin 4 on port 2 has pull up enabled, the following is used:
                if   (pValue & ADU_GPIO_PIN_4) {
                    the pull up is enabled for pin 4 on port 2
                } else {
                    the pull up is disabled for pin 4 on port 2
                }
 */

#ifdef __ICCARM__
#pragma diag_default=Pm008
#endif /* __ICCARM__ */

#include <adi_types.h>
#include <services/int/adi_int.h>

#if !defined(__ADUCM30xx__)
#error "Unknown processor family"
#endif

/* C++ linkage */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! Amount of memory(In bytes) required by the Gpio device driver for managing the operation.
 * This memory is completely owned by the driver till the end of the operation.
 */
#define ADI_GPIO_MEMORY_SIZE        (52u)

/* typedefs for 16-bit Ports */
typedef uint16_t ADI_GPIO_DATA;    /*!< pin data reg type */


/*! \enum ADI_GPIO_RESULT GPIO API function return codes */
typedef enum
{
    ADI_GPIO_SUCCESS = 0,                                       /*!< No error detected. */
    ADI_GPIO_FAILURE,                                           /*!< The API call failed. */
    ADI_GPIO_ALREADY_INITIALIZED,                               /*!< GPIO device has already been initialized. */
    ADI_GPIO_NOT_INITIALIZED,                                   /*!< GPIO device has not yet been initialized. */
    ADI_GPIO_INVALID_PORT,                                      /*!< Invalid port number. */
    ADI_GPIO_INVALID_PINS,                                      /*!< Invalid pin combination. */
    ADI_GPIO_INVALID_INTERRUPT,                                 /*!< Invalid interrupt number. */
    ADI_GPIO_INVALID_TRIGGER,                                   /*!< Invalid trigger condition. */
} ADI_GPIO_RESULT;


/*! \enum ADI_GPIO_IRQ_TRIGGER_CONDITION GPIO trigger condition enumerations */
typedef enum {
    ADI_GPIO_IRQ_RISING_EDGE    =(0x0),     /*!< Trigger an interrupt on a rising edge.    */
    ADI_GPIO_IRQ_FALLING_EDGE   =(0x1),     /*!< Trigger an interrupt on a falling edge.   */
    ADI_GPIO_IRQ_EITHER_EDGE    =(0x2),     /*!< Trigger an interrupt on either edge.      */
    ADI_GPIO_IRQ_HIGH_LEVEL     =(0x3),     /*!< Trigger an interrupt on a high level.     */
    ADI_GPIO_IRQ_LOW_LEVEL      =(0x4)      /*!< Trigger an interrupt on a low level.      */
} ADI_GPIO_IRQ_TRIGGER_CONDITION;

/*! \enum ADI_GPIO_EVENT GPIO event. Used only with the event associated with external event. */
typedef enum {
    ADI_GPIO_EVENT_EXT_INT0       = (XINT_EVT0_IRQn),     /*!< Event for external interrupt-0   */
    ADI_GPIO_EVENT_EXT_INT1       = (XINT_EVT1_IRQn),      /*!< Event for external interrupt-1   */
    ADI_GPIO_EVENT_EXT_INT2       = (XINT_EVT2_IRQn),     /*!< Event for external interrupt-2   */
    ADI_GPIO_EVENT_EXT_INT3       = (XINT_EVT3_IRQn),     /*!< Event for external interrupt-3   */
    ADI_GPIO_EVENT_UART_ACTIVITY  = ((uint32_t)XINT_EVT3_IRQn | (1LU<< BITP_XINT_EXT_STAT_STAT_UART_RXWKUP))     /*!< Event for  UART activity   */
} ADI_GPIO_EVENT;

/*! \enum ADI_GPIO_INTERRUPT_SOURCE. To indicate device which should act as interrupt source */
      
typedef enum {
    ADI_GPIO_INTERRUPT_DEV_UART0   /*!< UART act as interrupt source when there is activity on UART0-RX  */
} ADI_GPIO_INTERRUPT_SOURCE;


/*! \enum ADI_GPIO_PORT GPIO port enumerations */
typedef enum {
    ADI_GPIO_PORT0,      /*!< Port 0 */
    ADI_GPIO_PORT1,      /*!< Port 1 */
    ADI_GPIO_PORT2,      /*!< Port 2 */
    ADI_GPIO_NUM_PORTS   /*!< maximum number of ports */
} ADI_GPIO_PORT;

/* 16-bit port pin defs */
#define ADI_GPIO_PIN_0      ((ADI_GPIO_DATA)(0x0001)) /*!< Pin 0 */
#define ADI_GPIO_PIN_1      ((ADI_GPIO_DATA)(0x0002)) /*!< Pin 1 */
#define ADI_GPIO_PIN_2      ((ADI_GPIO_DATA)(0x0004)) /*!< Pin 2 */
#define ADI_GPIO_PIN_3      ((ADI_GPIO_DATA)(0x0008)) /*!< Pin 3 */
#define ADI_GPIO_PIN_4      ((ADI_GPIO_DATA)(0x0010)) /*!< Pin 4 */
#define ADI_GPIO_PIN_5      ((ADI_GPIO_DATA)(0x0020)) /*!< Pin 5 */
#define ADI_GPIO_PIN_6      ((ADI_GPIO_DATA)(0x0040)) /*!< Pin 6 */
#define ADI_GPIO_PIN_7      ((ADI_GPIO_DATA)(0x0080)) /*!< Pin 7 */
#define ADI_GPIO_PIN_8      ((ADI_GPIO_DATA)(0x0100)) /*!< Pin 8 */
#define ADI_GPIO_PIN_9      ((ADI_GPIO_DATA)(0x0200)) /*!< Pin 9 */
#define ADI_GPIO_PIN_10     ((ADI_GPIO_DATA)(0x0400)) /*!< Pin 10 */
#define ADI_GPIO_PIN_11     ((ADI_GPIO_DATA)(0x0800)) /*!< Pin 11 */
#define ADI_GPIO_PIN_12     ((ADI_GPIO_DATA)(0x1000)) /*!< Pin 12 */
#define ADI_GPIO_PIN_13     ((ADI_GPIO_DATA)(0x2000)) /*!< Pin 13 */
#define ADI_GPIO_PIN_14     ((ADI_GPIO_DATA)(0x4000)) /*!< Pin 14 */
#define ADI_GPIO_PIN_15     ((ADI_GPIO_DATA)(0x8000)) /*!< Pin 15 */

#define ADI_GPIO_PORT0_PIN_AVL    (0xFFFFu)           /*!< Port 0 pin mask */
#define ADI_GPIO_PORT1_PIN_AVL    (0xFFFFu)           /*!< Port 1 pin mask */
#define ADI_GPIO_PORT2_PIN_AVL    (0x0FFFu)           /*!< Port 2 pin mask */


/* GPIO API functions */
ADI_GPIO_RESULT  adi_gpio_Init                      (void* const pMemory, uint32_t const MemorySize);                                                          /*!< Initializes the GPIO device functions */

ADI_GPIO_RESULT  adi_gpio_UnInit                    (void);                                                                                                    /*!< Uninitializes the muxing and GPIOs */

ADI_GPIO_RESULT  adi_gpio_ResetToPowerUp            (void);                                                                                                    /*!< Resets GPIO registers to power up defaults */

ADI_GPIO_RESULT  adi_gpio_EnableExIRQ               (const IRQn_Type eIrq, const ADI_GPIO_IRQ_TRIGGER_CONDITION eCondition);                                   /*!< Enables an external GPIO interrupt */

ADI_GPIO_RESULT  adi_gpio_DisableExIRQ              (const IRQn_Type eIrq);                                                                                    /*!< Disables an external GPIO interrupt */

ADI_GPIO_RESULT  adi_gpio_RegisterCallback          (const IRQn_Type eIrq, ADI_CALLBACK const pfCallback, void *const pCBParam );                              /*!< Registers a callback for external interrupt */

ADI_GPIO_RESULT  adi_gpio_SetGroupInterruptPins     (const ADI_GPIO_PORT Port, const IRQn_Type eIrq, const ADI_GPIO_DATA Pins);                                /*!< Set pin interrupt group A/B register */

ADI_GPIO_RESULT  adi_gpio_GetGroupInterruptPins     (const ADI_GPIO_PORT Port, const IRQn_Type eIrq, const ADI_GPIO_DATA Pins, uint16_t* const pValue);        /*!< Get pin interrupt group A/B register */

ADI_GPIO_RESULT  adi_gpio_SetGroupInterruptPolarity (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);                                                      /*!< Set pin interrupt polarity */

ADI_GPIO_RESULT  adi_gpio_GetGroupInterruptPolarity (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Get pin interrupt polarity */

/* TODO: Will not be required */
ADI_GPIO_RESULT  adi_gpio_GetGroupInterruptStatus   (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Get a port's interrupt status bits*/

/* TODO: Will not be required */
ADI_GPIO_RESULT  adi_gpio_ClrGroupInterruptStatus   (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);                                                      /*!< Clear a port's interrupt status bits */

ADI_GPIO_RESULT  adi_gpio_OutputEnable              (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool_t bFlag);                                  /*!< Enables/disables the output driver for GPIO output pins */

ADI_GPIO_RESULT  adi_gpio_InputEnable               (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool_t bFlag);                                  /*!< Enables/disables the output driver for GPIO output pins */

ADI_GPIO_RESULT  adi_gpio_PullUpEnable              (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool_t bFlag);                                  /*!< Enables/disables the pullup for GPIO output pins */

ADI_GPIO_RESULT  adi_gpio_GetOutputEnable           (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Gets the output enable status for a port */

ADI_GPIO_RESULT  adi_gpio_GetInputEnable            (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Gets the input enable status for a port */

ADI_GPIO_RESULT  adi_gpio_GetPullUpEnable           (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Gets the pull up enable status for a port */

ADI_GPIO_RESULT  adi_gpio_SetHigh                   (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);                                                      /*!< Sets GPIO pins to a logical high level */

ADI_GPIO_RESULT  adi_gpio_SetLow                    (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);                                                      /*!< Sets GPIO pins to a logical low level */

ADI_GPIO_RESULT  adi_gpio_Toggle                    (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);                                                      /*!< Toggles the level of GPIO pins */

ADI_GPIO_RESULT  adi_gpio_SetData                   (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);                                                      /*!< Sets the absolute value of GPIOs */

ADI_GPIO_RESULT  adi_gpio_GetData                   (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Gets/Senses the Input Level of all GPIO Pins on the Given Port */

ADI_GPIO_RESULT  adi_gpio_GetOutputData             (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue);                              /*!< Gets the Output Level of all GPIO Pins on the Given Port */

ADI_GPIO_RESULT  adi_gpio_EnableDeviceInterrupt      (ADI_GPIO_INTERRUPT_SOURCE eDevice,ADI_GPIO_IRQ_TRIGGER_CONDITION eCondition);   

ADI_GPIO_RESULT  adi_gpio_DisableDeviceInterrupt    (ADI_GPIO_INTERRUPT_SOURCE eDevice);   

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __ADI_GPIO_V1_H__ */
